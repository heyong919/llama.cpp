#include "ggml.h"
#include "common.h"
#include "mlu_op.h"
#include "mlu_include/cnrt.h"
#include "mlu_include/cnnl.h"

cnnlDataType_t ggml_type_to_cnnl_type(ggml_type type) {
    switch (type) {
        case GGML_TYPE_F32:
            return CNNL_DTYPE_FLOAT;
        case GGML_TYPE_F16:
            return CNNL_DTYPE_HALF;
        case GGML_TYPE_I32:
            return CNNL_DTYPE_INT32;
        case GGML_TYPE_I64:
            return CNNL_DTYPE_INT64;
        default:
            GGML_ASSERT(false);
            return CNNL_DTYPE_INVALID;
    }
}

#if 1
bool ggml_mlu_forward_impl(ggml_backend_mlu_context & ctx, struct ggml_tensor * dst) {
// op=ADD 
// op=CONT 
// op=CPY 
// op=GET_ROWS 
// op=MUL 
// op=MUL_MAT 
// op=PERMUTE 
// op=RESHAPE 
// op=RMS_NORM 
// op=ROPE 
// op=SOFT_MAX 
// op=TRANSPOSE 
// op=UNARY 
// op=VIEW
    switch (dst->op) {
        // case GGML_OP_ARGMAX:
        //     ggml_mlu_argmax(ctx, dst);
        //     break;
        // case GGML_OP_COUNT_EQUAL:
        //     ggml_mlu_count_equal(ctx, dst);
        //     break;
        // case GGML_OP_REPEAT:
        //     ggml_mlu_op_repeat(ctx, dst);
        //     break;
        // case GGML_OP_REPEAT_BACK:
        //     ggml_mlu_op_repeat_back(ctx, dst);
        //     break;
        case GGML_OP_GET_ROWS:
            ggml_mlu_op_get_rows(ctx, dst);
            break;
        // case GGML_OP_GET_ROWS_BACK:
        //     ggml_mlu_op_get_rows_back(ctx, dst);
        //     break;
        case GGML_OP_DUP:
            ggml_mlu_op_dup(ctx, dst);
            break;
        case GGML_OP_CPY:
            ggml_mlu_op_cpy(ctx, dst);
            break;
        case GGML_OP_CONT:
            ggml_mlu_op_dup(ctx, dst);
            break;
        case GGML_OP_ADD:
        case GGML_OP_ADD1: // TODO: more efficient implementation
            ggml_mlu_op_add(ctx, dst);
            break;
        case GGML_OP_SUB:
            ggml_mlu_op_sub(ctx, dst);
            break;
        // case GGML_OP_ACC:
        //     ggml_mlu_op_acc(ctx, dst);
        //     break;
        case GGML_OP_MUL:
            ggml_mlu_op_mul(ctx, dst);
            break;
        case GGML_OP_DIV:
            ggml_mlu_op_div(ctx, dst);
            break;
        case GGML_OP_UNARY:
            switch (ggml_get_unary_op(dst)) {
                // case GGML_UNARY_OP_ABS:
                //     ggml_mlu_op_abs(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_SGN:
                //     ggml_mlu_op_sgn(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_NEG:
                //     ggml_mlu_op_neg(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_STEP:
                //     ggml_mlu_op_step(ctx, dst);
                //     break;
                case GGML_UNARY_OP_GELU:
                    ggml_mlu_op_gelu(ctx, dst);
                    break;
                // case GGML_UNARY_OP_SILU:
                //     ggml_mlu_op_silu(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_GELU_ERF:
                //     ggml_mlu_op_gelu_erf(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_GELU_QUICK:
                //     ggml_mlu_op_gelu_quick(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_TANH:
                //     ggml_mlu_op_tanh(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_RELU:
                //     ggml_mlu_op_relu(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_SIGMOID:
                //     ggml_mlu_op_sigmoid(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_HARDSIGMOID:
                //     ggml_mlu_op_hardsigmoid(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_HARDSWISH:
                //     ggml_mlu_op_hardswish(ctx, dst);
                //     break;
                // case GGML_UNARY_OP_EXP:
                //     ggml_mlu_op_exp(ctx, dst);
                //     break;
                default:
                    return false;
            }
            break;
        case GGML_OP_NORM:
            ggml_mlu_op_norm(ctx, dst);
            break;
        // case GGML_OP_GROUP_NORM:
        //     ggml_mlu_op_group_norm(ctx, dst);
        //     break;
        // case GGML_OP_L2_NORM:
        //     ggml_mlu_op_l2_norm(ctx, dst);
        //     break;
        // case GGML_OP_CONCAT:
        //     ggml_mlu_op_concat(ctx, dst);
        //     break;
        // case GGML_OP_UPSCALE:
        //     ggml_mlu_op_upscale(ctx, dst);
        //     break;
        // case GGML_OP_PAD:
        //     ggml_mlu_op_pad(ctx, dst);
        //     break;
        // case GGML_OP_ARANGE:
        //     ggml_mlu_op_arange(ctx, dst);
        //     break;
        // case GGML_OP_TIMESTEP_EMBEDDING:
        //     ggml_mlu_op_timestep_embedding(ctx, dst);
        //     break;
        // case GGML_OP_LEAKY_RELU:
        //     ggml_mlu_op_leaky_relu(ctx, dst);
        //     break;
        // case GGML_OP_SILU_BACK:
        //     ggml_mlu_op_silu_back(ctx, dst);
        //     break;
        case GGML_OP_RMS_NORM:
            ggml_mlu_op_rms_norm(ctx, dst);
            break;
        // case GGML_OP_RMS_NORM_BACK:
        //     ggml_mlu_op_rms_norm_back(ctx, dst);
        //     break;
        case GGML_OP_MUL_MAT:
            ggml_mlu_op_mul_mat(ctx, dst->src[0], dst->src[1], dst);
            break;
        // case GGML_OP_MUL_MAT_ID:
        //     ggml_mlu_mul_mat_id(ctx, dst);
        //     break;
        // case GGML_OP_OUT_PROD:
        //     ggml_mlu_out_prod(ctx, dst);
        //     break;
        // case GGML_OP_SCALE:
        //     ggml_mlu_op_scale(ctx, dst);
        //     break;
        // case GGML_OP_SQR:
        //     ggml_mlu_op_sqr(ctx, dst);
        //     break;
        case GGML_OP_SQRT:
            ggml_mlu_op_sqrt(ctx, dst);
            break;
        // case GGML_OP_SIN:
        //     ggml_mlu_op_sin(ctx, dst);
        //     break;
        // case GGML_OP_COS:
        //     ggml_mlu_op_cos(ctx, dst);
        //     break;
        // case GGML_OP_CLAMP:
        //     ggml_mlu_op_clamp(ctx, dst);
        //     break;
        // case GGML_OP_LOG:
        //     ggml_mlu_op_log(ctx, dst);
        //     break;
        case GGML_OP_NONE:
        case GGML_OP_RESHAPE:
        case GGML_OP_VIEW:
        case GGML_OP_PERMUTE:
        case GGML_OP_TRANSPOSE:
                break;
        // case GGML_OP_DIAG_MASK_INF:
        //     ggml_mlu_op_diag_mask_inf(ctx, dst);
        //     break;
        case GGML_OP_SOFT_MAX:
            ggml_mlu_op_soft_max(ctx, dst);
            break;
        // case GGML_OP_SOFT_MAX_BACK:
        //     ggml_mlu_op_soft_max_back(ctx, dst);
        //     break;
        case GGML_OP_ROPE:
            ggml_mlu_op_rope(ctx, dst);
            break;
        // case GGML_OP_ROPE_BACK:
        //     ggml_mlu_op_rope_back(ctx, dst);
        //     break;
        // case GGML_OP_IM2COL:
        //     ggml_mlu_op_im2col(ctx, dst);
        //     break;
        // case GGML_OP_CONV_TRANSPOSE_1D:
        //     ggml_mlu_op_conv_transpose_1d(ctx,dst);
        //     break;
        // case GGML_OP_POOL_2D:
        //     ggml_mlu_op_pool2d(ctx, dst);
        //     break;
        // case GGML_OP_SUM:
        //     ggml_mlu_op_sum(ctx, dst);
        //     break;
        // case GGML_OP_SUM_ROWS:
        //     ggml_mlu_op_sum_rows(ctx, dst);
        //     break;
        // case GGML_OP_SSM_CONV:
        //     ggml_mlu_op_ssm_conv(ctx, dst);
        //     break;
        // case GGML_OP_SSM_SCAN:
        //     ggml_mlu_op_ssm_scan(ctx, dst);
        //     break;
        // case GGML_OP_ARGSORT:
        //     ggml_mlu_op_argsort(ctx, dst);
        //     break;
        case GGML_OP_FLASH_ATTN_EXT:
            // ggml_mlu_flash_attn_ext(ctx, dst);
            break;
        // case GGML_OP_GATED_LINEAR_ATTN:
        //     ggml_mlu_op_gated_linear_attn(ctx, dst);
        //     break;
        default:
            return false;
    }
    return true;
}
#else
bool ggml_mlu_forward_impl(ggml_backend_mlu_context & ctx, struct ggml_tensor * dst) { return true; }
#endif

