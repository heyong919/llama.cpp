#ifndef CNPX_CNPX_MACRO_UTILS_H
#define CNPX_CNPX_MACRO_UTILS_H

#define _CNPX_CONCAT(a, b) a##b
#define CNPX_CONCAT(a, b) _CNPX_CONCAT(a, b)
#define CNPX_NUM_ARGS(...)                                                                                                      \
  CNPX_SELECTA32(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, \
                 7, 6, 5, 4, 3, 2, 1, throwaway)
#define CNPX_SELECTA32(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, \
                       a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, ...)                                                    \
  a33

#define CNPX_HELPER_FOREACH_1(func, userdata, a, ...) func(userdata, 0, a)
#define CNPX_HELPER_FOREACH_2(func, userdata, a, ...) func(userdata, 1, a) CNPX_HELPER_FOREACH_1(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_3(func, userdata, a, ...) func(userdata, 2, a) CNPX_HELPER_FOREACH_2(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_4(func, userdata, a, ...) func(userdata, 3, a) CNPX_HELPER_FOREACH_3(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_5(func, userdata, a, ...) func(userdata, 4, a) CNPX_HELPER_FOREACH_4(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_6(func, userdata, a, ...) func(userdata, 5, a) CNPX_HELPER_FOREACH_5(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_7(func, userdata, a, ...) func(userdata, 6, a) CNPX_HELPER_FOREACH_6(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_8(func, userdata, a, ...) func(userdata, 7, a) CNPX_HELPER_FOREACH_7(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_9(func, userdata, a, ...) func(userdata, 8, a) CNPX_HELPER_FOREACH_8(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_10(func, userdata, a, ...) func(userdata, 9, a) CNPX_HELPER_FOREACH_9(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_11(func, userdata, a, ...) func(userdata, 10, a) CNPX_HELPER_FOREACH_10(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_12(func, userdata, a, ...) func(userdata, 11, a) CNPX_HELPER_FOREACH_11(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_13(func, userdata, a, ...) func(userdata, 12, a) CNPX_HELPER_FOREACH_12(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_14(func, userdata, a, ...) func(userdata, 13, a) CNPX_HELPER_FOREACH_13(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_15(func, userdata, a, ...) func(userdata, 14, a) CNPX_HELPER_FOREACH_14(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_16(func, userdata, a, ...) func(userdata, 15, a) CNPX_HELPER_FOREACH_15(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_17(func, userdata, a, ...) func(userdata, 16, a) CNPX_HELPER_FOREACH_16(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_18(func, userdata, a, ...) func(userdata, 17, a) CNPX_HELPER_FOREACH_17(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_19(func, userdata, a, ...) func(userdata, 18, a) CNPX_HELPER_FOREACH_18(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_20(func, userdata, a, ...) func(userdata, 19, a) CNPX_HELPER_FOREACH_19(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_21(func, userdata, a, ...) func(userdata, 20, a) CNPX_HELPER_FOREACH_20(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_22(func, userdata, a, ...) func(userdata, 21, a) CNPX_HELPER_FOREACH_21(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_23(func, userdata, a, ...) func(userdata, 22, a) CNPX_HELPER_FOREACH_22(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_24(func, userdata, a, ...) func(userdata, 23, a) CNPX_HELPER_FOREACH_23(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_25(func, userdata, a, ...) func(userdata, 24, a) CNPX_HELPER_FOREACH_24(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_26(func, userdata, a, ...) func(userdata, 25, a) CNPX_HELPER_FOREACH_25(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_27(func, userdata, a, ...) func(userdata, 26, a) CNPX_HELPER_FOREACH_26(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_28(func, userdata, a, ...) func(userdata, 27, a) CNPX_HELPER_FOREACH_27(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_29(func, userdata, a, ...) func(userdata, 28, a) CNPX_HELPER_FOREACH_28(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_30(func, userdata, a, ...) func(userdata, 29, a) CNPX_HELPER_FOREACH_29(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_31(func, userdata, a, ...) func(userdata, 30, a) CNPX_HELPER_FOREACH_30(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_32(func, userdata, a, ...) func(userdata, 31, a) CNPX_HELPER_FOREACH_31(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH_PASSTHROUGH(...) __VA_ARGS__

#define CNPX_HELPER_FOREACH_IMPL(func, userdata, ...) \
  CNPX_CONCAT(CNPX_HELPER_FOREACH_, CNPX_NUM_ARGS(__VA_ARGS__))(func, userdata, __VA_ARGS__)
#define CNPX_HELPER_FOREACH(func, userdata, args) CNPX_HELPER_FOREACH_IMPL(func, userdata, CNPX_HELPER_FOREACH_PASSTHROUGH args)

#endif  // CNPX_CNPX_MACRO_UTILS_H
