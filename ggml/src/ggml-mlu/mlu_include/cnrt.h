/*************************************************************************
 * Copyright (C) [2023] by Cambricon, Inc.
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
 *************************************************************************/

/************************************************************************
 *
 *  @file cnrt.h
 *
 *  @brief CNRT APIs provide programmable interfaces for users to develop
 *  their-owned programs, which include device management, Context
 *  management, Memory management of both sides (devices and hosts), etc.
 *
 **************************************************************************/

#ifndef __CNRT_H
#define __CNRT_H

/************************************************************************
 *  Include files
 ************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif /*__cplusplus*/

#define CNRT_MAJOR_VERSION 6
#define CNRT_MINOR_VERSION 14
#define CNRT_PATCH_VERSION 1

#ifndef CNRT_VERSION
#define CNRT_VERSION \
  (CNRT_MAJOR_VERSION * 10000 + CNRT_MINOR_VERSION * 100 + CNRT_PATCH_VERSION)
#endif

#if !defined(__CNRT_VERSION_INTERNAL) && !defined(__DOXYGEN_ONLY__)
#define cnrtGetDeviceProperties cnrtGetDeviceProperties_V3
#endif

/************************************************************************
 *  Definitions
 ************************************************************************/
/*!
 *  @brief Describes the return values of CNRT APIs.
 */
typedef enum {
  cnrtSuccess = 0,
  /*!< The API call returns with no errors. */
  cnrtErrorNotReady = 1,
  /*!< This indicates that the device or resource requested is busy now. */
  cnrtErrorInit = 100002,
  /*!< This indicates that initialization of CNRT fails. */
  cnrtErrorNoDevice = 100004,
  /*!< This indicates that no device is found. */
  cnrtErrorDeviceInvalid = 100005,
  /*!< This indicates that the device ordinal passed to the API is out of range [0, cnrtGetDeviceCount() - 1]. */
  cnrtErrorArgsInvalid = 100006,
  /*!< This indicates that one of the parameters passed to the API is invalid or out of range. */
  cnrtErrorSys = 100007,
  /*!< This indicates that it fails to call system API. */
  cnrtErrorSysNoMem = 100010,
  /*!< This indicates that there is insufficient host memory. */
  cnrtErrorInvalidResourceHandle = 100014,
  /*!< This indicates that resource handle passed to the API is invalid. */
  cnrtErrorIllegalState = 100015,
  /*!< This indicates that a resource required by the API is not in a valid state to
    perform the request operation. */
  cnrtErrorNotSupport = 100050,
  /*!< This indicates that the feature is not supported now. */
  cnrtErrorOpsNotPermitted = 100051,
  /*!< This indicates that the attempted operation is not permitted. */
  cnrtErrorQueue = 100060,
  /*!< This indicates that it fails to get the default queue. */
  cnrtErrorNoMem = 100100,
  /*!< This indicates that there is insufficient MLU device memory. */
  cnrtErrorAssert = 100128,
  /*!< This indicates that a device-side assert is triggered during kernel execution. */
  cnrtErrorKernelTrap = 100132,
  /*!< This indicates that a device-side code proceeds to a specific trap. */

  cnrtErrorKernelUserTrap = 100133,
  /*!< This indicates that a device-side code proceeds to a user specific trap. */

  cnrtErrorNotFound = 100301,
  /*!< This indicates that specific resources are not found. */

  cnrtErrorInvalidKernel = 100302,
  /*!< This indicates that the kernel handle is invalid. */
  cnrtErrorUnsupportedKernelVersion = 100303,
  /*!< This indicates that the kernel is compiled with an unsupported toolchain. */

  cnrtErrorNoKernel = 101312,
  /*!< This indicates that CNCC has not registered the kernel to CNRT. */
  cnrtErrorNoModule = 101315,
  /*!< This indicates that CNCC has not registered the module to CNRT or fails to load the module. */

  cnrtErrorQueueCaptureUnsupported = 100360,
  /*!< This indicates that the operation is not permitted when the queue is capturing. */
  cnrtErrorQueueCaptureInvalidated = 100361,
  /*!< This indicates that the current capture sequence on the queue has been invalidated
    due to previous error. */

  cnrtErrorQueueCaptureWrongThread = 100362,
  /*!< This indicates that the current capture sequence on the queue does not begin with
   ::cnrtQueueCaptureModeRelaxed mode, and ends in a different thread. */

  cnrtErrorQueueCaptureMerged = 100363,
  /*!< This indicates that two independent capture sequences are merged. */
  cnrtErrorQueueCaptureUnjoined = 100364,
  /*!< This indicates that the capture sequence contains at least a fork that is not joined
       to the primary queue. */
  cnrtErrorQueueCaptureIsolation = 100365,
  /*!< This indicates that a queue in capture sequence is trying to create a dependency
       which crosses the queue capture boundary. */
  cnrtErrorQueueCaptureUnmatched = 100366,
  /*!< This indicates that the queue in capture sequence is not the initially captured one. */
  cnrtErrorTaskTopoEntityUpdateFailure = 100400,
  /*!< This indicates that the TaskTopo update is not performed because it includes
    changes which violate constraints specific to TaskTopo entity update. */

  cnrtErrorSetOnActiveProcess = 632002,
  /*!< This indicates that it fails to set device flag because the process is still active. */
  cnrtErrorDevice = 632006,
  /*!< This indicates that current resource is not from the current device. */
  cnrtErrorNoAttribute = 632009,
  /*!< This indicates that the attribute queried does not exist. */
  cnrtErrorMemcpyDirectionInvalid = 632013,
  /*!< This indicates that memory copying direction passed to the API is not in ::cnrtMemTransDir_t. */
  cnrtErrorBusy = 632014,
  /*!< Deprecated. This indicates that the device or resource is busy.*/
  cnrtErrorCndrvFuncCall = 632015,
  /*!< This indicates that it fails to call CNDrv API. */
  cnrtErrorCndevFuncCall = 632017,
  /*!< This indicates that it fails to call CNDev API. */
  cnrtErrorNoCnrtContext = 632019,
  /*!< This indicates that no CNRT Context is found. */
  cnrtErrorCndrvFuncNotFound = 632020,
  /*!< This indicates that CNDrv API is not found. */

  cnrtErrorInvalidSymbol = 632100,
  /*!< This indicates that the symbol name/identifier passed to the API is invalid. */

  cnrtErrorUnknown = 999991,
  /*!< Unknown error. */
  cnrtErrorMax
  /*!< The last one. */
} cnrtRet_t;

/**< Execution modes of tasks on MLU device. */
/*! @brief
 *
 *  Describes the number of cores used
 *  for computation on the MLU devices.
 */
typedef enum {
  cnrtFuncTypeBlock = 1,
  /*!< One MLU core is used to execute tasks. */
  cnrtFuncTypeBlock0 = cnrtFuncTypeBlock,
  /*!< The IP core 0 is used to execute tasks. */
  cnrtFuncTypeBlock1 = cnrtFuncTypeBlock0 + 1,
  /*!< The IP heterogeneous core 1 is used to execute tasks. */
  cnrtFuncTypeUnion1 = 4,
  /*!< Four MLU cores are used to execute tasks. */
  cnrtFuncTypeUnion2 = 8,
  /*!< Eight MLU cores are used to execute tasks. */
  cnrtFuncTypeUnion4 = 16,
  /*!< 16 MLU cores are used to execute tasks. */
  cnrtFuncTypeUnion8 = 32,
  /*!< 32 MLU cores are used to execute tasks. */
  cnrtFuncTypeUnion16 = 64,
  /*!< 64 MLU cores are used to execute tasks. */
  cnrtFuncTypeMutable = -1,
  /*!< Not used now. */
  cnrtJobTypeBlock = cnrtFuncTypeBlock,
  /*!< One MLU core is used to execute tasks.
   *   This is only used for tensor dimension mutable function.*/
  cnrtJobTypeUnion1 = cnrtFuncTypeUnion1,
  /*!< Four MLU cores are used to execute tasks.
   *   This is only used for tensor dimension mutable function.*/
  cnrtJobTypeUnion2 = cnrtFuncTypeUnion2,
  /*!< Eight MLU cores are used to execute tasks.
   *   This is only used for tensor dimension mutable function.*/
  cnrtJobTypeUnion4 = cnrtFuncTypeUnion4,
  /*!< 16 MLU cores are used to execute tasks.
   *   This is only used for tensor dimension mutable function.*/
} cnrtFunctionType_t;

typedef cnrtFunctionType_t cnrtJobType_t;

/*!
 *  @brief
 *
 *  Describes the compute modes of an MLU device.
 */
typedef enum {
  cnrtComputeModeDefault = 0,
  /*!< Default compute mode: multiple threads can use ::cnrtSetDevice() with this device. */
  cnrtComputeModeExclusiveProcess = 1,
  /*!< Compute-exclusive-process mode: many threads in one process will be able to use ::cnrtSetDevice() with this device .*/
} cnrtComputeMode_t;

/*!
 *  @brief
 *
 *  Describes the attributes of the MLU device.
 */
typedef enum {
  cnrtAttrComputeCapabilityMajor = 0x01,
  /*!< Major compute capability of the MLU device. */
  cnrtAttrComputeCapabilityMinor = 0x02,
  /*!< Minor compute capability of the MLU device. */
  cnrtAttrSparseComputingSupported = 0x03,
  /*!< 1: The device supports sparse computing; 0: The device does not. */
  cnrtAttrFP16ComputingSupported = 0x04,
  /*!< 1: The device supports FP16; 0: The device does not. */
  cnrtAttrINT4ComputingSupported = 0x05,
  /*!< 1: The device supports INT4; 0: The device does not. */
  cnrtAttrINT8ComputingSupported = 0x06,
  /*!< 1: The device supports INT8; 0: The device does not. */
  cnrtAttrBF16ComputingSupported = 0x07,
  /*!< 1: The device supports BF16; 0: The device does not. */
  cnrtAttrTF32ComputingSupported = 0x08,
  /*!< 1: The device supports TF32; 0: The device does not. */
  cnrtAttrComputeMode = 0x09,
  /*!< The compute mode that the device is currently in. See ::cnrtComputeMode_t for details.*/
  cnrtAttrQueueSize = 0x101,
  /*!< The maximum number of queues. */
  cnrtAttrNotifierSize = 0x102,
  /*!< The maximum number of notifiers. */
  cnrtAttrSupportQueuePriorities = 0x103,
  /*!< 1: The device supports setting queue priorities; 0: The device does not. */
  cnrtAttrTinyCoreSupported = 0x104,
  /*!< 1: The device supports using tiny core to accelerate collective inter-device or intra-device communication; 0: The device does not. */
  cnrtAttrCodecJPEGSupported = 0x105,
  /*!< 1: The device supports hardware JPEG codec acceleration; 0: The device does not. */
  cnrtAttrCodecH264Supported = 0x106,
  /*!< 1: The device supports hardware video H.264 codec acceleration; 0: The device does not. */
  cnrtAttrCodecH265Supported = 0x107,
  /*!< 1: The device supports hardware video H.265 codec acceleration; 0: The device does not. */
  cnrtAttrAIIspCoreSupported = 0x108,
  /*!< 1: The device supports AI ISP core. 0: The device does not. */
  cnrtAttrMultiCtxNotifierWaitSupported = 0x109,
  /*!< The device supports wait notifier on another context's queue. */
  cnrtAttrIPCNotifierSupported = 0x10a,
  /*!< The device supports ipcnotifier functions via ::cnrtIpcGetNotifierHandle and ::cnrtIpcOpenNotifierHandle. **/
  cnrtAttrMaxDimX = 0x201,
  /*!< The maximum block dimension X. */
  cnrtAttrMaxDimY = 0x202,
  /*!< The maximum block dimension Y. */
  cnrtAttrMaxDimZ = 0x203,
  /*!< The maximum block dimension Z. */
  cnrtAttrMaxClusterCountPerUnionTask = 0x204,
  /*!< The maximum number of clusters per union task. */
  cnrtAttrClusterCount = 0x205,
  /*!< The maximum number of clusters of the MLU device. */
  cnrtAttrMcorePerCluster = 0x206,
  /*!< The maximum number of MLU cores of each cluster. */
  cnrtAttrMaxQuadrantCount = 0x207,
  /*!< The maximum count of quadrants per device. Intra-quadrant clusters have the best unified memory access performance. */
  cnrtAttrMaxUnionTypePerQuadrant = 0x208,
  /*!< The maximum union task types that can maintain unified intra-quadrant memory access. */
  cnrtAttrMaxClusterPerUnionLimitTask = 0x209,
  /*!< The maximum number of clusters per union limitation task. */
  cnrtAttrISAVersion = 0x20a,
  /*!< ISA version of current MLU device in the form of three-digit number. */
  cnrtAttrIsMultipleTensorProcessor = 0x20b,
  /*!< 1: The device adopts multi-tensor-processor architecture; 0: The device does not. */
  cnrtAttrMaxL2CacheSize = 0x301,
  /*!< The size of L2 cache in bytes. */
  cnrtAttrNramSizePerMcore =
      0x302, /*!< The maximum NRAM memory available of each MLU core in bytes. */
  cnrtAttrWramSizePerMcore = 0x303,
  /*!< The maximum WRAM memory available of each MLU core in bytes. */
  cnrtAttrTotalConstMemorySize = 0x304,
  /*!< The memory available on device for __mlu_const__ variable in a Cambricon BANG C kernel in MB. */
  cnrtAttrLmemSizePerMcore = 0x305,
  /*!< The maximum local memory available of each core in MB. */
  cnrtAttrSramSizePerMcore = 0x306,
  /*!< The maximum SRAM memory available of each cluster in bytes. */
  cnrtAttrGlobalMemoryNodeCount = 0x307,
  /*!< The number of NUMA nodes on device. */
  cnrtAttrMaxPersistingL2CacheSize = 0x309,
  /*!< The maximum L2 persisting cache size in bytes. */
  cnrtAttrAvailableGlobalMemorySize = 0x312,
  /*!< Available global memory size in MB.*/
  cnrtAttrEccEnable = 0x401,
  /*!< 1: The device supports ECC; 0: The device does not. */
  cnrtAttrIpuClockRate = 0x402,
  /*!< The cluster clock frequency in kilohertz. */
  cnrtAttrMemClockRate = 0x403,
  /*!< The memory clock frequency in kilohertz. */
  cnrtAttrGmemBusWidth = 0x404,
  /*!< The global memory bus width in bits. */
  cnrtAttrTotalMemSize = 0x405,
  /*!< The maximum available memory in megabytes. */
  cnrtAttrPciBusID = 0x406,
  /*!< The PCI bus identifier of the MLU device. */
  cnrtAttrPciDeviceID = 0x407,
  /*!< The PCI device identifier of the MLU device. */
  cnrtAttrPciDomainID = 0x408,
  /*!< The PCI domain ID of the MLU device. */
  cnrtAttrMDRMemorySize = 0x409,
  /*!< MDR memory size in megabytes. */

  /*!< Not supported yet. */
  cnrtAttrUnsupportedFlag = 0xffffff,
  /*!< Not supported flag. */
  cnrtAttrCanMapHostMemory,
  /*!< 1: The device supports mapping host memory to MLU; 0: The device does not. */
  cnrtAttrCanSetQueueSize,
  /*!< 1: The device supports setting the maximum queue size; 0: The device does not. */
  cnrtAttrCanSetNotifierSize,
  /*!< 1: The device supports setting the maximum notifier size; 0: The device does not. */
  cnrtAttrConcurrentKernels,
  /*!< 1: The device supports multiple kernels within the same Context simultaneously; 0: The device does not. */
  cnrtAttrSupportUnifiedAddr,
  /*!< 1: The device supports sharing a unified address space with the host; 0: The device does not. */
  cnrtAttrSupportManagedMem,
  /*!< 1: The device supports allocating managed memory; 0: The device does not. */
  cnrtAttrSupportNativeAtomic,
  /*!< 1: The link between the device and the host supports native atomic operations; 0: The link between the device and the host does not. */
  cnrtAttrSupportPageableMemAccess,
  /*!< 1: The device supports accessing pageable memory coherently; 0: The device does not. */
  cnrtAttrCanUseHostPointer,
  /*!< 1: The device can access host registered memory at the same virtual address as the CPU; 0: The device can not. */
  cnrtAttrSupportHostRegsiter,
  /*!< 1: The device supports host memory registration; 0: The device does not. */
  cnrtAttrCacheSize,
  /*!< Deprecated. The size of system cache in bytes.*/
  cnrtAttrMaxNum,
  /*!< The last one. */
} cnrtDeviceAttr_t;

/*!
 *  @brief
 *
 *  Describes the P2P attributes of the MLU device.
 */
typedef enum {
  cnrtDevP2PAttrAccessSupported = 0,
  /*!< P2P access is enabled. */
  cnrtDevP2PAttrNativeAtomicSupported,
  /*!< Native atomic operation between the device and host is supported. */
  cnrtDevP2PAttrMaxNum,
  /*!< The last one. */
} cnrtDeviceP2PAttr_t;

/*!
 *  @brief Deprecated. Describes the limits of the MLU device.
 */
typedef enum {
  cnrtDevLimitStackSize = 0,
  /*!< MLU stack size can be used for each MLU core. */
  cnrtDevLimitPrintfFifoSize,
  /*!< MLU print line FIFO size. */
  cnrtDevLimitMaxNum,
  /*!< The last one. */
} cnrtDeviceLimit_t;

/*!
 * @brief Describes the device configurations.
 */
typedef enum {
  cnrtDeviceConfigReserved = 0,
  /*!< Reserved. */
  cnrtDeviceConfigPrintfFifoNum = 1,
  /*!< The record number of print line FIFO per MLU core. */
  cnrtDeviceConfigMaxPersistingL2CacheSize = 2,
  /*!< The maximum L2 persisting cache size in bytes. */
  cnrtDeviceConfigPrintfFifoSize = 3,
  /*!< The record number of print line FIFO size. */
  cnrtDeviceConfigMaxNum,
  /*!< The maximum number of device configuration enums. */
} cnrtDeviceConfig_t;

/*!
 *  @brief
 *
 *  Describes the device flags used for the
 *  current process execution on the current device.
 *
 *  The ::cnrtGetDeviceFlag API is used to retrieve the flags set.
 *
 */
typedef enum {
  cnrtDeviceScheduleSpin = 0,
  /*!< CPU actively spins when waiting for the device execution result. */
  cnrtDeviceScheduleBlock = 1,
  /*!< CPU thread is blocked on a synchronization primitive when
   *   waiting for the device execution results. */
  cnrtDeviceScheduleYield = 2,
  /*!< CPU thread yields when waiting for the device execution results. */
  cnrtDeviceScheduleAuto = 3,
  /*!< Automatic scheduling. */
  cnrtDeviceFlagsMaxNum
  /*!< The last one. */
} cnrtDeviceFlags_t;

/*! @brief
 *
 * Describes the direction of data copying.
 */
typedef enum {
  cnrtMemcpyHostToDev = 0,
  /*!< Host to device. */
  cnrtMemcpyDevToDev,
  /*!< Data copy in a single device. */
  cnrtMemcpyDevToHost,
  /*!< Device to host. */
  cnrtMemcpyHostToHost,
  /*!< Host to host. */
  cnrtMemcpyPeerToPeer,
  /*!< P2P in two different devices. */
  cnrtMemcpyNoDirection,
  /*!< Data copying without a specified direction. */
} cnrtMemTransDir_t;

/*! @brief
 *
 * Describes the range attributes.
 * Not used now.
 */
typedef enum {
  cnrtMemRangeAttributePreferredLocation = 0,
  /*!< The preferred location of the range. */
  cnrtMemRangeAttributeAccessedBy,
  /*!< The memory range set for the specified device. */
} cnrtMemRangeAttribute_t;

/*! @brief
 *
 * Describes memory types.
 */
typedef enum {
  cnrtMemTypeUnregistered = 0,
  /*!< Unregistered memory. */
  cnrtMemTypeHost,
  /*!< Host memory. */
  cnrtMemTypeDevice,
  /*!< Device memory. */
} cnrtMemType_t;

/*! @brief
 *
 * Describes UVA cache modes.
 */
typedef enum {
  cnrtUvaNotSupport = -1,
  /*!< The current platform or driver version does not support this enum. */
  cnrtUvaUnknown = 0,
  /*!< Unregistered cache mode. */
  cnrtUvaUnCached,
  /*!< Non-cacheable UVA. */
  cnrtUvaCached,
  /*!< Cacheable UVA. */
} cnrtUvaCacheMode_t;

/*! @brief
 *
 * Describes properties of allocated memory.
 * Not used now.
 */
typedef enum {
  cnrtHostAllocDefault = 0,
  /*!< Default host allocation type which is equal to host memory allocated by ::cnrtHostMalloc. */
  cnrtHostAllocMapped,
  /*!< Allocated host memory that is mapped to an MLU device.  */
} cnrtHostAllocFlags_t;

/*! @brief
 *
 * Describes the pointer attributes.
 */
typedef struct {
  cnrtMemType_t type;
  /*!< The memory type. */
  int device;
  /*!< Device ordinal which the pointer is allocated from. */
  size_t size;
  /*!< Size in bytes of the pointer from ::cnrtMalloc. */
  void *devicePointer;
  /*!< Device pointer related to the pointer. */
  void *hostPointer;
  /*!< Host pointer related to the pointer. */
  cnrtUvaCacheMode_t cacheMode;
  /*!< Cache mode of the host pointer. */
  void *deviceBasePointer;
  /*!< The base address of the device pointer. */
} cnrtPointerAttributes_t;

struct cnrtUUID_st {
  char uuid[16];
};

/*! @brief
 *
 *  UUID(Universally Unique Identifier) of the device.
 *
 */
typedef struct cnrtUUID_st cnrtUUID_t;

/*! @brief
 *
 * Describes the properties of the MLU device.
 */
typedef struct cnrtDeviceProp_V3 {
  char name[256];
  /*!< MLU device name. */
  int totalMem;
  /*!< Total memory available on device in MB. */
  int maxDim[3];
  /*!< The maximum size of each dimension of a block. */
  int ipuClockRate;
  /*!< Cluster clock frequency in kilohertz. */
  int memClockRate;
  /*!< Memory clock frequency in kilohertz. */
  int totalConstMem;
  /*!< Memory available on device for __mlu_const__ variables in MB. */
  int major;
  /*!< Major compute capability of the MLU device. */
  int minor;
  /*!< Minor compute capability of the MLU device. */
  int ECCEnabled;
  /*!< The device has ECC support enabled.  */
  int pciBusID;
  /*!< PCI bus identifier of the MLU device. */
  int pciDeviceID;
  /*!< PCI device identifier of the MLU device. */
  int pciDomainID;
  /*!< PCI domain ID of the MLU device. */
  int maxL2CacheSize;
  /*!< The size of L2 cache in bytes. */
  int persistingL2CacheMaxSize;
  /*!< The maximum L2 persisting cache size in bytes. */
  int queuePrioritiesSupported;
  /*!< 1: The device supports setting queue priorities; 0: The device does not. */
  int sparseComputingSupported;
  /*!< 1: The device supports sparse computing; 0: The device does not. */
  int FP16ComputingSupported;
  /*!< 1: The device supports FP16; 0: The device does not. */
  int INT4ComputingSupported;
  /*!< 1: The device supports INT4; 0: The device does not. */
  int INT8ComputingSupported;
  /*!< 1: The device supports INT8; 0: The device does not. */
  int BF16ComputingSupported;
  /*!< 1: The device supports BF16; 0: The device does not. */
  int TF32ComputingSupported;
  /*!< 1: The device supports TF32; 0: The device does not. */
  int maxQueueSize;
  /*!< The maximum number of queues. */
  int maxNotifierSize;
  /*!< The maximum number of Notifiers.*/
  int tinyCoreSupported;
  /*!< 1: The device supports using tiny core to accelerate collective inter-device or intra-device communication; 0: The device does not. */
  int codecJPEGSupported;
  /*!< 1: The device supports hardware JPEG codec acceleration; 0: The device does not. */
  int codecH264Supported;
  /*!< 1: The device supports hardware video H.264 codec acceleration; 0: The device does not. */
  int codecH265Supported;
  /*!< 1: The device supports hardware video H.265 codec acceleration; 0: The device does not. */
  int maxClusterCountPerUnionTask;
  /*!< The maximum number of clusters per union task.*/
  int clusterCount;
  /*!< The maximum number of clusters of the MLU device.*/
  int McorePerCluster;
  /*!< The maximum number of MLU Cores of each cluster.*/
  int maxQuadrantCount;
  /*!< The maximum count of quadrants per device. Intra-quadrant clusters have the best unified memory access performance. */
  int maxUnionTypePerQuadrant;
  /*!< The maximum union task types that can maintain unified intra-quadrant memory access. */
  int maxClusterPerUnionLimitTask;
  /*!< The maximum number of clusters per union limitation task. */
  int ISAVersion;
  /*!< ISA version of current MLU device in the form of three-digit number.*/
  int isMultipleTensorProcessor;
  /*!< 1: The device adopts multi-tensor-processor architecture; 0: The device does not. */
  int NramSizePerMcore;
  /*!< The maximum nram memory available of each MLU Core in bytes.*/
  int WramSizePerMcore;
  /*!< The maximum wram memory available of each MLU Core in bytes.*/
  int LmemSizePerMcore;
  /*!< The maximum local memory available of each Core in MB.*/
  int SramSizePerMcore;
  /*!< The maximum sram memory available of each MLU Core in bytes.*/
  int globalMemoryNodeCount;
  /*!< The number of NUMA nodes on device.*/
  int cacheSize;
  /*!< The size of system cache in bytes.*/
  int GmemBusWidth;
  /*!< Global memory bus width in bits.*/
  int computeMode;
  /*!< The compute mode that the device is currently in. See ::cnrtComputeMode_t for details.*/
  int MDRMemorySize;
  /*!< MDR memory size in megabytes.*/
  int availableGlobalMemorySize;
  /*!< Total available global memory size in MB.*/
  cnrtUUID_t uuid;
  /*!< Universally Unique Identifier of the device. */
  uint64_t totalGlobalMemSize;
  /*!< Total global memory size in bytes. */
  int reserved[30];
  /*!< Reserved. */
} cnrtDeviceProp_t;

/*!< Parameter for API call. */
/*!
 *
 * @brief
 *
 * Describes grid dimensions used for task execution.
 *
 */
typedef struct {
  unsigned int x;
  /*!< The X axis. The value of X equals to: the number of tasks
   *   to run on each core multiplies 4.
   */
  unsigned int y;
  /*!< The Y axis. Each task is to run ``y*z`` times.*/
  unsigned int z;
  /*!< The Z axis. Each task is to run ``y*z`` times.*/
} cnrtDim3_t;

/*!
 * @brief The offset of the address.
 */
typedef struct {
  size_t x;
  /*!< The offset in the X direction.*/
  size_t y;
  /*!< The offset in the Y direction.*/
  size_t z;
  /*!< The offset in the Z direction.*/
} cnrtPos_t;

/*!
 * @brief The pitch (alignment) of the address. None of the parameters can be 0.
 */
typedef struct {
  size_t pitch;
  /*!< The pitch of the memory. It cannot be less than the p->extent.width.*/
  void *ptr;
  /*!< The pointer of the memory. The same as the p->dst.*/
  size_t xsize;
  /*!< The memory X size (reserved). It is set to p->extent.width. */
  size_t ysize;
  /*!< The memory Y size. It cannot be less than the p->extent.height.*/
} cnrtPitchedPtr_t;

/*!
 * @brief The extent (size) of the address. None of the parameters can be 0.
 */
typedef struct {
  size_t depth;
  /*!< The depth of the memory.*/
  size_t height;
  /*!< The height of the memory.*/
  size_t width;
  /*!< The width of the memory.*/
} cnrtExtent_t;

/*!
 * @brief The configuration parameters of 3D memory copy.
 */
typedef struct cnrtMemcpy3dParam_st {
  void *dst;
  /*!< The destination address.*/
  cnrtPos_t dstPos;
  /*!< The destination address position.*/
  cnrtPitchedPtr_t dstPtr;
  /*!< The pitch of the destination address.*/
  cnrtExtent_t extent;
  /*!< The extent of the memory.*/
  cnrtMemTransDir_t dir;
  /*!< Data copy direction.*/
  void *src;
  /*!< The source address.*/
  cnrtPos_t srcPos;
  /*!< The source address position.*/
  cnrtPitchedPtr_t srcPtr;
  /*!< The pitch of the source address.*/
} cnrtMemcpy3dParam_t;

/*! @brief
 *
 * Describes the data types supported by CNRT.
 */
