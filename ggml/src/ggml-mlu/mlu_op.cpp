#include "ggml.h"
#include "common.h"
#include "mlu_op.h"
#include "mlu_include/cnrt.h"
#include "mlu_include/cnnl.h"
#include "mlu_include/cnnl_extra.h"

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
            ggml_mlu_op_mul_mat(ctx, dst);
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
    GGML_MLU_UNARY_OP_LOCALS

    GGML_ASSERT(ggml_is_contiguous(src));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src->type == GGML_TYPE_F32 || src->type == GGML_TYPE_F16);
    GGML_ASSERT(dst->type == GGML_TYPE_F32 || dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src->type == dst->type);

    CnnlTensorDesc x_desc(src);
    CnnlTensorDesc y_desc(dst);

    CNNL_CHECK(cnnlSqrt_v2(ctx.cnnl_handle(),
                    cnnlComputationPreference_t::CNNL_COMPUTATION_HIGH_PRECISION,
                    x_desc.desc(), src_d,
                    y_desc.desc(), dst_d));
}

void ggml_mlu_op_get_rows(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    GGML_MLU_BINARY_OP_LOCALS

    GGML_ASSERT(src1->type == GGML_TYPE_I32);
    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    // 创建张量描述符（使用默认的CnnlTensorDesc构造函数，会自动处理维度反转）
    CnnlTensorDesc params_desc(src0);
    CnnlTensorDesc indices_desc(src1);
    CnnlTensorDesc output_desc(dst);

    int params_ndim = params_desc.shape().size();
    int axis = params_ndim-2;
    int batch_dims = axis;
    
    if (params_ndim < 2) {
        GGML_ASSERT(false);
    }
    
    const int mode = 1;        // indices不包含负数

    // 执行gather操作
    CNNL_CHECK(cnnlBatchGatherV2_v2(
        ctx.cnnl_handle(),
        axis,
        batch_dims,
        mode,
        params_desc.desc(),
        src0_d,
        indices_desc.desc(),
        src1_d,
        output_desc.desc(),
        dst_d
    ));
}

void ggml_mlu_op_optensor(ggml_backend_mlu_context & ctx, ggml_tensor * dst,
                            cnnlOpTensorDesc_t op_type) {
    GGML_MLU_BINARY_OP_LOCALS

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    CnnlTensorDesc x_desc(src0);
    CnnlTensorDesc y_desc(src1);
    CnnlTensorDesc z_desc(dst);

    // Create and set OpTensor descriptor for ADD operation
    cnnlOpTensorDescriptor_t op_tensor_desc;
    CNNL_CHECK(cnnlCreateOpTensorDescriptor(&op_tensor_desc));
    CNNL_CHECK(cnnlSetOpTensorDescriptor(op_tensor_desc, op_type, ggml_type_to_cnnl_type(src0->type), CNNL_NOT_PROPAGATE_NAN));

    const float alpha1 = 1.0f;
    const float alpha2 = 1.0f;
    const float beta   = 0.0f;
    CNNL_CHECK(cnnlOpTensor(ctx.cnnl_handle(), op_tensor_desc,
                           &alpha1, x_desc.desc(), src0_d,
                           &alpha2, y_desc.desc(), src1_d,
                           nullptr, 0,
                           &beta, z_desc.desc(), dst_d));

    CNNL_CHECK(cnnlDestroyOpTensorDescriptor(op_tensor_desc));
}

void ggml_mlu_op_add(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    ggml_mlu_op_optensor(ctx, dst, CNNL_OP_TENSOR_ADD);
}

void ggml_mlu_op_sub(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    ggml_mlu_op_optensor(ctx, dst, CNNL_OP_TENSOR_SUB);
}

void ggml_mlu_op_mul(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    ggml_mlu_op_optensor(ctx, dst, CNNL_OP_TENSOR_MUL);
}

