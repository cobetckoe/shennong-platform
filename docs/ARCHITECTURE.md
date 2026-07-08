# Architecture

## System Topology

```
Tuya Cloud ←WiFi→ Gateway ←Zigbee→ Sub Device
                                ←Zigbee→ Sub Device
                                ←Zigbee→ Sub Device
```

## Device Types

| Type | Directory | Connection | Function |
|------|-----------|------------|----------|
| Gateway | gateway/ | WiFi + Zigbee | Bridge to cloud |
| Auxiliary | auxiliary/ | Standalone | Independent operation |
| Sub Device | subdevice/ | Zigbee | Local control |

## Hardware

| Device | Components | SDK |
|--------|-----------|-----|
| Gateway | WBR3 + ZS3L | TuyaOS |
| Sub Device | LK Shield + ZS3L | PlatformIO |

## Communication Protocol

| Command | Direction | Description |
|---------|-----------|-------------|
| 0x01 | Sub → Gateway | Heartbeat |
| 0x02 | Bidirectional | Sensor data / Control rule |

## Control Rule Structure

```c
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;
```

## Sensor Data Structure

```c
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;
```

## Data Flow

1. Sub device collects sensor data
2. Sub device sends data to gateway via Zigbee
3. Gateway reports data to Tuya Cloud
4. Tuya Cloud analyzes and generates rules
5. Tuya Cloud sends rules to gateway
6. Gateway forwards rules to sub devices
7. Sub device executes control locally
