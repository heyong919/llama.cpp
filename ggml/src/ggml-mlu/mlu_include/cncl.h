/***********************************************************************************
 * Copyright (c) [2019-2025] by Cambricon, Inc.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ***********************************************************************************/

/***********************************************************************************
 *
 *  @file cncl.h
 *
 *  @brief CNCL (Cambricon Communication Library) APIs provide programmable
 *  interfaces for users to develop their-owned programs, which includes communicator
 *  management, collective communication primitives, etc.
 *
 ***********************************************************************************/

#ifndef CNCL_H_  // NOLINT
#define CNCL_H_  // NOLINT

#define CNCL_MAJOR_VERSION 1
#define CNCL_MINOR_VERSION 25
#define CNCL_PATCH_VERSION 0

#ifndef CNCL_VERSION
#define CNCL_VERSION \
  (CNCL_MAJOR_VERSION * 10000 + CNCL_MINOR_VERSION * 100 + CNCL_PATCH_VERSION)
#endif

#include <cnrt.h>
#include <stdint.h>
#include <stddef.h>
#if defined(__cplusplus)
extern "C" {
#endif /*! __cplusplus*/

/***********************************************************************************
 *  Data Type Definitions
 ***********************************************************************************/

#define CNCL_CHECK(call)                                 \
  do {                                                   \
    cnclResult_t ret_code = (call);                      \
    if (ret_code != CNCL_RET_SUCCESS) {                  \
      printf("[%s:%d] CNCL error, msg: %s, code: %#x\n", \
             __FILE__,                                   \
             __LINE__,                                   \
             cnclGetErrorStr(ret_code),                  \
             ret_code);                                  \
      exit(1);                                           \
    }                                                    \
  } while (0);

/*! @brief The return values of CNCL APIs.
 *
 */
typedef enum {
  CNCL_RET_SUCCESS = 0x0,
  /*!< Returns when no error occurs. */

  CNCL_RET_ERR_INTERNAL = 0x01,
  /*!< Returns when CNCL internal error occurs. */
  CNCL_RET_ERR_UNSUPPORTED = 0x11,
  /*!< This internal error indicates unsupported operation, such as
   *   communication with unsupported data type. */
  CNCL_RET_ERR_NULL_POINTER = 0x21,
  /*!< This internal error indicates null pointer argument. */
  CNCL_RET_ERR_BUILD_RING = 0x31,
  /*!< This internal error returns when the ring topology for communication cannot be built. */
  CNCL_RET_ERR_BUILD_TREE = 0x41,
  /*!< This internal error returns when the tree topology for communication cannot be built. */
  CNCL_RET_ERR_INIT = 0x51,
  /*!< This internal error returns when communicators cannot be initialized. */
  CNCL_RET_ERR_NOT_INIT = 0x61,
  /*!< This internal error indicates uninitialized communicators. */
  CNCL_RET_ERR_REINIT = 0x71,
  /*!< This internal error returns when the communicators cannot be initialized,
   *   because it had been initialized already. */
  CNCL_RET_ERR_NO_ALGO = 0x81,
  /*!< This internal error returns when connections cannot be built between
   *   communicators for communication. */
  CNCL_RET_ERR_INVALID_VERSION = 0x91,
  /*!< This internal error returns when the version of CNCL cannot be extracted. */

  CNCL_RET_ERR_SYSTEM = 0x02,
  /*!< Returns when system error occurs. */
  CNCL_RET_ERR_SYS_MALLOC = 0x12,
  /*!< This system error indicates system memory allocation failure. */
  CNCL_RET_ERR_SYS_SOCKET = 0x22,
  /*!< This system error indicates the failure is caused by socket error. */
  CNCL_RET_ERR_SYS_IBV = 0x32,
  /*!< This system error indicates the failure is caused by InfiniBand verbs error. */

  CNCL_RET_ERR_MLU_RUNTIME = 0x03,
  /*!< Returns when the failure is caused by CNRT(Cambricon Runtime Library) API error. */
  CNCL_RET_ERR_CNRT_DEVICE = 0x13,
  /*!< This CNRT API error returns when the device handle cannot be obtained. */
  CNCL_RET_ERR_CNRT_MEMCOPY = 0x23,
  /*!< This CNRT API error returns when the memory cannot be copied between devices. */
  CNCL_RET_ERR_CNRT_MALLOC = 0x33,
  /*!< This CNRT API error returns when the memory cannot be allocated on device. */

  CNCL_RET_ERR_MLU_DEVICE = 0x04,
  /*!< Returns when the failure is caused by CNDev(Cambricon Device Interface) API error. */
  CNCL_RET_ERR_CNDEV_NODE = 0x14,
  /*!< This Device API error returns when the PCIe topology node of device cannot be obtained. */
  CNCL_RET_ERR_CNDEV_RELATIONSHIP = 0x24,
  /*!< This Device API error returns when the PCIe relationship of two nodes cannot be obtained. */

  CNCL_RET_ERR_MLU_INNETWORK_COMPUTE = 0x05,
  /*!< Returns when the failure is caused by MLU in-network compute API error. */
  CNCL_RET_ERR_CNLINC_TASK = 0x15,
  /*!< This In-network compute API error returns when the task cannot be created. */
  CNCL_RET_ERR_CNLINC_WR = 0x25,
  /*!< This In-network compute API error returns when the work request cannot be created. */
  CNCL_RET_ERR_CNLINC_KERNEL = 0x35,
  /*!< This In-network compute API error returns when the kernel cannot be invoked. */

  CNCL_RET_ASYNC_IN_PROGRESS = 0x06,
  /*!< Returns when the asynchronous operations has not completed yet. This result is not an error
   *   actually. */
  CNCL_RET_ASYNC_ERROR = 0x16,
  /*!< Indicates the asynchronous error. */
  CNCL_RET_ASYNC_UNKNOWN_ERROR = 0x26,
  /*!< Returns when the failure of asynchronous operations is caused by unknown error. This
   *   result may be changed in the future. */

  CNCL_RET_ERR_ARGUMENTS = 0x07,
  /*!< Returns when arguments are illegal. */

  CNCL_RET_ERR_MLU_ALLOCATE = 0x08,
  /*!< Indicates the MLU memory pool allocate error. */
  CNCL_RET_ERR_MLU_DEALLOCATE = 0x18,
  /*!< Indicates the MLU memory pool deallocate error. */

  CNCL_RET_IB_RECV_NOTREADY = 0x09
  /*!< Indicates the Receive side does not post recv request yet. */
} cnclResult_t;

/*! @brief The data types of CNCL.
 *
 */
typedef enum {
  cnclInvalid = 0x0,
  /*!< Invalid data. */

  cnclHalf = 0x10,
  /*!< 16-bit floating-point data. */
  cnclFloat16 = 0x10,
  /*!< 16-bit floating-point data. */
  cnclBfloat16 = 0x11,
  /*!< 16-bit brain floating-point data. */
  cnclFloat = 0x12,
  /*!< 32-bit floating-point data. */
  cnclFloat32 = 0x12,
  /*!< 32-bit floating-point data. */

  cnclInt8 = 0x20,
  /*!< 8-bit integer. */
  cnclChar = 0x20,
  /*!< 8-bit char, which is equal to 8-bit integer. */
  cnclInt16 = 0x21,
  /*!< 16-bit integer. */
  cnclInt = 0x22,
  /*!< 32-bit integer. */
  cnclInt32 = 0x22,
  /*!< 32-bit integer. */
  cnclInt64 = 0x23,
  /*!< 64-bit integer. */

  cnclUint8 = 0x30,
  /*!< 8-bit unsigned integer. */
  cnclUint16 = 0x31,
  /*!< 16-bit unsigned integer. */
  cnclUint32 = 0x32,
  /*!< 32-bit unsigned integer. */
  cnclUint64 = 0x33,
  /*!< 64-bit unsigned integer. */
} cnclDataType_t;

/*! @brief The reduce operation types of CNCL.
 *
 */
typedef enum {
  cnclSum = 0,
  /*!< The sum operation. */
  cnclProd = 1,
  /*!< The product operation. */
  cnclMax = 2,
  /*!< The maximum operation. */
  cnclMin = 3,
  /*!< The minimum operation. */
} cnclReduceOp_t;

/*! @brief The communication priority of CNCL communicators.
 *
 */
typedef enum {
  CNCL_PRIORITY_DEFAULT = 0x0,
  /*!< The default flag. */
  CNCL_PRIORITY_HIGH = 0x10,
  /*!< Indicates the communication tasks will be scheduled priorly. */
  CNCL_PRIORITY_HIGH_FOR_ONCE = 0x11,
  /*!< Indicates the next communication task will be scheduled priorly. */
} cnclPriority_t;

/*! @brief The communicator of CNCL.
 *
 * It is a semi-internal struct.
 *
 * @note:
 * - The ::cnclInitComms API is used to create an array of communicator instances.
 * - The ::cnclDestroyComms API is used to destroy an array of communicator
 *   instances.
 * - The ::cnclFreeComm API is used to destroy a communicator instance.
 */
typedef struct cnclComm* cnclComm_t;

#define CNCL_CLIQUE_ID_BYTES_SIZE 128
/*! @brief The unique ID for clique.
 *
 * @note
 *  - Only communicators which are created with the same ::cnclCliqueId can
 *    communicate with each other.
 *  - ::cnclGetCliqueId is used to acquire a unique ::cnclCliqueId.
 */
struct cnclCliqueId {
  char data[CNCL_CLIQUE_ID_BYTES_SIZE];
  uint64_t hash;
};
/*! A pointer points to the struct cnclCliqueId. */
typedef struct cnclCliqueId* cnclCliqueId_t;

/*! @brief The Communicator configuration.
 *
 */
struct cnclCommConfig {
  cnclPriority_t priority;
};
/*! A pointer points to the struct cnclCommConfig. */
typedef struct cnclCommConfig* cnclCommConfig_t;

/***********************************************************************************
 * Function prototype declaration
 ***********************************************************************************/

/***********************************************************************************
 * Communication Context Management
 ***********************************************************************************/

//Group:Communication Context Management
/**
 * @brief Initializes communicators in current clique. Each communicators is
 * associated to a MLU.
 *
 * @param [out] comms Array of communicators.
 * @param [in] num_comm Number of communicators in current process.
 * @param [in] dev_list Device list associated with communicators.
 * @param [in] rank_list Rank list of communicators in current process.
 * @param [in] nrank Total number of communicators in current clique.
 * @param [in] clique_id Clique identifier.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_NULL_POINTER
 * - ::CNCL_RET_ERR_INIT
 * - ::CNCL_RET_ERR_REINIT
 * - ::CNCL_RET_ERR_NO_ALGO
 * - ::CNCL_RET_ERR_SYS_SOCKET
 * - ::CNCL_RET_ERR_CNRT_MALLOC
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Only the communicators created with the same ``clique_id`` can communicate with
 *   each other.
 * - At least ``num_comm`` count of ::cnclComm_t is allocated in ``comms`` before
 *   this function.
 * - The value in ``rank_list`` must be in the range of [0, ``nrank``-1] and must
 *   keep unique within a clique.
 * - If ``clique_id`` is NULL, CNCL will acquire a clique identifier for
 *   initialization, which is only supported in single process communication. For
 *   multi-processes communication, ``clique_id`` must be acquired by
 *   ::cnclGetCliqueId.
 * - One ``clique_id`` can only be used for initialization once in each process.
 * - To initialize communicators in different cliques, the sequence
 *   of ::cnclInitComms among different cliques must be same in different processes.
 *
 * @par Requirements
 * - cn_api.h, cndev.h, cnrt.h
 * - libcndrv.so, libcndev.so, libcnrt.so
 *
 * @par Example
 * @code
 *   cnrtInit(0);
 *
 *   uint32_t num_dev = 0;
 *   int num_comms = 4;
 *   int nrank = num_comms;
 *
 *   // request source
 *   int* dev_list = new int[num_comms];
 *   int* rank_list = new int[num_comms];
 *   cnclComm_t* comms = new cnclComm_t[num_comms];
 *
 *   // set rank list and device list
 *   CNRT_CHECK(cnrtGetDeviceCount(&num_dev));
 *   for (int i = 0; i < num_comms; i++) {
 *     rank_list[i] = i;  // comm's rank
 *     dev_list[i] = rank_list[i] % num_dev;
 *   }
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   // free source
 *   delete[] comms;
 *   delete[] dev_list;
 *   delete[] rank_list;
 *
 *   cnrtDestroy();
 * @endcode
 */
cnclResult_t cnclInitComms(cnclComm_t* comms,
                           int num_comm,
                           const int* dev_list,
                           const int* rank_list,
                           int nrank,
                           const cnclCliqueId_t clique_id);

//Group:Communication Context Management
/**
 * @brief Acquires a clique identifier to initialize communicators.
 *
 * @details Each clique identifier acquired by this API is unique.
 *
 * @param [out] clique_id Pointer of identifier of clique.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Only communicators which are initialized with the same cnclCliqueId can
 *   communicate with each other.
 * - One ``clique_id`` can only be used for initialization once in each process.
 * - The ``clique_id`` should be distributed to all of the processes with
 *   communicators before calling ::cnclInitComms.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * @code
 *   ...
 *
 *   cnclCliqueId clique_id0;
 *   // get clique id 0
 *   CNCL_CHECK(cnclGetCliqueId(&clique_id0));
 *
 *   // initialize communicators of clique 0
 *   CNCL_CHECK(cnclInitComms(
 *       comms0, num_comms0, dev_list0, rank_list0, nrank0, clique_id0));
 *
 *   cnclCliqueId clique_id1;
 *   // get clique id 1
 *   CNCL_CHECK(cnclGetCliqueId(&clique_id1));
 *
 *   // initialize communicators of clique 1
 *   CNCL_CHECK(cnclInitComms(
 *       comms1, num_comms1, dev_list1, rank_list1, nrank1, clique_id1));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms1, num_comms1));
 *   CNCL_CHECK(cnclDestroyComms(comms0, num_comms0));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclGetCliqueId(cnclCliqueId_t clique_id);

//Group:Communication Context Management
/**
 * @brief Releases the resources of a set of communicators
 * after all running communication tasks are completed.
 *
 * @param [in] comms Array of communicators.
 * @param [in] num_comm Number of communicators in current process.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Calling ::cnclDestroyComms to release resources of communicators is equivalent
 *   to calling ::cnclFreeComm with communicators one by one.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * - See example in ::cnclInitComms.
 */
cnclResult_t cnclDestroyComms(cnclComm_t* comms, int num_comm);

//Group:Communication Context Management
/**
 * @brief Releases the resources of a communicator after all running communication
 * tasks of the communicator are completed.
 *
 * @param [in] comm The handler of the communicator.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Calling ::cnclDestroyComms to release resources of communicators is equivalent
 *   to calling ::cnclFreeComm with communicators one by one.
 * - Communicators in the clique must be released all at once, otherwise deadlock
 *   might occur.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   cnrtInit(0);
 *
 *   uint32_t num_dev = 0;
 *   int num_comms = 4;
 *   int nrank = num_comms;
 *
 *   // request source
 *   int* dev_list = new int[num_comms];
 *   int* rank_list = new int[num_comms];
 *   cnclComm_t* comms = new cnclComm_t[num_comms];
 *
 *   // set rank list and device list
 *   CNRT_CHECK(cnrtGetDeviceCount(&num_dev));
 *   for (int i = 0; i < num_comms; i++) {
 *     rank_list[i] = i;  // comm's rank
 *     dev_list[i] = rank_list[i] % num_dev;
 *   }
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // free communicators
 *   for (int i = 0; i < num_comms; i++) {
 *     CNCL_CHECK(cnclFreeComm(comms[i]));
 *   }
 *
 *   // free source
 *   delete[] comms;
 *   delete[] dev_list;
 *   delete[] rank_list;
 *
 *   cnrtDestroy();
 * @endcode
 */
cnclResult_t cnclFreeComm(cnclComm_t comm);

//Group:Communication Context Management
/**
 * @brief Suspends all of the running communication tasks of the communicator and
 * releases the resources of the communicator.
 *
 * @param [in] comm The handler of the communicator.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Communicators in the clique must be released all at once, otherwise
 *   deadlock might occur.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclAllReduce(send_data[i],
 *                                recv_data[i],
 *                                dnum,
 *                                cnclFloat,
 *                                cnclSum,
 *                                comms_[i],
 *                                mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // if communication error, abort communicators
 *   for (int i = 0; i < num_comms; i++) {
 *     if (cnclGetCommAsyncError(comms_[i]) == CNCL_RET_ASYNC_ERROR) {
 *       for (int j = 0; j < num_comms; j++) {
 *         CNCL_CHECK(cnclAbortComm(comms[i]));
 *       }
 *       break;
 *     }
 *   }
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclAbortComm(cnclComm_t comm);

//Group:Communication Context Management
/**
 * @brief Set communication configuration of the communicator in the clique.
 *
 * @param [inout] comm The handler of the communicator.
 * @param [in] config Configuration attributes to be set.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Configuration must keep consistent to all of the communicators in the same
 *   clique, otherwise error or unexpected performance might occur.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // config communication priority
 *   cnclCommConfig config = {};
 *   config.priority = CNCL_PRIORITY_HIGH;
 *   for (int i = 0; i < num_comms; i++) {
 *     CNCL_CHECK(cnclSetCommConfig(comms_[i], &config));
 *   }
 *
 *   // user programming code or communication operation code
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclAllReduce(send_data[i],
 *                                recv_data[i],
 *                                dnum,
 *                                cnclFloat,
 *                                cnclSum,
 *                                comms_[i],
 *                                mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclSetCommConfig(cnclComm_t comm, const cnclCommConfig_t config);

/***********************************************************************************
 * Communicators' information
 ***********************************************************************************/

//Group:Communicators' Information
/**
 * @brief Gets number of communicators in the clique.
 *
 * @param [out] count Number of communicators in the clique.
 * @param [in] comm The handler of the communicator.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - None.
 *
 * @par Requirements
 * - None.
 *
 *  * @par Example
 * @code
 *   ...
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   ...
 *
 *   int count = -1;
 *   int rank = -1;
 *   int dev_rank = -1;
 *   // get number of communicators, should be equal to `nrank`
 *   CNCL_CHECK(cnclGetCommCount(&count, comms[0]));
 *   // get rank of communicator 0, should be equal to `rank_list[0]`
 *   CNCL_CHECK(cnclGetCommRank(&rank, comms[0]));
 *   // get device ordinal of communicator 0, should be equal to `dev_list[0]`
 *   CNCL_CHECK(cnclGetCommDevice(&dev_rank, comms[0]));
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *   ...
 * @endcode
 */
cnclResult_t cnclGetCommCount(int* count, const cnclComm_t comm);

//Group:Communicators' Information
/**
 * @brief Gets the rank of the communicator in the clique.
 *
 * @param [out] rank The rank of the communicator.
 * @param [in] comm The handler of the communicator.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - None.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * - See example in ::cnclGetCommCount.
 */
cnclResult_t cnclGetCommRank(int* rank, const cnclComm_t comm);

//Group:Communicators' Information
/**
 * @brief Gets device ordinal which is associated to the communicator.
 *
 * @param [out] rank Device ordinal associated to the communicator.
 * @param [in] comm The handler of the communicator.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - None.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * - See example in ::cnclGetCommCount.
 */
cnclResult_t cnclGetCommDevice(int* rank, const cnclComm_t comm);

/***********************************************************************************
 * Collective Communication
 ***********************************************************************************/

//Group:Collective Communication
/**
 * @brief Copies the message data from ``root`` to all other communicators in the
 * clique.
 *
 * @param [in,out] buffer Starting address of buffer.
 * @param [in] count Number of data in buffer.
 * @param [in] data_type Type of data.
 * @param [in] root Data source of the communicator rank for broadcast.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - The argument ``root`` is the rank of the communicator (not the MLU ordinal)
 *   where data resides before the operation is started.
 * - This operation is implicitly InPlace.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // broadcast operation
 *   int root = 0;
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclBcast(data[i],
 *                            dnum,
 *                            cnclFloat,
 *                            root,
 *                            comms_[i],
 *                            mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclBcast(void* buffer,
                       size_t count,
                       cnclDataType_t data_type,
                       int root,
                       cnclComm_t comm,
                       cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief Copies the message data from ``root`` to all other communicators in the
 * clique.
 *
 * @param [in] send_buffer Starting address of send buffer.
 * @param [out] recv_buffer Starting address of receive buffer.
 * @param [in] count Number of data in buffer.
 * @param [in] data_type Type of data.
 * @param [in] root Data source of the communicator rank for broadcast.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - The argument ``root`` is the rank of the communicator (not the MLU ordinal)
 *   where data resides before the operation is started.
 * - InPlace operation will happen if ``send_buffer`` == ``recv_buffer``.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // broadcast operation
 *   int root = 0;
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclBroadcast(send_data[i],
 *                                recv_data[i],
 *                                dnum,
 *                                cnclFloat,
 *                                root,
 *                                comms_[i],
 *                                mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclBroadcast(const void* send_buffer,
                           void* recv_buffer,
                           size_t count,
                           cnclDataType_t data_type,
                           int root,
                           cnclComm_t comm,
                           cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief Collects the message data of each communicators in the clique, arranges the
 * message data according to the rank of the communicators.
 *
 * @param [in] send_buffer Starting address of send buffer.
 * @param [out] recv_buffer Starting address of receive buffer.
 * @param [in] count Number of data in send buffer.
 * @param [in] data_type Type of data.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - ``send_buffer`` must have a size of ``count`` data.
 * - ``recv_buffer`` must have a size of nrank * ``count`` data.
 * - InPlace operation will happen if ``send_buffer`` == ``recv_buffer`` +
 *   rank * ``count``.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // allgather operation
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclAllGather(send_data[i],
 *                                recv_data[i],
 *                                dnum,
 *                                cnclFloat,
 *                                comms_[i],
 *                                mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclAllGather(const void* send_buffer,
                           void* recv_buffer,
                           size_t count,
                           cnclDataType_t data_type,
                           cnclComm_t comm,
                           cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief Reduces the message data of all communicators in the clique on ``root``.
 *
 * @param [in] send_buffer Starting address of send buffer.
 * @param [out] recv_buffer Starting address of receive buffer.
 * @param [in] count Number of data in buffer.
 * @param [in] data_type Type of data.
 * @param [in] reduce_op Type of reduce operation.
 * @param [in] root Communicator rank of reduce root.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - ``recv_buffer`` may be ``nullptr`` on all calls except for root device.
 * - The argument ``root`` is the rank of the communicator (not the MLU ordinal)
 *   where data will reside after the operation is complete.
 * - InPlace operation will happen if ``send_buffer`` == ``recv_buffer``.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // reduce operation
 *   int root = 0;
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclReduce(send_data[i],
 *                             recv_data[i],
 *                             dnum,
 *                             cnclFloat,
 *                             cnclSum,
 *                             root,
 *                             comms_[i],
 *                             mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclReduce(const void* send_buffer,
                        void* recv_buffer,
                        size_t count,
                        cnclDataType_t data_type,
                        cnclReduceOp_t reduce_op,
                        int root,
                        cnclComm_t comm,
                        cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief Reduces the message data of all communicators in the clique on each
 * communicator.
 *
 * @param [in] send_buffer Starting address of send buffer.
 * @param [out] recv_buffer Starting address of receive buffer.
 * @param [in] count Number of data in buffer.
 * @param [in] data_type Type of data.
 * @param [in] reduce_op Type of reduce operation.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - InPlace operation will happen if ``send_buffer`` == ``recv_buffer``.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // allreduce operation
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclAllReduce(send_data[i],
 *                                recv_data[i],
 *                                dnum,
 *                                cnclFloat,
 *                                cnclSum,
 *                                comms_[i],
 *                                mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclAllReduce(const void* send_buffer,
                           void* recv_buffer,
                           size_t count,
                           cnclDataType_t data_type,
                           cnclReduceOp_t reduce_op,
                           cnclComm_t comm,
                           cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief Reduces the i-th block message data of all communicators in the clique on
 * the communicator with rank i.
 *
 * @param [in] send_buffer Starting address of send buffer.
 * @param [out] recv_buffer Starting address of receive buffer.
 * @param [in] count Number of data in recv buffer.
 * @param [in] data_type Type of data.
 * @param [in] reduce_op Type of reduce operation.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - ``send_buffer`` must have a size of nrank * ``count`` data.
 * - ``recv_buffer`` must have a size of ``count`` data.
 * - InPlace operation will happen if ``recv_buffer`` == ``send_buffer`` +
 *   rank * ``count``.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // reducescatter operation
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclReduceScatter(send_data[i],
 *                                    recv_data[i],
 *                                    dnum,
 *                                    cnclFloat,
 *                                    cnclSum,
 *                                    comms_[i],
 *                                    mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclReduceScatter(const void* send_buffer,
                               void* recv_buffer,
                               size_t count,
                               cnclDataType_t data_type,
                               cnclReduceOp_t reduce_op,
                               cnclComm_t comm,
                               cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief All to all primitive operation with variable data count.
 *
 * @details Copies i-th message data to the communicator with rank i in the clique,
 * and collects the message data of all communicators in the clique, arranges the
 * message data according to the rank of the communicator.
 *
 * @param [in] send_buffers Starting addresses of send data.
 * @param [in] send_counts Numbers of data in send_buffers to send.
 * @param [in] send_types Types of send data.
 * @param [out] recv_buffers Starting addresses of receive data.
 * @param [in] recv_counts Numbers of data to receive to recv_buffers.
 * @param [in] recv_types Type of recv data.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - Sends ``send_buffers[i]`` to the communicator with rank i, and receives data
 *   from the communicator with rank j into ``recv_buffers[j]``.
 * - ``send_buffers[i]`` must have a size of ``send_counts[i]`` data of
 *   ``send_types[i]``.
 * - ``recv_buffers[j]`` must have a size of ``recv_counts[j]`` data of
 *   ``recv_types[j]``.
 * - InPlace operation is not supported. Assume that ``send_buffers[i]`` !=
 *   ``recv_buffers[j]``. CNCL can keep running when InPlace, but the correctness of
 *   the results are not guaranteed.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // alltoallv operation
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclAlltoAllv(send_buffers[i],
 *                                send_counts[i],
 *                                send_types[i],
 *                                recv_buffers[i],
 *                                recv_counts[i],
 *                                recv_types[i],
 *                                comms_[i],
 *                                mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclAlltoAllv(const void** send_buffers,
                           uint32_t* send_counts,
                           cnclDataType_t* send_types,
                           void** recv_buffers,
                           uint32_t* recv_counts,
                           cnclDataType_t* recv_types,
                           cnclComm_t comm,
                           cnrtQueue_t queue);

//Group:Collective Communication
/**
 * @brief All to all primitive operation with constant data count.
 *
 * @details Copies i-th block of it's message data to the communicator with rank i in
 * the clique, and collects the message data of all communicators in the clique,
 * arranges the message data according to the rank of the communicator.
 *
 * @param [in] send_buffer Starting address of send buffer.
 * @param [out] recv_buffer Starting address of receive buffer.
 * @param [in] count Number of data to send or recv with other MLUs.
 * @param [in] data_type Types of data.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - ``send_buffer`` must have a size of nrank * ``count`` data.
 * - ``recv_buffer`` must have a size of nrank * ``count`` data.
 * - InPlace operation is not supported. Assume that ``send_buffer`` !=
 *   ``recv_buffer``. CNCL can keep running when InPlace, but the correct
 *   results are not guaranteed.
 *
 * @par Requirements
 * - cn_api.h, cnrt.h
 * - libcndrv.so, libcnrt.so
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // alltoall operation
 *   std::vector<std::thread> threads;
 *   for (int i = 0; i < num_comms; i++) {
 *     threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclAlltoAll(send_data[i],
 *                               recv_data[i],
 *                               dnum,
 *                               cnclFloat,
 *                               comms_[i],
 *                               mlu_queues[i]));
 *     }));
 *   }
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclAlltoAll(const void* send_buffer,
                          void* recv_buffer,
                          size_t count,
                          cnclDataType_t data_type,
                          cnclComm_t comm,
                          cnrtQueue_t queue);

/************************************************************************
 * Point-to-Point Communication
 ************************************************************************/

//Group:Point-to-Point Communication
/**
 * @brief Receives data from ``source`` to current communicator.
 *
 * @param [out] buffer Starting address of buffer.
 * @param [in] count Number of entries in buffer.
 * @param [in] data_type Data type of buffer.
 * @param [in] source Data source of the communicator rank for receive.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - The argument ``source`` is the rank of the communicator (not the MLU ordinal)
 *   where data receive from before the operation is started.
 *
 * @par Example
 * @code
 *   ...
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // send & recv operation
 *   std::vector<std::thread> threads;
 *   threads.push_back(std::thread([=]() {
 *     CNCL_CHECK(cnclSend(data[0],
 *                         dnum,
 *                         cnclFloat,
 *                         1,
 *                         comms_[0],
 *                         mlu_queues[0]));
 *   }));
 *   threads.push_back(std::thread([=]() {
 *       CNCL_CHECK(cnclRecv(data[1],
 *                           dnum,
 *                           cnclFloat,
 *                           0,
 *                           comms_[1],
 *                           mlu_queues[1]));
 *   }));
 *   for (int i = 0; i < num_comms; i++) {
 *     threads[i].join();
 *   }
 *
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclRecv(void* buffer,
                      const size_t count,
                      cnclDataType_t data_type,
                      const int source,
                      const cnclComm_t comm,
                      cnrtQueue_t queue);

//Group:Point-to-Point Communication
/**
 * @brief Sends data from current communicator to ``dest``.
 *
 * @param [in] buffer Starting address of buffer.
 * @param [in] count Number of entries in buffer.
 * @param [in] data_type Data type of buffer.
 * @param [in] dest Data destination of the communicator rank for send.
 * @param [in] comm The handler of the communicator.
 * @param [in] queue Runtime queue handler for asynchronous mode.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - The argument ``dest`` is the rank of the communicator (not the MLU ordinal)
 *   where data send to before the operation is started.
 *
 * @par Example
 * - See example in ::cnclRecv.
 */
cnclResult_t cnclSend(void* buffer,
                      const size_t count,
                      cnclDataType_t data_type,
                      const int dest,
                      const cnclComm_t comm,
                      cnrtQueue_t queue);

/***********************************************************************************
 * Error handling
 ***********************************************************************************/

//Group:Error Handling
/**
 * @brief Retrieves the error message of the error code of previous function.
 *
 * @param [in] ret The error code.
 *
 * @return
 * - The error message.
 *
 * @note
 * - None.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * @code
 *   ...
 *
 *   #define CNCL_CHECK(call)                                 \
 *     do {                                                   \
 *       cnclResult_t ret_code = (call);                      \
 *       if (ret_code != CNCL_RET_SUCCESS) {                  \
 *         printf("[%s:%d] CNCL error, msg: %s, code: %#x\n", \
 *                __FILE__,                                   \
 *                __LINE__,                                   \
 *                cnclGetErrorStr(ret_code),                  \
 *                ret_code);                                  \
 *         exit(1);                                           \
 *       }                                                    \
 *     } while (0);
 *
 *   // initialize communicators
 *   CNCL_CHECK(cnclInitComms(
 *       comms, num_comms, dev_list, rank_list, nrank, nullptr));
 *
 *   // user programming code or communication operation code
 *   ...
 *
 *   // destroy communicators
 *   CNCL_CHECK(cnclDestroyComms(comms, num_comms));
 *
 *   ...
 * @endcode
 */
const char* cnclGetErrorStr(cnclResult_t ret);

//Group:Error Handling
/**
 * @brief Gets the asynchronous errors encountered by the communciator.
 *
 * @param [in] comm The handler of the communicator.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ASYNC_IN_PROGRESS
 * - ::CNCL_RET_ASYNC_ERROR
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - None.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * - See example in ::cnclAbortComm.
 */
cnclResult_t cnclGetCommAsyncError(const cnclComm_t comm);

/*************************************************************************
 * Version control
 *************************************************************************/

//Group:Version Control
/**
 * @brief Gets the version of current library.
 *
 * @param[out] major Major version id.
 * @param[out] minor Minor version id, for API modifing and new features.
 * @param[out] patch Patch version id, for bug fixes.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_INVALID_VERSION
 *
 * @note
 * - None.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * @code
 *   ...
 *
 *   int major, minor, patch;
 *   // get version of CNCL
 *   CNCL_CHECK(cnclGetLibVersion(&major, &minor, &patch));
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclGetLibVersion(int* major, int* minor, int* patch);

/*************************************************************************
 * Group API
 *************************************************************************/

//Group:Group API
/**
 * @brief Starts a CNCL Group for a clique.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 *
 * @note
 * - cnclGroupStart() and cnclGroupEnd() must be used together.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * @code
 *   ...
 *   // using cnclGroup to merge several send/recv operations into one operation
 *   CNCL_CHECK(cnclGroupStart());
 *   for (int i = 0; i < num_comms_; i++) {
 *     int rank;
 *     cnclGetCommRank(&rank, comms_[i]);
 *     for (int j = 0; j < nrank_; j++) {
 *       if (rank == j) continue;
 *       CNCL_CHECK(cnclSend(send_data[i], dnum, dtype, j, comms_[i], mlu_queues[i]));
 *       CNCL_CHECK(cnclRecv(recv_data[i * nrank_ + j],
 *                           dnum,
 *                           dtype,
 *                           j,
 *                           comms_[i],
 *                           mlu_queues[i]));
 *     }
 *   }
 *   CNCL_CHECK(cnclGroupEnd());
 *
 *   ...
 * @endcode
 */
cnclResult_t cnclGroupStart();

//Group:Group API
/**
 * @brief Closes a CNCL Group for a clique.
 *
 * @return
 * - ::CNCL_RET_SUCCESS
 * - ::CNCL_RET_ERR_INTERNAL
 * - ::CNCL_RET_ERR_UNSUPPORTED
 * - ::CNCL_RET_ERR_NOT_INIT
 * - ::CNCL_RET_ERR_MLU_RUNTIME
 * - ::CNCL_RET_ERR_CNRT_MEMCOPY
 * - ::CNCL_RET_ERR_ARGUMENTS
 *
 * @note
 * - cnclGroupStart() and cnclGroupEnd() must be used together.
 *
 * @par Requirements
 * - None.
 *
 * @par Example
 * - See example in ::cnclGroupStart.
 */
cnclResult_t cnclGroupEnd();

#if defined(__cplusplus)
}
#endif  /*__cplusplus*/
#endif  // CNCL_H_ // NOLINT
