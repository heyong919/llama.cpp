寒武纪Torch-MLU-Ops⽤⼾⼿册

版本 1.3.2

2025 年 01 ⽉ 06 ⽇

Cambricon®

⽬录

表 格 ⽬ 录

(cid:63154) 版权声明

(cid:63155) 更新历史

(cid:63156) 简介

⽬录

i

(cid:63154)

(cid:63155)

(cid:63156)

(cid:63159)

(cid:63159)

(cid:63159)

(cid:63160)

(cid:63160)

(cid:63160)

(cid:63161)

(cid:63161)

(cid:63162)

(cid:63154)(cid:63153)

(cid:63156).(cid:63154) 应⽤场景 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63156).(cid:63155) 关键特性 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157) 安装和部署

(cid:63157).(cid:63154) 在主机上安装和部署 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157).(cid:63154).(cid:63154) 环境要求 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157).(cid:63154).(cid:63155) 通过docker镜像启动 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157).(cid:63154).(cid:63156) 编译安装 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157).(cid:63154).(cid:63157) 测试 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63158) 重点特性

(cid:63158).(cid:63154)

torch_mlu_ops算⼦ . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63153)

(cid:63158).(cid:63154).(cid:63154)

torch_mlu_ops.active . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63155)

(cid:63158).(cid:63154).(cid:63155)

torch_mlu_ops.apply_rotary . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63156)

(cid:63158).(cid:63154).(cid:63156)

torch_mlu_ops.attention_project . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63158)

(cid:63158).(cid:63154).(cid:63157)

torch_mlu_ops.batch_matmul

. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63159)

(cid:63158).(cid:63154).(cid:63158)

torch_mlu_ops.copy_blocks . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63160)

(cid:63158).(cid:63154).(cid:63159)

torch_mlu_ops.copy_blocks_out_of_place . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63161)

(cid:63158).(cid:63154).(cid:63160)

torch_mlu_ops.dequant_from_linear_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63162)

(cid:63158).(cid:63154).(cid:63161)

torch_mlu_ops.dequant_from_paged_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63155)(cid:63153)

(cid:63158).(cid:63154).(cid:63162)

torch_mlu_ops.ﬀn . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63155)(cid:63155)

(cid:63158).(cid:63154).(cid:63154)(cid:63153) torch_mlu_ops.ﬂash_attention . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63155)(cid:63156)

(cid:63158).(cid:63154).(cid:63154)(cid:63154) torch_mlu_ops.ﬂash_attn_sq_mm_allreduce . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63155)(cid:63158)

(cid:63158).(cid:63154).(cid:63154)(cid:63155) torch_mlu_ops.fused_layer_norm . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63155)(cid:63160)

(cid:63158).(cid:63154).(cid:63154)(cid:63156) torch_mlu_ops.fused_moe . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63155)(cid:63162)

(cid:63158).(cid:63154).(cid:63154)(cid:63157) torch_mlu_ops.fused_norm_attention_project . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63156)(cid:63154)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

i

Cambricon®

(cid:63158).(cid:63154).(cid:63154)(cid:63158) torch_mlu_ops.fused_norm_residual_ﬀn . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63156)(cid:63156)

(cid:63158).(cid:63154).(cid:63154)(cid:63159) torch_mlu_ops.fused_rms_norm . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63156)(cid:63158)

(cid:63158).(cid:63154).(cid:63154)(cid:63160) torch_mlu_ops.fused_rope . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63156)(cid:63159)

(cid:63158).(cid:63154).(cid:63154)(cid:63161) torch_mlu_ops.group_gemm . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63156)(cid:63161)

(cid:63158).(cid:63154).(cid:63154)(cid:63162) torch_mlu_ops.smooth_quant_group_gemm . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63156)(cid:63162)

(cid:63158).(cid:63154).(cid:63155)(cid:63153) torch_mlu_ops.matmul . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63154)

(cid:63158).(cid:63154).(cid:63155)(cid:63154) torch_mlu_ops.matmul_allreduce . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63155)

(cid:63158).(cid:63154).(cid:63155)(cid:63155) torch_mlu_ops.moe_combine_result

. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63156)

(cid:63158).(cid:63154).(cid:63155)(cid:63156) torch_mlu_ops.moe_expand_input . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63157)

(cid:63158).(cid:63154).(cid:63155)(cid:63157) torch_mlu_ops.moe_gen_idx . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63158)

(cid:63158).(cid:63154).(cid:63155)(cid:63158) torch_mlu_ops.moe_softmax_topk . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63159)

(cid:63158).(cid:63154).(cid:63155)(cid:63159) torch_mlu_ops.preload . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63159)

(cid:63158).(cid:63154).(cid:63155)(cid:63160) torch_mlu_ops.quant_to_linear_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63160)

(cid:63158).(cid:63154).(cid:63155)(cid:63161) torch_mlu_ops.quant_to_paged_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63157)(cid:63162)

(cid:63158).(cid:63154).(cid:63155)(cid:63162) torch_mlu_ops.oﬄine_quant_to_linear_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63153)

(cid:63158).(cid:63154).(cid:63156)(cid:63153) torch_mlu_ops.oﬄine_quant_to_paged_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63154)

(cid:63158).(cid:63154).(cid:63156)(cid:63154) torch_mlu_ops.reshape_linear_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63155)

(cid:63158).(cid:63154).(cid:63156)(cid:63155) torch_mlu_ops.reshape_paged_cache . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63156)

(cid:63158).(cid:63154).(cid:63156)(cid:63156) torch_mlu_ops.quantize . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63157)

(cid:63158).(cid:63154).(cid:63156)(cid:63157) torch_mlu_ops.single_query_cached_kv_attn . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63158)

(cid:63158).(cid:63154).(cid:63156)(cid:63158) torch_mlu_ops.smooth_quant_matmul

. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63160)

(cid:63158).(cid:63154).(cid:63156)(cid:63159) torch_mlu_ops.smooth_quant_matmul_allreduce . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63158)(cid:63162)

(cid:63158).(cid:63154).(cid:63156)(cid:63160) torch_mlu_ops.swap_blocks

. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63153)

(cid:63158).(cid:63154).(cid:63156)(cid:63161) torch_mlu_ops.weight_only_quant_matmul

. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63154)

(cid:63158).(cid:63154).(cid:63156)(cid:63162) torch_mlu_ops.moe_active . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63155)

(cid:63158).(cid:63154).(cid:63157)(cid:63153) torch_mlu_ops.moe_cast_gating . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63156)

(cid:63158).(cid:63154).(cid:63157)(cid:63154) torch_mlu_ops.update_out_and_lse . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63157)

(cid:63158).(cid:63154).(cid:63157)(cid:63155) torch_mlu_ops.single_query_mixed_cached_kv_attn . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63158)

(cid:63159) 最佳实践

(cid:63159)(cid:63161)

(cid:63159).(cid:63154)

(cid:63159).(cid:63155)

(cid:63159).(cid:63156)

(cid:63160) FAQ

Torch-MLU-Ops性能benchmark测试 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63161)

Torch-MLU-Ops gen case . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63162)

Torch-MLU-Ops 运⾏pt格式测例 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63160)(cid:63153)

(cid:63160)(cid:63156)

(cid:63160).(cid:63154) Cambricon Torch-MLU-Ops和BangTransformer的关系 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63160)(cid:63156)

(cid:63160).(cid:63155) Cambricon Torch-MLU-Ops和Cambricon PyTorch的关系 . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63160)(cid:63156)

(cid:63160).(cid:63156) Cambricon Torch-MLU-Ops和Cambricon MLU-Ops的关系 . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63160)(cid:63156)

(cid:63160).(cid:63157) Cambricon Torch-MLU-Ops中 ﬂash_attention 算⼦和Cambricon FlashAttention以及Cambricon PyTorch

源⽣SDPA算⼦的关系 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63160)(cid:63156)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

ii

Cambricon®

表 格 ⽬ 录

(cid:63157).(cid:63154) Torch-MLU-Ops不同平台⽀持软件版本 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157).(cid:63155) Torch-MLU-Ops配置依赖 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63157).(cid:63156) Torch-MLU-Ops⽀持的PyTorch版本 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(cid:63160)

(cid:63160)

(cid:63161)

(cid:63158).(cid:63154) torch_mlu_ops算⼦ . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63154)(cid:63153)

(cid:63159).(cid:63154) Torch-MLU-Ops benchmark测试⽀持算⼦ . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . (cid:63159)(cid:63161)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)

Cambricon®

免责声明

(cid:63154) 版权声明

中科寒武纪科技股份有限公司（下称“寒武纪”）不代表、担保（明⽰、暗⽰或法定的）或保证本⽂件所含信息，并明⽰放弃

对可销售性、所有权、不侵犯知识产权或特定⽬的适⽤性做出任何和所有暗⽰担保，且寒武纪不承担因应⽤或使⽤任何产品

或服务⽽产⽣的任何责任。寒武纪不应对因下列原因产⽣的任何违约、损害赔偿、成本或问题承担任何责任：（(cid:63154)）使⽤寒武

纪产品的任何⽅式违背本指南；或（(cid:63155)）客⼾产品设计。

责任限制

在任何情况下，寒武纪都不对因使⽤或⽆法使⽤本指南⽽导致的任何损害（包括但不限于利润损失、业务中断和信息损失等

损害）承担责任，即便寒武纪已被告知可能遭受该等损害。尽管客⼾可能因任何理由遭受任何损害，根据寒武纪的产品销售

条款与条件，寒武纪为本指南所述产品对客⼾承担的总共和累计责任应受到限制。

信息准确性

本⽂件提供的信息属于寒武纪所有，且寒武纪保留不经通知随时对本⽂件信息或对任何产品和服务做出任何更改的权利。本

指南所含信息和本指南所引⽤寒武纪⽂档的所有其他信息均“按原样”提供。寒武纪不担保信息、⽂本、图案、链接或本指

南内所含其他项⽬的准确性或完整性。寒武纪可不经通知随时对本指南或本指南所述产品做出更改，但不承诺更新本指南。

本指南列出的性能测试和等级要使⽤特定芯⽚或计算机系统或组件来测量。经该等测试，本指南所⽰结果反映了寒武纪产品

的⼤概性能。系统硬件或软件设计或配置的任何不同会影响实际性能。如上所述，寒武纪不代表、担保或保证本指南所述产

品将适⽤于任何特定⽤途。寒武纪不代表或担保测试每种产品的所有参数。客⼾全权承担确保产品适合并适⽤于客⼾计划的

应⽤以及对应⽤程序进⾏必要测试的责任，以期避免应⽤程序或产品的默认情况。

客⼾产品设计的脆弱性会影响寒武纪产品的质量和可靠性并导致超出本指南范围的额外或不同的情况和/或要求。

知识产权通知

寒武纪和寒武纪的标志是中科寒武纪科技股份有限公司在中国和其他国家的商标和/或注册商标。其他公司和产品名称应为

与其关联的各⾃公司的商标。

本指南为版权所有并受全世界版权法律和条约条款的保护。未经寒武纪的事先书⾯许可，不可以任何⽅式复制、重制、修改、

出版、上传、发布、传输或分发本指南。除了客⼾使⽤本指南信息和产品的权利，根据本指南，寒武纪不授予其他任何明⽰

或暗⽰的权利或许可。未免疑义，寒武纪不根据任何专利、版权、商标、商业秘密或任何其他寒武纪的知识产权或所有权对

客⼾授予任何（明⽰或暗⽰的）权利或许可。

(cid:15) 版权声明

(cid:15) © (cid:63155)(cid:63153)(cid:63155)(cid:63157) 中科寒武纪科技股份有限公司保留⼀切权利。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)

Cambricon®

(cid:15) V(cid:63154).(cid:63156).(cid:63155)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63158)年(cid:63153)(cid:63154)⽉(cid:63153)(cid:63162)⽇

更新内容:

(cid:63155) 更新历史

(cid:15) fused_layer_norm 与 fused_rms_norm 的输⼊和输出⽀持更多的 stride 组合。请参考torch_mlu_ops.fused_rms_norm、

torch_mlu_ops.fused_layer_norm章节。

(cid:15) V(cid:63154).(cid:63156).(cid:63154)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63154)(cid:63155)⽉(cid:63156)(cid:63153)⽇

更新内容:

‒ smooth_quant_group_gemm 与 fused_moe 算⼦⽀持w(cid:63157)w(cid:63161)混合量化。请参考torch_mlu_ops.smooth_quant_group_gemm

和torch_mlu_ops.fused_moe章节。

‒ 新增 dequant_from_paged_cache 算⼦，详细内容请参考torch_mlu_ops.dequant_from_paged_cache章节。

‒ dequant_from_linear_cache 算⼦不再⽀持ﬂoat(cid:63156)(cid:63155)的 key 和 value。

‒ 新增Torch-MLU-Opsgencase功能。

‒ 新增Torch-MLU-Ops运⾏pt格式测例章节内容。

(cid:15) V(cid:63154).(cid:63156).(cid:63153)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63154)(cid:63155)⽉(cid:63154)(cid:63160)⽇

更新内容:

‒ group_gemm 与 smooth_quant_group_gemm 算⼦的 max_m 参数由可选修改为必选。请参考torch_mlu_ops.group_gemm

和torch_mlu_ops.smooth_quant_group_gemm章节。

‒ moe_softmax_topk ⽀持mask功能，请参考torch_mlu_ops.moe_softmax_topk章节。

‒ 新增 dequant_from_linear_cache 算⼦，详细内容请参考torch_mlu_ops.dequant_from_linear_cache章节。

(cid:15) V(cid:63154).(cid:63155).(cid:63156)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63154)(cid:63155)⽉(cid:63153)(cid:63162)⽇

更新内容:

‒ single_query_cached_kv_attn 与 flash_attention 算 ⼦ ⽀ 持 head_size_qk != head_size_v， 请 参

考torch_mlu_ops.single_query_cached_kv_attn和torch_mlu_ops.ﬂash_attention章节。

‒ group_gemm 与 smooth_quant_group_gemm 算 ⼦ ⽀ 持 bias， 请 参 考torch_mlu_ops.group_gemm

和torch_mlu_ops.smooth_quant_group_gemm章节。

‒ matmul 删除原位输出参数，增加指定输出类型参数，请参考torch_mlu_ops.matmul章节。

(cid:15) V(cid:63154).(cid:63155).(cid:63155)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63154)(cid:63154)⽉(cid:63153)(cid:63161)⽇

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)

(cid:63155). 更新历史

更新内容:

‒ smooth_quant_group_gemm 算⼦新增int(cid:63157) group量化功能，请参考torch_mlu_ops.smooth_quant_group_gemm章

节。

‒ allreduce 类 算 ⼦ 删 除 act_mode， 请 参 考torch_mlu_ops.smooth_quant_matmul_allreduce

和torch_mlu_ops.matmul_allreduce章节。

‒ weight_only_quant_matmul 与 smooth_quant_matmul 添 加 use_hp_active 参 数， 控 制 激 活 计 算 ⽅ 式， 请 参

考torch_mlu_ops.weight_only_quant_matmul和torch_mlu_ops.smooth_quant_matmul章节。

‒ active 算⼦新增quick_gelu和swish激活模式，请参考torch_mlu_ops.active章节。

(cid:15) V(cid:63154).(cid:63155).(cid:63154)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63154)(cid:63154)⽉(cid:63153)(cid:63157)⽇

更新内容:

‒ quant_to_linear_cache 算⼦新增group量化及int(cid:63157)量化功能，请参考torch_mlu_ops.quant_to_linear_cache章节。

‒ 新增 moe_cast_gating 算⼦，请参考torch_mlu_ops.moe_cast_gating章节。

‒ 新增 update_out_and_lse 算⼦，请参考torch_mlu_ops.update_out_and_lse章节。

‒ fused_rope 算⼦⽀持int(cid:63161)/int(cid:63157) kv cache。

‒ matmul 与 batch_matmul 新 增 配 置 trans_a，trans_b 参 数， 请 参 考torch_mlu_ops.matmul

和torch_mlu_ops.batch_matmul章节。

‒ single_query_cached_kv_attn ⽀持int(cid:63157) kv cache，⽀持返回lse。请参考torch_mlu_ops.single_query_cached_kv_attn

章节。

‒ 新增 single_query_mixed_cached_kv_attn 算⼦。

‒ fused_layer_norm 与 fused_rms_norm ⽀ 持 输 出 动 态 量 化。 请 参 考torch_mlu_ops.fused_rms_norm、

torch_mlu_ops.fused_layer_norm章节。

(cid:15) V(cid:63154).(cid:63155).(cid:63153)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63154)(cid:63153)⽉(cid:63155)(cid:63157)⽇

更新内容:

‒ moe_softmax_topk 算⼦新增grouped_topk功能，请参考torch_mlu_ops.moe_softmax_topk章节。

‒ moe_softmax_topk 算⼦不再⽀持原位功能，请参考torch_mlu_ops.moe_softmax_topk章节。

‒ moe_gen_idx 算⼦不再⽀持原位功能，请参考torch_mlu_ops.moe_gen_idx章节。

‒ 新增 fused rope 算⼦，请参考torch_mlu_ops.fused_rope章节。

‒ matmul 算⼦⽀持INT(cid:63161)输⼊，请参考torch_mlu_ops.matmul章节。

‒ 新增 batch_matmul 算⼦，请参考torch_mlu_ops.batch_matmul章节。

(cid:15) V(cid:63154).(cid:63154).(cid:63157)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63153)(cid:63162)⽉(cid:63156)(cid:63153)⽇

更新内容:

‒ 新增 offline_quant_to_paged_cache 算⼦，详⻅torch_mlu_ops.oﬄine_quant_to_paged_cache章节。

‒ 新增 moe_gen_idx 和 moe_expand_input 算⼦，详⻅torch_mlu_ops.moe_gen_idx和 torch_mlu_ops.expand_input

章节。

‒ 新增 moe_combine_result 算⼦，详⻅torch_mlu_ops.moe_combine_result章节。

‒ 新增 moe_gen_idx 算⼦，详⻅torch_mlu_ops.moe_gen_idx章节。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)

(cid:63155). 更新历史

‒ 删 除 quant_matmul 算 ⼦， 由 smooth_quant_matmul 和 weight_only_quant_matmul 实 现 其 功 能， 详

⻅torch_mlu_ops.smooth_quant_matmul和torch_mlu_ops.weight_only_quant_matmul章节。

‒ flash_attention 算⼦新增 block_tables 、k/v_cache_quant_scale 参数。请参考torch_mlu_ops.ﬂash_attention

章节。

‒ matmul 算⼦⽀持激活计算⽅式配置，请参考torch_mlu_ops.matmul章节。

‒ FAQ章节新增对 flash_attention 算⼦的说明，请参考FAQ章节。

‒ 新增 moe_active 算⼦，详⻅torch_mlu_ops.moe_active章节。

(cid:15) V(cid:63154).(cid:63154).(cid:63156)

更新时间: (cid:63155)(cid:63153)(cid:63155)(cid:63157)年(cid:63153)(cid:63162)⽉(cid:63153)(cid:63159)⽇

更新内容:

‒ BangTransformer更名为Torch-MLU-Ops, 定位PyTorch第三⽅算⼦库。

‒ 原BangTransformer⽹络评测相关⽂档请参考《寒武纪BangTransformer⽤⼾⼿册》v(cid:63154).(cid:63154).(cid:63155)版本。

‒ bt_ops的命名空间变化为torch_mlu_ops, 请参考torch_mlu_ops算⼦。

‒ torch_mlu_ops.single_query_cached_kv_attn算⼦⽀持per_token量化和per_channel量化。

‒ torch_mlu_ops.fused_moe算⼦在⾮量化情况下⽀持EP模式，内部⽀持group gemm和allreduce并⾏。

‒ 提

供 ﬂash_attn_sq_mm_allreduce、matmul_allreduce、smooth_quant_matmul_allreduce 通

算

融 合 算 ⼦，fused_moe ⽀ 持 通 算 融 合。 请 参 考torch_mlu_ops.ﬂash_attn_sq_mm_allreduce、

torch_mlu_ops.matmul_allreduce、torch_mlu_ops.smooth_quant_matmul_allreduce、

torch_mlu_ops.fused_moe。

‒ torch_mlu_ops.ﬂash_attention、torch_mlu_ops.single_query_cached_kv_attn、torch_mlu_ops.fused_rms_norm、

torch_mlu_ops.fused_layer_norm ⽀ 持 原 位 输 出。 请 参 考torch_mlu_ops.ﬂash_attention、

torch_mlu_ops.single_query_cached_kv_attn、torch_mlu_ops.fused_rms_norm、torch_mlu_ops.fused_layer_norm。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)

Cambricon®

3.1 应⽤场景

(cid:63156) 简介

Torch-MLU-Ops是寒武纪设计和开发的PyTorch第三⽅算⼦库。对于使⽤PyTorch框架的开发者，通过Torch-MLU-Ops，能

够便捷地使⽤这些⾃定义算⼦，进⾏算⼦的集成、评测和业务部署。

Torch-MLU-Ops已全量覆盖LLM（Large Language Model）推理场景下的常⻅算⼦。作为后端，已⽀持Cambricon vLLM、

Cambricon TGI、Cambricon Stable Diﬀusion web UI、Cambricon ComfyUI以及Cambricon Diﬀusers。

3.2 关键特性

Torch-MLU-Ops在⽀持LLM模型部署时提供的关键特性如下所⽰：

(cid:15) PyTorch⻛格的算⼦API

(cid:15) ⽀持LLM推理场景下的常⻅算⼦

(cid:15) Flash Attention⽀持Multi-head Attention、Multi-query Attention、Group-query Attention特性

