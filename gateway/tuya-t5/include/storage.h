#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdbool.h>

// 存储初始化
bool storage_init(void);

// 保存数据到文件
bool storage_save(const char *filename, const void *data, uint32_t size);

// 从文件读取数据
bool storage_load(const char *filename, void *data, uint32_t size);

// 删除文件
bool storage_delete(const char *filename);

// 检查文件是否存在
bool storage_exists(const char *filename);

// 获取文件大小
uint32_t storage_size(const char *filename);

// 列出目录中的文件
int storage_list(const char *dir, char *files[], int max_count);

#endif // STORAGE_H