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
#include "ctest_item.h"

// -------------------------[STATIC DECLARATION]-------------------------

#ifndef PROJECT_VERSION_MAJOR
#define PROJECT_VERSION_MAJOR 1
#endif

#ifndef PROJECT_VERSION_MINOR
#define PROJECT_VERSION_MINOR 0
#endif

#ifndef PROJECT_VERSION_PATCH
#define PROJECT_VERSION_PATCH 0
#endif

#ifndef PROJECT_DEBUG_FLAG
#define PROJECT_DEBUG_FLAG 1
#endif

static const ctest_item_t ctest_item_all[] = {
    C_TEST_FUNC_ITEM(cini),
};

#define ctest_item_count       __c_array_size(ctest_item_all)
#define CTEST_FAIL_STRING      "\033[2;31mFAIL\033[0m"
#define CTEST_PASS_STRING      "\033[2;32mPASS\033[0m"
#define CTEST_STATUS_STRING(x) ((x) ? CTEST_FAIL_STRING : CTEST_PASS_STRING)

// 执行单个测试项
static inline int ctest_item_exec(const ctest_item_t *item, int argc, char **argv);
// 执行所有测试项
static inline int ctest_item_exec_all(int argc, char **argv);
// 打印命令说明
static inline void print_command_instructions(void);
// 打印项目版本
static inline void print_project_version(void);
// 打印测试项列表
static inline void print_item_list(void);

// -------------------------[GLOBAL DEFINITION]-------------------------

int main(int argc, char **argv)
{
    if (argc == 1) {
        return ctest_item_exec_all(argc - 1, argv + 1);
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_command_instructions();
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        print_project_version();
        return 0;
    }

    if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "-l") == 0) {
        print_item_list();
        return 0;
    }

    if (strcmp(argv[1], "--target") == 0 || strcmp(argv[1], "-t") == 0) {
        if (argc < 3) {
            printf("Invalid number of arguments for '--target' command. Use 'help' command for instructions.\n");
            return 1;
        }

        int    err = 0;
        size_t i   = 0;
        for (i = 0; i < ctest_item_count; ++i) {
            if (!ctest_item_all[i].name || strcmp(ctest_item_all[i].name, argv[2])) {
                continue;
            }
            err = ctest_item_exec(&ctest_item_all[i], argc - 3, argv + 3);
            break;
        }
        if (i == ctest_item_count) {
            printf("Invalid test item '%s'. Use '--list' command for instructions.\n", argv[2]);
            return 1;
        }
        return err;
    }

    printf("Invalid command. Use 'help' command for instructions.\n");
    return 1;
}

// -------------------------[STATIC DEFINITION]-------------------------

static inline int ctest_item_exec(const ctest_item_t *item, int argc, char **argv)
{
    const int result = item->func(argc, argv);
    printf("[%s] %s \n", CTEST_STATUS_STRING(result), item->name);
    return result;
}

static inline int ctest_item_exec_all(int argc, char **argv)
{
    int    err = 0;
    size_t i   = 0;
    for (i = 0; i < ctest_item_count; ++i) {
        err = ctest_item_exec(&ctest_item_all[i], argc - 3, argv + 3);
        if (err) {
            break;
        }
    }
    return err;
}

static inline void print_command_instructions(void)
{
    printf("cini test suite - Test suite for the cini config library\n");
    printf("Usage: test_cini [command] [options]\n");
    printf("\nCommands:\n");
    printf("  no command: Run all test cases\n");
    printf("  -h, --help: Print this help message\n");
    printf("  -v, --version: Print version information\n");
    printf("  -l, --list: List all test cases\n");
    printf("  -t, --target [test] [options]: Run specific test case\n");
    printf("\nOptions:\n");
    printf("  [test]: Name of test case to run\n");
    printf("  [options]: Optional arguments passed to test case\n");
}

static inline void print_project_version(void)
{
    printf("cini test suite version %d.%d.%d-%s\n", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH,
           PROJECT_DEBUG_FLAG ? "debug" : "release");
    printf("Copyright (C) 2023 Tayne\n");
    printf("Licensed under GNU LGPL v3\n");
}

static inline void print_item_list(void)
{
    printf("all test items:\n");

    size_t i = 0;
    for (i = 0; i < ctest_item_count; ++i) {
        printf("- %s\n", ctest_item_all[i].name);
    }
}