void ggml_mlu_op_div(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    GGML_MLU_BINARY_OP_LOCALS

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    CnnlTensorDesc x_desc(src0);
    CnnlTensorDesc y_desc(src1);
    CnnlTensorDesc z_desc(dst);

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
    GGML_MLU_UNARY_OP_LOCALS

    GGML_ASSERT(ggml_is_contiguous(src));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src->type == GGML_TYPE_F32 || src->type == GGML_TYPE_F16);
    GGML_ASSERT(dst->type == GGML_TYPE_F32 || dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src->type == dst->type);

    // Get epsilon parameter from dst->op_params
    float eps;
    memcpy(&eps, dst->op_params, sizeof(float));
    GGML_ASSERT(eps >= 0.0f);

    // Create tensor descriptors
    CnnlTensorDesc x_desc(src);
    CnnlTensorDesc y_desc(dst);

    // Create FuseNorm descriptor
    cnnlFuseNormDescriptor_t fuse_norm_desc;
    CNNL_CHECK(cnnlCreateFuseNormDescriptor(&fuse_norm_desc));
    
    // Set FuseNorm descriptor parameters
    // For RMS norm: no bias, no residual, no norm_bias, no norm_scale
    CNNL_CHECK(cnnlSetFuseNormDescriptor(
        fuse_norm_desc,
        eps,                             // eps
        1.0f,                            // scale (not used for RMS norm)
        false,                           // has_norm_scale
        false,                           // has_norm_bias 
        false,                           // has_bias
        false,                           // has_residual
        false,                           // store_output_before_norm
        y_desc.dtype(),                  // math_pre
        CNNL_TRANSFORMER_RMSNORM         // mode
    ));

    // Get workspace size
    size_t workspace_size;
    CNNL_CHECK(cnnlGetFuseNormWorkspaceSize(
        ctx.cnnl_handle(),
        fuse_norm_desc,
        x_desc.desc(),
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
        ctx.cnnl_handle(),
        x_desc.desc(), src_d,            // input
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
        y_desc.desc(),                   // output_desc
        dst_d,                           // output
        nullptr,                         // output_before_norm_desc (not used)
        nullptr,                         // output_before_norm (not used)
        nullptr,                         // output_quant_scale_desc (not used)
        nullptr                          // output_quant_scale (not used)
    ));

    // Destroy descriptor
    CNNL_CHECK(cnnlDestroyFuseNormDescriptor(fuse_norm_desc));
}

void ggml_mlu_op_mul_mat(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    GGML_MLU_BINARY_OP_LOCALS

    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(src1));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src0->type == GGML_TYPE_F32 || src0->type == GGML_TYPE_F16);
    GGML_ASSERT(src1->type == GGML_TYPE_F32 || src1->type == GGML_TYPE_F16);
    GGML_ASSERT( dst->type == GGML_TYPE_F32 ||  dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src0->type == src1->type && src0->type == dst->type);

    // 创建张量描述符
    // 为了匹配CPU行为 (B * A^T)，我们交换输入参数顺序并转置A
    CnnlTensorDesc lhs_desc(src1);  // B矩阵作为第一个参数
    CnnlTensorDesc rhs_desc(src0);  // A矩阵作为第二个参数
    CnnlTensorDesc z_desc(dst);
    
    // 创建MatMul描述符
    cnnlMatMulDescriptor_t matmul_desc;
    CNNL_CHECK(cnnlCreateMatMulDescriptor(&matmul_desc));
    
    int32_t trans_lhs = 0;  // B
    int32_t trans_rhs = 1;  // A^T
    CNNL_CHECK(cnnlSetMatMulDescAttr(matmul_desc, CNNL_MATMUL_DESC_TRANSA, &trans_lhs, sizeof(int32_t)));
    CNNL_CHECK(cnnlSetMatMulDescAttr(matmul_desc, CNNL_MATMUL_DESC_TRANSB, &trans_rhs, sizeof(int32_t)));
    
    // 创建算法描述符
    cnnlMatMulAlgo_t algo;
    CNNL_CHECK(cnnlCreateMatMulAlgo(&algo));
    
    // 获取启发式算法
    int requested_algo_count = 1;
    int return_algo_count = 0;
    cnnlMatMulHeuristicResult_t heuristic_result;
    CNNL_CHECK(cnnlCreateMatMulHeuristicResult(&heuristic_result));
    
    CNNL_CHECK(cnnlGetMatMulAlgoHeuristic(ctx.cnnl_handle(),
                                         matmul_desc,
                                         lhs_desc.desc(),
                                         rhs_desc.desc(),
                                         z_desc.desc(),
                                         z_desc.desc(),  // d_desc
                                         nullptr,         // preference
                                         requested_algo_count,
                                         &heuristic_result,
                                         &return_algo_count));
    
    GGML_ASSERT(return_algo_count > 0);
    
    // 从启发式结果获取算法和工作空间大小
    size_t workspace_size = 0;
    CNNL_CHECK(cnnlGetMatMulHeuristicResult(heuristic_result, algo, &workspace_size));
    
    // 分配工作空间
    ggml_mlu_pool_alloc<char> mlu_alloc(ctx.pool());
    void* workspace = nullptr;
    if (workspace_size > 0) {
        workspace = mlu_alloc.alloc(workspace_size, ctx.queue());
    }
    
    // 执行矩阵乘法
    const float alpha = 1.0f;
    const float beta = 0.0f;
    
    CNNL_CHECK(cnnlMatMul_v2(ctx.cnnl_handle(),
                            matmul_desc,
                            algo,
                            &alpha,
                            lhs_desc.desc(),  // B矩阵描述符
                            src1_d,           // B矩阵数据
                            rhs_desc.desc(),  // A矩阵描述符
                            src0_d,           // A矩阵数据
                            &beta,
                            z_desc.desc(),    // c_desc
                            dst_d,            // c (point to output, beta=0 so no effect)
                            workspace,
                            workspace_size,
                            z_desc.desc(),    // d_desc
                            dst_d));
    
    // 清理资源
    CNNL_CHECK(cnnlDestroyMatMulHeuristicResult(heuristic_result));
    CNNL_CHECK(cnnlDestroyMatMulAlgo(algo));
    CNNL_CHECK(cnnlDestroyMatMulDescriptor(matmul_desc));
}