typedef enum cnrtDataType {
  cnrtInvalid = 0x0,
  /*!< Invalid data. */
  cnrtFloat16 = 0x12,
  /*!< 16-bit floating-point data. */
  cnrtFloat32 = 0x13,
  /*!< 32-bit floating-point data. */
  cnrtFloat64 = 0x14,
  /*!< 64-bit floating-point data. */
  cnrtInt4 = 0x20,
  /*!< Not supported yet. */
  cnrtInt8 = 0x21,
  /*!< 8-bit integer. */
  cnrtInt16 = 0x22,
  /*!< 16-bit integer. */
  cnrtInt32 = 0x23,
  /*!< 32-bit integer. */
  cnrtInt64 = 0x24,
  /*!< 64-bit integer. */
  cnrtAuto = 0x25,
  /*!< Automatic bit-width integer. It changes among int8, int16, etc. */
  cnrtUInt8 = 0x31,
  /*!< 8-bit unsigned integer. */
  cnrtUInt16 = 0x32,
  /*!< 16-bit unsigned integer. */
  cnrtUInt32 = 0x33,
  /*!< 32-bit unsigned integer. */
  cnrtFix8 = 0x41,
  /*!< 8-bit fixed-point data. */
  cnrtQuant8 = 0x51,
  /*!< 8-bit data. */
  cnrtBool = 0x61,
  /*!< Boolean type. */
} cnrtDataType_t;

/*! @brief
 *
 * Describes the data types supported by CNRT.
 */
typedef enum cnrtDataType_V2 {
  cnrtUnknown = 0,
  /*!< Invalid data. */
  cnrtDouble = 0x1,
  /*!< 64-bit floating-point data. */
  cnrtFloat = 0x2,
  /*!< 32-bit floating-point data. */
  cnrtHalf = 0x3,
  /*!< 16-bit floating-point data. */
  cnrtBfloat = 0x4,
  /*!< BF16 data type. */
  cnrtUlonglong = 0x11,
  /*!< 64-bit unsigned integer. */
  cnrtUint = 0x12,
  /*!< 32-bit unsigned integer. */
  cnrtUshort = 0x13,
  /*!< 16-bit unsigned integer. */
  cnrtUchar = 0x14,
  /*!< 8-bit unsigned integer. */
  cnrtLonglong = 0x21,
  /*!< 64-bit integer. */
  cnrtInt = 0x22,
  /*!< 32-bit integer. */
  cnrtShort = 0x23,
  /*!< 16-bit integer. */
  cnrtChar = 0x24,
  /*!< 8-bit integer. */
  cnrtBoolean = 0x31,
  /*!< Boolean type. */
} cnrtDataType_V2_t;

/*! @brief
 *
 * Describes the rounding mode supported by CNRT.
 */
typedef enum cnrtRoundingMode {
  cnrtRounding_rn = 0,
  /*!< Converts an input number in round-to-nearest-even mode.*/
  cnrtRounding_rz = 1,
  /*!< Converts an input number in round-to-zero mode. */
  cnrtRounding_rd = 2,
  /*!< Converts an input number in round-down mode. */
  cnrtRounding_ru = 3,
  /*!< Converts an input number in round-up mode. */
  cnrtRounding_ro = 4,
  /*!< Converts an input number in round-off-zero mode. */
  cnrtRounding_rm = 5,
  /*!< Converts an input number in round-to-math mode. */
  cnrtRounding_max
  /*!< The last one. */
} cnrtRoundingMode_t;

/*!
 *
 *  @brief
 *
 *  A semi-internal struct describing the parameters to be quantized. */
struct cnrtQuantizedParam;

/*! A pointer to the struct of the parameters that are quantized. */
typedef struct cnrtQuantizedParam *cnrtQuantizedParam_t;

/*! The IPC handle size. */
#define CNRT_IPC_HANDLE_SIZE       8
#define CNRT_IPC_HANDLE_SIZE_BYTES (CNRT_IPC_HANDLE_SIZE * sizeof(uint64_t))

/*! The IPC notifier handle. */
typedef struct cnrtIpcNotifierHandle_st {
  uint64_t res[CNRT_IPC_HANDLE_SIZE];
  /*!< Reserved for IPC notifier handle. */
} cnrtIpcNotifierHandle_v1;
typedef cnrtIpcNotifierHandle_v1 cnrtIpcNotifierHandle;

/*! The IPC memory handle. Pointer to void by default. */
typedef void *cnrtIpcMemHandle;

/*!
 *  @struct CNqueue_st
 *  @brief
 *
 *  Describes queue types.
 *
 */
struct cnrtQueue;

#define cnrtQueue CNqueue_st
/*! @brief
 *
 *  A pointer to the ::cnrtQueue struct holding the information about a queue.
 *
 *  The ::cnrtQueueCreate and ::cnrtQueueDestroy APIs are used to create
 *  and destroy an instance of cnrtQueue_t respectively.
 *
 * @note
 * - This struct CNqueue_st is the same as struct ::cnrtQueue, which is defined in
 *   the versions before v5.1.1. After version v5.1.2, CNqueue_st is used to avoid
 *   compile warning when called mixedly with CNDrv.
 * - A definition from ::cnrtQueue to CNqueue_st is used to keep compatibility.
 *
 */
typedef struct cnrtQueue *cnrtQueue_t;

/*! @brief
 *
 *  Per-thread queue handle.
 *  Queue handle that can be passed as a ::cnrtQueue_t to use an implicit queue 
 *  with per-thread synchronization behavior.
 */
#define cnrtQueuePerThread ((cnrtQueue_t)1)

/*! @brief
 *
 *  Legacy queue handle.
 *  Queue handle that can be passed as a ::cnrtQueue_t to use an implicit queue 
 *  with per-process synchronization behavior.
 */
#define cnrtQueueLegacy ((cnrtQueue_t)2)

/*!
 *  @struct CNnotifier_st
 *  @brief
 *
 *  Describes notifier types.
 *
 */
struct cnrtNotifier;

#define cnrtNotifier CNnotifier_st
/*! A pointer to the ::cnrtNotifier struct holding the information
 *  about a notifier.
 *
 *  The ::cnrtNotifierCreate and ::cnrtNotifierDestroy APIs are
 *  used to create and destroy an instance of cnrtNotifier_t respectively.
 *
 * @note
 * - This struct CNnotifier_st is the same as struct ::cnrtNotifier, which is defined in
 *   the versions before v5.1.1. After version v5.1.2, CNnotifier_st is used to avoid
 *   compiling warning when called mixedly with CNDrv.
 * - A definition from ::cnrtNotifier to CNnotifier_st is used to keep compatibility.
 *
 */
typedef struct cnrtNotifier *cnrtNotifier_t;

/*! @brief
 *
 * Describes the flags of notifier, which are used by ::cnrtNotifierCreateWithFlags.
 */
typedef enum cnrtNotifierFlags {
  CNRT_NOTIFIER_DEFAULT = 0x0, /*!< The default notifier creation flag. */
  CNRT_NOTIFIER_DISABLE_TIMING_SW =
      0x2, /*!< The notifier will not record sw timestamp data. */
  CNRT_NOTIFIER_DISABLE_TIMING =
      CNRT_NOTIFIER_DISABLE_TIMING_SW, /*!< Deprecated. */
  CNRT_NOTIFIER_DISABLE_TIMING_ALL =
      0x4, /*!< The notifier will not record timestamp data to reduce overhead. */
  CNRT_NOTIFIER_INTERPROCESS =
      0x8, /*!< The notifier is suitable for interprocess use. CNRT_NOTIFIER_DISABLE_TIMING_ALL must be set.*/
} cnrtNotifierFlags_t;

/**
 * @brief
 *
 * The flags of notifier place, which are used by ::cnrtPlaceNotifierWithFlags.
 */
typedef enum cnrtNotifierPlaceFlags {
  CNRT_NOTIFIER_PLACE_DEFAULT = 0x0, /**< Default notifier place flag. */
  CNRT_NOTIFIER_PLACE_EXTERNAL,      /**< Creates an notifier place node instead
                                        of the default behavior of ::cnrtPlaceNotifierWithFlags when using
                                        queue capture. This flag is invalid when used outside
                                        of queue capture. */
} cnrtNotifierPlaceFlags_t;

/**
 * @brief
 *
 * The flags of notifier wait, which are used by ::cnrtQueueWaitNotifier.
 */
typedef enum cnrtNotifierWaitFlags {
  CNRT_NOTIFIER_WAIT_DEFAULT = 0x0, /**< Default notifier wait flag. */
  CNRT_NOTIFIER_WAIT_EXTERNAL,      /**< Creates an notifier wait node instead
                                        of the default behavior of ::cnrtQueueWaitNotifier when using
                                        queue capture. This flag is invalid when used outside
                                        of queue capture. */
} cnrtNotifierWaitFlags_t;

/*! @brief
 *
 * Describes the cache operation types.
 */
typedef enum {
  CNRT_FLUSH_CACHE = 1,
  /*!< Flushes dcache of the host CPU. */
  CNRT_INVALID_CACHE = 2,
  /*!< Invalidates dcache of the host CPU, which is currently reserved. */
} cnrtCacheOps_t;

/*!
 * @brief Specifies performance hint for hitProp and missProp with ::cnrtAccessPolicyWindow.
 */
typedef enum cnrtAccessProperty {
  cnrtAccessPolicyNormal = 0,
  /*!< Normal cache persistance. */
  cnrtAccessPolicyStreaming = 1,
  /*!< Streaming access is likely to persist in cache. */
  cnrtAccessPolicyPersisting = 2,
  /*!< Persisting access is more likely to persist in cache. */
} cnrtAccessProperty_t;

/*
 * ! @brief Specifies an access policy for a window.
 *
 * A contiguous extent of memory beginning at ``baseAddr`` and ending at ``baseAddr`` + ``numBytes``.
 * The regions and ration specifications are fitted to the capabilities of the architecture.
 */
typedef struct cnrtAccessPolicyWindow {
  void *baseAddr;
  /*!< Starting address of access policy window. */
  size_t numBytes;
  /*!< Size in bytes of access policy window. MLU driver may restrict the maximum size and alignment. */
  float hitRatio;
  /*!< hitRatio specifies percentage of cache lines assigned hitProp, the rest are assigned missProp.
       Valid range is [0, 1.0]. */
  enum cnrtAccessProperty hitProp;
  /*!< The access property set for cache hit in ::cnrtAccessProperty. */
  enum cnrtAccessProperty missProp;
  /*!< The access property set for cache miss in ::cnrtAccessProperty.
       It must be either ::cnrtAccessPolicyNormal or ::cnrtAccessPolicyStreaming. */
} cnrtAccessPolicyWindow_t;

/*! @brief
 *
 * Describes invoke attributes.
 */
typedef enum cnrtInvokeAttrID {
  cnrtInvokeAttributeEmpty = 0,
  /*!< Empty attribute entry. */
  cnrtInvokeAttributeAccessPolicyWindow = 1,
  /*!< Valid for queues and task topo nodes. Attribute ID used to change and query ::cnrtAccessPolicyWindow. */
} cnrtInvokeAttrID_t;

/*! @brief
 *
 * Describes the invoke attribute union.
 */
typedef union cnrtInvokeAttrValue {
  char pad[64];
  /*!< The padding. */
  cnrtAccessPolicyWindow_t accessPolicyWindow;
  /*!< Invoke attribute value for ::cnrtInvokeAttributeAccessPolicyWindow. */
} cnrtInvokeAttrValue_t;

/*! @brief
 *
 * Describes the queue attributes.
 */
typedef cnrtInvokeAttrID_t cnrtQueueAttrID_t;
#define cnrtQueueAttributeAccessPolicyWindow \
  cnrtInvokeAttributeAccessPolicyWindow

/*! @brief
 *
 * Describes the queue attribute union.
 */
typedef cnrtInvokeAttrValue_t cnrtQueueAttrValue_t;

/*! @brief
 *
 * Describes the task topo kernel node attributes.
 */
typedef cnrtInvokeAttrID_t cnrtKernelNodeAttr_t;
#define cnrtKernelNodeAttributeAccessPolicyWindow \
  cnrtInvokeAttributeAccessPolicyWindow

/*! @brief
 *
 * Describes the task topo kernel node attribute union.
 */
typedef cnrtInvokeAttrValue_t cnrtKernelNodeAttrValue_t;

/*!
 * @brief Memory Pool.
 */
typedef struct CNmemoryPool_st *cnrtMemPool_t;

/*!
 * @brief The memory available allocation types.
 */
typedef enum cnrtMemAllocationType {
  cnrtMemAllocationTypeInvalid =
      0x0, /*!< The invalid memory allocation type. */
  cnrtMemAllocationTypeDefault =
      0x1, /*!< The default memory allocation type. */
} cnrtMemAllocationType_t;

/*!
 *  @brief The requested handle types.
 */
typedef enum cnrtMemAllocationHandleType {
  cnrtMemHandleTypeNone = 0x0,
  /*!< The handle type that does not support memory exporting. */
  cnrtMemHandleTypePosixFileDescriptor = 0x1,
  /*!< The handle type that supports memory exporting to a POSIX file descriptor, which is only
        available for POSIX systems. */
} cnrtMemAllocationHandleType_t;

/*!
 * @brief The memory location types.
 */
typedef enum cnrtMemLocationType {
  cnrtMemLocationTypeInvalid = 0x0, /*!< The invalid memory location. */
  cnrtMemLocationTypeDevice = 0x1,  /*!< The MLU device location. */
} cnrtMemLocationType_t;

/*!
 *  @brief The memory location.
 */
typedef struct cnrtMemLocation {
  cnrtMemLocationType_t type;
  /*!< The location types which define the ID of MLU device ordinal. */
  uint32_t id;
  /*!< The ID of a given location type. If the type is
      ::cnrtMemLocationTypeDevice, the ID means the device ordinal. */
} cnrtMemLocation_t;

/*!
 *  @brief The properties of allocations made from the pool.
 */
typedef struct cnrtMemPoolProps {
  cnrtMemAllocationType_t type;
  /*!< The memory allocation type requested, which must be ::cnrtMemAllocationTypeDefault currently. */
  cnrtMemAllocationHandleType_t requestedHandleTypes;
  /*!< The requested handle types. */
  cnrtMemLocation_t location;
  /*!< The location of memory allocation. */
  size_t maxSize;
  /*!< Maximum pool size. When set to 0, defaults to a system-dependent value. */
  unsigned char reserved[64];
  /*!< Reserved for future use, must be 0. */
} cnrtMemPoolProps_t;

/*!
 * @brief Memory access flag.
 */
typedef enum cnrtMemoryAccessFlags {
  cnrtMemAccessFlagsProtReadWrite = 0x0,
  /*!< Makes the virtual address range readable and writable. */
  cnrtMemAccessFlagsProtReadOnly = 0x1,
  /*!< Makes the virtual address range readable. */
} cnrtMemoryAccessFlags_t;

/**
 *  @brief Memory pool attributes.
 */
typedef enum cnrtMemPoolAttr {
  /**
     * The value type is integer.
     * Allow ::cnrtMemAllocAsync to use memory that is asynchronously freed
     * in another queues as long as there is a queue ordering dependency
     * of the allocating queue on the free action.
     * Notifier interactions can create the required
     * queue ordered dependencies. Enabled by default.
     */
  cnrtMemPoolAttrReuseFollowNotifierDependencies = 0x1,
  /**
     * (value type = int)
     * Allow reuse of already completed frees when there is no dependency
     * between the free and allocation. (default enabled)
     */
  cnrtMemPoolAttrReuseAllowOpportunistic,
  /**
     * (value type = int)
     * Allow ::cnrtMemAllocAsync to insert new queue dependencies
     * in order to establish the queue ordering required to reuse
     * a piece of memory released by ::cnrtMemFreeAsync (default enabled).
     */
  cnrtMemPoolAttrReuseAllowInternalDependencies,
  /**
     * (value type = cn_int64_t)
     * Amount of reserved memory in bytes to hold onto before trying
     * to release memory back to the OS. When more than the release
     * threshold bytes of memory are held by the memory pool, the
     * allocator will try to release memory back to the OS on the
     * next call to queue, notifier or context synchronize. The default value is 0.
     */
  cnrtMemPoolAttrReleaseThreshold,
  /**
     * (value type = cn_int64_t)
     * Amount of backing memory currently allocated for the memory pool.
     */
  cnrtMemPoolAttrReservedMemCurrent,
  /**
     * (value type = cn_int64_t)
     * High watermark of backing memory allocated for the memory pool since the
     * last time it was reset. High watermark can only be reset to zero.
     */
  cnMemPoolAttrReservedMemHigh,
  /**
     * (value type = cn_int64_t)
     * Amount of memory from the pool that is currently in use by the application.
     */
  cnrtMemPoolAttrUsedMemCurrent,
  /**
     * (value type = cn_int64_t)
     * High watermark of the amount of memory from the pool that was in use by the application since
     * the last time it was reset. High watermark can only be reset to zero.
     */
  cnrtMemPoolAttrUsedMemHigh,
} cnrtMemPoolAttr_t;

/*! @brief
 *
 *  Describes a pointer to the ::cnrtTaskTopo_t struct holding the information about a
 *  TaskTopo.
 *
 */
typedef struct CNtaskTopo_st *cnrtTaskTopo_t;

/*! @brief
 *
 *  A pointer to the ::cnrtTaskTopoNode_t struct holding the information about a
 *  node of a TaskTopo.
 *
 */
typedef struct CNtaskTopoNode_st *cnrtTaskTopoNode_t;

/*!
 * @brief Queue capture statuses.
 */
typedef enum cnrtQueueCaptureStatus {
  cnrtQueueCaptureStatusNone = 0,   /*!< A queue is not capturing. */
  cnrtQueueCaptureStatusActive = 1, /*!< A queue is actively capturing. */
  cnrtQueueCaptureStatusInvalidated =
      2, /*!< A queue is partly capturing sequence
                                              that has been invalidated, but not terminated. */
} cnrtQueueCaptureStatus_t;

/*!
 * @brief Queue capture modes.
 *
 * When a queue is capturing, it may affect potentially unsafe APIs.
 *
 * The potentially unsafe APIs refer to memory allocation and queue synchronization
 * related APIs, such as ::cnrtQueueSync(), ::cnrtQueueQuery() and ::cnrtMalloc(), etc,
 * which may cause unexpected result per called when any queue is capturing.
 *
 * ::cnrtQueueCaptureModeThreadLocal is not supported yet.
 */
typedef enum cnrtQueueCaptureMode {
  cnrtQueueCaptureModeGlobal = 0,
  /*!< If any queue is actively capturing under ::cnrtQueueCaptureModeGlobal mode, all the
       potentially unsafe APIs are prohibited from calling. */
  cnrtQueueCaptureModeThreadLocal = 1,
  /*!< If any queue is actively capturing under ::cnrtQueueCaptureModeThreadLocal mode, all
       the potentially unsafe APIs in local thread will be prohibited from calling. */
  cnrtQueueCaptureModeRelaxed = 2,
  /*!< If there are only queue captures activated under ::cnrtQueueCaptureModeRelaxed mode,
       no potentially unsafe APIs will be prohibited from calling. */
} cnrtQueueCaptureMode_t;

/*!
 * @brief Flags for ::cnrtQueueUpdateCaptureDependencies.
 */
typedef enum cnrtUpdateQueueCaptureDependenciesFlags {
  cnrtQueueAddCaptureDependencies = 0,
  /*!< Adds new nodes to the dependency set. */
  cnrtQueueSetCaptureDependencies = 1,
  /*!< Replaces dependency set with new nodes. */
} cnrtUpdateQueueCaptureDependenciesFlags_t;

/*! @brief
 *
 * CNRT host function.
 */
typedef void (*cnrtHostFn_t)(void *usreData);

/************************************************************************
 * Function prototype declaration
 ************************************************************************/

/************************************************************************
 * Device management
 ************************************************************************/
//Group: Device Management
/**
 * @brief Retrieves the information about the device.
 * @details Retrieves the ``device`` attributes of ``attr`` in ``pValue``.
 *          See the supported ``attr`` in ::cnrtDeviceAttr_t.
 * @param[out] pValue Pointer to device attribute value.
 * @param[in]  attr The device attribute to retrieve.
 * @param[in]  device The device ordinal to retrieve.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoDevice
 * @note
 * - None.
 * @example
 * @code{.cpp}
    int main () {
      int ordinal = -1;
      cnrtGetDevice(&ordinal);
      int value = 0;
      cnrtDeviceGetAttribute(&value, cnrtAttrClusterCount, ordinal);
      printf("device: %d, cnrtAttrClusterCount: %d.\n", ordinal, value);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtDeviceGetAttribute(int *pValue, cnrtDeviceAttr_t attr,
                                 int device);
//Group: Device Management
/**
 * @brief Retrieves the information about the MLU device.
 * @details Retrieves the ``prop`` properties of ordinal ``device``.
 *          See details in ::cnrtDeviceProp_t.
 * @param[out] prop Pointer to the returned struct ::cnrtDeviceProp_t.
 * @param[in]  device Device ordinal to retrieve.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoDevice
 * @note
 * - None.
 * @example
 * @code{.cpp}
    int main () {
      int ordinal = -1;
      cnrtGetDevice(&ordinal);
      cnrtDeviceProp_t prop;
      cnrtGetDeviceProperties(&prop, ordinal);
      printf("device: %d, device name: %s.\n", ordinal, prop.name);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtGetDeviceProperties(cnrtDeviceProp_t *prop, int device);
//Group: Device Management
/**
 * @brief Retrieves which device is currently being used.
 * @details Retrieves the current MLU device ordinal being used in ``pOrdinal``.
 * @param[out] pOrdinal Pointer to the device ordinal being used.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoDevice
 * @note
 * - None.
 * @example
 * - See example in ::cnrtGetDeviceProperties.
 */
cnrtRet_t cnrtGetDevice(int *pOrdinal);
//Group: Device Management
/**
 * @brief Sets the device to be used.
 * @details Sets the currently used MLU device to ``ordinal`` for the calling
 *          host thread.
 * @param[in] ordinal The device ordinal to be used.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoDevice
 * @note
 * - The device ``ordinal`` ranges in [0, cnrtGetDeviceCount() - 1].
 * - All the device memory allocated by ::cnrtMalloc after the API is called will
 *   be physically on the device ``ordinal``. All the host memory allocated by ::cnrtHostMalloc
 *   will be associated with device ``ordinal``. All the queues and notifiers will be
 *   associated with device ``ordinal``. All the kernel launches will be executed on the device ``ordinal``.
 * @example
 * - See example in ::cnrtMemcpyPeer.
 */
cnrtRet_t cnrtSetDevice(int ordinal);
//Group: Device Management
/**
 * @brief Retrieves the number of MLU devices.
 * @details Retrieves in ``pCount`` the number of MLU devices in current system.
 * @param[out] pCount Pointer to the number of MLU devices.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - See example in ::cnrtMemcpyPeer.
 */
cnrtRet_t cnrtGetDeviceCount(unsigned int *pCount);
//Group: Device Management
/**
 * @brief Destroys all device resources and resets all state on the current
 *        device in the current process.
 * @details Releases all the allocated resources on the current device in the current process.
 *          If ::cnrtDeviceReset is called, any subsequent API call to this device will
 *          reinitialize the current device.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoDevice,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Once this API is called, the device will be initialized immediately.
 *   So synchronize the host threads, otherwise coredump may occur.
 * @example
 * @code{.cpp}
    int main () {
      size_t size = sizeof(size_t) * N;
      void *d = NULL;
      cnrtMalloc((void **)&d, size);
      cnrtDeviceReset();

      if (cnrtSuccess == cnrtFree(d)) {
        printf("device reset failed.\n");
        return -1;
      }

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtDeviceReset(void);
//Group: Device Management
/**
 * @brief Waits for the current device in the current process to complete
 *        all the preceding tasks.
 * @details Blocks any further executions on the host until all the operations in the queues
 *          in the current process on the current MLU device are completed absolutely.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtGetLastError.
 */
cnrtRet_t cnrtSyncDevice(void);
//Group: Device Management
/**
 * @brief Retrieves the device flags used for the current process executions on
 *        the current device. See ::cnrtSetDeviceFlag() for flag values.
 * @details Returns in ``pFlags`` how CPU interacts with the OS scheduler when
 *          waiting for the device execution result.
 * @param[out] pFlags Pointer to the flags specifies whether the
 *        CPU thread yields or spins when waiting for the device execution result.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoDevice,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtSetDeviceFlag.
 */
cnrtRet_t cnrtGetDeviceFlag(uint32_t *pFlags);
//Group: Device Management
/**
 * @brief Sets flags used for the current process executions on the current device.
 * @details After initializing the current device, this API specifies whether the
 *          CPU thread yields or spins when waiting for the device execution result in the ``flags``.
 * The flags of input parameters determine the CPU thread scheduler patterns, which include:
 * - cnrtDeviceScheduleSpin: Allows to spin actively when waiting for results from device. This pattern decreases latency but may lower the performance of CPU threads and high CPU usage.
 * - cnrtDeviceScheduleBlock: The thread is blocked with low CPU usage, and enters sleep mode.
 * - cnrtDeviceScheduleYield: Allows to yield current thread when waiting for results from device. This pattern may increase latency but increase the performance of CPU threads.
 * - cnrtDeviceScheduleAuto: Automatic scheduling. According to the device number and the number of processors in the system, choose the mode cnrtDeviceScheduleSpin or cnrtDeviceScheduleBlock.
 *
 * @param[in] flags Options of how CPU interacts with the OS scheduler when
 *        waiting for the device execution result. See supported flags in ::cnrtDeviceFlags_t.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoDevice,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorSetOnActiveProcess
 * @note
 * - This API should be called when Shared Context is inactive, and before calling
 *   ::cnrtMalloc, ::cnrtQueueCreate, etc. For more details about
 *   Shared Context, see "Cambricon CNRT Upgrade Guide".

 * @example
 * @code{.cpp}
    int main () {
      unsigned int flag = 0xFFFFFFFF;
      cnrtGetDeviceFlag(&flag);
      printf("default device flag: %d.\n", flag);

      unsigned int setFlag = (unsigned int)cnrtDeviceScheduleBlock;
      cnrtSetDeviceFlag(setFlag);
      cnrtGetDeviceFlag(&flag);
      printf("device flag after set flag: %d.\n", flag);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtSetDeviceFlag(uint32_t flags);
//Group: Device Management
/**
 * @brief Gets device resource configurations.
 * @details Returns in ``pValue`` the current value of the configuration ``type``.
 *
 * @param[out] pValue  Returned value of configuration type.
 * @param[in]  type    Configuration type.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 *
 * @note
 * - None.
 *
 * @example
 * @code{.cpp}
   int main() {
     int64_t value;
     cnrtRet_t res = cnrtDeviceSetConfig(cnrtDeviceConfigPrintfFifoNum, 4096);
     assert(res == cnrtSuccess);
     res = cnrtDeviceGetConfig(&value, cnrtDeviceConfigPrintfFifoNum);
     assert(res == cnrtSuccess && value == 4096);
   }
 * @endcode
 */
cnrtRet_t cnrtDeviceGetConfig(int64_t *pValue, cnrtDeviceConfig_t type);
//Group: Device Management
/**
 * @brief Sets device resource configurations.
 * @details Sets ``type`` to ``value`` to update the current configuration maintained
 *          by the device.
 *          The ``value`` might be rounded up or down to the nearest legal value.
 *
 * @param[in] type    The configuration type to set.
 * @param[in] value   The value of configuration type.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 *
 * @note
 * - None.
 * @example
 * - See example in ::cnrtDeviceGetConfig().
 */
cnrtRet_t cnrtDeviceSetConfig(cnrtDeviceConfig_t type, int64_t value);

// Group: Device Management
/**
 * @brief Gets the device memory usage of all kernels on current device.
 *
 * @param[out] pBytes   The size of the device memory used by all kernels.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoDevice,
 * ::cnrtErrorSysNoMem
 *
 * @note
 * - This API does not trigger kernels loaded to the device.
 * - This API still return all kernel memory usage, even if lazy module load.
 *
 * @example
 * @code{.cpp}
    int main() {
      size_t kernel_memory_usage;
      cnrtSetDevice(0);
      cnrtDeviceQueryKernelMemoryUsage(&kernel_memory_usage);
    }
 * @endcode
 */
cnrtRet_t cnrtDeviceQueryKernelMemoryUsage(size_t *pBytes);

