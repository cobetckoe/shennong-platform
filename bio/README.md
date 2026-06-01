# 生物分类架构

本目录按生物分类提供设备分类参考。实际设备代码位于 `firmware/arduino/` 中，采用相同的生物分类组织方式。

## 核心分类

- **Kingdom_Plantae** - 被子植物类设备（种子播种机、蔬菜气雾机、土豆气雾机）
- **Kingdom_Bacteria** - 蓝绿藻/微藻设备（螺旋藻培养机）
- **Kingdom_Fungi** - 真菌类设备（蘑菇种植机）

## 代码映射关系

| 生物分类 | 代码位置 | 设备 |
|---------|---------|------|
| `Kingdom_Plantae/Phylum_Magnoliophyta/` | `firmware/arduino/Kingdom_Plantae/Phylum_Magnoliophyta/` | 种子播种机、蔬菜气雾机、土豆气雾机 |
| `Kingdom_Bacteria/Phylum_Cyanobacteria/` | `firmware/arduino/Kingdom_Bacteria/Phylum_Cyanobacteria/` | 螺旋藻培养机 |
| `Kingdom_Fungi/Phylum_Basidiomycota/` | `firmware/arduino/Kingdom_Fungi/Phylum_Basidiomycota/` | 蘑菇种植机 |

## 目的

通过生物分类方式组织模块，更贴近农业生态与作物类型，便于模块扩展与行业传承。
