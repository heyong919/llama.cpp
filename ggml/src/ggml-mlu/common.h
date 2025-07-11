#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

#include "mlu_common.h"
#include "ggml.h"
#include "ggml-mlu.h"

#define GGML_MLU_NAME "MLU"
#define GGML_MLU_MAX_DEVICES 16
#define MATRIX_ROW_PADDING 512

struct ggml_tensor_extra_mlu {
    void * data_device[GGML_MLU_MAX_DEVICES]; // 1 pointer for each device for split tensors
};

struct ggml_mlu_pool {
    ggml_mlu_pool() : ggml_mlu_pool(0) {}
    ggml_mlu_pool(int device_id) {
        props.type = cnrtMemAllocationTypeDefault;
        props.requestedHandleTypes = cnrtMemHandleTypeNone;
        props.location.type = cnrtMemLocationTypeDevice;
        props.location.id = device_id;
        props.maxSize = 1 << 30;
        cnrtMemPoolCreate(&pool, &props);
        int enable = false;
        set_attr(cnrtMemPoolAttrReuseAllowInternalDependencies, &enable);
    }
    ~ggml_mlu_pool() {
        cnrtMemPoolDestroy(pool);
    }

    void set_device(int device_id) {
        props.location.id = device_id;
        cnrtMemPoolCreate(&pool, &props);
    }

    void * alloc(size_t size, cnrtQueue_t queue) {
        void * ptr = nullptr;
        CNRT_CHECK(cnrtMemAllocFromPoolAsync(&ptr, size, pool, queue));
        return ptr;
    }

    void free(void * ptr) {
        CNRT_CHECK(cnrtFree(ptr));
    }

    void set_attr(cnrtMemPoolAttr_t attr, void * value) {
        CNRT_CHECK(cnrtMemPoolSetAttribute(pool, attr, value));
    }

    void get_attr(cnrtMemPoolAttr_t attr, void * value) {
        CNRT_CHECK(cnrtMemPoolGetAttribute(pool, attr, value));
    }

    cnrtMemPoolProps_t props;
    cnrtMemPool_t pool;
};

template<typename T>
struct ggml_mlu_pool_alloc {
    ggml_mlu_pool * pool = nullptr;
    T * ptr = nullptr;
    size_t actual_size = 0;

    ggml_mlu_pool_alloc() = default;

    ggml_mlu_pool_alloc(ggml_mlu_pool & pool) : pool(&pool) {
    }

    ggml_mlu_pool_alloc(ggml_mlu_pool & pool, size_t size) : pool(&pool) {
        alloc(size);
    }

    ~ggml_mlu_pool_alloc() {
        if (ptr != nullptr) {
            pool->free(ptr);
        }
    }

    // size is in number of elements
    T * alloc(size_t size, cnrtQueue_t queue) {
        GGML_ASSERT(pool != nullptr);
        GGML_ASSERT(ptr == nullptr);
        ptr = (T *) pool->alloc(size * sizeof(T), queue);
        return ptr;
    }

    T * alloc(size_t size, cnnlHandle_t cnnl_handle) {
        cnrtQueue_t queue = nullptr;
        cnnlGetQueue(cnnl_handle, &queue);
        alloc(size, queue);
    }

    T * get() {
        return ptr;
    }

    ggml_mlu_pool_alloc(const ggml_mlu_pool_alloc &) = delete;
    ggml_mlu_pool_alloc(ggml_mlu_pool_alloc &&) = delete;
    ggml_mlu_pool_alloc& operator=(const ggml_mlu_pool_alloc &) = delete;
    ggml_mlu_pool_alloc& operator=(ggml_mlu_pool_alloc &&) = delete;
};

struct ggml_backend_mlu_context {
    int device;
    std::string name;

    cnrtQueue_t queues[GGML_MLU_MAX_DEVICES] = { nullptr };
    cnnlHandle_t cnnl_handles[GGML_MLU_MAX_DEVICES] = {nullptr};

    // std::unique_ptr<ggml_mlu_graph> mlu_graph;

    explicit ggml_backend_mlu_context(int device) :
        device(device),
        name(GGML_MLU_NAME + std::to_string(device)) {
    }

    ~ggml_backend_mlu_context() {
        for (int i = 0; i < GGML_MLU_MAX_DEVICES; ++i) {
            if (queues[i] != nullptr) {
                CNRT_CHECK(cnrtQueueDestroy(queues[i]));
            }
            if (cnnl_handles[i] != nullptr) {
                CNNL_CHECK(cnnlDestroy(cnnl_handles[i]));
            }
        }
    }

    cnrtQueue_t queue(int device) {
        if (queues[device] == nullptr) {
            // ggml_mlu_set_device(device);
            CNRT_CHECK(cnrtQueueCreate(&queues[device]));
        }
        return queues[device];
    }

    cnrtQueue_t queue() {
        return queue(device);
    }

    cnnlHandle_t cnnl_handle(int device) {
        if (cnnl_handles[device] == nullptr) {
            // ggml_mlu_set_device(device);
            CNNL_CHECK(cnnlCreate(&cnnl_handles[device]));
        }
        return cnnl_handles[device];
    }

    cnnlHandle_t cnnl_handle() {
        return cnnl_handle(device);
    }

    // pool
    std::unique_ptr<ggml_mlu_pool> pools[GGML_MLU_MAX_DEVICES];

    static std::unique_ptr<ggml_mlu_pool> new_pool_for_device(int device);

    ggml_mlu_pool & pool(int device) {
        if (pools[device] == nullptr) {
            pools[device] = new_pool_for_device(device);
        }
        return *pools[device];
    }

    ggml_mlu_pool & pool() {
        return pool(device);
    }
};