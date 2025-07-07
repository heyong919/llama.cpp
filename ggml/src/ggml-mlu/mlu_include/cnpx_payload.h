#ifndef CNPX_CNPX_PAYLOAD_H
#define CNPX_CNPX_PAYLOAD_H
#include <stddef.h>
#include <stdint.h>
#ifndef CNPX_ENABLED
#error Never include this file directly -- it is automatically included by cnpx.h
#endif
#include <stdint.h>
typedef struct cnpxPayloadData_v1 {
  // payload的schemaId
  uint64_t schemaId;
  // payload的size
  size_t size;
  // pointer point to the payload
  const void* payload;
} cnpxPayloadData_t;

typedef struct cnpxPayloadSchemaEntry_v1 {
  // A bit mask used to describe some attributes of this entry.
  // They are represented by macros starting with CNPX_PAYLOAD_ENTRY_FLAG_.
  uint64_t flags;
  // Indicates the type of this entry, which can be a predefined macro starting with CNPX_PAYLOAD_ENTRY_TYPE_,
  // or it can be a schemaId indicating that this entry is also a payload (similar to a structure within a structure).
  uint64_t type;
  // Indicates the name of this entry.
  const char* name;
  // Indicates the description of this entry.
  const char* description;
  // This entry is meaningful only when certain attributes are included in the flag.
  // The specific meaning will be introduced in the macro definition of the corresponding attribute.
  uint64_t arrayDetail;
  // The offset of this item in the entire payload.
  uint64_t offset;
} cnpxPayloadSchemaEntry_t;

typedef struct cnpxPayloadSchemaAttr_v1 {
  // Each field in the structure corresponds to a macro,
  // and the macros for the filled fields in the structure are combined into a bitmask to indicate which fields in the structure are valid.
  uint64_t fieldMask;
  // The name of the schema (optional).
  const char* name;
  // What entries make up the schema? The cnpxPayloadSchemaEntry_t stores the position,
  // type information, and other details for each item in the payload.
  const cnpxPayloadSchemaEntry_t* entries;
  // The number of `cnpxPayloadSchemaEntry_t`.
  size_t numEntries;
  // The size of the entire payload (optional). To reduce workload, the first version might make it a mandatory item.
  size_t payloadStaticSize;
  // The alignment of the entire payload (optional). To reduce workload, the first version might make it a mandatory item.
  size_t packAlign;
  // The ID of this schema (optional). If the user wishes to specify a static schemaId, this item needs to be filled in.
  uint64_t schemaId;
} cnpxPayloadSchemaAttr_t;

#define CNPX_PAYLOAD_SCHEMA_FIELD_NAME 1 << 0
#define CNPX_PAYLOAD_SCHEMA_FIELD_SIZE 1 << 2
#define CNPX_PAYLOAD_SCHEMA_FIELD_ALIGN 1 << 3
#define CNPX_PAYLOAD_SCHEMA_FIELD_SCHEMAID 1 << 4

