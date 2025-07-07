#pragma once

#include "common.h"

class CnnlTensorDesc final {
    public:
    CnnlTensorDesc() {
        CNNL_CHECK(cnnlCreateTensorDescriptor(&desc_));
    }
    ~CnnlTensorDesc() {
        CNNL_CHECK(cnnlDestroyTensorDescriptor(desc_));
    }
    void Set(const cnnlTensorLayout_t layout, const cnnlDataType_t dtype,
            const int ndims, const int64_t * dims) {
        CNNL_CHECK(cnnlSetTensorDescriptor_v2(desc_, layout, dtype, ndims, dims));
    }
    void SetEx(const cnnlTensorLayout_t layout, const cnnlDataType_t dtype,
            const int ndims, const int64_t * dims, const int64_t * strides) {
        CNNL_CHECK(cnnlSetTensorDescriptorEx_v2(desc_, layout, dtype, ndims, dims, strides));
    }
    cnnlTensorDescriptor_t desc() const { return desc_; }

    private:
    cnnlTensorDescriptor_t desc_;
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
void ggml_mlu_op_mul_mat(ggml_backend_mlu_context & ctx, const ggml_tensor * src0, const ggml_tensor * src1, ggml_tensor * dst);
void ggml_mlu_op_mat_mul(ggml_backend_mlu_context & ctx, ggml_tensor * dst);