(cid:15) ⽀持KV Cache INT(cid:63161)特性

(cid:15) Single Query Cached Attention算⼦⽀持Paged特性

(cid:15) 矩阵乘类算⼦⽀持Weight-only、AWQ、GPTQ、Smoothquant量化特性

(cid:15) ⽀持MOE场景下常⻅算⼦

(cid:15) 提供不同融合⼒度的融合算⼦

(cid:15) 提供通信——计算融合算⼦MC(cid:63155)

(cid:15) ⽀持Cambricon TGI调⽤

(cid:15) ⽀持Cambricon vLLM调⽤

(cid:15) ⽀持Cambricon Stable Diﬀusion web UI调⽤

(cid:15) ⽀持Cambricon ComfyUI调⽤

(cid:15) ⽀持Cambricon Diﬀusers调⽤

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)

Cambricon®

(cid:63157) 安装和部署

4.1 在主机上安装和部署

4.1.1 环境要求

(cid:15) Torch-MLU-Ops在不同的操作系统上有不同的软件版本依赖。

不同平台⽀持的软件版本如下：

表 (cid:63157).(cid:63154): Torch-MLU-Ops不同平台⽀持软件版本

平台

编译器版本

CMake版本

Ubuntu(cid:63155)(cid:63155).(cid:63153)(cid:63157) x(cid:63161)(cid:63159)-(cid:63159)(cid:63157)

gcc (cid:63162).(cid:63157).(cid:63153)或更⾼版本

(cid:63156).(cid:63154)(cid:63153)或更⾼版本

Debian(cid:63154)(cid:63153).(cid:63154)(cid:63154) x(cid:63161)(cid:63159).(cid:63159)(cid:63157)

gcc (cid:63162).(cid:63157).(cid:63153)或更⾼版本

(cid:63156).(cid:63154)(cid:63153)或更⾼版本

(cid:15) 编译、运⾏Torch-MLU-Ops同时还需要配置以下依赖。

表 (cid:63157).(cid:63155): Torch-MLU-Ops配置依赖

Torch-MLU-Ops Torch-MLU CNNL

CNNL_Extra CNCL

CNToolkit

v(cid:63154).(cid:63156).(cid:63155)

v(cid:63154).(cid:63156).(cid:63154)

v(cid:63154).(cid:63156).(cid:63153)

v(cid:63154).(cid:63155).(cid:63156)

v(cid:63154).(cid:63155).(cid:63155)

v(cid:63154).(cid:63155).(cid:63154)

v(cid:63154).(cid:63155).(cid:63153)

v(cid:63154).(cid:63154).(cid:63157)

v(cid:63154).(cid:63154).(cid:63156)

v(cid:63154).(cid:63155)(cid:63157).(cid:63154)

v(cid:63154).(cid:63155)(cid:63161).(cid:63156)

v(cid:63154).(cid:63154)(cid:63155).(cid:63156)

v(cid:63154).(cid:63155)(cid:63157).(cid:63154)

v(cid:63156).(cid:63154)(cid:63158).(cid:63159)

v(cid:63154).(cid:63155)(cid:63157).(cid:63154)

v(cid:63154).(cid:63155)(cid:63161).(cid:63156)

v(cid:63154).(cid:63154)(cid:63155).(cid:63155)

v(cid:63154).(cid:63155)(cid:63157).(cid:63154)

v(cid:63156).(cid:63154)(cid:63158).(cid:63158)

v(cid:63154).(cid:63155)(cid:63157).(cid:63153)

v(cid:63154).(cid:63155)(cid:63161).(cid:63155)

v(cid:63154).(cid:63154)(cid:63155).(cid:63154)

v(cid:63154).(cid:63155)(cid:63157).(cid:63153)

v(cid:63156).(cid:63154)(cid:63158).(cid:63157)

v(cid:63154).(cid:63155)(cid:63157).(cid:63153)

v(cid:63154).(cid:63155)(cid:63161).(cid:63154)

v(cid:63154).(cid:63154)(cid:63155).(cid:63154)

v(cid:63154).(cid:63155)(cid:63157).(cid:63153)

v(cid:63156).(cid:63154)(cid:63158).(cid:63156)

v(cid:63154).(cid:63155)(cid:63156).(cid:63154)

v(cid:63154).(cid:63155)(cid:63160).(cid:63157)

v(cid:63154).(cid:63154)(cid:63154).(cid:63156)

v(cid:63154).(cid:63155)(cid:63156).(cid:63153)

v(cid:63156).(cid:63154)(cid:63157).(cid:63156)

v(cid:63154).(cid:63155)(cid:63156).(cid:63154)

v(cid:63154).(cid:63155)(cid:63160).(cid:63156)

v(cid:63154).(cid:63154)(cid:63154).(cid:63156)

v(cid:63154).(cid:63155)(cid:63155).(cid:63156)

v(cid:63156).(cid:63154)(cid:63157).(cid:63155)

v(cid:63154).(cid:63155)(cid:63156).(cid:63153)

v(cid:63154).(cid:63155)(cid:63160).(cid:63154)

v(cid:63154).(cid:63154)(cid:63154).(cid:63154)

v(cid:63154).(cid:63155)(cid:63155).(cid:63154)

v(cid:63156).(cid:63154)(cid:63157).(cid:63154)

v(cid:63154).(cid:63155)(cid:63155).(cid:63155)

v(cid:63154).(cid:63155)(cid:63159).(cid:63160)

v(cid:63154).(cid:63154)(cid:63153).(cid:63157)

v(cid:63154).(cid:63155)(cid:63154).(cid:63154)

v(cid:63156).(cid:63154)(cid:63156).(cid:63160)

v(cid:63154).(cid:63155)(cid:63155).(cid:63155)

v(cid:63154).(cid:63155)(cid:63159).(cid:63159)

v(cid:63154).(cid:63154)(cid:63153).(cid:63156)

v(cid:63154).(cid:63155)(cid:63154).(cid:63154)

v(cid:63156).(cid:63154)(cid:63156).(cid:63158)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63160)

(cid:63157). 安装和部署

(cid:63157).(cid:63154). 在主机上安装和部署

(cid:15) 此外运⾏Torch-MLU-Ops还依赖PyTorch环境。Torch-MLU-Ops已⽀持的PyTorch版本请参考。

表 (cid:63157).(cid:63156): Torch-MLU-Ops⽀持的PyTorch版本

Torch-MLU-Ops PyTorch

v(cid:63154).(cid:63156).(cid:63155)

v(cid:63154).(cid:63156).(cid:63154)

v(cid:63154).(cid:63156).(cid:63153)

v(cid:63154).(cid:63155).(cid:63156)

v(cid:63154).(cid:63155).(cid:63155)

v(cid:63154).(cid:63155).(cid:63154)

v(cid:63154).(cid:63155).(cid:63153)

v(cid:63154).(cid:63154).(cid:63157)

v(cid:63154).(cid:63154).(cid:63156)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63157), v(cid:63155).(cid:63158)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63156), v(cid:63155).(cid:63157)

v(cid:63155).(cid:63154), v(cid:63155).(cid:63156)

4.1.2 通过docker镜像启动

寒武纪提供的PyTorch解决⽅案镜像已提供了Torch-MLU-Ops所需要的依赖，请参考PyTorch解决⽅案镜像的使⽤⼿册启动

镜像。

4.1.3 编译安装

Torch-MLU-Ops编 译 脚 本 依 赖 于NEUWARE_HOME环 境 变 量， 该 环 境 变 量 指 向 寒 武 纪CNToolkit的 安 装 路 径， 通 常 为

/usr/local/neuware 。

在运⾏⽰例程序前，您需要将 NEUWARE_HOME 中的 lib64 ⽬录添加到 LD_LIBRARY_PATH 环境变量中，例如：

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NEUWARE_HOME/lib64

如果您使⽤寒武纪提供的docker镜像，以上环境变量均已经被设置好了。

设置好 NEUWARE_HOME 环境变量后，您可以使⽤以下命令编译Torch-MLU-Ops。

从远端clone仓库，进⼊项⽬的主⽬录：

cd torch_mlu_ops

源码安装：

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63161)

(cid:63157).(cid:63154). 在主机上安装和部署

(cid:63157). 安装和部署

pip install -e .

wheel包安装：

python setup.py bdist_wheel

pip install dist/torch_mlu_ops*.whl

注意：

如果安装过程中遇到权限问题，请获取相关权限或切换到拥有权限的user执⾏pip install 这步操作。

移除Torch-MLU-Ops（Optional）：

pip uninstall torch_mlu_ops

4.1.4 测试

在测试前，请确保Torch-MLU-Ops已安装。可以通过 pip list 命令查询Torch-MLU-Ops的安装情况。若有如下类似打印，

则表明安装成功。

torch-mlu-ops

1.1.0+pt21

Torch-MLU-Ops模块的导⼊⽅法请参考。

import torch_mlu_ops as tmo

在 ./tests/ops_pytest ⽬录下，提供了 ops 级别的测试⽰例程序，您可以参考如下命令进⾏测试。

cd tests/ops_pytest/

./run_test.sh

或者单独测试某个测例，例如:

python test_flash_attention.py

在 ./tests/kernels_pytest ⽬录下，提供了 kernels 级别的测试⽰例程序，您可以参考如下命令进⾏测试。

cd tests/kernels_pytest

./build.sh

# 需要先编译

./run_test.sh

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63162)

Cambricon®

(cid:63158) 重点特性

5.1 torch_mlu_ops算⼦

torch_mlu_ops算⼦为Torch-MLU-Ops设计和封装的PyTorch⻛格的算⼦API，能够以灵活的⽅式集成到社区或者客⼾的

LLM推理引擎框架中，加速客⼾的LLM业务部署。

序号

算⼦名称

表 (cid:63158).(cid:63154): torch_mlu_ops算⼦

(cid:63154)

(cid:63155)

(cid:63156)

(cid:63157)

(cid:63158)

(cid:63159)

(cid:63160)

(cid:63161)

(cid:63162)

(cid:63154)(cid:63153)

(cid:63154)(cid:63154)

(cid:63154)(cid:63155)

(cid:63154)(cid:63156)

(cid:63154)(cid:63157)

torch_mlu_ops.active

torch_mlu_ops.apply_rotary

torch_mlu_ops.attention_project

torch_mlu_ops.batch_matmul

torch_mlu_ops.copy_blocks

torch_mlu_ops.copy_blocks_out_of_place

torch_mlu_ops.dequant_from_linear_cache

torch_mlu_ops.dequant_from_paged_cache

torch_mlu_ops.ﬀn

torch_mlu_ops.ﬂash_attention

torch_mlu_ops.ﬂash_attn_sq_mm_allreduce

torch_mlu_ops.fused_layer_norm

torch_mlu_ops.fused_moe

torch_mlu_ops.fused_norm_attention_project

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

下⻚继续

(cid:63154)(cid:63153)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

序号

算⼦名称

表 5.1 – 续上⻚

(cid:63154)(cid:63158)

(cid:63154)(cid:63159)

(cid:63154)(cid:63160)

(cid:63154)(cid:63161)

(cid:63154)(cid:63162)

(cid:63155)(cid:63153)

(cid:63155)(cid:63154)

(cid:63155)(cid:63155)

(cid:63155)(cid:63156)

(cid:63155)(cid:63157)

(cid:63155)(cid:63158)

(cid:63155)(cid:63159)

(cid:63155)(cid:63160)

(cid:63155)(cid:63161)

(cid:63155)(cid:63162)

(cid:63156)(cid:63153)

(cid:63156)(cid:63154)

(cid:63156)(cid:63155)

(cid:63156)(cid:63156)

(cid:63156)(cid:63157)

(cid:63156)(cid:63158)

(cid:63156)(cid:63159)

(cid:63156)(cid:63160)

torch_mlu_ops.fused_norm_residual_ﬀn

torch_mlu_ops.fused_rms_norm

torch_mlu_ops.fused_rope

torch_mlu_ops.group_gemm

torch_mlu_ops.matmul

torch_mlu_ops.matmul_allreduce

torch_mlu_ops.moe_active

torch_mlu_ops.moe_cast_gating

torch_mlu_ops.moe_combine_result

torch_mlu_ops.moe_expand_input

torch_mlu_ops.moe_gen_idx

torch_mlu_ops.moe_softmax_topk

torch_mlu_ops.oﬄine_quant_to_linear_cache

torch_mlu_ops.oﬄine_quant_to_paged_cache

torch_mlu_ops.preload

torch_mlu_ops.quant_to_linear_cache

torch_mlu_ops.quant_to_paged_cache

torch_mlu_ops.quantize

torch_mlu_ops.reshape_linear_cache

torch_mlu_ops.reshape_paged_cache

torch_mlu_ops.single_query_cached_kv_attn

torch_mlu_ops.single_query_mixed_cached_kv_attn

torch_mlu_ops.smooth_quant_matmul

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

下⻚继续

(cid:63154)(cid:63154)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

序号

(cid:63156)(cid:63161)

(cid:63156)(cid:63162)

(cid:63157)(cid:63153)

(cid:63157)(cid:63154)

表 5.1 – 续上⻚

算⼦名称

torch_mlu_ops.smooth_quant_matmul_allreduce

torch_mlu_ops.swap_blocks

torch_mlu_ops.update_out_and_lse

torch_mlu_ops.weight_only_quant_matmul

详细算⼦接⼝说明请参考以下章节。

5.1.1 torch_mlu_ops.active

torch_mlu_ops.active(input, act_mode, is_gated, active_coef)-> torch.Tensor

功能描述

激活算⼦。

公式说明

(cid:15) 公式如下：

(cid:15) 公式中各参数描述如下：

‒ input ：active算⼦的输⼊。

‒ is_gated：是否是gated结构。

(cid:15) 公式中各函数描述如下：

‒ active ：激活算⼦。

参数说明

C = input:shape[(cid:0)1]

if is_gated = T rue :

output = active(input[:::; : C//2]) (cid:3) input[:::; C//2 :]

else :

output = active(input)

(cid:15) input：Tensor类型，必选。输⼊维度 [..., C]。其中 C 为通道的个数，在最低维。

(cid:15) act_mode：string类型。可以是 silu ，gelu ，quick_gelu 或者 swish。

(cid:15) is_gated：Bool类型。当 is_gated=true 时，为gated激活。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63155)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) active_coef：FLOAT类型。默认值为(cid:63154).(cid:63153)，只有在act_mode为swish的情况下才需要额外设置。

返回值说明

(cid:15) output：激活后返回的tensor。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ output：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_add_bias_activation.py 。

5.1.2 torch_mlu_ops.apply_rotary

torch_mlu_ops.apply_rotary(input,sin_cache,cos_cache,position_ids,cu_seqlens,interleaved,discrete,dynamic_ntk,max_

,!seqlen)->Tensor

功能描述

计算Attention模块中将rotary embedding加在Q、K上的算⼦。

公式说明

请参考相关的GPT positon embedding⽂献：https://kexue.fm/archives/(cid:63161)(cid:63155)(cid:63159)(cid:63158)。该算⼦实现的主要功能为查表和加操作。

参数说明

(cid:15) input：Tensor类型，必选。Q或者K，pack模式下，维度 [total_seqlen, head_num, head_size]，⾮pack模式下，维

度 [batch_size, total_seqlen, head_num, head_size]，其中 head_num 表⽰transformer模块中的头数，head_size

表⽰每个头的维度。

(cid:15) sin_cache：Tensor类 型， 必 选。 输 ⼊ 计 算rotary embedding的sin表， 可 能 不 是 连 续。 当 dynamic_ntk=true 时，

维 度 [batch_size, rotary_seq_len, rotary_pos_emb_dim]； 当 dynamic_ntk=false 时， 维 度 [rotary_seq_len,

rotary_pos_emb_dim]。其中 rotary_seq_len ⼀般设置为⼤于等于 max_seqlen ，rotary_pos_emb_dim 在full rotary

情况下，⼀般等于 hidden_size，在partial rotary情况下，需要根据⽹络进⾏配置。

(cid:15) cos_cache：Tensor类 型， 必 选。 输 ⼊ 计 算rotary embedding的cos表， 可 能 不 是 连 续。 当 dynamic_ntk=true 时，

维 度 [batch_size, rotary_seq_len, rotary_pos_emb_dim]， 当 dynamic_ntk=false 时， 维 度 [rotary_seq_len,

rotary_pos_emb_dim]。

(cid:15) position_ids：Tensor类型，可选。当 discrete=true，维度 [total_seqlen] 当 discrete=false，维度 [batch_size] 。

(cid:15) cu_seqlens：Tensor类型，可选。在pack模式下，记录了input中每个batch的序列⻓度，维度 [batch + 1]。表⽰当

前batch的sequence⻓度与下⼀个batch的sequence⻓度的差值。如果不设置，则input中每个batch的序列⻓度为

max_seqlens。

(cid:15) interleaved：Bool类型，必选。表⽰rotary embedding的⽅式。当 interleaved=true 时，rotary embedding的⽅式

为 cross ，当 interleaved=false 时，rotary embedding的⽅式为 fold 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63156)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) discrete：Bool类型，必选。discrete表⽰position_ids是否是离散的。如果是离散的，需要给出每个token的完整的

position_ids，否则只需给出起始token的 position_ids ，算⼦内部根据起始 position_ids 和 seqlen 确定每个token

的 position_ids。默认情况下设置 discrete=False。

(cid:15) dynamic_ntk：Bool类型，必选。dynamic_ntk=true 时，表⽰所有的 batch 有不同的sin和cos值。当 dynamic_ntk=False

时，表⽰所有的 batch 有相同的sin和cos值。默认情况下设置 dynamic_ntk=False。

(cid:15) max_seqlen：Int类型，必选。context阶段，在pad模式下，等于 pad_seqlen ，pack模式下等于 cu_seqlens[-1]；generate

阶段，等于 valid_token.max() + 解码帧数。

返回值说明

(cid:15) output：Tensor类型，必选。rotary embedding加到Q或者K上的输出，维度 [total_seqlen, head_num, head_size]。

其中 total_seqlen pad模式下等于 batch*pad_seqlen，pack模式下等于 valid_seqlen 之和，即 cu_seqlens[-1]。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ sin_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ cos_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ position_ids：Int(cid:63156)(cid:63155)

‒ cu_seqlens：Int(cid:63156)(cid:63155)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

备注

(cid:15) 该算⼦中sin_cache/cos_cache的维度及⽣成⽅式和vLLM存在差异，具体表现在：

‒ rope 维 度：vLLM 中 sin_cache/cos_cache 的 rope 的 维 度 是 rotary_pos_emb_dim / 2,

torch_mlu_ops 中

sin_cache/cos_cache的rope的维度是完整的rotary_pos_emb_dim；

‒ ⽣成⽅式：

* vLLM 中 ⽣ 成 fold/cross 旋 转 ⻆ 度 公 式:

1.0 / (base ** (torch.arange(0, rotary_pos_emb_dim, 2) /

rotary_pos_emb_dim))；

* torch_mlu_ops中⽣成fold模式旋转⻆度公式: 1.0 / (base ** (torch.arange(0, rotary_pos_emb_dim, 1) %

half_rotary_pos_emb_dim / rotary_pos_emb_dim)), half_rotary_pos_emb_dim = rotary_pos_emb_dim // 2；

* torch_mlu_ops中⽣成cross模式旋转⻆度公式: 1.0 / (base ** (torch.arange(0, rotary_pos_emb_dim, 1) /

/ 2 * 2 / rotary_pos_emb_dim))。

(cid:15) 差异性带来的影响：

‒ 需要适配旋转⻆度的⽣成公式。

‒ ⽆论sin_cache/cos_cache是何种维度形式, 都是针对特定硬件平台的设计的。

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_apply_rotary.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63157)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.3 torch_mlu_ops.attention_project

torch_mlu_ops.attention_project(input,weight,bias,residual,alpha,beta)->Tensor

功能描述

计算Attention模块中的Q、K、V算⼦。适⽤于GPT或者 hidden_size ⼤于(cid:63155)(cid:63153)(cid:63157)(cid:63161)的transformer⽹络。

公式说明

(cid:15) 公式如下：

当 residual 存在时：

Q = f c(input; weight_q) + bias_q

K = f c(input; weight_k) + bias_k

V = f c(input; weight_v) + bias_v

Qout = alpha (cid:3) Q + beta (cid:3) residual

Kout = alpha (cid:3) K + beta (cid:3) residual

Vout = alpha (cid:3) V + beta (cid:3) residual

(cid:15) 公式中各参数描述如下：

‒ input ：attention_project算⼦的输⼊。

‒ weight_q、weight_k 、weight_v ：Query、Key、Value矩阵的权重。

‒ bias_q、bias_k 、bias_v ：Query、Key、Value矩阵的偏置。

(cid:15) 公式中各函数描述如下：

‒ fc ：全连接层（fully-connected）的缩写。

参数说明

(cid:15) input：Tensor类型，必选。attention_project 的输⼊。维度 [batch_size, seq_lens, input_size]。其中 batch_size

表⽰batch的⼤⼩，seq_lens 表⽰sequence的⻓度，input_size 表⽰输⼊的⼤⼩。

(cid:15) weight：Tensor类型，必选。attention_project 的矩阵权重参数。Q、K、V权重参数被pack在⼀起。在MHA场景下，维

度 [3*hidden_size, input_size]。在MQA场景下，维度 [(head_num+2*head_num_kv)*head_size, input_size]。其

中 hidden_size 表⽰隐层的⼤⼩。

(cid:15) bias：Tensor类型，可选。attention_project 的矩阵偏置参数。Q、K、V偏置参数被pack在⼀起。在MHA场景下，维

