#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cassert>
#include <cstring>
#include "ggml.h"
#include "ggml-alloc.h"
#include "ggml-backend.h"
#include "ggml-cpu.h"
#ifdef GGML_USE_MLU
#include "ggml-mlu.h"
#endif

// Function declarations
void init_random_data(float* data, size_t size, float min_val = -1.0f, float max_val = 1.0f);
void init_random_indices(int32_t* indices, size_t size, int32_t max_index);
float calculate_mse(const float* a, const float* b, size_t size);
float calculate_max_abs_error(const float* a, const float* b, size_t size);

void init_random_data(float* data, size_t size, float min_val, float max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min_val, max_val);
    
    for (size_t i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
}

void init_random_indices(int32_t* indices, size_t size, int32_t max_index) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(0, max_index - 1);
    
    for (size_t i = 0; i < size; ++i) {
        indices[i] = dis(gen);
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

// MLU Get Rows 算子测试类
class MLUGetRowsTestCase {
public:
    MLUGetRowsTestCase(const std::vector<int64_t>& params_shape, const std::vector<int64_t>& indices_shape)
        : params_shape_(params_shape), indices_shape_(indices_shape) {
        
        // 计算张量大小
        params_size_ = 1;
        for (auto dim : params_shape_) {
            params_size_ *= dim;
        }
        
        indices_size_ = 1;
        for (auto dim : indices_shape_) {
            indices_size_ *= dim;
        }
        
        // 计算输出张量大小
        // output shape: [params_shape[0], indices_shape[0], indices_shape[1], indices_shape[2]]
        output_size_ = params_shape_[0] * indices_size_;
        
        // 分配内存
        params_data_.resize(params_size_);
        indices_data_.resize(indices_size_);
        result_cpu_.resize(output_size_);
        result_mlu_.resize(output_size_);
        
        // 初始化随机数据
        init_random_data(params_data_.data(), params_size_);
        init_random_indices(indices_data_.data(), indices_size_, params_shape_[1]);  // indices range [0, params_height)
    }
    
    bool run_test() {
        std::cout << "Running MLU Get Rows test..." << std::endl;
        std::cout << "Params shape: [";
        for (size_t i = 0; i < params_shape_.size(); ++i) {
            std::cout << params_shape_[i];
            if (i < params_shape_.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
        std::cout << "Indices shape: [";
        for (size_t i = 0; i < indices_shape_.size(); ++i) {
            std::cout << indices_shape_[i];
            if (i < indices_shape_.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
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
        struct ggml_tensor* params_tensor = ggml_new_tensor(ctx, GGML_TYPE_F32, params_shape_.size(), params_shape_.data());
        struct ggml_tensor* indices_tensor = ggml_new_tensor(ctx, GGML_TYPE_I32, indices_shape_.size(), indices_shape_.data());
        
        // 设置数据
        memcpy(params_tensor->data, params_data_.data(), params_size_ * sizeof(float));
        memcpy(indices_tensor->data, indices_data_.data(), indices_size_ * sizeof(int32_t));
        
        // 执行 Get Rows 操作
        struct ggml_tensor* result = ggml_get_rows(ctx, params_tensor, indices_tensor);
        
        // 构建计算图
        struct ggml_cgraph* gf = ggml_new_graph(ctx);
        ggml_build_forward_expand(gf, result);
        
        // 执行计算
        if (ggml_backend_graph_compute(cpu_backend, gf) != GGML_STATUS_SUCCESS) {
            std::cerr << "CPU computation failed" << std::endl;
            ggml_free(ctx);
            ggml_backend_free(cpu_backend);
            return false;
        }
        
        // 复制结果
        memcpy(result_cpu_.data(), result->data, output_size_ * sizeof(float));
        
        // 清理资源
        ggml_free(ctx);
        ggml_backend_free(cpu_backend);
        
        return true;
    }
    
    bool run_mlu_test() {
        // 初始化 MLU 后端
        ggml_backend_t mlu_backend = ggml_backend_mlu_init(0);  // 使用设备 0
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
        struct ggml_tensor* params_tensor = ggml_new_tensor(ctx, GGML_TYPE_F32, params_shape_.size(), params_shape_.data());
        struct ggml_tensor* indices_tensor = ggml_new_tensor(ctx, GGML_TYPE_I32, indices_shape_.size(), indices_shape_.data());
        
        // 执行 Get Rows 操作
        struct ggml_tensor* result = ggml_get_rows(ctx, params_tensor, indices_tensor);
        
        // 构建计算图
        struct ggml_cgraph* gf = ggml_new_graph(ctx);
        ggml_build_forward_expand(gf, result);
        
        // 分配后端内存
        ggml_backend_buffer_t buffer = ggml_backend_alloc_ctx_tensors(ctx, mlu_backend);
        if (!buffer) {
            std::cerr << "Failed to allocate MLU buffer" << std::endl;
            ggml_free(ctx);
            ggml_backend_free(mlu_backend);
            return false;
        }
        
        // 设置输入数据
        ggml_backend_tensor_set(params_tensor, params_data_.data(), 0, params_size_ * sizeof(float));
        ggml_backend_tensor_set(indices_tensor, indices_data_.data(), 0, indices_size_ * sizeof(int32_t));
        
        // 执行计算
        if (ggml_backend_graph_compute(mlu_backend, gf) != GGML_STATUS_SUCCESS) {
            std::cerr << "MLU computation failed" << std::endl;
            ggml_backend_buffer_free(buffer);
            ggml_free(ctx);
            ggml_backend_free(mlu_backend);
            return false;
        }
        
        // 获取结果
        ggml_backend_tensor_get(result, result_mlu_.data(), 0, output_size_ * sizeof(float));
        
        // 清理资源
        ggml_backend_buffer_free(buffer);
        ggml_free(ctx);
        ggml_backend_free(mlu_backend);
        
        return true;
    }
    
    bool compare_results() {
        const float tolerance = 1e-5f;
        
        float mse = calculate_mse(result_cpu_.data(), result_mlu_.data(), output_size_);
        float max_error = calculate_max_abs_error(result_cpu_.data(), result_mlu_.data(), output_size_);
        
        std::cout << "Results comparison:" << std::endl;
        std::cout << "  MSE: " << mse << std::endl;
        std::cout << "  Max absolute error: " << max_error << std::endl;
        
        if (max_error > tolerance) {
            std::cerr << "Test FAILED: Max error " << max_error << " exceeds tolerance " << tolerance << std::endl;
            
            // 打印前几个不匹配的值用于调试
            std::cout << "First few mismatched values:" << std::endl;
            int count = 0;
            for (size_t i = 0; i < output_size_ && count < 10; ++i) {
                float error = std::abs(result_cpu_[i] - result_mlu_[i]);
                if (error > tolerance) {
                    std::cout << "  [" << i << "] CPU: " << result_cpu_[i] 
                              << ", MLU: " << result_mlu_[i] 
                              << ", Error: " << error << std::endl;
                    count++;
                }
            }
            return false;
        }
        
        std::cout << "Test PASSED: Results match within tolerance" << std::endl;
        return true;
    }
    
    std::vector<int64_t> params_shape_, indices_shape_;
    size_t params_size_, indices_size_, output_size_;
    std::vector<float> params_data_;
    std::vector<int32_t> indices_data_;
    std::vector<float> result_cpu_, result_mlu_;
};

int main(int argc, char** argv) {
    (void)argc; // 避免未使用参数警告
    (void)argv; // 避免未使用参数警告
    std::cout << "=== MLU Get Rows Operator Test ===" << std::endl;

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

    // 测试用例 1: 2D params tensor, 1D indices
    // params: [8, 16], indices: [5] -> requires params.ne[2] == indices.ne[1], so params: [8, 16, 1], indices: [5, 1]
    std::cout << "\n--- Test 1: 2D params [8, 16, 1], 1D indices [5, 1] ---" << std::endl;
    {
        MLUGetRowsTestCase test({8, 16, 1}, {5, 1});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 2: 2D params tensor, 2D indices
    // params: [10, 20], indices: [3, 4] -> requires params.ne[2] == indices.ne[1], so params: [10, 20, 4], indices: [3, 4]
    std::cout << "\n--- Test 2: 3D params [10, 20, 4], 2D indices [3, 4] ---" << std::endl;
    {
        MLUGetRowsTestCase test({10, 20, 4}, {3, 4});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 3: 3D params tensor, 1D indices
    // params: [6, 8, 4], indices: [5] -> requires params.ne[2] == indices.ne[1], so params: [6, 8, 1], indices: [5, 1]
    std::cout << "\n--- Test 3: 3D params [6, 8, 1], 1D indices [5, 1] ---" << std::endl;
    {
        MLUGetRowsTestCase test({6, 8, 1}, {5, 1});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 4: 3D params tensor, 2D indices
    // params: [12, 16, 8], indices: [4, 8] -> this should work as params.ne[2] == indices.ne[1]
    std::cout << "\n--- Test 4: 3D params [12, 16, 8], 2D indices [4, 8] ---" << std::endl;
    {
        MLUGetRowsTestCase test({12, 16, 8}, {4, 8});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 5: 3D params tensor, 3D indices
    // params: [8, 12, 6], indices: [3, 6, 1] -> this should work as params.ne[2] == indices.ne[1]
    std::cout << "\n--- Test 5: 3D params [8, 12, 6], 3D indices [3, 6, 1] ---" << std::endl;
    {
        MLUGetRowsTestCase test({8, 12, 6}, {3, 6, 1});
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }

    // 测试用例 6: 较大的张量
    std::cout << "\n--- Test 6: Larger tensors [128, 256, 1], [64, 1] ---" << std::endl;
    {
        MLUGetRowsTestCase test({128, 256, 1}, {64, 1});
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