enum cnpxPayloadEntryType {
  CNPX_PAYLOAD_ENTRY_TYPE_INVALID,
  CNPX_PAYLOAD_ENTRY_TYPE_SCHAR,
  CNPX_PAYLOAD_ENTRY_TYPE_UCHAR,
  CNPX_PAYLOAD_ENTRY_TYPE_SHORT,
  CNPX_PAYLOAD_ENTRY_TYPE_USHORT,
  CNPX_PAYLOAD_ENTRY_TYPE_INT,
  CNPX_PAYLOAD_ENTRY_TYPE_UINT,
  CNPX_PAYLOAD_ENTRY_TYPE_LONG,
  CNPX_PAYLOAD_ENTRY_TYPE_ULONG,
  CNPX_PAYLOAD_ENTRY_TYPE_LONGLONG,
  CNPX_PAYLOAD_ENTRY_TYPE_ULONGLONG,
  CNPX_PAYLOAD_ENTRY_TYPE_ADDRESS,
  CNPX_PAYLOAD_ENTRY_TYPE_CHAR,
  CNPX_PAYLOAD_ENTRY_TYPE_SIZE,
  CNPX_PAYLOAD_ENTRY_TYPE_META_ARRAY_SIZE,
  CNPX_PAYLOAD_ENTRY_TYPE_META_UPPER_BOUND = 0x100,
  CNPX_PAYLOAD_ENTRY_TYPE_INT8,
  CNPX_PAYLOAD_ENTRY_TYPE_UINT8,
  CNPX_PAYLOAD_ENTRY_TYPE_INT16,
  CNPX_PAYLOAD_ENTRY_TYPE_UINT16,
  CNPX_PAYLOAD_ENTRY_TYPE_INT32,
  CNPX_PAYLOAD_ENTRY_TYPE_UINT32,
  CNPX_PAYLOAD_ENTRY_TYPE_INT64,
  CNPX_PAYLOAD_ENTRY_TYPE_UINT64,
  CNPX_PAYLOAD_ENTRY_TYPE_FLOAT,
  CNPX_PAYLOAD_ENTRY_TYPE_DOUBLE,
  CNPX_PAYLOAD_ENTRY_TYPE_LONGDOUBLE,
  CNPX_PAYLOAD_ENTRY_TYPE_BYTE,
  CNPX_PAYLOAD_ENTRY_TYPE_INT128,
  CNPX_PAYLOAD_ENTRY_TYPE_UINT128,
  CNPX_PAYLOAD_ENTRY_TYPE_FLOAT16,
  CNPX_PAYLOAD_ENTRY_TYPE_FLOAT32,
  CNPX_PAYLOAD_ENTRY_TYPE_FLOAT64,
  CNPX_PAYLOAD_ENTRY_TYPE_FLOAT128,
  CNPX_PAYLOAD_ENTRY_TYPE_BF16,
  CNPX_PAYLOAD_ENTRY_TYPE_TF32,
  CNPX_PAYLOAD_ENTRY_TYPE_CATEGORY,
  CNPX_PAYLOAD_ENTRY_TYPE_COLOR_ARGB,
  CNPX_PAYLOAD_ENTRY_TYPE_SCOPE_ID,
  CNPX_PAYLOAD_ENTRY_TYPE_TID_UINT32,
  CNPX_PAYLOAD_ENTRY_TYPE_TID_UINT64,
  CNPX_PAYLOAD_ENTRY_TYPE_CSTRING,
  CNPX_PAYLOAD_ENTRY_TYPE_CNPX_REGISTERED_STRING_HANDLE,
  CNPX_PAYLOAD_ENTRY_TYPE_PAYLOAD_SCHEMA_REFERENCED,
};
#define CNPX_PAYLOAD_SCHEMA_STATIC_START (0x1ULL << 24)
#define CNPX_PAYLOAD_SCHEMA_DYNIMIC_START (0x1ULL << 32)
#define CNPX_PAYLOAD_ENTRY_FLAG_NONE 0
#define CNPX_PAYLOAD_ENTRY_FLAG_POINTER 1 << 0
#define CNPX_PAYLOAD_ENTRY_FLAG_OFFSET_FROM_BASE 1 << 1
#define CNPX_PAYLOAD_ENTRY_FLAG_ARRAY_FIXED_SIZE 1 << 2
#define CNPX_PAYLOAD_ENTRY_FLAG_ARRAY_ZERO_TERMINATED 2 << 2
#define CNPX_PAYLOAD_ENTRY_FLAG_ARRAY_LENGTH_INDEX 3 << 2
#define CNPX_PAYLOAD_ENTRY_FLAG_DEEP_COPY 1 << 4
#define CNPX_PAYLOAD_ENTRY_FLAG_HIDE 1 << 5
#define CNPX_PAYLOAD_ENTRY_FLAG_EVENT_TIMESTAMP 1 << 6
#define CNPX_PAYLOAD_ENTRY_FLAG_EVENT_MESSAGE 1 << 7
#define CNPX_PAYLOAD_ENTRY_FLAG_EVENT_NAME 1 << 8

typedef struct cnpxPayloadEnum_v1 {
  // The name of the enum value.
  const char* name;
  // The value of the enum value.
  uint64_t value;
  // Indicates whether this value is a bit flag, used to optimize display logic.
  int8_t isFlag;
} cnpxPayloadEnum_t;
typedef struct cnpxPayloadEnumAttr_v1 {
  // Each field in the structure corresponds to a macro.
  // The macros corresponding to the fields filled in the structure are combined into a bit mask,
  // which indicates which fields in the structure are valid.
  uint64_t fieldMask;
  // The name of the enum value (optional).
  const char* name;
  // Each item of the enum value is a `cnpxPayloadEnum_t`, and all information is stored in `entries`.
  const cnpxPayloadEnum_t* entries;
  // The number of entries.
  size_t numEntries;
  // The size of the enum value.
  size_t sizeOfEnum;
  // The schemaId of the enum value (optional). If the user wishes to specify a static schemaId, this item needs to be filled in.
  uint64_t schemaId;
} cnpxPayloadEnumAttr_t;