//Group: Device Management
/**
 * @brief Retrieves the inter-process memory handle for an allocated host or
 *        MLU device memory within the same process.
 * @details Returns the inter-process memory handle ``handle`` with the given host
 *          or MLU device memory pointed by ``ptr`` within the same process.
 *          The host or MLU device memory must be allocated by the ::cnrtHostMalloc or ::cnrtMalloc.
 * @param[out] handle Pointer to the unique inter-process handle for the host
 *                    or MLU device memory to share.
 * @param[in] ptr The base pointer to the allocated host or MLU device memory.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * @code{.cpp}
    int main () {
      int fd[2];
      if (pipe(fd) < 0) {
        printf("pipe fd failed.\n");
        return -1;
      }

      size_t size = 0x10000000;
      void *d_a = NULL;
      cnrtIpcMemHandle handle;
      pid_t pid = fork();

      if (pid == 0) {
        close(fd[1]);
        void *address = NULL;
        if (!read(fd[0], &handle, sizeof(handle))) {
          printf("read err.\n");
          return;
        }

        // Map the inter-process memory handle exported from another process
        cnrtMapMemHandle((void **)&address, handle, 0);
        cnrtMemset(address, 2, size);
        cnrtUnMapMemHandle(address);
        printf("child process dev addr[%p] mem handle[%p]\n", address, handle);
        exit(EXIT_SUCCESS);
      } else {
        cnrtMalloc((void **)&d_a, size);
        cnrtMemset(d_a, 1, size);

        // Acquire an inter-process memory handle
        cnrtAcquireMemHandle(&handle, d_a);
        printf("parent process d_a[%p] mem handle[%p]\n", d_a, handle);
        close(fd[0]);
        if (!write(fd[1], &handle, sizeof(handle))) {
          printf("write err.\n");
          return;
        }
        int status = -1;
        if (waitpid(pid, &status, 0) < 0) {
          printf("%s, waitpid error.\n", __func__);
          exit(EXIT_FAILURE);
        }
        EXPECT_EQ(WEXITSTATUS(status), EXIT_SUCCESS);
        EXPECT_NE(WIFEXITED(status), 0);
        char *h_a = (char *)malloc(size);
        cnrtMemcpy((void *)h_a, (void *)d_a, size, cnrtMemcpyNoDirection);
        for (size_t i = 0; i < size; i++) {
          if (2 != h_a[i]) {
            printf("data copy error.\n");
            break;
          }
        }
      }

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtAcquireMemHandle(cnrtIpcMemHandle *handle, void *ptr);
//Group: Device Management
/**
 * @brief Maps an inter-process memory handle exported from another process and
 *        returns a device pointer usable in the local process.
 * @details Maps an inter-process memory handle ``handle`` shared by another process into
 *          the memory address space of the current MLU device or host. Returns the
 *          host or MLU device memory pointer pointed by ``memPtr`` used in the local process.
 * @param[out] ptr Pointer to the host or MLU device memory.
 * @param[in]  handle The inter-process memory handle shared by another process.
 * @param[in]  flags Flags used in this operation. Currently it only supports value 0.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtAcquireMemHandle.
 */
cnrtRet_t cnrtMapMemHandle(void **ptr, cnrtIpcMemHandle handle, int flags);
//Group: Device Management
/**
 * @brief Attempts to close memory mapped with ::cnrtMapMemHandle.
 * @details Unmaps the mapping between the memory address space of the current device or host
 *          pointed by ``ptr`` and the inter-process memory handle shared by another process.
 *          The mapping relation is created by ::cnrtMapMemHandle.
 * @param[in] ptr Pointer to the host or MLU device memory.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtAcquireMemHandle.
 */
cnrtRet_t cnrtUnMapMemHandle(void *ptr);
//Group: Device Management
/**
 * @brief Queries if an MLU device is capable of directly accessing data
 *        on another MLU device.
 * @details Returns in ``canAccess`` if an MLU device ``ordinal`` can access data
 *          on another MLU device ``peerOrdinal``. If ``ordinal`` can access
 *          data on another MLU device ``peerOrdinal``, *canAccess is 1,
 *          otherwise it is 0.
 * @param[out] canAccess Pointer to the return value 1 or 0. If ``ordinal`` is
 *             able to access memories on another MLU device ``peerOrdinal``,
 *             the return value is 1, otherwise the return value is 0.
 * @param[in] ordinal The ordinal of the device if it can directly access
 *            memories on another device. Call ::cnrtGetDeviceCount
 *            to get the total number of devices in the system. The device
 *            ID is in the range [0, ::cnrtGetDeviceCount() – 1].
 * @param[in] peerOrdinal The ordinal of the device on which memories can be
 *            directly accessed. Call ::cnrtGetDeviceCount
 *            to get the total number of devices in the system. The ID of the
 *            device is in the range [0, ::cnrtGetDeviceCount() – 1].
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorDeviceInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtMemcpyPeer.
 */
cnrtRet_t cnrtGetPeerAccessibility(unsigned int *canAccess, int ordinal,
                                   int peerOrdinal);
//Group: Device Management
/**
 * @brief Retrieves the PCI bus ID for the required device.
 * @details Returns a string ``pciBusId`` of the ``device`` with a specified ``len``.
 * @param[out] pciBusId Pointer to the identifier for the device with the
 *                      format [domain]:[bus]:[device].[function]. The ``domain``, ``bus``,
 *                      ``device``, ``function`` will be shown as hexadecimal values.
 * @param[in] len The specified maximum length of string.
 * @param[in] device The device ordinal.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorDeviceInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtDeviceGetByPCIBusId API.
 */
cnrtRet_t cnrtDeviceGetPCIBusId(char *pciBusId, int len, int device);
//Group: Device Management
/**
 * @brief Retrieves a pointer to device.
 * @details Returns in the ``device`` by giving a PCI bus ID ``pciBusId``.
 * @param[out] device Pointer to the device ordinal.
 * @param[in]  pciBusId The string in one of the following forms: [domain]:[bus]:[device].[function][domain]:[bus]:[device][bus]:[device].[function]
 *                      where ``domain``, ``bus``, ``device`` and ``function`` are all hexadecimal values.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorDeviceInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * @code{.cpp}
    int main () {
      unsigned int count = 0;
      cnrtGetDeviceCount(&count);
      char str[100];
      int device = -1, ordinal = -1;
      for (unsigned int i = 0; i < count; i++) {
        cnrtSetDevice(i);
        cnrtDeviceGetPCIBusId(str, 100, i);
        cnrtDeviceGetByPCIBusId(&device, str);
        cnrtGetDevice(&ordinal);
        if (device != ordinal) {
          printf("Error: not the same device ordinal.\n");
        }
      }
      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtDeviceGetByPCIBusId(int *device, const char *pciBusId);
//Group: Device Management
/**
 * @brief Retrieves numerical values that correspond to the least and greatest
 *        queue priorities.
 * @details Returns in ``*pMinPriority`` and ``*pMaxPriority`` the numerical
 *          values that correspond to the least and greatest queue priorities
 *          respectively. Queue priorities follow a convention where lower numbers
 *          imply greater priorities. The range of meaningful queue priorities is
 *          given by [``*pMinPriority``, ``*pMaxPriority``]. If you
 *          attempt to create a queue with a priority value that is out of the
 *          meaningful range as specfied by this API, the priority is automatically
 *          clamped down or up to either ``*pMinPriority`` or ``*pMaxPriority``
 *          respectively. See ::cnrtQueueCreateWithPriority for details on
 *          creating a priority queue.
 * @param[out] pMinPriority Pointer to an integer in which the numerical value
 *             for least queue priority is returned.
 * @param[out] pMaxPriority Pointer to an integer in which the numerical value
 *             for greatest queue priority is returned.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.

 * @example
 * - See example in ::cnrtQueueCreateWithPriority API.
 */
cnrtRet_t cnrtDeviceGetQueuePriorityRange(int *pMinPriority, int *pMaxPriority);

// Group: Device Management
/**
 * @brief Resets all persisting cache lines in cache normal status.
 *
 * @details Resets all persisting cache lines for current device.
 *          This API takes effect immediately once it is returned.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNotSupport
 *
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtDeviceResetPersistingL2Cache(void);

// Group: Device Management
/**
 * @brief Gets an interprocess handle for a previously allocated notifier.
 * This handle might be copied into other processes and opened with ::cnrtIpcOpenNotifierHandle.
 *
 * @param[out] handle    Pointer to a user allocated ::cnrtIpcNotifierHandle in which to return the notifier IPC handle.
 * @param[in]  notifier  The notifier handle created by calling ::cnrtNotifierCreate
 *     with ::CNRT_NOTIFIER_INTERPROCESS and ::CNRT_NOTIFIER_DISABLE_TIMING_ALL.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorNoMem,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorNotSupport
 *
 * @note
 * - The ``handle`` will be invalid after you destroy the ``notifier`` by calling
 * ::cnrtNotifierDestroy.
 * - The new notifier allocated by ::cnrtIpcOpenNotifierHandle might be invalid if ``notifier`` has
 * been freed with ::cnrtNotifierDestroy.
 *
 * */
cnrtRet_t cnrtIpcGetNotifierHandle(cnrtIpcNotifierHandle *handle,
                                   cnrtNotifier_t notifier);

// Group: Device Management
/**
 * @brief Opens an interprocess notifier handle for use in the current process.
 * This API returns a new notifier handle like a locally created notifier with
 * ::CNRT_NOTIFIER_DISABLE_TIMING_ALL flag. This notifier must be freed with ::cnrtNotifierDestroy.
 *
 * @param[in]  handle     Interprocess handle got from ::cnrtIpcGetNotifierHandle.
 * @param[out] notifier  A new notifier handle.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorNoMem,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorNotSupport
 *
 * @note
 * - The ``notifier`` which is allocated by ::cnrtIpcOpenNotifierHandle cannot use
 * ::cnrtIpcGetNotifierHandle to get a new handle.
 *
 * */
cnrtRet_t cnrtIpcOpenNotifierHandle(cnrtNotifier_t *notifier,
                                    cnrtIpcNotifierHandle handle);

//Group: Device Management
/**
 * @brief Retrieves the default memory pool of a device.
 * @details Retrieves the default memory pool of a device managing the device memory.
 * @param[out] pool The memory pool.
 * @param[in]  dev The device.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 *
 * */
cnrtRet_t cnrtDeviceGetDefaultMemPool(cnrtMemPool_t *pool, int dev);

//Group: Device Management
/**
 * @brief Gets the current memory pool for a device.
 * @details Returns the last pool provided to ::cnrtDeviceSetMemPool for this device or the device's
 *          default memory pool if ::cnrtDeviceSetMemPool has never been called. By default the current
 *          memory pool is the default memory pool for a device. Otherwise the returned pool must have been set
 *          with ::cnrtDeviceSetMemPool.
 * @param[out] pool The memory pool.
 * @param[in]  dev The device.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 *
 * */
cnrtRet_t cnrtDeviceGetMemPool(cnrtMemPool_t *pool, int dev);

//Group: Device Management
/**
 * @brief Sets the current memory pool for a device.
 * @details The memory pool must be local to the specified device. ::cnrtMemAllocAsync allocates
 *          from the current memory pool of the provided queue's device. By default, a device's
 *          current memory pool is its default memory pool.
 * @param[in] pool The memory pool.
 * @param[in]  dev The device.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 *
 * */
cnrtRet_t cnrtDeviceSetMemPool(int dev, cnrtMemPool_t pool);

/************************************************************************
 * Execution control
 ************************************************************************/
//Group: Execution Control Management
/**
 * @brief Enqueues a host function call in a queue.
 * @details
 * This API is an asynchronous interface and ::cnrtSuccess does not
 * represent the completion of host function execution.
 * Call ::cnrtQueueQuery() or ::cnrtQueueSync() to confirm whether the host function call in the queue has been executed.
 * The host function call must not perform any synchronization that may depend on other asynchronous tasks
 * not mandated to run earlier, which can cause a deadlock.
 * Without a mandated order, host function is executed (in independent queues) in undefined order
 * and might be serialized.
 *
 * @param[in] queue    Enqueued queue.
 * @param[in] fn       The CPU API to call once preceding queue
 *                     operations are complete.
 * @param[in] userData User-specified data to be passed to the API.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorInit,
 * ::cnrtErrorCndrvFuncCall
 *
 * @note
 *  - The host function call must not make any CNRT or CNDrv API call.
 *    Attempting to use CNRT and CNDrv API may result in ::cnrtErrorCndrvFuncCall.
 *
 *
 * @example
 * @code{.cpp}
    // test.mlu
    void hostFunc1(void *args) {
      printf("%s: %p\n", __func__, args);
    }
    void hostFunc2(void *args) {
      printf("%s: %p\n", __func__, args);
    }
    __mlu_global__ kernel() {
      printf("%s\n", __func__);
    }

    int main() {
      void *arg1 = 0x1;
      void *arg2 = 0x2;

      // nullptr for default queue also works
      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);

      // ...
      kernel<<<dim, cnrtFuncTypeBlock, queue>>>();

      cnrtInvokeHostFunc(queue, hostFunc1, arg1);
      cnrtInvokeHostFunc(queue, hostFunc2, arg2);
      cnrtQueueSync(queue);

      // print out:
      // kernel
      // hostFunc1: 1
      // hostFunc2: 2

      // ...
    }
 * @endcode
 */
cnrtRet_t cnrtInvokeHostFunc(cnrtQueue_t queue, cnrtHostFn_t fn,
                             void *userData);

//Group: Execution Control Management
/**
 * @brief Enqueues a device kernel in a queue.
 *
 * @details
 * Invokes a ``kernel`` function, and enqueues the kernel to the ``queue``.
 * If the kernel has N parameters, the ``args`` should point to array of N pointers.
 * Each pointer, from args[0] to args[N - 1], pointer to the region of memory from
 * which the actual parameter will be copied.
 * This is an asynchronous API, the returned ::cnrtSuccess does not represent the
 * result of the kernel execution.
 *
 * @param[in] kernel     Device kernel symbol.
 * @param[in] dim        The dimension of {x, y, z}.
 * @param[in] ktype      The task size type.
 * @param[in] args       The array of pointers to kernel parameter.
 * @param[in] reserved   Reserved.
 * @param[in] queue      The queue to invoke kernel.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorInit,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoDevice,
 * ::cnrtErrorNoKernel,
 * ::cnrtErrorNoModule,
 * ::cnrtErrorNoCnrtContext
 *
 * @note
 * - None.
 *
 * @example
 * @code{.cpp}
    // x.mlu
    __mlu_entry__ void kernel(int a, int b) {
      printf("%d,%d\n", a, b);
    }

    int main() {
      cnrtQueue_t queue = NULL; // default queue
      cnrtDim3_t dim = {1, 1, 1};
      // The two ways to invoke kernel are equivalent.

      kernel<<<dim, cnrtFuncTypeBlock, queue>>>(1, 2);

      int a = 1, b = 2;
      void *args[] = {&a, &b};
      cnrtInvokeKernel(kernel, dim, cnrtFuncTypeBlock, args, 0, queue);

      cnrtQueueSync(queue);
    }
 * @endcode
 */
cnrtRet_t cnrtInvokeKernel(const void *kernel, cnrtDim3_t dim,
                           cnrtFunctionType_t ktype, void **args,
                           size_t reserved, cnrtQueue_t queue);

/************************************************************************
 * Memory management
 ************************************************************************/
//Group: Memory Management
/**
 * @brief Allocates memory on the current device.
 * @details Allocates the ``bytes`` size of current MLU device memory, and
 *          returns a pointer ``pPtr`` to the allocated memory.
 * @param[out] pPtr Pointer to allocated MLU memory.
 * @param[in]  bytes Requested memory size in bytes.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorNoMem,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtFree to release the allocated memory, otherwise memory leak may occur.

 * @example
 * @code{.cpp}
    int main () {
      // Specify the memory size
      size_t size = sizeof(size_t) * N;
      // Allocate page-locked memory. For more information about the advantages of using the cnrtHostMalloc,
      // see cnrtHostMalloc description.
      void *h = NULL;
      cnrtHostMalloc((void **)&h, size);

      // Initialize host memory allocated before
      ...

      // Allocate device memory
      void *d = NULL;
      cnrtMalloc((void **)&d, size);

      // Copy data from host to device
      cnrtMemcpy(d, h, size, cnrtMemcpyHostToDev);
      // Copy data from device to host
      cnrtMemcpy(h, d, size, cnrtMemcpyDevToHost);

      // Free resource
      cnrtFreeHost(h);
      cnrtFree(d);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMalloc(void **pPtr, size_t bytes);
//Group: Memory Management
/**
 * @brief Allocates constant memory on the current device.
 * @details Allocates the ``bytes`` size of current MLU device constant memory, and
 *          returns a pointer ``pPtr`` to the allocated memory.
 * @param[out] pPtr Pointer to allocated MLU memory.
 * @param[in]  bytes Requested memory size in bytes.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorNoMem,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtFree to release the allocated constant memory, otherwise memory leak may occur.

 * @example
 * @code{.cpp}
    int main () {
      // Specify the memory size
      size_t size = sizeof(size_t) * N;
      // Allocate page-locked memory. For more information about the advantages of using the cnrtHostMalloc,
      // see cnrtHostMalloc description.
      void *h = NULL;
      cnrtHostMalloc((void **)&h, size);

      // Initialize host memory allocated before
      ...

      // Allocate device memory
      void *d = NULL;
      cnrtMallocConstant((void **)&d, size);

      // Copy data from host to device
      cnrtMemcpy(d, h, size, cnrtMemcpyHostToDev);
      // Copy data from device to host
      cnrtMemcpy(h, d, size, cnrtMemcpyDevToHost);

      // Free resource
      cnrtFreeHost(h);
      cnrtFree(d);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMallocConstant(void **pPtr, size_t bytes);
//Group: Memory Management
/**
 * @brief Frees the memory on the device.
 * @details Frees the MLU device memory pointed by ``ptr``, which must have been
 *          returned by a previous call to ::cnrtMalloc. Otherwise, if
 *          ::cnrtFree(``ptr``) has already been called before, an error is returned.
 *          If ``ptr`` is 0, no operation is performed.
 * @param[in] ptr Pointer to the device memory to be freed.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.

 * @example
 * - See example in ::cnrtMalloc.
 */
cnrtRet_t cnrtFree(void *ptr);
//Group: Memory Management
/**
 * @brief Allocates page-locked memory on the host.
 * @details Allocates size of ``bytes`` of page-locked host memory, and
 *          returns a pointer ``pPtr`` to the allocated memory.
 *          The memory ranges allocated with this API can be tracked by the driver
 *          so that the API calls such as ::cnrtMemcpy are accelerated automatically.
 *          In comparison with pageable memory requested by the system malloc function,
 *          the page-locked memory has better read and write performance. However,
 *          allocating large amount of page-locked memory with this API may lead to
 *          lower performance due to reduction of available amount of memory for paging.
 *          So for best practice, it is recommended to use this API to allocate staging areas for data
 *          exchange between host and the MLU device.
 * @param[out] pPtr Pointer to the allocated host memory.
 * @param[in]  bytes The size requested to allocate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorSysNoMem,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtFreeHost to release the allocated host memory,
 *   otherwise the memory leak may occur.

 * @example
 * - See example in ::cnrtMalloc API.
 */
cnrtRet_t cnrtHostMalloc(void **pPtr, size_t bytes);
//Group: Memory Management
/**
 * @brief Frees the page-locked memory.
 * @details Frees the host memory pointed by ``ptr``, this API is only used to
 *          free the host memory that is allocated by the ::cnrtHostMalloc.
 * @param[in] ptr Pointer to the host memory.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.

 * @example
 * - See example in ::cnrtMalloc API.
 */
cnrtRet_t cnrtFreeHost(void *ptr);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address.
 * @details Synchronously copies the size of ``bytes`` bytes of data from the source address
 *          pointed by ``src`` to the destination address pointed by ``dst`` with the copy
 *          direction ``dir``.
 * @param[in] dst Pointer to the destination address.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size in bytes to be copied.
 * @param[in] dir Data copying direction.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API is used to copy data synchronously. To copy data asynchronously,
 *   call ::cnrtMemcpyAsync_V3.
 * - Calling ::cnrtMemcpy with pointers ``src`` and ``dst`` that do not
 *   match the copy direction ``dir`` may lead to unpredictable behavior.

 * @example
 * - See example in ::cnrtMalloc API.
 */
cnrtRet_t cnrtMemcpy(void *dst, void *src, size_t bytes, cnrtMemTransDir_t dir);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address.
 * @details Synchronously reads ``spitch * width`` bytes of data
 *          from the source address pointed by ``src``, and writes it to the
 *          ``dpitch * width`` bytes of destination address pointed by ``dst``
 *          with the copy direction ``dir``. ``dir`` must be
 *          ::cnrtMemcpyDevToDev, ::cnrtMemcpyHostToDev, ::cnrtMemcpyDevToHost,
 *          ::cnrtMemcpyHostToHost, or ::cnrtMemcpyNoDirection.
 * @param[in] dst Pointer to the destination address.
 * @param[in] dpitch The pitch of the destination memory. It must be
 *  greater than or equal to the width.
 * @param[in] src Pointer to the source address.
 * @param[in] spitch The pitch of the source memory. It must be
 *  greater than or equal to the width.
 * @param[in] width The width of the memory to be copied.
 * @param[in] height The height of the memory to be copied.
 * @param[in] direction Data copy direction. It must be
 *  ::cnrtMemcpyDevToDev, ::cnrtMemcpyHostToDev, ::cnrtMemcpyDevToHost, ::cnrtMemcpyHostToHost,
 *  or ::cnrtMemcpyNoDirection.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None of the parameters can be 0.

 * @example
 * @code{.cpp}
   // ...
   // Specify the memory size
   size_t size = dpitch * spitch;

   // Allocate device memory of the source memory
   void *src = NULL;
   cnrtMalloc((void **)&src, size);

   // Initialize src memory allocated before
   ...

   // Allocate device memory of the destination memory
   void *dst = NULL;
   cnrtMalloc((void **)&dst, size);

   // Initialize param of dpitch, spitch, width, height
   ...

   // Use 2D to copy data from src to dst
   cnrtMemcpy2D(dst, dpitch, src, spitch, width, height, cnrtMemcpyDevToDev);

   // Free resource
   cnrtFree(src);
   cnrtFree(dst);
   // ...
 * @endcode
 */
cnrtRet_t cnrtMemcpy2D(void *dst, size_t dpitch, const void *src, size_t spitch,
                       size_t width, size_t height,
                       cnrtMemTransDir_t direction);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address asynchronously.
 * @details Asynchronously reads ``spitch * width`` bytes of data
 *          from the source address pointed by ``src``, and writes it to the
 *          ``dpitch * width`` bytes of destination address pointed by ``dst``
 *          with the copy direction ``dir``. ``dir`` must be
 *          ::cnrtMemcpyDevToDev, ::cnrtMemcpyHostToDev, ::cnrtMemcpyDevToHost,
 *          ::cnrtMemcpyHostToHost, or ::cnrtMemcpyNoDirection.
 * @param[in] dst Pointer to the destination address.
 * @param[in] dpitch The pitch of the destination memory. It must be
 *  greater than or equal to the width.
 * @param[in] src Pointer to the source address.
 * @param[in] spitch The pitch of the source memory. It must be
 *  greater than or equal to the width.
 * @param[in] width The width of the memory to be copied.
 * @param[in] height The height of the memory to be copied.
 * @param[in] direction Data copy direction. It must be
 *  ::cnrtMemcpyDevToDev, ::cnrtMemcpyHostToDev, ::cnrtMemcpyDevToHost, ::cnrtMemcpyHostToHost,
 *  or ::cnrtMemcpyNoDirection.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None of the parameters can be 0.

 * @example
 * @code{.cpp}
   // ...
   // Specify the memory size
   size_t size = dpitch * spitch;

   // Allocate device memory of the source memory
   void *src = NULL;
   cnrtMalloc((void **)&src, size);

   // Initialize src memory allocated before
   ...

   // Allocate device memory of the destination memory
   void *dst = NULL;
   cnrtMalloc((void **)&dst, size);

   // Initialize param of dpitch, spitch, width, height
   ...

   // Use 2D to copy data from src to dst
   cnrtMemcpy2DAsync(dst, dpitch, src, spitch, width, height, cnrtMemcpyDevToDev, queue);
   cnrtQueueSync(queue);

   // Free resource
   cnrtFree(src);
   cnrtFree(dst);
   // ...
 * @endcode
 */
cnrtRet_t cnrtMemcpy2DAsync(void *dst, size_t dpitch, const void *src,
                            size_t spitch, size_t width, size_t height,
                            cnrtMemTransDir_t direction, cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address.
 * @details Synchronously reads ``src_size`` bytes of data from the
 *          source address pointed by ``p->src`` or ``p->srcPtr.ptr``, and
 *          writes it to the ``dst_size`` bytes of data from the
 *          destination address pointed by ``p->dst`` or ``p->dstPtr.ptr``.
 *          The ``src_size`` is configured by ``p->extent`` and ``p->srcPtr``,
 *          the ``dst_size`` is configured by ``p->extent`` and ``p->dstPtr``.
 *          The copy direction ``direction`` must be in ::cnrtMemTransDir_t.
 *          The ``direction`` must be ::cnrtMemcpyDevToDev, ::cnrtMemcpyNoDirection,
 *          ::cnrtMemcpyHostToDev, ::cnrtMemcpyDevToHost or cnrtMemcpyHostToHost.
 * @param[in] p Pointer to the 3D memory copy parameter.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None of the parameters can be 0.

 * @example
 * @code{.cpp}
   // ...
   cnrtMemcpy3dParam_t *p;
   p = (cnrtMemcpy3dParam_t *)malloc(sizeof(*p));

   // Initialize 3D param of p
   ...

   // Specify the memory size
   size_t size = dstPtr.pitch * srcPtr.pitch * extent.depth;

   // Allocate device memory of the source memory
   void *src = NULL;
   cnrtMalloc((void **)&src, size);

   // Initialize src memory allocated before
   ...

   // Allocate device memory of the destination memory
   void *dst = NULL;
   cnrtMalloc((void **)&dst, size);

   p->dstPtr.ptr = dst;
   p->srcPtr.ptr = src;

   // Use 3D to copy data from src to dst
   cnrtMemcpy3D(p);

   // Free resource
   cnrtFree(src);
   cnrtFree(dst);
   free(p);
   // ...
 * @endcode
 */
cnrtRet_t cnrtMemcpy3D(const cnrtMemcpy3dParam_t *p);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address asynchronously.
 * @details Asynchronously reads ``src_size`` bytes of data from the
 *          source address pointed by ``p->src`` or ``p->srcPtr.ptr``, and
 *          writes it to the ``dst_size`` bytes of data from the
 *          destination address pointed by ``p->dst`` or ``p->dstPtr.ptr``.
 *          ``src_size`` is configured by ``p->extent`` and ``p->srcPtr``,
 *          and ``dst_size`` is configured by ``p->extent`` and ``p->dstPtr``.
 *          The copy direction ``direction`` must be in ::cnrtMemTransDir_t.
 *          ``direction`` must be ::cnrtMemcpyDevToDev, ::cnrtMemcpyNoDirection,
 *          ::cnrtMemcpyHostToDev, ::cnrtMemcpyDevToHost or cnrtMemcpyHostToHost.
 * @param[in] p Pointer to the 3D memory copy parameter.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None of the parameters can be 0.

 * @example
 * @code{.cpp}
   // ...
   cnrtMemcpy3dParam_t *p;
   p = (cnrtMemcpy3dParam_t *)malloc(sizeof(*p));

   // Initialize 3D param of p
   ...

   // Specify the memory size
   size_t size = dstPtr.pitch * srcPtr.pitch * extent.depth;

   // Allocate device memory of the source memory
   void *src = NULL;
   cnrtMalloc((void **)&src, size);

   // Initialize src memory allocated before
   ...

   // Allocate device memory of the destination memory
   void *dst = NULL;
   cnrtMalloc((void **)&dst, size);

   p->dstPtr.ptr = dst;
   p->srcPtr.ptr = src;

   // Use 3D to copy data from src to dst
   cnrtMemcpy3DAsync(p, queue);
   cnrtQueueSync(queue);

   // Free resource
   cnrtFree(src);
   cnrtFree(dst);
   free(p);
   // ...
 * @endcode
 */
cnrtRet_t cnrtMemcpy3DAsync(const cnrtMemcpy3dParam_t *p, cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address asynchronously.
 * @details Asynchronously copies the size of ``bytes`` bytes of data from the source address
 *          pointed by ``src`` to the destination address pointed by ``dst`` with the copy
 *          direction ``dir``. The ``dir`` must be one of ::cnrtMemcpyHostToDev,
 *          ::cnrtMemcpyDevToDev, ::cnrtMemcpyDevToHost, ::cnrtMemcpyPeerToPeer,
 *          and ::cnrtMemcpyNoDirection, in ::cnrtMemTransDir_t. It is recommended to use
 *          ::cnrtMemcpyNoDirection.
 * @param[in] dst Pointer to the destination address.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] dir The data copying direction.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API is used to copy data asynchronously. To copy data synchronously,
 *   call ::cnrtMemcpy.
 * - Calling ::cnrtMemcpyAsync with pointers ``src`` and ``dst`` that do not
 *   match the copy direction ``dir`` may lead to unpredictable behavior.
 * - This function will be deprecated in a future release. Use ::cnrtMemcpyAsync_V3 instead.
 * @example
 * - See example in ::cnrtMemcpyAsync_V3.
 */
cnrtRet_t cnrtMemcpyAsync(void *dst, void *src, size_t bytes, cnrtQueue_t queue,
                          cnrtMemTransDir_t dir);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address asynchronously.
 * @details Asynchronously copies the size of ``bytes`` bytes of data from the source address
 *          pointed by ``src`` to the destination address pointed by ``dst`` with the copy
 *          direction ``dir``. The ``dir`` must be one of ::cnrtMemcpyHostToDev,
 *          ::cnrtMemcpyDevToDev, ::cnrtMemcpyDevToHost, ::cnrtMemcpyPeerToPeer,
 *          and ::cnrtMemcpyNoDirection, in ::cnrtMemTransDir_t. It is recommended to use
 *          ::cnrtMemcpyNoDirection.
 * @param[in] dst Pointer to the destination address.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] dir The data copying direction.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API exhibits asynchronous behavior for most use cases.
 * - Calling ::cnrtMemcpyAsync_V2 with pointers ``src`` and ``dst`` that do not
 *   match the copy direction ``dir`` may lead to unpredictable behavior.
 * - This function will be deprecated in a future release. Use ::cnrtMemcpyAsync_V3 instead.
 * @example
 * - See example in ::cnrtMemcpyAsync_V3.
 */