void ggml_mlu_op_soft_max(ggml_backend_mlu_context & ctx, ggml_tensor * dst) {
    GGML_MLU_UNARY_OP_LOCALS

    GGML_ASSERT(ggml_is_contiguous(src));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_ASSERT(src->type == GGML_TYPE_F32 || src->type == GGML_TYPE_F16);
    GGML_ASSERT(dst->type == GGML_TYPE_F32 || dst->type == GGML_TYPE_F16);
    GGML_ASSERT(src->type == dst->type);

    // Get parameters from dst->op_params
    float scale = 1.0f;
    float max_bias = 0.0f;
    
    // Read parameters from op_params if they exist
    if (dst->op_params[0] != 0.0f) {
        memcpy(&scale, dst->op_params, sizeof(float));
    }
    if (dst->op_params[1] != 0.0f) {
        memcpy(&max_bias, (float*)dst->op_params + 1, sizeof(float));
    }

    // CNNL softmax requires 3D input tensor
    // We need to reshape the tensor to 3D format
    int64_t ne0 = src->ne[0];
    int64_t ne1 = src->ne[1];
    int64_t ne2 = src->ne[2];
    int64_t ne3 = src->ne[3];
    
    // Calculate total size to ensure correctness
    int64_t total_size = ne0 * ne1 * ne2 * ne3;
    
    // Reshape to 3D: [batch, seq_len, features]
    int64_t cnnl_dims[3];
    if (ne3 > 1) {
        // 4D tensor: [ne3, ne2, ne1*ne0] -> [batch, seq, features]
        cnnl_dims[0] = ne3*ne2;
        cnnl_dims[1] = ne1;
        cnnl_dims[2] = ne0;
    } else if (ne2 > 1) {
        // 3D tensor: [ne2, ne1, ne0] -> [batch, seq, features]
        cnnl_dims[0] = ne2;
        cnnl_dims[1] = ne1;
        cnnl_dims[2] = ne0;
    } else if (ne1 > 1) {
        // 2D tensor: [1, ne1, ne0] -> [batch, seq, features]
        cnnl_dims[0] = 1;
        cnnl_dims[1] = ne1;
        cnnl_dims[2] = ne0;
    } else {
        // 1D tensor: [1, 1, ne0] -> [batch, seq, features]
        cnnl_dims[0] = 1;
        cnnl_dims[1] = 1;
        cnnl_dims[2] = ne0;
    }

    // Create 3D tensor descriptors for CNNL
    CnnlTensorDesc src_desc, dst_desc;
    src_desc.Set(ggml_type_to_cnnl_type(src->type), 3, cnnl_dims);
    dst_desc.Set(ggml_type_to_cnnl_type(dst->type), 3, cnnl_dims);
    
    // Set algorithm and mode
    cnnlSoftmaxAlgorithm_t algorithm = CNNL_SOFTMAX_ACCURATE;
    cnnlSoftmaxMode_t mode = CNNL_SOFTMAX_MODE_LOW_DIMENSION; // Apply to last dimension

    // Alpha and beta scaling factors (set to NULL as they are not supported currently)
    const void* alpha = nullptr;
    const void* beta = nullptr;

    // Apply scale parameter to input before softmax if it's not 1.0
    void* scaled_src_d = const_cast<void*>(src_d);
    ggml_mlu_pool_alloc<char> temp_alloc(ctx.pool());
    
    if (scale != 1.0f) {
        // Allocate temporary buffer for scaled input
        const size_t temp_size = ggml_nelements(src) * ggml_type_size(src->type);
        scaled_src_d = temp_alloc.alloc(temp_size, ctx.queue());

        // Create OpTensor descriptor for scalar multiplication
        cnnlOpTensorDescriptor_t op_tensor_desc;
        CNNL_CHECK(cnnlCreateOpTensorDescriptor(&op_tensor_desc));
        CNNL_CHECK(cnnlSetOpTensorDescriptor(op_tensor_desc, CNNL_OP_TENSOR_MUL, ggml_type_to_cnnl_type(src->type), CNNL_NOT_PROPAGATE_NAN));

        // Create scalar tensor descriptor for scale
        int64_t scalar_dims[1] = {1};
        CnnlTensorDesc scalar_desc;
        scalar_desc.Set(ggml_type_to_cnnl_type(src->type), 1, scalar_dims);

        // Allocate scalar on MLU device
        ggml_mlu_pool_alloc<float> scale_alloc(ctx.pool());
        void* scale_device = scale_alloc.alloc(1, ctx.queue());
        
        // Copy scale value to device
        if (src->type == GGML_TYPE_F32) {
            CNRT_CHECK(cnrtMemcpy(scale_device, &scale, sizeof(float), CNRT_MEM_TRANS_DIR_HOST2DEV));
        } else if (src->type == GGML_TYPE_F16) {
            ggml_fp16_t scale_fp16 = ggml_fp32_to_fp16(scale);
            CNRT_CHECK(cnrtMemcpy(scale_device, &scale_fp16, sizeof(ggml_fp16_t), CNRT_MEM_TRANS_DIR_HOST2DEV));
        }

        // Perform scaled_src = src * scale using OpTensor
        const float alpha1 = 1.0f;
        const float alpha2 = 1.0f;
        const float beta = 0.0f;
        
        CNNL_CHECK(cnnlOpTensor(ctx.cnnl_handle(), op_tensor_desc,
                               &alpha1, src_desc.desc(), src_d,
                               &alpha2, scalar_desc.desc(), scale_device,
                               nullptr, 0,  // workspace and workspace_size
                               &beta, src_desc.desc(), scaled_src_d));

        // Cleanup
        CNNL_CHECK(cnnlDestroyOpTensorDescriptor(op_tensor_desc));
    }

    // Call CNNL softmax forward function on scaled input
    CNNL_CHECK(cnnlSoftmaxForward(
        ctx.cnnl_handle(),
        algorithm,
        mode,
        alpha,
        src_desc.desc(),
        scaled_src_d,
        beta,
        dst_desc.desc(),
        dst_d
    ));
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

    src_desc.Set(ggml_type_to_cnnl_type(src0->type), src_ndim, src_dims);
    dst_desc.Set(ggml_type_to_cnnl_type(dst->type), dst_ndim, dst_dims);

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

        src0_desc.Set(ggml_type_to_cnnl_type(src0->type), src0_ndim, src0_dims);
        src1_desc.Set(ggml_type_to_cnnl_type(src1->type), src1_ndim, src1_dims);
        
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
