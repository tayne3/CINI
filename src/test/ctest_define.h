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
#ifndef _TEST_DEFINE_H
#define _TEST_DEFINE_H

#include <string.h>
#include <stdio.h>

// 测试函数声明宏
#define C_TEST_FUNC_DECL(name) int ctest_func_##name(int argc, char **argv)

// 测试函数项包装宏
#define C_TEST_FUNC_ITEM(name)                                                                                         \
    {                                                                                                                  \
        #name, ctest_func_##name                                                                                       \
    }

// 测试项
typedef struct ctest_item ctest_item_t;

// 测试项结构体
struct ctest_item {
    const char *const name;                            // 测试项名称
    int               (*func)(int argc, char **argv);  // 测试函数
};

// clang-format off
# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#	define __c_func__       __func__
#	define __c_file__       __FILE__
#	define __c_line__       __LINE__
# elif defined(__GNUC__)
#	define __c_func__       __FUNCTION__
#	define __c_file__       __FILE__
#	define __c_line__       __LINE__
# elif defined(_MSC_VER)
#	define __c_func__       __FUNCTION__
#	define __c_file__       __FILE__
#	define __c_line__       __LINE__
# elif defined(__TINYC__)
#	define __c_func__       __func__
#	define __c_file__       __FILE__
#	define __c_line__       __LINE__
# else
#	define __c_func__       ""
#	define __c_file__       ""
#	define __c_line__       0
# endif

# ifndef __c_unused
#   define __c_unused(x)                              (void)(x)
# endif
# ifndef __c_array_size
#   define __c_array_size(x)                          (sizeof(x) / sizeof((x)[0]))
# endif
# ifndef __c_bool_string
#   define __c_bool_string(x)                         ((x) ? "true" : "false")
# endif
// clang-format on

#define CTEST_CONPARE_CHAR(x) (x) == -1 ? "<" : (x) == 1 ? ">" : "=="

enum ctest_compare_result {
    CTEST_LESS    = 0x01,  // -1
    CTEST_EQUAL   = 0x02,  // 0
    CTEST_GREATER = 0x04,  // 1
};

