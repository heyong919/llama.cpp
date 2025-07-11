#include "common.h"
#include "ggml-mlu.h"
#include "ggml-impl.h"
#include "ggml-backend-impl.h"
#include "mlu_op.h"
#include "mlu_include/cndev.h"
#include "mlu_include/cnrt.h"
#include "mlu_include/cnnl.h"
#include "debug_print.h"
#include <map>
#include <array>
#include <memory>

[[noreturn]]
void ggml_mlu_error(const char * stmt, const char * func, const char * file, int line, const char * msg) {
    int id = -1; // 如果 cnrtGetDevice 失败则使用默认值
    (void)cnrtGetDevice(&id);

    GGML_LOG_ERROR(GGML_MLU_NAME " error: %s\n", msg);
    GGML_LOG_ERROR("  current device: %d, in function %s at %s:%d\n", id, func, file, line);
    GGML_LOG_ERROR("  %s\n", stmt);
    // 使用 GGML_ABORT 中止程序以获取堆栈跟踪
    GGML_ABORT(GGML_MLU_NAME " error");
}

/**
 * @brief 设置当前使用的 MLU 设备
 *
 * @param device 设备 ID
 */
void ggml_mlu_set_device(int device) {
    int current_device;
    CNRT_CHECK(cnrtGetDevice(&current_device));

    if (device == current_device) {
        return;
    }

    CNRT_CHECK(cnrtSetDevice(device));
}

/**
 * @brief 获取当前使用的 MLU 设备 ID
 *
 * @return 当前设备 ID
 */
int ggml_mlu_get_device() {
    int id;
    CNRT_CHECK(cnrtGetDevice(&id));
    return id;
}

/**
 * @brief 在指定设备上分配内存
 *
 * @param ptr 指向分配内存的指针的指针
 * @param size 要分配的内存大小
 * @param device 设备 ID
 * @return cnrtRet_t 操作结果
 */
static cnrtRet_t ggml_mlu_device_malloc(void ** ptr, size_t size, int device) {
    ggml_mlu_set_device(device);
    return cnrtMalloc(ptr, size);
}

/**
 * @brief MLU 设备信息结构体
 */
struct ggml_mlu_device_info {
    int device_count = 0;
    struct device {
        size_t total_vram = 0;
    } devices[GGML_MLU_MAX_DEVICES];
};

/**
 * @brief 初始化 MLU 设备信息
 *
 * @return ggml_mlu_device_info 设备信息结构体
 */
static ggml_mlu_device_info ggml_mlu_init() {
    ggml_mlu_device_info info = {};

    CNDEV_CHECK(cndevInit(0));
    cnrtRet_t err = cnrtGetDeviceCount((unsigned int*)&info.device_count);

    if (err != CNRT_RET_SUCCESS) {
        GGML_LOG_ERROR("%s: failed to initialize MLU: %s\n",
                __func__, cnrtGetErrorStr(err));
        return info;
    }

    GGML_ASSERT(info.device_count <= GGML_MLU_MAX_DEVICES);

    for (int id = 0; id < info.device_count; ++id) {
        size_t free, total;
        ggml_backend_mlu_get_device_memory(id, &free, &total);
        info.devices[id].total_vram = total;
    }

    return info;
}

/**
 * @brief 获取 MLU 设备信息
 *
 * @return const ggml_mlu_device_info& 设备信息结构体的引用
 */
const ggml_mlu_device_info& ggml_mlu_info() {
    static ggml_mlu_device_info info = ggml_mlu_init();
    return info;
}

struct ggml_backend_mlu_buffer_context {
    int device;
    void * dev_ptr = nullptr;
    std::string name;

    ggml_backend_mlu_buffer_context() = default;

    ggml_backend_mlu_buffer_context(int device, void * dev_ptr) :
        device(device), dev_ptr(dev_ptr),
        name(GGML_MLU_NAME + std::to_string(device)) {
    }

    ~ggml_backend_mlu_buffer_context() {
        if (dev_ptr != nullptr) {
            ggml_mlu_set_device(device);
            CNRT_CHECK(cnrtFree(dev_ptr));
            dev_ptr = nullptr;
        }
    }
};

//
// MLU 缓冲区类型
//

struct ggml_backend_mlu_buffer_type_context {
    int device;
    std::string name;
};

static void ggml_backend_mlu_buffer_free_buffer(ggml_backend_buffer_t buffer) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *)buffer->context;
    delete ctx;
}

static bool ggml_backend_buffer_is_mlu(ggml_backend_buffer_t buffer) {
    return buffer->iface.free_buffer == ggml_backend_mlu_buffer_free_buffer;
}

static void * ggml_backend_mlu_buffer_get_base(ggml_backend_buffer_t buffer) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *) buffer->context;
    return ctx->dev_ptr;
}

static enum ggml_status ggml_backend_mlu_buffer_init_tensor(ggml_backend_buffer_t buffer, ggml_tensor * tensor) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *) buffer->context;

    if (tensor->view_src != NULL) {
        assert(tensor->view_src->buffer->buft == buffer->buft);
        return GGML_STATUS_SUCCESS;
    }

    if (ggml_is_quantized(tensor->type) && tensor->view_src == nullptr && ggml_backend_buffer_get_usage(buffer) != GGML_BACKEND_BUFFER_USAGE_COMPUTE) {
        // initialize padding to 0 to avoid possible NaN values
        const size_t original_size = ggml_nbytes(tensor);
        const size_t padded_size = ggml_backend_buft_get_alloc_size(buffer->buft, tensor);

        if (padded_size > original_size) {
            ggml_mlu_set_device(ctx->device);
            CNRT_CHECK(cnrtMemset((char *)tensor->data + original_size, 0, padded_size - original_size));
        }
    }
    return GGML_STATUS_SUCCESS;
}

static void ggml_backend_mlu_buffer_memset_tensor(ggml_backend_buffer_t buffer, ggml_tensor * tensor, uint8_t value, size_t offset, size_t size) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *)buffer->context;
    
    ggml_mlu_set_device(ctx->device);
    CNRT_CHECK(cnrtMemset((char *)tensor->data + offset, value, size));
}

static void ggml_backend_mlu_buffer_set_tensor(ggml_backend_buffer_t buffer, ggml_tensor * tensor, const void * data, size_t offset, size_t size) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *) buffer->context;

    ggml_mlu_set_device(ctx->device);
    CNRT_CHECK(cnrtMemcpy((char *)tensor->data + offset, (void *)data, size, CNRT_MEM_TRANS_DIR_HOST2DEV));
}

static void ggml_backend_mlu_buffer_get_tensor(ggml_backend_buffer_t buffer, const ggml_tensor * tensor, void * data, size_t offset, size_t size) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *) buffer->context;

    ggml_mlu_set_device(ctx->device);
    CNRT_CHECK(cnrtMemcpy(data, (char *)tensor->data + offset, size, CNRT_MEM_TRANS_DIR_DEV2HOST));
}

static bool ggml_backend_mlu_buffer_cpy_tensor(ggml_backend_buffer_t buffer, const ggml_tensor * src, ggml_tensor * dst) {
    if (ggml_backend_buffer_is_mlu(src->buffer)) {
        ggml_backend_mlu_buffer_context * src_ctx = (ggml_backend_mlu_buffer_context *)src->buffer->context;
        ggml_backend_mlu_buffer_context * dst_ctx = (ggml_backend_mlu_buffer_context *)dst->buffer->context;
        if (src_ctx->device == dst_ctx->device) {
            CNRT_CHECK(cnrtMemcpy(dst->data, src->data, ggml_nbytes(src), CNRT_MEM_TRANS_DIR_DEV2DEV));
        } else {
#ifdef GGML_MLU_NO_PEER_COPY
            return false;
#else
            CNRT_CHECK(cnrtMemcpyPeer(dst->data, dst_ctx->device, src->data, src_ctx->device, ggml_nbytes(src)));
#endif
        }
        return true;
    }
    return false;

    GGML_UNUSED(buffer);
}