度 [3*hidden_size]。在MQA场景下，维度 [(head_num+2*head_num_kv)*head_size]。其中 head_num_kv 表⽰MQA场

景下kv的数量。

(cid:15) residual: Tensor类型，可选。attention_project 的参差参数。维度与 input 保持⼀致。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63158)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) alpha ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。当 residual 存在时，作为Q、K、V的系数。

(cid:15) beta ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。当 residual 存在时，作为 residual 的系数。

返回值说明

(cid:15) output: Tensor类型，必选。attention_project的输出。Q、K、V被pack在⼀起。在MHA场景下，维度[batch_size, seq_lens,

(cid:63156)*hidden_size]。在MQA场景下，维度[batch_size, seq_lens, (head_num+(cid:63155)*head_num_kv)*head_size]。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_attn_proj.py 。

5.1.4 torch_mlu_ops.batch_matmul

torch_mlu_ops.batch_matmul(input, ﬁlter, residual, alpha, beta, a_scale, b_scale, trans_a, trans_b)->

torch.Tensor

功能描述

通⽤的批量矩阵乘法算⼦。

公式说明

(cid:15) 批量matmul标准结构公式如下：

(cid:15) 公式中各参数描述如下：

‒ input ：matmul的左矩阵。

‒ filter：matmul的右矩阵。

‒ residual：参差。

‒ output ：计算最终的输出。

(cid:15) 公式中各函数描述如下：

‒ batch_matmul ：批量矩阵乘。

参数说明

mulout = batchmatmul(input; f ilter)

residual = alpha (cid:3) mulout + beta (cid:3) residual

output = residual

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63159)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) input：Tensor类型，必选。矩阵乘的左矩阵。维度 [batch, mat_m, mat_k]。

(cid:15) filter：Tensor类型，必选。矩阵乘的右矩阵。维度 [batch, mat_n, mat_k]。

(cid:15) residual：Tensor类型，可选。参差。维度 [batch, mat_m, mat_n]，如果有，计算结束后residual会被改变。

(cid:15) alpha：Float(cid:63156)(cid:63155)类型，必选。矩阵乘法系数。

(cid:15) beta：Float(cid:63156)(cid:63155)类型，必选。参差的系数。

(cid:15) a_scale：Float(cid:63156)(cid:63155)类型，默认值为(cid:63154).(cid:63153)。当input为INT(cid:63161)类型时对应的量化系数。

(cid:15) b_scale：Float(cid:63156)(cid:63155)类型，默认值为(cid:63154).(cid:63153)。当weight为INT(cid:63161)类型时对应的量化系数。

(cid:15) trans_a：Bool类型，默认值为False，标记input是否需要转置。

(cid:15) trans_b：Bool类型，默认值为True，标记weight是否需要转置。

返回值说明

(cid:15) out：Tensor类型，算⼦的输出，维度 [batch, mat_m, mat_n]。

规格限制

(cid:15) 数据类型⽀持：- input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161) - ﬁlter：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161) - residual：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

- out：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155) - alpha：FP(cid:63156)(cid:63155) - beta：FP(cid:63156)(cid:63155) - a_scale：FP(cid:63156)(cid:63155) - b_scale：FP(cid:63156)(cid:63155) - trans_a：BOOL - trans_b：

BOOL

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_batch_matmul.py 。

5.1.5 torch_mlu_ops.copy_blocks

torch_mlu_ops.copy_blocks(k_caches, v_caches, block_mapping)

功能描述

将 k_caches、v_caches 中源位置的BLOCK复制到指定位置。

公式说明

⽆。

参数说明

(cid:15) k_caches：Vector<Tensor>类型，必选。输⼊维度 [num_layers<num_blocks, num_heads, block_size, head_size>]。

其中 num_layers 表⽰layer数量，[num_heads, block_size, head_size] 整体⼤⼩作为每次需要copy的BLOCK⼤⼩。

num_blocks 表⽰BLOCK数量；block_size 表⽰输⼊序列中的特定块或⼦序列⼤⼩；num_heads 表⽰transformer模块中

的头数；head_size 表⽰每个头的维度。

(cid:15) v_caches：Vector<Tensor>类型，必选。输⼊维度 [num_layers<num_blocks, num_heads, head_size, block_size>]。

其中每个参数含义与 k_caches ⼀致。

(cid:15) block_mapping：Map<int(cid:63159)(cid:63157), vector<int(cid:63159)(cid:63157)>>类型，必选。第⼀个int(cid:63159)(cid:63157)表⽰需要copy的源BLOCK位置（⼩于num_blocks），

第⼆个int(cid:63159)(cid:63157)表⽰将源BLOCK copy到的⽬标位置（⼩于num_blocks），vector 表⽰⼀个源BLOCK可以copy到多个⽬标

位置。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63160)

(cid:63158). 重点特性

返回值说明

(cid:15) ⽆，原位操作。

规格限制

(cid:15) 数据类型⽀持：

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ k_caches：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、UINT(cid:63161)、INT(cid:63154)(cid:63159)、INT(cid:63156)(cid:63155)、INT(cid:63159)(cid:63157)

‒ v_caches：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、UINT(cid:63161)、INT(cid:63154)(cid:63159)、INT(cid:63156)(cid:63155)、INT(cid:63159)(cid:63157)

‒ block_mapping：INT(cid:63159)(cid:63157)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_copy_blocks.py 。

5.1.6 torch_mlu_ops.copy_blocks_out_of_place

torch_mlu_ops.copy_blocks_out_of_place(k_caches, v_caches, block_mapping)->Tuple(k_caches, v_caches)

功能描述

将 k_caches、v_caches 中源位置的BLOCK复制到指定位置。

公式说明

⽆。

参数说明

(cid:15) k_caches：Vector<Tensor>类型，必选。输⼊维度 [num_layers<num_blocks, num_heads, block_size, head_size>]。

其中 num_layers 表⽰layer数量，[num_heads, block_size, head_size] 整体⼤⼩作为每次需要copy的BLOCK⼤⼩。

num_blocks 表⽰BLOCK数量；block_size 表⽰输⼊序列中的特定块或⼦序列⼤⼩；num_heads 表⽰transformer模块中

的头数；head_size 表⽰每个头的维度。

(cid:15) v_caches：Vector<Tensor>类型，必选。输⼊维度 [num_layers<num_blocks, num_heads, head_size, block_size>]。

其中每个参数含义与 k_caches ⼀致。

(cid:15) block_mapping：Map<int(cid:63159)(cid:63157), vector<int(cid:63159)(cid:63157)>>类型，必选。第⼀个int(cid:63159)(cid:63157)表⽰需要copy的源BLOCK位置（⼩于num_blocks），

第⼆个int(cid:63159)(cid:63157)表⽰将源BLOCK copy到的⽬标位置（⼩于num_blocks），vector 表⽰⼀个源BLOCK可以copy到多个⽬标

位置。

返回值说明

(cid:15) k_caches, v_caches。

规格限制

(cid:15) 数据类型⽀持：

‒ k_caches：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、UINT(cid:63161)、INT(cid:63154)(cid:63159)、INT(cid:63156)(cid:63155)、INT(cid:63159)(cid:63157)

‒ v_caches：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、UINT(cid:63161)、INT(cid:63154)(cid:63159)、INT(cid:63156)(cid:63155)、INT(cid:63159)(cid:63157)

‒ block_mapping：INT(cid:63159)(cid:63157)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63161)

(cid:63158). 重点特性

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_copy_blocks.py 。

5.1.7 torch_mlu_ops.dequant_from_linear_cache

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

torch_mlu_ops.dequant_from_linear_cache(key,value,key_cache,value_cache,key_cache_scale,value_cache_scale,context_

,!lengths,max_context_len,context_seq_offset,cache_bs_id,cache_seqlen_offset,quant_mode,quant_bit)

功能描述

将计算得到的K-V cache拼接到K-V上的反量化算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类型，必选。Transformer模块中计算得到的 Key。在context阶段，维度为 [total_seq_len, head_num,

head_size]。

(cid:15) value：Tensor类型，可选。Transformer模块中计算得到的 Value。在context阶段，维度为 [total_seq_len, head_num,

head_size]。

(cid:15) key_cache：Tensor类型，必选。Transformer模块中保存 Key cache 的Tensor。当 quant_bit 为(cid:63161)时，维度为 [max_batch,

head_num, cache_mem_len, head_size]； 当 quant_bit 为(cid:63157)时， 维 度 为 [max_batch, head_num, cache_mem_len,

head_size//2]。其中 max_batch 要求⼤于等于 batch_size，cache_mem_len 为⽹络⽀持的最⼤sequences⻓度。

(cid:15) value_cache：Tensor类 型， 可 选。Transformer模 块 中 保 存 Value cache 的Tensor。 当 quant_bit 为(cid:63161)时， 维 度

为 [max_batch, head_num, cache_mem_len, head_size]； 当 quant_bit 为 (cid:63157) 时， 维 度 为 [max_batch, head_num,

cache_mem_len//2, head_size]。

(cid:15) key_cache_scale：Tensor类型，必选。Transformer模块中保存的 Key 的 scale 值。在context阶段，维度为 per-channel

量化 [head_num, head_size] 或者 per-head 量化 [max_batch, head_num, cache_mem_len]。

(cid:15) value_cache_scale：Tensor类 型， 可 选。Transformer模 块 中 保 存 的 Value 的 scale 值。 在context阶 段， 维 度 为

per-channel 量化 [head_num, head_size] 或者 per-head 量化 [max_batch, head_num, cache_mem_len]。

(cid:15) context_lengths：Tensor类型，必选。为每个batch的sequences length⻓度。

(cid:15) max_context_len：Int类型，必选。在context阶段，表⽰最⼤的sequence length。

(cid:15) context_seq_offset：Tensor类型，可选。表⽰context的seq的累积偏移，需要保证每个batch的sequences不重合，维

度 [batch_size]。如果为空，则默认为context_lens的累加和。

(cid:15) cache_bs_id：Tensor类型，可选。表⽰cache的batch的偏置，维度 [batch_size]。如果为空，则默认值为{(cid:63153), (cid:63154), (cid:63155) ...

batch - (cid:63154)}。

(cid:15) cache_seq_offset：Tensor类型，可选。表⽰cache的seq_lens的偏置，维度 [batch_size]。如果为空，则默认值为{(cid:63153),

(cid:63153), (cid:63153) ... (cid:63153)}。

(cid:15) quant_mode: Int类型，可选。表⽰量化的模式，可选(cid:63153)（表⽰per-channel）或者(cid:63154)（表⽰per-head），默认值为(cid:63153)。

(cid:15) quant_bit：Int类型，可选。表⽰量化结果的位宽，可选(cid:63161)（表⽰int(cid:63161)）或(cid:63157)（表⽰int(cid:63157)），默认值为(cid:63161)。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63154)(cid:63162)

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:63158). 重点特性

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache：INT(cid:63161)

‒ value_cache：INT(cid:63161)

‒ key_cache_scale：FP(cid:63156)(cid:63155)

‒ value_cache_scale：FP(cid:63156)(cid:63155)

‒ context_lengths：INT(cid:63156)(cid:63155)

‒ max_context_len：INT(cid:63156)(cid:63155)

‒ context_seq_oﬀset：INT(cid:63156)(cid:63155)

‒ cache_bs_id：INT(cid:63156)(cid:63155)

‒ cache_seqlen_oﬀset：INT(cid:63156)(cid:63155)

‒ quant_mode: INT(cid:63156)(cid:63155)

‒ quant_bit: INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_dequant_from_linear_cache.py 。

5.1.8 torch_mlu_ops.dequant_from_paged_cache

torch_mlu_ops.dequant_from_paged_cache(key,value,key_cache,value_cache,key_cache_scale,value_cache_scale,context_

,!lengths,max_context_len,context_seq_offset,block_tables,quant_mode,quant_bit)

功能描述

将计算得到的K-V cache（Paged模式）拼接到K-V上的反量化算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类型，必选。Transformer模块中计算得到的 Key。在context阶段，维度为 [total_seq_len, head_num,

head_size]。

(cid:15) value：Tensor类型，可选。Transformer模块中计算得到的 Value。在context阶段，维度为 [total_seq_len, head_num,

head_size]。

(cid:15) key_cache：Tensor类型，必选。Transformer模块中保存 Key cache 的Tensor。当 quant_bit 为(cid:63161)时，维度为 [block_num,

head_num, block_size, head_size]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63153)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) value_cache：Tensor类 型， 可 选。Transformer模 块 中 保 存 Value cache 的Tensor。 当 quant_bit 为(cid:63161)时， 维 度 为

[block_num, head_num, block_size, head_size]。

(cid:15) key_cache_scale：Tensor类型，必选。Transformer模块中保存的 Key 的 scale 值。在context阶段，维度为 per-channel

量化 [head_num, head_size] 或者 per-head 量化 [block_num, head_num, block_size]。

(cid:15) value_cache_scale：Tensor类 型， 可 选。Transformer模 块 中 保 存 的 Value 的 scale 值。 在context阶 段， 维 度 为

per-channel 量化 [head_num, head_size] 或者 per-head 量化 [block_num, head_num, block_size]。

(cid:15) context_lengths：Tensor类型，必选。为每个batch的sequences length⻓度。

(cid:15) max_context_len：Int类型，必选。在context阶段，表⽰最⼤的sequence length。

(cid:15) context_seq_offset：Tensor类型，可选。表⽰context的seq的累积偏移，需要保证每个batch的sequences不重合，维

度 [batch_size]。如果为空，则默认为context_lens的累加和。

(cid:15) block_tables：Tensor类型，可选。表⽰cache所在的block序号，维度 [batch_size, max_block_num]。

(cid:15) quant_mode: Int类型，可选。表⽰量化的模式，可选(cid:63153)（表⽰per-channel）或者(cid:63154)（表⽰per-head），默认值为(cid:63153)。

(cid:15) quant_bit：Int类型，可选。表⽰量化结果的位宽，可选(cid:63161)（表⽰int(cid:63161)），默认值为(cid:63161)。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache：INT(cid:63161)

‒ value_cache：INT(cid:63161)

‒ key_cache_scale：FP(cid:63156)(cid:63155)

‒ value_cache_scale：FP(cid:63156)(cid:63155)

‒ context_lengths：INT(cid:63156)(cid:63155)

‒ max_context_len：INT(cid:63156)(cid:63155)

‒ context_seq_oﬀset：INT(cid:63156)(cid:63155)

‒ block_tables: INT(cid:63156)(cid:63155)

‒ quant_mode: INT(cid:63156)(cid:63155)

‒ quant_bit: INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_dequant_from_paged_cache.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63154)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.9 torch_mlu_ops.ffn

torch_mlu_ops.ffn(input,up_fc_weight,up_fc_bias,down_proj_weight,down_proj_bias,gate_up_proj_weight,gate_up_proj_

,!bias,act_mode)->Tensor

功能描述

计算transformer中的FFN（FeedForward）前馈神经⽹络模块的算⼦。适⽤于GPT或者 hidden_size ⼤于(cid:63155)(cid:63153)(cid:63157)(cid:63161)的transformer

⽹络。

公式说明

(cid:15) FFN标准结构公式如下：

f c_output = f c(input; up_f c_weight) + up_f c_bias

active_output = activation(f c_output)

basic_output = f c(active_output; down_proj_weight) + down_proj_bias

(cid:15) Gated FFN标准结构公式如下：

f c_output = f c(input; up_f c_weight) + up_f c_bias

active_output = activation(f c_output)

gatedf c_output = f c(input; gate_up_proj_weight) + gate_up_proj_bias

elementwise_product_output = elementwise_product(active_output; gated_f c_output)

gated_output = f c(elementwise_product_output; down_proj_weight) + down_proj_bias

(cid:15) 公式中各参数描述如下：

‒ input ：FFN模块的输⼊。

‒ fc_output ：FFN模块经过升维后的输出。

‒ active_output ：FFN模块经过激活后的输出。

‒ basic_output ：标准FFN模块经过降维后的输出。

‒ gated_fc_output ：gated FFN旁路经过升维后的输出。

‒ elementwise_product_output ：gated FFN主路和旁路经过elementwise product后的输出。

‒ gated_output ：gated FFN模块经过降维后的输出。

(cid:15) 公式中各函数描述如下：

‒ fc ：全连接层（fully-connected）的缩写。

‒ activation ：⽤⼾可配的激活函数，可选的激活函数："relu"、"gelu"、"silu"。

‒ elementwise_product ：点积。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63155)

(cid:63158). 重点特性

参数说明

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) input：Tensor类型，必选。FFN算⼦的输⼊。维度[batch_size, seq_lens, hidden_size]。

(cid:15) up_fc_weight：Tensor类型，必选。FFN算⼦中升维矩阵权重参数，维度[ﬀn_inner_size, hidden_size]。

(cid:15) up_fc_bias：Tensor类型，可选。FFN算⼦中升维矩阵偏置参数，维度[ﬀn_inner_size]。其中ﬀn_inner_size表⽰FFN中

间层的维度。

(cid:15) down_proj_weight：Tensor类型，必选。FFN算⼦中降维矩阵权重参数，维度[hidden_size, ﬀn_inner_size]。

(cid:15) down_proj_bias：Tensor类型，可选。FFN算⼦中降维矩阵偏置参数，维度[hidden_size]。

(cid:15) gate_up_proj_weight：Tensor类型，可选。Gated FFN算⼦中升维矩阵权重参数，维度[hidden_size, ﬀn_inner_size]。

(cid:15) gate_up_proj_bias：Tensor类型，可选。Gated FFN算⼦中升维矩阵偏置参数，维度[hidden_size]。

(cid:15) act_mode：string类型，必选。FFN的激化函数类型，可选的激活函数："relu"、"gelu"、"silu"、"none"。

返回值说明

(cid:15) output: Tensor类型，必选。ﬀn的输出。维度[batch_size, seq_lens, hidden_size]。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ up_fc_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ up_fc_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ down_proj_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ down_proj_bias: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ gate_up_proj_weight: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ gate_up_proj_bias: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ output: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_ffn.py 。

5.1.10 torch_mlu_ops.flash_attention

torch_mlu_ops.flash_attention(q,k,v,out,cu_seq_lens_q,cu_seq_lens_kv,alibi_slope,attn_bias,k_cache_quant_scale,v_

,!cache_quant_scale,block_tables,max_seq_len_q,max_seq_len_kv,softmax_scale,is_causal,window_size_left,window_

,!size_right,compute_dtype,return_lse)

功能描述

Context阶段的Flash Attention算⼦。

公式说明

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63156)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) ﬂash attention标准结构公式如下：

scores = batch_matmul(q; k; transpose_b = T rue) (cid:3) sof tmax_scale

scores+ = ALiBi + attn_bias + causal_mask + window_mask

output_lse = logsumexp(scores)

attention = sof tmax(scores)

output = batch_matmul(attention; v)

(cid:15) 公式中各参数描述如下：

‒ q/k/v ：ﬂash attention模块的输⼊。

‒ attn_bias：ﬂash attention模块的偏置。

‒ ALiBi：ﬂash attention模块在线计算的ALiBi位置编码矩阵。

‒ causal_mask：如果是因果attention的，则需要添加mask。

‒ window_mask：如果设置了window的尺⼨，则需要添加mask。

‒ scores ：ﬂash attention模块第⼀个批量矩阵乘的输出。

‒ output_lse：ﬂash_attention的lse输出。

‒ attention ：ﬂash attention模块经过softmax后的输出。

‒ output ：ﬂash attention模块第⼆个批量矩阵乘的输出，也是模块的最终输出。

(cid:15) 公式中各函数描述如下：

‒ batch_matmul ：批量矩阵乘。

‒ softmax ：归⼀化指数函数。

‒ logsumexp ：对数求和指数函数。

参数说明

(cid:15) q：Tensor类型，必选。ﬂash attention算⼦的输⼊。维度 [total_q, q_head_num, head_size_qk]。

(cid:15) k：Tensor 类 型， 必 选。ﬂash attention 算 ⼦ 的 输 ⼊。 当 block_tables 为 空 时， 维 度 [total_k, kv_head_num,

head_size_qk]，否则维度 [num_blocks, kv_head_num, block_size, head_size_qk]。

(cid:15) k：Tensor类型，必选。ﬂash attention算⼦的输⼊。当block_tables为空时，维度 [total_k, kv_head_num, head_size_v]，

否则维度 [num_blocks, kv_head_num, block_size, head_size_v]。

(cid:15) out：Tensor类型，可选。ﬂash attention算⼦的原位输⼊。维度同input。

(cid:15) cu_seq_lens：Tensor类型，可选。当输⼊q/k/v为pack模式时候记录每个batch的seq的位置，维度 [batch+1]。

(cid:15) alibi_slope：Tensor类型，可选。是⼀种相对位置编码⽅法，维度 [batch, q_head_num]``或者 ``[q_head_num]，类

型必须是ﬂoat(cid:63156)(cid:63155)。

(cid:15) attn_bias：Tensor类型，可选。attention的偏置，维度 [batch, max_seq_len, max_seq_len] 或者 [batch, q_head_num,

max_seq_len, max_seq_len]。

(cid:15) k_cache_quant_scale：Tensor类型，预留参数，必须为空。

(cid:15) v_cache_quant_scale：Tensor类型，预留参数，必须为空。

(cid:15) block_tables：Tensor类 型， 可 选。block的id与kv_cache之 间 的 映 射 关 系， 为 空 时 表 ⽰ ⾮paged模 式。 维 度 [bs,

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63157)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

max_num_blocks_per_seq]，max_num_blocks_per_seq为(cid:63154)时block_size=max(seq_len_kv)。

(cid:15) max_seq_len：Int(cid:63156)(cid:63155)类型，必选。算⼦输⼊的最⼤seq⻓度。

