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
#include "core/cini.h"
#include <string.h>
#include <stdio.h>

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

// 打印命令说明
static inline void print_command_instructions(void);
// 打印项目版本
static inline void print_project_version(void);

// -------------------------[GLOBAL DEFINITION]-------------------------

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Invalid number of arguments. Use 'help' command for instructions.\n");
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_command_instructions();
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        print_project_version();
        return 0;
    }

    if (strcmp(argv[1], "get") == 0) {
        if (argc < 6) {
            printf("Invalid number of arguments for 'get' command. Use 'help' command for instructions.\n");
            return 1;
        }
        cini_t cini = CINI_INITIALIZATION;
        cini_path_set(&cini, argv[2]);
        cini_group_begin(&cini, argv[3]);
        char buffer[256] = {0};
        cini_value_get(&cini, argv[4], argv[5], buffer, sizeof(buffer));
        printf("%s\n", buffer);
        return 0;
    }

    if (strcmp(argv[1], "set") == 0) {
        if (argc < 6) {
            printf("Invalid number of arguments for 'set' command. Use 'help' command for instructions.\n");
            return 1;
        }
        cini_t cini = CINI_INITIALIZATION;
        cini_path_set(&cini, argv[2]);
        cini_group_begin(&cini, argv[3]);
        cini_value_set(&cini, argv[4], argv[5]);
        return 0;
    }

    if (strcmp(argv[1], "rm") == 0) {
        if (argc < 5) {
            printf("Invalid number of arguments for 'rm' command. Use 'help' command for instructions.\n");
            return 1;
        }
        cini_t cini = CINI_INITIALIZATION;
        cini_path_set(&cini, argv[2]);
        cini_group_begin(&cini, argv[3]);
        cini_value_remove(&cini, argv[4]);
        return 0;
    }

    printf("Invalid command. Use 'help' command for instructions.\n");
    return 1;
}

// -------------------------[STATIC DEFINITION]-------------------------

static inline void print_command_instructions(void)
{
    printf("Usage: cini [command] [arguments]\n");
    printf("\nCommands:\n");
    printf("  -h, --help: Print this help message\n");
    printf("  -v, --version: Print version information\n");
    printf("  get [path] [group] [key] [default_value]: Get the value of key 'key' in group 'group' of ini file "
           "'path'. \n");
    printf("\t\t\t\t\t    If key does not exist, print 'default_value'.\n");
    printf("  set [path] [group] [key] [value]: Set the value of key 'key' in group 'group' of ini file 'path' to "
           "'value'.\n");
    printf("  rm [path] [group] [key]: Remove the key 'key' in group 'group' of ini file 'path'.\n");
}

static inline void print_project_version(void)
{
    printf("cini, version %d.%d.%d-%s\n", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH,
           PROJECT_DEBUG_FLAG ? "debug" : "release");
    printf("Copyright (C) 2023 Tayne\n");
    printf("Licensed under GNU LGPL v3\n");
}