static void ggml_backend_mlu_buffer_clear(ggml_backend_buffer_t buffer, uint8_t value) {
    ggml_backend_mlu_buffer_context * ctx = (ggml_backend_mlu_buffer_context *) buffer->context;

    ggml_mlu_set_device(ctx->device);
    CNRT_CHECK(cnrtMemset(ctx->dev_ptr, value, buffer->size));
}

static struct ggml_backend_buffer_i ggml_backend_mlu_buffer_interface = {
    /* .free_buffer     = */ ggml_backend_mlu_buffer_free_buffer,
    /* .get_base        = */ ggml_backend_mlu_buffer_get_base,
    /* .init_tensor     = */ ggml_backend_mlu_buffer_init_tensor,
    /* .memset_tensor   = */ ggml_backend_mlu_buffer_memset_tensor,
    /* .set_tensor      = */ ggml_backend_mlu_buffer_set_tensor,
    /* .get_tensor      = */ ggml_backend_mlu_buffer_get_tensor,
    /* .cpy_tensor      = */ ggml_backend_mlu_buffer_cpy_tensor,
    /* .clear           = */ ggml_backend_mlu_buffer_clear,
    /* .reset           = */ NULL,
};

static const char * ggml_backend_mlu_buffer_type_get_name(ggml_backend_buffer_type_t buft) {
    ggml_backend_mlu_buffer_type_context * ctx = (ggml_backend_mlu_buffer_type_context *)buft->context;

    return ctx->name.c_str();
}

static ggml_backend_buffer_t ggml_backend_mlu_buffer_type_alloc_buffer(ggml_backend_buffer_type_t buft, size_t size) {
    ggml_backend_mlu_buffer_type_context * ctx = (ggml_backend_mlu_buffer_type_context *) buft->context;

    ggml_backend_mlu_buffer_context * buffer_ctx = new ggml_backend_mlu_buffer_context();
    buffer_ctx->device = ctx->device;
    buffer_ctx->dev_ptr = nullptr;

    size = std::max(size, (size_t)1);

    cnrtRet_t err = ggml_mlu_device_malloc(&buffer_ctx->dev_ptr, size, ctx->device);
    if (err != CNRT_RET_SUCCESS) {
        delete buffer_ctx;
        return nullptr;
    }

    ggml_backend_buffer_t buffer = new ggml_backend_buffer();
    buffer->buft = buft;
    buffer->context = buffer_ctx;
    buffer->size = size;
    buffer->usage = GGML_BACKEND_BUFFER_USAGE_ANY;
    buffer->iface = ggml_backend_mlu_buffer_interface;

    return buffer;
}

static size_t ggml_backend_mlu_buffer_type_get_alignment(ggml_backend_buffer_type_t buft) {
    return 128;
}

static size_t ggml_backend_mlu_buffer_type_get_alloc_size(ggml_backend_buffer_type_t buft, const ggml_tensor * tensor) {
    return ggml_nbytes(tensor);
}

static bool ggml_backend_mlu_buffer_type_is_host(ggml_backend_buffer_type_t buft) {
    return false;
}

static struct ggml_backend_buffer_type_i ggml_backend_mlu_buffer_type_interface = {
    /* .get_name         = */ ggml_backend_mlu_buffer_type_get_name,
    /* .alloc_buffer     = */ ggml_backend_mlu_buffer_type_alloc_buffer,
    /* .get_alignment    = */ ggml_backend_mlu_buffer_type_get_alignment,
    /* .get_max_size     = */ NULL, // defaults to SIZE_MAX
    /* .get_alloc_size   = */ ggml_backend_mlu_buffer_type_get_alloc_size,
    /* .is_host          = */ ggml_backend_mlu_buffer_type_is_host,
};

ggml_backend_buffer_type_t ggml_backend_mlu_buffer_type(int device) {
    static std::map<int, ggml_backend_buffer_type_t> buffer_types;
    static std::mutex mutex;

    std::lock_guard<std::mutex> lock(mutex);

    auto it = buffer_types.find(device);
    if (it != buffer_types.end()) {
        return it->second;
    }

    ggml_backend_mlu_buffer_type_context * ctx = new ggml_backend_mlu_buffer_type_context();
    ctx->device = device;

    ggml_backend_buffer_type_t buft = new ggml_backend_buffer_type();
    buft->context = ctx;
    buft->iface = ggml_backend_mlu_buffer_type_interface;

    buffer_types[device] = buft;

    return buft;
}

static bool ggml_backend_buft_is_mlu(ggml_backend_buffer_type_t buft) {
    return buft->iface.get_name == ggml_backend_mlu_buffer_type_get_name;
}

// mlu split buffer

static int64_t get_row_rounding(const std::array<float, GGML_MLU_MAX_DEVICES> & tensor_split) {
    int64_t row_rounding = 0;
    for (int id = 0; id < ggml_mlu_info().device_count; ++id) {
        if (tensor_split[id] >= (id + 1 < ggml_mlu_info().device_count ? tensor_split[id + 1] : 1.0f)) {
            continue;
        }
        // MLU doesn't have compute capability like CUDA, use a default rounding value
        row_rounding = std::max(row_rounding, (int64_t)32); // Default rounding for MLU
    }
    return row_rounding;
}

static void get_row_split(int64_t * row_low, int64_t * row_high, const ggml_tensor * tensor, const std::array<float, GGML_MLU_MAX_DEVICES> & tensor_split, int id) {
    const int64_t nrows = ggml_nrows(tensor);
    const int64_t rounding = get_row_rounding(tensor_split);

    *row_low = id == 0 ? 0 : nrows*tensor_split[id];
    *row_low -= *row_low % rounding;

    if (id == ggml_mlu_info().device_count - 1) {
        *row_high = nrows;
    } else {
        *row_high = nrows*tensor_split[id + 1];
        *row_high -= *row_high % rounding;
    }
}

static size_t ggml_nbytes_split(const struct ggml_tensor * tensor, int nrows_split) {
    static_assert(GGML_MAX_DIMS == 4, "GGML_MAX_DIMS is not 4 - update this function");

    return nrows_split*ggml_row_size(tensor->type, tensor->ne[0]);
}

struct ggml_backend_mlu_split_buffer_type_context {
    int main_device;
    std::array<float, GGML_MLU_MAX_DEVICES> tensor_split;
    std::string name;
};

struct ggml_backend_mlu_split_buffer_context {
    ~ggml_backend_mlu_split_buffer_context() {
        for (ggml_tensor_extra_mlu * extra : tensor_extras) {
            for (int id = 0; id < GGML_MLU_MAX_DEVICES; ++id) {
                if (extra->data_device[id] != nullptr) {
                    ggml_mlu_set_device(id);
                    CNRT_CHECK(cnrtFree(extra->data_device[id]));
                }
            }
            delete extra;
        }
    }

    std::vector<ggml_tensor_extra_mlu *> tensor_extras;
};

static void ggml_backend_mlu_split_buffer_free_buffer(ggml_backend_buffer_t buffer) {
    ggml_backend_mlu_split_buffer_context * ctx = (ggml_backend_mlu_split_buffer_context *)buffer->context;
    delete ctx;
}

static void * ggml_backend_mlu_split_buffer_get_base(ggml_backend_buffer_t buffer) {
    // the pointers are stored in the tensor extras, this is just a dummy address and never dereferenced
    return (void *)0x1000;

    GGML_UNUSED(buffer);
}

