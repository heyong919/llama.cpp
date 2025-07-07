
#ifndef CNPX_TYPES_H_
#define CNPX_TYPES_H_
#define CNPX_VERSION_0 0
#define CNPX_VERSION_1 1
#define CNPX_VERSION_2 2
#define CNPX_VERSION_3 3
#define CNPX_VERSION_4 4
#define CNPX_VERSION_5 5
#define CNPX_VERSION CNPX_VERSION_5
#ifndef CNPX_ENABLED
#error Never include this file directly -- it is automatically included by cnpx.h
#endif
#include <stdint.h>

typedef void (*cnpxInit_t)(const void *reserved);
typedef void (*cnpxMark_t)(const char *message);
typedef int (*cnpxRangePush_t)(const char *message);
typedef int (*cnpxRangePop_t)();
typedef void (*cnpxNameOsThread_t)(const char *name);
typedef void (*cnpxNameQueue_t)(const char *name, CNqueue queue);
typedef int (*cnpxRangeStart_t)(const char *message, void **handle);
typedef void (*cnpxRangeEnd_t)(void *handle);
typedef cnpxDomainHandle_t (*cnpxDomainCreate_t)(const char *name);
typedef void (*cnpxDomainDestroy_t)(cnpxDomainHandle_t domain);
typedef void (*cnpxDomainMark_t)(cnpxDomainHandle_t domain, const char *message);
typedef int (*cnpxDomainRangePush_t)(cnpxDomainHandle_t domain, const char *message);
typedef int (*cnpxDomainRangePop_t)(cnpxDomainHandle_t domain);
typedef int (*cnpxDomainRangeStart_t)(cnpxDomainHandle_t domain, const char *message, void **handle);
typedef void (*cnpxDomainRangeEnd_t)(cnpxDomainHandle_t domain, void *handle);
typedef void (*cnpxNameCNdev_t)(const char *name, CNdev device);
typedef void (*cnpxNameCNcontext_t)(const char *name, CNcontext context);
typedef void (*cnpxNameCNnotifier_t)(const char *name, CNnotifier notifier);
typedef void (*cnpxNameCNqueue_t)(const char *name, CNqueue queue);
typedef uint32_t (*cnpxDomainRegisterString_t)(cnpxDomainHandle_t domain, const char *string);
typedef uint64_t (*cnpxPayloadSchemaRegister_t)(cnpxDomainHandle_t domain, const cnpxPayloadSchemaAttr_t *attr);
typedef uint64_t (*cnpxPayloadEnumRegister_t)(cnpxDomainHandle_t domain, const cnpxPayloadEnumAttr_t *attr);
typedef void (*cnpxMarkPayload_t)(cnpxDomainHandle_t domain, const cnpxPayloadData_t *payloadData, size_t count);
typedef int (*cnpxRangePushPayload_t)(cnpxDomainHandle_t domain, const cnpxPayloadData_t *payloadData, size_t count);
typedef int (*cnpxRangePopPayload_t)(cnpxDomainHandle_t domain, const cnpxPayloadData_t *payloadData, size_t count);
typedef int (*cnpxRangeStartPayload_t)(cnpxDomainHandle_t domain, void **handle, const cnpxPayloadData_t *payloadData,
                                       size_t count);
typedef int (*cnpxRangeEndPayload_t)(cnpxDomainHandle_t domain, void *handle, const cnpxPayloadData_t *payloadData, size_t count);
typedef void (*cnpxInitInternalV1_t)(uint64_t *sizeof_types, uint64_t *aligned_types, size_t types_size);

// Only add new elements to the back of this struct, do not update pre elements.
typedef struct cnpxApi {
  volatile uint32_t init;
  uint32_t version;
  struct {
    cnpxInit_t f_cnpxInit;
    cnpxMark_t f_cnpxMark;
    cnpxRangePush_t f_cnpxRangePush;
    cnpxRangePop_t f_cnpxRangePop;
    cnpxNameOsThread_t f_cnpxNameOsThread;
    cnpxNameQueue_t f_cnpxNameQueue;
    cnpxRangeStart_t f_cnpxRangeStart;
    cnpxRangeEnd_t f_cnpxRangeEnd;
    cnpxDomainCreate_t f_cnpxDomainCreate;
    cnpxDomainDestroy_t f_cnpxDomainDestroy;
    cnpxDomainMark_t f_cnpxDomainMark;
    cnpxDomainRangePush_t f_cnpxDomainRangePush;
    cnpxDomainRangePop_t f_cnpxDomainRangePop;
    cnpxDomainRangeStart_t f_cnpxDomainRangeStart;
    cnpxDomainRangeEnd_t f_cnpxDomainRangeEnd;
    cnpxNameCNdev_t f_cnpxNameCNdev;
    cnpxNameCNcontext_t f_cnpxNameCNcontext;
    cnpxNameCNnotifier_t f_cnpxNameCNnotifier;
    cnpxNameCNqueue_t f_cnpxNameCNqueue;
    cnpxDomainRegisterString_t f_cnpxDomainRegisterString;
    cnpxPayloadSchemaRegister_t f_cnpxPayloadSchemaRegister;
    cnpxPayloadEnumRegister_t f_cnpxPayloadEnumRegister;
    cnpxMarkPayload_t f_cnpxMarkPayload;
    cnpxRangePushPayload_t f_cnpxRangePushPayload;
    cnpxRangePopPayload_t f_cnpxRangePopPayload;
    cnpxRangeStartPayload_t f_cnpxRangeStartPayload;
    cnpxRangeEndPayload_t f_cnpxRangeEndPayload;
    cnpxInitInternalV1_t f_cnpxInitInternalV1;
  };
} cnpxApi_t;

#endif  // CNPX_TYPES_H_
