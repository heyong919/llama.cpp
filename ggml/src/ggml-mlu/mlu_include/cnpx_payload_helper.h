#ifndef CNPX_CNPX_PAYLOAD_HELPER_H
#define CNPX_CNPX_PAYLOAD_HELPER_H
#include "cnpx_macro_utils.h"
#define CNPX_PAYLOAD_ENTRY_THROWAWAY
#define CNPX_PAYLOAD_PASS_THROUGH(...) __VA_ARGS__
#define CNPX_PAYLOAD_NESTED(schemaId) THROWAWAY schemaId

#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX_2(member, etype) 0, CNPX_PAYLOAD_ENTRY_##etype, NULL, NULL, 0,
#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX_3(member, etype, name) 0, CNPX_PAYLOAD_ENTRY_##etype, name, NULL, 0,
#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX_4(member, etype, name, desc) 0, CNPX_PAYLOAD_ENTRY_##etype, name, desc, 0,
#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX_5(member, etype, name, desc, arraylen) \
  0, CNPX_PAYLOAD_ENTRY_##etype, name, desc, arraylen,
#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX_6(member, etype, name, desc, arraylen, flags) \
  CNPX_PAYLOAD_ENTRY_FLAG_##flags, CNPX_PAYLOAD_ENTRY_##etype, name, desc, arraylen,