static enum ggml_status ggml_backend_mlu_split_buffer_init_tensor(ggml_backend_buffer_t buffer, ggml_tensor * tensor) {
    GGML_ASSERT(tensor->view_src == nullptr); // views of split tensors are not supported
    GGML_ASSERT(ggml_is_contiguous(tensor) && "split buffers only supported for contiguous tensors");

    ggml_backend_mlu_split_buffer_context * ctx = (ggml_backend_mlu_split_buffer_context *)buffer->context;
    ggml_backend_mlu_split_buffer_type_context * buft_ctx = (ggml_backend_mlu_split_buffer_type_context *)buffer->buft->context;

    const int64_t ne0 = tensor->ne[0];

    ggml_tensor_extra_mlu * extra = new ggml_tensor_extra_mlu{};
    ctx->tensor_extras.push_back(extra);

    for (int id = 0; id < ggml_mlu_info().device_count; ++id) {
        int64_t row_low, row_high;
        get_row_split(&row_low, &row_high, tensor, buft_ctx->tensor_split, id);

        int64_t nrows_split = row_high - row_low;
        if (nrows_split == 0) {
            continue;
        }

        size_t size = ggml_nbytes_split(tensor, nrows_split);
        const size_t original_size = size;

        // pad last row to a multiple of 512 elements to avoid out-of-bounds memory accesses
        if (ne0 % MATRIX_ROW_PADDING != 0) {
            size += ggml_row_size(tensor->type, MATRIX_ROW_PADDING - ne0 % MATRIX_ROW_PADDING);
        }

        // FIXME: do not crash if cnrtMalloc fails
        // currently, init_tensor cannot fail, it needs to be fixed in ggml-backend first
        ggml_mlu_set_device(id);
        char * buf;
        CNRT_CHECK(ggml_mlu_device_malloc((void**)&buf, size, id));

        // set padding to 0 to avoid possible NaN values
        if (size > original_size) {
            CNRT_CHECK(cnrtMemset(buf + original_size, 0, size - original_size));
        }

        extra->data_device[id] = buf;
    }
    tensor->extra = extra;
    return GGML_STATUS_SUCCESS;
}

static void ggml_backend_mlu_split_buffer_set_tensor(ggml_backend_buffer_t buffer, ggml_tensor * tensor, const void * data, size_t offset, size_t size) {
    // split tensors must always be set in their entirety at once
    GGML_ASSERT(offset == 0);
    GGML_ASSERT(size == ggml_nbytes(tensor));
    GGML_ASSERT(ggml_is_contiguous(tensor) && "split buffers only supported for contiguous tensors");

    ggml_backend_mlu_split_buffer_type_context * buft_ctx = (ggml_backend_mlu_split_buffer_type_context *)buffer->buft->context;

    const int64_t ne0 = tensor->ne[0];
    const size_t nb1 = tensor->nb[1];
    ggml_tensor_extra_mlu * extra = (ggml_tensor_extra_mlu *)tensor->extra;

    for (int id = 0; id < ggml_mlu_info().device_count; ++id) {
        int64_t row_low, row_high;
        get_row_split(&row_low, &row_high, tensor, buft_ctx->tensor_split, id);

        int64_t nrows_split = row_high - row_low;
        if (nrows_split == 0) {
            continue;
        }

        const size_t offset_split = row_low*nb1;
        size_t size = ggml_nbytes_split(tensor, nrows_split);
        const size_t original_size = size;

        // pad last row to a multiple of 512 elements to avoid out-of-bounds memory accesses
        if (ne0 % MATRIX_ROW_PADDING != 0) {
            size += ggml_row_size(tensor->type, MATRIX_ROW_PADDING - ne0 % MATRIX_ROW_PADDING);
        }

        const char * buf_host = (const char *)data + offset_split;
        ggml_mlu_set_device(id);
        CNRT_CHECK(cnrtMemcpy(extra->data_device[id], const_cast<void*>((const void*)buf_host), original_size, CNRT_MEM_TRANS_DIR_HOST2DEV));
    }
}

static void ggml_backend_mlu_split_buffer_get_tensor(ggml_backend_buffer_t buffer, const ggml_tensor * tensor, void * data, size_t offset, size_t size) {
    // split tensors must always be set in their entirety at once
    GGML_ASSERT(offset == 0);
    GGML_ASSERT(size == ggml_nbytes(tensor));
    GGML_ASSERT(ggml_is_contiguous(tensor) && "split buffers only supported for contiguous tensors");

    ggml_backend_mlu_split_buffer_type_context * buft_ctx = (ggml_backend_mlu_split_buffer_type_context *)buffer->buft->context;

    const int64_t ne0 = tensor->ne[0];
    const size_t nb1 = tensor->nb[1];
    ggml_tensor_extra_mlu * extra = (ggml_tensor_extra_mlu *)tensor->extra;

    for (int id = 0; id < ggml_mlu_info().device_count; ++id) {
        int64_t row_low, row_high;
        get_row_split(&row_low, &row_high, tensor, buft_ctx->tensor_split, id);

        int64_t nrows_split = row_high - row_low;
        if (nrows_split == 0) {
            continue;
        }

        const size_t offset_split = row_low*nb1;
        size_t size = ggml_nbytes_split(tensor, nrows_split);
        const size_t original_size = size;

        // pad last row to a multiple of 512 elements to avoid out-of-bounds memory accesses
        if (ne0 % MATRIX_ROW_PADDING != 0) {
            size += ggml_row_size(tensor->type, MATRIX_ROW_PADDING - ne0 % MATRIX_ROW_PADDING);
        }

        char * buf_host = (char *)data + offset_split;
        ggml_mlu_set_device(id);
        CNRT_CHECK(cnrtMemcpy(buf_host, extra->data_device[id], original_size, CNRT_MEM_TRANS_DIR_DEV2HOST));
    }
}

static void ggml_backend_mlu_split_buffer_clear(ggml_backend_buffer_t buffer, uint8_t value) {
    GGML_UNUSED(buffer);
    GGML_UNUSED(value);
}

static const ggml_backend_buffer_i ggml_backend_mlu_split_buffer_interface = {
    /* .free_buffer     = */ ggml_backend_mlu_split_buffer_free_buffer,
    /* .get_base        = */ ggml_backend_mlu_split_buffer_get_base,
    /* .init_tensor     = */ ggml_backend_mlu_split_buffer_init_tensor,
    /* .memset_tensor   = */ NULL,
    /* .set_tensor      = */ ggml_backend_mlu_split_buffer_set_tensor,
    /* .get_tensor      = */ ggml_backend_mlu_split_buffer_get_tensor,
    /* .cpy_tensor      = */ NULL,
    /* .clear           = */ ggml_backend_mlu_split_buffer_clear,
    /* .reset           = */ NULL,
};

// mlu split buffer type

static const char * ggml_backend_mlu_split_buffer_type_get_name(ggml_backend_buffer_type_t buft) {
    ggml_backend_mlu_split_buffer_type_context * ctx = (ggml_backend_mlu_split_buffer_type_context *)buft->context;

    return ctx->name.c_str();
}

static bool ggml_backend_buft_is_mlu_split(ggml_backend_buffer_type_t buft) {
    return buft->iface.get_name == ggml_backend_mlu_split_buffer_type_get_name;
}

static ggml_backend_buffer_t ggml_backend_mlu_split_buffer_type_alloc_buffer(ggml_backend_buffer_type_t buft, size_t size) {
    // since we don't know the exact split after rounding, we cannot allocate the device buffers at this point
    // instead, we allocate them for each tensor separately in init_tensor
    // however, the size still represents the maximum cumulative size of all the device buffers after the tensors are allocated,
    // as returned by get_alloc_size. this limit is enforced during tensor allocation by ggml-alloc, so it needs to be correct.
    ggml_backend_mlu_split_buffer_context * ctx = new ggml_backend_mlu_split_buffer_context();

    return ggml_backend_buffer_init(buft, ggml_backend_mlu_split_buffer_interface, ctx, size);
}

