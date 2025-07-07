#ifndef CNPERF_API_RECORD_DATA_H
#define CNPERF_API_RECORD_DATA_H

#include <stdint.h>
#include "cnperf_api_types.h"
#ifdef __cplusplus
extern "C" {
#endif

uint64_t cnperfGetTimestamp();


typedef enum cnperfRecordDataType_t {
  CNPERF_RECORD_DATA_TYPE_INVALID = 0,
  CNPERF_RECORD_DATA_TYPE_OP_RANGE,
  CNPERF_RECORD_DATA_TYPE_SIZE
} cnperfRecordDataType_t;

cnperfResult_t cnperfSessionRecordData(cnperfSession_t session, cnperfRecordDataType_t dataType, void* data);


#ifdef __cplusplus
}
#endif


#endif  // CNPERF_API_RECORD_DATA_H