(cid:15) softmax_scale：Float(cid:63156)(cid:63155)类型，必选。softmax之前的系数。

(cid:15) is_causal：Bool类型，标记是否是因果的，如果为因果的则计算过程中需要使⽤ causal_mask。

(cid:15) window_size_left：Int(cid:63156)(cid:63155)类 型, 左 侧window size⼤ ⼩， ⽆ 限 制 则 设 置 为-(cid:63154)。 实 际 参 与 计 算 的key和value⻓ 度 为

window_size_left + 1。

(cid:15) window_size_right：Int(cid:63156)(cid:63155)类 型， 右 侧window size⼤ ⼩， ⽆ 限 制 则 设 置 为-(cid:63154)。 实 际 参 与 计 算 的key和value⻓ 度 为

window_size_right + 1。

(cid:15) return_lse：Bool类型，标记是否返回lse tensor。

返回值说明

(cid:15) output：Tensor类型，可选。ﬂash attention的输出，当输⼊的out不为空时⽆此输出。维度 [total_q, q_head_num,

head_size_v]。

(cid:15) output_lse：Tensor类 型， 可 选。ﬂash attention的 输 出， 当return_lse为true时 有 此 输 出， 维 度 [batch, head,

max_seq_len_q]。

规格限制

(cid:15) 数据类型⽀持：

‒ q/k/v：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ cu_seq_lens：INT(cid:63156)(cid:63155)

‒ alibi_slope：FP(cid:63156)(cid:63155)

‒ attn_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ block_tables：INT(cid:63156)(cid:63155)

‒ max_seq_len：INT(cid:63156)(cid:63155)

‒ softmax_scale：FP(cid:63156)(cid:63155)

‒ is_causal：BOOL

‒ window_size_left：INT(cid:63156)(cid:63155)

‒ window_size_right：INT(cid:63156)(cid:63155)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_flash_attention.py。

5.1.11 torch_mlu_ops.flash_attn_sq_mm_allreduce

torch_mlu_ops.flash_attn_sq_mm_allreduce(cncl_comm, q, k, v, cu_seq_lens_q, cu_seq_lens_kv,

alibi_slope, attn_bias, smooth, weight, weight_scale,

bias, max_seq_len_q, max_seq_len_kv, softmax_scale,

is_causal, window_size_left, window_size_right,

compute_dtype, block_seq)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63158)

(cid:63158). 重点特性

功能描述

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

Context阶段的Flash Attention、smooth_quant matmul与allreduce通信计算融合算⼦。

公式说明

⽆

参数说明

(cid:15) cncl_comm ：Int类型，cnclComm，⽤于归约通信。

(cid:15) q：Tensor类型，必选。ﬂash attention算⼦的输⼊。维度 [total_q, q_head_num, head_size]。

(cid:15) k/v：Tensor类型，必选。ﬂash attention算⼦的输⼊。维度 [total_k, kv_head_num, head_size]。

(cid:15) cu_seq_lens_q：Tensor类型，可选。当输⼊q为pack模式时候记录每个batch的seq的位置，维度 [batch + 1]。

(cid:15) cu_seq_lens_kv：Tensor类型，可选。当输⼊k/v为pack模式时候记录每个batch的seq的位置，维度 [batch + 1]。

(cid:15) alibi_slope：Tensor类型，可选。是⼀种相对位置编码⽅法，维度 [batch, q_head_num] 或者 [q_head_num]，类型必

须是ﬂoat(cid:63156)(cid:63155)。

(cid:15) attn_bias：Tensor类型，可选。attention的偏置，维度 [batch, max_seq_len, max_seq_len] 或者 [batch, q_head_num,

max_seq_len, max_seq_len]。

(cid:15) smooth：Tensor类型，必选。平滑系数, 维度 [head_num_q * head_size]。

(cid:15) weight：Tensor类型，必选。, smooth_quant matmul的权重矩阵，维度 [head_num_q * head_size, head_num_q *

head_size]。

(cid:15) weight_scale：Tensor类型，必选。, smooth_quant matmul的权重矩阵的量化参数，维度 [head_num_q * head_size]。

(cid:15) bias：Tensor类型，可选。, smooth_quant matmul的偏置，维度 [head_num_q * head_size]。

(cid:15) max_seq_len_q：Int(cid:63156)(cid:63155)类型，必选。q的最⼤seq⻓度。

(cid:15) max_seq_len_kv：Int(cid:63156)(cid:63155)类型，必选。k/v的最⼤seq⻓度。

(cid:15) softmax_scale：Float(cid:63156)(cid:63155)类型，必选。softmax之前的系数。

(cid:15) is_causal：Bool类型，标记是否是因果的，如果为因果的则计算过程中需要使⽤ causal_mask。

(cid:15) window_size_left：Int(cid:63156)(cid:63155)类型, 左侧window size⼤⼩，⽆限制则设置为-(cid:63154)。

(cid:15) window_size_right：Int(cid:63156)(cid:63155)类型, 右侧window size⼤⼩，⽆限制则设置为-(cid:63154)。

(cid:15) compute_dtype：torch类型，在计算softmax时使⽤。

(cid:15) block_seq: Int类型，保留字段，设置(cid:63153)即可。

返回值说明

(cid:15) output：Tensor类型，必选。输出，维度 [total_q, q_head_num * head_size]。

规格限制

(cid:15) 数据类型⽀持：

‒ cncl_comm: INT(cid:63159)(cid:63157)

‒ q/k/v：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ cu_seq_lens_q：INT(cid:63156)(cid:63155)

‒ cu_seq_lens_kv：INT(cid:63156)(cid:63155)

‒ alibi_slope：FP(cid:63156)(cid:63155)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63159)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ attn_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ smooth：FP(cid:63156)(cid:63155)

‒ weight：INT(cid:63161)

‒ weight_scale：FP(cid:63156)(cid:63155)

‒ bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ max_seq_len_q：INT(cid:63156)(cid:63155)

‒ max_seq_len_kv：INT(cid:63156)(cid:63155)

‒ softmax_scale：FP(cid:63156)(cid:63155)

‒ is_causal：BOOL

‒ window_size_left：INT(cid:63156)(cid:63155)

‒ window_size_right：INT(cid:63156)(cid:63155)

‒ block_seq：INT(cid:63156)(cid:63155)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/allreduce_case/test_flash_attn_sq_mm_allreduce.py。

5.1.12 torch_mlu_ops.fused_layer_norm

torch_mlu_ops.fused_layer_norm(input,residual,gamma,beta,bias,eps,store_output_before_norm,quant_scale,out,

,!dynamic_quant)

功能描述

layernorm归⼀化算⼦。

公式说明

(cid:15) layernorm标准结构公式如下：

output = weight (cid:2) input (cid:0) M ean(input)
√
V ariance(input) + eps

+ bias

output = Quantize(output; quantscale)

(cid:15) 公式中各参数描述如下：

‒ input layernorm的输⼊。

‒ Mean() 表⽰对input取均值。

‒ Variance() 表⽰对input取⽅值。

‒ Quantize() 表⽰对输出做量化。

‒ weight 表⽰layernorm的 weight 参数。

‒ bias 表⽰layernorm的 bias 参数。

‒ eps 表⽰layernorm eps的值，⼀个极⼩的正数，避免除零，⼀般默认值设定为(cid:63153).(cid:63153)(cid:63153)(cid:63153)(cid:63153)(cid:63154)。

‒ output layernorm的输出。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63160)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ quant_scale 量化参数。

参数说明

(cid:15) input：Tensor类型，必选。layernorm算⼦的输⼊。维度 [batch，seq_lens, hidden_size]，⽀持batch维度和seq_lens

维度的stride。

(cid:15) residual：Tensor类型，可选。layernorm算⼦的残差输⼊。在pad模式下，维度 [batch，seq_lens, hidden_size]，⽀

持batch维度和seq_lens维度的stride。

(cid:15) gamma：Tensor类型，可选。layernorm算⼦的 weight 。维度 [hidden_size]。

(cid:15) beta：Tensor类型，可选。layernorm算⼦的 bias 。维度 [hidden_size]。

(cid:15) bias：Tensor类型，可选。layernorm算⼦的残差的偏置输⼊。维度 [hidden_size]。

(cid:15) eps：Float类型，必选。layernorm算⼦的 eps 的值，⼀般设定为(cid:63153).(cid:63153)(cid:63153)(cid:63153)(cid:63153)(cid:63154)。

(cid:15) store_output_before_norm：Bool类型，必选。layernorm算⼦是否输出 input+residual 的值。当为true时，第⼆个返

回值输出 input+residual，当为false时，⽆第⼆个返回值。

(cid:15) quant_scale：Float类型，可选。量化参数。维度 [hidden_size]。当 dynamic_quant 为False时表⽰离线量化参数，为

True时表⽰在线量化时的平滑因⼦。当该参数不为空时，输⼊输出必须连续。

(cid:15) out：Tensor类型，可选。layernorm算⼦的原位输出。维度同input，⽀持batch维度和seq_lens维度的stride。

(cid:15) dynamic_quant：Bool类型，为False且 quant_scale 不为空时表⽰离线per-channel量化输出，否则为在线动态per-token

量化输出。

返回值说明

(cid:15) output：Tensor类型，可选。layernorm算⼦的输出，当输⼊参数out不为空时⽆此输出。维度 [batch_size，seq_lens,

hidden_size]，⽀持batch维度和seq_lens维度的stride。

(cid:15) residual_output：Tensor类型，可选。layernorm算⼦的residual输出。当store_output_before_norm为true时，返回值输

出 input+residual，当store_output_before_norm为false时，⽆此返回值。在pad模式下，维度 [batch_size*seq_lens,

hidden_size]；在pack模式下，维度 [total_seqlen, hidden_size]。

(cid:15) smooth_quant_scale：Tensor类型，可选。维度 [batch_size*seq_lens] 。当 dynamic_quant 为True时输出的量化scale。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ residual：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ beta：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ gamma：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ eps：Float(cid:63156)(cid:63155)

‒ store_output_before_norm：Bool

‒ quant_scale：Float

‒ dynamic_quant：Bool

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、int(cid:63161)

‒ residual_output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63161)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ smooth_quant_scale：FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_fusenorm.py 。

5.1.13 torch_mlu_ops.fused_moe

torch_mlu_ops.fused_moe(input, router_logit, w1, w2, bias1, bias2,

residual, input_smooth, act_smooth, w1_scale,

w2_scale, topk, renormalize, gated, act_mode,

start_expert_id, block_n, cncl_comm, w1_quant_flag, w2_quant_flag)

功能描述

mixtral of experts, FFN（FeedForward）前馈神经⽹络模块的平替层，⽀持通信计算融合模式。

公式说明

⽆

参数说明

(cid:15) input：Tensor类 型， 必 选。MOEFFN算 ⼦ 的 输 ⼊。 维 度 [batch_size, seq_lens, hidden_size] 或 [total_seq,

hidden_size]。

(cid:15) router_logit：Tensor类 型， 必 选。MOEFFN算 ⼦ 中expert的 得 分。 维 度 [batch_size, seq_lens, expert_num] 或

[batch_size, seq_lens, expert_num]。

(cid:15) w1：Tensor类 型， 必 选。MOEFFN算 ⼦ 中 升 维 矩 阵 权 重 参 数。 维 度 [expert_size, ffn_inner_size*(1+gated),

hidden_size] 或 [expert_size, ffn_inner_size*(1+gated), hidden_size / 2]。

(cid:15) w2：Tensor类型，必选。MOEFFN算⼦中降维矩阵权重参数。维度 [expert_size, hidden_size, ffn_inner_size]

或 [sub_expert_size, hidden_size, block, ffn_inner_size] 或 [expert_size, hidden_size, ffn_inner_size /

2]。

(cid:15) bias1：Tensor类型，可选。MOEFFN算⼦中升维矩阵偏置参数。维度 [expert_size, ffn_inner_size*(1+gated)]。

(cid:15) bias2：Tensor类型，可选。MOEFFN算⼦中降维矩阵偏置参数。维度 [expert_size, hidden_size]。

(cid:15) residual：Tensor类型，可选。MOEFFN算⼦的残差。维度 [batch_size, seq_lens, hidden_size] 或 [total_seq,

hidden_size]。

(cid:15) input_smooth：Tensor类型，可选。MOEFFN算⼦smooth_quant模式时input的平滑系数。维度 [hidden_size]。

(cid:15) act_smooth：Tensor类型，可选。MOEFFN算⼦smooth_quant模式时激活操作之后的平滑系数。维度 [ffn_inner_size]。

(cid:15) w1_scale：Tensor 类 型， 可 选。MOEFFN 算 ⼦ 量 化 模 式 时 升 维 权 重 的 量 化 参 数。 维 度 [expert_size,

ffn_inner_size*(1+gated)] 或 [w1_quant_group, expert_size, ffn_inner_size*(1+gated)]。

(cid:15) w2_scale：Tensor类型，可选。MOEFFN算⼦量化模式时降维权重的量化参数。维度 [expert_size, hidden_size] 或

[w2_quant_group, expert_size, hidden_size]。

(cid:15) topk：Int类型，必选。MOEFFN算⼦中，从每个token的expert_logits中选择topk个。

(cid:15) renormalize：Bool类型，必选。MOEFFN算⼦中softmax结果后是否需要归⼀化。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63155)(cid:63162)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) gated：Bool类型，必选。MOEFFN层中是否有gated结构。

(cid:15) act_mode：string类型，必选。MOEFFN的激化函数类型，可选的激活函数：gelu、silu。

(cid:15) start_expert_id：Int类型，可选。⽤于表明在专家并⾏模式下专家的起始位置。

(cid:15) block_n：Int类型，可选。⽤于表明在通算融合模式下group_gemm和allreduce的拆分次数。

(cid:15) cncl_comm：Int类型，可选。⽤于表明在通算融合模式下的通信句柄。

(cid:15) w1_quant_flag：List类型，可选。MOEFFN算⼦w(cid:63157)w(cid:63161)混合量化模式时标记quant_group的bit位，数值必须是(cid:63157)或(cid:63161)，其

它值会引发错误。⻓度 [expert_size * w1_quant_group]。

(cid:15) w2_quant_flag：List类型，可选。MOEFFN算⼦w(cid:63157)w(cid:63161)混合量化模式时标记quant_group的bit位，数值必须是(cid:63157)或(cid:63161)，其

它值会引发错误。⻓度 [expert_size * w2_quant_group]。

返回值说明

(cid:15) output: Tensor类型。维度 [batch_size, seq_lens, hidden_size] 或 [total_seq, hidden_size]。

规格限制

(cid:15) 参数限制：

‒ input_smooth/act_smooth/w(cid:63154)_scale/w(cid:63155)_scale: 只有smooth quant pertoken模式启⽤，input采⽤pertoken量化，

权重采⽤per-channel量化。

(cid:15) 数据类型⽀持：

‒ input：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ router_logit：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ w(cid:63154)：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、INT(cid:63161)(INT(cid:63157)x(cid:63155))

‒ w(cid:63155)：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、INT(cid:63161)(INT(cid:63157)x(cid:63155))

‒ bias(cid:63154)：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ bias(cid:63155)：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ residual：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ input_smooth：FP(cid:63156)(cid:63155)

‒ act_smooth：FP(cid:63156)(cid:63155)

‒ w(cid:63154)_scale：FP(cid:63156)(cid:63155)

‒ w(cid:63155)_scale：FP(cid:63156)(cid:63155)

‒ start_expert_id: INT(cid:63156)(cid:63155)

‒ block_n: INT(cid:63156)(cid:63155)

‒ cncl_comm: INT(cid:63159)(cid:63157)

‒ output：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ w(cid:63154)_quant_ﬂag: INT

‒ w(cid:63155)_quant_ﬂag: INT

备注

(cid:15) w(cid:63155)维度是(cid:63157)D时，sub_expert_size * block 必须等于 expert_size, 并且要求ﬀn_inner_size是(cid:63154)(cid:63153)(cid:63155)(cid:63157)的整数倍。

(cid:15) group 量 化 时，w(cid:63154)_quant_group * w(cid:63154)_quant_group_size 必 须 等 于 hidden_size，w(cid:63155)_quant_group *

w(cid:63155)_quant_group_size 必须等于 ﬀn_inner_size。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63153)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) group量化时，w(cid:63154)_scale的形状必须是 (w(cid:63154)_quant_group, expert_size, ﬀn_inner_size*((cid:63154)+gated))，w(cid:63155)_scale的形状必

须是 (w(cid:63155)_quant_group, expert_size, hidden_size)。

(cid:15) w(cid:63154)_quant_group_size与w(cid:63155)_quant_group_size表⽰group量化场景下⼀个group block的元素数量。必须是(cid:63154)(cid:63155)(cid:63161)的整

数倍。

(cid:15) w(cid:63155)维度是(cid:63157)D时不⽀持int(cid:63157)量化。

(cid:15) 通算融合模式不⽀持int(cid:63157)量化。

(cid:15) w(cid:63154)_quant_ﬂag 与 w(cid:63155)_quant_ﬂag存在时为w(cid:63157)w(cid:63161)混合量化模式，其元素只能是(cid:63157)或(cid:63161)。(cid:63157)表⽰该quant_group对应的权

重是int(cid:63157)量化，(cid:63161)表⽰该quant_group对应的权重是int(cid:63161)量化。

(cid:15) w(cid:63157)w(cid:63161)混合量化模式要求w(cid:63154)与w(cid:63155)必须是(cid:63154)D。

使⽤⽰例

请 参 考 torch_mlu_ops/tests/ops_pytest/test_moe.py 和 torch_mlu_ops/tests/ops_pytest/allreduce_case/

test_moe_all_reduce.py。

5.1.14 torch_mlu_ops.fused_norm_attention_project

torch_mlu_ops.fused_norm_attention_project(input,q_weight,q_bias,k_weight,k_bias,v_weight,v_bias,norm_weight,norm_

,!bias,eps,out_layout,head_size,norm_out)->Tuple(Tensor)

功能描述

计算Attention模块中的Q、K、V算⼦。前融合 layernorm 的 attention_project``算⼦。适⽤于GPT或者 ``hidden_size

⼩于(cid:63155)(cid:63153)(cid:63157)(cid:63161)的transformer⽹络。

公式说明

(cid:15) 公式如下：

normed = layernorm(input; normweight; normbias; eps)Q = f c(normed; weight_q) + bias_q

K = f c(normed; weight_k) + bias_k

V = f c(normed; weight_v) + bias_v

(cid:15) 公式中各参数描述如下：

‒ input ：fused_norm_attention_project算⼦的输⼊。

‒ normed ：经过 layernorm 计算后的输出。

‒ weight_q、weight_k 、weight_v 、norm_weight ：Query、Key、Value矩阵的权重。

‒ bias_q、bias_k 、bias_v 、nrom_bias ：Query、Key、Value矩阵的偏置。

‒ eps ：layernorm中的 eps 参数。

(cid:15) 公式中各函数描述如下：

‒ fc ：全连接层（fully-connected）的缩写。

‒ layernorm ：layernorm归⼀化。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63154)

(cid:63158). 重点特性

参数说明

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) input：Tensor类型，必选。fused_norm_attention_project 的输⼊。维度 [batch_size, seq_lens, input_size]。其

中 batch_size 表⽰batch的⼤⼩，seq_lens 表⽰sequence的⻓度，input_size 表⽰输⼊的⼤⼩。

(cid:15) q_weight：Tensor类型，必选。fused_norm_attention_project 的Q矩阵权重参数。维度 [hidden_size, input_size]。

(cid:15) q_bias：Tensor类型，可选。fused_norm_attention_project` 的Q矩阵偏置参数。维度 ``[hidden_size]。

(cid:15) k_weight：Tensor类型，必选。fused_norm_attention_project 的K矩阵权重参数。维度 [hidden_size, input_size]。

(cid:15) k_bias：Tensor类型，可选。fused_norm_attention_project` 的K矩阵偏置参数。维度 ``[hidden_size]。

(cid:15) v_weight：Tensor类型，必选。fused_norm_attention_project 的V矩阵权重参数。维度 [hidden_size, input_size]。

(cid:15) v_bias：Tensor类型，可选。fused_norm_attention_project` 的V矩阵偏置参数。维度 ``[hidden_size]。

(cid:15) norm_weight：Tensor类型，可选。fused_norm_attention_project 的layernorm权重参数。维度 [input_size]。

(cid:15) norm_bias：Tensor类型，可选。fused_norm_attention_project` 的Vlayernorm偏置参数。维度 ``[input_size]。

(cid:15) eps ：ﬂoat类型。layernorm的 eps 参数。

(cid:15) out_layout ：String类型。允许的layout为 nthc 或者 nhtc。

(cid:15) head_size : String类型。output 的最后⼀维，当 out_layout 为 nhtc 。

(cid:15) norm_out : Bool类型。当等于 True 时，返回 layernorm 的输出。

返回值说明

(cid:15) output: Tensor类型，必选。fused_norm_attention_project的输出。根据⼊参的条件，返回不同的结果。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ q_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ q_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ k_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ k_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ v_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ v_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ norm_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ norm_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_fused_attn_proj.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63155)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.15 torch_mlu_ops.fused_norm_residual_ffn

torch_mlu_ops.fused_norm_residual_ffn(input,up_fc_weight,up_fc_bias,down_proj_weight,down_proj_bias,gate_up_proj_

,!weight,gate_up_proj_bias,layernorm_weight,layernorm_bias,eps,act_mode,residual_is,alhpa,beta)->Tensor

功能描述

计算transformer中的FFN（FeedForward）前馈神经⽹络模块的算⼦。前融合layernorm算⼦，适⽤于GPT或者 hidden_size

