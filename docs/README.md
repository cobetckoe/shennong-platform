# Shennong Platform Documentation

## Overview

Distributed AI agriculture production system using Tuya IoT platform.

## Architecture

### System Topology
```
Tuya Cloud ←WiFi→ Gateway ←Zigbee→ Sub Device
                                ←Zigbee→ Sub Device
                                ←Zigbee→ Sub Device
```

### Device Types
| Type | Directory | Connection | Function |
|------|-----------|------------|----------|
| Gateway | gateway/ | WiFi + Zigbee | Bridge to cloud |
| Auxiliary | auxiliary/ | Standalone | Independent operation |
| Sub Device | subdevice/ | Zigbee | Local control |

### Hardware
| Device | Components | SDK | Cost |
|--------|-----------|-----|------|
| Gateway | WBR3 + ZS3L | TuyaOS | ¥25 |
| Sub Device | LK Shield + ZS3L | PlatformIO | ¥20 |

## Development

### Gateway (TuyaOS)
1. Install Tuya Wind IDE
2. Download gateway development kit
3. Configure product information
4. Build and flash

### Sub Device (PlatformIO)
1. Install PlatformIO
2. Configure device ID
3. Implement sensor reading
4. Build and flash

## Communication Protocol

### Command Types
| Command | Direction | Description |
|---------|-----------|-------------|
| 0x01 | Sub → Gateway | Heartbeat |
| 0x02 | Bidirectional | Sensor data / Control rule |

### Data Structures
```c
// Control Rule
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;

// Sensor Data
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;
```

## Device List

### Plant (edible parts)
| Device | ID | Part | Crops |
|--------|-----|------|-------|
| tuber | 0x02 | Tuber | Potato |
| root | 0x03 | Root | Radish |
| stem | 0x04 | Stem | Celery |
| leaf | 0x05 | Leaf | Spinach |
| flower | 0x06 | Flower | Broccoli |
| fruit | 0x07 | Fruit | Tomato |

### Algae
| Device | ID | Name |
|--------|-----|------|
| spiral | 0x10 | Spirulina |
| chlorella | 0x11 | Chlorella |

### Fungi
| Device | ID | Name |
|--------|-----|------|
| mushroom | 0x20 | Mushroom |
| fungus | 0x21 | Tremella |

### Animal
| Device | ID | Name |
|--------|-----|------|
| bee | 0x30 | Bee |
| earthworm | 0x31 | Earthworm |

### Insect
| Device | ID | Name |
|--------|-----|------|
| cricket | 0x40 | Cricket |
| mealworm | 0x41 | Mealworm |

## References

- [Tuya Developer Platform](https://developer.tuya.com/)
- [TuyaOS Documentation](https://developer.tuya.com/cn/docs/iot-device-dev)
- [PlatformIO](https://platformio.org/)
