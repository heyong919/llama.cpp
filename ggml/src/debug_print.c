#include "ggml.h"
#include <stdio.h>
#include <string.h>
#include "debug_print.h"

/**
 * 打印 ggml_tensor 结构体的内部信息到终端，用于调试
 * @param tensor 要打印的张量指针
 * @param prefix 可选的前缀字符串，用于标识输出（可以为NULL）
 */
void ggml_tensor_print_info(const struct ggml_tensor * tensor, const char * prefix) {
    if (tensor == NULL) {
        printf("Error: NULL tensor pointer\n");
        return;
    }

    // 处理可选前缀
    const char * pre = prefix ? prefix : "";
    const char * name = tensor->name[0] ? tensor->name : "<unnamed>";
    
    // 第一行：名称、操作类型、数据类型
    printf("[%s%s] op=%s type=%s\n", pre, name, ggml_op_name(tensor->op), ggml_type_name(tensor->type));
    
    // 第二行：形状、步长
    printf("[%s%s] shape=[%lld,%lld,%lld,%lld] strides=[%zu,%zu,%zu,%zu]\n", pre, name,
           (long long)tensor->ne[0], (long long)tensor->ne[1], 
           (long long)tensor->ne[2], (long long)tensor->ne[3],
           tensor->nb[0]/ggml_type_size(tensor->type),
           tensor->nb[1]/ggml_type_size(tensor->type),
           tensor->nb[2]/ggml_type_size(tensor->type),
           tensor->nb[3]/ggml_type_size(tensor->type));
    
    // 第三行：源张量及对应的op类型
    printf("[%s%s] src=", pre, name);
    if (tensor->src[0] == NULL) {
        printf("none");
    } else {
        for (int i = 0; i < GGML_MAX_SRC; ++i) {
            if (tensor->src[i] != NULL) {
                if (i > 0) {
                    printf(", ");
                }
                printf("%s(%s)[%lld,%lld,%lld,%lld]", 
                       tensor->src[i]->name[0] ? tensor->src[i]->name : "<unnamed>", 
                       ggml_op_name(tensor->src[i]->op),
                        (long long)tensor->src[i]->ne[0], (long long)tensor->src[i]->ne[1], 
                        (long long)tensor->src[i]->ne[2], (long long)tensor->src[i]->ne[3]);
            }
        }
    }
    
    // 如果有视图源，添加到第三行
    if (tensor->view_src != NULL) {
        printf(" view_src=%s(offs=%zu)", 
               tensor->view_src->name[0] ? tensor->view_src->name : "<unnamed>", 
               tensor->view_offs);
    }
    
    printf("\n");

}