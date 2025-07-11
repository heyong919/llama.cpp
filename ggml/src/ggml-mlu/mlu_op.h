#pragma once

#include "ggml.h"
#include "common.h"

cnnlDataType_t ggml_type_to_cnnl_type(ggml_type type);

#define GGML_MLU_BINARY_OP_LOCALS \
    const ggml_tensor * src0 = dst->src[0]; \
    const ggml_tensor * src1 = dst->src[1]; \
    const void * src0_d = src0->data; \
    const void * src1_d = src1->data; \
    void * dst_d = dst->data;

#define GGML_MLU_UNARY_OP_LOCALS \
    const ggml_tensor * src = dst->src[0]; \
    const void * src_d = src->data; \
    void * dst_d = dst->data;


class CnnlTensorDesc final {
    public:
    CnnlTensorDesc() {
        CNNL_CHECK(cnnlCreateTensorDescriptor(&desc_));
    }
    CnnlTensorDesc(const ggml_tensor * tensor) {
        CNNL_CHECK(cnnlCreateTensorDescriptor(&desc_));
        SetEx(tensor);
    }
    ~CnnlTensorDesc() {
        CNNL_CHECK(cnnlDestroyTensorDescriptor(desc_));
    }
    void SetEx(const ggml_tensor * tensor) {
        int ndim = GGML_MAX_DIMS;
        while (tensor->ne[ndim-1] == 1 && ndim > 1) ndim--;
        shape_.assign(tensor->ne, tensor->ne + ndim);
        std::reverse(shape_.begin(), shape_.end());
        strides_.resize(ndim);
        for (int i = 0; i < ndim; i++) {
            strides_[i] = tensor->nb[ndim-i-1] / ggml_type_size(tensor->type);
        }
        SetEx(ggml_type_to_cnnl_type(tensor->type), ndim, shape_.data(), strides_.data());
    }
    void Set(const cnnlDataType_t dtype, const int ndims, const int64_t * dims,
                const cnnlTensorLayout_t layout = CNNL_LAYOUT_ARRAY) {
        CNNL_CHECK(cnnlSetTensorDescriptor_v2(desc_, layout, dtype, ndims, dims));
    }
    void SetEx(const cnnlDataType_t dtype, const int ndims, const int64_t * dims,
                const int64_t * strides, const cnnlTensorLayout_t layout = CNNL_LAYOUT_ARRAY) {
        CNNL_CHECK(cnnlSetTensorDescriptorEx_v2(desc_, layout, dtype, ndims, dims, strides));
    }

    cnnlTensorDescriptor_t desc() const { return desc_; }
    cnnlDataType_t dtype() const { return dtype_; }
    const std::vector<int64_t> & shape() const { return shape_; }
    const std::vector<int64_t> & strides() const { return strides_; }

    private:
    cnnlTensorDescriptor_t desc_;
    cnnlDataType_t dtype_;
    std::vector<int64_t> shape_;
    std::vector<int64_t> strides_;
};

cnnlDataType_t ggml_type_to_cnnl_type(ggml_type type);

bool ggml_mlu_forward_impl(ggml_backend_mlu_context & ctx, struct ggml_tensor * dst);

// MLU operation function declarations
void ggml_mlu_op_get_rows(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_dup(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_cpy(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_add(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_sub(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_mul(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_div(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_gelu(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_norm(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_abs(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_sgn(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_neg(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_rms_norm(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_sqrt(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_soft_max(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_rope(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
void ggml_mlu_op_mul_mat(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