void ggml_mlu_op_sqrt(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const void * src0_d = src0->data;
    void * dst_d = dst->data;

    GGML_ASSERT(ggml_is_contiguous(src0));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == dst->type);

    CnnlTensorDesc x_desc, y_desc;
    auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
    int64_t dims[] = { ggml_nelements(dst) };
    x_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);
    y_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);

    CNNL_CHECK(cnnlSqrt_v2(ctx.cnnl_handle(),
                    cnnlComputationPreference_t::CNNL_COMPUTATION_HIGH_PRECISION,
                    x_desc.desc(), src0_d,
                    y_desc.desc(), dst_d));
}

void ggml_mlu_op_get_rows(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
}

void ggml_mlu_op_add(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];
    const void * src0_d = src0->data;
    const void * src1_d = src1->data;
    void * dst_d = dst->data;

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    CnnlTensorDesc x_desc, y_desc, z_desc;
    auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
    // int64_t dims[] = { ggml_nelements(dst) };
    
    x_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);
    y_desc.Set(layout_array, ggml_type_to_cnnl_type(src1->type), GGML_MAX_DIMS, src1->ne);
    z_desc.Set(layout_array, ggml_type_to_cnnl_type(dst->type), GGML_MAX_DIMS, dst->ne);

    // Create and set OpTensor descriptor for ADD operation
    cnnlOpTensorDescriptor_t op_tensor_desc;
    CNNL_CHECK(cnnlCreateOpTensorDescriptor(&op_tensor_desc));
    CNNL_CHECK(cnnlSetOpTensorDescriptor(op_tensor_desc, CNNL_OP_TENSOR_ADD, ggml_type_to_cnnl_type(src0->type), CNNL_NOT_PROPAGATE_NAN));

    const float alpha1 = 1.0f, alpha2 = 1.0f, beta = 0.0f;
    CNNL_CHECK(cnnlOpTensor(ctx.cnnl_handle(), op_tensor_desc,
                           &alpha1, x_desc.desc(), src0_d,
                           &alpha2, y_desc.desc(), src1_d,
                           nullptr, 0,
                           &beta, z_desc.desc(), dst_d));

    CNNL_CHECK(cnnlDestroyOpTensorDescriptor(op_tensor_desc));
}