cnrtRet_t cnrtMemcpyAsync_V2(void *dst, void *src, size_t bytes,
                             cnrtQueue_t queue, cnrtMemTransDir_t dir);
//Group: Memory Management
/**
 * @brief Copies data from source address to destination address asynchronously.
 * @details Asynchronously copies the size of ``bytes`` bytes of data from the source address
 *          pointed by ``src`` to the destination address pointed by ``dst`` with the copy
 *          direction ``dir``. The ``dir`` must be one of ::cnrtMemcpyHostToDev,
 *          ::cnrtMemcpyDevToDev, ::cnrtMemcpyDevToHost, ::cnrtMemcpyPeerToPeer,
 *          and ::cnrtMemcpyNoDirection, in ::cnrtMemTransDir_t. It is recommended to use
 *          ::cnrtMemcpyNoDirection.
 * @param[in] dst Pointer to the destination address.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] dir The data copying direction.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API exhibits asynchronous behavior for most use cases.
 * - Calling ::cnrtMemcpyAsync_V3 with pointers ``src`` and ``dst`` that do not
 *   match the copy direction ``dir`` may lead to unpredictable behavior.
 * @example
 * @code{.cpp}
    int main () {
      // Prepare input and output
      size_t size = sizeof(size_t) * N;
      char *h0 = NULL;
      char *h1 = NULL;
      cnrtHostMalloc((void **)&h0, size);
      cnrtHostMalloc((void **)&h1, size);
      memset(h0, 'a', size);

      void *d = NULL;
      cnrtMalloc((void **)&d, size);

      // Create queue
      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);

      // Memcpy Async
      cnrtMemcpyAsync_V3(d, h0, size, queue, cnrtMemcpyNoDirection);
      cnrtMemcpyAsync_V3(h1, d, size, queue, cnrtMemcpyNoDirection);
      cnrtQueueSync(queue);

      // Free resource
      cnrtQueueDestroy(queue);
      cnrtFreeHost(h0);
      cnrtFreeHost(h1);
      cnrtFree(d);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMemcpyAsync_V3(void *dst, void *src, size_t bytes,
                             cnrtQueue_t queue, cnrtMemTransDir_t dir);
//Group: Memory Management
/**
 * @brief Copies data between two devices.
 * @details Synchronously copies ``bytes`` of data from the address pointed by ``src``
 *          in source device ordinal ``srcDev`` to the address pointed by ``dst`` in
 *          destination device ordinal ``dstDev``.
 * @param[in] dst Pointer to the destination address.
 * @param[in] dstDev The destination device ordinal.
 * @param[in] src Pointer to the source address.
 * @param[in] srcDev The source device ordinal.
 * @param[in] bytes The memory size to be copied in bytes.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorDeviceInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Devices support peer-to-peer communications. If the two MLU devices
 *   are not peerable, it might be hardware problems.
 * - The two devices must be peerable when calling ::cnrtMemcpyPeer to copy data. Call
 *   ::cnrtGetPeerAccessibility to check if the two devices are peerable.

 * @example
 * @code{.cpp}
    int main () {
      unsigned int count = 0;
      cnrtGetDeviceCount(&count);
      if (count < 2) {
        printf("Warning: Two or more MLU devices are required for Peer-to-Peer.\n");
        return 0;
      }

      // To simplify sample code, it is supposed that srcDev is 0, and dstDev is 1
      int srcDev = 0;
      int dstDev = 1;

      int canAccess = -1;
      cnrtGetPeerAccessibility(&canAccess, srcDev, dstDev);
      if (canAccess != 1) {
          printf("Error: There is no p2p accessibility MLU devices in the current system.\n");
          return -1;
      }

      cnrtSetDevice(srcDev);
      char *src = NULL;
      cnrtMalloc((void **)&src, size);
      cnrtMemset(src, 'a', size);

      cnrtSetDevice(dstDev);
      char *dst = NULL;
      cnrtMalloc((void **)&dst, size);

      cnrtMemcpyPeer(dst, dstDev, src, srcDev, size);

      // Free resource
      cnrtFree((void *)src);
      cnrtFree((void *)dst);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMemcpyPeer(void *dst, int dstDev, void *src, int srcDev,
                         size_t bytes);
//Group: Memory Management
/**
 * @brief Copies data between two devices asynchronously.
 * @details Asynchronously copies ``bytes`` of data from the address pointed by ``src``
 *          in source device ordinal ``srcDev`` to the address pointed by ``dst`` in
 *          destination device ordinal ``dstDev``.
 * @param[in] dst Pointer to the destination address.
 * @param[in] dstDev The destination device ordinal.
 * @param[in] src Pointer to the source address.
 * @param[in] srcDev The source device ordinal.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @param[in] bytes The memory size to be copied in bytes.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorDeviceInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API is used to copy data asynchronously. To copy data synchronously,
 *   call ::cnrtMemcpyPeer.
 * - Devices support peer-to-peer communications. If the two MLU devices
 *   are not peerable, it might be hardware problems.
 * - The two devices must be peerable when calling ::cnrtMemcpyPeerAsync to copy data. Call
 *   ::cnrtGetPeerAccessibility to check if the two devices are peerable.
 * @example
 * @code{.cpp}
    int main () {
      unsigned int count = 0;
      cnrtGetDeviceCount(&count);
      if (count < 2) {
        printf("Warning: Two or more MLU devices are required for Peer-to-Peer.\n");
        return 0;
      }

      // To simplify sample code, suppose sdev is 0, ddev is 1
      int srcDev = 0;
      int dstDev = 1;

      int canAccess = -1;
      cnrtGetPeerAccessibility(&canAccess, srcDev, dstDev);
      if (canAccess != 1) {
          printf("Error: There is no P2P accessibility MLU devices in the current system.\n");
          return -1;
      }

      cnrtSetDevice(srcDev);
      char *src = NULL;
      cnrtMalloc((void **)&src, size);

      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);
      cnrtMemsetAsync(src, 'a', size, queue);

      cnrtSetDevice(dstDev);
      char *dst = NULL;
      cnrtMalloc((void **)&dst, size);

      cnrtMemcpyPeer(dst, dstDev, src, srcDev, size, queue);
      cnrtQueueSync(queue);

      // Free resource
      cnrtFree((void *)src);
      cnrtFree((void *)dst);
      cnrtQueueDestroy(queue);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMemcpyPeerAsync(void *dst, int dstDev, void *src, int srcDev,
                              size_t bytes, cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Finds the address of an MLU symbol.
 * @details Returns in ``pPtr`` the address of ``symbol`` on device.
 * @param[out] pPtr Pointer to the address of the ``symbol`` on device.
 * @param[in] symbol Symbol to query.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorInvalidSymbol
 * @example
 * @code{.cpp}
    // __mlu_entry__ function and __mlu_device__ variables must be written in a single file named *.mlu
    #define TEST_SIZE 64
    __mlu_device__ int array_f[TEST_SIZE];
    __mlu_entry__ void kernelSetDeviceMem(size_t size) {
      int i = 0;
      for (i = 0; i < size; i++) {
        array_f[i] = 0x12345678;
      }
    }

    // Hybrid programming
    int main () {
      size_t size = 0;
      void *addr = NULL;
      cnrtDim3_t dim = {1, 1, 1};
      cnrtQueue_t queue;
      CNRT_CHECK(cnrtQueueCreate(&queue));
      CNRT_CHECK(cnrtGetSymbolAddress(&addr, array_f));
      CNRT_CHECK(cnrtGetSymbolSize(&size, array_f));
      int _h_array[TEST_SIZE];
      int i = 0;
      for (i = 0; i < TEST_SIZE;i++) {
        _h_array[i] = 0;
      }
      hostSetDeviceMem(TEST_SIZE, dim, cnrtFuncTypeBlock, queue);
      CNRT_CHECK(cnrtGetLastError());
      CNRT_CHECK(cnrtQueueSync(queue));
      CNRT_CHECK(cnrtMemcpyFromSymbol(_h_array, (const void *)&array_f, TEST_SIZE * sizeof(int), 0, cnrtMemcpyNoDirection));
      for (i = 0; i < TEST_SIZE; i++) {
        printf("After copy from symbol, array[%d] is %x.\n", i, _h_array[i]);
      }
      cnrtQueueDestroy(queue);
    }
 * @endcode
 */
cnrtRet_t cnrtGetSymbolAddress(void **pPtr, const void *symbol);

//Group: Memory Management
/**
 * @brief Finds the size of an MLU symbol.
 * @details Returns in ``size`` the size of ``symbol`` on the device.
 * @param[out] size Pointer to the size of the ``symbol`` on the device.
 * @param[in] symbol Symbol to query.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorInvalidSymbol
 * @example
 * - See example in ::cnrtGetSymbolAddress API.
 */
cnrtRet_t cnrtGetSymbolSize(size_t *size, const void *symbol);
//Group: Memory Management
/**
 * @brief Copies data from the given symbol on the device.
 * @details Copies ``bytes`` data from the memory area pointed to by ``offset`` from
 *          the start of ``symbol`` to the memory area pointed to by ``dst``. The
 *          memory areas cannot overlap.
 * @param[in] dst Pointer to the destination address.
 * @param[in] symbol Symbol address on the host.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] offset Offset from start of symbol in bytes.
 * @param[in] dir Data copy direction. It must be ::cnrtMemcpyDevToHost,
 *            ::cnrtMemcpyPeerToPeer , ::cnrtMemcpyDevToDev or ::cnrtMemcpyNoDirection.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorInvalidSymbol
 * @note
 * - This API is used to copy data synchronously. To copy data asynchronously,
 *   call ::cnrtMemcpyFromSymbolAsync.
 * @example
 * - See example in ::cnrtGetSymbolAddress API.
 */
cnrtRet_t cnrtMemcpyFromSymbol(void *dst, const void *symbol, size_t bytes,
                               size_t offset, cnrtMemTransDir_t dir);

//Group: Memory Management
/**
 * @brief Copies data to the given symbol on the device.
 * @details Copies ``bytes`` data from the memory area pointed to by ``src`` to the
 *          memory area pointed to by ``offset`` bytes from the start of ``symbol``.
 *          The memory areas cannot overlap.
 * @param[in] symbol Symbol address on the host.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] offset Offset from start of symbol in bytes.
 * @param[in] dir Data copy direction. It must be ::cnrtMemcpyHostToDev,
 *            ::cnrtMemcpyPeerToPeer , ::cnrtMemcpyDevToDev or ::cnrtMemcpyNoDirection.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorInvalidSymbol
 * @note
 * - This API is used to copy data synchronously. To copy data asynchronously,
 *   call ::cnrtMemcpyToSymbolAsync.
 * @example
 * - None.
 */
cnrtRet_t cnrtMemcpyToSymbol(const void *symbol, const void *src, size_t bytes,
                             size_t offset, cnrtMemTransDir_t dir);
//Group: Memory Management
/**
 * @brief Copies data asynchronously from the given symbol on the device.
 * @details Copies ``bytes`` data asynchronously from the memory area pointed
 *          to by ``offset`` from the start of ``symbol`` to the memory area
 *          pointed to by ``dst``. The memory areas cannot overlap.
 * @param[in] dst Pointer to the destination address.
 * @param[in] symbol Symbol address on the host.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] offset Offset from start of symbol in bytes.
 * @param[in] dir Data copy direction. It must be ::cnrtMemcpyDevToHost,
 *            ::cnrtMemcpyPeerToPeer , ::cnrtMemcpyDevToDev or ::cnrtMemcpyNoDirection.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorInvalidSymbol
 * @note
 * - This API is used to copy data asynchronously. To copy data synchronously,
 *   call ::cnrtMemcpyFromSymbol.
 * @example
 * - None.
 */
cnrtRet_t cnrtMemcpyFromSymbolAsync(void *dst, const void *symbol, size_t bytes,
                                    size_t offset, cnrtMemTransDir_t dir,
                                    cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Copies data asynchronously from the given symbol on the device.
 * @details Copies ``bytes`` data asynchronously from the memory area pointed
 *          to by ``offset`` from the start of ``symbol`` to the memory area
 *          pointed to by ``dst``. The memory areas cannot overlap.
 * @param[in] dst Pointer to the destination address.
 * @param[in] symbol Symbol address on the host.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] offset Offset from start of symbol in bytes.
 * @param[in] dir Data copy direction. It must be ::cnrtMemcpyDevToHost,
 *            ::cnrtMemcpyPeerToPeer , ::cnrtMemcpyDevToDev or ::cnrtMemcpyNoDirection.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorInvalidSymbol
 * @note
 * - This API exhibits asynchronous behavior for most use cases.
 * @example
 * - None.
 */
cnrtRet_t cnrtMemcpyFromSymbolAsync_V2(void *dst, const void *symbol,
                                       size_t bytes, size_t offset,
                                       cnrtMemTransDir_t dir,
                                       cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Copies data asynchronously to the given symbol on the device.
 * @details Copies ``bytes`` data asynchronously from the memory area pointed
 *          to by ``src`` to the memory area pointed to by ``offset`` bytes from
 *          the start of ``symbol``. The memory areas cannot overlap.
 * @param[in] symbol Symbol address on the host.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] offset Offset from start of symbol in bytes.
 * @param[in] dir Data copy direction. It must be ::cnrtMemcpyHostToDev,
 *            ::cnrtMemcpyPeerToPeer , ::cnrtMemcpyDevToDev or ::cnrtMemcpyNoDirection.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorInvalidSymbol
 * @note
 * - This API is used to copy data asynchronously. To copy data synchronously,
 *   call ::cnrtMemcpyToSymbol.
 * @example
 * - None.
 */
