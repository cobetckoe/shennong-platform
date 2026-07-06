#include "storage.h"
#include <stdio.h>
#include <string.h>

#define STORAGE_ROOT "/data"

// 存储初始化
bool storage_init(void) {
    // 创建根目录
    mkdir(STORAGE_ROOT, 0755);
    return true;
}

// 保存数据到文件
bool storage_save(const char *filename, const void *data, uint32_t size) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_ROOT, filename);
    
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        return false;
    }
    
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    
    return written == size;
}

// 从文件读取数据
bool storage_load(const char *filename, void *data, uint32_t size) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_ROOT, filename);
    
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return false;
    }
    
    size_t read = fread(data, 1, size, fp);
    fclose(fp);
    
    return read == size;
}

// 删除文件
bool storage_delete(const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_ROOT, filename);
    
    return remove(path) == 0;
}

// 检查文件是否存在
bool storage_exists(const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_ROOT, filename);
    
    FILE *fp = fopen(path, "r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

// 获取文件大小
uint32_t storage_size(const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_ROOT, filename);
    
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return 0;
    }
    
    fseek(fp, 0, SEEK_END);
    uint32_t size = ftell(fp);
    fclose(fp);
    
    return size;
}

// 列出目录中的文件
int storage_list(const char *dir, char *files[], int max_count) {
    // 简化实现，实际需要遍历目录
    return 0;
}