void ggml_mlu_op_sub(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];
    const void * src0_d = src0->data;
    const void * src1_d = src1->data;
    void * dst_d = dst->data;

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    CnnlTensorDesc x_desc, y_desc, z_desc;
    auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
    int64_t dims[] = { ggml_nelements(dst) };
    
    x_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);
    y_desc.Set(layout_array, ggml_type_to_cnnl_type(src1->type), GGML_MAX_DIMS, src1->ne);
    z_desc.Set(layout_array, ggml_type_to_cnnl_type(dst->type), GGML_MAX_DIMS, dst->ne);

    // Create and set OpTensor descriptor for SUB operation
    cnnlOpTensorDescriptor_t op_tensor_desc;
    CNNL_CHECK(cnnlCreateOpTensorDescriptor(&op_tensor_desc));
    CNNL_CHECK(cnnlSetOpTensorDescriptor(op_tensor_desc, CNNL_OP_TENSOR_SUB, ggml_type_to_cnnl_type(src0->type), CNNL_NOT_PROPAGATE_NAN));

    const float alpha1 = 1.0f, alpha2 = 1.0f, beta = 0.0f;
    CNNL_CHECK(cnnlOpTensor(ctx.cnnl_handle(), op_tensor_desc,
                           &alpha1, x_desc.desc(), src0_d,
                           &alpha2, y_desc.desc(), src1_d,
                           nullptr, 0,
                           &beta, z_desc.desc(), dst_d));

    CNNL_CHECK(cnnlDestroyOpTensorDescriptor(op_tensor_desc));
}

