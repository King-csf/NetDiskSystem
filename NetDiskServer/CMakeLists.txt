cmake_minimum_required(VERSION 3.10)
project(NetDiskServer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 设置项目目录
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_DIR ${PROJECT_ROOT_DIR}/include)
set(SRC_DIR ${PROJECT_ROOT_DIR}/src)
set(STORAGE_DIR ${PROJECT_ROOT_DIR}/storage)

# 查找必要的库
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# 查找workflow库
find_path(WORKFLOW_INCLUDE_DIR
    NAMES workflow/Workflow.h
    PATHS /usr/local/include /usr/include
)

find_library(WORKFLOW_LIB
    NAMES workflow
    PATHS /usr/local/lib /usr/lib
)

if(NOT WORKFLOW_INCLUDE_DIR OR NOT WORKFLOW_LIB)
    message(FATAL_ERROR "Workflow library not found. Please install workflow library first.")
endif()

# 包含头文件目录
include_directories(${INCLUDE_DIR})
include_directories(${WORKFLOW_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})

# 收集源文件
file(GLOB_RECURSE SRC_FILES 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.c"
)

# 收集头文件
file(GLOB_RECURSE HEADER_FILES 
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
)

# 添加可执行文件
add_executable(NetDiskServer ${SRC_FILES} ${HEADER_FILES})

# 设置目标属性
target_include_directories(NetDiskServer PRIVATE 
    ${INCLUDE_DIR}
    ${WORKFLOW_INCLUDE_DIR}
)

# 链接库
target_link_libraries(NetDiskServer 
    ${WORKFLOW_LIB}
    ${OPENSSL_LIBRARIES}
    Threads::Threads
    pthread
    ssl
    crypto
)

# 设置输出目录
set_target_properties(NetDiskServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 创建storage目录（如果不存在）
file(MAKE_DIRECTORY ${STORAGE_DIR})

# 复制storage目录到build目录
file(COPY ${STORAGE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# 安装规则
install(TARGETS NetDiskServer
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${STORAGE_DIR}
    DESTINATION .
)

# 打印配置信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Include directory: ${INCLUDE_DIR}")
message(STATUS "Source directory: ${SRC_DIR}")
message(STATUS "Storage directory: ${STORAGE_DIR}")
message(STATUS "Workflow include dir: ${WORKFLOW_INCLUDE_DIR}")
message(STATUS "Workflow library: ${WORKFLOW_LIB}")
message(STATUS "Source files found: ${SRC_FILES}")
message(STATUS "Header files found: ${HEADER_FILES}")