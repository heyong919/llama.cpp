#ifndef GGML_DEBUG_PRINT_H
#define GGML_DEBUG_PRINT_H

#include "ggml.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 打印 ggml_tensor 结构体的内部信息到终端，用于调试
 * @param tensor 要打印的张量指针
 * @param prefix 可选的前缀字符串，用于标识输出（可以为NULL）
 */
void ggml_tensor_print_info(const struct ggml_tensor * tensor, const char * prefix);

#ifdef __cplusplus
}
#endif

#endif // GGML_DEBUG_PRINT_H