#define ctest_assert_bool(expr)                                                                                        \
    do {                                                                                                               \
        const bool v = expr;                                                                                           \
        if (!v) {                                                                                                      \
            printf("assert fatal %d: %s \n", __c_line__, __c_bool_string(v));                                          \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_string(expr1, expr2)                                                                              \
    do {                                                                                                               \
        const char *l = (expr1);                                                                                       \
        const char *r = (expr2);                                                                                       \
        if (l != r) {                                                                                                  \
            if (l == NULL || r == NULL || strcmp(l, r) != 0) {                                                         \
                printf("assert fatal %d: %s != %s \n", __c_line__, l, r);                                              \
                return __c_line__;                                                                                     \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define ctest_assert_char(expr1, expr2, cond)                                                                          \
    do {                                                                                                               \
        const char l      = expr1;                                                                                     \
        const char r      = expr2;                                                                                     \
        const bool result = (l > r) - (l < r);                                                                         \
        bool       isok   = false;                                                                                     \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %c %s %c \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_uint(expr1, expr2, cond)                                                                          \
    do {                                                                                                               \
        const uint l      = expr1;                                                                                     \
        const uint r      = expr2;                                                                                     \
        const bool result = (l > r) - (l < r);                                                                         \
        bool       isok   = false;                                                                                     \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %u %s %u \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_int(expr1, expr2, cond)                                                                           \
    do {                                                                                                               \
        const int  l      = expr1;                                                                                     \
        const int  r      = expr2;                                                                                     \
        const bool result = (l > r) - (l < r);                                                                         \
        bool       isok   = false;                                                                                     \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %d %s %d \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_ulong(expr1, expr2, cond)                                                                         \
    do {                                                                                                               \
        const ulong l      = expr1;                                                                                    \
        const ulong r      = expr2;                                                                                    \
        const bool  result = (l > r) - (l < r);                                                                        \
        bool        isok   = false;                                                                                    \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %lu %s %lu \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                    \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_long(expr1, expr2, cond)                                                                          \
    do {                                                                                                               \
        const long l      = expr1;                                                                                     \
        const long r      = expr2;                                                                                     \
        const bool result = (l > r) - (l < r);                                                                         \
        bool       isok   = false;                                                                                     \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %ld %s %ld \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                    \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_float(expr1, expr2, cond)                                                                         \
    do {                                                                                                               \
        const float l      = expr1;                                                                                    \
        const float r      = expr2;                                                                                    \
        const bool  result = (l > r) - (l < r);                                                                        \
        bool        isok   = false;                                                                                    \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %f %s %f \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_double(expr1, expr2, cond)                                                                        \
    do {                                                                                                               \
        const double l      = expr1;                                                                                   \
        const double r      = expr2;                                                                                   \
        const bool   result = (l > r) - (l < r);                                                                       \
        bool         isok   = false;                                                                                   \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %f %s %f \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_uint8(expr1, expr2, cond)                                                                         \
    do {                                                                                                               \
        const uint8 l      = expr1;                                                                                    \
        const uint8 r      = expr2;                                                                                    \
        const bool  result = (l > r) - (l < r);                                                                        \
        bool        isok   = false;                                                                                    \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %u %s %u \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_int8(expr1, expr2, cond)                                                                          \
    do {                                                                                                               \
        const int8 l      = expr1;                                                                                     \
        const int8 r      = expr2;                                                                                     \
        const bool result = (l > r) - (l < r);                                                                         \
        bool       isok   = false;                                                                                     \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %d %s %d \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_uint16(expr1, expr2, cond)                                                                        \
    do {                                                                                                               \
        const uint16 l      = expr1;                                                                                   \
        const uint16 r      = expr2;                                                                                   \
        const bool   result = (l > r) - (l < r);                                                                       \
        bool         isok   = false;                                                                                   \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %u %s %u \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_int16(expr1, expr2, cond)                                                                         \
    do {                                                                                                               \
        const int16 l      = expr1;                                                                                    \
        const int16 r      = expr2;                                                                                    \
        const bool  result = (l > r) - (l < r);                                                                        \
        bool        isok   = false;                                                                                    \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %d %s %d \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_uint32(expr1, expr2, cond)                                                                        \
    do {                                                                                                               \
        const uint32 l      = expr1;                                                                                   \
        const uint32 r      = expr2;                                                                                   \
        const bool   result = (l > r) - (l < r);                                                                       \
        bool         isok   = false;                                                                                   \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %u %s %u \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_int32(expr1, expr2, cond)                                                                         \
    do {                                                                                                               \
        const int32 l      = expr1;                                                                                    \
        const int32 r      = expr2;                                                                                    \
        const bool  result = (l > r) - (l < r);                                                                        \
        bool        isok   = false;                                                                                    \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %d %s %d \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                      \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_uint64(expr1, expr2, cond)                                                                        \
    do {                                                                                                               \
        const uint64 l      = expr1;                                                                                   \
        const uint64 r      = expr2;                                                                                   \
        const bool   result = (l > r) - (l < r);                                                                       \
        bool         isok   = false;                                                                                   \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %llu %s %llu \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                  \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#define ctest_assert_int64(expr1, expr2, cond)                                                                         \
    do {                                                                                                               \
        const int64 l      = expr1;                                                                                    \
        const int64 r      = expr2;                                                                                    \
        const bool  result = (l > r) - (l < r);                                                                        \
        bool        isok   = false;                                                                                    \
        switch (result) {                                                                                              \
        case -1:                                                                                                       \
            isok = (cond) & CTEST_LESS;                                                                                \
            break;                                                                                                     \
        case 1:                                                                                                        \
            isok = (cond) & CTEST_GREATER;                                                                             \
            break;                                                                                                     \
        case 0:                                                                                                        \
        default:                                                                                                       \
            isok = (cond) & CTEST_EQUAL;                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        if (!isok) {                                                                                                   \
            printf("assert fatal %d: %lld %s %lld \n", __c_line__, l, CTEST_CONPARE_CHAR(result), r);                  \
            return __c_line__;                                                                                         \
        }                                                                                                              \
    } while (0)

#endif