void ggml_mlu_op_mul(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];
    const void * src0_d = src0->data;
    const void * src1_d = src1->data;
    void * dst_d = dst->data;

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    CnnlTensorDesc x_desc, y_desc, z_desc;
    auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
    int64_t dims[] = { ggml_nelements(dst) };
    
    x_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);
    y_desc.Set(layout_array, ggml_type_to_cnnl_type(src1->type), GGML_MAX_DIMS, src1->ne);
    z_desc.Set(layout_array, ggml_type_to_cnnl_type(dst->type), GGML_MAX_DIMS, dst->ne);

    // Create and set OpTensor descriptor for MUL operation
    cnnlOpTensorDescriptor_t op_tensor_desc;
    CNNL_CHECK(cnnlCreateOpTensorDescriptor(&op_tensor_desc));
    CNNL_CHECK(cnnlSetOpTensorDescriptor(op_tensor_desc, CNNL_OP_TENSOR_MUL, ggml_type_to_cnnl_type(src0->type), CNNL_NOT_PROPAGATE_NAN));

    const float alpha1 = 1.0f, alpha2 = 1.0f, beta = 0.0f;
    CNNL_CHECK(cnnlOpTensor(ctx.cnnl_handle(), op_tensor_desc,
                           &alpha1, x_desc.desc(), src0_d,
                           &alpha2, y_desc.desc(), src1_d,
                           nullptr, 0,
                           &beta, z_desc.desc(), dst_d));

    CNNL_CHECK(cnnlDestroyOpTensorDescriptor(op_tensor_desc));
}

void ggml_mlu_op_div(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];
    const void * src0_d = src0->data;
    const void * src1_d = src1->data;
    void * dst_d = dst->data;

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    CnnlTensorDesc x_desc, y_desc, z_desc;
    auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
    int64_t dims[] = { ggml_nelements(dst) };
    
    x_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);
    y_desc.Set(layout_array, ggml_type_to_cnnl_type(src1->type), GGML_MAX_DIMS, src1->ne);
    z_desc.Set(layout_array, ggml_type_to_cnnl_type(dst->type), GGML_MAX_DIMS, dst->ne);

    // Create and set Div descriptor for DIV operation
    cnnlDivDescriptor_t div_desc;
    CNNL_CHECK(cnnlCreateDivDescriptor(&div_desc));
    cnnlDivMode_t div_mode = CNNL_DIV_TRUE;
    CNNL_CHECK(cnnlSetDivDescAttr(div_desc, CNNL_DIV_MODE, &div_mode, sizeof(div_mode)));

    // Get workspace size and allocate workspace if needed
    size_t workspace_size = 0;
    CNNL_CHECK(cnnlGetDivWorkspaceSize_v2(ctx.cnnl_handle(), div_desc,
                                         x_desc.desc(), y_desc.desc(), z_desc.desc(),
                                         &workspace_size));
    
    ggml_mlu_pool_alloc<char> mlu_alloc(ctx.pool());
    void* workspace = nullptr;
    if (workspace_size > 0) {
        workspace = mlu_alloc.alloc(workspace_size, ctx.queue());
    }

    CNNL_CHECK(cnnlDiv_v3(ctx.cnnl_handle(), div_desc,
                         x_desc.desc(), src0_d,
                         y_desc.desc(), src1_d,
                         workspace, workspace_size,
                         z_desc.desc(), dst_d));
    CNNL_CHECK(cnnlDestroyDivDescriptor(div_desc));
}

void ggml_mlu_op_gelu(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
}

void ggml_mlu_op_norm(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
}

