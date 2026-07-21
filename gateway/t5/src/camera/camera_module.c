/**
 * 神农系统 T5 网关 - 摄像头/图像识别模块
 * 支持 T5-AI 摄像头，本地或云端图像识别
 */

#include "camera_module.h"
#include "tuya_camera.h"
#include "tuya_ai.h"
#include <string.h>

#define CAPTURE_INTERVAL_MS     300000   /* 5分钟拍一次 */
#define DISEASE_CONF_THRESHOLD  0.7      /* 病虫害置信度阈值 */

static camera_result_cb_t result_callback = NULL;
static uint32_t last_capture_time = 0;
static bool camera_enabled = false;

void camera_module_init(camera_result_cb_t cb) {
    result_callback = cb;
    last_capture_time = 0;

    /* 初始化摄像头 */
    tuya_camera_config_t cam_cfg = {
        .resolution = TUYA_CAMERA_RES_320x240,
        .fps = 1,
        .format = TUYA_CAMERA_FORMAT_JPEG,
    };
    if (tuya_camera_init(&cam_cfg) == 0) {
        camera_enabled = true;
    }
}

void camera_module_loop(void) {
    if (!camera_enabled) return;

    uint32_t now = tuya_system_get_millisecond();
    if (now - last_capture_time < CAPTURE_INTERVAL_MS) return;
    last_capture_time = now;

    /* 拍照 */
    tuya_camera_frame_t frame;
    if (tuya_camera_capture(&frame) != 0) return;

    /* 图像识别 (云端 AI) */
    tuya_ai_request_t req = {
        .type = TUYA_AI_TYPE_IMAGE_CLASSIFY,
        .image_data = frame.data,
        .image_len = frame.len,
        .model = "plant_disease_detect",
    };

    tuya_ai_response_t resp;
    if (tuya_ai_infer(&req, &resp) == 0) {
        image_result_t result;
        memset(&result, 0, sizeof(result));

        /* 解析识别结果 */
        if (resp.confidence > DISEASE_CONF_THRESHOLD) {
            result.has_disease = true;
            strncpy(result.disease_name, resp.label, sizeof(result.disease_name) - 1);
            result.confidence = resp.confidence;
        }

        /* 回调通知 */
        if (result_callback) {
            result_callback(0xFF, &result);  /* 0xFF = 网关自身摄像头 */
        }
    }

    tuya_camera_release(&frame);
}

void camera_module_capture_now(void) {
    last_capture_time = 0;  /* 立即触发拍照 */
}

void camera_module_set_enabled(bool enabled) {
    camera_enabled = enabled;
}
