#ifndef CNPERF_API_H
#define CNPERF_API_H

#include <stdint.h>
#include "cnperf_api_types.h"
#include "cnperf_api_record_data.h"
#include "cnperf_api_parser.h"
#ifdef __cplusplus
extern "C" {
#endif

cnperfResult_t cnperfInit(int flag);

cnperfResult_t cnperfSetBaseDir(const char* base_dir);

cnperfResult_t cnperfConfigSet(cnperfConfig_t config, const char* key, const void* value, int64_t value_size);

cnperfResult_t cnperfConfigGet(cnperfConfig_t config, const char* key, void* value, int64_t* value_size);

cnperfResult_t cnperfConfigCreate(cnperfConfig_t* config);

cnperfResult_t cnperfConfigDestroy(cnperfConfig_t config);

cnperfResult_t cnperfConfigEnable(cnperfConfig_t config);

cnperfResult_t cnperfStart(cnperfSession_t* session);

cnperfResult_t cnperfStop();

cnperfResult_t cnperfSessionDestroy(cnperfSession_t session);

cnperfResult_t cnperfSessionGetResultPath(cnperfSession_t session, char** path);

cnperfResult_t cnperfRelease();

cnperfResult_t cnperfGetLibVersion(char const ** version);

#ifdef __cplusplus
}
#endif

#endif  // CNPERF_API_H