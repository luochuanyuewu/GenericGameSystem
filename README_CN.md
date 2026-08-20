# Generic Game System（通用游戏系统）

<div align="center">

![GenericGameSystem](https://img.shields.io/badge/版本-1.6-blue)
![UE5.7+](https://img.shields.io/badge/UE5.7%2B-兼容-green)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Platform](https://img.shields.io/badge/平台-Windows%20%7C%20macOS%20%7C%20Android%20%7C%20Linux-lightgrey)

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
- **跨平台支持**：支持 Windows、macOS、Android 和 Linux

## 🏗️ 架构

GGS 采用模块化架构，由七个 Runtime 模块和一个仅编辑器集成模块组成：

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

### GenericSettingsSystem（通用设置系统）
- **用途**：通用运行时游戏设置框架
- **特性**：
  - 面向本地玩家的设置注册表和设置集合
  - 支持统一 Local/Shared Accessor 的离散值与标量值
  - 支持应用、恢复、过滤和编辑条件
  - 蓝图编辑条件按引用接收 `FGSS_GameSettingEditableState`，可通过 `UGSS_GameSettingEditableStateLibrary` 隐藏、禁用、排除选项或禁止重置。内置条件还可以检查另一个设置的待应用值，或使用 `FPlatformProperties::SupportsWindowedMode()`
  - 支持 Data Asset 配置，以及面向蓝图/C++ 的 Local/Shared 轻量反射值 Accessor。Data Asset 的 Getter/Setter 为下拉列表，按项目 `GameUserSettings` 类（Local）或 Shared Settings Class（Shared）上签名匹配的函数筛选；无效配对会在注册时被拒绝。C++ Provider 可使用 `GSS_MAKE_LOCAL_ACCESSOR` / `GSS_MAKE_SHARED_ACCESSOR` 在编译期检查函数名。Scalar Definition 与 `AddScalar` 可选择原始值、百分比或整数等内置显示格式
  - 支持运行时 Provider 注册、本地 `UGameUserSettings` 和按 LocalPlayer 隔离的 Shared SaveGame 偏好
  - `GenericSettingsSystemCommon` 提供可选的 Video、Audio、Input、Accessibility 和 Language Provider。`UGSS_CommonSettingsShared` 是可携带偏好的预制基类；项目应继承它并重写 `ApplySettings`，将已提交的 Audio、Input 与 Accessibility 值应用到自身系统。Input 仅负责设备偏好；玩法策略、Enhanced Input 改键、平台设备选择、性能统计和在线/DLC 设置应由项目或可选集成 Provider 提供。离散 Accessor 将布尔值持久化为 `true`/`false`，将枚举持久化为枚举名。Common Video 仅允许主本地玩家修改机器级 `UGameUserSettings` 项，在不支持窗口化显示的平台隐藏窗口模式和分辨率，在窗口化全屏下禁用分辨率，并仅在独占全屏下启用垂直同步。若只需隐藏个别 Video 项，请创建 `UGSS_VideoSettingsProvider` 的蓝图子类并取消 `IncludedSettings` 中的对应行，而不必重写 `RegisterSettings`。亮度写入 `GEngine->DisplayGamma`，并持久化到 GameUserSettings.ini 的 `[GSS.Video]`。Language 会枚举已 cook 的游戏 Culture，空值表示系统默认，并写入 Unreal 的 Internationalization Culture；部分语言变更仍需要重启。按玩家隔离的 Audio、Input、Accessibility 偏好仍可由每个本地玩家修改
  - `GenericSettingsSystemUI` 将 GSS 注册表适配到现有 CommonUI/GUIS 布局，避免运行时核心依赖 UI

**快速配置**：创建 `UGSS_CommonSettingsShared` 的蓝图或 C++ 子类，重写 `ApplySettings` 将已提交的可携带偏好同步到项目系统，并在 **项目设置 → Generic Settings System → Shared Settings Class** 中配置该子类。再在 **Startup Provider Classes** 中按需加入 `UGSS_VideoSettingsProvider`、`UGSS_AudioSettingsProvider`、`UGSS_InputSettingsProvider`、`UGSS_AccessibilitySettingsProvider`、`UGSS_LanguageSettingsProvider`。普通值项使用 Local 或 Shared Accessor，并配置零参数 `UFUNCTION` Getter 与单参数 `UFUNCTION` Setter。创建继承 `UGSS_GameSettingsMenu` 的蓝图，绑定名为 `Settings_Panel` 的 `UGSS_GameSettingsPanel`，其列表控件名为 `ListView_Settings`；再为该列表配置 `UGSS_GameSettingsEntryWidgetFactory`，并提供离散值、标量、Action 和导航节点的蓝图条目 Widget。设置条目继承 `CommonUserWidget`：离散条目可选绑定 `Panel_Value`、`Rotator_SettingValue`、`Button_Decrease`、`Button_Increase`；标量条目可选绑定 `Panel_Value`、`Slider_SettingValue`、`Text_SettingValue`；命令行可选绑定 `Button_Action` 或 `Button_Navigate`。若需标准详情区，在 Panel 中添加名为 `Details_Settings`、继承 `UGSS_GameSettingsDetailView` 的 Widget，并按需绑定标题、说明、动态详情、警告和禁用详情字段。该类还继承 `UGUIS_ListEntryDetailView`：可绑定 `Box_DetailSections`、指定 `UGSS_GameSettingsDetailSectionBuilder`，并将 `UGSS_GameSetting` 或其子类映射到一个或多个继承 `UGSS_GameSettingsDetailSection` 的 Section 蓝图。

**自动顶层 Tab**：在 Menu 蓝图启用 `bAutoBuildTopSettingsTabs`，并为 `TopSettingsTabButtonType` 指定按钮类，即可根据顶层 GSS Collection 自动构建 Tab。Menu 使用每个 Collection 的 Tag 名与显示名，且仅响应它自动创建的 Tab 的导航请求。

**内置布局**：Video 包含 Display（窗口模式、分辨率、亮度）、Graphics Quality（含各项 Scalability）和 Advanced 渲染设置；Audio 包含 Volume 与 Sound；Input 包含 Mouse & Keyboard 与 Gamepad；Accessibility 包含 Subtitles、Color Vision 与 Motion；Language 包含 Culture 选择器。除 Video 与 Language 外的设置均是偏好值：应由所属游戏系统订阅对应 `GSS.Settings.*` Tag 后实际执行。Language 会在 Apply 时写入 Unreal 的 Culture 设置。

### GenericGameSystem（核心系统）
- **用途**：核心工具和共享功能
- **特性**：
  - 用于游戏对象的交互框架
  - 通用工具和辅助函数
  - 跨模块集成层

### GenericGameSystemEditor（编辑器模块）

- **用途**：仅编辑器的内容浏览器集成
- **特性**：
  - 在 **Generic Game System | Effects**、**Gameplay**、**UI**、**Settings** 下提供右键资产创建入口
  - 为主要 GGS Data Asset 与配置资产提供创建项

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