#define CNPX_PAYLOAD_ENUM_FIELD_NAME 1 << 0
#define CNPX_PAYLOAD_ENUM_FIELD_DESCRIPTION 1 << 1
#define CNPX_PAYLOAD_ENUM_FIELD_SIZE 1 << 2
#define CNPX_PAYLOAD_ENUM_FIELD_SCHEMAID 1 << 3

/**
 * \brief:  Register a string in the domain for future use.
 * \param:  cnpxDomainHandle_t  --  the domain handle
 * \param:  string  --  the string to be registered
 * \return:  uint32_t  --  the string handle
 * */
CNPX_DECLDPEC uint32_t cnpxDomainRegisterString(cnpxDomainHandle_t domain, const char* string);
/**
 * \brief:  Register a Schema in the domain for future use.
 * \param:  domain  --  the domain handle
 * \param:  attr  --  the attribute of the schema
 * \return:  uint64_t  --  the schemaId of the registered schema
 * */
CNPX_DECLDPEC uint64_t cnpxPayloadSchemaRegister(cnpxDomainHandle_t domain, const cnpxPayloadSchemaAttr_t* attr);
/**
 * \brief:  Register a Enum in the domain for future use.
 * @param  domain  -- the domain handle
 * @param  attr  -- the attribute of the enum
 * @return  uint64_t  -- the schemaId of the registered enum
 */
CNPX_DECLDPEC uint64_t cnpxPayloadEnumRegister(cnpxDomainHandle_t domain, const cnpxPayloadEnumAttr_t* attr);
/**
 * \brief:  Mark an instantaneous event with payload in a domain
 * \param:  domain  --  the domain handle
 * \param:  payloadData  --  pointer to the payload data array
 * \param:  count  --  the number of payload data
 * \return:  void
 * */
CNPX_DECLDPEC void cnpxMarkPayload(cnpxDomainHandle_t domain, const cnpxPayloadData_t* payloadData, size_t count);
/**
 * \brief:  Push an instantaneous event with payload in a domain
 * \param:  domain  --  the domain handle
 * \param:  payloadData  --  pointer to the payload data array
 * \param:  count  --  the number of payload data
 * \return:  Reserved. If an error occurs, a negative value is returned.
 * */
CNPX_DECLDPEC int cnpxRangePushPayload(cnpxDomainHandle_t domain, const cnpxPayloadData_t* payloadData, size_t count);
/**
 * \brief:  Pop an instantaneous event with payload in a domain
 * \param:  domain  --  the domain handle
 * \param:  payloadData  --  pointer to the payload data array
 * \param:  count  --  the number of payload data
 * \return:  Reserved. If an error occurs, a negative value is returned.
 * */
CNPX_DECLDPEC int cnpxRangePopPayload(cnpxDomainHandle_t domain, const cnpxPayloadData_t* payloadData, size_t count);
/**
 * \brief:  Start a range event with payload in a domain
 * \param:  domain  --  the domain handle
 * \param:  handle  --  the handle of the range event
 * \param:  payloadData  --  pointer to the payload data array
 * \param:  count  --  the number of payload data
 * \return:  Reserved. If an error occurs, a negative value is returned.
 * */
CNPX_DECLDPEC int cnpxRangeStartPayload(cnpxDomainHandle_t domain, void** handle, const cnpxPayloadData_t* payloadData,
                                        size_t count);
/**
 * \brief:  End a range event with payload in a domain
 * \param:  domain  --  the domain handle
 * \param:  handle  --  the handle of the range event
 * \param:  payloadData  --  pointer to the payload data array
 * \param:  count  --  the number of payload data
 * \return:  Reserved. If an error occurs, a negative value is returned.
 * */
CNPX_DECLDPEC int cnpxRangeEndPayload(cnpxDomainHandle_t domain, void* handle, const cnpxPayloadData_t* payloadData,
                                      size_t count);
#endif  // CNPX_CNPX_PAYLOAD_H
