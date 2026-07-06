#include <stdio.h>
#include "storage.h"
#include "model_manager.h"
#include "data_manager.h"
#include "bio_config.h"

// 系统初始化
bool system_init(void) {
    printf("Initializing Shennong Gateway...\n");
    
    // 初始化存储模块
    if (!storage_init()) {
        printf("Failed to initialize storage\n");
        return false;
    }
    
    // 初始化模型管理模块
    if (!model_manager_init()) {
        printf("Failed to initialize model manager\n");
        return false;
    }
    
    // 初始化数据管理模块
    if (!data_manager_init()) {
        printf("Failed to initialize data manager\n");
        return false;
    }
    
    // 初始化生物配置模块
    if (!bio_config_init()) {
        printf("Failed to initialize bio config\n");
        return false;
    }
    
    printf("Shennong Gateway initialized successfully\n");
    return true;
}

// 主函数
int main(void) {
    if (!system_init()) {
        printf("System initialization failed\n");
        return -1;
    }
    
    printf("Shennong Gateway running...\n");
    
    // 主循环
    while (1) {
        // TODO: 实现主循环逻辑
        // 1. 接收养殖设备数据
        // 2. 执行控制逻辑
        // 3. 保存数据
        // 4. 更新模型
        
        // 简单延时
        // delay(1000);
    }
    
    return 0;
}