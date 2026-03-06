# Generic Game System

<div align="center">

![GenericGameSystem](https://img.shields.io/badge/Version-1.6-blue)
![UE5.6+](https://img.shields.io/badge/UE5.6-Compatible-green)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Android%20%7C%20Linux-lightgrey)

[**Website**](https://yuewu.dev/en) • [**Documentation**](https://www.yuewu.dev/wiki/a1d2yzik8sZuC3aj3Il3X) • [**Discord**](https://discord.com/invite/xMRXAB2) • [**Fab**](https://www.fab.com/listings/98b2c4a0-9520-4d6b-8bc2-86d5c82612ca) • [**中文**](README_CN.md)

**A comprehensive multi-module plugin for Unreal Engine 5.6+ providing essential game systems**

</div>

---

## 📖 Overview

**Generic Game System (GGS)** is a free, multi-module plugin designed to provide foundational functionalities for any type of game. It extends Unreal Engine's native capabilities with enhanced systems for visual effects, camera management, UI extensions, and core utilities.

### 🎯 Key Features

- **Context-Based Effects System**: Gameplay Tag-driven VFX/SFX playback
- **Advanced Camera Management**: Flexible camera mode management system
- **UI Extensions**: Enhanced CommonUI components and utilities
- **Interaction System**: Robust interaction framework for gameplay objects
- **Cross-Platform**: Supports Windows, Android, and Linux

## 🏗️ Architecture

GGS is built with a modular architecture consisting of four independent modules:

### GenericEffectsSystem
- **Purpose**: Context-based visual and sound effects playback
- **Features**:
  - Gameplay Tag-driven effect triggering
  - Niagara integration for advanced particle effects
  - Efficient effect pooling and management
  - Seamless integration with GameplayAbilities

### GenericCameraSystem
- **Purpose**: Advanced camera mode management
- **Features**:
  - Multiple camera mode support with smooth transitions
  - Third-person and first-person camera implementations
  - Target locking and tracking
  - Enhanced Input integration for camera controls

### GenericUISystem
- **Purpose**: Extended CommonUI functionality
- **Features**:
  - Enhanced UI components and widgets
  - Input routing and action handling
  - Data-driven UI systems
  - Localization support

### GenericGameSystem
- **Purpose**: Core utilities and shared functionality
- **Features**:
  - Interaction framework for gameplay objects
  - Common utilities and helper functions
  - Cross-module integration layer

## 🚀 Installation

### Method 1: Fab Marketplace (Recommended)
1. Open Unreal Engine 5.6
2. Navigate to **Fab** marketplace
3. Search for "Generic Game System"
4. Click **Add to Project**
5. Enable the plugin in **Edit → Plugins → Generic Game System**

### Method 2: Manual Installation
1. Download the plugin source
2. Place the `GenericGameSystem` folder in your project's `Plugins/` directory
3. Restart your project
4. Enable the plugin in **Edit → Plugins → Generic Game System**

### Method 3: Git Submodule
```bash
cd YourProject/Plugins
git submodule add https://github.com/luochuanyuewu/GenericGameSystem.git
```

## 📋 Requirements

- **Unreal Engine**: 5.6 or higher
- **Required Plugins**:
  - Niagara (for particle effects)
  - CommonUI (for UI system)
  - Enhanced Input (for input handling)
  - ModularGameplay (for modular architecture)
  - TargetingSystem (for camera targeting)
  - SmartObjects (for AI interaction)
  - GameplayBehaviors (for AI behaviors)
  - GameplayBehaviorSmartObjects (for AI-smart object integration)

## 🎮 Usage

### Basic Setup

1. **Enable the Plugin**: Navigate to **Edit → Plugins → Generic Game System** and enable it
2. **Restart the Editor**: Close and reopen your project
3. **Add Components**: Attach desired system components to your Actor classes

### Quick Start Examples

#### Playing Context Effects
```cpp
#include "Feedback/GES_ContextEffectComponent.h"

UGES_ContextEffectComponent* CtxEffectSys = ActorPtr->FindComponentByClass<UGES_ContextEffectComponent>();
if (CtxEffectSys)
{
    CtxEffectSys->PlayContextEffectsWithInput(InputParams);
}
```

#### Setting Up Camera Modes
```cpp
#include "GCMS_CameraSystemComponent.h"

UGCMS_CameraSystemComponent* CameraSys = UGCMS_CameraSystemComponent::GetCameraSystemComponent();
if (CameraSys)
{
    // Set camera mode
    CameraSys->PushCameraMode(CameraModeClass);
}
```

## 📜 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

**Summary:**

- ✅ Free to use in personal and commercial projects
- ✅ Free to modify and adapt
- ✅ Free to distribute in compiled games (shipped products)
- ✅ Attribution required (keep copyright notice)

**Note:** While the plugin itself is open source under MIT, please respect the original author's work and consider supporting via [Fab marketplace](https://www.fab.com/listings/98b2c4a0-9520-4d6b-8bc2-86d5c82612ca).

## 🌟 Other Plugins

Explore more plugins in the Generic* ecosystem:

- **GenericGameFramework** - Bundle version (In Development)
- **[GenericGameplayAbilities](https://2fab.link/gga)** - Comprehensive GAS extensions
- **[GenericCombatSystem](https://2fab.link/gcs)** - Advanced combat framework
- **[GenericMovementSystem](https://2fab.link/gms)** - Modern movement system
- **[GenericInventorySystem](https://2fab.link/gis)** - Flexible inventory framework
- **[GenericInputSystem](https://2fab.link/gips)** - Enhanced input handling
- **GenericAISystem** - StateTree-based AI system (In Development)

---

<div align="center">

**Made with ❤️ by [YuewuDev](https://yuewu.dev/en)**

[English](README.md) | [中文](README_CN.md)

</div>