static size_t ggml_backend_mlu_split_buffer_type_get_alignment(ggml_backend_buffer_type_t buft) {
    return 128;
    GGML_UNUSED(buft);
}

static size_t ggml_backend_mlu_split_buffer_type_get_alloc_size(ggml_backend_buffer_type_t buft, const ggml_tensor * tensor) {
    ggml_backend_mlu_split_buffer_type_context * ctx = (ggml_backend_mlu_split_buffer_type_context *)buft->context;

    size_t total_size = 0;

    const int64_t ne0 = tensor->ne[0];

    for (int id = 0; id < ggml_mlu_info().device_count; ++id) {
        int64_t row_low, row_high;
        get_row_split(&row_low, &row_high, tensor, ctx->tensor_split, id);

        int64_t nrows_split = row_high - row_low;
        if (nrows_split == 0) {
            continue;
        }

        total_size += ggml_nbytes_split(tensor, nrows_split);

        // pad last row to a multiple of 512 elements to avoid out-of-bounds memory accesses
        if (ne0 % MATRIX_ROW_PADDING != 0) {
            total_size += ggml_row_size(tensor->type, MATRIX_ROW_PADDING - ne0 % MATRIX_ROW_PADDING);
        }
    }

    return total_size;
}

static bool ggml_backend_mlu_split_buffer_type_is_host(ggml_backend_buffer_type_t buft) {
    return false;
    GGML_UNUSED(buft);
}

static const ggml_backend_buffer_type_i ggml_backend_mlu_split_buffer_type_interface = {
    /* .get_name         = */ ggml_backend_mlu_split_buffer_type_get_name,
    /* .alloc_buffer     = */ ggml_backend_mlu_split_buffer_type_alloc_buffer,
    /* .get_alignment    = */ ggml_backend_mlu_split_buffer_type_get_alignment,
    /* .get_max_size     = */ NULL, // defaults to SIZE_MAX
    /* .get_alloc_size   = */ ggml_backend_mlu_split_buffer_type_get_alloc_size,
    /* .is_host          = */ ggml_backend_mlu_split_buffer_type_is_host,
};

ggml_backend_buffer_type_t ggml_backend_mlu_split_buffer_type(int main_device, const float * tensor_split) {
    GGML_ASSERT(main_device < ggml_mlu_info().device_count);

    static std::map<std::array<float, GGML_MLU_MAX_DEVICES>, std::unique_ptr<ggml_backend_buffer_type>> buft_map;

    std::array<float, GGML_MLU_MAX_DEVICES> tensor_split_arr = {};

    bool all_zero = tensor_split == nullptr || tensor_split[0] == 0.0f;
    for (int i = 0; i < ggml_mlu_info().device_count; ++i) {
        tensor_split_arr[i] = all_zero ? 1.0f / ggml_mlu_info().device_count : tensor_split[i];
    }

    auto it = buft_map.find(tensor_split_arr);
    if (it != buft_map.end()) {
        return it->second.get();
    }

    struct ggml_backend_mlu_split_buffer_type_context * ctx = new ggml_backend_mlu_split_buffer_type_context {
        /* .main_device    = */ main_device,
        /* .tensor_split   = */ tensor_split_arr,
        /* .name           = */ GGML_MLU_NAME "_Split",
    };

    ggml_backend_buffer_type_t buft = new ggml_backend_buffer_type {
        /* .iface   = */ ggml_backend_mlu_split_buffer_type_interface,
        /* .device  = */ ggml_backend_reg_dev_get(ggml_backend_mlu_reg(), main_device),
        /* .context = */ ctx,
    };

    auto result = buft_map.emplace(tensor_split_arr, std::unique_ptr<ggml_backend_buffer_type>(buft));
    return result.first->second.get();
}

//
// MLU 主机缓冲区类型
//

static const char * ggml_backend_mlu_host_buffer_type_get_name(ggml_backend_buffer_type_t buft) {
    return "MLU host";
}

static bool ggml_backend_buft_is_mlu_host(ggml_backend_buffer_type_t buft) {
    return buft->iface.get_name == ggml_backend_mlu_host_buffer_type_get_name;
}

static void ggml_backend_mlu_host_buffer_free_buffer(ggml_backend_buffer_t buffer) {
    CNRT_CHECK(cnrtFreeHost(buffer->context));
}

static void * ggml_mlu_host_malloc(size_t size) {
    void * ptr = nullptr;
    CNRT_CHECK(cnrtHostMalloc(&ptr, size));
    return ptr;
}

static ggml_backend_buffer_t ggml_backend_mlu_host_buffer_type_alloc_buffer(ggml_backend_buffer_type_t buft, size_t size) {
    void * ptr = ggml_mlu_host_malloc(size);

    if (ptr == nullptr) {
        // fallback to cpu buffer
        return ggml_backend_buft_alloc_buffer(ggml_backend_cpu_buffer_type(), size);
    }

    ggml_backend_buffer_t buffer = ggml_backend_cpu_buffer_from_ptr(ptr, size);
    buffer->buft = buft;
    buffer->iface.free_buffer = ggml_backend_mlu_host_buffer_free_buffer;

    return buffer;
}

ggml_backend_buffer_type_t ggml_backend_mlu_host_buffer_type() {
    static struct ggml_backend_buffer_type ggml_backend_mlu_buffer_type_host = {
        /* .iface    = */ {
            /* .get_name         = */ ggml_backend_mlu_host_buffer_type_get_name,
            /* .alloc_buffer     = */ ggml_backend_mlu_host_buffer_type_alloc_buffer,
            /* .get_alignment    = */ ggml_backend_cpu_buffer_type()->iface.get_alignment,
            /* .get_max_size     = */ NULL, // defaults to SIZE_MAX
            /* .get_alloc_size   = */ ggml_backend_cpu_buffer_type()->iface.get_alloc_size,
            /* .is_host          = */ ggml_backend_cpu_buffer_type()->iface.is_host,
        },
        /* .device   = */ ggml_backend_reg_dev_get(ggml_backend_mlu_reg(), 0),
        /* .context  = */ nullptr,
    };

    return &ggml_backend_mlu_buffer_type_host;
}

//
// MLU 后端实现
//

static const char * ggml_backend_mlu_get_name(ggml_backend_t backend) {
    ggml_backend_mlu_context * mlu_ctx = (ggml_backend_mlu_context *) backend->context;

    return mlu_ctx->name.c_str();
}

static void ggml_backend_mlu_free(ggml_backend_t backend) {
    ggml_backend_mlu_context * mlu_ctx = (ggml_backend_mlu_context *) backend->context;

    delete mlu_ctx;
    delete backend;
}

static void ggml_backend_mlu_set_tensor_async(ggml_backend_t backend, ggml_tensor * tensor, const void * data, size_t offset, size_t size) {
    ggml_backend_mlu_context * mlu_ctx = (ggml_backend_mlu_context *)backend->context;
    ggml_backend_buffer_t buf = tensor->view_src ? tensor->view_src->buffer : tensor->buffer;

    GGML_ASSERT(buf->buft == ggml_backend_mlu_buffer_type(mlu_ctx->device) && "unsupported buffer type");

    CNRT_CHECK(cnrtMemcpyAsync((char *)tensor->data + offset, const_cast<void*>(data), size, mlu_ctx->queue(), cnrtMemcpyHostToDev));
}