⼩于(cid:63155)(cid:63153)(cid:63157)(cid:63161)的transformer⽹络。

公式说明

(cid:15) 前融合layernorm算⼦的FFN标准结构公式如下：

normed = layernorm(input; normweight; normbias; eps)f c_output = f c(normed; up_f c_weight) + up_f c_bias

active_output = activation(f c_output)

basic_output = f c(active_output; down_proj_weight) + down_proj_bias

(cid:15) 前融合layernorm算⼦的Gated FFN标准结构公式如下：

normed = layernorm(input; normweight; normbias; eps)f c_output = f c(normed; up_f c_weight) + up_f c_bias

active_output = activation(f c_output)

gatedf c_output = f c(input; gate_up_proj_weight) + gate_up_proj_bias

elementwise_product_output = elementwise_product(active_output; gated_f c_output)

gated_output = f c(elementwise_product_output; down_proj_weight) + down_proj_bias

(cid:15) 公式中各参数描述如下：

‒ input ：FFN模块的输⼊。

‒ normed ：经过 layernorm 计算后的输出。

‒ fc_output ：FFN模块经过升维后的输出。

‒ active_output ：FFN模块经过激活后的输出。

‒ basic_output ：标准FFN模块经过降维后的输出。

‒ gated_fc_output ：gated FFN旁路经过升维后的输出。

‒ elementwise_product_output ：gated FFN主路和旁路经过elementwise product后的输出。

‒ gated_output ：gated FFN模块经过降维后的输出。

(cid:15) 公式中各函数描述如下：

‒ fc ：全连接层（fully-connected）的缩写。

‒ activation ：⽤⼾可配的激活函数，可选的激活函数："relu"、"gelu"、"silu"。

‒ elementwise_product ：点积。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63156)

(cid:63158). 重点特性

参数说明

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) input：Tensor类型，必选。fused_norm_residual_ffn 算⼦的输⼊。维度 [batch_size, seq_lens, hidden_size] 。

(cid:15) up_fc_weight：Tensor类 型， 必 选。fused_norm_residual_ffn 算 ⼦ 中 升 维 矩 阵 权 重 参 数， 维 度 [ffn_inner_size,

hidden_size] 。

(cid:15) up_fc_bias：Tensor类型，可选。fused_norm_residual_ffn 算⼦中升维矩阵偏置参数，维度 [ffn_inner_size] 。其中

ﬀn_inner_size表⽰FFN中间层的维度。

(cid:15) down_proj_weight：Tensor类型，必选。fused_norm_residual_ffn 算⼦中降维矩阵权重参数，维度 [hidden_size,

ffn_inner_size] 。

(cid:15) down_proj_bias：Tensor类型，可选。fused_norm_residual_ffn 算⼦中降维矩阵偏置参数，维度 [hidden_size] 。

(cid:15) gate_up_proj_weight：Tensor类型，可选。Gated FFN算⼦中升维矩阵权重参数，维度 [hidden_size, ffn_inner_size]

。

(cid:15) gate_up_proj_bias：Tensor类型，可选。Gated FFN算⼦中升维矩阵偏置参数，维度 [hidden_size] 。

(cid:15) norm_weight：Tensor类型，可选。fused_norm_residual_ffn 的layernorm权重参数。维度 [input_size]。

(cid:15) norm_bias：Tensor类型，可选。fused_norm_residual_ffn 的layernorm偏置参数。维度 [input_size]。

(cid:15) eps ：ﬂoat类型。layernorm的 eps 参数。

(cid:15) act_mode：string类型，必选。FFN的激化函数类型，可选的激活函数："relu"、"gelu"、"silu"、"none"。

(cid:15) residual_is : string类型，决定 residual 的来源，当 residual_is = input 时，表⽰来⾃ input 。

(cid:15) alpha ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。当 residual 存在时，作FNN模块的系数。

(cid:15) beta ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。当 residual 存在时，作为 residual 的系数。

返回值说明

(cid:15) output: Tensor类型，必选。ffn 的输出。维度 [batch_size, seq_lens, hidden_size] 。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ up_fc_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ up_fc_bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ down_proj_weight：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ down_proj_bias: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ gate_up_proj_weight: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ gate_up_proj_bias: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ norm_weight: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ norm_bias: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ output: FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_fused_ffn.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63157)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.16 torch_mlu_ops.fused_rms_norm

torch_mlu_ops.fused_rms_norm(input,residual,gamma,beta,bias,eps,store_output_before_norm,quant_scale,out)

功能描述

RMSnorm归⼀化算⼦。

公式说明

(cid:15) RMSnorm标准结构公式如下：

output = weight (cid:2)

√

input

RM S(input) + eps

+ bias

output = Quantize(output; quantscale)

(cid:15) 公式中各参数描述如下：

‒ input：RMSnorm的输⼊。

‒ RMS()：表⽰对input取均⽅（mean square）。

‒ Quantize() 表⽰对输出做量化。

‒ weight：表⽰RMSnorm的 weight 参数。

‒ bias：表⽰RMSnorm的 bias 参数。

‒ eps：表⽰RMSnorm eps的值，⼀个极⼩的正数，避免除零，⼀般默认值设定为(cid:63153).(cid:63153)(cid:63153)(cid:63153)(cid:63153)(cid:63154)。

‒ output：RMSnorm的输出。

参数说明

(cid:15) input：Tensor类型，必选。RMSnorm算⼦的输⼊。维度 [batch，seq_lens, hidden_size]，⽀持batch维度和seq_lens

维度的stride。

(cid:15) residual：Tensor类型，可选。RMSnorm算⼦的残差输⼊。在pad模式下，维度 [batch，seq_lens, hidden_size]，⽀

持batch维度和seq_lens维度的stride。

(cid:15) gamma：Tensor类型，可选。RMSnorm算⼦的 weight 。维度 [hidden_size]。

(cid:15) beta：Tensor类型，可选。RMSnorm算⼦的 bias 。维度 [hidden_size]。

(cid:15) bias：Tensor类型，可选。RMSnorm算⼦的残差的偏置输⼊。维度 [hidden_size]。

(cid:15) eps：Float类型，必选。RMSnorm算⼦的 eps 的值，⼀般设定为(cid:63153).(cid:63153)(cid:63153)(cid:63153)(cid:63153)(cid:63154)。

(cid:15) store_output_before_norm：Bool类型，必选。RMSnorm算⼦是否输出 input+residual 的值。当为true时，第⼆个返

回值输出 input+residual，当为false时，⽆第⼆个返回值。

(cid:15) quant_scale：Float类型，可选。量化参数。维度 [hidden_size]。当 dynamic_quant 为False时表⽰离线量化参数，为

True时表⽰在线量化时的平滑因⼦。当该参数不为空时，输⼊输出必须连续。

(cid:15) out：Tensor类型，可选。RMSnorm算⼦的原位输出。维度同input，⽀持batch维度和seq_lens维度的stride。

(cid:15) dynamic_quant：Bool类型，为False且 quant_scale 不为空时表⽰离线per-channel量化输出，否则为在线动态per-token

量化输出。

返回值说明

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63158)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) output：Tensor类 型，可 选。RMSnorm算⼦的 输出，当 输⼊参 数out不为空 时⽆此 输出。维 度 [batch, seq_lens,

hidden_size]，⽀持batch维度和seq_lens维度的stride。

(cid:15) residual_output：Tensor类型，可选。RMSnorm算⼦的residual输出。当store_output_before_norm为true时，返回值输

出 input+residual，当store_output_before_norm为false时，⽆此返回值。在pad模式下，维度 [batch_size*seq_lens,

hidden_size]；在pack模式下，维度 [total_seqlen, hidden_size]。

(cid:15) smooth_quant_scale：Tensor类型，可选。维度 [batch_size*seq_lens] 。当 dynamic_quant 为True时输出的量化scale。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ residual：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ beta：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ gamma：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ eps：Float(cid:63156)(cid:63155)

‒ store_output_before_norm：Bool

‒ quant_scale：Float

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、int(cid:63161)

‒ residual_output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ smooth_quant_scale：FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_fusenorm.py 。

5.1.17 torch_mlu_ops.fused_rope

torch_mlu_ops.fused_rope(input, key_cache_hp, value_cache_hp, sin_cache, cos_cache, position_id, gamma, beta, key_

,!cache_lp, value_cache_lp, cache_bs_id_hp, cache_seq_offsets_hp, cache_bs_id_lp, cache_seq_offsets_lp, k_scale_

,!hp, v_scale_hp, k_scale_lp, v_scale_lp, slot_mapping_hp, slot_mapping_lp, eps)-> None

功能描述

fold rope融合算⼦。

公式说明

(cid:15) 公式如下：

该算⼦为apply rotary，layernorm，以及reshape linear/paged cache功能的融合。

参数说明

(cid:15) qkv：Tensor类型，必选。输⼊维度 [batch, 1, head_num_q + head_num_k * 2, head_size] 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63159)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) key_cache_hp：Tensor 类 型， 必 选。 当 paged=False``时， 维 度 为 ``(max_bs, head_num_k, max_decode_len,

head_size) 否则 (num_blocks, head_num_k, block_size, head_size) 。

(cid:15) value_cache_hp：Tensor类 型， 必 选。 当 paged=False``时， 维 度 为 ``(max_bs, head_num_k, max_decode_len,

head_size) 否则 (num_blocks, head_num_k, block_size, head_size) 。

(cid:15) sin_table：Tensor类型，必选。输⼊维度 [rotary_seq, head_size] 。

(cid:15) cos_table：Tensor类型，必选。输⼊维度 [rotary_seq, head_size] 。

(cid:15) position_id：Tensor类型，必选。输⼊维度为 [batch] 。

(cid:15) gamma：Tensor类型，必选。输⼊维度是 [head_size] 。

(cid:15) beta：Tensor类型，必选。输⼊维度是 [head_size] 。

(cid:15) key_cache_lp：Tensor类型，可选。当 paged=False``时，维度为 ``(max_bs, head_num_k, max_decode_len, head_size

/ 2) 否则 (num_blocks, head_num_k, block_size, head_size / 2) 。

(cid:15) value_cache_lp：Tensor类型，可选。当 paged=False``时，维度为 ``(max_bs, head_num_k, max_decode_len / 2,

head_size) 否则 (num_blocks, head_num_k, block_size / 2, head_size) 。

(cid:15) cache_bs_id_hp：Tensor类型，可选，默认值为None。当前batch在cache中的位置，维度 [batch] 。

(cid:15) cache_seq_offsets_hp：Tensor类型，可选，默认值为None。当前kv在cache中的位置，维度 [batch] 。

(cid:15) cache_bs_id_lp：Tensor类型，可选，默认值为None。当前batch在cache中的位置，维度 [batch] 。

(cid:15) cache_seq_offsets_lp：Tensor类型，可选，默认值为None。当前kv在cache中的位置，维度 [batch] 。

(cid:15) key_scale_hp：Tensor类型，可选，默认值为None。key的量化参数，维度 [head_num_k, head_size] 。

(cid:15) value_scale_hp：Tensor类型，可选，默认值为None。value的量化参数，维度 [head_num_k, head_size] 。

(cid:15) key_scale_lp：Tensor类型，可选，默认值为None。key的量化参数，维度 [max_bs, head_num_k, group_num] 。

(cid:15) value_scale_lp：Tensor类型，可选，默认值为None。value的量化参数，维度 [max_bs, head_num_k, group_num] 。

(cid:15) slot_mapping_hp：Tensor类型，可选，默认值为None。paged_cahe时指⽰当前kv在cache中的位置，维度 [batch] 。

(cid:15) slot_mapping_lp：Tensor类型，可选，默认值为None。paged_cahe时指⽰当前kv在cache中的位置，维度 [batch] 。

(cid:15) eps：ﬂoat(cid:63156)(cid:63155)类型，可选。layernorm的eps参数，默认值为(cid:63154)e-(cid:63158) 。

返回值说明

(cid:15) qkv：原位操作。

(cid:15) key_cache_hp：原位操作。

(cid:15) value_cache_hp：原位操作。

(cid:15) key_cache_lp：原位操作。

(cid:15) value_cache_lp：原位操作。

(cid:15) key_scale_lp：原位操作。

(cid:15) value_scale_lp：原位操作。

规格限制

(cid:15) 数据类型⽀持：

‒ qkv：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache_hp：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ value_cache_hp：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ sin_table：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63160)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ cos_table：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ position_id：INT(cid:63156)(cid:63155)

‒ gamma：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ beta：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache_lp: INT(cid:63159)(cid:63157)

‒ value_cache_lp: INT(cid:63157)

‒ cache_bs_id_hp：INT(cid:63156)(cid:63155)

‒ cache_seq_oﬀsets_hp：INT(cid:63156)(cid:63155)

‒ cache_bs_id_lp：INT(cid:63156)(cid:63155)

‒ cache_seq_oﬀsets_lp：INT(cid:63156)(cid:63155)

‒ key_scale_hp：FP(cid:63156)(cid:63155)

‒ value_scale_hp：FP(cid:63156)(cid:63155)

‒ key_scale_lp：FP(cid:63156)(cid:63155)

‒ value_scale_lp：FP(cid:63156)(cid:63155)

‒ slot_mapping_hp：INT(cid:63156)(cid:63155)

‒ slot_mapping_lp: INT(cid:63156)(cid:63155)

‒ eps：FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_fused_rope.py 。

5.1.18 torch_mlu_ops.group_gemm

torch_mlu_ops.group_gemm(a, b, m_list, expand_idx, c, alpha, beta, max_m, bias) -> torch.Tensor

功能描述

多组GEMM集成算⼦。

公式说明

⽆

参数说明

(cid:15) a：Tensor类型，必选。GEMM算⼦的输⼊矩阵。维度 [m, k]。expand_idx `` 存在时，``total_m 等于 m_list 元素之

和，否则 total_m 等于 m。

(cid:15) b：Tensor类型，必选。GEMM算⼦的权重矩阵。维度 [experts_num, n, k]。

(cid:15) m_list：Tensor类型，必选。⽤于指定每个⼦矩阵的m维度。维度 [experts_num]。

(cid:15) expand_idx：Tensor类型，可选。⽤于扩展输⼊矩阵。维度 [total_m]。

(cid:15) c：Tensor类型，可选。残差。维度 [total_m, n]。

(cid:15) alpha：Tensor类型，可选。a乘b的系数。维度 [experts_num]。

(cid:15) beta：Tensor类型，可选。c的系数。维度 [experts_num]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63161)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) max_m: int类型，必选。m_list中的最⼤值。由于m_list是device数据，获取真实的最⼤值会影响性能，建议提供m_list

中的理论最⼤值。

(cid:15) bias: Tensor类型，可选。GEMM算⼦的bias矩阵。维度 [experts_num, n]。

返回值说明

(cid:15) output: Tensor类型。维度 [total_m, n]。

规格限制

(cid:15) 数据类型⽀持：

‒ a：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ b：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ m_list: INT(cid:63156)(cid:63155)

‒ expand_idx: INT(cid:63156)(cid:63155)

‒ c：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ alhpa：FP(cid:63156)(cid:63155)

‒ beta：FP(cid:63156)(cid:63155)

‒ output：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ bias: HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_group_gemm.py 。

5.1.19 torch_mlu_ops.smooth_quant_group_gemm

torch_mlu_ops.smooth_quant_group_gemm(a, b, m_list, expand_idx, c, alpha, beta, a_scale, b_scale, dtype, max_m,␣

,!bias, quant_flag) -> torch.Tensor

功能描述

多组GEMM集成量化功能算⼦。

公式说明

⽆

参数说明

(cid:15) a：Tensor类型，必选。GEMM算⼦的输⼊矩阵。维度 [m, k]。expand_idx `` 存在时，``total_m 等于 m_list 元素之

和，否则 total_m 等于 m。

(cid:15) b：Tensor类型，必选。GEMM算⼦的权重矩阵。维度 [experts_num, n, k]。

(cid:15) m_list：Tensor类型，必选。⽤于指定每个⼦矩阵的m维度。维度 [experts_num]。

(cid:15) expand_idx：Tensor类型，可选。⽤于扩展输⼊矩阵。维度 [total_m]。

(cid:15) c：Tensor类型，可选。残差。维度 [total_m, n]。

(cid:15) alpha：Tensor类型，可选。a乘b的系数。维度 [experts_num]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63156)(cid:63162)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) beta：Tensor类型，可选。c的系数。维度 [experts_num]。

(cid:15) a_scale：Tensor类型，可选。输⼊的分token量化的scale系数。维度 [total_m]。

(cid:15) b_scale：Tensor类 型， 可 选。 权 重 的 分channel量 化 的scale系 数。 维 度 [experts_num, n] ⾮group量 化 场 景， 或

[quant_group, experts_num, n] group量化场景。

(cid:15) dtype：dype类型，output 的数据类型，必须是 torch.half 、torch.bfloat16 。

(cid:15) max_m: int类型，必选。m_list中的最⼤值。由于m_list是device数据，获取真实的最⼤值会影响性能，建议提供m_list

中的理论最⼤值。

(cid:15) bias: Tensor类型，可选。GEMM算⼦的bias矩阵。维度 [experts_num, n]。

(cid:15) quant_flag：List类型，可选。w(cid:63157)w(cid:63161)混合量化模式时标记quant_group的bit位，数值必须是(cid:63157)或(cid:63161)，其它值会引发错误。

⻓度 [experts_num * quant_group]。

返回值说明

(cid:15) output: Tensor类型。维度 [total_m, n]。

规格限制

(cid:15) 数据类型⽀持：

‒ a：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ b：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、INT(cid:63161)(INT(cid:63157)x(cid:63155))

‒ m_list: INT(cid:63156)(cid:63155)

‒ expand_idx: INT(cid:63156)(cid:63155)

‒ c：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ alhpa：FP(cid:63156)(cid:63155)

‒ beta：FP(cid:63156)(cid:63155)

‒ a_scale: FP(cid:63156)(cid:63155)

‒ b_scale: FP(cid:63156)(cid:63155)

‒ output：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ bias: HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

备注

(cid:15) group量化场景，quant_group * quant_group_size == k，quant_group_size 表⽰该场景下每个group的元素数量，

quant_group_size 可以取值(cid:63154)(cid:63155)(cid:63161)、(cid:63155)(cid:63158)(cid:63159)、(cid:63158)(cid:63154)(cid:63155)、(cid:63154)(cid:63153)(cid:63155)(cid:63157)。

(cid:15) group量化场景，b_scale的形状必须是(quant_group, experts_num, n)。

(cid:15) gather 模式不⽀持int(cid:63157)量化。

(cid:15) (cid:63157)D 转数模式不⽀持int(cid:63157)量化。

(cid:15) 不⽀持通⽤量化模式的int(cid:63157)量化。

(cid:15) quant_flag 存在时为w(cid:63157)w(cid:63161)混合量化模式，其元素只能是(cid:63157)或(cid:63161)。(cid:63157)表⽰该quant_group对应的权重是int(cid:63157)量化，(cid:63161)表⽰该

quant_group对应的权重是int(cid:63161)量化。

(cid:15) w(cid:63157)w(cid:63161)混合量化模式要求b必须是(cid:63154)D。

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_group_gemm.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63153)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.20 torch_mlu_ops.matmul

torch_mlu_ops.matmul(input, ﬁlter, bias, residual, act_mode, alpha, beta, use_fast, approximate,

out_dtype, a_scale, b_scale, trans_a, trans_b)-> torch.Tensor

功能描述

通⽤的矩阵乘法算⼦。

公式说明

(cid:15) 带后融合的matmul标准结构公式如下：

mulout = alpha (cid:3) (matmul(input; f ilter) + bias) + beta (cid:3) residual

output = active(mulout)

(cid:15) 公式中各参数描述如下：

‒ input ：matmul的左矩阵。

‒ filter：matmul的右矩阵。

‒ bias：偏置。

‒ residual：参差。

‒ output ：计算最终的输出。

(cid:15) 公式中各函数描述如下：

‒ matmul ：矩阵乘。

‒ active：激活计算。

参数说明

(cid:15) input：Tensor类型，必选。矩阵乘的左矩阵。维度 [mat_m, mat_k]。

(cid:15) filter：Tensor类型，必选。矩阵乘的右矩阵。维度 [mat_n, mat_k]。

(cid:15) bias：Tensor类型，可选。矩阵乘的偏置。维度 [1, mat_n]。

(cid:15) residual：Tensor类型，可选。参差。维度 [mat_m, mat_n]。

(cid:15) act_mode：String类型，可选。后融合激活的类型，可选的激活函数：relu 、gelu 、silu。

(cid:15) alpha：Float(cid:63156)(cid:63155)类型，必选。矩阵乘法系数。

(cid:15) beta：Float(cid:63156)(cid:63155)类型，必选。参差的系数。

(cid:15) use_fast：Bool类型，必选。是否使⽤⾼性能的⽅式计算激活。

(cid:15) approximate：Bool类型，必选。gelu是否使⽤更精确的近似计算⽅式。

(cid:15) out_dtype：String类型，可选。如果输⼊为INT(cid:63161)类型，则必须指定输出类型。

(cid:15) a_scale：Float(cid:63156)(cid:63155)类型，默认值(cid:63154).(cid:63153)。当input为INT(cid:63161)类型时对应的量化系数。

(cid:15) b_scale：Float(cid:63156)(cid:63155)类型，默认值(cid:63154).(cid:63153)。当weight为INT(cid:63161)类型时对应的量化系数。

(cid:15) trans_a：Bool类型，默认值为False，标记input是否需要转置。

(cid:15) trans_b：Bool类型，默认值为True，标记weight是否需要转置。

