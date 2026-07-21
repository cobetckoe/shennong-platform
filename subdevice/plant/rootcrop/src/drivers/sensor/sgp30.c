/**
 * 传感器驱动 - SGP30 VOC/CO2 (实现)
 */

#include "sgp30.h"
#include "hal/delay.h"

/* SGP30命令 */
#define SGP30_CMD_IAQ_INIT     0x2003
#define SGP30_CMD_IAQ_MEASURE  0x2008

static uint8_t crc8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }
    return crc;
}

void sgp30_init(sgp30_t *dev) {
    /* 发送IAQ init命令: 寄存器0x20, 数据0x03 */
    hal_i2c_write_byte(dev->port, dev->addr, 0x20, 0x03);
    hal_delay_ms(10);
}

uint8_t sgp30_measure(sgp30_t *dev) {
    uint8_t buf[6];
    
    /* 发送测量命令: 寄存器0x20, 数据0x08 */
    hal_i2c_write_byte(dev->port, dev->addr, 0x20, 0x08);
    hal_delay_ms(12);
    
    /* 读取6字节数据: CO2(2) + CRC(1) + TVOC(2) + CRC(1) */
    for (uint8_t i = 0; i < 6; i++) {
        buf[i] = hal_i2c_read_byte(dev->port, dev->addr, i);
    }
    
    /* 验证CRC */
    if (crc8(buf, 2) != buf[2] || crc8(buf + 3, 2) != buf[5])
        return 0;
    
    dev->co2 = (buf[0] << 8) | buf[1];
    dev->tvoc = (buf[3] << 8) | buf[4];
    return 1;
}