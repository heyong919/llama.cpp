#pragma once

#include "mlu_include/cndev.h"
#include "mlu_include/cnrt.h"
#include "mlu_include/cnnl.h"
#include "mlu_include/cnnl_extra.h"

#ifndef CNRT_CHECK
#define CNRT_CHECK(err)                                                                                  \
    do {                                                                                                 \
        cnrtRet_t err_ = (err);                                                                          \
        if (err_ != CNRT_RET_SUCCESS) {                                                                  \
            fprintf(stderr, "MLU error %d at %s:%d: %s\n", err_, __FILE__, __LINE__, cnrtGetErrorStr(err_)); \
            exit(1);                                                                                      \
        }                                                                                                 \
    } while (0)
#endif

#ifndef CNNL_CHECK
#define CNNL_CHECK(err)                                                                                   \
    do {                                                                                                  \
        cnnlStatus_t err_ = (err);                                                                        \
        if (err_ != CNNL_STATUS_SUCCESS) {                                                                \
            fprintf(stderr, "MLU error %d at %s:%d\n", err_, __FILE__, __LINE__);                       \
            exit(1);                                                                                      \
        }                                                                                                 \
    } while (0)
#endif

#ifndef CNDEV_CHECK
#define CNDEV_CHECK(err)                                                                                  \
    do {                                                                                                  \
        cndevRet_t err_ = (err);                                                                          \
        if (err_ != CNDEV_SUCCESS) {                                                                      \
            fprintf(stderr, "CNDEV error %d at %s:%d\n", err_, __FILE__, __LINE__);                       \
            exit(1);                                                                                      \
        }                                                                                                 \
    } while (0)
#endif