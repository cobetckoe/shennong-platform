# Shennong Platform

Distributed AI agriculture production system.

## Architecture

- Gateway: WBR3 + ZS3L (Zigbee bridge)
- Cultivators: LK Shield + ZS3L (local control)
- Auxiliary: Standalone devices

## Project Structure

```
shennong-platform/
├── gateway/
│   └── wbr3/
├── auxiliary/
├── cultivators/
│   ├── plant-farm/
│   │   ├── tuber/          # 0x02
│   │   ├── root/           # 0x03
│   │   ├── stem/           # 0x04
│   │   ├── leaf/           # 0x05
│   │   ├── flower/         # 0x06
│   │   └── fruit/          # 0x07
│   ├── algae-farm/
│   │   ├── spiral/         # 0x10
│   │   └── chlorella/      # 0x11
│   ├── fungi-farm/
│   │   ├── mushroom/       # 0x20
│   │   └── fungus/         # 0x21
│   ├── animal-farm/
│   │   ├── bee/            # 0x30
│   │   └── earthworm/      # 0x31
│   └── insect-farm/
│       ├── cricket/        # 0x40
│       └── mealworm/       # 0x41
├── shared/
└── docs/
```

## Device List

### Plant

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

## Hardware

| Device | Components | Dev | Cost |
|--------|-----------|-----|------|
| Gateway | WBR3 + ZS3L | TuyaOS | ¥25 |
| Cultivator | LK Shield + ZS3L | PlatformIO | ¥20 |

## Communication

```
Tuya Cloud ←WiFi→ Gateway ←Zigbee→ Cultivator
```

## Development

```bash
cd cultivators/plant-farm/tuber
pio run
```
