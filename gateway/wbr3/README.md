# WBR3 Gateway

## Overview

Gateway device connecting Tuya Cloud via WiFi and sub devices via Zigbee.

## Hardware

- WBR3 module (ESP32-C3)
- ZS3L module (Zigbee transparent)

## Development Environment

### Requirements
- Tuya Wind IDE
- TuyaOS SDK (Gateway framework)
- Tuya developer account

### Setup
1. Install Tuya Wind IDE
2. Login with Tuya account
3. Download gateway development kit
4. Create project in Tuya Wind IDE

## Project Structure

```
wbr3/
├── src/
│   └── main.c          # Application code
├── include/            # Header files
└── README.md
```

## Configuration

### Product Information
Modify in `src/main.c`:
```c
#define PRODUCT_KEY     "your_product_key"
#define PRODUCT_SECRET  "your_product_secret"
#define DEVICE_NAME     "your_device_name"
#define DEVICE_SECRET   "your_device_secret"
```

### Pin Connection
- ZS3L TX → WBR3 GPIO4
- ZS3L RX → WBR3 GPIO5
- ZS3L VCC → 3.3V
- ZS3L GND → GND

## Build

1. Open project in Tuya Wind IDE
2. Right-click project → Build Project
3. Enter version number (X.X.X)
4. Wait for build to complete

## Flash

1. Connect WBR3 via USB
2. Select correct COM port
3. Flash firmware

## Debug

### Check WiFi
- LED indicator shows WiFi status
- Check Tuya Cloud connection

### Check Zigbee
- Monitor UART output
- Check ZS3L communication

## References

- [TuyaOS Gateway Guide](https://developer.tuya.com/cn/docs/iot-device-dev/Gateway-Product-Software-Development-Kit?id=Kbfjuqtma64w4)
- [Tuya Wind IDE](https://developer.tuya.com/cn/docs/iot-device-dev/tuyaos-wind-ide?id=Kbfy6kfuuqqu3)
