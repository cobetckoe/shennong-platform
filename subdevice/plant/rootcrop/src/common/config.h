/**
 * 神农 - 项目公共配置
 *
 * 全局参数在此统一定义，设备无需重复定义。
 */

#ifndef CONFIG_H
#define CONFIG_H

/* 固件版本 */
#define FW_VERSION_MAJOR    1
#define FW_VERSION_MINOR    0
#define FW_VERSION_PATCH    0

/* 通信配置 */
#define ZIGBEE_BAUD         115200
#define ZIGBEE_PORT         1       /* UART2 */

/* 定时器间隔 (ms) */
#define HB_INTERVAL         30000
#define SENSOR_INTERVAL     5000
#define UART_TIMEOUT        10

/* 传感器采样参数 */
#define DHT22_RETRY         3       /* DHT22重试次数 */
#define ADC_SAMPLE_TIME     239     /* ADC采样周期 */

#endif
