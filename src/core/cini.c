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
#include <stdio.h>
#include <string.h>
#include "cini.h"

// -------------------------[STATIC DECLARATION]-------------------------

#define CINI_LINE_MAX 1024

/**
 * @brief 设置参数
 * @param self cini对象
 * @param group 组名
 * @param length 组名的长度
 * @param start 起始索引
 * @param end 结束索引
 */
static inline void cini_param_set(cini_t *self, const char *group, const size_t start, const size_t end);

/**
 * @brief 创建文件
 * @param self cini对象
 */
static inline bool cini_file_create(cini_t *self);

/**
 * @brief 组是否存在
 * @param self cini对象
 * @return true=组存在; false=组不存在
 */
static inline bool cini_group_isexist(cini_t *self);

/**
 * @brief 查找组
 * @param self cini对象
 * @param group 组名
 * @param start 起始索引
 * @return 若找到组则返回 true，否则返回 false
 */
static inline bool cini_group_find(cini_t *self, const char *group, const size_t start);

/**
 * @brief 获取指定的键的所在行号
 * @param self cini对象
 * @param key 键名
 * @param length 键名的长度
 * @return 若找到键则返回键的索引，否则返回 0
 */
static inline size_t cini_pair_line(cini_t *self, const char *key, const size_t length);

/**
 * @brief 移除指定的键
 * @param self cini对象
 * @param key 键名
 * @param length 键名的长度
 */
static inline void cini_pair_remove(cini_t *self, const char *key, const size_t length);

/**
 * @brief 获取指定键的值
 * @param self cini对象
 * @param key 键名
 * @param length 键名的长度
 * @param buffer 用于存储值的缓冲区
 * @param max 缓冲区的最大大小
 * @return 若找到键并将值复制到缓冲区中则返回 true，否则返回 false
 */
static inline bool cini_pair_value(cini_t *self, const char *key, const size_t length, char *buffer, size_t max);

/**
 * @brief 修改指定键的值
 * @param self cini对象
 * @param key 键名
 * @param length 键名的长度
 * @param value 新的值
 */
static inline void cini_pair_modify(cini_t *self, const char *key, const size_t length, char *value);

// -------------------------[GLOBAL DEFINITION]-------------------------

const char *cini_path_get(cini_t *self)
{
    return self->path;
}

void cini_path_set(cini_t *self, const char *path)
{
    self->path = path;
    cini_param_set(self, STR_NULL, 0, 0);
}

void cini_group_begin(cini_t *self, const char *group)
{
    if (!group) {
        cini_param_set(self, STR_NULL, 0, 0);
        return;
    }
    if (!cini_group_find(self, group, 0)) {
        cini_param_set(self, group, 0, 0);
    }
}

void cini_group_end(cini_t *self)
{
    cini_param_set(self, STR_NULL, 0, 0);
}

const char *cini_group_get(cini_t *self)
{
    return self->group_name;
}

void cini_value_get(cini_t *self, const char *key, const char *default_value, char *buffer, size_t max)
{
    if (!key || !buffer || !max) {
        return;
    }
    if (!cini_group_isexist(self)) {
        snprintf(buffer, max, "%s", default_value);
        return;
    }
    if (!cini_pair_value(self, key, strlen(key), buffer, max)) {
        snprintf(buffer, max, "%s", default_value);
        return;
    }
}

void cini_value_set(cini_t *self, const char *key, char *value)
{
    if (!key || !value) {
        return;
    }
    cini_pair_modify(self, key, strlen(key), value);
}

void cini_value_remove(cini_t *self, const char *key)
{
    if (!key || !cini_group_isexist(self)) {
        return;
    }
    cini_pair_remove(self, key, strlen(key));
}

bool cini_value_contains(cini_t *self, const char *key)
{
    if (!key || !cini_group_isexist(self)) {
        return false;
    }
    return cini_pair_line(self, key, strlen(key)) > 0;
}

// -------------------------[STATIC DEFINITION]-------------------------

static inline void cini_param_set(cini_t *self, const char *group, const size_t start, const size_t end)
{
    self->group_name  = group;
    self->group_start = start;
    self->group_end   = end;
}

