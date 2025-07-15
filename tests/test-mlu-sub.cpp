#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include "ggml.h"
#include "ggml-alloc.h"
#include "ggml-backend.h"
#include "ggml-cpu.h"
#ifdef GGML_USE_MLU
#include "ggml-mlu.h"
#endif
#include "debug_print.h"

// Function declarations
void init_random_data(float* data, size_t size, float min_val = -1.0f, float max_val = 1.0f);
void init_random_data_fp16(ggml_fp16_t* data, size_t size, float min_val = -1.0f, float max_val = 1.0f);
float calculate_mse(const float* a, const float* b, size_t size);
float calculate_mse_fp16(const ggml_fp16_t* a, const ggml_fp16_t* b, size_t size);
float calculate_max_abs_error(const float* a, const float* b, size_t size);
float calculate_max_abs_error_fp16(const ggml_fp16_t* a, const ggml_fp16_t* b, size_t size);

void init_random_data(float* data, size_t size, float min_val, float max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min_val, max_val);

    for (size_t i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
}

void init_random_data_fp16(ggml_fp16_t* data, size_t size, float min_val, float max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min_val, max_val);

    for (size_t i = 0; i < size; ++i) {
        data[i] = ggml_fp32_to_fp16(dis(gen));
    }
}

// 辅助函数：计算均方误差
float calculate_mse(const float* a, const float* b, size_t size) {
    float mse = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        float diff = a[i] - b[i];
        mse += diff * diff;
    }
    return mse / size;
}

float calculate_mse_fp16(const ggml_fp16_t* a, const ggml_fp16_t* b, size_t size) {
    float mse = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        float diff = ggml_fp16_to_fp32(a[i]) - ggml_fp16_to_fp32(b[i]);
        mse += diff * diff;
    }
    return mse / size;
}

// 辅助函数：计算最大绝对误差
float calculate_max_abs_error(const float* a, const float* b, size_t size) {
    float max_error = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        float error = std::abs(a[i] - b[i]);
        if (error > max_error) {
            max_error = error;
        }
    }
    return max_error;
}

float calculate_max_abs_error_fp16(const ggml_fp16_t* a, const ggml_fp16_t* b, size_t size) {
    float max_error = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        float error = std::abs(ggml_fp16_to_fp32(a[i]) - ggml_fp16_to_fp32(b[i]));
        if (error > max_error) {
            max_error = error;
        }
    }
    return max_error;
}

// MLU Sub 算子测试类
class MLUSubTestCase {
public:
    MLUSubTestCase(const std::vector<int64_t>& shape_a, const std::vector<int64_t>& shape_b, ggml_type data_type = GGML_TYPE_F32)
        : shape_a_(shape_a), shape_b_(shape_b), data_type_(data_type) {

        // 计算张量大小
        size_a_ = 1;
        for (auto dim : shape_a_) {
            size_a_ *= dim;
        }

        size_b_ = 1;
        for (auto dim : shape_b_) {
            size_b_ *= dim;
        }

        // 计算输出张量大小（广播后的形状）
        size_out_ = std::max(size_a_, size_b_);

        // 根据数据类型分配内存
        if (data_type_ == GGML_TYPE_F16) {
            data_a_fp16_.resize(size_a_);
            data_b_fp16_.resize(size_b_);
            result_cpu_fp16_.resize(size_out_);
            result_mlu_fp16_.resize(size_out_);
            // 初始化随机数据
            init_random_data_fp16(data_a_fp16_.data(), size_a_);
            init_random_data_fp16(data_b_fp16_.data(), size_b_);
        } else {
            data_a_.resize(size_a_);
            data_b_.resize(size_b_);
            result_cpu_.resize(size_out_);
            result_mlu_.resize(size_out_);
            // 初始化随机数据
            init_random_data(data_a_.data(), size_a_);
            init_random_data(data_b_.data(), size_b_);
        }
    }
    
