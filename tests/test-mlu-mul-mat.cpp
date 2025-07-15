#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cassert>
#include <cstring>
#include <chrono>
#include <thread>
#include <atomic>
#include <fstream>
#include <iomanip>
#include <string>
#include "ggml.h"
#include "ggml-alloc.h"
#include "ggml-backend.h"
#include "ggml-cpu.h"
#include "ggml-impl.h"
#ifdef GGML_USE_MLU
#include "ggml-mlu.h"
#endif

// Function declarations
void init_random_data(float* data, size_t size, float min_val = -1.0f, float max_val = 1.0f);
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

// 计算均方误差
float calculate_mse(const float* a, const float* b, size_t size) {
    float mse = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        float diff = a[i] - b[i];
        mse += diff * diff;
    }
    return mse / size;
}

// 计算最大绝对误差
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

// MLU 矩阵乘法测试类
class MLUMulMatTestCase {
public:
    MLUMulMatTestCase(int64_t m, int64_t n, int64_t k)
        : m_(m), n_(n), k_(k) {
        
        // 计算张量大小
        size_a_ = m * k;  // A: [m, k]
        size_b_ = k * n;  // B: [k, n]
        size_c_ = m * n;  // C: [m, n]
        
        // 分配内存
        data_a_.resize(size_a_);
        data_b_.resize(size_b_);
        result_cpu_.resize(size_c_);
        result_mlu_.resize(size_c_);
        
        // 初始化随机数据
        init_random_data(data_a_.data(), size_a_, -0.5f, 0.5f);
        init_random_data(data_b_.data(), size_b_, -0.5f, 0.5f);
    }
    
