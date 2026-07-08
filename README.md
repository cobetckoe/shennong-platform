# Shennong Platform

Distributed AI agriculture production system.

## Project Structure

```
shennong-platform/
├── gateway/              # Gateway device
│   └── wbr3/
├── auxiliary/            # Standalone device
├── subdevice/            # Sub device (via gateway)
│   ├── plant/
│   │   ├── tuber/       # 0x02
│   │   ├── root/        # 0x03
│   │   ├── stem/        # 0x04
│   │   ├── leaf/        # 0x05
│   │   ├── flower/      # 0x06
│   │   └── fruit/       # 0x07
│   ├── algae/
│   │   ├── spiral/      # 0x10
│   │   └── chlorella/   # 0x11
│   ├── fungi/
│   │   ├── mushroom/    # 0x20
│   │   └── fungus/      # 0x21
│   ├── animal/
│   │   ├── bee/         # 0x30
│   │   └── earthworm/   # 0x31
│   └── insect/
│       ├── cricket/     # 0x40
│       └── mealworm/    # 0x41
├── shared/
└── docs/
```

## Device Types

| Type | Directory | Connection |
|------|-----------|------------|
| Gateway | gateway/ | WiFi + Zigbee |
| Auxiliary | auxiliary/ | Standalone |
| Sub Device | subdevice/ | Zigbee |

## Hardware

| Device | Components | Dev | Cost |
|--------|-----------|-----|------|
| Gateway | WBR3 + ZS3L | TuyaOS | ¥25 |
| Sub Device | LK Shield + ZS3L | PlatformIO | ¥20 |

## Communication

```
Tuya Cloud ←WiFi→ Gateway ←Zigbee→ Sub Device
```

## Development

```bash
cd subdevice/plant/tuber
pio run
```