返回值说明

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63154)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) out：Tensor类型，算⼦的输出，维度 [mat_m, mat_n]。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)

‒ ﬁlter：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)

‒ bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ residual：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ out：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ alpha：FP(cid:63156)(cid:63155)

‒ beta：FP(cid:63156)(cid:63155)

‒ use_fast：BOOL

‒ approximate：BOOL

‒ a_scale：FP(cid:63156)(cid:63155)

‒ b_scale：FP(cid:63156)(cid:63155)

‒ trans_a：BOOL

‒ trans_b：BOOL

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_matmul.py 。

5.1.21 torch_mlu_ops.matmul_allreduce

torch_mlu_ops.matmul_allreduce(cncl_comm, input, filter, bias, residual, alpha, beta, block_m)-> torch.Tensor

功能描述

矩阵乘法与allreduce的通信计算融合算⼦。

公式说明

⽆

参数说明

(cid:15) cncl_comm：Int类型，必选。cnclComm，⽤于归约通信。

(cid:15) input：Tensor类型，必选。矩阵乘的左矩阵。维度 [mat_m, mat_k]。

(cid:15) filter：Tensor类型，必选。矩阵乘的右矩阵。维度 [mat_n, mat_k]。

(cid:15) bias：Tensor类型，可选。矩阵乘的偏置。维度 [1, mat_n]。

(cid:15) residual：Tensor类型，可选。参差。维度 [mat_m, mat_n]。

(cid:15) alpha：Float(cid:63156)(cid:63155)类型，必选。矩阵乘法系数。

(cid:15) beta：Float(cid:63156)(cid:63155)类型，必选。参差的系数。

(cid:15) block_m：Int类型，可选。input在mat_m维度拆分的份数。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63155)

(cid:63158). 重点特性

返回值说明

(cid:15) out：Tensor类型，算⼦的输出，维度 [mat_m, mat_n]。

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

规格限制

(cid:15) 数据类型⽀持：

‒ cncl_comm: INT(cid:63159)(cid:63157)

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ ﬁlter：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ bias：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ residual：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ out：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ alpha：FP(cid:63156)(cid:63155)

‒ beta：FP(cid:63156)(cid:63155)

‒ block_m: INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/allreduce_case/test_matmul_all_reduce.py 。

5.1.22 torch_mlu_ops.moe_combine_result

torch_mlu_ops.moe_combine_result(input, reduce_weight, gather_ids, residual(optional),

cusum_token_count(optional), start_expert_id, expert_size,

bias(optional))->Tensor

功能描述

融合算⼦。从不同的专家模型中收集计算结果 (gather) ，然后对这些结果进⾏加权组合 (reduce) 。

公式说明

⽆。

参数说明

(cid:15) input：Tensor类型，必选。算⼦的输⼊，维度 [num_tokens, hidden_size] 。其中 num_tokens = num_token * topk ，

num_token 表⽰token数量，topk 表⽰每个token选取概率在前topk的专家，hidden_size 表⽰隐向量维数。

(cid:15) reduce_weight：Tensor类型，必选。token的权重信息，维度 [num_tokens] 。

(cid:15) gather_ids：Tensor类型，必选。⽤于收集token的索引，维度 [num_tokens] 。

(cid:15) residual：Tensor类型，可选，默认值为None。表⽰残差，维度 [num_token, hidden_size] 。

(cid:15) cusum_token_count：Tensor类型，可 选，默认 值为None。token_count的前缀 和，维度 [num_expert + 1] ，其 中

num_expert 表⽰专家数量。

(cid:15) start_expert_id：Int(cid:63156)(cid:63155)类型，必选。表⽰EP模型下起始专家的位置信息。

(cid:15) expert_size：Int(cid:63156)(cid:63155)类型，必选。表⽰EP模型下实际使⽤的专家数量。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63156)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) bias：Tensor类型，可选，默认值为。表⽰偏置，维度 [num_expert, hidden_size] 。

返回值说明

(cid:15) output: Tensor类型，必选。维度为 [num_token, hidden_size] 。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ reduce_weight：FP(cid:63156)(cid:63155)

‒ gather_ids：INT(cid:63156)(cid:63155)

‒ residual：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ cusum_token_count：INT(cid:63156)(cid:63155)

‒ start_expert_id：INT(cid:63156)(cid:63155)

‒ expert_size：INT(cid:63156)(cid:63155)

‒ bias：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_combine_result.py 。

5.1.23 torch_mlu_ops.moe_expand_input

torch_mlu_ops.moe_expand_input(input, gather_idx, cusum_token_count, output, start_expert_id, expert_size)->Tensor

功能描述

根据 gather_idx 在 input 执⾏索引操作。其中 cusum_token_count 、start_expert_id、expert_size 在EP模式下使⽤，

⽤于确定当前专家处理token的index范围。

公式说明

⽆。

参数说明

(cid:15) input：Tensor类 型， 必 选。 算 ⼦ 的 输 ⼊， 输 ⼊ 维 度 [token_num, hidden_size]。 其 中 token_num 表 ⽰token数 量，

hidden_size 表⽰各个token的特征信息。

(cid:15) gather_idx：Tensor类型，必选。输⼊维度 [token_num * topk]，topk 表⽰每个token选取概率在前topk的专家进⾏计

算。

(cid:15) cusum_token_count：Tensor类型，可选，默认值为None。输⼊维度为 [expert_num + 1] ，其中 expert_num 表⽰专家

的数量。

(cid:15) output：Tensor类型，可选，默认值为None。算⼦的原位输出，维度为 [token_num * topk, hidden_size] 。

(cid:15) start_expert_id：Int(cid:63156)(cid:63155)类型，可选，默认值为(cid:63153)。表⽰EP模型下起始专家的位置信息。

(cid:15) expert_size：Int(cid:63156)(cid:63155)类型，可选，默认值为(cid:63153)。表⽰EP模式下实际使⽤的专家数量。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63157)

(cid:63158). 重点特性

返回值说明

(cid:15) output，可选。维度为 [token_num * topk, hidden_size]。

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

规格限制

(cid:15) 数据类型⽀持：

‒ input：INT(cid:63161)，FP(cid:63154)(cid:63159)，FP(cid:63156)(cid:63155)，BF(cid:63154)(cid:63159)

‒ gather_idx：INT(cid:63156)(cid:63155)

‒ cusum_token_count：INT(cid:63156)(cid:63155)

‒ output：INT(cid:63161)，FP(cid:63154)(cid:63159)，FP(cid:63156)(cid:63155)，BF(cid:63154)(cid:63159)

‒ start_expert_id：INT(cid:63156)(cid:63155)

‒ expert_size：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_expand_input.py 。

5.1.24 torch_mlu_ops.moe_gen_idx

torch_mlu_ops.moe_gen_idx(expert_id, expert_num)->Tuple[torch.Tensor, torch.Tensor, torch.Tensor, torch.Tensor]

功能描述

基于 expert_id 和 expert_num ⽣成 expand_idx 、combine_idx 、token_count 、cusum_token_count 。

公式说明

⽆。

参数说明

(cid:15) expert_id：Tensor类型，必选。算⼦的输⼊，输⼊维度 [token_num, topk]。其中 token_num 表⽰token数量，topk 表

⽰每个token选取概率在前topk的专家进⾏计算。

(cid:15) expert_num：Int(cid:63156)(cid:63155)类型，必选。算⼦的输⼊，表⽰专家的数量。

返回值说明

(cid:15) outputs，Tuple[Tensor, Tensor, Tensor, Tensor]类 型， 可 选。 输 出 的(cid:63157)个Tensor分 别 是expand_idx、combine_idx、

token_count和cusum_token_count，维度同上。

规格限制

(cid:15) 数据类型⽀持：

‒ expert_id：INT(cid:63156)(cid:63155)

‒ expert_num：INT(cid:63156)(cid:63155)

‒ outputs：输出的(cid:63157)个Tensor类型均为INT(cid:63156)(cid:63155)

使⽤⽰例

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63158)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_gen_idx.py 。

5.1.25 torch_mlu_ops.moe_softmax_topk

torch_mlu_ops.moe_softmax_topk(input,topk,num_expert_group,topk_group,normalize,mask,normed_by)->Tuple[torch.

,!Tensor]

功能描述

MOE Route模块中的softmax topk融合算⼦。

公式说明

⽆。

参数说明

(cid:15) input：Tensor类型，必选。算⼦的输⼊，输⼊维度 [..., expert_num]。需要保证连续性。

(cid:15) topk：INT(cid:63156)(cid:63155)类型，每个token选取的专家数。

(cid:15) num_expert_group：INT(cid:63156)(cid:63155)类型，表⽰每个专家的分组个数。

(cid:15) topk_group：INT(cid:63156)(cid:63155)类型，每个专家组的topk个数。

(cid:15) normalize：Bool类型，是否给 reduce_weight 添加归⼀化操作。

(cid:15) mask：Tensor类型，可选。⽤于与softmax的计算结果相乘, 维度数量与input⼀致，后两维度⼤⼩与input⼀致。需要保

证连续性。

(cid:15) normed_by：Str类型，仅在normalize为True的时候使能，⽤于表⽰归⼀化的模式(topk_logit或softmax_logit)。

返回值说明

(cid:15) outputs : 可选，Tuple[Tensor, Tensor]类型。输出的两个Tensor分别是 reduce_weight 和 expert_id 。

规格限制

(cid:15) 数据类型⽀持：

‒ input ：Float，half，bﬂoat(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_softmax_topk.py 。

5.1.26 torch_mlu_ops.preload

torch_mlu_ops.preload(weight,size)

功能描述

预加载权重。

公式说明

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63159)

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:63158). 重点特性

⽆。

参数说明

(cid:15) weight：Tensor类型，必选。预加载的权重。

(cid:15) size：int类型，必选。预加载的数据⼤⼩（字节）。

返回值说明

⽆返回值。

规格限制

⽆。

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_preload.py 。

5.1.27 torch_mlu_ops.quant_to_linear_cache

torch_mlu_ops.quant_to_linear_cache(key,value,key_cache,value_cache,key_cache_scale,value_cache_scale,context_

,!lengths,max_context_len,packed,context_seq_offset,cache_bs_id,cache_seqlen_offset,quant_bit)

功能描述

将计算得到的K-V拼接到K-V cache上的INT(cid:63161)量化版本算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类型，必选。Transformer模块中计算得到的 Key。在context阶段，维度为 [batch_size, max_context_len,

head_num, head_size]。

(cid:15) value：Tensor 类 型， 可 选。Transformer 模 块 中 计 算 得 到 的 Value。 在 context 阶 段， 维 度 为 [batch_size,

max_context_len, head_num, head_size]。

(cid:15) key_cache：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache 的Tensor。 维 度 为 [max_batch, head_num,

cache_mem_len, head_size]。其中max_batch要求⼤于batch_size，max_context_len为⽹络⽀持的最⼤sequences

⻓度。

(cid:15) value_cache：Tensor类型，可选。Transformer模块中保存 Value cache 的Tensor。维度为 [max_batch, head_num,

cache_mem_len, head_size]。

(cid:15) key_cache_scale：Tensor类型，必选。Transformer模块中保存的 Key 的 scale 值。在context阶段，维度为 [batch_size,

head_num, cache_mem_len] 或者 [batch_size, head_num, cache_mem_len, group_num]。

(cid:15) value_cache_scale：Tensor类 型， 可 选。Transformer模 块 中 保 存 的 Value 的 scale 值。 在context阶 段， 维 度 为

[batch_size, head_num, cache_mem_len] 或者 [batch_size, head_num, cache_mem_len, group_num]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63160)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) context_lengths：Tensor类型，必选。为每批batch的sequences length⻓度。当 packed=false 时，维度为 [batch_size]，

每个batch⾥为对于的sequence length；当 packed=true 时，维度为 [batch_size+1]，⽤sequence与sequence之间的

⻓度差表⽰sequence⻓度。

(cid:15) max_context_len：Int类型，必选。在context阶段，表⽰最⼤的sequence length。

(cid:15) packed：Bool类型，必选。当 packed=true 时，表⽰开启pack模式，当 packed=false 时，表⽰关闭packed模式。

(cid:15) context_seq_offset：Tensor类型，可选。表⽰context的seq的偏置，维度 [batch_size]。

(cid:15) cache_bs_id：Tensor类型，可选。表⽰cache的batch的偏置，维度 [batch_size]。如果为空，则默认值为{(cid:63153), (cid:63154), (cid:63155) ...

batch - (cid:63154)}。

(cid:15) cache_seqlen_offset：Tensor类型，可选。表⽰cache的seq_lens的偏置，维度 [batch_size]。如果为空，则默认值

为{(cid:63153), (cid:63153), (cid:63153) ... (cid:63153)}。

(cid:15) quant_bit：Int类型，可选。表⽰量化结果的位宽，可选(cid:63161)（表⽰int(cid:63161)）或(cid:63157)（表⽰int(cid:63157)），默认值为(cid:63161)。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache：INT(cid:63161)

‒ value_cache：INT(cid:63161)

‒ key_cache_scale：FP(cid:63156)(cid:63155)

‒ value_cache_scale：FP(cid:63156)(cid:63155)

‒ context_lengths：INT(cid:63156)(cid:63155)

‒ max_context_len：INT(cid:63156)(cid:63155)

‒ packed：Bool

‒ context_seq_oﬀset：INT(cid:63156)(cid:63155)

‒ cache_bs_id：INT(cid:63156)(cid:63155)

‒ cache_seqlen_oﬀset：INT(cid:63156)(cid:63155)

‒ quant_bit : INT(cid:63156)(cid:63155)

‒ group_size: INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_quant_to_linear_cache.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63161)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.28 torch_mlu_ops.quant_to_paged_cache

torch_mlu_ops.quant_to_paged_cache(key,value,key_cache,value_cache,key_cache_scale,value_cache_scale,slot_mapping)

功能描述

torch_mlu_ops.reshape_paged_cache的INT(cid:63161)量化形式算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类 型， 必 选。Transformer模 块 中 计 算 得 到 的 Key。 在context阶 段， 维 度 为 [num_tokens, head_num,

head_size]。

(cid:15) value：Tensor类型，必选。Transformer模块中计算得到的 Value。在context阶段，维度为 [num_tokens, head_num,

head_size]。

(cid:15) key_cache：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache 的Tensor。 维 度 为 [num_blocks, head_num,

block_size, head_size]。

(cid:15) value_cache：Tensor类型，必选。Transformer模块中保存 Value cache 的Tensor。维度为 [num_blocks, head_num,

block_size, head_size]。

(cid:15) key_cache_scale：Tensor类型，必选。Transformer模块中保存 Key cache scale 的Tensor。维度为 [num_blocks,

head_num, block_size]。

(cid:15) value_cache_scale：Tensor类型，必选。Transformer模块中保存 Value cache scale 的Tensor。维度为 [num_blocks,

head_num, block_size]。

(cid:15) slot_mapping：Tensor类型，必选。表⽰Paged Attention中 K-V cache 存放位置。维度为 [num_tokens]。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159), FP(cid:63156)(cid:63155)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159), FP(cid:63156)(cid:63155)

‒ key_cache：INT(cid:63161)

‒ value_cache：INT(cid:63161)

‒ key_cache_scale：FP(cid:63156)(cid:63155)

‒ value_cache_scale：FP(cid:63156)(cid:63155)

‒ slot_mapping：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/kernel_pytest/quant_paged_cache_kernel_test/quant_paged_cache_kernel_test.py。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63157)(cid:63162)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.29 torch_mlu_ops.offline_quant_to_linear_cache

torch_mlu_ops.offline_quant_to_linear_cache(key, value, key_cache, value_cache, key_cache_quant_scale, value_

,!cache_quant_scale, context_lengths, max_context_len, quant_mode, packed, context_seq_offset, cache_bs_id, cache_

,!seqlen_offset)

功能描述

torch_mlu_ops.quant_to_linear_cache的离线量化形式算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类型，必选。Transformer模块中计算得到的 Key。如果使⽤pack模式，维度为 [num_tokens, head_num_kv,

head_size]，如果使⽤pad模式，维度为 [batch, seqlen, head_num_kv, head_size]。

(cid:15) value：Tensor类型，可选。Transformer模块中计算得到的 Value。维度与key相同。

(cid:15) key_cache：Tensor类型，必选。Transformer模块中保存 Key cache 的Tensor。维度为 [max_batch, head_num_kv,

cache_mem_len, head_size]。

(cid:15) value_cache：Tensor类型，可选。Transformer模块中保存 Value cache 的Tensor。维度为与key_cache相同。

(cid:15) key_cache_quant_scale：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache scale 的Tensor。 当 量 化 模

式 是 per_channel时， 维 度 为 [head_num_kv, head_size], 当 量 化 模 式 是per_head时， 维 度 是 [head_num_kv,

cache_mem_len]。

(cid:15) value_cache_quant_scale：Tensor类 型， 可 选。Transformer模 块 中 保 存 Value cache scale 的Tensor。 维 度 与

value_cache_quant_scale相同。

(cid:15) context_lengths：Tensor类型，必选。表⽰context的⻓度。如果使⽤pack模式，维度为 [batch + 1]，如果使⽤pad

模式，维度为 [batch]。

(cid:15) max_context_len：Int类型，必选。表⽰context的最⼤⻓度。

(cid:15) quant_mode：int类型，必选。表⽰量化模式。当该参数为True时，表⽰使⽤per_head量化，否则使⽤per_channel量化。

(cid:15) packed：bool类型，必选。该值为True时，表⽰输⼊为pack模式，否则为pad模式。

(cid:15) context_seq_offset：Tensor类型，可选。表⽰待量化的key或value的位置索引。维度为 [batch]。

(cid:15) cache_bs_id：Tensor类型，可选。表⽰key或value放⼊的cache的batch索引。维度为 [batch]。

(cid:15) cache_seqlen_offset：Tensor类型，可选。表⽰key或value放⼊cache的起始位置。维度为 [batch]。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159), FP(cid:63156)(cid:63155)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159), FP(cid:63156)(cid:63155)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63153)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ key_cache：INT(cid:63161)

‒ value_cache：INT(cid:63161)

‒ key_cache_quant_scale：FP(cid:63156)(cid:63155)

‒ value_cache_quant_scale：FP(cid:63156)(cid:63155)

‒ context_lengths：INT(cid:63156)(cid:63155)

‒ max_context_len：INT(cid:63156)(cid:63155)

‒ quant_mode：INT(cid:63156)(cid:63155)

‒ packed：BOOL

‒ context_seq_oﬀset：INT(cid:63156)(cid:63155)

‒ cache_bs_id：INT(cid:63156)(cid:63155)

‒ cache_seqlen_oﬀset：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/kernel_pytest/test_offline_quant_to_linear_cache.py。

5.1.30 torch_mlu_ops.offline_quant_to_paged_cache

torch_mlu_ops.offline_quant_to_paged_cache(key,value,key_cache_scale,value_cache_scale,slot_mapping,key_cache,

,!value_cache)

功能描述

torch_mlu_ops.quant_to_paged_cache的离线量化形式算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类 型， 必 选。Transformer模 块 中 计 算 得 到 的 Key。 在context阶 段， 维 度 为 [num_tokens, head_num,

head_size]。

(cid:15) value：Tensor类型，必选。Transformer模块中计算得到的 Value。在context阶段，维度为 [num_tokens, head_num,

head_size]。

(cid:15) key_cache_scale：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache scale 的Tensor。 维 度 为 [head_num,

head_size]。

(cid:15) value_cache_scale：Tensor类型，必选。Transformer模块中保存 Value cache scale 的Tensor。维度为 [head_num,

head_size]。

(cid:15) slot_mapping：Tensor类型，必选。表⽰Paged Attention中 K-V cache 存放位置。维度为 [num_tokens]。

(cid:15) key_cache：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache 的Tensor。 维 度 为 [num_blocks, head_num,

block_size, head_size]。

(cid:15) value_cache：Tensor类型，必选。Transformer模块中保存 Value cache 的Tensor。维度为 [num_blocks, head_num,

block_size, head_size]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63154)

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:63158). 重点特性

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159), FP(cid:63156)(cid:63155)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159), FP(cid:63156)(cid:63155)

‒ key_cache_scale：FP(cid:63156)(cid:63155)

‒ value_cache_scale：FP(cid:63156)(cid:63155)

‒ slot_mapping：INT(cid:63156)(cid:63155)

‒ key_cache：INT(cid:63161)

‒ value_cache：INT(cid:63161)

使⽤⽰例

请参考 torch_mlu_ops/tests/kernel_pytest/test_offline_quant_to_paged_cache_kernel.py。

5.1.31 torch_mlu_ops.reshape_linear_cache

torch_mlu_ops.reshape_linear_cache(key,value,key_cache,value_cache,context_lengths,max_context_len,packed,context_

,!seq_offset,cache_bs_id,cache_seqlen_offset)

功能描述

将计算得到的K-V拼接到K-V cache上的算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类型，必选。Transformer模块中计算得到的 Key。在context阶段，维度为 [batch_size, max_context_len,

head_num, head_size]。

(cid:15) value：Tensor 类 型， 可 选。Transformer 模 块 中 计 算 得 到 的 Value。 在 context 阶 段， 维 度 为 [batch_size,

max_context_len, head_num, head_size]。

(cid:15) key_cache：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache 的Tensor。 维 度 为 [max_batch, head_num,

cache_mem_len, head_size]。其中 max_batch 要求⼤于 batch_size，max_context_len 为⽹络⽀持的最⼤sequences

⻓度。

(cid:15) value_cache：Tensor类型，可选。Transformer模块中保存 Value cache 的Tensor。维度为 [max_batch, head_num,

cache_mem_len, head_size]。

