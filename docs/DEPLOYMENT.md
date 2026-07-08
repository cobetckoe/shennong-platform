# Deployment

## Gateway

### Requirements
- WBR3 development board
- Tuya Wind IDE
- TuyaOS SDK
- Tuya developer account

### Setup
1. Create product on Tuya platform
2. Open project with Tuya Wind IDE
3. Configure product key/secret
4. Build and flash
5. Connect ZS3L to UART

### Pin Connection
- ZS3L TX → WBR3 GPIO4
- ZS3L RX → WBR3 GPIO5
- ZS3L VCC → 3.3V
- ZS3L GND → GND

## Sub Device

### Requirements
- LK Shield (STM32F103)
- PlatformIO
- ZS3L module

### Setup
1. Copy template to device directory
2. Modify DEVICE_ID and DEVICE_TYPE
3. Implement sensor reading
4. Build and flash

### Pin Connection
- ZS3L TX → PA3 (USART2_RX)
- ZS3L RX → PA2 (USART2_TX)
- ZS3L VCC → 3.3V
- ZS3L GND → GND
- Spray pump → PB0
- Fan → PB1
- LED → PB2

## Device ID Allocation

| Device Type | ID Range |
|-------------|----------|
| tuber | 0x02-0x02 |
| root | 0x03-0x03 |
| stem | 0x04-0x04 |
| leaf | 0x05-0x05 |
| flower | 0x06-0x06 |
| fruit | 0x07-0x07 |
| spiral | 0x10-0x10 |
| chlorella | 0x11-0x11 |
| mushroom | 0x20-0x20 |
| fungus | 0x21-0x21 |
| bee | 0x30-0x30 |
| earthworm | 0x31-0x31 |
| cricket | 0x40-0x40 |
| mealworm | 0x41-0x41 |

## Testing

### Gateway
- Check WiFi connection
- Check Tuya Cloud connection
- Check Zigbee communication

### Sub Device
- Check heartbeat sending
- Check sensor data collection
- Check control execution

### End-to-End
- Sub device reports to cloud
- Cloud sends rules to gateway
- Gateway forwards to sub device
- Sub device executes control