    bool run_test() {
        std::cout << "Running MLU Sub test (" << ggml_type_name(data_type_) << ")..." << std::endl;
        
        // 运行 CPU 测试
        if (!run_cpu_test()) {
            std::cerr << "CPU test failed!" << std::endl;
            return false;
        }
        
        // 运行 MLU 测试
        if (!run_mlu_test()) {
            std::cerr << "MLU test failed!" << std::endl;
            return false;
        }
        
        // 比较结果
        return compare_results();
    }
    
private:
    bool run_cpu_test() {
        // 初始化 CPU 后端
        ggml_backend_t cpu_backend = ggml_backend_cpu_init();
        if (!cpu_backend) {
            std::cerr << "Failed to initialize CPU backend" << std::endl;
            return false;
        }
        
        // 创建计算图
        struct ggml_init_params params;
        params.mem_size = 1 * 1024 * 1024 * 1024;  // 1GB
        params.mem_buffer = nullptr;
        params.no_alloc = false;
        
        struct ggml_context* ctx = ggml_init(params);
        if (!ctx) {
            std::cerr << "Failed to initialize ggml context" << std::endl;
            ggml_backend_free(cpu_backend);
            return false;
        }
        
        // 创建张量
        struct ggml_tensor* tensor_a = ggml_new_tensor(ctx, data_type_, shape_a_.size(), shape_a_.data());
        struct ggml_tensor* tensor_b = ggml_new_tensor(ctx, data_type_, shape_b_.size(), shape_b_.data());

        // 设置数据
        if (data_type_ == GGML_TYPE_F16) {
            memcpy(tensor_a->data, data_a_fp16_.data(), size_a_ * sizeof(ggml_fp16_t));
            memcpy(tensor_b->data, data_b_fp16_.data(), size_b_ * sizeof(ggml_fp16_t));
        } else {
            memcpy(tensor_a->data, data_a_.data(), size_a_ * sizeof(float));
            memcpy(tensor_b->data, data_b_.data(), size_b_ * sizeof(float));
        }
        
        // 执行 Add 操作
        struct ggml_tensor* result = ggml_sub(ctx, tensor_a, tensor_b);
        
        // 构建计算图
        struct ggml_cgraph* gf = ggml_new_graph(ctx);
        ggml_build_forward_expand(gf, result);
        ggml_tensor_print_info(result, "CPU-");
        
        // 执行计算
        if (ggml_backend_graph_compute(cpu_backend, gf) != GGML_STATUS_SUCCESS) {
            std::cerr << "CPU computation failed" << std::endl;
            ggml_free(ctx);
            ggml_backend_free(cpu_backend);
            return false;
        }
        
        // 复制结果
        if (data_type_ == GGML_TYPE_F16) {
            memcpy(result_cpu_fp16_.data(), result->data, size_out_ * sizeof(ggml_fp16_t));
        } else {
            memcpy(result_cpu_.data(), result->data, size_out_ * sizeof(float));
        }
        
        // 清理资源
        ggml_free(ctx);
        ggml_backend_free(cpu_backend);
        
        return true;
    }
    
