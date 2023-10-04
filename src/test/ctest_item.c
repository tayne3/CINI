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
#include "core/cini.h"

// -------------------------[STATIC DECLARATION]-------------------------

#define CINI_TEST_FILE "test.ini"

// -------------------------[GLOBAL DEFINITION]-------------------------

int ctest_func_cini(int argc, char **argv)
{
    cini_t cini = CINI_INITIALIZATION;

    // 设置路径
    {
        const char *path = CINI_TEST_FILE;
        cini_path_set(&cini, path);
        ctest_assert_string(cini_path_get(&cini), path);
    }

    char group[256]  = {0};
    char key[256]    = {0};
    char value[256]  = {0};
    char result[256] = {0};

    int group_index = 1;
    int value_index = 1;
    int key_index   = 1;

    for (group_index = 1; group_index <= 10; ++group_index) {
        snprintf(group, sizeof(group), "group_%d", group_index);
        cini_group_begin(&cini, group);
        ctest_assert_string(cini_group_get(&cini), group);

        for (key_index = 1; key_index <= 10; ++key_index) {
            snprintf(key, sizeof(key), "key_%d", key_index);

            for (value_index = 1; value_index <= 10; ++value_index) {
                snprintf(value, sizeof(value), "value_%d", value_index);

                cini_value_set(&cini, key, value);
                cini_value_get(&cini, key, "default", result, sizeof(result));
                ctest_assert_string(value, result);
                ctest_assert_bool(cini_value_contains(&cini, key));

                cini_value_remove(&cini, key);
                ctest_assert_bool(!cini_value_contains(&cini, key));
            }
        }
        cini_group_end(&cini);
        ctest_assert_string(cini_group_get(&cini), STR_NULL);
    }
    remove(CINI_TEST_FILE);
    return 0;
    __c_unused(argc);
    __c_unused(argv);
}

// -------------------------[STATIC DEFINITION]-------------------------