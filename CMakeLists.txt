# Copyright (C) 2023 Tayne
#
# This file is part of cini.
# 
# cini is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# 指定CMake的最小版本要求
cmake_minimum_required(VERSION 3.12)

# 项目名称
set(PROJECT cini)
set(STATICLIB static_lib_${PROJECT})
set(SHAREDLIB shared_lib_${PROJECT})
set(MAINAPP main_${PROJECT})
set(TESTAPP test_${PROJECT})

# 工作路径
set(WORK_DIR ${CMAKE_SOURCE_DIR})

# 头文件路径
set(INC_DIR ${WORK_DIR}/src)

# 源文件路径
set(SRC_DIR ${WORK_DIR}/src)

# 构建路径
set(BUILD_DIR ${WORK_DIR}/build)

# 目标文件路径
set(BIN_DIR ${BUILD_DIR}/bin)

# 设置构建路径
set(CMAKE_BINARY_DIR ${BUILD_DIR})

# 定义版本号宏
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# 指定项目版本和语言
project(${PROJECT}
    VERSION ${PROJECT_VERSION}
    LANGUAGES C
    DESCRIPTION "A simple ini config parser"
    HOMEPAGE_URL "https://github.com/tayne3/CINI"
)

# 判断编译类型是否定义
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 根据编译类型不同
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug") # 用于调试目的，包含符号表和未优化的代码 (-O0 -g)
    set(PROJECT_DEBUG_FLAG 1)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release") # 用于发布目的，包含优化的代码，并且通常不包含符号表 (-O3 -DNDEBUG)
    set(PROJECT_DEBUG_FLAG 0)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo") # 包含优化的代码和符号表，用于在发生错误时进行调试 (-O2 -g -DNDEBUG)
    set(PROJECT_DEBUG_FLAG 1)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel") # 最小化二进制文件大小，启用优化并删除不必要的符号 (-Os -DNDEBUG)
    set(PROJECT_DEBUG_FLAG 0)
else()
    message(FATAL_ERROR "Unsupported build type \"${CMAKE_BUILD_TYPE}\".")
endif()

# 设置目标平台操作系统名称
# set(CMAKE_SYSTEM_NAME Linux)
# 设置目标平台处理器架构
# set(CMAKE_SYSTEM_PROCESSOR x86_64)
# 构建时生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 设置C标准版本
set(CMAKE_C_STANDARD 99)
# 设置C++标准版本
# set(CMAKE_CXX_STANDARD 11)
# 要求完全支持C++标准
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 禁用特定的C++编译器扩展
# set(CMAKE_CXX_EXTENSIONS ON)
# 设置并行编译的级别为 4
# set(CMAKE_BUILD_PARALLEL_LEVEL 4)
# 设置交叉工具链路径
# set(CMAKE_TOOLCHAIN_FILE )

# 定义公共源文件
set(COMMON_SRCS
)

# 定义源文件
set(SHARED_SRCS ${COMMON_SRCS}
    ${SRC_DIR}/core/cini.c
)

# 定义动态库
add_library(${SHAREDLIB} SHARED ${SHARED_SRCS})

# 设置编译选项
target_compile_options(${SHAREDLIB} PRIVATE
    -Wall                               #启用常见警告
    -Wextra                             #启用额外警告
    -Wconversion                        #检查类型转换 
    -Wsign-conversion                   #检查符号转换
    -Wstrict-aliasing                   #增强类型别名检查
    -Wundef                             #检查未定义宏 
    -Wshadow                            #检查变量遮蔽 
    -Wcast-align                        #检查指针对齐
    -Wstrict-prototypes                 #检查函数原型
    -Wmissing-declarations              #检查缺失声明 
    -Wstrict-overflow                   #检查求值溢出
    -Wno-deprecated-declarations        #禁用已废弃声明警告
    -pedantic                           #要求代码严格符合C/C++标准
    -pedantic-errors                    #将不符合标准的代码作为错误处理
)

# 头文件路径
target_include_directories(${SHAREDLIB} PRIVATE
    ${INC_DIR}
)

# 设置目标属性
SET_TARGET_PROPERTIES(${SHAREDLIB} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR} # 设置输出路径
    OUTPUT_NAME ${PROJECT}              # 设置输出名称
)

# 添加宏定义
target_compile_definitions(${SHAREDLIB} PRIVATE
    CINI_LIBRARY
)

# 定义源文件
set(STATIC_SRCS ${COMMON_SRCS}
    ${SRC_DIR}/core/cini.c
)

# 定义静态库
add_library(${STATICLIB} STATIC ${STATIC_SRCS})