static void ggml_backend_mlu_get_tensor_async(ggml_backend_t backend, const ggml_tensor * tensor, void * data, size_t offset, size_t size) {
    ggml_backend_mlu_context * mlu_ctx = (ggml_backend_mlu_context *)backend->context;
    ggml_backend_buffer_t buf = tensor->view_src ? tensor->view_src->buffer : tensor->buffer;

    GGML_ASSERT(buf->buft == ggml_backend_mlu_buffer_type(mlu_ctx->device) && "unsupported buffer type");

    CNRT_CHECK(cnrtMemcpyAsync(data, (char *)tensor->data + offset, size, mlu_ctx->queue(), cnrtMemcpyDevToHost));
}

static bool ggml_backend_mlu_cpy_tensor_async(ggml_backend_t backend_src, ggml_backend_t backend_dst, const ggml_tensor * src, ggml_tensor * dst) {
    ggml_backend_buffer_t buf_src = src->view_src ? src->view_src->buffer : src->buffer;
    ggml_backend_buffer_t buf_dst = dst->view_src ? dst->view_src->buffer : dst->buffer;

    if (!ggml_backend_is_mlu(backend_src) || !ggml_backend_is_mlu(backend_dst)) {
        return false;
    }

    if (!ggml_backend_buffer_is_mlu(src->buffer) || !ggml_backend_buffer_is_mlu(dst->buffer)) {
        return false;
    }

    // device -> device copy
    ggml_backend_mlu_context * mlu_ctx_src = (ggml_backend_mlu_context *)backend_src->context;
    ggml_backend_mlu_context * mlu_ctx_dst = (ggml_backend_mlu_context *)backend_dst->context;

    ggml_backend_mlu_buffer_context * buf_ctx_src = (ggml_backend_mlu_buffer_context *)buf_src->context;
    ggml_backend_mlu_buffer_context * buf_ctx_dst = (ggml_backend_mlu_buffer_context *)buf_dst->context;

    if (mlu_ctx_src->device != buf_ctx_src->device || mlu_ctx_dst->device != buf_ctx_dst->device) {
#ifndef NDEBUG
        GGML_LOG_DEBUG("%s: backend and buffer devices do not match\n", __func__);
#endif
        return false;
    }

    if (backend_src != backend_dst) {
        // copy on src queue
        if (mlu_ctx_src->device == mlu_ctx_dst->device) {
            CNRT_CHECK(cnrtMemcpyAsync(dst->data, src->data, ggml_nbytes(dst), mlu_ctx_src->queue(), cnrtMemcpyDevToDev));
        } else {
#ifdef GGML_MLU_NO_PEER_COPY
            return false;
#else
            CNRT_CHECK(cnrtMemcpyPeerAsync(dst->data, mlu_ctx_dst->device, src->data, mlu_ctx_src->device, ggml_nbytes(dst), mlu_ctx_src->queue()));
#endif
        }
    } else {
        // src and dst are on the same backend
        CNRT_CHECK(cnrtMemcpyAsync(dst->data, src->data, ggml_nbytes(dst), mlu_ctx_src->queue(), cnrtMemcpyDevToDev));
    }
    return true;
}

static void ggml_backend_mlu_synchronize(ggml_backend_t backend) {
    ggml_backend_mlu_context * mlu_ctx = (ggml_backend_mlu_context *) backend->context;
    ggml_mlu_set_device(mlu_ctx->device);
    CNRT_CHECK(cnrtSyncDevice());
}

static bool ggml_mlu_compute_forward(ggml_backend_mlu_context & ctx, struct ggml_tensor * dst) {
    return ggml_mlu_forward_impl(ctx, dst);
}

static void evaluate_and_capture_mlu_graph(ggml_backend_mlu_context * mlu_ctx, ggml_cgraph * cgraph,
    bool & graph_evaluated_or_captured, bool & use_mlu_graph, bool & mlu_graph_update_required) {
    // flag used to determine whether it is an integrated_gpu
    const bool integrated = false; // ggml_mlu_info().devices[mlu_ctx->device].integrated;

    for (int i=0; i<cgraph->n_nodes; i++) {
        ggml_tensor * node = cgraph->nodes[i];
        ggml_tensor_print_info(node, nullptr);
    }

    while (!graph_evaluated_or_captured) {
        // Only perform the graph execution if MLU graphs are not enabled, or we are capturing the graph.
        // With the use of MLU graphs, the execution will be performed by the graph launch.
        if (!use_mlu_graph || mlu_graph_update_required) {
            for (int i = 0; i < cgraph->n_nodes; i++) {
                ggml_tensor * node = cgraph->nodes[i];

                if (ggml_is_empty(node) || node->op == GGML_OP_RESHAPE || node->op == GGML_OP_TRANSPOSE || node->op == GGML_OP_VIEW || node->op == GGML_OP_PERMUTE || node->op == GGML_OP_NONE) {
                    continue;
                }

#ifndef NDEBUG
                assert(node->buffer->buft == ggml_backend_mlu_buffer_type(mlu_ctx->device));
                for (int j = 0; j < GGML_MAX_SRC; j++) {
                    if (node->src[j] != nullptr) {
                        assert(node->src[j]->buffer);
                        assert(node->src[j]->buffer->buft == ggml_backend_mlu_buffer_type(mlu_ctx->device) ||
                               (integrated && ggml_backend_buft_is_mlu_host(node->src[j]->buffer->buft)));
                    }
                }
#endif

                bool ok = ggml_mlu_compute_forward(*mlu_ctx, node);
                if (!ok) {
                    GGML_LOG_ERROR("%s: op not supported %s (%s)\n", __func__, node->name, ggml_op_name(node->op));
                }
                GGML_ASSERT(ok);
            }
        }
        graph_evaluated_or_captured = true;
    }
}

static enum ggml_status ggml_backend_mlu_graph_compute(ggml_backend_t backend, ggml_cgraph * cgraph) {
    ggml_backend_mlu_context * mlu_ctx = (ggml_backend_mlu_context *)backend->context;
    ggml_mlu_set_device(mlu_ctx->device);

    bool graph_evaluated_or_captured = false;
    bool use_mlu_graph = false;
    bool mlu_graph_update_required = true;
    evaluate_and_capture_mlu_graph(mlu_ctx, cgraph, graph_evaluated_or_captured, use_mlu_graph, mlu_graph_update_required);

    return GGML_STATUS_SUCCESS;
}

static ggml_backend_i ggml_backend_mlu_interface = {
    /* .get_name                = */ ggml_backend_mlu_get_name,
    /* .free                    = */ ggml_backend_mlu_free,
    /* .set_tensor_async        = */ ggml_backend_mlu_set_tensor_async,
    /* .get_tensor_async        = */ ggml_backend_mlu_get_tensor_async,
    /* .cpy_tensor_async        = */ ggml_backend_mlu_cpy_tensor_async,
    /* .synchronize             = */ ggml_backend_mlu_synchronize,
    /* .graph_plan_create       = */ NULL,
    /* .graph_plan_free         = */ NULL,
    /* .graph_plan_update       = */ NULL,
    /* .graph_plan_compute      = */ NULL,
    /* .graph_compute           = */ ggml_backend_mlu_graph_compute,
    /* .event_record            = */ NULL,
    /* .event_wait              = */ NULL,
};

ggml_guid_t ggml_backend_mlu_guid() {
    static ggml_guid guid = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    return &guid;
}

struct ggml_backend_mlu_device_context {
    int device;
    std::string name;
    std::string description;
};

static const char * ggml_backend_mlu_device_get_name(ggml_backend_dev_t dev) {
    ggml_backend_mlu_device_context * ctx = (ggml_backend_mlu_device_context *) dev->context;
    return ctx->name.c_str();
}

static const char * ggml_backend_mlu_device_get_description(ggml_backend_dev_t dev) {
    ggml_backend_mlu_device_context * ctx = (ggml_backend_mlu_device_context *) dev->context;
    return ctx->description.c_str();
}