cnrtRet_t cnrtMemcpyToSymbolAsync(const void *symbol, const void *src,
                                  size_t bytes, size_t offset,
                                  cnrtMemTransDir_t dir, cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Copies data asynchronously to the given symbol on the device.
 * @details Copies ``bytes`` data asynchronously from the memory area pointed
 *          to by ``src`` to the memory area pointed to by ``offset`` bytes from
 *          the start of ``symbol``. The memory areas cannot overlap.
 * @param[in] symbol Symbol address on the host.
 * @param[in] src Pointer to the source address.
 * @param[in] bytes The memory size to be copied in bytes.
 * @param[in] offset Offset from start of symbol in bytes.
 * @param[in] dir Data copy direction. It must be ::cnrtMemcpyHostToDev,
 *            ::cnrtMemcpyPeerToPeer , ::cnrtMemcpyDevToDev or ::cnrtMemcpyNoDirection.
 * @param[in] queue The queue handle created by calling ::cnrtQueueCreate.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorMemcpyDirectionInvalid,
 * ::cnrtErrorInvalidSymbol
 * @note
 * - This API exhibits asynchronous behavior for most use cases.
 * @example
 * - None.
 */
cnrtRet_t cnrtMemcpyToSymbolAsync_V2(const void *symbol, const void *src,
                                     size_t bytes, size_t offset,
                                     cnrtMemTransDir_t dir, cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Gets the free and total device memory.
 * @details Returns in ``total`` the total amount of memory available to the
 *          current device. Returns in ``free`` the amount of memory on the
 *          device that is free according to the OS.
 * @param[out] free Pointer to the free memory in bytes.
 * @param[out] total Pointer to the total memory in bytes.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - It is not guaranteed that all of the memory that OS reports as free can be
 *   allocated by user.
 * @example
 * @code{.cpp}
    int main () {
      size_t available, total;
      cnrtMemGetInfo(&available, &total);
      printf("free: %#lx, total: %#lx.\n", available, total);
      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMemGetInfo(size_t *free, size_t *total);
//Group: Memory Management
/**
 * @brief Initializes or sets device memory to a value.
 * @details Synchronously fills the first ``bytes`` of the memory area pointed
 *          to by ``ptr`` with constant byte value ``value``.
 * @param[in] ptr Pointer to the device address to be set.
 * @param[in] value The value to set.
 * @param[in] bytes Size in bytes to set.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API is used to set data synchronously. To set data asynchronously,
 *   call ::cnrtMemsetAsync.
 * @example
 * - See example in ::cnrtMalloc API.
 */
cnrtRet_t cnrtMemset(void *ptr, int value, size_t bytes);
//Group: Memory Management
/**
 * @brief Initializes or sets device memory to a value.
 * @details Asynchronously fills the first ``bytes`` of the memory area pointed
 *          to by ``ptr`` with constant byte value ``value``.
 * @param[in] ptr Pointer to the device address to be set.
 * @param[in] value The value to set.
 * @param[in] bytes Size in bytes to set.
 * @param[in] queue The queue.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API is used to set data asynchronously. To set data synchronously,
 *   call ::cnrtMemset.
 * - The ``value`` is set for each byte.
 * @example
 * - See example in ::cnrtMemcpyPeerAsync API.
 */
cnrtRet_t cnrtMemsetAsync(void *ptr, int value, size_t bytes,
                          cnrtQueue_t queue);
//Group: Memory Management
/**
 * @brief Gets the attributes of a specified pointer.
 * @details Gets attributes of ``ptr`` in ``attr``. See "Cambricon BANG C/C++ Programming Guide" for details.
 * @param[out] attr Pointer to ::cnrtPointerAttributes_t.
 * @param[in] ptr A specified pointer.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorNotSupport,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * @code{.cpp}
    int main () {
      cnrtPointerAttributes_t attributes;
      size_t size = sizeof(size_t) * N;
      void *dev_addr = NULL, *cpu_addr = NULL;

      cnrtMalloc(&dev_addr, size);
      cnrtMmap(dev_addr, &cpu_addr, size);

      // UVA without offset
      cnrtPointerGetAttributes(&attributes, cpu_addr);
      if (attributes.devicePointer != dev_addr) {
        printf("cnrtPointerGetAttributes failed.\n");
        return -1;
      }

      // UVA with offset
      cnrtPointerGetAttributes(&attributes, (void *)((unsigned long)cpu_addr + 0x100));
      if (attributes.devicePointer != (void *)((unsigned long)dev_addr + 0x100)) {
        printf("cnrtPointerGetAttributes failed.\n");
        return -1;
      }

      cnrtMunmap(cpu_addr, size);
      cnrtFree(dev_addr);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtPointerGetAttributes(cnrtPointerAttributes_t *attr,
                                   const void *ptr);

/************************************************************************
 * Queue Ordered Memory Allocator
 ************************************************************************/
//Group: Queue Ordered Memory Allocator
/**
 * @brief Allocates memory following the order determined by the queue.
 * @details  Places a memory allocation operation in ``queue``.
 * A pointer to the allocated memory returns in ``pPtr`` right away,
 * but access to the allocated memory is restricted until the allocation operation is finished. 
 * The allocation comes from the memory pool reserved for the queue on the device.
 * @param[out] pPtr The allocated MLU memory.
 * @param[in] byteSize The number of bytes to allocate.
 * @param[in] queue The handle of a queue.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - The memory pool of an MLU contains device memory from the MLU by default.
 * - The subsequent tasks submitted into the same queue can use the allocation according to the basic queue ordering.
 * - Use queue query, queue synchronization, and notifier to guarantee that the allocation
 *   operation is completed before task submitted in a separate queue runs.
 *
 * */
cnrtRet_t cnrtMemAllocAsync(void **pPtr, size_t byteSize, cnrtQueue_t queue);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Allocates memory from a specified pool following the order determined by the queue.
 * @details Places a memory allocation operation in ``queue``.
 * A pointer to the allocated memory returns in ``pPtr`` right away,
 * but access to the allocated memory is restricted until the allocation operation is finished. 
 * The allocation comes from the memory pool reserved for the queue on the device.
 * @param[out] pPtr The allocated MLU memory.
 * @param[in] byteSize The number of bytes to allocate.
 * @param[in] pool The memory pool from which to allocate the memory.
 * @param[in] hqueue The queue determining the order of queue.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - The specified memory pool might be from a device different than that of the specified @p hqueue.
 * - The subsequent tasks submitted into the same queue can use the allocation according to the basic queue ordering.
 * - Use queue query, queue synchronization, and notifier to guarantee that the allocation
 *   operation is completed before task submitted in a separate queue runs.
 *
 * */
cnrtRet_t cnrtMemAllocFromPoolAsync(void **pPtr, size_t byteSize,
                                    cnrtMemPool_t pool, cnrtQueue_t hqueue);

//Group: Queue Ordered Memory Allocator
/**
 * @brief  Frees memory following the order determined by the queue.
 * @details Places a free operation into ``hqueue``.
 * Access to the memory must cease once the queue reaches this free operation.
 * @param[out] mluAddr The memory to free.
 * @param[in] hqueue The queue establishing the queue ordering contract.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - After calling ::cnrtMemFreeAsync, any access to the memory, 
 *   on the device or querying its pointer attributes from subsequent tasks 
 *   causes unexpected behavior.
 *
 * */
cnrtRet_t cnrtMemFreeAsync(void *mluAddr, cnrtQueue_t hqueue);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Establishes a memory pool.
 * @details Allocates resources and establishes a memory pool.
 *          The properties of the pool, for example, the backing and IPC capabilities, is defined by ``poolProps``. 
 *          The handle to this pool is stored in ``pool``. 
 *        
 * @param[out] pool The memory pool.
 * @param[in] poolProps The properties of allocations made from the pool.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall  
 * @note
 * - The memory pool created with  CN_MEM_HANDLE_TYPE_NONE does not 
 *   support memory exporting.
 * - The memory of the pool is accessible from the device on which it is allocated, by default.
 * - The maximum size of the pool can be controlled by specifying a non-zero value for
 *   ::cnrtMemPoolProps::maxSize. If set to 0, the maximum size of the pool defaults to a
 *   system-dependent value.
 * */
cnrtRet_t cnrtMemPoolCreate(cnrtMemPool_t *pool,
                            const cnrtMemPoolProps_t *poolProps);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Releases the reserved memory pool.
 * @details When invoked, this function immediately returns if there are any pointers from the pool 
 * that have not  been freed or if there are pending free operations.
 * The associated resources will automatically be released once all outstanding allocations are resolved. 
 * @param[in] pool The memory pool.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - The default memory pool of a device cannot be destroyed.
 * - Destroying a device's current memory pool automatically 
 *   sets that device's default memory pool as its new current memory pool.
 * */
cnrtRet_t cnrtMemPoolDestroy(cnrtMemPool_t pool);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Determines how accessible a memory is from a specific device.
 * @details Returns the degree of accessibility of a memory pool from a device.
 * @param[out] flags The accessibility of a memory pool from the specified location.
 * @param[in] pool The memory pool being accessed.
 * @param[in] location The location trying to access the pool.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 *
 * */
cnrtRet_t cnrtMemPoolGetAccess(cnrtMemoryAccessFlags_t *flags,
                               cnrtMemPool_t pool, cnrtMemLocation_t *location);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Retrieves attributes of a memory pool.
 * @details
  * Here is a list of supported attributes:
 *
 * - ::cnrtMemPoolAttrReleaseThreshold: The type of value is ``cn_uint64_t``. This attribute defines the minimum 
 *   amount of free memory (in
 *   bytes) that the pool maintains before attempting to release memory back to the operating system.
 *   When the amount of free memory of the memory pool falls below threshold,
 *   the allocator will attempt to release memory
 *   back to the operating system during the next queue, notifier or context synchronize. The default value is
 *   0.
 * - ::cnrtMemPoolAttrReuseFollowNotifierDependencies: The type of value is ``int``. This attribute enables 
 *   ::cnrtMemAllocAsync to reuse memory asynchronously freed in another queue, provided there's a queue ordering
 *   dependency between the allocating queue and the free action. The interaction between notifiers and a null queue
 *   can establish the necessary queue ordered dependencies. This attribute is enabled by default.
 * - ::cnrtMemPoolAttrReuseAllowOpportunistic: The type of value is ``int``. This attribute allows reuse memory from already
 *   completed frees as long as there is no dependency between the free and allocation. This attribute is enabled by default.
 * - ::cnrtMemPoolAttrReuseAllowInternalDependencies: The type of value is ``int``. This attribute allows
 *   the queue to reuse memory internally by analyzing and inserting new dependency relationships. 
 *   For example, when the dependency established, ::cnrtMemAllocAsync can reuse the memory released by ::cnrtMemFreeAsync. 
 *   This attribute is enabled by default.
 * - ::cnrtMemPoolAttrReservedMemCurrent: The type of value is ``cn_uint64_t``. This attribute indicates the current amount of backing memory
 *   allocated for the memory pool.
 * - ::cnMemPoolAttrReservedMemHigh: The type of value is ``cn_uint64_t``. This attribute indicates the maximum amount of 
 *   backing memory allocated for the memory pool since the last time it was reset.
 * - ::cnrtMemPoolAttrUsedMemCurrent: The type of value is ``cn_uint64_t``. This attribute indicates the current amount of memory 
 *   from the memory pool being used by the application.
 * - ::cnrtMemPoolAttrUsedMemHigh: The type of value is ``cn_uint64_t``. This attribute indicates the maximum amount of
 *   memory from the memory pool that was in use by the application.
 * @param[in] pool The memory pool to get attributes of.
 * @param[in] attr The attribute to retrieve from the memory pool.
 * @param[out] value The returned attribute value.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 *
 * */
cnrtRet_t cnrtMemPoolGetAttribute(cnrtMemPool_t pool, cnrtMemPoolAttr_t attr,
                                  void *value);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Sets attributes of a memory pool.
 * @details
 * Here is a list of supported attributes:
 *
 * - ::cnrtMemPoolAttrReleaseThreshold: The type of value is ``cn_uint64_t``. This attribute defines the minimum 
 *   amount of free memory (in
 *   bytes) that the pool maintains before attempting to release memory back to the operating system.
 *   When the amount of free memory of the memory pool falls below threshold,
 *   the allocator will attempt to release memory
 *   back to the operating system during the next queue, notifier or context synchronize. The default value is
 *   0.
 * - ::cnrtMemPoolAttrReuseFollowNotifierDependencies: The type of value is ``int``. This attribute enables 
 *   ::cnrtMemAllocAsync to reuse memory asynchronously freed in another queue, provided there's a queue ordering
 *   dependency between the allocating queue and the free action. The interaction between notifiers and a null queue
 *   can establish the necessary queue ordered dependencies. This attribute is enabled by default.
 * - ::cnrtMemPoolAttrReuseAllowOpportunistic: The type of value is ``int``. This attribute allows reuse memory from already
 *   completed frees as long as there is no dependency between the free and allocation. This attribute is enabled by default.
 * - ::cnrtMemPoolAttrReuseAllowInternalDependencies: The type of value is ``int``. This attribute allows
 *   the queue to reuse memory internally by analyzing and inserting new dependency relationships. 
 *   For example, when the dependency established, ::cnrtMemAllocAsync can reuse the memory released by ::cnrtMemFreeAsync. 
 *   This attribute is enabled by default.
 * - ::cnrtMemPoolAttrReservedMemCurrent: The type of value is ``cn_uint64_t``. This attribute indicates the current amount of backing memory
 *   allocated for the memory pool.
 * - ::cnMemPoolAttrReservedMemHigh: The type of value is ``cn_uint64_t``. This attribute indicates the maximum amount of 
 *   backing memory allocated for the memory pool since the last time it was reset.
 * - ::cnrtMemPoolAttrUsedMemCurrent: The type of value is ``cn_uint64_t``. This attribute indicates the current amount of memory 
 *   from the memory pool being used by the application.
 * - ::cnrtMemPoolAttrUsedMemHigh: The type of value is ``cn_uint64_t``. This attribute indicates the maximum amount of
 *   memory from the memory pool that was in use by the application.
 *
 * @param[in] pool The memory pool to set its attributes.
 * @param[in] attr The attribute to be set. 
 * @param[in] value The pointer to the value to assign.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 *
 * */
cnrtRet_t cnrtMemPoolSetAttribute(cnrtMemPool_t pool, cnrtMemPoolAttr_t attr,
                                  void *value);

//Group: Queue Ordered Memory Allocator
/**
 * @brief Releases memory back to the operating system.
 * @details Releases memory back to the operating system as long as the bytes in the pool are less than
 *           ``minByteToKeep`` reserved bytes. This function cannot release memory that is backing any 
 *           outstanding asynchronous allocations. The operating system allocations may happen at
 *           different granularity from the user allocations.
 * @param[in] pool The memory pool to trim.
 * @param[in] minByteToKeep The minimum number of bytes to keep by memory pool.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - If the pool has less than ``minByteToKeep`` bytes reserved, ::cnrtMemPoolTrimTo is no-op.
 *   Otherwise the pool will be guaranteed to have at least ``minByteToKeep`` bytes reserved
 *   after the operation.
 * - Allocations that have not been freed are considered outstanding.
 * - Allocations that have been asynchronously freed but whose completion has not been observed on
 *   the host(e.g. by a synchronize) can be considered outstanding.
 *
 * */
cnrtRet_t cnrtMemPoolTrimTo(cnrtMemPool_t pool, size_t minByteToKeep);

/************************************************************************
 * Notify management
 ************************************************************************/
//Group: Notifier Management
/**
 * @brief Creates a notifier for the current device.
 * @details Returns a pointer
 *          ``notifier`` to the new notifier. For more information about notifier,
 *          see "Cambricon BANG C/C++ Programming Guide".
 * @param[out] pNotifier Pointer to the new notifier.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtNotifierDestroy to release the notifier memory;
 *   otherwise the memory leaks may occur.
 * - To learn more about queue life cycle management, see
 *   "Cambricon CNDrv Developer Guide".
 * @example
 * - See example in ::cnrtNotifierElapsedTime.
 */
cnrtRet_t cnrtNotifierCreate(cnrtNotifier_t *pNotifier);
//Group: Notifier Management
/**
 * @brief Creates a notifier with flags for the current device.
 * @details Returns a pointer
 *          ``notifier`` to the new notifier. The flags that can be specified include
 *          ::CNRT_NOTIFIER_DEFAULT,
 *          ::CNRT_NOTIFIER_DISABLE_TIMING_SW,
 *          ::CNRT_NOTIFIER_DISABLE_TIMING_ALL,
 *          ::CNRT_NOTIFIER_INTERPROCESS.
 *          For more information about notifier, see "Cambricon BANG C/C++ Programming Guide".
 *
 * @param[out] pNotifier Pointer to the new notifier.
 * @param[in]   flags notifier creation flags.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtNotifierDestroy to release the notifier memory;
 *   otherwise the memory leaks may occur.
 * - To learn more about queue life cycle management, see
 *   "Cambricon CNDrv Developer Guide".
 * @example
 * - See example in ::cnrtNotifierElapsedTime.
 */
cnrtRet_t cnrtNotifierCreateWithFlags(cnrtNotifier_t *pNotifier,
                                      unsigned int flags);
//Group: Notifier Management
/**
 * @brief Destroys a notifier that is created by ::cnrtNotifierCreate.
 * @details Destroys a notifier pointed by ``notifier``. For more information about notifier,
 *          see "Cambricon BANG C/C++ Programming Guide".
 * @param[in] notifier The notifier that is created by ::cnrtNotifierCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - If a notifier is placed into a queue, but it is not completed when the ::cnrtNotifierDestroy
 *   API is called, the ::cnrtNotifierDestroy API will return immediately; but
 *   the resources associated with this notifier will be released automatically only after
 *   the notifier is completed.
 * @example
 * - See example in ::cnrtNotifierElapsedTime.
 *
 *
 */
cnrtRet_t cnrtNotifierDestroy(cnrtNotifier_t notifier);
//Group: Notifier Management
/**
 * @brief Computes the elapsed time between two notifiers.
 * @details Each notifier records a global timestamp when it is executed.
 *          The elapsed time is the duration between the timestamps of ``end`` and ``start``.
 *          If either notifier is created with ::CNRT_NOTIFIER_DISABLE_TIMING_SW or
 *          ::CNRT_NOTIFIER_DISABLE_TIMING_ALL flag, this function returns
 *          ::cnrtErrorArgsInvalid.
 *
 * @param[in] start The handle of the start notifier created by the
 *                  ::cnrtNotifierCreate API.
 * @param[in] end The handle of end notifier created by the ::cnrtNotifierCreate
 *                API.
 * @param[out] ms The elapsed time between the ``start`` and ``end`` in ms.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorBusy,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtPlaceNotifier or ::cnrtPlaceNotifierWithFlags to place the ``start`` and ``end``
 *   notifiers into the queue respectively first.
 *   Otherwise, ::cnrtErrorCndrvFuncCall is returned.
 * - If the ::cnrtNotifierElapsedTime API is called before the ``start``
 *   or ``end`` notifier is placed into the queue, ::cnrtErrorCndrvFuncCall is
 *   returned.
 * @example
 * @code{.cpp}
    __mlu_global__ kernel() {
      printf("%s\n", __func__);
    }

    int main () {
      int ret;

      // Create notifier
      cnrtNotifier_t notifier_s;
      cnrtNotifier_t notifier_e;
      cnrtNotifierCreate(&notifier_s);
      cnrtNotifierCreate(&notifier_e);

      // Create queue
      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);

      cnrtDim3_t dim = {1, 1, 1};
      // Place notifier into a queue
      cnrtPlaceNotifier(notifier_s, queue);
      kernel<<<dim, cnrtFuncTypeBlock, queue>>>();
      cnrtPlaceNotifier(notifier_e, queue);

      cnrtQueueSync(queue);

      // Query notifier
      cnrtQueryNotifier(notifier_s);
      cnrtQueryNotifier(notifier_e);

      // Wait for notifier
      cnrtWaitNotifier(notifier_s);
      cnrtWaitNotifier(notifier_e);

      // Compute the software elapsed time
      float ms;
      cnrtNotifierElapsedTime(notifier_s, notifier_e, &ms);
      printf("software time consuming between the two notifier is %f\n", ms);

      cnrtNotifierDestroy(notifier_s);
      cnrtNotifierDestroy(notifier_e);

      cnrtQueueDestroy(queue);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtNotifierElapsedTime(cnrtNotifier_t start, cnrtNotifier_t end,
                                  float *ms);
//Group: Notifier Management
/**
 * @brief Queries the status of notifier in a queue.
 * @details Returns ::cnrtSuccess if the ``notifier`` in the queue is completed.
 *          Returns ::cnrtErrorBusy if the ``notifier`` is still executing.
 * @param[in] notifier The handle of the notifier to query.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorBusy,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - When querying the status of the notifier, if you call ::cnrtPlaceNotifier again
 *   on the same notifier, the query will be based on the most recent call
 *   to ::cnrtPlaceNotifier. The result of the previous query will be overwritten.
 * @example
 * - See example in ::cnrtNotifierElapsedTime API.
 */
cnrtRet_t cnrtQueryNotifier(cnrtNotifier_t notifier);
//Group: Notifier Management
/**
 * @brief Places a notifier into a specified ``queue``.
 * @details The notifier can be
 *          used to measure the execution time of all the tasks.
 * @param[in] notifier The handle of the notifier to be placed into the queue.
 *                     Create the notifier by calling ::cnrtNotifierCreate.
 * @param[in] queue The queue in which the notifier is placed. Create
 *                  the queue by calling ::cnrtQueueCreate.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtNotifierElapsedTime.
 */
cnrtRet_t cnrtPlaceNotifier(cnrtNotifier_t notifier, cnrtQueue_t queue);
//Group: Notifier Management
/**
 * @brief Places a notifier into a specified ``queue`` with flags.
 * @details The notifier can be used to measure the execution time of all the tasks.
 * @param[in] notifier The handle of the notifier to be placed into the queue.
 *                     Create the notifier by calling ::cnrtNotifierCreate.
 * @param[in] queue The queue in which the notifier is placed. Create
 *                  the queue by calling ::cnrtQueueCreate.
 * @param[in] flags See ::cnrtNotifierPlaceFlags_t.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtNotifierElapsedTime.
 */
cnrtRet_t cnrtPlaceNotifierWithFlags(cnrtNotifier_t notifier, cnrtQueue_t queue,
                                     unsigned int flags);
//Group: Notifier Management
/**
 * @brief Waits for a notifier to be completed.
 * @details Waits for a ``notifier`` in the queue to be completed before executing all
 *          future tasks in this queue. The ``notifier`` is the most recent one called by
 *          the ::cnrtPlaceNotifier or ::cnrtPlaceNotifierWithFlags API in the queue. Returns ::cnrtSuccess
 *          if the notifier is completed.
 * @param[in] notifier The handle of the notifier to be waited for.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - If ::cnrtPlaceNotifier has not been called on the notifier before
 *   calling this API, ::cnrtSuccess will be returned.
 * - This API is used for synchronization in a single queue. To synchronize across
 *   queues, use ::cnrtQueueWaitNotifier.
 * @example
 * - See example in ::cnrtNotifierElapsedTime.
 */
cnrtRet_t cnrtWaitNotifier(cnrtNotifier_t notifier);

/************************************************************************
 * Queue management
 ************************************************************************/
//Group: Queue Management
/**
 * @brief Creates a queue.
 * @details Creates a queue on the current device, and returns a pointer
 * ``pQueue`` to the new queue.
 * Define how the queues are synchronized with ::cnrtSetDeviceFlag.
 * By default ::cnrtDeviceScheduleSpin is used. Call
 * ::cnrtGetDeviceFlag to query the current behavior.
 *
 * - ::cnrtDeviceScheduleSpin: CPU actively spins when waiting for the device
 *   execution result. For this option, the latency might be lower, but it may
 *   decrease the performance of CPU threads if the tasks are executed in parallel
 *   with MLU. This value is used by default.
 * - ::cnrtDeviceScheduleBlock: CPU thread is blocked on a synchronization
 *   primitive when waiting for the device execution result.
 * - ::cnrtDeviceScheduleYield: CPU thread yields when waiting for the device
 *   execution results. For this option, the latency might be higher, but it can
 *   increase the performance of CPU threads if the tasks are executed in parallel
 *   with the device.
 *
 * @param[out] pQueue Pointer to the new queue.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Call ::cnrtQueueDestroy to release the queue resources;
 *   otherwise, the memory leaks may occur.
 * - To learn more about queue lifecycle management, please see
 *   "Cambricon CNDrv Developer Guide".
 * @example
 * - See example in ::cnrtQueueSync.
 */
cnrtRet_t cnrtQueueCreate(cnrtQueue_t *pQueue);
//Group: Queue Management
/**
 * @brief Creates a queue with the specified priority.
 * @details Creates a queue on the current device with the specified
 *          priority, and returns a pointer ``pQueue`` to the new queue.
 *          If you want to define how the queues are synchronized with
 *          ::cnrtSetDeviceFlag, see description in ::cnrtQueueCreate.
 * @param[out] pQueue Pointer to the new queue.
 * @param[in] flags Flag used in this operation, which is reserved for further use.
 *            It is recommended to set this parameter to 0.
 * @param[in] priority Priority of the queue. Lower numbers represent higher priorities.
 *            See ::cnrtDeviceGetQueuePriorityRange for more information about
 *            the meaningful queue priorities that can be passed.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - ::cnrtDeviceGetQueuePriorityRange can be called to query the range of
 *   meaningful numerical priorities. If the specified priority is out of the
 *   legal range returned by ::cnrtDeviceGetQueuePriorityRange, it will automatically
 *   be clamped to the lowest or the highest number in the legal range.
 * - Call ::cnrtQueueDestroy to release the queue resources;
 *   otherwise, the memory leaks may occur.
 * @example
 * @code{.cpp}
    int main () {
      void *dev_mem0 = NULL;
      void *dev_mem1 = NULL;
      cnrtMalloc(&dev_mem0, size);
      cnrtMalloc(&dev_mem1, size);

      int least_priority;
      int greatest_priority;

      cnrtDeviceGetQueuePriorityRange(&least_priority, &greatest_priority);

      // Create a queue
      cnrtQueue_t queue;
      cnrtQueueCreateWithPriority(&queue, 0, greatest_priority);

      int priority;
      cnrtQueueGetPriority(queue, &priority);
      printf("The priority of the queue is %d\n", priority);

      // Allocate memory on device and host
      size_t size = 0x1000000;
      char *host_mem0 = NULL;
      char *host_mem1 = NULL;
      cnrtHostMalloc((void **)&host_mem0, size);
      cnrtHostMalloc((void **)&host_mem1, size);

      // Copy data asynchronously in two queues
      cnrtMemcpyAsync_V3(dev_mem0, host_mem0, size, queue, cnrtMemcpyHostToDev);
      cnrtMemcpyAsync_V3(host_mem1, dev_mem1, size, queue, cnrtMemcpyDevToHost);

      // Query the status of a queue.
      printf("before sync queue, %d\n", cnrtQueueQuery(queue));
      cnrtQueueSync(queue);
      printf("after sync queue, %d\n", cnrtQueueQuery(queue));

      // Release resources
      cnrtQueueDestroy(queue);

      cnrtFree(dev_mem0);
      cnrtFree(dev_mem1);

      cnrtFreeHost(host_mem0);
      cnrtFreeHost(host_mem1);

      return 0;
    }
 * @endcode
 */

cnrtRet_t cnrtQueueCreateWithPriority(cnrtQueue_t *pQueue, unsigned int flags,
                                      int priority);
//Group: Queue Management
/**
 * @brief Queries the priority of a queue.
 * @details Queries the priority of a queue created using ::cnrtQueueCreate or
 *          ::cnrtQueueCreateWithPriority and returns the priority in ``priority``.
 * @param[in]  queue The queue to query.
 * @param[out] priority Pointer to a signed integer in which the queue's priority is returned.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall,
 * ::cnrtErrorSysNoMem
 * @note
 * - If the queue is created with a priority outside the numerical range returned by
 *   ::cnrtDeviceGetQueuePriorityRange, this API will return the clamped priority.
 * @example
 * - See example in ::cnrtQueueSync.
 */
cnrtRet_t cnrtQueueGetPriority(cnrtQueue_t queue, int *priority);
//Group: Queue Management
/**
 * @brief Destroys a queue.
 * @details Destroys a ``queue`` that is created by ::cnrtQueueCreate.
 *          If the queue is still executing operations when ::cnrtQueueDestroy
 *          is called, this API will return immediately, but the resources associated with
 *          the queue are released automatically after all the operations in the queue are
 *          completed.
 * @param[in] queue The queue to be destroyed.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtQueueSync.
 */
cnrtRet_t cnrtQueueDestroy(cnrtQueue_t queue);
//Group: Queue Management
/**
 * @brief Queries if a queue is completed.
 * @details Returns the status of the ``queue``. If all the operations in the
 *          queue are completed, ::cnrtSuccess is returned. If the operations in
 *          the queue are still being executed, ::cnrtErrorBusy is returned.
 * @param[in] queue The queue to query.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorBusy,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - If ``queue`` is set to NULL, the default queue will be used.
 * @example
 * @code{.cpp}
    int main () {
      // Create a queue
      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);

      // Allocate memory on device and host
      size_t size = 0x1000000;
      char *host_mem0 = NULL;
      char *host_mem1 = NULL;
      host_mem0 = (char *)malloc(size);
      host_mem1 = (char *)malloc(size);
      void *dev_mem0 = NULL;
      void *dev_mem1 = NULL;
      cnrtMalloc(&dev_mem0, size);
      cnrtMalloc(&dev_mem1, size);

      // Copy data asynchronously in two queues
      cnrtMemcpyAsync_V3(dev_mem0, host_mem0, size, queue, cnrtMemcpyHostToDev);
      cnrtMemcpyAsync_V3(host_mem1, dev_mem1, size, queue, cnrtMemcpyDevToHost);

      // Query the status of a queue.
      printf("before sync queue, %d\n", cnrtQueueQuery(queue));
      cnrtQueueSync(queue);
      printf("after sync queue, %d\n", cnrtQueueQuery(queue));

      // Release resources
      cnrtQueueDestroy(queue);

      cnrtFree(dev_mem0);
      cnrtFree(dev_mem1);

      cnrtFreeHost(host_mem0);
      cnrtFreeHost(host_mem1);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtQueueQuery(cnrtQueue_t queue);
//Group: Queue Management
/**
 * @brief Waits for queue operations to be completed.
 * @details Blocks further executions in CPU thread until all the tasks
 *          in the queue on the current MLU device are completed.
 * @param[in] queue The queue to be waited for.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - If ``queue`` is set to NULL, the default queue will be used.
 * - This API may also return ::cnrtErrorQueue from previous and asynchronous
 *   operations.
 * @example
 * @code{.cpp}
    int main () {
      // Create a notifier
      cnrtNotifier_t notifier;
      cnrtNotifierCreate(&notifier);

      // Create a queue
      cnrtQueue_t queue_0;
      cnrtQueue_t queue_1;
      cnrtQueueCreate(&queue_0);
      cnrtQueueCreate(&queue_1);

      // Allocate memory on device and host
      size_t size = 0x100000;
      char *host_mem0 = NULL;
      char *host_mem1 = NULL;
      cnrtHostMalloc((void **)&host_mem0, size);
      cnrtHostMalloc((void **)&host_mem1, size);
      void *dev_mem0 = NULL;
      void *dev_mem1 = NULL;
      cnrtMalloc(&dev_mem0, size);
      cnrtMalloc(&dev_mem1, size);

      // Copy data asynchronously in two queues
      cnrtMemcpyAsync_V3(dev_mem0, host_mem0, size, queue_0, cnrtMemcpyHostToDev);
      cnrtMemcpyAsync_V3(host_mem1, dev_mem1, size, queue_1, cnrtMemcpyDevToHost);
      // Put a notifier into a queue
      cnrtPlaceNotifier(notifier, queue_0);

      // Synchronize two queues
      cnrtQueueWaitNotifier(notifier, queue_1, 0);

      // Wait until all tasks are completed
      cnrtQueueSync(queue_0);
      cnrtQueueSync(queue_1);

      // Release resources
      cnrtQueueDestroy(queue_0);
      cnrtQueueDestroy(queue_1);
      cnrtNotifierDestroy(notifier);

      cnrtFree(dev_mem0);
      cnrtFree(dev_mem1);
      cnrtFreeHost(host_mem0);
      cnrtFreeHost(host_mem1);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtQueueSync(cnrtQueue_t queue);
//Group: Queue Management
/**
 * @brief Waits all the preceding tasks before a notifier to be completed before
 *        the specified queue does any further executions.
 * @details Blocks all future tasks in the ``queue`` until all the preceding tasks
 *          before a notifier are completed. The queue only waits for the completion
 *          of the most recent host call of ::cnrtPlaceNotifier or ::cnrtPlaceNotifierWithFlags on the notifier.
 *          This API is used to synchronize the queue efficiently on the device.
 * @param[in] notifier The notifier to be waited for.
 * @param[in] queue The queue to wait.
 * @param[in] flag See ::cnrtNotifierWaitFlags_t.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - If ``queue``is set to NULL, the default queue will be used.
 * - This API is used for synchronization across queues. To synchronize in
 *   a single queue, use ::cnrtWaitNotifier.
 * @example
 * - See example in ::cnrtQueueSync.
 */
cnrtRet_t cnrtQueueWaitNotifier(cnrtNotifier_t notifier, cnrtQueue_t queue,
                                unsigned int flag);

// Group: Queue Management
/**
 * @brief Sets the queue attribute.
 *
 * @details Sets the attributes corresponding to ``attr_id`` for ``queue`` from corresponding
 *          attribute of ``value``.
 *
 * @param[in] queue    The queue handle to be set.
 * @param[in] attr_id  The attribute ID.
 * @param[in] value    The attribute value to set.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNotSupport,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 *
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtQueueSetAttribute(cnrtQueue_t queue, cnrtQueueAttrID_t attr_id,
                                const cnrtQueueAttrValue_t *value);

// Group: Queue Management
/**
 * @brief Queries the queue attribute.
 *
 * @details Queries the attributes corresponding to ``attr_id`` for ``queue``, and stores it in corresponding
 *          member of ``value``.
 *
 * @param[in] queue      The queue handle to query.
 * @param[in] attr_id    The attribute ID.
 * @param[out] value_out The room to store the attribute value.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorNotSupport,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 *
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtQueueGetAttribute(cnrtQueue_t queue, cnrtQueueAttrID_t attr_id,
                                cnrtQueueAttrValue_t *value_out);

// Group: Queue Management
/**
 * @brief Copies queue attributes from source queue to destination queue.
 *
 * @details Copies attributes from source queue ``src`` to destination queue ``dst``.
 *
 * @param[in] dst   The destination queue.
 * @param[in] src   The source queue.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 *
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtQueueCopyAttributes(cnrtQueue_t dst, cnrtQueue_t src);

// Group: Queue Management
/**
 * @brief Begins capture on a queue.
 *
 * @details When the ``queue`` is capturing, all the tasks pushed to this queue will not be executed
 *          but instead captured into a TaskTopo.
 *          Call ::cnrtQueueIsCapturing() to query whether the queue is capturing.
 *          Call ::cnrtQueueGetCaptureInfo() to query the unique ID representing the sequence number
 *          of the capturing and other information.
 *
 * @param[in] queue  The queue to begin capture for.
 * @param[in] mode   The capture mode.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorIllegalState
 *
 * @note
 * - If ``mode`` is not ::cnrtQueueCaptureModeRelaxed, ::cnrtQueueEndCapture() must be called on this
 *   ``queue`` from the same thread.
 * - It is not allowed to pass to ``queue`` with NULL as a default queue to begin capturing.
 * - The capture must be ended on the same queue as it is started.
 *
 *
 * @example
 * @code{.cpp}
   int main() {
     cnrtTaskTopo_t topo;
     cnrtTaskTopoEntity_t entity;
     uint64_t id; // the capture sequence unique id.
     const cnrtTaskTopoNode_t *dependencies;
     size_t numDependencies;
     cnrtQueue_t queue;
     cnrtQueueCaptureStatus_t status;
     cnrtTaskTopoNode_t node0, node1;
     cnrtTaskTopoNodeType_t type;

     cnrtRet_t ret = cnrtQueueCreate(&queue);
     if (ret != cnrtSuccess) return ret;

     ret = cnrtQueueBeginCapture(queue, cnrtQueueCaptureModeRelaxed);
     if (ret != cnrtSuccess) return ret;

     ret = cnrtQueueIsCapturing(queue, &status);
     if (ret != cnrtSuccess || status != cnrtQueueCaptureStatusActive) return ret;

     // Capture the kernel task.
     cnrtDim3_t dim = {1, 1, 1};
     kernel<<<dim, cnrtFuncTypeBlock, queue>>>();

     ret = cnrtQueueGetCaptureInfo(queue, &status, &id, &topo,
                                   &dependencies, &numDependencies);
     if (ret != cnrtSuccess) return ret;
     assert(numDependencies == 1);
     node0 = dependencies[0];
     ret = cnrtTaskTopoNodeGetType(dependencies[0], type);
     assert(ret == cnrtSuccess);
     assert(cnrtTaskTopoNodeTypeKernel == type);

     // Capture the host function task.
     cnrtInvokeHostFunc(queue, [](void *args) {//...}, nullptr);

     ret = cnrtQueueGetCaptureInfo(queue, &status, &id, &topo,
                                   &dependencies, &numDependencies);
     if (ret != cnrtSuccess) return ret;

     // Nodes in topo are {kernel_node -> host_node}
     node1 = dependencies[0];
     assert(status == cnrtQueueCaptureStatusActive);
     assert(numDependencies == 1);
     ret = cnrtTaskTopoNodeGetType(node1, &type);
     assert(ret == cnrtSuccess);
     assert(type == cnrtTaskTopoNodeTypeHost);

     cnrtTaskTopoNode_t updated_deps[2] = {node0, node1};
     ret = cnrtQueueUpdateCaptureDependencies(queue, updated_deps, 2,
                                              cnrtQueueSetCaptureDependencies);
     if (ret != cnrtSuccess) return ret;

     ret = cnrtQueueGetCaptureInfo(queue, &status, &id, &topo,
                                   &dependencies, &numDependencies);
     if (ret != cnrtSuccess) return ret;
     assert(numDependencies == 2);
     assert((dependencies[0] == node0 && dependencies[1] == node1) ||
            (dependencies[0] == node1 && dependencies[1] == node0));

     // Invoke more asynchronous tasks
     // ...

     ret = cnrtQueueEndCapture(queue, &topo);
     if (ret != cnrtSuccess) return ret;

     ret = cnrtTaskTopoInstantiate(&entity, topo, nullptr, nullptr, 0);
     if (ret != cnrtSuccess) return ret;

     // Invoke TaskTopo, real time to invoke tasks
     ret = cnrtTaskTopoEntityInvoke(entity, queue);
     if (ret != cnrtSuccess) return ret;

     // ...
   }
 * @endcode
 */
cnrtRet_t cnrtQueueBeginCapture(cnrtQueue_t queue,
                                enum cnrtQueueCaptureMode mode);

// Group: Queue Management
/**
 * @brief Ends a queue capture, and returns the captured TaskTopo.
 *
 * @details Ends a queue capture sequence that is begun with ::cnrtQueueBeginCapture() and returns
 *          the captured TaskTopo in ``pTaskTopo``.
 *          If the queue is not in capture status, the API call returns ::cnrtErrorIllegalState.
 *
 * @param[in] queue The queue in which to end capturing.
 * @param[out] pTaskTopo The captured TaskTopo.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorIllegalState,
 * ::cnrtErrorQueueCaptureInvalidated
 *
 * @note
 * - If the queue is not capturing under ::cnrtQueueCaptureModeRelaxed, ::cnrtQueueEndCapture
 *   must be called on this ``queue`` from the same thread.
 * - If the capture is invalidated, then the NULL TaskTopo will be returned.
 * - Capture must have been begun on ``queue`` via ::cnrtQueueBeginCapture.
 *
 *
 * @example
 * - See example in ::cnrtQueueBeginCapture().
 */
cnrtRet_t cnrtQueueEndCapture(cnrtQueue_t queue, cnrtTaskTopo_t *pTaskTopo);

// Group: Queue Management
/**
 * @brief Queries a queue's capture status.
 *
 * @details Returns the ``queue`` capture status via ``pStatus``.
 *          If the ``queue`` is not in capture status, ::cnrtQueueCaptureStatusNone is returned.
 *          If the ``queue`` is in capture status but the capture sequence has been invalidated due to
 *          previous error, ::cnrtQueueCaptureStatusInvalidated is returned; otherwise
 *          ::cnrtQueueCaptureStatusActive is returned.
 *
 * @param[in] queue    The queue to query.
 * @param[out] pStatus The capture status.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 *
 * @note
 * - None.
 *
 *
 * @example
 * - See example in ::cnrtQueueBeginCapture().
 */
cnrtRet_t cnrtQueueIsCapturing(cnrtQueue_t queue,
                               enum cnrtQueueCaptureStatus *pStatus);

// Group: Queue Management
/**
 * @brief Queries a queue's capture information.
 *
 * @details Returns detailed information if the ``queue`` is in active capture status.
 *          The parameters ``idOut``, ``pTaskTopo``, ``pDependenciesOut``, ``pNumDependencies``
 *          are optional, which can be NULL, and nothing is returned.
 *
 * @param[in] queue              The queue to query.
 * @param[out] pStatus           The capture status.
 * @param[out] idOut             The unique sequence number of current capturing.
 * @param[out] pTaskTopo         The current captured TaskTopo.
 * @param[out] pDependenciesOut  A pointer to store an array of dependency nodes.
 * @param[out] pNumDependencies  The count of dependency nodes.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 *
 * @note
 * - None.
 *
 * @example
 * - See example in ::cnrtQueueBeginCapture().
 */
cnrtRet_t cnrtQueueGetCaptureInfo(cnrtQueue_t queue,
                                  enum cnrtQueueCaptureStatus *pStatus,
                                  uint64_t *idOut, cnrtTaskTopo_t *pTaskTopo,
                                  const cnrtTaskTopoNode_t **pDependenciesOut,
                                  size_t *pNumDependencies);

// Group: Queue Management
/**
 * @brief Updates the set of dependencies in a capturing queue.
 *
 * @details Modifies the dependency set of capturing ``queue``. The dependency set is
 *          the set of nodes that the next captured node in the ``queue`` will depend on.
 *
 * @param[in] queue            The queue in capture status.
 * @param[in] dependencies     The array of dependency nodes to modify the capturing sequence dependencies.
 * @param[in] numDependencies  The node count of the ``dependencies``.
 * @param[in] flags            Modification flag. Valid flags are ::cnrtQueueAddCaptureDependencies and
 *                             ::cnrtQueueSetCaptureDependencies.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorIllegalState,
 * ::cnrtErrorQueueCaptureInvalidated,
 * ::cnrtErrorSysNoMem
 *
 * @note
 * - None.
 *
 * @example
 * - See example in ::cnrtQueueBeginCapture().
 */
cnrtRet_t cnrtQueueUpdateCaptureDependencies(cnrtQueue_t queue,
                                             cnrtTaskTopoNode_t *dependencies,
                                             size_t numDependencies,
                                             unsigned int flags);

// Group: Queue Management
/**
 * @brief Exchanges the queue capture mode of the calling thread.
 * @details This function swaps the current queue capture mode of the calling thread with the mode specified by @p *mode, and updates
 * the value at @p *mode to the thread's previous mode.
 * This swap mechanism is designed to be used in a push-pop fashion to ensure deterministic behavior across function or module
 * boundaries.
 *
 * @example
 * @code{.cpp}
 *  cnrtQueueCaptureMode_t mode = desiredMode;
 *  cnrtThreadExchangeQueueCaptureMode(&mode);
 *  ...
 *  cnrtThreadExchangeQueueCaptureMode(&mode);
 * @endcode
 *
 * @details During the process of queue capture, certain API calls such as
 * ::cnrtMalloc might be unsafe. These calls do not enqueue operations asynchronously to a queue,
 * and hence are not observed by queue capture. 
 * The sequence of operations captured via
 * ::cnrtQueueBeginCapture depended on a synchronized memory allocation, and during the launch of TaskTopo it is necessary to 
 * reallocate this memory,  
 * therefore, capturing TaskTopo is invalid.
 *
 * See ::cnrtQueueBeginCapture for details about the queue capture.
 *
 * The queue capture places restrictions on API calls that can be made within or concurrently
 * to a
 * ::cnrtQueueBeginCapture-::cnrtQueueEndCapture sequence. The behavior can be controlled via this API
 * and flag to ::cnrtQueueBeginCapture.
 *
 * Queue capture modes include: 
 * - ::cnrtQueueCaptureModeGlobal: This is the default mode. 
 *   In this mode, potentially unsafe API calls are prohibited if any thread (including the calling thread) has 
 *   an ongoing capture not initiated with ::cnrtQueueCaptureModeRelaxed. 
 * - ::cnrtQueueCaptureModeRelaxed: This mode allows the local thread to make potentially unsafe API
 * calls. Note that the thread is still prohibited from API calls which necessarily conflict with
 * queue capture.
 *
 * @param[in,out] mode Pointer to mode value to swap with the current mode.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorIllegalState,
 * ::cnrtErrorQueueCaptureInvalidated,
 * ::cnrtErrorSysNoMem
 *
 */
cnrtRet_t cnrtThreadExchangeQueueCaptureMode(cnrtQueueCaptureMode_t *mode);

//Group: Notifier Management
/**
 * @brief Computes the total kernel hardware execution time between two notifiers in one queue.
 *
 * @details Each notifier records a hardware timestamp when it is executed.
 *          The elapsed time is the hardware execution time difference between
 *          timesatmps of ``end`` and ``start``. If either notifier is created
 *          with ::CNRT_NOTIFIER_DISABLE_TIMING_ALL flag, this function returns
 *          ::cnrtErrorArgsInvalid.
 *
 * @param[in]  start The handle of the starting notifier created by the
 *                   ::cnrtNotifierCreate API.
 * @param[in]  end   The handle of ending notifier created by the ::cnrtNotifierCreate
 *                   API.
 * @param[out] us    The hardware time duration between the ``start`` and ``end`` of
 *                   notifiers in microsecond.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorBusy,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - Only kernel task guarantees that hardware execution time is recorded.
 * - Call ::cnrtPlaceNotifier or ::cnrtPlaceNotifierWithFlags to place the starting and ending
 *   notifiers into the same queue respectively first.
 *   Otherwise, ::cnrtErrorCndrvFuncCall is returned.
 * - If the ::cnrtNotifierDuration API is called before the starting notifier
 *   or ending notifier is placed into the queue, ::cnrtErrorCndrvFuncCall is
 *   returned.
 *
 *
 * @example
 * @code{.cpp}
    __mlu_global__ kernel() {
      printf("%s\n", __func__);
    }

    int main () {
      // Create notifier
      cnrtNotifier_t notifier_s;
      cnrtNotifier_t notifier_e;
      cnrtNotifierCreate(&notifier_s);
      cnrtNotifierCreate(&notifier_e);

      // Create queue
      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);

      cnrtDim3_t dim = {1, 1, 1};
      // Place notifier into a queue
      cnrtPlaceNotifier(notifier_s, queue);
      kernel<<<dim, cnrtFuncTypeBlock, queue>>>();
      cnrtPlaceNotifier(notifier_e, queue);

      cnrtQueueSync(queue);

      // Query notifier
      cnrtQueryNotifier(notifier_s);
      cnrtQueryNotifier(notifier_e);

      // Wait for notifier
      cnrtWaitNotifier(notifier_s);
      cnrtWaitNotifier(notifier_e);

      // Compute the hardware duration
      float us;
      cnrtNotifierDuration(notifier_s, notifier_e, &us);
      printf("hardware time consuming between the two notifier is %f\n", us);

      cnrtNotifierDestroy(notifier_s);
      cnrtNotifierDestroy(notifier_e);

      cnrtQueueDestroy(queue);

      return 0;
    }
 * @endcode
 *
 */
cnrtRet_t cnrtNotifierDuration(cnrtNotifier_t start, cnrtNotifier_t end,
                               float *us);

/************************************************************************
 * version management
 ************************************************************************/
//Group: Version Management
/**
 * @brief Retrieves the version of the current driver.
 * @details Returns the major version in ``major``, minor version in ``minor``,
 *          and patch version in ``patch`` of the current driver.
 * @param[out] major Pointer to the major of version.
 * @param[out] minor Pointer to the minor of version.
 * @param[out] patch Pointer to the patch of version.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * @code{.cpp}
    int main () {
      int major, minor, patch;
      cnrtDriverGetVersion(&major, &minor, &patch);
      printf("driver version is %d.%d.%d\n", major, minor, patch);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtDriverGetVersion(int *major, int *minor, int *patch);
//Group: Version Management
/**
 * @brief Retrieves the version of the current CNRT.
 * @details Returns the major version in ``major``, minor version in ``minor``,
 *          and patch version in ``patch`` of the current CNRT instance.
 * @param[out] major Pointer to the major of version.
 * @param[out] minor Pointer to the minor of version.
 * @param[out] patch Pointer to the patch of version.
 * @return
 * ::cnrtSuccess
 * @note
 * - None.

 * @example
 * @code{.cpp}
    int main () {
      int major, minor, patch;
      cnrtGetLibVersion(&major, &minor, &patch);
      printf("cnrt version is %d.%d.%d\n", major, minor, patch);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtGetLibVersion(int *major, int *minor, int *patch);

/************************************************************************
 * error management
 ************************************************************************/
//Group: Error Handling Management
/**
 * @brief Retrieves the error name of an error code.
 * @details Returns the string containing the name of an error code in the enum.
 * @param[in] error The error code to convert to string.
 * @return A pointer to string of the error code.
 * @note
 * - None.
 * @example
 * - See example in ::cnrtGetLastError API.
*/
const char *cnrtGetErrorName(cnrtRet_t error);
//Group: Error Handling Management
/**
 * @brief Retrieves the error message of an error code.
 * @details Returns the description string for an error code.
 * @param[in] error The error code to convert to string.
 * @return A pointer to string message according to the error code.
 * @note
 * - None.
 * @example
 * - See example in ::cnrtGetLastError API.
*/
const char *cnrtGetErrorStr(cnrtRet_t error);
//Group: Error Handling Management
/**
 * @brief Retrieves the last error from CNRT API call.
 * @details Returns the last error code returned from the CNRT API call in the
 *          same host thread.
 * @return
 * ::cnrtSuccess,
   ::cnrtErrorNotReady,
   ::cnrtErrorNoDevice,
   ::cnrtErrorDeviceInvalid,
   ::cnrtErrorArgsInvalid,
   ::cnrtErrorSys,
   ::cnrtErrorSysNoMem,
   ::cnrtErrorInvalidResourceHandle,
   ::cnrtErrorIllegalState,
   ::cnrtErrorNotSupport,
   ::cnrtErrorOpsNotPermitted,
   ::cnrtErrorQueue,
   ::cnrtErrorNoMem,
   ::cnrtErrorAssert,
   ::cnrtErrorKernelTrap,
   ::cnrtErrorKernelUserTrap,
   ::cnrtErrorNotFound,
   ::cnrtErrorInvalidKernel,
   ::cnrtErrorNoKernel,
   ::cnrtErrorNoModule,
   ::cnrtErrorQueueCaptureUnsupported,
   ::cnrtErrorQueueCaptureInvalidated,
   ::cnrtErrorQueueCaptureWrongThread,
   ::cnrtErrorQueueCaptureMerged,
   ::cnrtErrorQueueCaptureUnjoined,
   ::cnrtErrorQueueCaptureIsolation,
   ::cnrtErrorQueueCaptureUnmatched,
   ::cnrtErrorTaskTopoEntityUpdateFailure,
   ::cnrtErrorSetOnActiveProcess,
   ::cnrtErrorDevice,
   ::cnrtErrorNoAttribute,
   ::cnrtErrorMemcpyDirectionInvalid,
   ::cnrtErrorCndrvFuncCall,
   ::cnrtErrorNoCnrtContext,
   ::cnrtErrorInvalidSymbol
 * @note
 * - Once ::cnrtGetLastError is called, the error code will be reset to ::cnrtSuccess.

 * @example
 * @code{.cpp}
    // __mlu_global__ function must be written in a single file named *.mlu
    __mlu_global__ void bangKernelAdd(int *a, int *b, unsigned int size) {
      for (int i = 0; i < size; i++) {
        b[i] += a[i];
      }
    }
    void hostAdd(int *x, int *y, unsigned int size,
                  cnrtDim3_t dim, cnrtFunctionType_t type, cnrtQueue_t queue) {
      bangKernelAdd<<<dim, type, queue>>>(x, y, size);
    }

    // Hybrid programming
    int main () {
      int a, b;
      uint32_t seed = 0x123;
      a = (int)rand_r(&seed) % 10000;
      b = (int)rand_r(&seed) % 10000;

      cnrtDim3_t dim;
      dim.x = 1;
      dim.y = 1;
      dim.z = 1;
      cnrtFunctionType_t type = cnrtFuncTypeBlock;

      // Ensure the origin host thread error code is cnrtSuccess.
      cnrtGetLastError();
      cnrtQueue_t queue;
      cnrtQueueCreate(&queue);

      int *k_a, *k_b;
      cnrtMalloc((void **)&k_a, sizeof(int));
      cnrtMalloc((void **)&k_b, sizeof(int));
      cnrtMemcpy(k_a, &a, sizeof(int), cnrtMemcpyHostToDev);
      cnrtMemcpy(k_b, &b, sizeof(int), cnrtMemcpyHostToDev);

      // Launch kernel
      hostAdd(k_a, k_b, 1, dim, type, queue);
      cnrtRet_t ret = cnrtPeekAtLastError();
      printf("cnrtGetErrorName: %s\n", cnrtGetErrorName(ret));
      printf("cnrtGetErrorStr: %s\n", cnrtGetErrorStr(cnrtGetLastError()));
      cnrtSyncDevice();

      // Free resource
      cnrtFree(k_a);
      cnrtFree(k_b);
      cnrtQueueDestroy(queue);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtGetLastError(void);
//Group: Error Handling Management
/**
 * @brief Retrieves the last error from the CNRT API call without resetting.
 * @details Returns the last error code returned from the CNRT API call.
 * @return
 * ::cnrtSuccess,
   ::cnrtErrorNotReady,
   ::cnrtErrorNoDevice,
   ::cnrtErrorDeviceInvalid,
   ::cnrtErrorArgsInvalid,
   ::cnrtErrorSys,
   ::cnrtErrorSysNoMem,
   ::cnrtErrorInvalidResourceHandle,
   ::cnrtErrorIllegalState,
   ::cnrtErrorNotSupport,
   ::cnrtErrorOpsNotPermitted,
   ::cnrtErrorQueue,
   ::cnrtErrorNoMem,
   ::cnrtErrorAssert,
   ::cnrtErrorKernelTrap,
   ::cnrtErrorKernelUserTrap,
   ::cnrtErrorNotFound,
   ::cnrtErrorInvalidKernel,
   ::cnrtErrorNoKernel,
   ::cnrtErrorNoModule,
   ::cnrtErrorQueueCaptureUnsupported,
   ::cnrtErrorQueueCaptureInvalidated,
   ::cnrtErrorQueueCaptureWrongThread,
   ::cnrtErrorQueueCaptureMerged,
   ::cnrtErrorQueueCaptureUnjoined,
   ::cnrtErrorQueueCaptureIsolation,
   ::cnrtErrorQueueCaptureUnmatched,
   ::cnrtErrorTaskTopoEntityUpdateFailure,
   ::cnrtErrorSetOnActiveProcess,
   ::cnrtErrorDevice,
   ::cnrtErrorNoAttribute,
   ::cnrtErrorMemcpyDirectionInvalid,
   ::cnrtErrorCndrvFuncCall,
   ::cnrtErrorNoCnrtContext,
   ::cnrtErrorInvalidSymbol
 * @note
 * - The error code will not be reset to ::cnrtSuccess after calling ::cnrtPeekAtLastError.

 * @example
 * - See example in ::cnrtGetLastError.
*/
cnrtRet_t cnrtPeekAtLastError(void);

/************************************************************************
 * utility management
 ************************************************************************/
//Group: Utility Management
/**
 * @brief Creates and sets the quantization parameters used for casting data types.
 * @details Creates the quantization parameters ``pos``, ``scale``, and ``offset``, and sets the
 *          values to these parameters. Returns in ``param`` the quantization parameters used for casting
 *          data types. For more information about quantization, see "Cambricon BANG C/C++ Programming Guide".
 * @param[out] param Pointer to the quantization parameters defined in ::cnrtQuantizedParam_t.
 * @param[in]  pos The position factor used in quantization.
 * @param[in]  scale The scale factor used in quantization.
 * @param[in]  offset The offset factor used in quantization.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - After this API is called, the output pointer ``param`` is used in the ::cnrtCastDataType
 *   or cnrtTransOrderAndCast API to cast data type.
 * - The ::cnrtDestroyQuantizedParam API should be called to release the memory space when
 *   ``param`` is no longer needed.
 */
cnrtRet_t cnrtCreateQuantizedParam(cnrtQuantizedParam_t *param, int pos,
                                   float scale, int offset);
//Group: Utility Management
/**
 * @brief Creates and sets the quantization parameters used for casting data types. Quantizes data by channel.
 * @details Creates quantization parameters ``poses``, ``scales`` and ``offsets``, and sets the
 *          values to these parameters. Returns in ``param`` the quantization parameters defined in
 *          ::cnrtQuantizedParam_t used for casting data type.
 *          The data is divided into groups based on the number of elements of the channel
 *          dimension and quantized filter data for each group. The ::cnrtCreateQuantizedParam API
 *          quantizes data without division, and is usually for input or output data.
 *          Compared with the ::cnrtCreateQuantizedParam API, this API has a higher precision quantization.
 *          For more information about quantization, see "Cambricon BANG C/C++ Programming Guide".
 * @param[out] param Pointer to the quantization parameters defined in ::cnrtQuantizedParam_t.
 * @param[in]  poses The position factor used in quantization.
 * @param[in]  scales The scale factor used in quantization.
 * @param[in]  offsets The offset factor used in quantization.
 * @param[in]  dimNum The number of dimensions of the filter data to be quantized.
 * @param[in]  dimValues The number of elements for each dimension of the filter data to be quantized.
 * @param[in]  channelDim The dimension index of the channel in the filter data layout.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSys
 * @note
 * - After this API is called, the output pointer ``param`` is used in the ::cnrtCastDataType
 *   or cnrtTransOrderAndCast API to cast data type.
 * - The ::cnrtDestroyQuantizedParam API should be called to release the memory space when
 *   ``param`` is no longer needed.
 * - This API is to set parameters for quantization by channel, so the number of ``scales``,
 *   ``poses``, ``offsets`` should be the same as that of channel elements.
 */
cnrtRet_t cnrtCreateQuantizedParamByChannel(cnrtQuantizedParam_t *param,
                                            int *poses, float *scales,
                                            float *offsets, int dimNum,
                                            int *dimValues, int channelDim);
//Group: Utility Management
/**
 * @brief Releases the memory resources of the quantization parameters.
 * @details Destroys the quantization parameters ``param`` and cleans up the parameter resources.
 * @param[in] param Pointer to the quantization parameters defined in ::cnrtQuantizedParam_t.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSys
 * @note
 * - None.
 */
cnrtRet_t cnrtDestroyQuantizedParam(cnrtQuantizedParam_t param);
//Group: Utility Management
/**
 * @brief Deprecated. Converts the data into another data type.
 * @details Converts data pointed by ``src`` in ``srcDataType`` data type into the
 *          ``dstDataType`` data type with data quantization if the quantization parameter ``param`` is
 *          not set to NULL. Returns the converted data in ``dst``.
 * @param[out] dst Pointer to the converted output data.
 * @param[in] src Pointer to the input data to be converted.
 * @param[in] srcDataType The data type of the input data to be converted. The data type is
 *            defined in ::cnrtDataType_t.
 * @param[in]  dstDataType The data type of the data to be converted. The data type is defined
 *             in ::cnrtDataType_t.
 * @param[in] count The number of data to be converted.
 * @param[in] param Pointer to the quantization parameters defined in ::cnrtQuantizedParam_t.
 *            Create and set quantization parameters via the ::cnrtCreateQuantizedParam or
 *            ::cnrtCreateQuantizedParamByChannel API. To ignore quantizing data, set this
 *            parameter to NULL.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - If the input pointer ``param`` is set to NULL, the data will not be quantized.
 *   The data types are shown in the order  ``srcDataType``-``dstDataType``.
 *
 *   Supported combinations:
 *    float32-float16, float32-uint8, int64-float16, float16-float32, float16-uint8,
 *    uint8-float32, uint8-float16, float32-float32
 *    in int64-float16 case, int64 is first converted to float,and then the float data is converted.
 * - If the input pointer ``param`` is not set to NULL, the data will be quantized.
 *   The data types are shown in the order  ``srcDataType``-``dstDataType``.
 *
 *   Supported combinations:
 *    float32-float16, float32-int16, float32-int8, float32-int32, int32-float32,
 *    float16-int16, int16-float32, int8-float32, float32-float32
 */
cnrtRet_t cnrtCastDataType(void *src, cnrtDataType_t srcDataType, void *dst,
                           cnrtDataType_t dstDataType, int count,
                           cnrtQuantizedParam_t param);
//Group: Utility Management
/**
 * @brief Converts the input data into another data type in specified rounding mode.
 * @details Converts data pointed by ``src`` in ``srcDataType`` data type into the
 *          ``dstDataType`` data type with data quantization if the quantization parameter ``param`` is
 *          not set to NULL. Returns the converted data in ``dst``.
 * @param[out] dst Pointer to the converted output data.
 * @param[in] src Pointer to the input data to be converted.
 * @param[in] srcDataType The data type of the input data to be converted. The data type is
 *           defined in ::cnrtDataType_t.
 * @param[in]  dstDataType The data type of the data to be converted. The data type is defined
 *             in ::cnrtDataType_t.
 * @param[in] count The number of data to be converted.
 * @param[in] param Pointer to the quantization parameters defined in ::cnrtQuantizedParam_t.
 *            Create and set quantization parameters via the ::cnrtCreateQuantizedParam or
 *            ::cnrtCreateQuantizedParamByChannel API. To ignore quantizing data, set this
 *            parameter to NULL.
 * @param[in] roundingMode The rounding mode for data type conversion.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - If the input value ``count`` is zero, the API will return ``cnrtSuccess`` .
 * - If the input pointer ``param`` is set to NULL, the data will not be quantized.
 *   The data types are shown in the order  ``srcDataType``-``dstDataType``.
 *
 *   Supported combinations:
 *   double->half, double->bfloat,
 *   float->half, float->bfloat16, float->uint64, float->int64, float->uint32,
 *   float->int32, float->uint16, float->int16, float->uint8, float->int8,
 *   float->bool,
 *   bfloat->float, bfloat->uint64, bfloat->uint32, bfloat->uint16, bfloat->int64,
 *   bfloat->int32,
 *   half->float, half->bfloat, half->uint64, half->int64, half->uint32,
 *   half->int32, half->uint16, half->int16, half->uint8, half->int8,
 *   uint64->half, uint64->bfloat, uint64->float,
 *   int64->half, int64->bfloat, int64->float,
 *   uint32->half, uint32->bfloat, uint32->float,
 *   int32->half, int32->bfloat, int32->float,
 *   uint16->half, uint16->bfloat, uint16->float,
 *   int16->half, int16->bfloat, int16->float,
 *   uint8->half, uint8->bfloat, uint8->float,
 *   int8->half, int8->bfloat, int8->float.
 * - If the input pointer ``param`` is not set to NULL, the data will be quantized.
 *   The data types are shown in the order  ``srcDataType``-``dstDataType``.
 *
 *   Supported combinations:
 *   float->half, float->int16, float->int8, float->int32, int32->float,
 *   half->int16, half->int8, int16->float, int16->half, int8->float, int8->half.
 */
cnrtRet_t cnrtCastDataType_V2(const void *src, cnrtDataType_V2_t srcDataType,
                              void *dst, cnrtDataType_V2_t dstDataType,
                              int count, cnrtQuantizedParam_t param,
                              cnrtRoundingMode_t roundingMode);
//Group: Utility Management
/**
 * @brief Reshapes the input filter data.
 * @details Reshapes the filter data from source memory address pointed
 *          by ``dst`` to destination memory address pointed by ``src`` with the
 *          source data shape src[NHWC] and data type ``type`` that is specified
 *          in ``cnrtDataType_t`` enum.
 * @param[out] dst The destination address.
 * @param[in]  src The source address.
 * @param[in]  n The batch size.
 * @param[in]  h The height.
 * @param[in]  w The width.
 * @param[in]  c The channel.
 * @param[in]  type The data type of the source and destination data.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * @note
 * - None.
 * @example
 * - None.
 */
extern cnrtRet_t cnrtFilterReshape(void *dst, void *src, int n, int h, int w,
                                   int c, cnrtDataType_t type);
//Group: Memory Management
/**
 * @brief Flushes or invalidates cache on the host.
 * @details Ensures cache consistency if the ``hostPtr`` is cached.
 *          Specifies the operation on cache with ::cnrtCacheOps_t enum ``ops``.
 *          Currently, only the flush operation is supported.
 * @param[in] ptr Reserved for checking the device address legality.
 * @param[in] hostPtr Host memory address to do cache operation..
 * @param[in] size The number of bytes to do cache operation.
 * @param[in] ops The operation type on cache.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - This API first makes ``hostPtr`` be aligned to cache line. The ``size`` may
 *   change to be aligned to cache line. The cache line now is 64-byte in driver.
 * @example
 * - See example in ::cnrtMmapCached.
 */
cnrtRet_t cnrtMcacheOperation(void *ptr, void *hostPtr, size_t size,
                              cnrtCacheOps_t ops);
//Group: Memory Management
/**
 * @brief Maps the range of device memory into the user-mode uncached virtual address.
 * @details Maps memory address of the device address pointer ``ptr`` into the
 *          user-mode address, and returns the uncached host memory address pointer ``pHostPtr``.
 * @param[in] ptr The device address to map.
 * @param[out] pHostPtr Pointer to the host memory address to be mapped into.
 * @param[in] size The size of the memory in bytes to be mapped into.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - The output parameter ``pHostPtr`` is a pointer to uncached host address. it is not necessary to
 *   ensure cache consistency, but this may impact performance.
 * @example
 * @code{.cpp}
    int main () {
      void *addr = NULL;
      void *host = NULL;
      size_t size = 0x100000;

      cnrtMalloc(&addr, size);
      cnrtMmap(addr, &host, size);

      memset(host, 'a', size);

      void *host1 = NULL;
      cnrtMmap(addr, &host1, size);
      memset(host1, 'b', size);

      void *cmp = malloc(size);
      cnrtMemcpy(cmp, addr, size, cnrtMemcpyNoDirection);

      if (memcmp(cmp, host1, size)) printf("memcmp failed!\n");

      free(cmp);
      cnrtMunmap(host1, size);
      cnrtFree(addr);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMmap(void *ptr, void **pHostPtr, size_t size);
//Group: Memory Management
/**
 * @brief Maps the range of device memory address into the cached host address space.
 * @details Maps memory address of the device address pointer ``ptr`` into the
 *          host memory space, and returns the cached host address pointer ``pHostPtr``.
 * @param[in] ptr The device address to map.
 * @param[out] pHostPtr Pointer to the host address to be mapped into.
 * @param[in] size The size of the memory in bytes to be mapped into.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - The output parameter ``pHostPtr`` is a pointer to cached host address, so
 *   a cache flush operation is required  to ensure cache consistency.
 * @example
 * @code{.cpp}
    int main () {
      void *addr = NULL;
      void *host = NULL;
      size_t size = 0x100000;

      cnrtMalloc(&addr, size);
      cnrtMmapCached(addr, &host, size);
      memset(host, 'a', size);
      cnrtMcacheOperation(addr, host, size, CNRT_FLUSH_CACHE);

      void *host1 = NULL;
      cnrtMmapCached(addr, &host1, size);
      memset(host1, 'b', size);
      cnrtMcacheOperation(addr, host, size, CNRT_FLUSH_CACHE);

      void *cmp = malloc(size);
      cnrtMemcpy(cmp, addr, size, cnrtMemcpyNoDirection);

      if (memcmp(cmp, host1, size)) printf("memcmp failed!\n");

      free(cmp);
      cnrtMunmap(host1, size);
      cnrtFree(addr);

      return 0;
    }
 * @endcode
 */
cnrtRet_t cnrtMmapCached(void *ptr, void **pHostPtr, size_t size);
//Group: Memory Management
/**
 * @brief Unmaps the host address.
 * @details Unmaps the mapped host address and device memory address.
 *          The mapping is created by ::cnrtMmap or ::cnrtMmapCached.
 * @param[in] hostPtr Host address to be unmapped.
 * @param[in] size The size of the host address to be unmapped.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorNoCnrtContext,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - See example in ::cnrtMmapCached.
 */
cnrtRet_t cnrtMunmap(void *hostPtr, size_t size);
//Group: TaskTopo Management
/**
 * @brief Creates an empty TaskTopo.
 * @details Initializes a new TaskTopo and returns a pointer ``pTaskTopo``.
 * @param[out] pTaskTopo Pointer to the new TaskTopo.
 * @param[in]  flags TaskTopo creation flags.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - ``flags`` must be 0.
 * @example
 * @code{.cpp}
    int main () {
      cnrtTaskTopo_t topo;
      cnrtRet_t res = cnrtTaskTopoCreate(&topo, 0);
      assert(res = cnrtSuccess);
    }
 * @endcode
 */
cnrtRet_t cnrtTaskTopoCreate(cnrtTaskTopo_t *pTaskTopo, unsigned int flags);
//Group: TaskTopo Management
/**
 * @brief Destroys a specified TaskTopo along with all its node.
 * @details Deallocates the TaskTopo and all associated nodes specified by ``taskTopo``.
 * @param[in]  taskTopo The TaskTopo to be destroyed.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoDestroy(cnrtTaskTopo_t taskTopo);
//Group: TaskTopo Management
/**
 * @brief Clones an existing TaskTopo.
 * @details Creates a copy of TaskTopo specified by ``originalTaskTopo`` and returns the new TaskTopo via ``pTaskTopoClone``.
 * This API clones all the nodes and copies all the parameters.
 * @param[out] pTaskTopoClone Pointer to the newly cloned TaskTopo.
 * @param[in]  originalTaskTopo The original TaskTopo to clone.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoClone(cnrtTaskTopo_t *pTaskTopoClone,
                            cnrtTaskTopo_t originalTaskTopo);
//Group: TaskTopo Management
/**
 * @brief Finds the corresponding node in cloned TaskTopo.
 * @details Retrieves the node in ``clonedTaskTopo`` corresponding to ``originalNode``
 *          in the original TaskTopo.
 * @param[out] pNode Pointer to the cloned node.
 * @param[in]  originalNode The original node.
 * @param[in]  clonedTaskTopo The cloned TaskTopo to query.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNodeFindInClone(cnrtTaskTopoNode_t *pNode,
                                      cnrtTaskTopoNode_t originalNode,
                                      cnrtTaskTopo_t clonedTaskTopo);
//Group: TaskTopo Management
/**
 * @brief Destroys a node from the TaskTopo.
 * @details Destroys the node specified by ``node`` from the TaskTopo.
 * @param[in] node Node to be removed.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - ::cnrtTaskTopoDestroyNode not only removes the specified node from the TaskTopo but also
 *   serves any existing dependencies between that node and other nodes in the TaskTopo.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoDestroyNode(cnrtTaskTopoNode_t node);
//Group: TaskTopo Management
/**
 * @brief Retrieves the dependency edges of a TaskTopo.
 * @details Retrieves a list of dependency edges of the TaskTopo. The relationships are
 *          indicated by indices, with each edge from ``from[i]`` pointing to ``to[i]``.
 *          - If both ``from`` and ``to`` are NULL, this API 
 *            retrieves the total number of edges in the TaskTopo and stores it in ``numEdges``.
 *          - If one of ``from`` and ``to`` is NULL while the other is non-NULL, this API 
 *            returns ::cnrtErrorArgsInvalid.
 *          - If both ``from`` and ``to`` are non-NULL, this API retrieves edges and fills in entries.
 *          - If the input value of ``numEdges`` is greater than the actual
 *            number of edges, this API sets the remaining entries in ``from`` and ``to``
 *            to NULL, and stores the number of edges actually returned in
 *            ``numEdges``.
 *
 * @param[in]  taskTopo The TaskTopo from which edges are retrieved.
 * @param[out] from Location to return the edge of a source node.
 * @param[out] to Location to return the edge of a destination node.
 * @param[in,out] numEdges The number of edges. See the description for details.
 * @return
 * ::cnrtSuccess, 
 * ::cnrtErrorArgsInvalid
 *
 * @note
 * - ``from`` and ``to`` must either both be NULL or both be non-NULL. Otherwise,
 *   ::cnrtErrorArgsInvalid will be returned.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoGetEdges(cnrtTaskTopo_t taskTopo,
                               cnrtTaskTopoNode_t *from, cnrtTaskTopoNode_t *to,
                               size_t *numEdges);
//Group: TaskTopo Management
/**
 * @brief Retrieves nodes of a TaskTopo.
 * @details Retrieves a list of nodes of a TaskTopo. 
 *          - If ``pNode`` is NULL, 
 *            this API retrieves the number of nodes and stores it in ``numNodes``.
 *          - If ``pNode`` is non-NULL, ``numNodes`` entries are filled in.
 *          - If the input value of ``numNodes``
 *            is greater than the actual number of nodes, this API sets the remaining entries in
 *            ``pNode`` to NULL, and stores the number of nodes actually
 *            obtained in ``numNodes``.
 *
 * @param[in]  taskTopo TaskTopo to query.
 * @param[out] pNode The array to store the returned nodes.
 * @param[in,out] numNodes The number of nodes. See the description for details.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoGetNodes(cnrtTaskTopo_t taskTopo,
                               cnrtTaskTopoNode_t *pNode, size_t *numNodes);
//Group: TaskTopo Management
/**
 * @brief Retrieves root nodes of a TaskTopo.
 * @details Retrieves a list of root nodes of a TaskTopo. 
 *          - If ``pRootNode`` is
 *            NULL, this API returns the number of nodes in
 *            ``numRootNodes``. 
 *          - If ``pRootNode`` is non-NULL, ``numRootNodes`` entries are filled in.
 *          - If the input value of ``numRootNodes`` is greater than the actual number of nodes,
 *            this API sets the remaining entries in ``pRootNode`` to NULL, and stores the
 *            number of nodes actually obtained ``pRootNode``.
 *
 * @param[in]  taskTopo TaskTopo to query.
 * @param[out] pRootNode The array to store the returned root nodes.
 * @param[in,out] numRootNodes The number of rood nodes. See the description for details.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoGetRootNodes(cnrtTaskTopo_t taskTopo,
                                   cnrtTaskTopoNode_t *pRootNode,
                                   size_t *numRootNodes);
//Group: TaskTopo Management
/**
 * @brief Adds edges between nodes in a TaskTopo to establish dependencies.
 * @details Creates dependencies between nodes as specified by corresponding indices in ``from`` and ``to``.
 *          ``numDependencies`` defines the number of dependencies to be added to ``taskTopo``.
 *
 * @param[in] taskTopo The TaskTopo to add dependency edges to.
 * @param[in] from Array of nodes that act as dependencies.
 * @param[in] to Array of nodes that depend on the ``from`` nodes.
 * @param[in] numDependencies The number of dependencies to be added.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - Each node in ``from`` and ``to`` must belong to ``taskTopo``.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddDependencies(cnrtTaskTopo_t taskTopo,
                                      const cnrtTaskTopoNode_t *from,
                                      const cnrtTaskTopoNode_t *to,
                                      size_t numDependencies);
//Group: TaskTopo Management
/**
 * @brief Removes dependency edges from a TaskTopo.
 * @details Removes dependencies between nodes as specified by corresponding indices in ``from`` and ``to``.
 *          ``numDependencies`` defines the number of dependencies to be removed from ``taskTopo``.
 * @param[in] taskTopo The TaskTopo to remove dependency edges from.
 * @param[in] from Array of nodes that act as dependencies.
 * @param[in] to Array of nodes that depend on the ``from`` nodes.
 * @param[in] numDependencies The number of dependencies to be removed.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - Each node in ``from`` and ``to`` must belong to ``taskTopo``.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoRemoveDependencies(cnrtTaskTopo_t taskTopo,
                                         const cnrtTaskTopoNode_t *from,
                                         const cnrtTaskTopoNode_t *to,
                                         size_t numDependencies);
//Group: TaskTopo Management
/**
 * @brief Retrieves the dependencies of a node.
 * @details Retrieves a list of dependencies of ``node``.
 *          - If ``pDependencies`` is
 *          NULL, this API returns the number of dependencies in
 *          ``numDependencies``. 
 *          - If ``pDependencies`` is provided, this API fills in ``numDependencies``.
 *          - If the input value of ``numDependencies`` is greater than the actual number of
 *          dependencies, this API sets the remaining entries in ``pDependencies``
 *          to NULL, and returns the number of nodes actually obtained 
 *          in ``numDependencies``.
 *
 * @param[in]     node The node to query.
 * @param[out]    pDependencies The array to store returned dependency of a node.
 * @param[in,out] numDependencies See the description above for details.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNodeGetDependencies(cnrtTaskTopoNode_t node,
                                          cnrtTaskTopoNode_t *pDependencies,
                                          size_t *numDependencies);
//Group: TaskTopo Management
/**
 * @brief Retrieves the nodes that depend on the specified node.
 * @details Retrieves a list of dependent nodes that  depend on ``node``.
 *          - If ``pDependentNodes`` is
 *            NULL, this API returns the number of dependent nodes in ``numDependentNodes``. 
 *          - If ``pDependentNodes`` is provided, this API fills in ``numDependentNodes`` entries.
 *          - If the input value of ``numDependentNodes`` is greater than the actual number of
 *          dependent nodes, this API sets the remaining entries in ``pDependentNodes``
 *          to NULL, and returns the number of nodes actually obtained
 *          in ``pDependentNodes``.
 *
 * @param[in]     node The node whose dependents are to be queried.
 * @param[out]    pDependentNodes The array to store returned dependent nodes of ``node``.
 * @param[in,out] numDependentNodes The number of dependent nodes. See the description above for details.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNodeGetDependentNodes(cnrtTaskTopoNode_t node,
                                            cnrtTaskTopoNode_t *pDependentNodes,
                                            size_t *numDependentNodes);

/*! @brief
 *
 *  A pointer to the ::cnrtUserObject_t struct holding the information about a
 *  user object.
 *
 */
typedef struct CNuserObject_st *cnrtUserObject_t;
//Group: TaskTopo Management
/**
 * @brief Creates a user object.
 * @details Creates a user object with the specified destructor callback and
 *          initial reference count. The initial references are owned by the caller.
 *          Destructor callbacks cannot make CNRT or CNDrv API calls and should
 *          avoid blocking behavior.
 * @param[out] object_out Location to return the user object handle.
 * @param[in]  ptr Pointer to pass the destroy function.
 * @param[in]  destroy Callback to free the user object when it is no longer in use.
 * @param[in]  initialRefcount The initial reference count to create the object with, which is typically 1.
 * @param[in]  flags Currently it is required to pass 1. This indicates that the
 *             destroy callback cannot be waited on by any CNRT or CNDrv API.
 *             If you require synchronization of the callback, you should signal its
 *             completion manually.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtUserObjectCreate(cnrtUserObject_t *object_out, void *ptr,
                               cnrtHostFn_t destroy,
                               unsigned int initialRefcount,
                               unsigned int flags);
//Group: TaskTopo Management
/**
 * @brief Acquires a reference for a user object.
 * @details Acquires new references for a user object. The new references are
 *          owned by the caller.
 * @param[in]  object The user object to acquire a reference for.
 * @param[in]  count The number of reference to acquire, which is typically 1.
 *             The value must be nonzero and not larger than ``INT_MAX``.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtUserObjectAcquire(cnrtUserObject_t object, unsigned int count);

//Group: TaskTopo Management
/**
 * @brief Releases a reference for a user object.
 * @details Releases user object references owned by the caller. The user object's
 *          destructor is invoked if the reference count reaches zero. It is undefined
 *          behavior to release references not owned by the caller, or to use object
 *          handle after all references are released.
 * @param[in]  object The user object to release a reference for.
 * @param[in]  count The number of reference to release, which is typically 1.
 *             The value must be nonzero and not larger than ``INT_MAX``.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtUserObjectRelease(cnrtUserObject_t object, unsigned int count);
/**
 * @brief Flags for acquiring user object references for TaskTopo.
 */
typedef enum cnrtUserObjectAcquireflags {
  cnrtTaskTopoUserObjectMove = 0x1,
  /*!< Transfers references from the caller rather than creating new references. */
} cnrtUserObjectAcquireflags_t;
//Group: TaskTopo Management
/**
 * @brief Acquires a reference for a user object from a TaskTopo.
 * @details Creates or moves user object references that will be owned by a TaskTopo.
 * @param[in] taskTopo  The TaskTopo to associate the reference with.
 * @param[in] object The user object to acquire a reference for.
 * @param[in] count  The number of references to add to the TaskTopo, which is typically 1.
 *                 The value must be nonzero and not larger than ``INT_MAX``.
 * @param[in] flags  The optional flag ::cnrtTaskTopoUserObjectMove transfers references
 *                   from the caller, rather than creating new references,
 *                   which are created by passing 0.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAcquireUserObject(cnrtTaskTopo_t taskTopo,
                                        cnrtUserObject_t object,
                                        unsigned int count, unsigned int flags);
//Group: TaskTopo Management
/**
 * @brief Releases a reference for a user object from a TaskTopo.
 * @details Releases user object references owned by a TaskTopo.
 * @param[in] taskTopo The TaskTopo that will release the reference.
 * @param[in] object The user object to release a reference for.
 * @param[in] count The number of references to release, which is typically 1.
 *            The value must be nonzero and not larger than ``INT_MAX``.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoReleaseUserObject(cnrtTaskTopo_t taskTopo,
                                        cnrtUserObject_t object,
                                        unsigned int count);
// Group: TaskTopo Management
/**
 * @brief Creates an empty node and adds it to a TaskTopo.
 * @details Creates a new node which performs no operation, and adds it to ``taskTopo`` with
 *          ``numDependencies`` dependencies specified via ``dependencies``. A handle to the
 *          new node will be returned in ``pNode``.
 *
 *          An empty node performs no operation during the execution, but it can be transitive
 *          ordering. For example, for a phased execution TaskTopo with 2 groups of nodes, one
 *          group has m nodes, and the other has n nodes,  m+n dependency edges are needed with
 *          an empty node, while m*n dependency edges are needed without an empty node.
 * @param[out] pNode The value of the granularity returned.
 * @param[in]  taskTopo The properties determine the granularity.
 * @param[in]  dependencies The option to determine the granularity returned. It cannot
 *             have any duplicate entries.
 * @param[in]  numDependencies The number of dependencies, which can be 0, in which
 *             case the node will be placed at the root of the TaskTopo.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddEmptyNode(cnrtTaskTopoNode_t *pNode,
                                   cnrtTaskTopo_t taskTopo,
                                   const cnrtTaskTopoNode_t *dependencies,
                                   size_t numDependencies);
/**
 * @brief Host node parameters.
 */
typedef struct cnrtHostNodeParams_st {
  cnrtHostFn_t fn; /*!< The API to call when the node is being executed. */
  void *userData;  /*!< The argument to be passed to the API. */
} cnrtHostNodeParams_t;

// Group: TaskTopo Management
/**
 * @brief Creates a host execution node and adds it to a TaskTopo.
 * @details Creates a new host API node and adds it to ``taskTopo`` with
 *          ``numDependencies`` dependencies specified via ``dependencies``
 *          and arguments specified in ``nodeParams``.
 * @param[out] pNode The new node. When the TaskTopo is invoked, the
 *             node will invoke the specified CPU function.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node, which cannot have any duplicate entries.
 * @param[in] numDependencies The number of dependencies, which can be 0,
 *            in which case the node will be placed at the root of the TaskTopo.
 * @param[in] nodeParams Parameters for the host node.
 *
 * @returns
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddHostNode(cnrtTaskTopoNode_t *pNode,
                                  cnrtTaskTopo_t taskTopo,
                                  const cnrtTaskTopoNode_t *dependencies,
                                  size_t numDependencies,
                                  const cnrtHostNodeParams_t *nodeParams);
// Group: TaskTopo Management
/**
 * @brief Retrieves the parameters of a host node.
 * @param[in]  node The node to get the parameters for.
 * @param[out] nodeParams Pointer to return the parameters.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoHostNodeGetParams(cnrtTaskTopoNode_t node,
                                        cnrtHostNodeParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a host node.
 * @details Sets the parameters of host node ``node`` to ``nodeParams``.
 * @param[in] node The node to set the parameters for.
 * @param[in] nodeParams The parameters to copy.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoHostNodeSetParams(cnrtTaskTopoNode_t node,
                                        const cnrtHostNodeParams_t *nodeParams);
/**
 * @brief Kernel node parameters.
 */
typedef struct cnrtKernelNodeParams_st {
  void *func;
  /*!< The kernel to invoke. */
  cnrtDim3_t dim;
  /*!< The grid dimensions. */
  cnrtFunctionType_t type;
  /*!< The union type of kernel. */
  unsigned int reserve;
  /*!< Reserved parameter. */
  void **kernelParams;
  /*!< The array of pointers to kernel parameters. */
  void **extra;
  /*!< Extra options, such as packaged parameters. */
} cnrtKernelNodeParams_t;
// Group: TaskTopo Management
/**
 * @brief Creates a kernel execution node and adds it to a TaskTopo.
 * @details Creates a new kernel execution node and adds it to ``taskTopo`` with
 *          ``numDependencies`` dependencies specified via ``dependencies`` and
 *          arguments specified in ``nodeParams``.
 * @param[out] pNode The new node.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node, which cannot have any duplicate entries.
 * @param[in] numDependencies The number of dependencies, which can be 0, in which case
 *            the node will be placed at the root of the TaskTopo.
 * @param[in] nodeParams Parameters for kernel node.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddKernelNode(cnrtTaskTopoNode_t *pNode,
                                    cnrtTaskTopo_t taskTopo,
                                    const cnrtTaskTopoNode_t *dependencies,
                                    size_t numDependencies,
                                    const cnrtKernelNodeParams_t *nodeParams);
// Group: TaskTopo Management
/**
 * @brief Retrieves the parameters of a kernel node.
 * @details Retrieves the parameters of kernel node ``node`` in ``nodeParams``.
 *          The ``extra`` array returned in ``nodeParams``, as well as the
 *          argument values it points to, are owned by the node.
 *
 *          This memory remains valid until the node is destroyed or its parameters
 *          are modified, and should not be modified directly. Use ::cnrtTaskTopoKernelNodeSetParams
 *          to update the parameters of this node.
 * @param[in] node Node to get the parameters for.
 * @param[out] nodeParams Pointer to return the parameters.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoKernelNodeGetParams(cnrtTaskTopoNode_t node,
                                          cnrtKernelNodeParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a kernel node.
 * @details Sets the parameters of kernel node ``node`` to ``nodeParams``.
 * @param[in] node Node to set the parameters for.
 * @param[in] nodeParams Parameters to copy.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoKernelNodeSetParams(
    cnrtTaskTopoNode_t node, const cnrtKernelNodeParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Creates a memcpy node and adds it to a TaskTopo
 * @details Creates a new memcpy node and adds it to ``taskTopo`` with ``numDependencies``
 *          dependencies specified via ``dependencies``. ``numDependencies`` can be 0,
 *          in which case the node will be placed at the root of the TaskTopo.
 *          ``dependencies`` cannot have any duplicate entries. A handle to the new
 *          node will be returned in ``pNode``.
 *
 *          When the TaskTopo is invoked, the node will perform the memcpy described
 *          by ``copyParams``.
 *
 *          See ::cnrtMemcpy3D for the description of the struct and its restrictions.
 *
 *          Currently, memcpy node only supports 1D memcpy, and does not support host to host memory copying.
 *          Here is the restriction of ``copyParams`` on setting 1D memcpy node:
 * @code
 *   #define N copy_size
 *
 *   cnrtMemcpy3dParam_t memcpy_param = {0};
 *
 *   memcpy_param.dstPtr.pitch = N;
 *   memcpy_param.dstPtr.ysize = 0x1;
 *
 *   memcpy_param.extent.depth = 0x1;
 *   memcpy_param.extent.height = 0x1;
 *   memcpy_param.extent.width = N;
 *
 *   memcpy_param.srcPtr.pitch = N;
 *   memcpy_param.srcPtr.ysize = 0x1;
 *
 *   memcpy_param.src = src_addr;
 *   memcpy_param.dst = dst_addr;
 * @endcode
 * Ignores other parameters when setting 1D memcpy node.
 *
 * @param[out] pNode The new node.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] copyParams The parameters for the memory copy.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddMemcpyNode(cnrtTaskTopoNode_t *pNode,
                                    cnrtTaskTopo_t taskTopo,
                                    const cnrtTaskTopoNode_t *dependencies,
                                    size_t numDependencies,
                                    const cnrtMemcpy3dParam_t *copyParams);

// Group: TaskTopo Management
/**
 * @brief Retrieves the parameters of a memcpy node.
 * @details Retrieves the parameters of memcpy node ``node`` in ``nodeParams``.
 * @param[in] node The node to get the parameters for.
 * @param[out] nodeParams Pointer to return the parameters.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoMemcpyNodeGetParams(cnrtTaskTopoNode_t node,
                                          cnrtMemcpy3dParam_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a memcpy node.
 * @details Sets the parameters of memcpy node ``node`` to ``nodeParams``.
 * The restrictions of ``nodeParams`` are the same as ::cnrtTaskTopoAddMemcpyNode().
 * @param[in] node The node to set the parameters for.
 * @param[in] nodeParams The parameters to copy.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoMemcpyNodeSetParams(
    cnrtTaskTopoNode_t node, const cnrtMemcpy3dParam_t *nodeParams);
/**
 * @brief Memset node parameters.
 */
typedef struct cnrtMemsetParams {
  void *dst;
  /*!< Destination device pointer. */
  size_t pitch;
  /*!< Pitch of destination device pointer, which will be ignored if the height is 1. */
  unsigned int value;
  /*!< Value to be set. */
  unsigned int elementSize;
  /*!< The size of each element in bytes, which must be 1, 2, or 4. */
  size_t width;
  /*!< Width of the row in elements. */
  size_t height;
  /*!< Number of rows. */
} cnrtMemsetParams_t;
// Group: TaskTopo Management
/**
 * @brief Creates a memset node and adds it to a TaskTopo.
 * @details Creates a new memset node and adds it to ``pNode`` with ``numDependencies``
 *          dependencies specified via ``dependencies``. ``numDependencies`` can be 0,
 *          in which case the node will be placed at the root of the TaskTopo.
 *          ``dependencies`` cannot have any duplicate entries. A handle to the new
 *          node will be returned in ``pNode``.
 *
 *          The element size must be 1, 2, or 4 bytes. When the TaskTopo is invoked,
 *          the node will perform the memset described by ``copyParams``.
 *
 *          Currently, memset node only supports 1D memset. Set ``height`` 1 to represent 1D memset.
 * @param[out] pNode The new node.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] copyParams The parameters for the memory set.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddMemsetNode(cnrtTaskTopoNode_t *pNode,
                                    cnrtTaskTopo_t taskTopo,
                                    const cnrtTaskTopoNode_t *dependencies,
                                    size_t numDependencies,
                                    const cnrtMemsetParams_t *copyParams);

// Group: TaskTopo Management
/**
 * @brief Retrieves the parameters of a memset node.
 * @details Retrieves the parameters of memset node ``node`` in ``nodeParams``.
 * @param[in] node The node to get the parameters for.
 * @param[out] nodeParams Pointer to return the parameters.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoMemsetNodeGetParams(cnrtTaskTopoNode_t node,
                                          cnrtMemsetParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a memset node.
 * @details Sets the parameters of memset node ``node`` to ``nodeParams``.
 * @param[in] node The node to set the parameters for.
 * @param[in] nodeParams The parameters to copy.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoMemsetNodeSetParams(cnrtTaskTopoNode_t node,
                                          const cnrtMemsetParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Creates a child TaskTopo node and adds it to a TaskTopo.
 * @details Creates a new node which executes an embedded TaskTopo, and adds it
 *          to ``taskTopo`` with ``numDependencies`` dependencies specified via
 *          ``dependencies``. ``numDependencies`` can be 0, in which case the node
 *          will be placed at the root of the TaskTopo. ``dependencies`` cannot
 *          have any duplicate entries. A handle to the new node will be returned
 *          in ``pNode``. The node executes an embedded child TaskTopo. The child
 *          TaskTopo is cloned in this call.
 * @param[out] pNode The new node.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] hChildTopo The TaskTopo to clone into this node.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddChildTopoNode(cnrtTaskTopoNode_t *pNode,
                                       cnrtTaskTopo_t taskTopo,
                                       const cnrtTaskTopoNode_t *dependencies,
                                       size_t numDependencies,
                                       cnrtTaskTopo_t hChildTopo);

// Group: TaskTopo Management
/**
 * @brief Gets a handle to the embedded TaskTopo of a child TaskTopo node.
 * @details This call does not clone the TaskTopo. Changes to the TaskTopo will
 *          be reflected in the node, and the node retains ownership of the TaskTopo.
 * @param[in] node Node to get the embedded TaskTopo for.
 * @param[out] pTaskTopo Location to store a handle to the TaskTopo.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoChildTopoNodeGetTopo(cnrtTaskTopoNode_t node,
                                           cnrtTaskTopo_t *pTaskTopo);
// Group: TaskTopo Management
/**
 * @brief Creates a notifier place node within a specified TaskTopo.
 * @details Adds a newly created notifier place node to ``taskTopo``.
 *          This node places the specified notifier when the TaskTopo is launched and its dependencies are executed.
 *          If ``numDependencies`` is 0, the node is placed at the root of the TaskTopo.
 * @param[out] pNode The new node.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] hNotifier The notifier handle for the node.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - ``dependencies`` cannot contain any duplicate entries.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddNotifierPlaceNode(
    cnrtTaskTopoNode_t *pNode, cnrtTaskTopo_t taskTopo,
    const cnrtTaskTopoNode_t *dependencies, size_t numDependencies,
    cnrtNotifier_t hNotifier);
// Group: TaskTopo Management
/**
 * @brief Retrieves the notifier handle of a notifier place node ``node`` in ``phNotifier``.
 *
 * @param[in] node The node to get the parameters for.
 * @param[out] phNotifier Pointer to return the notifier handle.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNotifierPlaceNodeGetNotifier(cnrtTaskTopoNode_t node,
                                                   cnrtNotifier_t *phNotifier);
// Group: TaskTopo Management
/**
 * @brief Sets notifier handle for a notifier place node.
 *
 * @param[in] node The node to set the parameters for.
 * @param[in] hNotifier Notifier to use.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNotifierPlaceNodeSetNotifier(cnrtTaskTopoNode_t node,
                                                   cnrtNotifier_t hNotifier);
// Group: TaskTopo Management
/**
 * @brief Creates a notifier wait node within a specified TaskTopo.
 * @details Adds a newly created notifier wait node to ``taskTopo``.
 *          This node waits for completion of the specified notifier after the execution of its dependencies.
 *          If ``numDependencies`` is 0, the node is placed at the root of the TaskTopo.
 * @param[out] pNode The new node.
 * @param[in] taskTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] hNotifier The notifier handle for the node.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - ``dependencies`` cannot contain any duplicate entries.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoAddNotifierWaitNode(
    cnrtTaskTopoNode_t *pNode, cnrtTaskTopo_t taskTopo,
    const cnrtTaskTopoNode_t *dependencies, size_t numDependencies,
    cnrtNotifier_t hNotifier);
// Group: TaskTopo Management
/**
 * @brief Retrieves the notifier handle of a notifier wait node ``node`` in ``phNotifier``.
 *
 * @param[in] node The node to get the parameters for.
 * @param[out] phNotifier Pointer to return the notifier handle.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNotifierWaitNodeGetNotifier(cnrtTaskTopoNode_t node,
                                                  cnrtNotifier_t *phNotifier);
// Group: TaskTopo Management
/**
 * @brief Sets notifier handle for a notifier wait node.
 *
 * @param[in] node The node to set the parameters for.
 * @param[in] hNotifier Notifier to use.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNotifierWaitNodeSetNotifier(cnrtTaskTopoNode_t node,
                                                  cnrtNotifier_t hNotifier);
/**
 * @brief Executable TaskTopo entity.
 */
typedef struct CNtaskTopoEntity_st *cnrtTaskTopoEntity_t;
// Group: TaskTopo Management
/**
 * @brief Creates an executable TaskTopo from a TaskTopo.
 * @details Instantiates ``taskTopo`` as an executable TaskTopo. The TaskTopo is
 *          validated for any structural constraints or intra-node constraints which
 *          were not previously validated. If instantiation is successful, a handle
 *          to the instantiated TaskTopo is returned in ``entity``.
 *
 *          If there are any errors, diagnostic information might be returned in
 *          ``errorNode`` and ``logBuffer``. This is the primary way to inspect
 *          instantiation errors. The output will be null terminated unless the
 *          diagnostics overflow the buffer. In this case, they will be truncated,
 *          and the last byte can be inspected to determine if truncation occurs.
 * @param[out] entity Returns instantiated TaskTopo.
 * @param[in] taskTopo TaskTopo to instantiate.
 * @param[out] pErrorNode In case of an instantiation error, this might be modified to
 *                indicate a node contributing to the error.
 * @param[out] logBuffer A character buffer to store diagnostic messages.
 * @param[in] bufferSize Size of the log buffer in bytes.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoInstantiate(cnrtTaskTopoEntity_t *entity,
                                  cnrtTaskTopo_t taskTopo,
                                  cnrtTaskTopoNode_t *pErrorNode,
                                  char *logBuffer, size_t bufferSize);

// Group: TaskTopo Management
/**
 * @brief Destroys an executable TaskTopo.
 * @details Destroys the executable TaskTopo specified by ``entity``, as well as
 *          all of its executable nodes. If the executable TaskTopo is being
 *          executed, it will not be terminated by this API, while asynchronously
 *          released on completion of the TaskTopo execution.
 * @param[in] entity The executable TaskTopo to destroy.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityDestroy(cnrtTaskTopoEntity_t entity);

// Group: TaskTopo Management
/**
 * @brief Invokes an executable TaskTopo in a queue.
 * @details Executes ``entity`` in ``queue``. Only one instance of ``entity`` may
 *          be executed at a time. For each invoke, the entity will be ordered
 *          after both the previously invoked entity ``entity`` and the task
 *          previously invoked to a queue ``queue``. To execute a TaskTopo
 *          concurrently, it must be instantiated multiple times into multiple
 *          executable TaskTopo.
 * @param[in] entity Executable TaskTopo to invoke.
 * @param[in] queue Queue in which to invoke the TaskTopo.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityInvoke(cnrtTaskTopoEntity_t entity,
                                   cnrtQueue_t queue);
/**
 * @brief TaskTopo node types.
 */
typedef enum cnrtTaskTopoNodeType {
  cnrtTaskTopoNodeTypeEmpty = 0,
  /*!< Empty node. */
  cnrtTaskTopoNodeTypeKernel = 1,
  /*!< Kernel node. */
  cnrtTaskTopoNodeTypeHost = 2,
  /*!< Host function node. */
  cnrtTaskTopoNodeTypeMemcpy = 3,
  /*!< Memcpy node. */
  cnrtTaskTopoNodeTypeMemset = 4,
  /*!< Memset node. */
  cnrtTaskTopoNodeTypeTaskTopo = 5,
  /*!< Child TaskTopo node. */
  cnrtTaskTopoNodeTypeNotifierPlace = 6,
  /*!< External notifier place node. */
  cnrtTaskTopoNodeTypeNotifierWait = 7,
  /*!< External notifier wait node. */
} cnrtTaskTopoNodeType_t;
// Group: TaskTopo Management
/**
 * @brief Retrieves the node type.
 * @param[in]  node    The node to query.
 * @param[out] pType   Pointer to return the node type.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoNodeGetType(cnrtTaskTopoNode_t node,
                                  cnrtTaskTopoNodeType_t *pType);

/**
 * @brief TaskTopo entity update error result.
 */
typedef enum cnrtTaskTopoEntityUpdateResult {
  cnrtTaskTopoEntityUpdateSuccess = 0x0,
  /*!< The update succeeds. */
  cnrtTaskTopoEntityUpdateError = 0x1,
  /*!< The update fails for an unexpected reason. */
  cnrtTaskTopoEntityUpdateErrorTopologyChanged = 0x2,
  /*!< The update fails because the topology changed. */
  cnrtTaskTopoEntityUpdateErrorNodeTypeChanged = 0x3,
  /*!< The update fails because a node type changed. */
  cnrtTaskTopoEntityUpdateErrorParametersChanged = 0x4,
  /*!< The update fails because the parameters change in a way that is not supported. */
  cnrtTaskTopoEntityUpdateErrorNotSupported = 0x5,
  /*!< The update fails because something about the node is not supported. */
  cnrtTaskTopoEntityUpdateErrorUnsupportedFunctionChange = 0x6,
  /*!< The update fails because the function of a kernel node changed in an unsupported way. */
  cnrtTaskTopoEntityUpdateErrorAttributesChanged = 0x7
  /*!< The update fails because the node attributes changed in a way that is not supported. */
} cnrtTaskTopoEntityUpdateResult_t;

/**
 * @brief The additional write flags to create DOT file.
 */
typedef enum cnrtTaskTopoDebugDotFlags {
  cnrtTaskTopoDebugDotFlagsVerbose = (1 << 0),
  /*!< Outputs all debug data as if every debug flag is enabled. */
  cnrtTaskTopoDebugDotFlagsRuntimeTypes = (1 << 1),
  /*!< Uses runtime structs for output. */
  cnrtTaskTopoDebugDotFlagsHandles = (1 << 2),
  /*!< Adds handles to output. */
  cnrtTaskTopoDebugDotFlagsKernelNodeParams = (1 << 3),
  /*!< Adds ::cnrtKernelNodeParams_t values to output. */
  cnrtTaskTopoDebugDotFlagsMemcpyNodeParams = (1 << 4),
  /*!< Adds ::cnrtMemcpy3dParam_t values to output. .*/
  cnrtTaskTopoDebugDotFlagsMemsetNodeParams = (1 << 5),
  /*!< Adds ::cnrtMemsetParams_t values to output .*/
  cnrtTaskTopoDebugDotFlagsHostNodeParams = (1 << 6),
  /*!< Adds ::cnrtHostNodeParams_t values to output. */
  cnrtTaskTopoDebugDotFlagsKernelNodeAttribute = (1 << 7),
  /*!< Adds ::cnrtKernelNodeAttrValue_t values to output .*/
  cnrtTaskTopoDebugDotFlagsNotifierNodeParams = (1 << 8),
  /*!< Adds ::cnrtNotifier_t handle values to output .*/
} cnrtTaskTopoDebugDotFlags_t;

// Group: TaskTopo Management
/**
 * @brief Writes a DOT file describing TaskTopo struct.
 * @param[in] taskTopo    The TaskTopo to create DOT file from.
 * @param[in] path        The path to write the DOT file to.
 * @param[in] flags       Flags from ::cnrtTaskTopoDebugDotFlags_t for specifying
 *                        the additional node information to write.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorSysNoMem,
 * ::cnrtErrorSys
 */
cnrtRet_t cnrtTaskTopoDebugDotPrint(cnrtTaskTopo_t taskTopo, const char *path,
                                    unsigned int flags);

// Group: TaskTopo Management
/**
 * @brief Queries the TaskTopo kernel node attribute.
 * @details Queries the attribute corresponding to ``attrId`` from ``node``,
 *          and stores it in corresponding member of ``valueOut``.
 * @param[in]  node       The node to query.
 * @param[in]  attrId     The attribute ID.
 * @param[out] valueOut   The room to store the attribute value.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - The type of ``node`` must be ::cnrtTaskTopoNodeTypeKernel.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoKernelNodeGetAttribute(
    cnrtTaskTopoNode_t node, cnrtKernelNodeAttr_t attrId,
    cnrtKernelNodeAttrValue_t *valueOut);
// Group: TaskTopo Management
/**
 * @brief Sets the TaskTopo kernel node attribute.
 * @details Sets the attribute corresponding to ``attrId`` for ``node`` from
 *          corresponding attribute of ``value``.
 * @param[in] node       The node to set attribute for.
 * @param[in] attrId     The attribute ID.
 * @param[in] value      The attribute value to set.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - The type of ``node`` must be ::cnrtTaskTopoNodeTypeKernel.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoKernelNodeSetAttribute(
    cnrtTaskTopoNode_t node, cnrtKernelNodeAttr_t attrId,
    const cnrtKernelNodeAttrValue_t *value);

// Group: TaskTopo Management
/**
 * @brief Copies TaskTopo kernel node attributes from source node to the destination node.
 * @param[in] dst The destination node.
 * @param[in] src The source node.
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - Both nodes must belong to the same TaskTopo.
 * - The type of both nodes must be ::cnrtTaskTopoNodeTypeKernel.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoKernelNodeCopyAttributes(cnrtTaskTopoNode_t dst,
                                               cnrtTaskTopoNode_t src);
// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a host node in the given TaskTopo ``entity``.
 * @details The host node is identified by the corresponding ``node`` in the
 *          non-executable TaskTopo, from which the executable TaskTopo is instantiated.
 *          Changes to to-and-from ``node`` edges are ignored.
 *          The changes only affect future launches of ``entity``. Already enqueued
 *          or running launches of ``entity`` are not affected by this call. ``node`` cannot
 *          be modified by this call either.
 *
 * @param[in] entity The executable TaskTopo in which to set the specified node.
 * @param[in] node Host node of the TaskTopo which is used to instantiate entity.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityHostNodeSetParams(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopoNode_t node,
    const cnrtHostNodeParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a kernel node in the TaskTopo ``entity``.
 * @details The kernel node is identified by the corresponding ``node`` in the
 *          non-executable TaskTopo, from which the executable TaskTopo entity is instantiated.
 *          Changes to to-and-from ``node`` edges are ignored.
 *
 *          ``node`` must not have been removed from the original TaskTopo entity.
 *          The ``func`` field of ``nodeParams`` cannot be modified and must match the
 *          original value. All other values can be modified.
 *
 *          The changes only affect future launches of ``entity``. Already
 *          enqueued or running launches of ``entity`` are not affected by this API.
 *          ``node`` cannot be modified by this API either.
 *
 * @param[in] entity The executable TaskTopo entity in which to set the specified node.
 * @param[in] node Kernel node of the TaskTopo from which TaskTopo entity is instantiated.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityKernelNodeSetParams(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopoNode_t node,
    const cnrtKernelNodeParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a memcpy node in the given TaskTopo entity ``entity``.
 * @details The memcpy node is identified by the corresponding ``node`` in the
 *          non-executable TaskTopo, from which the executable TaskTopo entity is instantiated.
 *          Changes to to-and-from ``node`` edges are ignored.
 *
 *          If origin memcpy node is DtoH or HtoD, the device memory must be allocated
 *          from the same Context as the original memory. If origin memcpy node is DtoD,
 *          the source memory must be allocated from the same Context as the original source memory.
 *          Both the instantiation-time memory operands and the memory operands in ``nodeParams`` must be
 *          1D. Zero-length operations are not supported. The restrictions of ``nodeParams`` are
 *          the same as ::cnrtTaskTopoAddMemcpyNode().
 *
 *          The changes only affect future launches of ``entity``.  Already enqueued
 *          or running launches of ``entity`` are not affected by this call. ``node`` cannot be modified
 *          by this call.
 *
 *          Returns ::cnrtErrorArgsInvalid if the memory operands' mappings change; or
 *          either the original or new memory operands are multidimensional.
 *
 * @param[in] entity The executable TaskTopo entity in which to set the specified node.
 * @param[in] node Memcpy node of the TaskTopo which is used to instantiate TaskTopo entity.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityMemcpyNodeSetParams(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopoNode_t node,
    const cnrtMemcpy3dParam_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a memset node in the given TaskTopo ``entity``.
 * @details The memset node is identified by the corresponding ``node`` in the
 *          non-executable TaskTopo, from which the executable TaskTopo entity is instantiated.
 *          Changes to to-and-from ``node`` edges are ignored.
 *
 *          The destination memory in ``nodeParams`` must be allocated from the same
 *          Context as the original destination memory. Both the instantiation-time
 *          memory operand and the memory operand in ``nodeParams`` must be 1D.
 *          Zero-length operations are not supported.
 *
 *          The changes only affect future launches of ``entity``.  Already enqueued
 *          or running launches of ``entity`` are not affected by this call. ``node``
 *          cannot be modified by this call either.
 *
 *          Returns ::cnrtErrorArgsInvalid if the memory operand's mappings change; or
 *          either the original or new memory operand is multi-dimensional.
 *
 * @param[in] entity The executable TaskTopo entity in which to set the specified node.
 * @param[in] node Memset node of the TaskTopo which is used to instantiate TaskTopo entity.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityMemsetNodeSetParams(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopoNode_t node,
    const cnrtMemsetParams_t *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Updates node parameters in the child TaskTopo node in the given TaskTopo ``entity``.
 * @details The child node is identified by the corresponding ``node`` in the
 *          non-executable TaskTopo, from which the executable TaskTopo entity is instantiated.
 *          Changes to to-and-from ``node`` edges are ignored.
 *
 *          The topology of ``childTopo``, as well as the node insertion order, must match that
 *          of the TaskTopo contained in ``node``.  See ::cnrtTaskTopoEntityUpdate() for a list of
 *          restrictions on what can be updated in TaskTopo entity. The update is recursive, so child Topo
 *          nodes contained within the top level child TaskTopo will also be updated.
 *
 *          The changes only affect future launches of ``entity``. Already enqueued
 *          or running launches of ``entity`` are not affected by this call. ``node`` cannot be
 *          modified by this call either.
 *
 * @param[in] entity The executable TaskTopo entity in which to set the specified node.
 * @param[in] node Child node of the TaskTopo which is used to instantiate TaskTopo entity.
 * @param[in] childTopo The child TaskTopo supplying the updated parameters.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityChildTopoNodeSetParams(cnrtTaskTopoEntity_t entity,
                                                   cnrtTaskTopoNode_t node,
                                                   cnrtTaskTopo_t childTopo);
// Group: TaskTopo Management
/**
 * @brief Updates the notifier associated with a specific notifier place node in the given TaskTopo ``entity``.
 *
 * @details ``node`` is a notifier place node in the non-executable TaskTopo, from which the executable TaskTopo entity
 *          is instantiated. Changes to to-and-from ``node`` edges are ignored.
 *
 *          ::cnrtTaskTopoEntityNotifierPlaceNodeSetNotifier ensures that future executions of the TaskTopo will utilize the updated notifier settings, without affecting any currently running or previously enqueued ``entity``. 
 *
 * @param[in] entity The executable TaskTopo in which the notifier of the specified node is set.
 * @param[in] node The notifier place node of the TaskTopo which is used to instantiate TaskTopo entity.
 * @param[in] hNotifier The updated notifier to set.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityNotifierPlaceNodeSetNotifier(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopoNode_t node,
    cnrtNotifier_t hNotifier);
// Group: TaskTopo Management
/**
 * @brief Updates the notifier associated with a specific notifier wait node in the given TaskTopo ``entity``.
 *
 * @details ``node`` is a notifier place node in the non-executable TaskTopo, from which the executable TaskTopo entity
 *          is instantiated. Changes to to-and-from ``node`` edges are ignored.
 *
 *          ::cnrtTaskTopoEntityNotifierWaitNodeSetNotifier ensures that future executions of the TaskTopo will utilize the updated notifier settings, without affecting any currently running or previously enqueued ``entity``. 
 *
 * @param[in] entity The executable TaskTopo entity in which to set the specified node.
 * @param[in] node The notifier place node of the TaskTopo which is used to instantiate TaskTopo entity.
 * @param[in] hNotifier The updated notifier to set.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoEntityNotifierWaitNodeSetNotifier(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopoNode_t node,
    cnrtNotifier_t hNotifier);
// Group: TaskTopo Management
/**
 * @brief Checks whether an executable TaskTopo entity can be updated with a TaskTopo
 *        and performs the update accordingly.
 * @details Updates the node parameters in the instantiated TaskTopo entity
 *          specified by ``entity`` with the node parameters in a topologically identical
 *          TaskTopo specified by ``topo``.
 *
 * Limitations:
 *
 * - Kernel node restrictions:
 *   - The owning Context of the node cannot be changed.
 * - Memset node restrictions:
 *   - The device(s) to which the operand(s) is allocated/mapped cannot be changed.
 *   - The memory must be allocated from the same Context as the original memory.
 *   - Only 1D memset is supported now.
 *   - Zero-length operations are not supported.
 * - Memcpy node restrictions:
 *   - If memcpy is DtoH or HtoD, the device memory must be allocated from the same
 *     Context as the original memory.
 *   - If memcpy is DtoD, the source memory must be allocated from the same Context as
 *     the original source memory.
 *   - Only 1D memcpy is supported now.
 *   - Zero-length operations are not supported.
 *
 * ::cnrtTaskTopoEntityUpdate() sets ``updateResult_out`` to
 * ::cnrtTaskTopoEntityUpdateErrorTopologyChanged under the following conditions:
 *
 * - The count of nodes directly in ``entity`` and ``node`` differ, in which case ``updateResult_out``
 *   is NULL.
 * - A node is deleted in ``node`` but not its pair from ``entity``, in which case ``updateResult_out``
 *   is NULL.
 * - A node is deleted in ``entity`` but not its pair from ``node``, in which case ``updateResult_out``
 *   is the pairless node from ``node``.
 * - The dependent nodes of a pair differ, in which case ``updateResult_out`` is the node from ``topo``.
 *
 * ::cnrtTaskTopoEntityUpdate() sets ``updateResult_out`` to:
 *
 * - ::cnrtTaskTopoEntityUpdateError if an invalid value is passed.
 * - ::cnrtTaskTopoEntityUpdateErrorTopologyChanged if the TaskTopo topology is changed.
 * - ::cnrtTaskTopoEntityUpdateErrorNodeTypeChanged if the type of a node is changed, in which
 *   case ``updateResult_out`` is set to the node from ``topo``.
 * - ::cnrtTaskTopoEntityUpdateErrorUnsupportedFunctionChange if the function changes in an
 *   unsupported way (see note above), in which case ``pErrorNode_out`` is set to the node from ``topo``.
 * - ::cnrtTaskTopoEntityUpdateErrorParametersChanged if any parameter of a node is changed in a
 *   way that is not supported, in which case ``pErrorNode_out`` is set to the node from ``topo``.
 * - ::cnrtTaskTopoEntityUpdateErrorAttributesChanged if any attribute of a node is changed in a
 *   way that is not supported, in which case ``pErrorNode_out`` is set to the node from ``topo``.
 * - ::cnrtTaskTopoEntityUpdateErrorNotSupported if something about a node is unsupported, in
 *   which case ``pErrorNode_out`` is set to the node from ``topo``.
 *
 * If ``updateResult_out`` isn't set in one of the situations described above, the update check
 * passes and ::cnrtTaskTopoEntityUpdate() updates ``entity`` to match the contents of ``topo``. If an
 * error occurs during the update, ``updateResult_out`` will be set to
 * ::cnrtTaskTopoEntityUpdateError; otherwise, ``updateResult_out`` is set to ::cnrtTaskTopoEntityUpdateSuccess.
 *
 * ::cnrtTaskTopoEntityUpdate() returns ::cnrtSuccess when the update is performed successfully. It
 * returns ::cnrtErrorTaskTopoEntityUpdateFailure if the TaskTopo entity update is not
 * performed because it includes changes which violate constraints specific to instantiated Task
 * Topo entity update.
 *
 * @param[in] entity The instantiated TaskTopo entity to be updated.
 * @param[in] topo The TaskTopo containing the updated parameters.
 * @param[out] pErrorNode_out The node which causes the permissibility check to forbid the update, if any.
 * @param[out] updateResult_out Whether the TaskTopo update is permitted, and what the reason is if it is forbidden.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid,
 * ::cnrtErrorTaskTopoEntityUpdateFailure
 * @note
 * - The API may add further restrictions in future releases. The return code should always be
 *   checked.
 * @example
 * - None.

 */
cnrtRet_t cnrtTaskTopoEntityUpdate(
    cnrtTaskTopoEntity_t entity, cnrtTaskTopo_t topo,
    cnrtTaskTopoNode_t *pErrorNode_out,
    cnrtTaskTopoEntityUpdateResult_t *updateResult_out);

// Group: TaskTopo Management
/**
 * @brief  Uploads an executable TaskTopo in a queue.
 *
 * @details Uploads @p entity to the device in @p queue for later execution.
 * The upload is performed asynchronously within the specified @p queue. Uploads of the same @p entity are processed serially,
 * meaning subsequent uploads is queued behind the current one. 
 *
 * @param[in] entity Executable TaskTopo to upload.
 * @param[in] queue Queue where the TaskTopo entity is uploaded.
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorArgsInvalid
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtTaskTopoUpload(cnrtTaskTopoEntity_t entity, cnrtQueue_t queue);

// Group: Profiler Management
/**
 * @brief  Starts profile collection.
 *
 * @details Starts profile collection for the current process. 
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtProfilerStart(void);

// Group: Profiler Management
/**
 * @brief  Stops profile collection.
 *
 * @details Stops profile collection for the current process. 
 *
 * @return
 * ::cnrtSuccess,
 * ::cnrtErrorCndrvFuncCall
 * @note
 * - None.
 * @example
 * - None.
 */
cnrtRet_t cnrtProfilerStop(void);

/*!
 * @brief
 *
 * Describes the return values of CNRT APIs.
 */
#define CNRT_RET_SUCCESS                     cnrtSuccess
#define CNRT_RET_WARNING_FAKE_DEVICE         ((cnrtRet_t)1)
#define CNRT_RET_ERR_NOT_READY               ((cnrtRet_t)632006)
#define CNRT_RET_ERR_INVALID                 ((cnrtRet_t)632007)
#define CNRT_RET_ERR_NOMEM                   ((cnrtRet_t)632008)
#define CNRT_RET_ERR_NODEV                   ((cnrtRet_t)632009)
#define CNRT_RET_ERR_SYS                     ((cnrtRet_t)632011)
#define CNRT_RET_ERR_ACCES                   ((cnrtRet_t)632012)
#define CNRT_RET_ERR_ACCESS                  ((cnrtRet_t)632012)
#define CNRT_RET_ERR_FAULT                   ((cnrtRet_t)632013)
#define CNRT_RET_ERR_BUSY                    ((cnrtRet_t)632014)
#define CNRT_RET_ERR_NORES                   ((cnrtRet_t)632019)
#define CNRT_RET_ERR_UNSUPPORTED             ((cnrtRet_t)632020)
#define CNRT_RET_ERR_INVALID_POINTER         ((cnrtRet_t)632021)
#define CNRT_RET_ERR_NO_EXIST                ((cnrtRet_t)632022)
#define CNRT_RET_ERR_BROKEN                  ((cnrtRet_t)632023)
#define CNRT_RET_ERR_INIT                    ((cnrtRet_t)632024)
#define CNRT_RET_ERR_QUEUE                   ((cnrtRet_t)632025)
#define CNRT_RET_ERR_OUT_RANGE               ((cnrtRet_t)632026)
#define CNRT_RET_ERR_FUNC_CALL               ((cnrtRet_t)632028)
#define CNRT_RET_ERR_UNHANDLED               ((cnrtRet_t)632029)
#define CNRT_RET_ERR_INVALID_TYPE            ((cnrtRet_t)632030)
#define CNRT_RET_ERR_INVALID_OP              ((cnrtRet_t)632031)
#define CNRT_RET_ERR_RESHAPE                 ((cnrtRet_t)632035)
#define CNRT_RET_ERR_MEMCPY                  ((cnrtRet_t)632036)
#define CNRT_RET_ERR_ENCRYPT                 ((cnrtRet_t)632037)
#define CNRT_RET_ERR_INVALID_DATADESC        ((cnrtRet_t)632038)
#define CNRT_RET_ERR_MAP                     ((cnrtRet_t)632039)
#define CNRT_RET_ERR_UNMAP                   ((cnrtRet_t)632040)
#define CNRT_RET_ERR_CACHE                   ((cnrtRet_t)632041)
#define CNRT_RET_ERR_FIND_DEV_ADDR           ((cnrtRet_t)632042)
#define CNRT_RET_ERR_KERNEL_VERSION_TOO_HIGH ((cnrtRet_t)632043)
#define CNRT_RET_ERR_CNDRV_FUNC_CALL         ((cnrtRet_t)632046)
#define CNRT_RET_ERR_CNDEV_FUNC_CALL         ((cnrtRet_t)632047)
#define CNRT_RET_ERR_IO                      ((cnrtRet_t)632048)
#define CNRT_RET_ERR_TIMEOUT                 ((cnrtRet_t)632049)
#define CNRT_RET_ERR_NOSYS                   ((cnrtRet_t)632050)
#define CNRT_RET_ERR_AGAIN                   ((cnrtRet_t)632051)
#define CNRT_RET_ERR_EXIST                   ((cnrtRet_t)632052)
#define CNRT_RET_ERR_MATH_OVERFLOW           ((cnrtRet_t)632053)
#define CNRT_RET_ERR_MLU                     ((cnrtRet_t)632054)
#define CNRT_RET_ERR_ONCHIP_CORE             ((cnrtRet_t)632055)
#define CNRT_RET_ERR_GET_MEM_RANGE           ((cnrtRet_t)632056)
#define CNRT_RET_ERR_GET_MEM_INFO            ((cnrtRet_t)632057)
#define CNRT_RET_ERR_EVENT                   ((cnrtRet_t)632058)
#define CNRT_RET_ERR_NOTIFIER                ((cnrtRet_t)632059)
#define CNRT_RET_ERR_GET_IMP_DEV             ((cnrtRet_t)632060)
#define CNRT_RET_ERR_LOAD_MODULE             ((cnrtRet_t)632061)
#define CNRT_RET_ERR_UNKNOWN                 ((cnrtRet_t)999991)
#define CNRT_RET_ERR_MAX                     ((cnrtRet_t)999992)
/*!
 *  @brief
 *  Describes the queue flags used for the
 *  current process execution on the current device.
 *  The ::cnrtSetDeviceFlag API is used to set the flag.
 *  The ::cnrtGetDeviceFlag API is used to retrieve the flag set.
 *
 */
#define CNRT_QUEUE_SYNC_SPIN  ((cnrtDeviceFlags_t)0)
#define CNRT_QUEUE_SYNC_BLOCK ((cnrtDeviceFlags_t)1)
#define CNRT_QUEUE_SYNC_YIELD ((cnrtDeviceFlags_t)2)

/**< Execution modes of tasks on MLU. */
/*! @brief
 *
 *  Describes the number of cores used
 *  for computation on the MLU devices.
 */
#define CNRT_FUNC_TYPE_BLOCK   ((cnrtFunctionType_t)1)
#define CNRT_FUNC_TYPE_BLOCK0  ((cnrtFunctionType_t)1)
#define CNRT_FUNC_TYPE_BLOCK1  ((cnrtFunctionType_t)2)
#define CNRT_FUNC_TYPE_UNION1  ((cnrtFunctionType_t)4)
#define CNRT_FUNC_TYPE_UNION2  ((cnrtFunctionType_t)8)
#define CNRT_FUNC_TYPE_UNION4  ((cnrtFunctionType_t)16)
#define CNRT_FUNC_TYPE_UNION8  ((cnrtFunctionType_t)32)
#define CNRT_FUNC_TYPE_UNION16 ((cnrtFunctionType_t)64)
#define CNRT_FUNC_TYPE_MUTABLE ((cnrtFunctionType_t)-1)
#define CNRT_JOB_TYPE_BLOCK    ((cnrtFunctionType_t)CNRT_FUNC_TYPE_BLOCK)
#define CNRT_JOB_TYPE_UNION1   ((cnrtFunctionType_t)CNRT_FUNC_TYPE_UNION1)
#define CNRT_JOB_TYPE_UNION2   ((cnrtFunctionType_t)CNRT_FUNC_TYPE_UNION2)
#define CNRT_JOB_TYPE_UNION4   ((cnrtFunctionType_t)CNRT_FUNC_TYPE_UNION4)

/*! @brief
 *
 * Describes the directions of data copying.
 */
#define CNRT_MEM_TRANS_DIR_HOST2DEV  ((cnrtMemTransDir_t)0)
#define CNRT_MEM_TRANS_DIR_DEV2DEV   ((cnrtMemTransDir_t)1)
#define CNRT_MEM_TRANS_DIR_DEV2HOST  ((cnrtMemTransDir_t)2)
#define CNRT_MEM_TRANS_DIR_HOST2HOST ((cnrtMemTransDir_t)3)
#define CNRT_MEM_TRANS_DIR_PEER2PEER ((cnrtMemTransDir_t)4)
#define CNRT_MEM_TRANS_DIR_NODIR     ((cnrtMemTransDir_t)5)

/**< Data type and data order.*/
/*! @brief
 *
 * Describes the data types supported by CNRT.
 */
#define CNRT_INVALID ((cnrtDataType_t)0x0)
#define CNRT_FLOAT16 ((cnrtDataType_t)0x12)
#define CNRT_FLOAT32 ((cnrtDataType_t)0x13)
#define CNRT_FLOAT64 ((cnrtDataType_t)0x14)
#define CNRT_INT4    ((cnrtDataType_t)0x20)
#define CNRT_INT8    ((cnrtDataType_t)0x21)
#define CNRT_INT16   ((cnrtDataType_t)0x22)
#define CNRT_INT32   ((cnrtDataType_t)0x23)
#define CNRT_INT64   ((cnrtDataType_t)0x24)
#define CNRT_AUTO    ((cnrtDataType_t)0x25)
#define CNRT_UINT8   ((cnrtDataType_t)0x31)
#define CNRT_UINT16  ((cnrtDataType_t)0x32)
#define CNRT_UINT32  ((cnrtDataType_t)0x33)
#define CNRT_FIX8    ((cnrtDataType_t)0x41)
#define CNRT_QUANT8  ((cnrtDataType_t)0x51)
#define CNRT_BOOL    ((cnrtDataType_t)0x61)

#if !defined(__DOXYGEN_ONLY__)

// Group: NOT DOC
cnrtRet_t cnrtGetDeviceProperties_V2(cnrtDeviceProp_t *prop, int device);
// Group: NOT DOC
cnrtRet_t cnrtGetDeviceProperties_V3(cnrtDeviceProp_t *prop, int device);

#endif

#if defined(__CNRT_VERSION_INTERNAL) && !defined(__DOXYGEN_ONLY__)

// Group: NOT DOC
cnrtRet_t cnrtGetDeviceProperties(cnrtDeviceProp_t *prop, int device);

#endif

#ifndef __DOXYGEN_ONLY__
/** Not documenting @{ */

// clang-format off

#if defined(__GNUC__)
#if defined(__clang__) || (!defined(__PGIC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic push
#endif
#if defined(__clang__) || (!defined(__PGIC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2)))
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma waring(disable: 4820)
#endif

#if defined(__GNUC__)
#if defined(__clang__) || (!defined(__PGIC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic pop
#endif
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

// clang-format on

/** @} */
#endif

#if defined(__cplusplus)
}
#endif /*__cplusplus*/

#ifndef CNRT_CHECK

#include <stdio.h>

#ifndef CNRT_CHECK_PRINT_CHANNEL
#define CNRT_CHECK_PRINT_CHANNEL ((FILE *)stdout)
#endif

#ifndef CNRT_CHECK_ERROR_BEHAVIOR
#define CNRT_CHECK_ERROR_BEHAVIOR(ret) exit(-1)
#endif

#define CNRT_CHECK(val)                                                        \
  do {                                                                         \
    cnrtRet_t ret = val;                                                       \
    if (ret) {                                                                 \
      fprintf(CNRT_CHECK_PRINT_CHANNEL,                                        \
              "[%s:%d] CNRT error, code=%d(%s) \"%s\" \n", __FILE__, __LINE__, \
              (unsigned int)ret, cnrtGetErrorStr(ret), #val);                  \
      fflush(CNRT_CHECK_PRINT_CHANNEL);                                        \
      CNRT_CHECK_ERROR_BEHAVIOR(ret);                                          \
    }                                                                          \
  } while (0)

#endif

#endif /*__CNRT_H*/
