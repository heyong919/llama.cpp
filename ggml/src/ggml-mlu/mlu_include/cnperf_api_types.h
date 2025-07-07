#ifndef CNPERF_API_TYPES_H
#define CNPERF_API_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum cnperfResult_t {
  CNPERF_SUCCESS,
  CNPERF_ERROR_NOT_INITIALIZED,
  CNPERF_ERROR_INVALID_OPERATION,
  CNPERF_ERROR_BAD_PARAM,
  CNPERF_RESULT_SIZE
} cnperfResult_t;

struct cnperfConfigImpl;

typedef struct cnperfConfigImpl* cnperfConfig_t;

struct cnperfSessionImpl;

typedef struct cnperfSessionImpl* cnperfSession_t;

typedef enum cnperfTracingStatus_t {
  CNPERF_TRACING_STATUS_INVALID = 0,
  CNPERF_TRACING_STATUS_TRACING,
  CNPERF_TRACING_STATUS_FAILED,
  CNPERF_TRACING_STATUS_SUCCESS,
  CNPERF_TRACING_STATUS_POSTPROCESSING,
  CNPERF_TRACING_STATUS_SIZE
} cnperfTracingStatus_t;

#define CNPERF_DATA_VERSION_1 1
#define CNPERF_DATA_VERSION_CURRENT CNPERF_DATA_VERSION_1

typedef struct cnperfDataOpRange_t {
  int32_t version;
  int32_t thread_id;
  uint64_t start;
  uint64_t end;
  uint64_t external_correlation_id;
  const char* name;
  const char* extra;
} cnperfDataOpRange_t;


typedef struct cnperfDataFunction_t {
  int32_t version;
  int32_t thread_id;
  uint64_t correlation_id;
  uint64_t start;
  uint64_t end;
  const char* name;
  const char* extra;
} cnperfDataFunction_t;

typedef enum cnperfDataDeviceTaskType_t {
  CNPERF_DATA_DEVICE_TASK_KERNEL = 0,
  CNPERF_DATA_DEVICE_TASK_ATOMIC,
  CNPERF_DATA_DEVICE_TASK_HOST_FUNC,
  CNPERF_DATA_DEVICE_TASK_MEMCPY,
  CNPERF_DATA_DEVICE_TASK_MEMORY,
  CNPERF_DATA_DEVICE_TASK_MEMSET,
  CNPERF_DATA_DEVICE_TASK_NOTIFIER,
  CNPERF_DATA_DEVICE_TASK_SIZE
} cnperfDataDeviceTaskType_t;

typedef struct cnperfDataDeviceTask_t {
  cnperfDataDeviceTaskType_t type;
  int32_t process_id;
  uint64_t correlation_id;
  uint64_t start;
  uint64_t end;
  uint64_t device_id;
  uint64_t context_id;
  uint64_t queue_id;
  uint64_t tasktopo_id;
  uint64_t tasktopo_node_id;
  uint64_t is_async;
  const char* name;
  const char* extra;
} cnperfDataDeviceTask_t;

typedef struct cnperfDataDeviceTaskKernel_t {
  cnperfDataDeviceTaskType_t type;
  int32_t process_id;
  uint64_t correlation_id;
  uint64_t start;
  uint64_t end;
  uint64_t device_id;
  uint64_t context_id;
  uint64_t queue_id;
  uint64_t tasktopo_id;
  uint64_t tasktopo_node_id;
  uint64_t is_async;
  const char* name;
  const char* extra;
  int32_t is_computation_kernel;
  int32_t kernel_type;
  int32_t dimx;
  int32_t dimy;
  int32_t dimz;
} cnperfDataDeviceTaskKernel_t;


typedef enum cnperfDataPmuValueType_t {
  CNPERF_DATA_PMU_VALUE_TYPE_UINT64,
  CNPERF_DATA_PMU_VALUE_TYPE_INT64,
  CNPERF_DATA_PMU_VALUE_TYPE_DOUBLE,
  CNPERF_DATA_PMU_VALUE_TYPE_SIZE
} cnperfDataPmuValueType_t;

typedef union cnperfDataPmuValue_t {
  uint64_t u64;
  int64_t i64;
  double dbl;
} cnperfDataPmuValue_t;

typedef struct cnperfDataPmuInfo_t {
  const char* name;
  cnperfDataPmuValueType_t type;
  union {
    // -1 if not valid
    int32_t cluster_id;
    int32_t llc_id;
    int32_t index_0;
  };
  union {
    // -1 if not valid
    int32_t core_id;
    int32_t index_1;
  };
} cnperfDataPmuInfo_t;

typedef enum cnperfDataCommTaskType_t {
  CNPERF_DATA_COMM_TASK_TYPE_UNKNOWN,
  CNPERF_DATA_COMM_TASK_TYPE_COLLECTIVE,
  CNPERF_DATA_COMM_TASK_TYPE_P2P,
  CNPERF_DATA_COMM_TASK_TYPE_SIZE
} cnperfDataCommTaskType_t;

typedef struct cnperfDataCommTask_t {
  cnperfDataCommTaskType_t type;
  int32_t rank;
  uint64_t clique_id;
  uint64_t start;
  uint64_t end;
  uint64_t bytes;
  const char* name;
} cnperfDataCommTask_t;

#ifdef __cplusplus
}
#endif


#endif  // CNPERF_API_TYPES_H