static void ggml_backend_mlu_device_get_memory(ggml_backend_dev_t dev, size_t * free, size_t * total) {
    ggml_backend_mlu_device_context * ctx = (ggml_backend_mlu_device_context *) dev->context;
    ggml_backend_mlu_get_device_memory(ctx->device, free, total);
}

static enum ggml_backend_dev_type ggml_backend_mlu_device_get_type(ggml_backend_dev_t dev) {
    return GGML_BACKEND_DEVICE_TYPE_GPU;
}

static void ggml_backend_mlu_device_get_props(ggml_backend_dev_t dev, struct ggml_backend_dev_props * props) {
    ggml_backend_mlu_device_context * ctx = (ggml_backend_mlu_device_context *) dev->context;

    props->name = ctx->name.c_str();
    props->description = ctx->description.c_str();
    props->type = GGML_BACKEND_DEVICE_TYPE_GPU;
    props->caps.async = true;
    props->caps.host_buffer = true;
    props->caps.buffer_from_host_ptr = false;
    props->caps.events = true;

    ggml_backend_mlu_get_device_memory(ctx->device, &props->memory_free, &props->memory_total);
}

static ggml_backend_t ggml_backend_mlu_device_init_backend(ggml_backend_dev_t dev, const char * params) {
    GGML_UNUSED(params);
    ggml_backend_mlu_device_context * ctx = (ggml_backend_mlu_device_context *)dev->context;
    return ggml_backend_mlu_init(ctx->device);
}

static ggml_backend_buffer_type_t ggml_backend_mlu_device_get_buffer_type(ggml_backend_dev_t dev) {
    ggml_backend_mlu_device_context * ctx = (ggml_backend_mlu_device_context *)dev->context;
    return ggml_backend_mlu_buffer_type(ctx->device);
}

static ggml_backend_buffer_type_t ggml_backend_mlu_device_get_host_buffer_type(ggml_backend_dev_t dev) {
    GGML_UNUSED(dev);
    return ggml_backend_mlu_host_buffer_type();
}

