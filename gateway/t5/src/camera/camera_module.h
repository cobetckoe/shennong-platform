/**
 * 神农 T5 网关 - 摄像头/图像识别模块
 */

#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool has_disease;
    char disease_name[32];
    float confidence;
    char image_url[128];
} image_result_t;

typedef void (*camera_result_cb_t)(uint8_t dev_id, const image_result_t *result);

void camera_module_init(camera_result_cb_t cb);
void camera_module_loop(void);
void camera_module_capture_now(void);
void camera_module_set_enabled(bool enabled);

#endif