    bool run_test() {
        std::cout << "Running MLU MulMat test [" << m_ << "x" << k_ << "] * [" 
                  << k_ << "x" << n_ << "] = [" << m_ << "x" << n_ << "]" << std::endl;
        
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
        
        // 创建张量 A: k columns, m rows => [k_, m_], B: k columns, n rows => [k_, n_]
        struct ggml_tensor* tensor_a = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, k_, m_);
        struct ggml_tensor* tensor_b = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, k_, n_);
        
        // 设置数据
        memcpy(tensor_a->data, data_a_.data(), size_a_ * sizeof(float));
        memcpy(tensor_b->data, data_b_.data(), size_b_ * sizeof(float));
        
        // 执行矩阵乘法操作
        struct ggml_tensor* result = ggml_mul_mat(ctx, tensor_a, tensor_b);
        
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
        memcpy(result_cpu_.data(), result->data, size_c_ * sizeof(float));
        
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
        
        // 创建张量 A: k columns, m rows => [k_, m_], B: k columns, n rows => [k_, n_]
        struct ggml_tensor* tensor_a = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, k_, m_);
        struct ggml_tensor* tensor_b = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, k_, n_);
        
        // 执行矩阵乘法操作
        struct ggml_tensor* result = ggml_mul_mat(ctx, tensor_a, tensor_b);
        
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
        ggml_backend_tensor_set(tensor_a, data_a_.data(), 0, size_a_ * sizeof(float));
        ggml_backend_tensor_set(tensor_b, data_b_.data(), 0, size_b_ * sizeof(float));
        
        // 执行计算 - 添加超时和详细调试
        std::cout << "Starting MLU computation..." << std::endl;
        std::cout << "Graph nodes: " << gf->n_nodes << std::endl;
        std::cout << "Graph leafs: " << gf->n_leafs << std::endl;
        
        // 打印计算图信息
        for (int i = 0; i < gf->n_nodes; i++) {
            struct ggml_tensor * node = gf->nodes[i];
            std::cout << "Node " << i << ": op=" << ggml_op_name(node->op) << ", type=" << node->type
                      << ", ne=[" << node->ne[0] << "," << node->ne[1] << "," << node->ne[2] << "," << node->ne[3] << "]" << std::endl;
        }
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // 使用异步方式执行计算，添加超时检测
        std::atomic<bool> computation_done{false};
        ggml_status result_status = GGML_STATUS_SUCCESS;
        std::thread compute_thread([&]() {
            std::cout << "Compute thread started" << std::endl;
            result_status = ggml_backend_graph_compute(mlu_backend, gf);
            std::cout << "Compute thread finished" << std::endl;
            computation_done = true;
        });
        
        // 等待计算完成或超时（10秒）
        const int timeout_seconds = 10;
        for (int i = 0; i < timeout_seconds * 10; i++) {
            if (computation_done) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (i % 10 == 0) {
                std::cout << "Waiting for computation... " << i / 10 << " seconds" << std::endl;
            }
        }
        
        if (!computation_done) {
            std::cout << "ERROR: MLU computation timed out after " << timeout_seconds << " seconds!" << std::endl;
            compute_thread.detach(); // 分离线程避免阻塞
            ggml_backend_buffer_free(buffer);
            ggml_free(ctx);
            ggml_backend_free(mlu_backend);
            return false;
        }
        
        compute_thread.join();
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "MLU computation took " << duration.count() << " ms" << std::endl;
        
        if (result_status != GGML_STATUS_SUCCESS) {
            std::cerr << "MLU computation failed with status: " << result_status << std::endl;
            ggml_backend_buffer_free(buffer);
            ggml_free(ctx);
            ggml_backend_free(mlu_backend);
            return false;
        }
        
        std::cout << "MLU computation completed successfully!" << std::endl;
        
        // 获取结果
        ggml_backend_tensor_get(result, result_mlu_.data(), 0, size_c_ * sizeof(float));
        
        // 清理资源
        ggml_backend_buffer_free(buffer);
        ggml_free(ctx);
        ggml_backend_free(mlu_backend);
        
        return true;
    }
    
    void save_results_to_files() {
        // 生成基于矩阵大小的文件名
        std::string filename_prefix = "mlu_test_" + std::to_string(m_) + "x" + std::to_string(k_) + "_" + std::to_string(k_) + "x" + std::to_string(n_);
        
        // 保存输入矩阵A
        std::string input_a_filename = filename_prefix + "_input_a.txt";
        std::ofstream input_a_file(input_a_filename);
        if (input_a_file.is_open()) {
            input_a_file << "Input Matrix A [" << m_ << "x" << k_ << "]:" << std::endl;
            input_a_file << std::endl;
            
            for (int64_t i = 0; i < m_; ++i) {
                for (int64_t j = 0; j < k_; ++j) {
                    input_a_file << std::fixed << std::setprecision(6) << data_a_[i * k_ + j];
                    if (j < k_ - 1) input_a_file << "\t";
                }
                input_a_file << std::endl;
            }
            input_a_file.close();
            std::cout << "Input matrix A saved to: " << input_a_filename << std::endl;
        }
        
        // 保存输入矩阵B
        std::string input_b_filename = filename_prefix + "_input_b.txt";
        std::ofstream input_b_file(input_b_filename);
        if (input_b_file.is_open()) {
            input_b_file << "Input Matrix B [" << k_ << "x" << n_ << "]:" << std::endl;
            input_b_file << std::endl;
            
            for (int64_t i = 0; i < k_; ++i) {
                for (int64_t j = 0; j < n_; ++j) {
                    input_b_file << std::fixed << std::setprecision(6) << data_b_[i * n_ + j];
                    if (j < n_ - 1) input_b_file << "\t";
                }
                input_b_file << std::endl;
            }
            input_b_file.close();
            std::cout << "Input matrix B saved to: " << input_b_filename << std::endl;
        }
        
        // 保存CPU结果
        std::string cpu_filename = filename_prefix + "_cpu.txt";
        std::ofstream cpu_file(cpu_filename);
        if (cpu_file.is_open()) {
            cpu_file << "CPU Matrix Multiplication Result [" << m_ << "x" << n_ << "]:" << std::endl;
            cpu_file << "Matrix dimensions: A[" << m_ << "x" << k_ << "] * B[" << k_ << "x" << n_ << "] = C[" << m_ << "x" << n_ << "]" << std::endl;
            cpu_file << std::endl;
            
            for (int64_t i = 0; i < m_; ++i) {
                for (int64_t j = 0; j < n_; ++j) {
                    cpu_file << std::fixed << std::setprecision(6) << result_cpu_[i * n_ + j];
                    if (j < n_ - 1) cpu_file << "\t";
                }
                cpu_file << std::endl;
            }
            cpu_file.close();
            std::cout << "CPU results saved to: " << cpu_filename << std::endl;
        }
        
        // 保存MLU结果
        std::string mlu_filename = filename_prefix + "_mlu.txt";
        std::ofstream mlu_file(mlu_filename);
        if (mlu_file.is_open()) {
            mlu_file << "MLU Matrix Multiplication Result [" << m_ << "x" << n_ << "]:" << std::endl;
            mlu_file << "Matrix dimensions: A[" << m_ << "x" << k_ << "] * B[" << k_ << "x" << n_ << "] = C[" << m_ << "x" << n_ << "]" << std::endl;
            mlu_file << std::endl;
            
            for (int64_t i = 0; i < m_; ++i) {
                for (int64_t j = 0; j < n_; ++j) {
                    mlu_file << std::fixed << std::setprecision(6) << result_mlu_[i * n_ + j];
                    if (j < n_ - 1) mlu_file << "\t";
                }
                mlu_file << std::endl;
            }
            mlu_file.close();
            std::cout << "MLU results saved to: " << mlu_filename << std::endl;
        }
        
        // 保存差异结果
        std::string diff_filename = filename_prefix + "_diff.txt";
        std::ofstream diff_file(diff_filename);
        if (diff_file.is_open()) {
            diff_file << "Difference Matrix (CPU - MLU) [" << m_ << "x" << n_ << "]:" << std::endl;
            diff_file << "Matrix dimensions: A[" << m_ << "x" << k_ << "] * B[" << k_ << "x" << n_ << "] = C[" << m_ << "x" << n_ << "]" << std::endl;
            diff_file << std::endl;
            
            for (int64_t i = 0; i < m_; ++i) {
                for (int64_t j = 0; j < n_; ++j) {
                    float diff = result_cpu_[i * n_ + j] - result_mlu_[i * n_ + j];
                    diff_file << std::fixed << std::setprecision(6) << diff;
                    if (j < n_ - 1) diff_file << "\t";
                }
                diff_file << std::endl;
            }
            diff_file.close();
            std::cout << "Difference results saved to: " << diff_filename << std::endl;
        }
    }
    
    bool compare_results() {
        const float tolerance = 1e-4f;  // 矩阵乘法的容差稍大一些
        
        // 保存CPU和MLU结果到文件
        // save_results_to_files();
        
        float mse = calculate_mse(result_cpu_.data(), result_mlu_.data(), size_c_);
        float max_error = calculate_max_abs_error(result_cpu_.data(), result_mlu_.data(), size_c_);
        
        std::cout << "Results comparison:" << std::endl;
        std::cout << "  MSE: " << mse << std::endl;
        std::cout << "  Max absolute error: " << max_error << std::endl;
        
        if (max_error > tolerance) {
            std::cerr << "Test FAILED: Max error " << max_error << " exceeds tolerance " << tolerance << std::endl;
            
            // 打印前几个不匹配的值用于调试
            std::cout << "First few mismatched values:" << std::endl;
            int count = 0;
            for (size_t i = 0; i < size_c_ && count < 10; ++i) {
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
    
    int64_t m_, n_, k_;
    size_t size_a_, size_b_, size_c_;
    std::vector<float> data_a_, data_b_;
    std::vector<float> result_cpu_, result_mlu_;
};

int main(int argc, char** argv) {
    (void)argc; // 避免未使用参数警告
    (void)argv; // 避免未使用参数警告
    std::cout << "=== MLU Matrix Multiplication Test ===" << std::endl;
    
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
    
    // 测试用例 1: 矩形矩阵 (2x3)*(3x4)=(2x4)
    std::cout << "\n--- Test 1: Rectangular matrices (2x3)*(3x4) ---" << std::endl;
    {
        MLUMulMatTestCase test(2, 4, 3);  // m=2, n=4, k=3
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }
    
    // 测试用例 2: 矩形矩阵
    std::cout << "\n--- Test 2: Rectangular matrices ---" << std::endl;
    {
        MLUMulMatTestCase test(8, 16, 12);
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }
    
    // 测试用例 3: 中等大小矩阵
    std::cout << "\n--- Test 3: Medium matrices ---" << std::endl;
    {
        MLUMulMatTestCase test(64, 128, 96);
        if (!test.run_test()) {
            all_tests_passed = false;
        }
    }
    
    // 测试用例 4: 大矩阵
    std::cout << "\n--- Test 4: Large matrices ---" << std::endl;
    {
        MLUMulMatTestCase test(512, 1024, 768);
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