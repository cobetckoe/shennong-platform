# Shennong Platform

Distributed AI agriculture production system.

## Architecture

- Gateway: WBR3 + ZS3L (Zigbee bridge)
- Cultivators: LK Shield + ZS3L (local control)
- Auxiliary: Standalone devices

## Project Structure

```
shennong-platform/
├── gateway/                      # Gateway
│   └── wbr3/                   # WBR3 gateway (TuyaOS)
├── auxiliary/                    # Standalone devices
├── cultivators/                  # Cultivators
│   ├── plants/                 # Plants
│   │   ├── tuber-planter/     # Tuber (0x02)
│   │   ├── root-planter/      # Root (0x03)
│   │   ├── stem-planter/      # Stem (0x04)
│   │   ├── leaf-planter/      # Leaf (0x05)
│   │   ├── flower-planter/    # Flower (0x06)
│   │   └── fruit-planter/     # Fruit (0x07)
│   ├── algae-farm/              # Algae
│   │   ├── spiral-algae/      # Spirulina (0x10)
│   │   └── chlorella-algae/   # Chlorella (0x11)
│   ├── fungi-farm/              # Fungi
│   │   ├── mushroom/          # Mushroom (0x20)
│   │   └── fungus/            # Tremella (0x21)
│   ├── animal-farm/             # Animal
│   │   ├── bee/               # Bee (0x30)
│   │   └── earthworm/         # Earthworm (0x31)
│   └── insect-farm/             # Insect
│       ├── cricket/           # Cricket (0x40)
│       └── mealworm/          # Mealworm (0x41)
├── shared/                       # Shared
└── docs/                         # Docs
```

## Device List

### Plants (edible parts)

| Device | ID | Part | Crops |
|--------|-----|------|-------|
| tuber-planter | 0x02 | Tuber | Potato |
| root-planter | 0x03 | Root | Radish |
| stem-planter | 0x04 | Stem | Celery |
| leaf-planter | 0x05 | Leaf | Spinach |
| flower-planter | 0x06 | Flower | Broccoli |
| fruit-planter | 0x07 | Fruit | Tomato |

### Algae

| Device | ID | Name |
|--------|-----|------|
| spiral-algae | 0x10 | Spirulina |
| chlorella-algae | 0x11 | Chlorella |

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
cd cultivators/plants/tuber-planter
pio run
```
