# Generic Game System

<div align="center">

![GenericGameSystem](https://img.shields.io/badge/Version-1.6-blue)
![UE5.7+](https://img.shields.io/badge/UE5.7-Compatible-green)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Android%20%7C%20Linux-lightgrey)

[**Website**](https://yuewu.dev/en) • [**Documentation**](https://www.yuewu.dev/wiki/a1d2yzik8sZuC3aj3Il3X) • [**Discord**](https://discord.com/invite/xMRXAB2) • [**Fab**](https://www.fab.com/listings/98b2c4a0-9520-4d6b-8bc2-86d5c82612ca) • [**中文**](README_CN.md)

**A comprehensive multi-module plugin for Unreal Engine 5.7+ providing essential game systems**

</div>

---

## 📖 Overview

**Generic Game System (GGS)** is a free, multi-module plugin designed to provide foundational functionalities for any type of game. It extends Unreal Engine's native capabilities with enhanced systems for visual effects, camera management, UI extensions, and core utilities.

### 🎯 Key Features

- **Context-Based Effects System**: Gameplay Tag-driven VFX/SFX playback
- **Advanced Camera Management**: Flexible camera mode management system
- **UI Extensions**: Enhanced CommonUI components and utilities
- **Interaction System**: Robust interaction framework for gameplay objects
- **Cross-Platform**: Supports Windows, macOS, Android, and Linux

## 🏗️ Architecture

GGS is built with seven Runtime modules plus an editor-only integration module:

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

### GenericSettingsSystem
- **Purpose**: Generic runtime game-settings framework
- **Features**:
  - Per-local-player setting registries and setting collections
  - Discrete and scalar values with unified Local/Shared accessors
  - Apply, restore, filtering, and edit-condition support
	- Blueprint edit conditions receive an `FGSS_GameSettingEditableState` value by reference; use `UGSS_GameSettingEditableStateLibrary` to hide, disable, exclude options, or prevent reset
  - Data Asset authoring plus Local/Shared reflected accessors for Blueprint/C++ targets
	- Runtime provider registration, local `UGameUserSettings`, and per-LocalPlayer Shared SaveGame preferences
	- `GenericSettingsSystemCommon` supplies optional Video, Audio, Input, and Accessibility providers. `UGSS_CommonSettingsShared` is a portable-preference preset: projects should subclass it and override `ApplySettings` to apply committed Audio, Input, and Accessibility values to their own systems. Input covers device preferences only; gameplay policy, Enhanced Input rebinding, platform device selection, performance telemetry, and online/DLC settings remain project or optional integration Providers.
  - `GenericSettingsSystemUI` adapts GSS registries to the existing CommonUI/GUIS layout without coupling the runtime core to UI

**Quick setup**: Create a Blueprint or C++ subclass of `UGSS_CommonSettingsShared`, override `ApplySettings` to propagate committed portable preferences, and set that subclass as **Project Settings → Generic Settings System → Shared Settings Class**. Then add any needed `UGSS_VideoSettingsProvider`, `UGSS_AudioSettingsProvider`, `UGSS_InputSettingsProvider`, and `UGSS_AccessibilitySettingsProvider` classes to **Startup Provider Classes**. A standard value uses a Local or Shared Accessor with zero-argument `UFUNCTION` Getter and one-argument `UFUNCTION` Setter. Create a Blueprint derived from `UGSS_SettingsScreen`, bind a `UGSS_SettingsPanel` named `Settings_Panel`, then configure the panel list's `UGSS_SettingsEntryWidgetFactory` with Blueprint entry widgets for discrete, scalar, action, and navigation nodes. Settings entries derive from `CommonUserWidget`: optional native bindings are `Panel_Value`, `Rotator_SettingValue`, `Button_Decrease`, and `Button_Increase` for discrete entries; `Panel_Value`, `Slider_SettingValue`, and `Text_SettingValue` for scalar entries; and `Button_Action` / `Button_Navigate` for command rows. For optional standard details, add a `UGSS_SettingsDetailView` named `Details_Settings` and bind any of its title, description, dynamic-details, warning and disabled-details fields. It also inherits `UGUIS_ListEntryDetailView`: bind `Box_DetailSections`, assign a `UGSS_SettingsDetailSectionBuilder`, and map `UGSS_GameSetting` or its subclasses to one or more Section Blueprints derived from `UGSS_SettingsDetailSection`.

**Automatic top tabs**: Enable `bAutoBuildTopSettingsTabs` on the Screen Blueprint and assign `TopSettingsTabButtonType` to build tabs from top-level GSS Collections. The Screen uses each Collection's tag name and display name, and navigates on the automatically created tabs only.

**Built-in layout**: Video contains Display, Graphics Quality (including individual scalability groups), and Advanced rendering settings. Audio contains Volume and Sound. Input contains Mouse & Keyboard and Gamepad collections. Accessibility contains Subtitles, Color Vision, and Motion. Every non-Video setting is a preference: subscribe to its `GSS.Settings.*` tag in the owning game system to enact it.

### GenericGameSystem
- **Purpose**: Core utilities and shared functionality
- **Features**:
  - Interaction framework for gameplay objects
  - Common utilities and helper functions
  - Cross-module integration layer

### GenericGameSystemEditor

- **Purpose**: Editor-only Content Browser integration
- **Features**:
  - Right-click asset creation under **Generic Game System | Effects**, **Gameplay**, **UI**, and **Settings**
  - Creation entries for the primary GGS Data Asset and configuration asset types

## 🚀 Installation

### Method 1: Fab Marketplace (Recommended)
1. Open Unreal Engine 5.7
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

- **Unreal Engine**: 5.7 or higher
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