static inline bool cini_file_create(cini_t *self)
{
    FILE *wfd = fopen(self->path, "w");
    if (wfd) {
        fclose(wfd);
        return true;
    }
    return false;
}

static inline bool cini_group_isexist(cini_t *self)
{
    return self->group_end > 0;
}

static inline bool cini_group_find(cini_t *self, const char *group, const size_t start)
{
    // 打开文件
    FILE *rfd = fopen(self->path, "r");
    if (!rfd) {
        return false;
    }

    char line_buffer[CINI_LINE_MAX] = {0};

    const size_t group_length = strlen(group);
    size_t       line_current = 0;
    size_t       line_length  = 0;
    size_t       group_start  = 0;
    size_t       group_end    = 0;

    // 将文件指针移动到文件开始
    fseek(rfd, 0, SEEK_SET);

    while (fgets(line_buffer, CINI_LINE_MAX, rfd)) {
        // 如果当前行号小于起始行号，跳过此行
        if (++line_current < start) {
            continue;
        }

        if (group_start == 0) {
            if (line_buffer[0] != '[') {
                continue;
            }

            line_length = strlen(line_buffer);

            if (line_length > 0 && line_buffer[line_length - 1] == '\n') {
                --line_length;
                if (line_length > 0 && line_buffer[line_length - 1] == '\r') {
                    --line_length;
                }
            }

            if (line_buffer[line_length - 1] != ']') {
                continue;
            } else {
                line_buffer[line_length - 1] = '\0';
                --line_length;
            }

            if (line_length - 1 != group_length) {
                continue;
            }
            if (strncmp(line_buffer + 1, group, line_length - 1) != 0) {
                continue;
            }
            group_end = group_start = line_current;
            continue;
        }

        if (line_current > group_start) {
            if (line_buffer[0] == '[') {
                break;
            }

            line_length = strlen(line_buffer);

            if (line_length > 0 && line_buffer[line_length - 1] == '\n') {
                --line_length;
                if (line_length > 0 && line_buffer[line_length - 1] == '\r') {
                    --line_length;
                }
            }

            if (line_length == 0) {
                continue;
            }

            group_end = line_current;
        }
    }

    // 关闭文件
    fclose(rfd);

    if (group_start == 0) {
        return false;
    } else {
        cini_param_set(self, group, group_start, group_end);
        return true;
    }
}

static inline size_t cini_pair_line(cini_t *self, const char *key, const size_t length)
{
    // 打开文件
    FILE *rfd = fopen(self->path, "r");
    if (!rfd) {
        return 0;
    }

    char line_buffer[CINI_LINE_MAX] = {0};

    size_t line_current = 0;
    size_t line_length  = 0;
    size_t key_line     = 0;
    size_t value_start  = 0;
    bool   isok         = true;

    // 将文件指针移动到文件开始
    fseek(rfd, 0, SEEK_SET);

    while (fgets(line_buffer, CINI_LINE_MAX, rfd)) {
        // 如果当前行号小于起始行号，跳过此行
        if (++line_current < self->group_start) {
            continue;
        }
        if (line_current > self->group_end) {
            break;
        }

        if (line_buffer[0] >= '0' && line_buffer[0] <= '9') {
        } else if (line_buffer[0] >= 'a' && line_buffer[0] <= 'z') {
        } else if (line_buffer[0] >= 'A' && line_buffer[0] <= 'Z') {
        } else {
            continue;
        }

        line_length = strlen(line_buffer);

        if (line_length <= length) {
            continue;
        }

        if (line_length > 0 && line_buffer[line_length - 1] == '\n') {
            --line_length;
            if (line_length > 0 && line_buffer[line_length - 1] == '\r') {
                --line_length;
            }
        }

        if (strncmp(line_buffer, key, length) != 0) {
            continue;
        }

        value_start = length;
        do {
            if (value_start >= line_length) {
                isok = false;
                break;
            } else if (line_buffer[length] == ' ') {
                ++value_start;
                continue;
            } else if (line_buffer[length] == '=') {
                key_line = line_current;
                break;
            } else {
                isok = false;
                break;
            }
        } while (0);

        if (isok) {
            break;
        } else {
            isok = true;
            continue;
        }
    }
    // 关闭文件
    fclose(rfd);
    return key_line;
}

