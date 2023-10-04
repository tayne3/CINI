/*
 * Copyright (C) 2023 Tayne
 *
 * This file is part of cini.
 *
 * cini is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cini is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with cini.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _CINI_H
#define _CINI_H

#include <stddef.h>

// clang-format off

// bool type
# ifndef __cplusplus
#   include <stdbool.h>
#   ifndef _Bool
#       define _Bool int
#   endif
#   ifndef true
#       define true 1
#   endif
#   ifndef false
#       define false 0
#   endif
#   ifndef bool
#       define bool _Bool
#   endif
# endif

# if defined(_WIN32) || defined(_WIN64)         // Windows 
#   define __C_PLATFORM_WIN
# elif defined(__linux__) || defined(__linux)   // Linux
#   define __C_PLATFORM_LINUX
#elif defined(__APPLE__) || defined(__MACH__)   // Mac
#   define __C_PLATFORM_MAC
# elif defined(__wasm__)                        // WebAssembly
#   define __C_PLATFORM_WASM
# else                                          // Other
#   error Unknown platform!
# endif

#if defined(__C_PLATFORM_WIN)
    #define __C_DECL_EXPORT __declspec(dllexport)
    #define __C_DECL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    #define __C_DECL_EXPORT __attribute__ ((visibility ("default")))
    #define __C_DECL_IMPORT __attribute__ ((visibility ("default")))
#else
    #define __C_DECL_EXPORT
    #define __C_DECL_IMPORT
#endif

# if defined(CINI_LIBRARY)
#  define CINI_EXPORT __C_DECL_EXPORT
# else
#  define CINI_EXPORT __C_DECL_IMPORT
# endif

# ifndef STR_NULL
#   define STR_NULL ("")
# endif

# ifdef __C_PLATFORM_WIN
#   define STR_NEWLINE      "\r\n"
# else
#   define STR_NEWLINE      "\n"
# endif
// clang-format on

// cini配置结构体
typedef struct cini cini_t;

/**
 * @brief cini配置结构体
 * 用于存储cini配置文件的路径和当前组的信息
 */
struct cini {
    const char *path;         // 配置文件路径
    const char *group_name;   // 当前组名称
    size_t      group_start;  // 当前组起始行
    size_t      group_end;    // 当前组结束行
};

#define CINI_INITIALIZATION                                                                                            \
    {                                                                                                                  \
        .path = STR_NULL, .group_name = STR_NULL, .group_start = 0, .group_end = 0                                     \
    }

#define CINI_NULL (cini_t) CINI_INITIALIZATION

/**
 * @brief 获取配置文件路径
 * @param self cini指针
 * @return const char* 配置文件路径
 */
CINI_EXPORT const char *cini_path_get(cini_t *self);

/**
 * @brief 设置配置文件路径
 * @param self cini指针
 * @param path 配置文件路径
 */
CINI_EXPORT void cini_path_set(cini_t *self, const char *path);

/**
 * @brief 打开组
 * @param self cini指针
 * @param group 组名称
 */
CINI_EXPORT void cini_group_begin(cini_t *self, const char *group);

/**
 * @brief 关闭组
 * @param self cini指针
 */
CINI_EXPORT void cini_group_end(cini_t *self);

/**
 * @brief 获取当前组名称
 * @param self cini指针
 * @return const char* 当前组名称
 */
CINI_EXPORT const char *cini_group_get(cini_t *self);

/**
 * @brief 获取当前组中指定键的值
 * @param self cini指针
 * @param key 键名称
 * @param default_value 默认值
 * @param buffer 存储值的缓冲区
 * @param max 缓冲区大小
 */
CINI_EXPORT void cini_value_get(cini_t *self, const char *key, const char *default_value, char *buffer, size_t max);

/**
 * @brief 设置当前组中指定键的值
 * @param self cini指针
 * @param key 键名称
 * @param value 修改值
 */
CINI_EXPORT void cini_value_set(cini_t *self, const char *key, char *value);

/**
 * @brief 从当前组中移除指定键值对
 * @param self cini指针
 * @param key 键名称
 */
CINI_EXPORT void cini_value_remove(cini_t *self, const char *key);

/**
 * @brief 判断当前组中指定键是否存在
 * @param self cini指针
 * @param key 键名称
 * @return bool 存在返回true，不存在返回false
 */
CINI_EXPORT bool cini_value_contains(cini_t *self, const char *key);

#endif