static bool ggml_backend_mlu_device_supports_op(ggml_backend_dev_t dev, const ggml_tensor * op) {
    ggml_backend_mlu_device_context * dev_ctx = (ggml_backend_mlu_device_context *) dev->context;

    // check if the operation is MUL_MAT before checking for split buffers
    if (op->op == GGML_OP_MUL_MAT) {
        // split buffers are only supported for MUL_MAT operations
        struct ggml_tensor * a = op->src[0];
        struct ggml_tensor * b = op->src[1];
        
        if (a->buffer && ggml_backend_buft_is_mlu_split(a->buffer->buft)) {
            if (a->ne[2] > 1 || a->ne[3] > 1) {
                return false; // split buffers don't support batch dimensions
            }
            // check if this device has any rows assigned
            ggml_backend_mlu_split_buffer_type_context * buft_ctx = (ggml_backend_mlu_split_buffer_type_context *)a->buffer->buft->context;
            int device_id = dev_ctx->device;
            int64_t row_low, row_high;
            get_row_split(&row_low, &row_high, a, buft_ctx->tensor_split, device_id);
            if (row_low == row_high) {
                return false; // no rows assigned to this device
            }
        }
    } else {
        // for non-MUL_MAT operations, check that none of the source tensors are split buffers
        for (int i = 0; i < GGML_MAX_SRC; ++i) {
            if (op->src[i] != nullptr && op->src[i]->buffer && ggml_backend_buft_is_mlu_split(op->src[i]->buffer->buft)) {
                return false;
            }
        }
    }

    // check if all the sources are allocated on this device
    for (int i = 0; i < GGML_MAX_SRC; i++) {
        if (op->src[i] && op->src[i]->buffer && ggml_backend_buft_is_mlu(op->src[i]->buffer->buft)) {
            ggml_backend_mlu_buffer_type_context * buft_ctx =
                    (ggml_backend_mlu_buffer_type_context *)op->src[i]->buffer->buft->context;
            if (buft_ctx->device != dev_ctx->device) {
                return false;
            }
        }
    }

    switch (op->op) {
        case GGML_OP_UNARY:
            switch (ggml_get_unary_op(op)) {
                case GGML_UNARY_OP_ABS:
                case GGML_UNARY_OP_SGN:
                case GGML_UNARY_OP_NEG:
                case GGML_UNARY_OP_STEP:
                case GGML_UNARY_OP_GELU:
                case GGML_UNARY_OP_SILU:
                case GGML_UNARY_OP_RELU:
                case GGML_UNARY_OP_SIGMOID:
                case GGML_UNARY_OP_HARDSIGMOID:
                case GGML_UNARY_OP_HARDSWISH:
                case GGML_UNARY_OP_GELU_ERF:
                case GGML_UNARY_OP_GELU_QUICK:
                case GGML_UNARY_OP_TANH:
                case GGML_UNARY_OP_EXP:
                    return ggml_is_contiguous(op->src[0]);
                default:
                    return false;
            }
            break;
        case GGML_OP_MUL_MAT:
        case GGML_OP_MUL_MAT_ID:
            {
                struct ggml_tensor * a = op->src[0];
                struct ggml_tensor * b = op->src[1];
                if (b->type == GGML_TYPE_F16 && a->type != GGML_TYPE_F16) {
                    return false;
                }

                switch (a->type) {
                    case GGML_TYPE_F32:
                    case GGML_TYPE_F16:
                    case GGML_TYPE_Q4_0:
                    case GGML_TYPE_Q4_1:
                    case GGML_TYPE_Q5_0:
                    case GGML_TYPE_Q5_1:
                    case GGML_TYPE_Q8_0:
                    case GGML_TYPE_Q2_K:
                    case GGML_TYPE_Q3_K:
                    case GGML_TYPE_Q4_K:
                    case GGML_TYPE_Q5_K:
                    case GGML_TYPE_Q6_K:
                    case GGML_TYPE_Q8_K:
                    case GGML_TYPE_IQ1_M:
                    case GGML_TYPE_IQ1_S:
                    case GGML_TYPE_IQ2_S:
                    case GGML_TYPE_IQ2_XS:
                    case GGML_TYPE_IQ2_XXS:
                    case GGML_TYPE_IQ3_S:
                    case GGML_TYPE_IQ3_XXS:
                    case GGML_TYPE_IQ4_NL:
                    case GGML_TYPE_IQ4_XS:
                    case GGML_TYPE_BF16:
                        return true;
                    default:
                        return false;
                }
            } break;
        case GGML_OP_OUT_PROD:
            return op->type == GGML_TYPE_F32 && op->src[0]->type == GGML_TYPE_F32 && op->src[1]->type == GGML_TYPE_F32;
        case GGML_OP_GET_ROWS:
            {
                switch (op->src[0]->type) {
                    case GGML_TYPE_F16:
                    case GGML_TYPE_F32:
                    case GGML_TYPE_Q4_0:
                    case GGML_TYPE_Q4_1:
                    case GGML_TYPE_Q5_0:
                    case GGML_TYPE_Q5_1:
                    case GGML_TYPE_Q8_0:
                        return true;
                    default:
                        return false;
                }
            } break;
        // case GGML_OP_GET_ROWS_BACK:
        //     {
        //         return op->type == GGML_TYPE_F32 && op->src[0]->type == GGML_TYPE_F32 && op->ne[2] == 1 && op->ne[3] == 1;
        //     } break;
        case GGML_OP_CPY:
            {
                ggml_type src0_type = op->src[0]->type;
                ggml_type src1_type = op->src[1]->type;
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_BF16) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_F16) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_Q8_0) {
                    return true;
                }
                if (src0_type == GGML_TYPE_Q8_0 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_Q4_0) {
                    return true;
                }
                if (src0_type == GGML_TYPE_Q4_0 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_Q4_1) {
                    return true;
                }
                if (src0_type == GGML_TYPE_Q4_1 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_Q5_0) {
                    return true;
                }
                if (src0_type == GGML_TYPE_Q5_0 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_Q5_1) {
                    return true;
                }
                if (src0_type == GGML_TYPE_Q5_1 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_IQ4_NL) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F16 && src1_type == GGML_TYPE_F16) {
                    return true;
                }
                if (src0_type == GGML_TYPE_F16 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                if (src0_type == src1_type && ggml_is_contiguous(op->src[0]) && ggml_is_contiguous(op->src[1])) {
                    return true;
                }
                return false;
            } break;
        case GGML_OP_DUP:
            {
                ggml_type src0_type = op->src[0]->type;
                return src0_type != GGML_TYPE_I32 && src0_type != GGML_TYPE_I16;
            } break;
        case GGML_OP_ARGMAX:
        case GGML_OP_COUNT_EQUAL:
            {
                return true;
            } break;
        case GGML_OP_REPEAT:
            {
                ggml_type src0_type = op->src[0]->type;
                return src0_type != GGML_TYPE_I32 && src0_type != GGML_TYPE_I16;
            } break;
        // case GGML_OP_REPEAT_BACK:
        //         return op->type == GGML_TYPE_F32 && (op->src[0]->ne[2]*op->src[0]->ne[3]) <= (1 << 15);
        case GGML_OP_CONCAT:
            {
                ggml_type src0_type = op->src[0]->type;
                return src0_type != GGML_TYPE_I32 && src0_type != GGML_TYPE_I16;
            } break;
        case GGML_OP_CONV_TRANSPOSE_1D:
            {
                ggml_type src0_type = op->src[0]->type;
                ggml_type src1_type = op->src[1]->type;
                if (src0_type == GGML_TYPE_F32 && src1_type == GGML_TYPE_F32) {
                    return true;
                }
                return false;
            } break;
        // case GGML_OP_SILU_BACK:
        //     return ggml_is_contiguous(op->src[0]) && op->src[0]->type == GGML_TYPE_F32;
        //     break;
        case GGML_OP_NORM:
        case GGML_OP_RMS_NORM:
        case GGML_OP_L2_NORM:
            return true;
        // case GGML_OP_RMS_NORM_BACK:
        //     return ggml_is_contiguous(op->src[0]) && op->ne[0] % WARP_SIZE == 0;
        //     break;
        case GGML_OP_NONE:
        case GGML_OP_RESHAPE:
        case GGML_OP_VIEW:
        case GGML_OP_PERMUTE:
        case GGML_OP_TRANSPOSE:
        case GGML_OP_ADD:
        case GGML_OP_ADD1:
        case GGML_OP_SUB:
        case GGML_OP_MUL:
        case GGML_OP_DIV:
        case GGML_OP_SCALE:
        case GGML_OP_SQR:
        case GGML_OP_SQRT:
        case GGML_OP_SIN:
        case GGML_OP_COS:
        case GGML_OP_CLAMP:
        case GGML_OP_LOG:
        case GGML_OP_SSM_SCAN:
        case GGML_OP_SSM_CONV:
            return true;
        case GGML_OP_CONT:
            return op->src[0]->type != GGML_TYPE_BF16;
        case GGML_OP_DIAG_MASK_INF:
        case GGML_OP_SOFT_MAX:
            return true;
        case GGML_OP_SOFT_MAX_BACK: {
            float max_bias = 0.0f;
            memcpy(&max_bias, (const float *) op->op_params + 1, sizeof(float));
            return max_bias == 0.0f;
        }
        case GGML_OP_ROPE:
        case GGML_OP_ROPE_BACK: {
            return op->src[0]->nb[0] == ggml_type_size(op->src[0]->type) && ggml_is_contiguous_2(op->src[0]);
        }
        case GGML_OP_IM2COL:
        case GGML_OP_POOL_2D:
        case GGML_OP_SUM:
        case GGML_OP_SUM_ROWS:
        case GGML_OP_ARGSORT:
        case GGML_OP_ACC:
            return true;
        case GGML_OP_GROUP_NORM:
            return ggml_is_contiguous(op->src[0]);
        case GGML_OP_UPSCALE:
            return op->src[0]->type == GGML_TYPE_F32 && op->op_params[0] == GGML_SCALE_MODE_NEAREST;
        case GGML_OP_PAD:
        case GGML_OP_ARANGE:
        case GGML_OP_TIMESTEP_EMBEDDING:
        case GGML_OP_LEAKY_RELU:
        case GGML_OP_RWKV_WKV6:
        case GGML_OP_GATED_LINEAR_ATTN:
        case GGML_OP_RWKV_WKV7:
            return true;
        case GGML_OP_FLASH_ATTN_EXT: {
#ifndef FLASH_ATTN_AVAILABLE
            return false;
#endif // FLASH_ATTN_AVAILABLE
            if (op->src[1]->ne[0] != op->src[2]->ne[0]) {
                const int gqa_ratio = op->src[0]->ne[2] / op->src[1]->ne[2];
                return op->src[1]->ne[0] == 576 && op->src[2]->ne[0] == 512 && op->src[3] && gqa_ratio % 16 == 0;
            }
            if (op->src[0]->ne[0] == 192) {
                return false;
            }
            if (op->src[0]->ne[3] != 1) {
                return false;
            }
            if (op->src[1]->type == GGML_TYPE_BF16 || op->src[2]->type == GGML_TYPE_BF16) {
                return false;
            }
            if (op->src[0]->ne[0] ==  64 && op->src[1]->type == GGML_TYPE_F16) {
                return true;
            }
            if (op->src[0]->ne[0] == 128) {
                return true;
            }
            if (op->src[0]->ne[0] == 256 && op->src[1]->type == GGML_TYPE_F16 && op->src[2]->type == GGML_TYPE_F16) {
                return true;
            }
            return op->src[1]->type == GGML_TYPE_F16 && op->src[2]->type == GGML_TYPE_F16;
        }
        case GGML_OP_CROSS_ENTROPY_LOSS:
        case GGML_OP_CROSS_ENTROPY_LOSS_BACK:
        case GGML_OP_OPT_STEP_ADAMW:
            return true;
        default:
            return false;
    }
}

static bool ggml_backend_mlu_supports_buft(ggml_backend_dev_t dev, ggml_backend_buffer_type_t buft) {
    if (ggml_backend_buft_is_mlu(buft)) {
        return buft->device == dev;
    }
    if (ggml_backend_buft_is_mlu_split(buft)) {
        return true; // split buffers are supported by all MLU devices
    }
    return false;
}

static int64_t get_op_batch_size(const ggml_tensor * op) {
    switch (op->op) {
        case GGML_OP_GET_ROWS:
            return 0;
        case GGML_OP_MUL_MAT:
            return op->ne[1];
        case GGML_OP_MUL_MAT_ID:
        case GGML_OP_ROPE:
        case GGML_OP_ROPE_BACK:
            return op->ne[2];
        default:
            return ggml_nrows(op);
    }
}

static bool ggml_backend_mlu_device_offload_op(ggml_backend_dev_t dev, const ggml_tensor * op) {
    const std::vector<ggml_op> mlu_support_op_list = {
        GGML_OP_MUL_MAT,
        GGML_OP_UNARY,
        GGML_OP_ADD,
        GGML_OP_MUL,
        GGML_OP_RMS_NORM,
        GGML_OP_ROPE,
        GGML_OP_SOFT_MAX,
        GGML_OP_VIEW,
        GGML_OP_TRANSPOSE,
        GGML_OP_RESHAPE,
        GGML_OP_PERMUTE,
        GGML_OP_CONT,
        GGML_OP_CPY,
        GGML_OP_GET_ROWS,
    };

    if (std::find(mlu_support_op_list.begin(), mlu_support_op_list.end(), op->op) != mlu_support_op_list.end()) {
        return true;
    }
    return false;

    GGML_UNUSED(dev);
}

