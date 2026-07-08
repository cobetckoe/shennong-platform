# Tuber Planter

## Overview

Sub device for tuber crops (potato, sweet potato).

## Hardware

- LK Shield (STM32F103C8)
- ZS3L module (Zigbee)
- Sensors: Temperature, Humidity, Light, pH
- Actuators: Spray pump, Fan, LED

## Development Environment

### Requirements
- PlatformIO
- ST-Link debugger
- Arduino framework

### Setup
1. Install PlatformIO
2. Open project folder
3. Build and flash

## Configuration

### Device ID
Modify in `src/main.cpp`:
```c
#define DEVICE_ID       0x02
#define DEVICE_TYPE     0x02
```

### Pin Connection
- ZS3L TX → PA3 (USART2_RX)
- ZS3L RX → PA2 (USART2_TX)
- ZS3L VCC → 3.3V
- ZS3L GND → GND
- Spray pump → PB0
- Fan → PB1
- LED → PB2

## Build

```bash
pio run
```

## Flash

```bash
pio run -t upload
```

## Debug

```bash
pio device monitor --baud 115200
```

## Control Parameters

| Parameter | Default | Range |
|-----------|---------|-------|
| Temperature high | 28°C | 20-40°C |
| Temperature low | 15°C | 5-25°C |
| Humidity high | 75% | 50-100% |
| Humidity low | 50% | 30-80% |

## References

- [PlatformIO](https://platformio.org/)
- [STM32 Arduino](https://docs.platformio.org/en/latest/boards/ststm32/genericSTM32F103C8.html)
