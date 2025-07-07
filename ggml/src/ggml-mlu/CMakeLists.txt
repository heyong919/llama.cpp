# 从环境变量获取NEUWARE_HOME路径，支持多个可能的安装位置
if (DEFINED ENV{NEUWARE_HOME} AND EXISTS $ENV{NEUWARE_HOME})
    set(NEUWARE_HOME $ENV{NEUWARE_HOME})
elseif (EXISTS "/usr/local/neuware")
    set(NEUWARE_HOME "/usr/local/neuware")
else()
    message(WARNING "NEUWARE_HOME not found. Please set NEUWARE_HOME environment variable or install MLU SDK to standard location.")
    return()
endif()

message(STATUS "NEUWARE_HOME: ${NEUWARE_HOME}")

# 设置MLU SDK的头文件和库文件路径
set(MLU_INCLUDE_DIR "${NEUWARE_HOME}/include")
set(MLU_LIBRARY_DIR "${NEUWARE_HOME}/lib64")

# 检查头文件目录是否存在
if (NOT EXISTS "${MLU_INCLUDE_DIR}")
    message(WARNING "MLU include directory not found: ${MLU_INCLUDE_DIR}")
    return()
endif()

# 检查库文件目录是否存在
if (NOT EXISTS "${MLU_LIBRARY_DIR}")
    message(WARNING "MLU library directory not found: ${MLU_LIBRARY_DIR}")
    return()
endif()

# 查找MLU SDK库
find_library(CNRT_LIBRARY cnrt PATHS ${MLU_LIBRARY_DIR} NO_DEFAULT_PATH)
find_library(CNNL_LIBRARY cnnl PATHS ${MLU_LIBRARY_DIR} NO_DEFAULT_PATH)
find_library(CNNL_EXTRA_LIBRARY cnnl_extra PATHS ${MLU_LIBRARY_DIR} NO_DEFAULT_PATH)
find_library(CNDEV_LIBRARY cndev PATHS ${MLU_LIBRARY_DIR} NO_DEFAULT_PATH)

# 检查所有必需的库是否找到
set(MLU_LIBRARIES_MISSING "")
if (NOT CNRT_LIBRARY)
    list(APPEND MLU_LIBRARIES_MISSING "cnrt")
endif()
if (NOT CNNL_LIBRARY)
    list(APPEND MLU_LIBRARIES_MISSING "cnnl")
endif()
if (NOT CNNL_EXTRA_LIBRARY)
    list(APPEND MLU_LIBRARIES_MISSING "cnnl_extra")
endif()
if (NOT CNDEV_LIBRARY)
    list(APPEND MLU_LIBRARIES_MISSING "cndev")
endif()

if (MLU_LIBRARIES_MISSING)
    message(WARNING "MLU libraries not found: ${MLU_LIBRARIES_MISSING}. Please check NEUWARE_HOME: ${NEUWARE_HOME}")
    return()
endif()

message(STATUS "MLU libraries found:")
message(STATUS "  CNRT: ${CNRT_LIBRARY}")
message(STATUS "  CNNL: ${CNNL_LIBRARY}")
message(STATUS "  CNNL_EXTRA: ${CNNL_EXTRA_LIBRARY}")
message(STATUS "  CNDEV: ${CNDEV_LIBRARY}")

# 获取MLU源文件
file(GLOB GGML_SOURCES_MLU "*.cpp")
list(APPEND GGML_HEADERS_MLU "../../include/ggml-mlu.h")

# 添加MLU后端库
ggml_add_backend_library(ggml-mlu
                         ${GGML_HEADERS_MLU}
                         ${GGML_SOURCES_MLU}
                        )

# 添加编译定义
if (NOT GGML_BACKEND_DL)
    target_compile_definitions(ggml PUBLIC GGML_USE_MLU)
endif()

# 添加MLU库依赖
target_include_directories(ggml-mlu PRIVATE ${MLU_INCLUDE_DIR})
target_include_directories(ggml-mlu PRIVATE ..)
target_link_directories(ggml-mlu PRIVATE ${MLU_LIBRARY_DIR})
target_link_libraries(ggml-mlu PRIVATE ${CNRT_LIBRARY} ${CNNL_LIBRARY} ${CNNL_EXTRA_LIBRARY} ${CNDEV_LIBRARY})

# 添加编译选项
target_compile_options(ggml-mlu PRIVATE -fPIC)

message(STATUS "MLU backend configured successfully")