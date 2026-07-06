#ifndef SHENNONG_PROTOCOLS_H
#define SHENNONG_PROTOCOLS_H

#include <Arduino.h>

// 消息类型定义
typedef enum {
    MSG_TYPE_SENSOR_DATA,
    MSG_TYPE_CONTROL_CMD,
    MSG_TYPE_STATUS_REPORT,
    MSG_TYPE_HEARTBEAT
} message_type_t;

// 消息结构
typedef struct {
    uint8_t source_id;
    uint8_t dest_id;
    message_type_t type;
    uint8_t data[32];
    uint8_t length;
} mesh_message_t;

// 初始化通信协议
void protocol_init(void);

// 发送消息
bool protocol_send(mesh_message_t *msg);

// 接收消息
bool protocol_receive(mesh_message_t *msg);

// 广播消息
bool protocol_broadcast(mesh_message_t *msg);

#endif // SHENNONG_PROTOCOLS_H