    bool run_mlu_test() {
        // 初始化 MLU 后端
        ggml_backend_t mlu_backend = ggml_backend_mlu_init((int32_t)0);  // 使用设备 0
        if (!mlu_backend) {
            std::cerr << "Failed to initialize MLU backend" << std::endl;
            return false;
        }
        
        // 创建计算图
        struct ggml_init_params params;
        params.mem_size = 1 * 1024 * 1024 * 1024; // 1GB
        params.mem_buffer = nullptr;
        params.no_alloc = true;  // MLU 后端需要手动分配内存
        
        struct ggml_context* ctx = ggml_init(params);
        if (!ctx) {
            std::cerr << "Failed to initialize ggml context" << std::endl;
            ggml_backend_free(mlu_backend);
            return false;
        }
        
        // 创建张量
        struct ggml_tensor* tensor_a = ggml_new_tensor(ctx, data_type_, shape_a_.size(), shape_a_.data());
        struct ggml_tensor* tensor_b = ggml_new_tensor(ctx, data_type_, shape_b_.size(), shape_b_.data());

        // 执行 Add 操作
        struct ggml_tensor* result = ggml_sub(ctx, tensor_a, tensor_b);

        // 构建计算图
        struct ggml_cgraph* gf = ggml_new_graph(ctx);
        ggml_build_forward_expand(gf, result);
        ggml_tensor_print_info(result, "MLU-");

        // 分配后端内存
        ggml_backend_buffer_t buffer = ggml_backend_alloc_ctx_tensors(ctx, mlu_backend);
        if (!buffer) {
            std::cerr << "Failed to allocate MLU buffer" << std::endl;
            ggml_free(ctx);
            ggml_backend_free(mlu_backend);
            return false;
        }

        // 设置输入数据
        if (data_type_ == GGML_TYPE_F16) {
            ggml_backend_tensor_set(tensor_a, data_a_fp16_.data(), 0, size_a_ * sizeof(ggml_fp16_t));
            ggml_backend_tensor_set(tensor_b, data_b_fp16_.data(), 0, size_b_ * sizeof(ggml_fp16_t));
        } else {
            ggml_backend_tensor_set(tensor_a, data_a_.data(), 0, size_a_ * sizeof(float));
            ggml_backend_tensor_set(tensor_b, data_b_.data(), 0, size_b_ * sizeof(float));
        }
        
        // 执行计算
        if (ggml_backend_graph_compute(mlu_backend, gf) != GGML_STATUS_SUCCESS) {
            std::cerr << "MLU computation failed" << std::endl;
            ggml_backend_buffer_free(buffer);
            ggml_free(ctx);
            ggml_backend_free(mlu_backend);
            return false;
        }
        
        // 获取结果
        if (data_type_ == GGML_TYPE_F16) {
            ggml_backend_tensor_get(result, result_mlu_fp16_.data(), 0, size_out_ * sizeof(ggml_fp16_t));
        } else {
            ggml_backend_tensor_get(result, result_mlu_.data(), 0, size_out_ * sizeof(float));
        }
        
        // 清理资源
        ggml_backend_buffer_free(buffer);
        ggml_free(ctx);
        ggml_backend_free(mlu_backend);
        
        return true;
    }
    
    bool compare_results() {
        // 根据数据类型设置不同的容忍度
        const float tolerance = (data_type_ == GGML_TYPE_F16) ? 1e-2f : 1e-5f;
        const float mse_threshold = (data_type_ == GGML_TYPE_F16) ? 1e-2f : 1e-4f;

        float mse, max_error;
        if (data_type_ == GGML_TYPE_F16) {
            mse = calculate_mse_fp16(result_cpu_fp16_.data(), result_mlu_fp16_.data(), size_out_);
            max_error = calculate_max_abs_error_fp16(result_cpu_fp16_.data(), result_mlu_fp16_.data(), size_out_);
        } else {
            mse = calculate_mse(result_cpu_.data(), result_mlu_.data(), size_out_);
            max_error = calculate_max_abs_error(result_cpu_.data(), result_mlu_.data(), size_out_);
        }

        std::cout << "Results comparison:" << std::endl;
        std::cout << "  MSE: " << mse << std::endl;
        std::cout << "  Max absolute error: " << max_error << std::endl;
        
        if (max_error > tolerance || mse > mse_threshold) {
            std::cerr << "Test FAILED: Max error " << max_error << " exceeds tolerance " << tolerance
                      << " or MSE " << mse << " exceeds threshold " << mse_threshold << std::endl;

            // 打印前几个不匹配的值用于调试
            std::cout << "First few mismatched values:" << std::endl;
            int count = 0;
            for (size_t i = 0; i < size_out_ && count < 10; ++i) {
                float cpu_val, mlu_val, error;
                if (data_type_ == GGML_TYPE_F16) {
                    cpu_val = ggml_fp16_to_fp32(result_cpu_fp16_[i]);
                    mlu_val = ggml_fp16_to_fp32(result_mlu_fp16_[i]);
                } else {
                    cpu_val = result_cpu_[i];
                    mlu_val = result_mlu_[i];
                }
                error = std::abs(cpu_val - mlu_val);
                if (error > tolerance) {
                    std::cout << "  [" << i << "] CPU: " << cpu_val
                              << ", MLU: " << mlu_val
                              << ", Error: " << error << std::endl;
                    count++;
                }
            }
            return false;
        }
        
        std::cout << "Test PASSED: Results match within tolerance" << std::endl;
        return true;
    }
    