(cid:15) context_lengths：Tensor类型，必选。为每批batch的sequences length⻓度。当 packed=false 时，维度为 [batch_size]，

每个batch⾥为对于的sequence length；当 packed=true 时，维度为 [batch_size+1]，⽤sequence与sequence之间的

⻓度差表⽰sequence⻓度。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63155)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) max_context_len：Int类型，必选。在context阶段，表⽰最⼤的sequence length。

(cid:15) packed：Bool类型，必选。当 packed=true 时，表⽰开启pack模式，当 packed=false 时，表⽰关闭pack模式。

(cid:15) context_seq_offset：Tensor类型，可选。表⽰context的seq的偏置，维度 [batch_size]。

(cid:15) cache_bs_id：Tensor类型，可选。表⽰cache的batch的偏置，维度 [batch_size]。如果为空，则默认值为{(cid:63153), (cid:63154), (cid:63155) ...

batch - (cid:63154)}。

(cid:15) cache_seqlen_offset：Tensor类型，可选。表⽰cache的seq_lens的偏置，维度 [batch_size]。如果为空，则默认值

为{(cid:63153), (cid:63153), (cid:63153) ... (cid:63153)}。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ context_lengths：INT(cid:63156)(cid:63155)

‒ max_context_len：INT(cid:63156)(cid:63155)

‒ packed：Bool

‒ context_seq_oﬀset：INT(cid:63156)(cid:63155)

‒ cache_bs_id：INT(cid:63156)(cid:63155)

‒ cache_seqlen_oﬀset：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_reshape_linear_cache.py 。

5.1.32 torch_mlu_ops.reshape_paged_cache

torch_mlu_ops.reshape_paged_cache(key,value,key_cache,value_cache,slot_mapping)

功能描述

torch_mlu_ops.reshape_linear_cache的Paged形式算⼦。

公式说明

⽆。

参数说明

(cid:15) key：Tensor类 型， 必 选。Transformer模 块 中 计 算 得 到 的 Key。 在context阶 段， 维 度 为 [num_tokens, head_num,

head_size]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63156)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) value：Tensor类型，必选。Transformer模块中计算得到的 Value。在context阶段，维度为 [num_tokens, head_num,

head_size]。

(cid:15) key_cache：Tensor类 型， 必 选。Transformer模 块 中 保 存 Key cache 的Tensor。 维 度 为 [num_blocks, head_num,

block_size, head_size]。

(cid:15) value_cache：Tensor类型，必选。Transformer模块中保存 Value cache 的Tensor。维度为 [num_blocks, head_num,

block_size, head_size]。

(cid:15) slot_mapping：Tensor类型，必选。表⽰Paged Attention中 K-V cache 存放位置。维度为 [num_tokens]。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ key：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ key_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ value_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ slot_mapping：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_reshape_paged_cache.py 。

5.1.33 torch_mlu_ops.quantize

torch_mlu_ops.quantize(x, scale, zero=None)-> torch.Tensor

功能描述

对输⼊tensor进⾏量化的算⼦。

公式说明

(cid:15) 公式如下：

scaled = x (cid:3) scale

output = scaled:round():clamp((cid:0)128; 127):to(torch:int8)

(cid:15) 公式中各参数描述如下：

‒ x ：quantize算⼦的输⼊。

‒ scale：量化过程的scale参数。

‒ output：quantize算⼦的输出。

参数说明

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63157)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) x ：Tensor类型，必选。quantize算⼦的输⼊，维度是 [..., C]，其中 C 是通道的维度。

(cid:15) scale ：Tensor类型，必选。quantize算⼦的 scale``输⼊，维度是 ``[C]。

(cid:15) zero ：Tensor类型，必选。quantize算⼦的保留参数，填⼊ None。

返回值说明

(cid:15) output：Tensor类型，必选。算⼦的输出，维度和 x ⼀致。

规格限制

(cid:15) 数据类型⽀持：

‒ x：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ scale：FP(cid:63156)(cid:63155)

‒ output：INT(cid:63161)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_smooth_quant.py 。

5.1.34 torch_mlu_ops.single_query_cached_kv_attn

torch_mlu_ops.single_query_cached_kv_attn(q_ori,k_cache,v_cache,out,block_tables,context_lens,k_cache_quant_scale,

,!v_cache_quant_scale,alibi_slopes,max_context_len,windows_size_left,windows_size_right,softmax_scale,return_lse,

,!kv_cache_quant_bit_size)->Tensor

功能描述

Decoder阶段的Single Query Cached KV Attention算⼦。

公式说明

(cid:15) Single Query Cached KV Attention标准结构公式如下：

scores = batch_matmul(q_ori; k_cache; transpose_b = T rue) (cid:3) sof tmax_scale

scores+ = ALiBi

scores+ = causal_mask

attention = sof tmax(scores)

output = batch_matmul(attention; v_cache)

lse = logsumexp(scores)

(cid:15) 公式中各参数描述如下：

‒ q_ori ：Single Query Cached KV Attention模块的输⼊。

‒ ALiBi：在线计算的ALiBi位置编码矩阵。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63158)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ scores ：Single Query Cached KV Attention模块第⼀个批量矩阵乘的输出。

‒ causal_mask：因果mask，当seq_q>(cid:63154)时默认使能。

‒ attention ：Single Query Cached KV Attention模块经过sotmax后的输出。

‒ output ：Single Query Cached KV Attention模块第⼆个批量矩阵乘的输出，也是模块的最终输出。

‒ lse ：Single Query Cached KV Attention的lse输出。

(cid:15) 公式中各函数描述如下：

‒ batch_matmul ：批量矩阵乘。

‒ softmax ：归⼀化指数函数。

‒ logsumexp ：对数求和指数函数。

参数说明

(cid:15) q_ori：Tensor类型，必选。算⼦的输⼊。维度 [batch, seq_q, q_head_num, head_size_qk]。

(cid:15) k_cache：Tensor类型，必选。算⼦的输⼊。维度 [num_blocks, kv_head_num, block_size, head_size_qk]。

(cid:15) v_cache：Tensor类型，必选。算⼦的输⼊。维度 [num_blocks, kv_head_num, block_size, head_size_v]。

(cid:15) out：Tensor类型，可选。维度同input。

(cid:15) block_tables：Tensor类型，必选。block的id与kv_cache之间的映射关系。维度 [bs, max_num_blocks_per_seq]，⾮

paged attention时，max_num_blocks_per_seq为(cid:63154)。

(cid:15) context_lens：Tensor类型，必选。记录每个batch的实际seq⻓度，维度 [batch]。

(cid:15) k_cache_quant_scale：Tensor类型，可选。k_cache&v_cache为量化类型时k_cache的量化参数，维度 [num_blocks,

kv_head_num, block_size]``表⽰per_token量化、``[num_blocks, kv_head_num, block_size, 1] 表⽰带group的

per_token量化，但当前group_num必须为(cid:63154)，以及 [kv_head_num, head_size_qk] 表⽰per_channel量化。

(cid:15) v_cache_quant_scale：Tensor 类 型， 可 选。k_cache&v_cache 为 量 化 类 型 时 v_cache 的 量 化 参 数， 维 度 同

k_cache_quant_scale。

(cid:15) alibi_slope：Tensor类型，可选。是⼀种相对位置编码⽅法，维度 [batch, q_head_num]，类型必须是ﬂoat(cid:63156)(cid:63155)。

(cid:15) max_context_len：Int(cid:63159)(cid:63157)类型，必选。算⼦的最⼤seq⻓度。

(cid:15) window_size_left：Int(cid:63159)(cid:63157)类型，左侧窗⼝⼤⼩，未指定时设置-(cid:63154)。实际参与计算的key和value⻓度为 window_size_left

+ 1。

(cid:15) window_size_right：Int(cid:63159)(cid:63157)类型，保留字段，设置-(cid:63154)即可。

(cid:15) softmax_scale：Float(cid:63156)(cid:63155)类型，必选。softmax之前的系数。

(cid:15) return_lse：Bool类型，标记是否返回lse tensor。为True时当前仅⽀持seq_q=(cid:63154)。

(cid:15) kv_cache_quant_bit_size：INT(cid:63156)(cid:63155)类型，KV cache量化的位宽，可选(cid:63161)（表⽰int(cid:63161)）、(cid:63157)（表⽰int(cid:63157)）和-(cid:63154)（表⽰和input

⼀样的数据类型），默认值为-(cid:63154)

返回值说明

(cid:15) output：Tensor类型，可选。算⼦的输出，当out不为空时有此输出，维度 [batch, seq_q, q_head_num, head_size]。

(cid:15) output_lse：Tensor类型，可选。算⼦的lse输出，当 return_lse 为True时有此输出，维度 [batch, q_head_num, seq_q]。

规格限制

(cid:15) 数据类型⽀持：

‒ q_ori：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63159)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ k_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)、int(cid:63157)

‒ v_cache：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)、int(cid:63157)

‒ block_tables：INT(cid:63156)(cid:63155)

‒ context_lens：INT(cid:63156)(cid:63155)

‒ k_cache_quant_scale：FP(cid:63156)(cid:63155)

‒ v_cache_quant_scale：FP(cid:63156)(cid:63155)

‒ alibi_slope：FP(cid:63156)(cid:63155)

‒ max_context_len：INT(cid:63159)(cid:63157)

‒ window_size_left：INT(cid:63156)(cid:63155)

‒ window_size_right：INT(cid:63156)(cid:63155)

‒ softmax_scale：FP(cid:63156)(cid:63155)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ output_lse：FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_single_query_cached_kv_attn.py 。

5.1.35 torch_mlu_ops.smooth_quant_matmul

torch_mlu_ops.smooth_quant_matmul(a,a_scale,b,b_scale,dtype,bias,c,act_mode,alpha,beta,use_hp_active)-> torch.

,!Tensor

功能描述

实现smooth_quant量化功能的GEMM算⼦。

公式说明

(cid:15) smooth_quant_matmul标准结构公式如下：

if ascaleisnotN one :

ab = matmul(a; b) (cid:3) ascale:outer(bscale) + bias

else :

ab = matmul(a; b) (cid:3) bscale + bias

acted = active(ab)

output = alpha (cid:3) acted + beta (cid:3) c

(cid:15) 公式中各参数描述如下：

‒ a ：矩阵乘运算的a矩阵。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63160)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ a_scale : 输⼊的分token量化的scale系数

‒ b ：矩阵乘运算的b矩阵。

‒ b_scale : 权重的分channel量化的scale系数

‒ bias ：偏置。

‒ alpha ：浮点标量，默认(cid:63154).(cid:63153)f。

‒ beta ：浮点标量，默认(cid:63154).(cid:63153)f。

‒ c ：第三个elemenwise加c矩阵。

‒ ouput :GEMM返回值。

(cid:15) 公式中各函数描述如下：- matmul :矩阵乘操作。- active :激活操作。

参数说明

(cid:15) a：Tensor类型，必选。GEMM算⼦的输⼊。维度 [M, K]。

(cid:15) a_scale：Tensor类型，可选。输⼊的分token量化的scale系数。维度 [N]。

(cid:15) b：Tensor类型，必选。GEMM算⼦的权重。维度 [N, K]。

(cid:15) b_scale：Tensor类型，可选。权重的分channel量化的scale系数。维度 [N]。

(cid:15) dtype：dype类型，output 的数据类型，必须是 torch.float 、torch.half 、torch.bfloat16 。

(cid:15) bias：Tensor类型，可选。GEMM算⼦的偏置。维度 [N]。

(cid:15) c ：Tensor类型，可选。维度 [M, N]。

(cid:15) act_mode : string类型，可选的激活函数："relu"、"gelu"、"silu"、"none"。

(cid:15) alpha ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。

(cid:15) beta ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。

(cid:15) use_hp_active ：bool类型，激活的计算⽅式，默认值为False。

返回值说明

(cid:15) ouput: Tensor类型, 维度 [M, N]。

规格限制

(cid:15) 数据类型⽀持：

‒ a：INT(cid:63161)

‒ a_scale：FP(cid:63156)(cid:63155)

‒ b: INT(cid:63161)

‒ b_scale：FP(cid:63156)(cid:63155)

‒ bias：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ c：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ output：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_quant_ffn.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63161)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.36 torch_mlu_ops.smooth_quant_matmul_allreduce

torch_mlu_ops.smooth_quant_matmul_allreduce(cncl_comm, a, a_scale, b, b_scale, dtype, bias, c, alpha, beta, block_

,!m)

功能描述

集成平滑量化功能的GEMM通信计算融合算⼦。

公式说明

⽆。

参数说明

(cid:15) cncl_comm：Int类型，必选。cnclComm，⽤于归约通信。

(cid:15) a：Tensor类型，必选。GEMM算⼦的输⼊。维度 [M, K]。

(cid:15) a_scale：Tensor类型，必选。输⼊的scale系数。维度 [M]。

(cid:15) b：Tensor类型，必选。GEMM算⼦的权重。维度 [N, K]。

(cid:15) b_scale：Tensor类型，必选。权重的scale系数。维度 [N]。

(cid:15) dtype：Torch.dtype类型，必选。返回值类型，可选的类型：torch.half、torch.bfloat16, torch.float32。

(cid:15) bias：Tensor类型，可选。GEMM算⼦的偏置。维度 [N]。

(cid:15) c：Tensor类型，可选。残差。维度 [M, N]。

(cid:15) alpha：ﬂoat类型，可选。默认(cid:63154).(cid:63153)f。

(cid:15) beta：ﬂoat类型，可选。默认(cid:63154).(cid:63153)f。

(cid:15) block_m：Int类型，可选。a在M维拆分的份数。

返回值说明

(cid:15) d: Tensor类型, 维度 [M, N]。

规格限制

(cid:15) 参数限制：

‒ M/N/K: M<=INT(cid:63156)(cid:63155)_MAX, NK<=INT(cid:63156)(cid:63155)_MAX, K>=(cid:63154)(cid:63159), N>=(cid:63154)(cid:63159)。

‒ a_scale/b_scale: 维度分别是 [M] 和 [N]。

(cid:15) 数据类型⽀持：

‒ cncl_comm：INT(cid:63159)(cid:63157)

‒ a：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ a_scale：FP(cid:63156)(cid:63155)

‒ b：INT(cid:63161)

‒ b_scale：FP(cid:63156)(cid:63155)

‒ bias：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ c：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ block_m: INT(cid:63156)(cid:63155)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63158)(cid:63162)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ d：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/allreduce_case/test_quant_matmul_all_reduce.py 。

5.1.37 torch_mlu_ops.swap_blocks

torch_mlu_ops.swap_blocks(dst, src, block_mapping)

功能描述

将 src 中源位置的BLOCK复制到 dst 的指定位置，拷⻉⽅向可以是：HostToDev、DevToHost、DevToDev。

公式说明

⽆。

参数说明

(cid:15) src：Vector<Tensor>类型，必选。输⼊维度 [num_layers<num_blocks, num_heads, block_size, head_size>]。其

中 num_layers 表 ⽰layer数 量，[num_heads, block_size, head_size] 整 体 ⼤ ⼩ 作 为 每 次 需 要copy的BLOCK⼤ ⼩。

num_blocks 表⽰BLOCK数量；block_size 表⽰输⼊序列中的特定块或⼦序列⼤⼩；num_heads 表⽰transformer模块中

的头数；head_size 表⽰每个头的维度。

(cid:15) dst：Vector<Tensor>类型，必选。输⼊维度 [num_layers<num_blocks, num_heads, block_size, head_size>]。其中

每个参数含义与 src ⼀致。

(cid:15) block_mapping：Map<int(cid:63159)(cid:63157), vector<int(cid:63159)(cid:63157)>>类型，必选。第⼀个int(cid:63159)(cid:63157)表⽰需要copy的源BLOCK位置（⼩于num_blocks），

第⼆个int(cid:63159)(cid:63157)表⽰将源BLOCK copy到的⽬标位置（⼩于num_blocks），vector 表⽰⼀个源BLOCK可以copy到多个⽬标

位置。

返回值说明

(cid:15) dst，原位操作。

规格限制

(cid:15) 数据类型⽀持：

‒ src：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、UINT(cid:63161)、INT(cid:63154)(cid:63159)、INT(cid:63156)(cid:63155)、INT(cid:63159)(cid:63157)

‒ dst：FP(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)、UINT(cid:63161)、INT(cid:63154)(cid:63159)、INT(cid:63156)(cid:63155)、INT(cid:63159)(cid:63157)

‒ block_mapping：INT(cid:63159)(cid:63157)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_swap_blocks.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63153)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

5.1.38 torch_mlu_ops.weight_only_quant_matmul

torch_mlu_ops.weight_only_quant_matmul(a,b,scale,zero,bias,c,act_mode,quant_bit_size,alpha,beta,use_hp_active)->␣

,!torch.Tensor

功能描述

实现仅权重量化功能的GEMM算⼦。

公式说明

(cid:15) weight_only_quant_matmul标准结构公式如下：

ab = matmul(a; b) (cid:3) scale + bias

acted = active(ab)

output = alpha (cid:3) acted + beta (cid:3) c

(cid:15) 公式中各参数描述如下：

‒ a ：矩阵乘运算的a矩阵。

‒ b ：矩阵乘运算的b矩阵。

‒ scale : 权重的分channel scale系数。

‒ bias ：偏置。

‒ alpha ：浮点标量，默认(cid:63154).(cid:63153)f。

‒ beta ：浮点标量，默认(cid:63154).(cid:63153)f。

‒ c ：第三个elemenwise加c矩阵。

‒ ouput :GEMM返回值。

(cid:15) 公式中各函数描述如下：- matmul :矩阵乘操作。- active :激活操作。

参数说明

(cid:15) a：Tensor类型，必选。GEMM算⼦的输⼊。维度 [M, K]。

(cid:15) b：Tensor类型，必选。GEMM算⼦的权重。当 quant_bit_size = 8 ，维度 [N, K]，当 quant_bit_size = 8 ，维度 [N,

K//2]。

(cid:15) scale：Tensor类型，可选。当group_wise量化时，维度为 [N, G]，G表⽰Group_num，Group_size为K//Group_num，

Group_size必须为(cid:63159)(cid:63157)、(cid:63154)(cid:63155)(cid:63161)、(cid:63155)(cid:63158)(cid:63159)或(cid:63158)(cid:63154)(cid:63155)，类型必须与a相同。当per_channel量化时，维度为 [N]，类型必须是ﬂoat，

(cid:15) zero：Tensor类型，⽬前不开放，设置为None。

(cid:15) bias：Tensor类型，可选。GEMM算⼦的偏置。维度 [N]。

(cid:15) c ：Tensor类型，可选。维度 [M, N]。

(cid:15) act_mode : string类型，可选的激活函数："relu"、"gelu"、"silu"、"none"。

(cid:15) quant_bit_size : int类型，权重 b 的位宽。

(cid:15) alpha ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。

(cid:15) beta ：ﬂoat类型。默认值为(cid:63154).(cid:63153)。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63154)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) use_hp_active ：bool类型，激活的计算⽅式，默认值为False。

返回值说明

(cid:15) ouput: Tensor类型, 维度 [M, N]。

规格限制

(cid:15) 数据类型⽀持：

‒ a：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)、INT(cid:63161)

‒ b: INT(cid:63161)

‒ scale：当使⽤group_wise量化时，类型与a相同，使⽤per_channel量化时，类型为FLOAT(cid:63156)(cid:63155)

‒ bias：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ c：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

‒ output：HALF、FP(cid:63156)(cid:63155)、BF(cid:63154)(cid:63159)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_quant_ffn.py 。

5.1.39 torch_mlu_ops.moe_active

torch_mlu_ops.moe_active(input, act_mode, is_gated, output, bias, cusumt_token_count, start_expert_id, expert_

,!size)-> torch.Tensor

功能描述

激活算⼦。

公式说明

(cid:15) 公式如下：

⽆。

参数说明

(cid:15) input：Tensor类型，必选。输⼊维度 [..., C]。其中 C 为通道的个数，在最低维。

(cid:15) act_mode：string类型。可以是 silu 或者 gelu 。

(cid:15) is_gated：Bool类型。当 is_gated=true 时，为gated激活。

(cid:15) output：Tensor类型，可选。当 is_gated=true 时，维度为 [..., C//2]，否则与input形状⼀致。

(cid:15) bias：Tensor类型，可选。输⼊维度是 [expert_num, C] 。其中 expert_num 为专家的总数。

(cid:15) cusum_token_count：Tensor类型，可选，默认值为None。token_count的前缀和，维度 [num_expert + 1] 。

(cid:15) start_expert_id：Int(cid:63156)(cid:63155)类型，必选。表⽰EP模型下起始专家的位置信息。

(cid:15) expert_size：Int(cid:63156)(cid:63155)类型，必选。表⽰EP模型下实际使⽤的专家数量。

返回值说明

(cid:15) output：激活后返回的tensor。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63155)

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:63158). 重点特性

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ output：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ bias：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ cusumt_token_count：INT(cid:63156)(cid:63155)

‒ start_expert_id：INT(cid:63156)(cid:63155)

‒ expert_size：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_add_bias_activation.py 。

5.1.40 torch_mlu_ops.moe_cast_gating

torch_mlu_ops.moe_cast_gating(input, weight) -> torch.Tensor

功能描述

将input的数据类型强转成ﬂoat类型，再和weight进⾏矩阵乘操作。

公式说明

(cid:15) 公式如下：

⽆。

参数说明

(cid:15) input：Tensor类型，必选。输⼊维度 [..., hidden_size]。其中 hidden_size 表⽰各个token的特征信息。该输⼊必须

连续。

