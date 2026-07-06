#include "shennong_protocols.h"

// 初始化通信协议
void protocol_init(void) {
    // 初始化Meshtastic模块
    // 配置UART通信
}

// 发送消息
bool protocol_send(mesh_message_t *msg) {
    // 通过Meshtastic发送消息
    return true;
}

// 接收消息
bool protocol_receive(mesh_message_t *msg) {
    // 从Meshtastic接收消息
    return false;
}

// 广播消息
bool protocol_broadcast(mesh_message_t *msg) {
    // 通过Meshtastic广播消息
    return true;
}