void ggml_mlu_op_rms_norm(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const void * src0_d = src0->data;
    void * dst_d = dst->data;
    cnnlHandle_t handle = ctx.cnnl_handle();

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->type == GGML_TYPE_F32);

    // Get epsilon parameter from dst->op_params
    float eps;
    memcpy(&eps, dst->op_params, sizeof(float));
    GGML_ASSERT(eps >= 0.0f);

    // Create tensor descriptors
    CnnlTensorDesc src_desc, dst_desc;
    src_desc.Set(CNNL_LAYOUT_ARRAY, ggml_type_to_cnnl_type(src0->type), GGML_MAX_DIMS, src0->ne);
    dst_desc.Set(CNNL_LAYOUT_ARRAY, ggml_type_to_cnnl_type(dst->type), GGML_MAX_DIMS, dst->ne);

    // Create FuseNorm descriptor
    cnnlFuseNormDescriptor_t fuse_norm_desc;
    CNNL_CHECK(cnnlCreateFuseNormDescriptor(&fuse_norm_desc));
    
    // Set FuseNorm descriptor parameters
    // For RMS norm: no bias, no residual, no norm_bias, no norm_scale
    CNNL_CHECK(cnnlSetFuseNormDescriptor(
        fuse_norm_desc,
        eps,                              // eps
        1.0f,                            // scale (not used for RMS norm)
        false,                           // has_norm_scale
        false,                           // has_norm_bias  
        false,                           // has_bias
        false,                           // has_residual
        false,                           // store_output_before_norm
        CNNL_DTYPE_FLOAT,               // math_pre
        CNNL_TRANSFORMER_RMSNORM        // mode
    ));

    // Get workspace size
    size_t workspace_size;
    CNNL_CHECK(cnnlGetFuseNormWorkspaceSize(
        handle,
        fuse_norm_desc,
        src_desc.desc(),
        &workspace_size
    ));

    // Allocate workspace
    ggml_mlu_pool_alloc<char> mlu_alloc(ctx.pool());
    void* workspace = nullptr;
    if (workspace_size > 0) {
        workspace = mlu_alloc.alloc(workspace_size, ctx.queue());
    }

    // Call cnnlFuseNorm_v3
    CNNL_CHECK(cnnlFuseNorm_v3(
        handle,
        src_desc.desc(),                 // input_desc
        src0_d,                          // input
        nullptr,                         // input_scale_desc (not used)
        nullptr,                         // input_scale (not used)
        nullptr,                         // norm_scale_desc (not used)
        nullptr,                         // norm_scale (not used)
        nullptr,                         // norm_bias_desc (not used)
        nullptr,                         // norm_bias (not used)
        nullptr,                         // residual_desc (not used)
        nullptr,                         // residual (not used)
        nullptr,                         // bias_desc (not used)
        nullptr,                         // bias (not used)
        eps,                             // eps
        CNNL_QUANTIZE_NONE,             // quant_scheme (not used)
        false,                           // store_output_before_norm
        CNNL_TRANSFORMER_RMSNORM,       // norm_type
        CNNL_DTYPE_FLOAT,               // math_pre
        workspace,                       // workspace
        workspace_size,                  // workspace_size
        dst_desc.desc(),                 // output_desc
        dst_d,                           // output
        nullptr,                         // output_before_norm_desc (not used)
        nullptr,                         // output_before_norm (not used)
        nullptr,                         // output_quant_scale_desc (not used)
        nullptr                          // output_quant_scale (not used)
    ));

    // Destroy descriptor
    CNNL_CHECK(cnnlDestroyFuseNormDescriptor(fuse_norm_desc));
}

void ggml_mlu_op_mul_mat(ggml_backend_mlu_context & ctx, const ggml_tensor * src0, const ggml_tensor * src1, ggml_tensor * dst) {
    (void)ctx;
    (void)src0;
    (void)src1;
    (void)dst;
    // TODO: implement mul_mat operation
}

void ggml_mlu_op_soft_max(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
    // TODO: implement soft_max operation
}

void ggml_mlu_op_rope(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
    // TODO: implement rope operation
}

void ggml_mlu_op_dup(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const void * src0_d = src0->data;
    void * dst_d = dst->data;
    cnrtQueue_t queue = ctx.queue();

    GGML_ASSERT(ggml_nelements(src0) == ggml_nelements(dst));
    GGML_ASSERT(src0->type == dst->type);

    // If both tensors are contiguous and have the same type, use simple memory copy
    if (ggml_is_contiguous(src0) && ggml_is_contiguous(dst)) {
        GGML_ASSERT(ggml_nbytes(src0) == ggml_nbytes(dst));
        CNRT_CHECK(cnrtMemcpyAsync(dst_d, const_cast<void*>(src0_d), ggml_nbytes(src0), queue, cnrtMemcpyDevToDev));
        return;
    }

    // For non-contiguous tensors, use CNNL copy operation
    CnnlTensorDesc src_desc, dst_desc;
    auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
    
    // Set up tensor descriptors
    int64_t src_dims[GGML_MAX_DIMS];
    int64_t dst_dims[GGML_MAX_DIMS];
    int src_ndim = 0, dst_ndim = 0;
    
    // Convert GGML dimensions to CNNL format
    for (int i = 0; i < GGML_MAX_DIMS; i++) {
        if (src0->ne[i] > 1 || i == 0) {
            src_dims[src_ndim++] = src0->ne[i];
        }
        if (dst->ne[i] > 1 || i == 0) {
            dst_dims[dst_ndim++] = dst->ne[i];
        }
    }
    
    // Ensure at least 1D
    if (src_ndim == 0) {
        src_dims[0] = 1;
        src_ndim = 1;
    }
    if (dst_ndim == 0) {
        dst_dims[0] = 1;
        dst_ndim = 1;
    }

    src_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), src_ndim, src_dims);
    dst_desc.Set(layout_array, ggml_type_to_cnnl_type(dst->type), dst_ndim, dst_dims);

    // Get workspace size and allocate workspace
    size_t workspace_size = 0;
    CNNL_CHECK(cnnlGetCopyWorkspaceSize(ctx.cnnl_handle(), src_desc.desc(), dst_desc.desc(), &workspace_size));
    
    ggml_mlu_pool_alloc<char> mlu_alloc(ctx.pool());
    void* workspace = nullptr;
    if (workspace_size > 0) {
        workspace = mlu_alloc.alloc(workspace_size, ctx.queue());
    }

    // Perform the copy operation
    CNNL_CHECK(cnnlCopy_v2(ctx.cnnl_handle(),
                          src_desc.desc(), src0_d,
                          dst_desc.desc(), dst_d,
                          workspace, workspace_size));
}

