# Generic Game System（通用游戏系统）

<div align="center">

![GenericGameSystem](https://img.shields.io/badge/版本-1.6-blue)
![UE5.7+](https://img.shields.io/badge/UE5.6-兼容-green)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Platform](https://img.shields.io/badge/平台-Windows%20%7C%20Android%20%7C%20Linux-lightgrey)

[**官网**](https://yuewu.dev) • [**文档**](https://www.yuewu.dev/wiki/a1d2yzik8sZuC3aj3Il3X) • [**Discord**](https://discord.com/invite/xMRXAB2) • [**Fab**](https://www.fab.com/listings/98b2c4a0-9520-4d6b-8bc2-86d5c82612ca) • [**English**](README.md)

**一个为虚幻引擎 5.7+ 提供核心游戏系统的综合多模块插件**

</div>

---

## 📖 概述

**Generic Game System (GGS)** 是一个免费的多模块插件，旨在为任何类型的游戏提供基础功能。它通过增强的视觉特效、相机管理、UI 扩展和核心工具扩展了虚幻引擎的原生能力。

### 🎯 核心特性

- **上下文特效系统**：基于 Gameplay 标签驱动的 VFX/SFX 播放
- **高级相机管理**：灵活的相机模式管理系统
- **UI 扩展**：增强的 CommonUI 组件和工具
- **交互系统**：用于游戏对象的健壮交互框架
- **跨平台支持**：支持 Windows、Android 和 Linux

## 🏗️ 架构

GGS 采用模块化架构，由四个独立模块组成：

### GenericEffectsSystem（特效系统）
- **用途**：基于上下文的视觉和音效播放
- **特性**：
  - Gameplay 标签驱动的特效触发
  - Niagara 集成，实现高级粒子效果
  - 高效的特效池管理和性能优化
  - 与 GameplayAbilities 无缝集成

### GenericCameraSystem（相机系统）
- **用途**：高级相机模式管理
- **特性**：
  - 多相机模式支持，过渡流畅
  - 第三人称和第一人称相机实现
  - 目标锁定和追踪功能
  - Enhanced Input 集成，实现相机控制

### GenericUISystem（UI 系统）
- **用途**：扩展的 CommonUI 功能
- **特性**：
  - 增强的 UI 组件和控件
  - 输入路由和操作处理
  - 数据驱动的 UI 系统
  - 本地化支持

### GenericGameSystem（核心系统）
- **用途**：核心工具和共享功能
- **特性**：
  - 用于游戏对象的交互框架
  - 通用工具和辅助函数
  - 跨模块集成层

## 🚀 安装

### 方法 1：Fab 市场（推荐）
1. 打开虚幻引擎 5.7
2. 导航至 **Fab** 市场
3. 搜索 "Generic Game System"
4. 点击 **Add to Project**
5. 在 **编辑 → 插件 → Generic Game System** 中启用

### 方法 2：手动安装
1. 下载插件源代码
2. 将 `GenericGameSystem` 文件夹放入项目的 `Plugins/` 目录
3. 重启项目
4. 在 **编辑 → 插件 → Generic Game System** 中启用

### 方法 3：Git 子模块
```bash
cd YourProject/Plugins
git submodule add https://github.com/luochuanyuewu/GenericGameSystem.git
```

## 📋 系统要求

- **虚幻引擎**：5.7 或更高版本
- **必需插件**：
  - Niagara（粒子效果）
  - CommonUI（UI 系统）
  - Enhanced Input（输入处理）
  - ModularGameplay（模块化架构）
  - TargetingSystem（相机目标锁定）
  - SmartObjects（AI 交互）
  - GameplayBehaviors（AI 行为）
  - GameplayBehaviorSmartObjects（AI-智能对象集成）

## 🎮 使用方法

### 基础设置

1. **启用插件**：导航至 **编辑 → 插件 → Generic Game System** 并启用
2. **重启编辑器**：关闭并重新打开项目
3. **添加组件**：将所需的系统组件附加到 Actor 类

### 快速上手示例

#### 播放上下文特效
```cpp
#include "Feedback/GES_ContextEffectComponent.h"

UGES_ContextEffectComponent* CtxEffectSys = ActorPtr->FindComponentByClass<UGES_ContextEffectComponent>();
if (CtxEffectSys)
{
    CtxEffectSys->PlayContextEffectsWithInput(InputParams);
}
```

#### 设置相机模式
```cpp
#include "GCMS_CameraSystemComponent.h"

UGCMS_CameraSystemComponent* CameraSys = UGCMS_CameraSystemComponent::GetCameraSystemComponent();
if (CameraSys)
{
    // 设置相机模式
    CameraSys->PushCameraMode(CameraModeClass);
}
```

## 📜 许可

本项目采用 **MIT 许可证** - 详见 [LICENSE](LICENSE) 文件。

**概要：**

- ✅ 个人和商业项目中免费使用
- ✅ 自由修改和适应
- ✅ 可在编译的游戏中分发（最终产品）
- ✅ 需要保留版权声明

**注意：** 虽然插件本身采用 MIT 开源许可，但请尊重原作者的工作，并考虑通过 [Fab 市场](https://www.fab.com/listings/98b2c4a0-9520-4d6b-8bc2-86d5c82612ca) 支持我们。

## 🌟 其他插件

探索 Generic* 生态系统中的更多插件：

- **GenericGameFramework** - 整包（开发中）
- **[GenericGameplayAbilities](https://2fab.link/gga)** - 全面的 GAS 扩展
- **[GenericCombatSystem](https://2fab.link/gcs)** - 高级战斗框架
- **[GenericMovementSystem](https://2fab.link/gms)** - 现代移动系统
- **[GenericInventorySystem](https://2fab.link/gis)** - 灵活的库存框架
- **[GenericInputSystem](https://2fab.link/gips)** - 增强的输入处理
- **GenericAISystem** - 基于 StateTree 的 AI系统（开发中）

---

<div align="center">

**由 [YuewuDev](https://yuewu.dev) 用 ❤️ 制作**

[English](README.md) | [中文](README_CN.md)

</div>
