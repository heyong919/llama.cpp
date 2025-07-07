#pragma once

#include "ggml.h"
#include "ggml-common.h"
#include "ggml-backend.h"
// #include "ggml-backend-impl.h"

#ifdef __cplusplus
extern "C" {
#endif

GGML_BACKEND_API ggml_backend_reg_t ggml_backend_mlu_reg(void);

/**
 * @brief Initializes the MLU backend for a specified device.
 *
 * @param device The index of the device to initialize.
 * @return A pointer to the initialized backend instance, or nullptr on failure.
 */
GGML_BACKEND_API ggml_backend_t ggml_backend_mlu_init(int32_t device);

/**
 * @brief Checks if a given backend is an MLU backend.
 *
 * @param backend The backend instance to check.
 * @return True if the backend is an MLU backend, false otherwise.
 */
GGML_BACKEND_API bool ggml_backend_is_mlu(ggml_backend_t backend);

/**
 * @brief Retrieves the MLU buffer type for a specified device.
 *
 * @param device The device index for which to retrieve the buffer type.
 * @return A pointer to the buffer type interface for the specified device.
 */
GGML_BACKEND_API ggml_backend_buffer_type_t ggml_backend_mlu_buffer_type(int32_t device);

/**
 * @brief Retrieves the number of MLU devices available.
 *
 * @return The number of MLU devices available.
 */
GGML_BACKEND_API int32_t ggml_backend_mlu_get_device_count(void);

/**
 * @brief Pinned host buffer for use with the CPU backend for faster copies between CPU and MLU.
 *
 * @return A pointer to the host buffer type interface.
 */
GGML_BACKEND_API ggml_backend_buffer_type_t ggml_backend_mlu_host_buffer_type(void);

/**
 * @brief Retrieves the description of a specific MLU device.
 *
 * @param device The device index to retrieve the description for.
 * @param description Pointer to a buffer where the description will be written.
 * @param description_size Size of the description buffer.
 */
GGML_BACKEND_API void ggml_backend_mlu_get_device_description(
    int32_t device, char* description, size_t description_size);

/**
 * @brief Retrieves the memory information of a specific MLU device.
 *
 * @param device The device index to retrieve memory information for.
 * @param free Pointer to a variable where the free memory size will be stored.
 * @param total Pointer to a variable where the total memory size will be stored.
 */
GGML_BACKEND_API void ggml_backend_mlu_get_device_memory(int32_t device,
                                                  size_t* free,
                                                  size_t* total);

/**
 * @brief Creates an MLU split buffer type for multi-device tensor splitting.
 *
 * @param main_device The main device index.
 * @param tensor_split Array of split ratios for each device (can be NULL for equal split).
 * @return A pointer to the split buffer type interface.
 */
GGML_BACKEND_API ggml_backend_buffer_type_t ggml_backend_mlu_split_buffer_type(int main_device, const float * tensor_split);

/**
 * @brief Checks if a buffer is an MLU split buffer.
 *
 * @param buffer The buffer to check.
 * @return True if the buffer is an MLU split buffer, false otherwise.
 */
GGML_BACKEND_API bool ggml_backend_buffer_is_mlu_split(ggml_backend_buffer_t buffer);

#ifdef __cplusplus
}
#endif