static inline void cini_pair_remove(cini_t *self, const char *key, const size_t length)
{
    // 打开文件
    FILE *rfd = fopen(self->path, "r");
    if (!rfd) {
        return;
    }

    char wpath[128] = {0};
    snprintf(wpath, 128, "%s.tmp", self->path);

    // 打开文件
    FILE *wfd = fopen(wpath, "wb");
    if (!wfd) {
        fclose(rfd);
        return;
    }

    char line_buffer[CINI_LINE_MAX] = {0};

    size_t line_current = 0;
    size_t line_length  = 0;
    size_t value_start  = 0;

    // 将文件指针移动到文件开始
    fseek(rfd, 0, SEEK_SET);

    bool isput = false;

    while (fgets(line_buffer, CINI_LINE_MAX, rfd)) {
        do {
            // 如果当前行号小于起始行号，跳过此行
            if (++line_current < self->group_start) {
                isput = true;
                break;
            }
            if (line_current > self->group_end) {
                isput = true;
                break;
            }

            if (line_buffer[0] >= '0' && line_buffer[0] <= '9') {
            } else if (line_buffer[0] >= 'a' && line_buffer[0] <= 'z') {
            } else if (line_buffer[0] >= 'A' && line_buffer[0] <= 'Z') {
            } else {
                isput = true;
                break;
            }

            line_length = strlen(line_buffer);

            if (line_length <= length) {
                isput = true;
                break;
            }

            if (line_length > 0 && line_buffer[line_length - 1] == '\n') {
                --line_length;
                if (line_length > 0 && line_buffer[line_length - 1] == '\r') {
                    --line_length;
                }
            }

            if (strncmp(line_buffer, key, length) != 0) {
                isput = true;
                break;
            }

            value_start = length;
            do {
                if (value_start >= line_length) {
                    isput = true;
                    break;
                } else if (line_buffer[length] == ' ') {
                    ++value_start;
                    continue;
                } else if (line_buffer[length] == '=') {
                    --self->group_end;
                    isput = false;
                    break;
                } else {
                    isput = true;
                    break;
                }
            } while (0);
        } while (0);
        if (isput) {
            fputs(line_buffer, wfd);
        }
    }
    // 关闭文件
    fclose(wfd);
    fclose(rfd);
    remove(self->path);
    rename(wpath, self->path);
    return;
}

static inline bool cini_pair_value(cini_t *self, const char *key, const size_t length, char *buffer, const size_t max)
{
    // 打开文件
    FILE *rfd = fopen(self->path, "r");
    if (!rfd) {
        buffer[0] = '\0';
        return false;
    }

    char line_buffer[CINI_LINE_MAX] = {0};

    size_t line_current = 0;
    size_t line_length  = 0;
    size_t value_start  = 0;
    bool   isok         = true;

    // 将文件指针移动到文件开始
    fseek(rfd, 0, SEEK_SET);

    while (fgets(line_buffer, CINI_LINE_MAX, rfd)) {
        ++line_current;

        // 如果当前行号小于起始行号，跳过此行
        if (line_current < self->group_start) {
            continue;
        }
        if (line_current > self->group_end) {
            break;
        }

        if (line_buffer[0] >= '0' && line_buffer[0] <= '9') {
        } else if (line_buffer[0] >= 'a' && line_buffer[0] <= 'z') {
        } else if (line_buffer[0] >= 'A' && line_buffer[0] <= 'Z') {
        } else {
            continue;
        }

        line_length = strlen(line_buffer);

        if (line_length <= length) {
            continue;
        }

        if (line_length > 0 && line_buffer[line_length - 1] == '\n') {
            line_buffer[line_length - 1] = '\0';
            --line_length;
            if (line_length > 0 && line_buffer[line_length - 1] == '\r') {
                line_buffer[line_length - 1] = '\0';
                --line_length;
            }
        }

        if (strncmp(line_buffer, key, length) != 0) {
            continue;
        }

        value_start = length;
        do {
            if (value_start >= line_length) {
                isok = false;
                break;
            } else if (line_buffer[length] == ' ') {
                ++value_start;
                continue;
            } else if (line_buffer[value_start] == '=') {
                ++value_start;
                break;
            } else {
                isok = false;
                break;
            }
        } while (0);

        if (!isok) {
            isok = true;
            continue;
        }

        do {
            if (value_start >= line_length) {
                break;
            } else if (line_buffer[value_start] == ' ') {
                ++value_start;
                continue;
            }
        } while (0);

        snprintf(buffer, max, "%s", line_buffer + value_start);
        break;
    }
    // 关闭文件
    fclose(rfd);

    if (!isok) {
        buffer[0] = '\0';
    }
    return isok;
}