(cid:15) weight：Tensor类型，必选。输⼊维度 [expert_num, hidden_size] 。其中 expert_num 表⽰专家数量。该输⼊必须连续。

返回值说明

(cid:15) output：Tensor类型。维度为 [..., expert_num]。

规格限制

(cid:15) 数据类型⽀持：

‒ input：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ weight：FP(cid:63156)(cid:63155)

‒ output：FP(cid:63156)(cid:63155)

(cid:15) 规模⽀持：

‒ hidden_size：⼩于或等于(cid:63154)(cid:63159)(cid:63156)(cid:63161)(cid:63157)

‒ expert_num：⼩于或等于(cid:63154)(cid:63155)(cid:63161)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63156)

(cid:63158). 重点特性

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_moe_cast_gating.py 。

5.1.41 torch_mlu_ops.update_out_and_lse

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

torch_mlu_ops.update_out_and_lse(out, lse, block_out, block_lse, seq_offset, cu_seqs, block_cu_seqs)-> None

功能描述

根据seq_oﬀset, cu_seqs, block_cu_seqs，将block_out和block_lse的结果更新到out和lse中。

公式说明

(cid:15) 公式如下：.. math:

\begin{array}{lcl}

out = out - sigmoid(block_lse - lse) * (out - block_out)\\

lse = lse - logsigmoid(lse - block_lse)\\

\end{array}

参数说明

(cid:15) out：Tensor 类 型， 必 选。 输 ⼊ 维 度 pad 模 式：[batch, max_seq_len, head_num, head_size] pack 模 式：

[total_seq_lens, head_num, head_size]。

(cid:15) lse：Tensor类型，必选。输⼊维度 pad模式：[batch, head_num, max_seq_len] 不⽀持pack模式。

(cid:15) block_out：Tensor类 型， 必 选。 输 ⼊ 维 度 pad模 式：[batch, block_seq_len, head_num, head_size] pack模 式：

[total_block_seq_lens, head_num, head_size]。

(cid:15) block_lse：Tensor类型，必选。输⼊维度 pad模式：[batch, head_num, block_seq_len] 不⽀持pack模式。

(cid:15) seq_offset：Tensor类型，可选。输⼊维度是 [batch] 。

(cid:15) cu_seqs：Tensor类型，可选。输⼊维度是 [batch + 1] ，仅限pack模式。

(cid:15) block_cu_seqs：Tensor类型，可选。输⼊维度是 [batch + 1] ，仅限pack模式。

返回值说明

⽆返回值。

规格限制

(cid:15) 数据类型⽀持：

‒ out：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ lse：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ block_out：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ block_lse：FP(cid:63156)(cid:63155)、FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)

‒ seq_oﬀset：INT(cid:63156)(cid:63155)

‒ cu_seqs：INT(cid:63156)(cid:63155)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63157)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ block_cu_seqs：INT(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_update_out_and_lse.py 。

5.1.42 torch_mlu_ops.single_query_mixed_cached_kv_attn

torch_mlu_ops.single_query_mixed_cached_kv_attn(q_ori,k_cache_lp,v_cache_lp,k_cache_hp,v_cache_hp,out,block_

,!tables_lp,block_tables_hp,context_lens_lp,context_lens_hp,k_cache_quant_scale_lp,v_cache_quant_scale_lp,k_cache_

,!quant_scale_hp,v_cache_quant_scale_hp,alibi_slopes,max_context_len_lp,max_context_len_hp,softmax_scale,return_

,!lse,kv_cache_quant_bit_size,kv_cache_quant_bit_size_lp,kv_cache_quant_bit_size_hp)->Tensor

功能描述

Decoder阶段的context_seq分段量化，采⽤低精度与⾼精度融合的Single Query Cached KV Attention算⼦。

公式说明

(cid:15) context_seq分段量化的融合的Single Query Cached KV Attention标准结构公式如下：

lse1; attn_out1 = single_query_cached_kv_attn(q_ori; k_cache_lp; v_cache_lp; block_tables_lp; :::)

lse2; attn_out2 = single_query_cached_kv_attn(q_ori; k_cache_hp; v_cache_hp; block_tables_hp; :::)

output = update_out_and_lse(lse1; attn_out1; lse2; attn_out2)

(cid:15) 公式中各参数描述如下：

‒ q_ori ：Single Query Cached KV Attention模块的输⼊。

‒ attn_out1、attn_out1 ：两段Single Query Cached KV Attention的输出。

‒ lse1、lse2 ：两段Single Query Cached KV Attention的lse输出。

‒ output：最终的拼接输出。

(cid:15) 公式中各函数描述如下：

‒ single_query_cached_kv_attn ：Single Query Cached KV Attention算⼦。

‒ update_out_and_lse：update_out_and_lse算⼦。

参数说明

(cid:15) q_ori：Tensor类型，必选。算⼦的输⼊。维度 [batch, seq_q, q_head_num, head_size]。

(cid:15) k_cache_lp：Tensor类型，必选。算⼦的输⼊, 第⼀段低精度context对应的cache。维度 [num_blocks_lp, kv_head_num,

block_size_lp, head_size]。

(cid:15) v_cache_lp：Tensor类型，必选。算⼦的输⼊, 第⼀段低精度context对应的cache。维度 [num_blocks_lp, kv_head_num,

block_size_lp, head_size]。

(cid:15) k_cache_hp：Tensor类型，必选。算⼦的输⼊, 第⼆段⾼精度context对应的cache。维度 [num_blocks_hp, kv_head_num,

block_size_hp, head_size]。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63158)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

(cid:15) v_cache_hp：Tensor类型，必选。算⼦的输⼊, 第⼆段⾼精度context对应的cache。维度 [num_blocks_hp, kv_head_num,

block_size_hp, head_size]。

(cid:15) out：Tensor类型，可选。维度同input。

(cid:15) block_tables_lp：Tensor 类 型， 必 选。 低 精 度 部 分 block 的 id 与 kv_cache 之 间 的 映 射 关 系。 维 度 [bs,

max_num_blocks_per_seq_lp]，⾮paged attention时，max_num_blocks_per_seq_lp为(cid:63154)。

(cid:15) block_tables_hp：Tensor 类 型， 必 选。 ⾼ 精 度 部 分 block 的 id 与 kv_cache 之 间 的 映 射 关 系。 维 度 [bs,

max_num_blocks_per_seq_hp]，⾮paged attention时，max_num_blocks_per_seq_hp为(cid:63154)。

(cid:15) context_lens_lp：Tensor类型，必选。记录低精度部分每个batch的实际seq⻓度，维度 [batch]。

(cid:15) context_lens_hp：Tensor类型，必选。记录⾼精度部分每个batch的实际seq⻓度，维度 [batch]。

(cid:15) k_cache_quant_scale_lp：Tensor 类 型， 可 选。k_cache_lp&v_cache_lp 为 量 化 类 型 时 k_cache_lp 的 量 化 参

数，维度 [num_blocks_lp, kv_head_num, block_size_lp]``表⽰per_token量化、``[num_blocks_lp, kv_head_num,

block_size_lp, 1] 表⽰带group的per_token量化，但当前group_num必须为(cid:63154)，以及 [kv_head_num, head_size] 表

⽰per_channel量化。

(cid:15) v_cache_quant_scale_lp：Tensor类型，可选。k_cache_lp&v_cache_lp为量化类型时v_cache_lp的量化参数，维度同

k_cache_quant_scale。

(cid:15) k_cache_quant_scale_hp：Tensor 类 型， 可 选。k_cache_hp&v_cache_hp 为 量 化 类 型 时 k_cache_hp 的 量 化 参

数，维度 [num_blocks_hp, kv_head_num, block_size_hp]``表⽰per_token量化、``[num_blocks_hp, kv_head_num,

block_size_hp, 1] 表⽰带group的per_token量化，但当前group_num必须为(cid:63154)，以及 [kv_head_num, head_size] 表

⽰per_channel量化。

(cid:15) v_cache_quant_scale_hp：Tensor类型，可选。k_cache_hp&v_cache_hp为量化类型时v_cache_hp的量化参数，维度

同 k_cache_quant_scale。

(cid:15) alibi_slope：Tensor类型，可选。是⼀种相对位置编码⽅法，维度 [batch, q_head_num]，类型必须是ﬂoat(cid:63156)(cid:63155)。

(cid:15) max_context_len_lp：Int(cid:63159)(cid:63157)类型，必选。算⼦低精度部分的最⼤seq⻓度。

(cid:15) max_context_len_hp：Int(cid:63159)(cid:63157)类型，必选。算⼦⾼精度部分的最⼤seq⻓度。

(cid:15) softmax_scale：Float(cid:63156)(cid:63155)类型，必选。softmax之前的系数。

(cid:15) return_lse：Bool类型，标记是否返回lse tensor。

(cid:15) kv_cache_quant_bit_size_lp：INT(cid:63156)(cid:63155)类型，低精度部分KV cache量化的位宽，可选(cid:63161)（表⽰int(cid:63161)）、(cid:63157)（表⽰int(cid:63157)）和-(cid:63154)

（表⽰和input⼀样的数据类型）。

(cid:15) kv_cache_quant_bit_size_hp：INT(cid:63156)(cid:63155)类型，⾼精度部分KV cache量化的位宽，可选(cid:63161)（表⽰int(cid:63161)）、(cid:63157)（表⽰int(cid:63157)）和-(cid:63154)

（表⽰和input⼀样的数据类型）。

返回值说明

(cid:15) output：Tensor类型，可选。算⼦的输出，当out不为空时有此输出，维度 [batch, seq_q, q_head_num, head_size]。

(cid:15) output_lse：Tensor类型，可选。算⼦的lse输出，当 return_lse 为True时有此输出，维度 [batch, q_head_num, seq_q]。

规格限制

(cid:15) 数据类型⽀持：

‒ q_ori：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ k_cache_lp：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)、int(cid:63157)

‒ v_cache_lp：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)、int(cid:63157)

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63159)

(cid:63158). 重点特性

(cid:63158).(cid:63154). TORCH_MLU_OPS算⼦

‒ k_cache_hp：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)、int(cid:63157)

‒ v_cache_hp：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)、INT(cid:63161)、int(cid:63157)

‒ block_tables_lp：INT(cid:63156)(cid:63155)

‒ block_tables_hp：INT(cid:63156)(cid:63155)

‒ context_lens_lp：INT(cid:63156)(cid:63155)

‒ context_lens_hp：INT(cid:63156)(cid:63155)

‒ k_cache_quant_scale_lp：FP(cid:63156)(cid:63155)

‒ v_cache_quant_scale_lp：FP(cid:63156)(cid:63155)

‒ k_cache_quant_scale_hp：FP(cid:63156)(cid:63155)

‒ v_cache_quant_scale_hp：FP(cid:63156)(cid:63155)

‒ alibi_slope：FP(cid:63156)(cid:63155)

‒ max_context_len_lp：INT(cid:63159)(cid:63157)

‒ max_context_len_hp：INT(cid:63159)(cid:63157)

‒ softmax_scale：FP(cid:63156)(cid:63155)

‒ output：FP(cid:63154)(cid:63159)、BF(cid:63154)(cid:63159)、FP(cid:63156)(cid:63155)

‒ output_lse：FP(cid:63156)(cid:63155)

使⽤⽰例

请参考 torch_mlu_ops/tests/ops_pytest/test_single_query_mixed_cached_kv_attn.py 。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63160)

(cid:63159) 最佳实践

Cambricon®

6.1 Torch-MLU-Ops性能benchmark测试

Torch-MLU-Ops benchmark测试脚本为⽤⼾提供了进⾏算⼦性能测试的便捷⼊⼝。

⽤⼾可通过以下命令获取各个参数的含义。

python3 benchmark_xxx.py --help

各个参数含义如下：

(cid:15) -h, --help：显⽰帮助信息。

(cid:15) --repeat_times, REPEAT_TIMES：配置测试重复次数。

(cid:15) --csv：将报告数据输出到csv⽂件。

(cid:15) -o O：当配置了 --csv 参数后，指定csv⽂件的输出⽬录。

测试命令⽰例如下:

python3 benchmark_active.py --repeat_times 10 --csv -o './active/'

⽀持如下算⼦：

表 (cid:63159).(cid:63154): Torch-MLU-Ops benchmark测试⽀持算⼦

op_name

active

apply_rotary

attention_project

combine_result

ﬀn

ﬂash_attn

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

下⻚继续

(cid:63159)(cid:63161)

(cid:63159). 最佳实践

(cid:63159).(cid:63155). TORCH-MLU-OPSGENCASE

表 6.1 – 续上⻚

op_name

fused_moe

fused_norm_attention_project

fused_norm_residual_ﬀn

fused_rms_norm

group_gemm

matmul

oﬄine_quant_to_linear_cache

per_token_smooth_quantize

preload

quantize

quant_to_linear_cache

reshape_linear_cache

reshape_paged_cache

single_query_cached_kv_attn

smooth_quant_matmul

weight_only_quant_matmul

6.2 Torch-MLU-Ops gen case

Torch-MLU-Ops 通过把算⼦参数信息保存成pt⽂件实现gen case功能。

gen case功能提供(cid:63157)个环境变量，如下所⽰：

(cid:15) TMO_GEN_CASE：值为(cid:63154)时开启dump case功能。

(cid:15) TMO_GEN_CASE_DUMP_DATA：值为(cid:63154)时，所有参数均保存真值；TMO_GEN_CASE_DUMP_DATA 不存在或为(cid:63153)时，则基础类型：

int(cid:63154)(cid:63159)、int(cid:63156)(cid:63155)、int(cid:63159)(cid:63157)类型的tensor，以及元素为整型的list保存真值。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63159)(cid:63162)

(cid:63159). 最佳实践

(cid:63159).(cid:63156). TORCH-MLU-OPS运⾏PT格式测例

(cid:15) TMO_GEN_CASE_OP_NAME：指定需要dump case的算⼦名称，torch_mlu_ops/_ops.py ⽂件的 __all__ 列表记录的算⼦⼤

部分已⽀持；如果不存在，则dump 程序运⾏过程中遇到的所有的TMO算⼦信息。

(cid:15) TMO_GEN_CASE_PATH：指定case⽂件的输出⽬录，默认保存在当前⽬录下。

使⽤⽰例：

export TMO_GEN_CASE=1

python test_group_gemm.py

bs: 1, seq_len: 5, k: 512, n: 512, experts_num: 8, topk: 2, dtype: torch.float16, idx_mode: False, has_bias:␣

,!False testing...

[torch_mlu_ops] dump case ====> tmo_gen_case/group_gemm/group_gemm_1734940034732854.pt

bs: 1, seq_len: 5, k: 512, n: 512, experts_num: 8, topk: 2, dtype: torch.float16, idx_mode: True, has_bias: True␣

,!testing...

[torch_mlu_ops] dump case ====> tmo_gen_case/group_gemm/group_gemm_1734940034736277.pt

export TMO_GEN_CASE=1

export TMO_GEN_CASE_DUMP_DATA＝1

export TMO_GEN_CASE_OP_NAME="group_gemm"

python test_moe.py

bs: 3, seq_len: 5, hidden_size: 512, inner_size: 768, experts_num: 8, start_expert_id: 4, expert_size: 2, topk: 2,

,! gated: True, renormalize: True, data_type: torch.float16, act_mode: sil

u testing...

[torch_mlu_ops] dump case ====>

tmo_gen_case/group_gemm/group_gemm_1735008239641861.pt

[torch_mlu_ops] dump case ====>

tmo_gen_case/group_gemm/group_gemm_1735008239672238.pt

6.3 Torch-MLU-Ops 运⾏pt格式测例

使⽤ tests/ops_pytest/run_gen_case.py ⽂件运⾏pt格式的case。

python3 run_gen_case.py --help

各个参数含义如下：

(cid:15) -h, --help：显⽰帮助信息。

(cid:15) --case_path CASE_PATH：指定pt⽂件的路径。

(cid:15) --detail：不运⾏case，只打印case信息。

测试命令⽰例如下:

python run_gen_case.py --case_path tmo_gen_case/group_gemm/group_gemm_1735008239672238.pt

[torch_mlu_ops] run tmo_gen_case/group_gemm/group_gemm_1735008239672238.pt ...

[torch_mlu_ops] run tmo_gen_case/group_gemm/group_gemm_1735008239672238.pt successfully

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63160)(cid:63153)

(cid:63159). 最佳实践

(cid:63159).(cid:63156). TORCH-MLU-OPS运⾏PT格式测例

python run_gen_case.py --detail --case_path tmo_gen_case/group_gemm/group_gemm_1735008239672238.pt

op: group_gemm

dump_data: True

a: tensor([[-7.5928e-02,

0.0000e+00, -2.9156e+01, ..., -0.0000e+00,

1.3725e+02, -3.6625e+02],

[-8.6609e-02, 4.9902e-01, 0.0000e+00, ..., -1.4185e-01,

-2.5859e+01, -5.4844e+01],

[ 4.5031e+01, 0.0000e+00, 8.7500e-01, ..., -9.3359e-01,

1.3960e+03, -0.0000e+00],

...,

[ 0.0000e+00, 0.0000e+00, 0.0000e+00, ..., 0.0000e+00,

0.0000e+00, 0.0000e+00],

[ 2.0000e+00, -6.6250e+00, 5.1200e+02, ..., -6.5664e+00,

5.1200e+02, 5.8984e+00],

[-5.1200e+02, -5.5352e+00, 2.0000e+00, ..., 0.0000e+00,

0.0000e+00, 0.0000e+00]], device='mlu:0', dtype=torch.float16)

b: tensor([[[ 8.9502e-01, 2.5488e-01, -9.5947e-01, ..., 1.0635e+00,

2.3848e+00, -1.8872e-01],

[-1.4636e-01, -1.2900e+00, -3.3154e-01, ..., 1.3516e+00,

5.0244e-01, 6.2012e-01],

[ 1.3877e+00, 1.3916e-01, -6.0303e-01, ..., -1.6094e+00,

-5.7861e-01, 1.3838e+00],

...,

[ 1.2350e-04, 4.5502e-02, 6.7041e-01, ..., 9.9316e-01,

-4.0845e-01, -7.1777e-01],

[-8.8818e-01, -1.3330e-01, -2.3779e-01, ..., -2.8662e-01,

1.5908e+00, -7.5098e-01],

[-1.0459e+00, -1.2305e+00, 8.1934e-01, ..., -1.6914e+00,

-1.5176e+00, -1.4102e+00]],

[[-1.8301e+00, 7.3975e-01, -6.4575e-02, ..., 4.7534e-01,

-2.6953e-01, -1.2490e+00],

[-1.9775e-01, -1.9775e+00, -1.2598e+00, ..., -1.8860e-01,

1.3220e-01, 9.9219e-01],

[-8.6768e-01, -9.7949e-01, 2.9712e-01, ..., 1.0332e+00,

7.2266e-02, -9.5068e-01],

...,

[ 3.0615e-01, 2.4023e+00, 1.1484e+00, ..., 1.0166e+00,

4.0161e-01, -1.0527e+00],

[ 3.8965e-01, -3.8867e-01, -1.0420e+00, ..., -1.3867e+00,

1.1553e+00, -8.5938e-01],

[-8.1152e-01, 2.6904e-01, -1.5840e+00, ..., 1.0518e+00,

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63160)(cid:63154)

(cid:63159). 最佳实践

(cid:63159).(cid:63156). TORCH-MLU-OPS运⾏PT格式测例

2.1855e+00, -1.6035e+00]]], device='mlu:0', dtype=torch.float16)

m_list: tensor([6, 4], device='mlu:0', dtype=torch.int32)

expand_idx: None

c: None

alpha: None

beta: None

max_m: 15

bias: None

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63160)(cid:63155)

Cambricon®

(cid:63160) FAQ

7.1 Cambricon Torch-MLU-Ops和BangTransformer的关系

Torch-MLU-Ops从BangTransformer演化⽽来，保留了BangTransformer提供的PyTorch第三⽅算⼦API能⼒，重新命名为

Torch-MLU-Ops。原先BangTransformer的⽹络评测功能由Cambricon vLLM继承。

7.2 Cambricon Torch-MLU-Ops和Cambricon PyTorch的关系

Cambricon PyTorch为能够运⾏在寒武纪设备上的PyTorch框架组件。Cambricon Torch-MLU-Ops为寒武纪PyTorch第三

⽅算⼦库。在运⾏时，Torch-MLU-Ops需要依赖Cambricon PyTorch环境。

7.3 Cambricon Torch-MLU-Ops和Cambricon MLU-Ops的关系

Cambricon Torch-MLU-Ops为寒武纪PyTorch第三⽅算⼦库，Cambricon MLU-Ops为寒武纪C++算⼦库，两者⽆直接关系。

7.4 Cambricon Torch-MLU-Ops中 flash_attention 算⼦和Cambricon FlashAtten-

tion以及Cambricon PyTorch源⽣SDPA算⼦的关系

Cambricon Torch-MLU-Ops中提供的 ﬂash_attention 算⼦为专⻔为推理场景下定制优化的算⼦，后端调⽤的是CNNL_Extra

组件中的 cnnlScaledDotProductAttn_v3 算⼦。

Cambricon FlashAttention和Cambricon PyTorch源⽣SDPA算⼦均针对训练场景，后端调⽤的是CNNL_Extra组件中的

cnnlFlashAttnFwd 和 cnnlFlashAttnBwd 算⼦。

针对推理场景下的ﬂash attention算⼦测试，应当使⽤Cambricon Torch-MLU-Ops中的 ﬂash_attention 算⼦，以获得MLU

计算卡上更好的性能表现。

Copyright © (cid:63155)(cid:63153)(cid:63155)(cid:63158) Cambricon Corporation.

(cid:63160)(cid:63156)