    std::vector<int64_t> shape_a_, shape_b_;
    size_t size_a_, size_b_, size_out_;
    ggml_type data_type_;

    // F32 data
    std::vector<float> data_a_, data_b_;
    std::vector<float> result_cpu_, result_mlu_;

    // F16 data
    std::vector<ggml_fp16_t> data_a_fp16_, data_b_fp16_;
    std::vector<ggml_fp16_t> result_cpu_fp16_, result_mlu_fp16_;
};

int main(int argc, char** argv) {
    (void)argc; // 避免未使用参数警告
    (void)argv; // 避免未使用参数警告
    std::cout << "=== MLU Sub Operator Test ===" << std::endl;
    
#ifndef GGML_USE_MLU
    std::cerr << "MLU backend is not compiled" << std::endl;
    return 77; // Skip test
#else
    // 检查 MLU 后端是否可用
    if (ggml_backend_mlu_get_device_count() == 0) {
        std::cerr << "MLU backend is not available" << std::endl;
        return 77; // Skip test
    }
    
    std::cout << "MLU backend is available" << std::endl;
    
    bool all_tests_passed = true;
    
    // 测试用例 1: 相同形状的张量相加
    std::cout << "\n--- Test 1: Same shape tensors ---" << std::endl;
    {
        MLUSubTestCase test({4, 4}, {4, 4});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }
    
    // 测试用例 2: 广播相加 (标量 + 张量)
    std::cout << "\n--- Test 2: Scalar + Tensor broadcast ---" << std::endl;
    {
        MLUSubTestCase test({4, 4}, {1});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }
    
    // 测试用例 3: 向量广播
    std::cout << "\n--- Test 3: Vector broadcast ---" << std::endl;
    {
        MLUSubTestCase test({4, 4}, {4, 1});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }
    
    // 测试用例 4: 更大的张量
    std::cout << "\n--- Test 4: Larger tensors ---" << std::endl;
    {
        MLUSubTestCase test({1024, 1024, 8}, {1, 1024});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // FP16 测试用例
    std::cout << "\n=== FP16 Tests ===" << std::endl;

    // 测试用例 5: FP16 相同形状的张量相加
    std::cout << "\n--- Test 5: FP16 Same shape tensors ---" << std::endl;
    {
        MLUSubTestCase test({4, 4}, {4, 4}, GGML_TYPE_F16);
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 6: FP16 广播相加 (标量 + 张量)
    std::cout << "\n--- Test 6: FP16 Scalar + Tensor broadcast ---" << std::endl;
    {
        MLUSubTestCase test({4, 4}, {1}, GGML_TYPE_F16);
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 7: FP16 向量广播
    std::cout << "\n--- Test 7: FP16 Vector broadcast ---" << std::endl;
    {
        MLUSubTestCase test({4, 4}, {4, 1}, GGML_TYPE_F16);
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 8: FP16 更大的张量
    std::cout << "\n--- Test 8: FP16 Larger tensors ---" << std::endl;
    {
        MLUSubTestCase test({1024, 1024, 8}, {1, 1024}, GGML_TYPE_F16);
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    std::cout << "\n=== Test Summary ===" << std::endl;
    if (all_tests_passed) {
        std::cout << "All tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED!" << std::endl;
        return 1;
    }
#endif // GGML_USE_MLU
}