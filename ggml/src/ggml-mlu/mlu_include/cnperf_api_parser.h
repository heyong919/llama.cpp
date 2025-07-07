#ifndef CNPERF_API_PARSER_H
#define CNPERF_API_PARSER_H

#include <stdint.h>

#include "cnperf_api_types.h"
#ifdef __cplusplus
extern "C" {
#endif

struct cnperfParserHandleImpl;
typedef struct cnperfParserHandleImpl* cnperfParserHandle_t;

cnperfResult_t cnperfParserCreateFromPath(const char* path, cnperfParserHandle_t* handle);
cnperfResult_t cnperfParserCreateFromSession(cnperfSession_t session, cnperfParserHandle_t* handle);
cnperfResult_t cnperfParserDestroy(cnperfParserHandle_t handle);

typedef void (*cnperfParserKernelPmuDataCallback)(const cnperfDataPmuInfo_t* pmu_info, const cnperfDataPmuValue_t* value,
                                                  void* userdata);
cnperfResult_t cnperfParserGetKernelPmuData(cnperfParserHandle_t handle, uint64_t correlation_id, uint64_t tasktopo_id,
                                            uint64_t tasktopo_node_id, cnperfParserKernelPmuDataCallback callback,
                                            void* userdata);

typedef void (*cnperfParserKernelHighRatePmuDataCallback)(const cnperfDataPmuInfo_t* pmu_info, const cnperfDataPmuValue_t* value,
                                                          uint64_t size, void* userdata);
cnperfResult_t cnperfParserGetKernelHighRatePmuData(cnperfParserHandle_t handle, uint64_t correlation_id, uint64_t tasktopo_id,
                                                    uint64_t tasktopo_node_id, cnperfParserKernelHighRatePmuDataCallback callback,
                                                    void* userdata);

// only provide data torch needed for now
typedef enum cnperfParserDataType_t {
  CNPERF_PARSER_DATA_TYPE_FUNCTION,
  CNPERF_PARSER_DATA_TYPE_DEVICE_TASK,
  CNPERF_PARSER_DATA_TYPE_DEVICE_TASK_KERNEL,
  CNPERF_PARSER_DATA_TYPE_SIZE
} cnperfParserDataType_t;

typedef void (*cnperfParserDataCallback)(void* data, void* userdata);
cnperfResult_t cnperfParserGetData(cnperfParserHandle_t handle, cnperfParserDataType_t data_type,
                                   cnperfParserDataCallback callback, void* userdata);

cnperfResult_t cnperfParserGetOpRanges(cnperfParserHandle_t handle, uint64_t correlation_id, cnperfParserDataCallback callback,
                                       void* userdata);
cnperfResult_t cnperfParserGetTaskTopoNodeOpRanges(cnperfParserHandle_t handle, uint64_t tasktopo_id,
                                                   uint64_t tasktopo_node_id, cnperfParserDataCallback callback, void* userdata);
cnperfResult_t cnperfParserGetStartTimestamp(cnperfParserHandle_t handle, uint64_t* timestamp);

typedef void (*cnperfParserCommDataCallback)(const cnperfDataOpRange_t* op_range, const cnperfDataCommTask_t* data,
                                             void* userdata);
cnperfResult_t cnperfParserGetCommDataInOpRange(cnperfParserHandle_t handle, cnperfParserCommDataCallback callback,
                                                void* userdata);

#ifdef __cplusplus
}
#endif

#endif  // CNPERF_API_PARSER_H