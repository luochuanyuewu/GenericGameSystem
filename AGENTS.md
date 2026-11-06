# GenericGameSystem

本文件是 `Plugins/GenericGameSystem` 的插件级上下文。宿主路径、引擎路径与通用构建命令见仓库根 `AGENTS.md`。使用者说明见本插件 `README.md` / `README_CN.md`。

## 定位

UE 5.8+ code-only 插件（`CanContainContent: false`），提供通用游戏基础能力。行为归属所属模块，不要跨模块塞逻辑，不要包含其它模块的 `Private` 头。

## 模块边界

| 模块 | 职责 |
|------|------|
| `GenericEffectsSystem` | Tag + Niagara 上下文 VFX/SFX（`GES_`） |
| `GenericCameraSystem` | 相机模式栈、混合、穿透规避（`GCMS_`） |
| `GenericUISystem` | CommonUI 子系统、布局、输入路由（`GUIS_`） |
| `GenericSettingsSystem` | 设置模型、Registry、Provider、变更追踪（`GSS_`） |
| `GenericSettingsSystemCommon` | Video/Audio/Input/Accessibility 等通用 Provider |
| `GenericSettingsSystemUI` | CommonUI 设置展示；不把 UI 放进核心 GSS |
| `GenericGameSystem` | 交互、布娃娃、Smart Objects、Targeting 等共享工具（`GGS_`） |
| `GenericGameSystemEditor` | DataAsset 创建入口（Editor only） |

## 约束

- 除非任务明确要求，不要新增 `Content/` 或示例地图
- 改 `*.Build.cs` 时同步核对 Public/Private 依赖
- 不要擅自引入第三方库或新插件依赖

## 高风险区域

- 反射、复制、RPC、组件所有权
- Tag 驱动流程：结构体、Query、数据入口、蓝图路径一起查
- UI：子系统、Policy、Layout、激活/出栈
- 相机：Mode Stack、Blend、碰撞规避
- 交互 / 布娃娃 / Smart Objects：网络权限与 Owner-only
- 设置树：`AddEditCondition` 在 Initialize 后刷新 EditableState；Shared Settings 加载后立刻 `ApplySettings`；注销 Provider 时整树恢复脏值

## 优先阅读

- `GES_ContextEffectComponent.h`、`GCMS_CameraSystemComponent.h`、`GCMS_CameraModeStack.h`
- `GGS_InteractionSystemComponent.h`、`GGS_RagdollComponent.h`
- `GUIS_GameUISubsystem.h`、`GUIS_GameUIPolicy.h`
- `GSS_GameSettingRegistry.h`、`GSS_GameSettingsSubsystem.h`、`GSS_SettingsShared.h`

对照 Epic GameSettings / Lyra 时用本机 Lyra 5.8（`/Users/kinson/Documents/Unreal Projects/Lyra5.8`），不要用旧的独立 GameSettings 路径。