static ggml_backend_event_t ggml_backend_mlu_device_event_new(ggml_backend_dev_t dev) {
    ggml_backend_mlu_device_context * dev_ctx = (ggml_backend_mlu_device_context *)dev->context;

    ggml_mlu_set_device(dev_ctx->device);

    cnrtNotifier_t notifier;
    CNRT_CHECK(cnrtNotifierCreate(&notifier));

    return new ggml_backend_event {
        /* .device = */ dev,
        /* .context = */ notifier,
    };
}

static void ggml_backend_mlu_device_event_free(ggml_backend_dev_t dev, ggml_backend_event_t event) {
    GGML_UNUSED(dev);

    CNRT_CHECK(cnrtNotifierDestroy((cnrtNotifier_t)event->context));
    delete event;
}

static void ggml_backend_mlu_device_event_synchronize(ggml_backend_dev_t dev, ggml_backend_event_t event) {
    GGML_UNUSED(dev);
    CNRT_CHECK(cnrtWaitNotifier((cnrtNotifier_t)event->context));
}

static const ggml_backend_device_i ggml_backend_mlu_device_interface = {
    /* .get_name                = */ ggml_backend_mlu_device_get_name,
    /* .get_description         = */ ggml_backend_mlu_device_get_description,
    /* .get_memory              = */ ggml_backend_mlu_device_get_memory,
    /* .get_type                = */ ggml_backend_mlu_device_get_type,
    /* .get_props               = */ ggml_backend_mlu_device_get_props,
    /* .init_backend            = */ ggml_backend_mlu_device_init_backend,
    /* .get_buffer_type         = */ ggml_backend_mlu_device_get_buffer_type,
    /* .get_host_buffer_type    = */ ggml_backend_mlu_device_get_host_buffer_type,
    /* .buffer_from_host_ptr    = */ NULL, // 不支持 MLU
    /* .supports_op             = */ ggml_backend_mlu_device_supports_op,
    /* .supports_buft           = */ ggml_backend_mlu_supports_buft,
    /* .offload_op              = */ ggml_backend_mlu_device_offload_op,
    /* .event_new               = */ ggml_backend_mlu_device_event_new,
    /* .event_free              = */ ggml_backend_mlu_device_event_free,
    /* .event_synchronize       = */ ggml_backend_mlu_device_event_synchronize,
};

//
// 后端注册
//

struct ggml_backend_mlu_reg_context {
    std::vector<ggml_backend_dev_t> devices;
};

static const char * ggml_backend_mlu_reg_get_name(ggml_backend_reg_t reg) {
    GGML_UNUSED(reg);
    return GGML_MLU_NAME;
}

static size_t ggml_backend_mlu_reg_get_device_count(ggml_backend_reg_t reg) {
    ggml_backend_mlu_reg_context * ctx = (ggml_backend_mlu_reg_context *)reg->context;
    return ctx->devices.size();
}

static ggml_backend_dev_t ggml_backend_mlu_reg_get_device(ggml_backend_reg_t reg, size_t index) {
    ggml_backend_mlu_reg_context * ctx = (ggml_backend_mlu_reg_context *)reg->context;
    GGML_ASSERT(index < ctx->devices.size());
    return ctx->devices[index];
}

static void * ggml_backend_mlu_reg_get_proc_address(ggml_backend_reg_t reg, const char * name) {
    GGML_UNUSED(reg);
    GGML_UNUSED(name);
    // 保留供将来使用
    return nullptr;
}

static const ggml_backend_reg_i ggml_backend_mlu_reg_interface = {
    /* .get_name          = */ ggml_backend_mlu_reg_get_name,
    /* .get_device_count  = */ ggml_backend_mlu_reg_get_device_count,
    /* .get_device        = */ ggml_backend_mlu_reg_get_device,
    /* .get_proc_address  = */ ggml_backend_mlu_reg_get_proc_address,
};

// 后端注册，仅为 MLU 后端调用一次
ggml_backend_reg_t ggml_backend_mlu_reg() {
    static ggml_backend_reg reg;
    static bool initialized = false;

    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        if (!initialized) {
            ggml_backend_mlu_reg_context * ctx = new ggml_backend_mlu_reg_context;
            // CNDEV_CHECK(cndevInit(0));

            for (int i = 0; i < ggml_mlu_info().device_count; i++) {
                ggml_backend_mlu_device_context* dev_ctx = new ggml_backend_mlu_device_context();
                dev_ctx->device = i;
                dev_ctx->name = GGML_MLU_NAME + std::to_string(i);
                
                char description[128];
                ggml_backend_mlu_get_device_description(i, description, sizeof(description));
                dev_ctx->description = description;
                
                ggml_mlu_set_device(i);
                ggml_backend_dev_t dev = new ggml_backend_device {
                    /* .iface   = */ ggml_backend_mlu_device_interface,
                    /* .reg     = */ &reg,
                    /* .context = */ dev_ctx
                };
                ctx->devices.push_back(dev);
            }

            reg = ggml_backend_reg {
                /* .api_version = */ GGML_BACKEND_API_VERSION,
                /* .iface       = */ ggml_backend_mlu_reg_interface,
                /* .context     = */ ctx
            };
        }

        initialized = true;
    }

    return &reg;
}

ggml_backend_t ggml_backend_mlu_init(int32_t device) {
    if (device < 0 || device >= ggml_backend_mlu_get_device_count()) {
        GGML_LOG_ERROR("%s: error: invalid device %d\n", __func__, device);
        return nullptr;
    }

    ggml_backend_mlu_context* ctx = new ggml_backend_mlu_context(device);
    if (ctx == nullptr) {
        GGML_LOG_ERROR("%s: error: failed to allocate context\n", __func__);
        return nullptr;
    }
    ggml_mlu_set_device(ctx->device);
    ggml_backend_t mlu_backend =
        new ggml_backend{/* .guid      = */ ggml_backend_mlu_guid(),
                         /* .interface = */ ggml_backend_mlu_interface,
                         /* .device    = */ ggml_backend_reg_dev_get(ggml_backend_mlu_reg(), device),
                         /* .context   = */ ctx};

    return mlu_backend;
}

bool ggml_backend_is_mlu(ggml_backend_t backend) {
    return backend != NULL &&
           ggml_guid_matches(backend->guid, ggml_backend_mlu_guid());
}

int32_t ggml_backend_mlu_get_device_count() {
    return ggml_mlu_info().device_count;
}

void ggml_backend_mlu_get_device_description(int32_t device, char* description, size_t description_size) {
    ggml_mlu_set_device(device);
    const char* card_name_string = cndevGetCardNameStringByDevId(device);
    snprintf(description, description_size, "%s", card_name_string);
}

void ggml_backend_mlu_get_device_memory(int32_t device, size_t* free, size_t* total) {
    ggml_mlu_set_device(device);
    CNRT_CHECK(cnrtMemGetInfo(free, total));
}

bool ggml_backend_buffer_is_mlu_split(ggml_backend_buffer_t buffer) {
    return buffer && ggml_backend_buft_is_mlu_split(buffer->buft);
}

std::unique_ptr<ggml_mlu_pool> ggml_backend_mlu_context::new_pool_for_device(int device) {
    return std::make_unique<ggml_mlu_pool>(device);
}

GGML_BACKEND_DL_IMPL(ggml_backend_mlu_reg)