static inline void cini_pair_modify(cini_t *self, const char *key, const size_t length, char *value)
{
    bool isread = true;

    // 打开文件
    FILE *rfd = fopen(self->path, "r");
    if (!rfd) {
        cini_file_create(self);
        rfd = fopen(self->path, "r");
        if (!rfd) {
            return;
        }
        isread = false;
    }

    char wpath[128] = {0};
    snprintf(wpath, 128, "%s.tmp", self->path);

    // 打开文件
    FILE *wfd = fopen(wpath, "wb");
    if (!wfd) {
        fclose(rfd);
        return;
    }

    char line_buffer[CINI_LINE_MAX] = {0};

    size_t line_current = 0;
    size_t line_length  = 0;
    size_t value_start  = 0;

    // 将文件指针移动到文件开始
    fseek(rfd, 0, SEEK_SET);

    bool ismodify = false;

    while (isread && fgets(line_buffer, CINI_LINE_MAX, rfd)) {
        do {
            if (line_current < self->group_start) {
                break;
            }
            if (line_current > self->group_end) {
                break;
            }

            if (line_buffer[0] >= '0' && line_buffer[0] <= '9') {
            } else if (line_buffer[0] >= 'a' && line_buffer[0] <= 'z') {
            } else if (line_buffer[0] >= 'A' && line_buffer[0] <= 'Z') {
            } else {
                break;
            }

            line_length = strlen(line_buffer);

            if (line_length <= length) {
                break;
            }

            if (line_length > 0 && line_buffer[line_length - 1] == '\n') {
                --line_length;
                if (line_length > 0 && line_buffer[line_length - 1] == '\r') {
                    --line_length;
                }
            }

            if (strncmp(line_buffer, key, length) != 0) {
                break;
            }

            value_start = length;
            do {
                if (value_start >= line_length) {
                    snprintf(line_buffer, CINI_LINE_MAX, "%s=%s" STR_NEWLINE, key, value);
                    ismodify = true;
                    break;
                } else if (line_buffer[length] == ' ') {
                    ++value_start;
                    continue;
                } else if (line_buffer[length] == '=') {
                    snprintf(line_buffer, CINI_LINE_MAX, "%s=%s" STR_NEWLINE, key, value);
                    ismodify = true;
                    break;
                }
            } while (0);
        } while (0);

        ++line_current;
        fputs(line_buffer, wfd);

        if (self->group_end != 0 && line_current >= self->group_end && !ismodify) {
            ismodify = true;
            snprintf(line_buffer, CINI_LINE_MAX, "%s=%s" STR_NEWLINE, key, value);
            ++line_current;
            self->group_end = line_current;
            fputs(line_buffer, wfd);
        }
    }

    if (self->group_end == 0) {
        ++line_current;
        snprintf(line_buffer, CINI_LINE_MAX, STR_NEWLINE);
        fputs(line_buffer, wfd);

        ++line_current;
        self->group_start = line_current;
        snprintf(line_buffer, CINI_LINE_MAX, "[%s]" STR_NEWLINE, self->group_name);
        fputs(line_buffer, wfd);

        ++line_current;
        self->group_end = line_current;
        snprintf(line_buffer, CINI_LINE_MAX, "%s=%s" STR_NEWLINE, key, value);
        fputs(line_buffer, wfd);

        ismodify = true;
    }

    if (!ismodify) {
        ++line_current;
        self->group_end = line_current;
        snprintf(line_buffer, CINI_LINE_MAX, "%s=%s" STR_NEWLINE, key, value);
        fputs(line_buffer, wfd);
    }

    // 关闭文件
    fclose(rfd);
    fclose(wfd);
    remove(self->path);
    rename(wpath, self->path);
    return;
}