# 设置编译选项   
target_compile_options(${STATICLIB} PRIVATE
    -Wall                               #启用常见警告
    -Wextra                             #启用额外警告
    -Wconversion                        #检查类型转换 
    -Wsign-conversion                   #检查符号转换
    -Wstrict-aliasing                   #增强类型别名检查
    -Wundef                             #检查未定义宏 
    -Wshadow                            #检查变量遮蔽 
    -Wcast-align                        #检查指针对齐
    -Wstrict-prototypes                 #检查函数原型
    -Wmissing-declarations              #检查缺失声明 
    -Wstrict-overflow                   #检查求值溢出
    -Wno-deprecated-declarations        #禁用已废弃声明警告
    -pedantic                           #要求代码严格符合C/C++标准
    -pedantic-errors                    #将不符合标准的代码作为错误处理
)

# 头文件路径
target_include_directories(${STATICLIB} PRIVATE
    ${INC_DIR}
)

# 设置目标属性
SET_TARGET_PROPERTIES(${STATICLIB} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR} # 设置输出路径
    OUTPUT_NAME ${PROJECT}              # 设置输出名称
)

# 添加宏定义
target_compile_definitions(${STATICLIB} PRIVATE
)

# 定义源文件
set(MAIN_SRCS ${COMMON_SRCS}
    ${SRC_DIR}/example/main.c
)

# 定义目标文件
add_executable(${MAINAPP} ${MAIN_SRCS})

# 链接 libcini库
target_link_libraries(${MAINAPP} ${SHAREDLIB})
# target_link_libraries(${MAINAPP} ${STATICLIB})

# 设置编译选项
target_compile_options(${MAINAPP} PRIVATE
    -Wall                               #启用常见警告
    -Wextra                             #启用额外警告
    -Wconversion                        #检查类型转换 
    -Wsign-conversion                   #检查符号转换
    -Wstrict-aliasing                   #增强类型别名检查
    -Wundef                             #检查未定义宏 
    -Wshadow                            #检查变量遮蔽 
    -Wcast-align                        #检查指针对齐
    -Wstrict-prototypes                 #检查函数原型
    -Wmissing-declarations              #检查缺失声明 
    -Wstrict-overflow                   #检查求值溢出
    -Wno-deprecated-declarations        #禁用已废弃声明警告
    -pedantic                           #要求代码严格符合C/C++标准
    -pedantic-errors                    #将不符合标准的代码作为错误处理
)

# 头文件路径
target_include_directories(${MAINAPP} PRIVATE
    ${INC_DIR}
)

# 设置目标属性
SET_TARGET_PROPERTIES(${MAINAPP} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} # 设置输出路径
    OUTPUT_NAME ${PROJECT}              # 设置输出名称
)

# 添加宏定义
target_compile_definitions(${MAINAPP} PRIVATE  
    PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    PROJECT_DEBUG_FLAG=${PROJECT_DEBUG_FLAG}
    PROJECT_VERSION="${PROJECT_VERSION}"
)

# 添加源文件
set(TEST_SRCS ${COMMON_SRCS}
    ${SRC_DIR}/test/ctest_item.c
    ${SRC_DIR}/test/main.c
)

# 定义目标文件
add_executable(${TESTAPP} ${TEST_SRCS})

# 链接 libcini库
target_link_libraries(${TESTAPP} ${SHAREDLIB})
# target_link_libraries(${TESTAPP} ${STATICLIB})

# 设置编译选项
target_compile_options(${TESTAPP} PRIVATE
    -Wall                               #启用常见警告
    -Wextra                             #启用额外警告
    -Wconversion                        #检查类型转换 
    -Wsign-conversion                   #检查符号转换
    -Wstrict-aliasing                   #增强类型别名检查
    -Wundef                             #检查未定义宏 
    -Wshadow                            #检查变量遮蔽 
    -Wcast-align                        #检查指针对齐
    -Wstrict-prototypes                 #检查函数原型
    -Wmissing-declarations              #检查缺失声明 
    -Wstrict-overflow                   #检查求值溢出
    -Wno-deprecated-declarations        #禁用已废弃声明警告
    -pedantic                           #要求代码严格符合C/C++标准
    -pedantic-errors                    #将不符合标准的代码作为错误处理
)

# 头文件路径
target_include_directories(${TESTAPP} PRIVATE
    ${INC_DIR}
)

# 设置目标属性
SET_TARGET_PROPERTIES(${TESTAPP} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} # 设置输出路径
    OUTPUT_NAME ${TESTAPP}              # 设置输出名称
)