void ggml_mlu_op_cpy(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    ggml_tensor * src1 = dst->src[1];
    
    const int64_t ne = ggml_nelements(src0);
    GGML_ASSERT(ne == ggml_nelements(src1));
    
    // For contiguous tensors of the same type, use direct memory copy
    if (src0->type == src1->type && ggml_is_contiguous(src0) && ggml_is_contiguous(src1)) {
        GGML_ASSERT(ggml_nbytes(src0) == ggml_nbytes(src1));
        CNRT_CHECK(cnrtMemcpyAsync(src1->data, src0->data, ggml_nbytes(src0), 
                                   ctx.queue(), cnrtMemcpyDevToDev));
    } else {
        // For non-contiguous tensors or type conversions, use CNNL copy
        CnnlTensorDesc src0_desc, src1_desc;
        auto layout_array = cnnlTensorLayout_t::CNNL_LAYOUT_ARRAY;
        
        // Set up tensor descriptors
        int64_t src0_dims[GGML_MAX_DIMS];
        int64_t src1_dims[GGML_MAX_DIMS];
        int src0_ndim = 0, src1_ndim = 0;
        
        // Convert GGML dimensions to CNNL format
        for (int i = 0; i < GGML_MAX_DIMS; i++) {
            if (src0->ne[i] > 1 || i == 0) {
                src0_dims[src0_ndim++] = src0->ne[i];
            }
            if (src1->ne[i] > 1 || i == 0) {
                src1_dims[src1_ndim++] = src1->ne[i];
            }
        }
        
        // Ensure at least 1D
        if (src0_ndim == 0) {
            src0_dims[0] = 1;
            src0_ndim = 1;
        }
        if (src1_ndim == 0) {
            src1_dims[0] = 1;
            src1_ndim = 1;
        }

        src0_desc.Set(layout_array, ggml_type_to_cnnl_type(src0->type), src0_ndim, src0_dims);
        src1_desc.Set(layout_array, ggml_type_to_cnnl_type(src1->type), src1_ndim, src1_dims);
        
        // Get workspace size
        size_t workspace_size = 0;
        CNNL_CHECK(cnnlGetCopyWorkspaceSize(ctx.cnnl_handle(), src0_desc.desc(), src1_desc.desc(), &workspace_size));
        
        ggml_mlu_pool_alloc<char> mlu_alloc(ctx.pool());
        void * workspace = nullptr;
        if (workspace_size > 0) {
            workspace = mlu_alloc.alloc(workspace_size, ctx.queue());
        }
        
        // Perform copy operation
        CNNL_CHECK(cnnlCopy_v2(ctx.cnnl_handle(),
                              src0_desc.desc(), src0->data,
                              src1_desc.desc(), src1->data,
                              workspace, workspace_size));
    }
}

void ggml_mlu_op_abs(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
    // TODO: Implement abs operation
}

void ggml_mlu_op_sgn(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
    // TODO: Implement sgn operation
}

void ggml_mlu_op_neg(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
    // TODO: Implement neg operation
}

void ggml_mlu_op_mat_mul(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    (void)ctx;
    (void)dst;
    // TODO: Implement mat_mul operation
}