#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX(...) \
  CNPX_CONCAT(CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX_, CNPX_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_MEMBER(member, ...) member

#define CNPX_PAYLOAD_SCHEMA_ATTR(struct_id, schema_name, schema_id, mask_add)                        \
  cnpxPayloadSchemaAttr_t struct_id##Attr = {                                                        \
      /*.fieldMask = */ mask_add | CNPX_PAYLOAD_SCHEMA_FIELD_SIZE | CNPX_PAYLOAD_SCHEMA_FIELD_ALIGN, \
      /*.name = */ schema_name,                                                                      \
      /*.entries = */ struct_id##Schema,                                                             \
      /*.numEntries = */ sizeof(struct_id##Schema) / sizeof(struct_id##Schema[0]),                   \
      /*.payloadStaticSize = */ sizeof(struct_id),                                                   \
      /*.packAlign = */ CNPX_ALIGN(struct_id),                                                       \
      /*.schemaId = */ schema_id};

#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY(struct_id, index, entry) \
  {CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_PREFIX entry offsetof(struct_id, CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY_MEMBER entry)},

#define CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRIES(struct_id, entries) \
  cnpxPayloadSchemaEntry_t struct_id##Schema[] = {CNPX_HELPER_FOREACH(CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRY, struct_id, entries)};
#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_1(FIX_ARG_HANDLER, ENTRIES_HANDLER, struct_id, entries) \
  FIX_ARG_HANDLER(struct_id, entries)                                                         \
  CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRIES(struct_id, ENTRIES_HANDLER(entries))                 \
  CNPX_PAYLOAD_SCHEMA_ATTR(struct_id, NULL, 0, 0)

#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_2(FIX_ARG_HANDLER, ENTRIES_HANDLER, struct_id, schema_name, entries) \
  FIX_ARG_HANDLER(struct_id, entries)                                                                      \
  CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRIES(struct_id, ENTRIES_HANDLER(entries))                              \
  CNPX_PAYLOAD_SCHEMA_ATTR(struct_id, schema_name, 0, CNPX_PAYLOAD_SCHEMA_FIELD_NAME)

#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_3(FIX_ARG_HANDLER, ENTRIES_HANDLER, struct_id, schema_name, prefix, entries) \
  FIX_ARG_HANDLER(struct_id, entries)                                                                              \
  prefix CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRIES(struct_id, ENTRIES_HANDLER(entries))                               \
      prefix CNPX_PAYLOAD_SCHEMA_ATTR(struct_id, schema_name, 0, CNPX_PAYLOAD_SCHEMA_FIELD_NAME)

#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_4(FIX_ARG_HANDLER, ENTRIES_HANDLER, struct_id, schema_name, prefix, schema_id, entries) \
  FIX_ARG_HANDLER(struct_id, entries)                                                                                         \
  prefix CNPX_PAYLOAD_SCHEMA_FOR_STRUCT_ENTRIES(struct_id, ENTRIES_HANDLER(entries)) prefix CNPX_PAYLOAD_SCHEMA_ATTR(         \
      struct_id, schema_name, schema_id, CNPX_PAYLOAD_SCHEMA_FIELD_NAME | CNPX_PAYLOAD_SCHEMA_FIELD_SCHEMAID)

#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_FIX_ARG_HANDLER(...)
#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_ENTRIES_HANDLER(a) a

#define CNPX_DEFINE_SCHEMA_FOR_STRUCT(struct_id, ...)                     \
  CNPX_CONCAT(CNPX_DEFINE_SCHEMA_FOR_STRUCT_, CNPX_NUM_ARGS(__VA_ARGS__)) \
  (CNPX_DEFINE_SCHEMA_FOR_STRUCT_FIX_ARG_HANDLER, CNPX_DEFINE_SCHEMA_FOR_STRUCT_ENTRIES_HANDLER, struct_id, __VA_ARGS__)
#define CNPX_PAYLOAD_SCHEMA_REGISTER(domain, struct_name) cnpxPayloadSchemaRegister(domain, &struct_name##Attr)

/* Extract struct member for fixed-size arrays. */
#define CNPX_PAYLOAD_STRUCT_ARR_MEM1(name) name
#define CNPX_PAYLOAD_STRUCT_ARR_MEM2(name, count) name[count]
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR1(name) name
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR2(name, count) name

#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR_3(ctype, member, etype) , (member, etype)
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR_4(ctype, member, etype, name) , (member, etype, name)
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR_5(ctype, member, etype, name, desc) , (member, etype, name, desc)
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR_6(ctype, member, etype, name, desc, arraylen) \
  , (CNPX_CONCAT(CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR, CNPX_NUM_ARGS member) member, etype, name, desc, arraylen)
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR_7(ctype, member, etype, name, desc, arraylen, flags) \
  , (CNPX_CONCAT(CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR, CNPX_NUM_ARGS member) member, etype, name, desc, arraylen, flags)

#define CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR(data, i, entry) \
  CNPX_CONCAT(CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR_, CNPX_NUM_ARGS entry) entry
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR_STRIP_BEGIN_(nothing, ...) __VA_ARGS__
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR_STRIP_BEGIN(arg) CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR_STRIP_BEGIN_(arg)
#define CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR_(entries) \
  CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR_STRIP_BEGIN(CNPX_HELPER_FOREACH(CNPX_PAYLOAD_CONVERT_WITH_ENTRY_TO_FOR, ff, entries))

/* Extract type and member name and handle special case of fixed-size array. */
#define CNPX_PAYLOAD_STRUCT_2(type, member) type member;
#define CNPX_PAYLOAD_STRUCT_3(type, member, etype) type member;
#define CNPX_PAYLOAD_STRUCT_4(type, member, etype, name) type member;
#define CNPX_PAYLOAD_STRUCT_5(type, member, etype, name, desc) type member;
#define CNPX_PAYLOAD_STRUCT_6(type, member, etype, name, desc, arraylen) \
  type CNPX_CONCAT(CNPX_PAYLOAD_STRUCT_ARR_MEM, CNPX_NUM_ARGS member) member;
#define CNPX_PAYLOAD_STRUCT_7(type, member, etype, name, desc, arraylen, flags) \
  CNPX_PAYLOAD_STRUCT_6(type, member, etype, name, desc, arraylen)

#define CNPX_PAYLOAD_STRUCT_ENTRY(data, i, entry) CNPX_CONCAT(CNPX_PAYLOAD_STRUCT_, CNPX_NUM_ARGS entry) entry

#define CNPX_PAYLOAD_TYPEDEF_STRUCT(struct_id, args)       \
  typedef struct {                                         \
    CNPX_HELPER_FOREACH(CNPX_PAYLOAD_STRUCT_ENTRY, , args) \
  } struct_id;

#define CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR(entries) (CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR_(entries))

#define CNPX_DEFINE_STRUCT_WITH_SCHEMA(struct_id, ...)                    \
  CNPX_CONCAT(CNPX_DEFINE_SCHEMA_FOR_STRUCT_, CNPX_NUM_ARGS(__VA_ARGS__)) \
  (CNPX_PAYLOAD_TYPEDEF_STRUCT, CNPX_PAYLOAD_CONVERT_WITH_ENTRIES_TO_FOR, struct_id, __VA_ARGS__)

#define CNPX_DEFINE_STRUCT_WITH_SCHEMA_AND_REGISTER(domain_id, struct_id, ...) \
  CNPX_DEFINE_STRUCT_WITH_SCHEMA(struct_id, __VA_ARGS__)                       \
  const uint64_t struct_id##_schemaId = CNPX_PAYLOAD_SCHEMA_REGISTER(domain_id, struct_id);

#define CNPX_DEFINE_SCHEMA_FOR_STRUCT_AND_REGISTER(domain_id, struct_id, ...) \
  CNPX_DEFINE_SCHEMA_FOR_STRUCT_AND(struct_id, __VA_ARGS__)                   \
  const uint64_t struct_id##_schemaId = CNPX_PAYLOAD_SCHEMA_REGISTER(domain_id, struct_id);
#endif  // CNPX_CNPX_PAYLOAD_HELPER_H
