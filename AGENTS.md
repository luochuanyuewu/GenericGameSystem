# AGENTS.md

## 这个文件的作用

- Codex 会在开始工作前读取本文件，用它理解当前仓库的架构、约定、验证方式和高风险区域。
- 本文件只写本仓库长期有效的项目规则。个人偏好、临时任务说明、一次性计划不要放在这里。
- 如果某个子目录以后需要更细的规则，可以在更靠近该目录的位置添加 `AGENTS.md` 或 `AGENTS.override.md`。Codex 会从仓库根目录一路读到当前工作目录，越靠近当前目录的说明优先级越高。
- 如果发现指令没有生效，先确认 Codex 是从本仓库根目录或其子目录启动的，并检查上层目录或 Codex Home 中是否存在 `AGENTS.override.md`。

## 项目概览

- `GenericGameSystem` 是一个面向 Unreal Engine 5.7+ 的代码型插件仓库，不包含可运行游戏工程。
- 插件不包含内容资产；`GenericGameSystem.uplugin` 中 `CanContainContent` 为 `false`。
- 插件面向 Windows、macOS、Android、Linux，主打通用游戏基础能力：上下文特效、相机模式、CommonUI 扩展、交互、布娃娃和 Smart Objects 集成。
- 根目录中的 `README.md`、`README_CN.md` 面向使用者；本文件面向 Codex 和维护者，说明如何安全地修改代码。

## 模块边界

插件由 `Source/` 下七个 Runtime 模块和一个仅编辑器加载的模块组成：

- `GenericEffectsSystem`：基于 Gameplay Tags 和 Niagara 的上下文 VFX/SFX 播放。
- `GenericCameraSystem`：相机模式栈、相机混合、第三人称相机和穿透规避。
- `GenericUISystem`：基于 CommonUI 的 UI 子系统、布局策略、扩展点和输入路由。
- `GenericSettingsSystem`：面向本地玩家的通用运行时设置模型、Data Asset 定义、Local/Shared Accessor、注册表、运行时 Provider 和变更追踪。
- `GenericSettingsSystemCommon`：可选的通用设置 Provider 与 `UGSS_CommonSettingsShared`：Video 使用 `UGameUserSettings`，Audio、Input、Accessibility 存储在按 LocalPlayer 隔离的 Shared SaveGame 中。
- `GenericSettingsSystemUI`：依赖 `GenericSettingsSystem` 与 `GenericUISystem` 的 CommonUI 设置展示适配层；不在核心 GSS 中放置 UI。
- `GenericGameSystem`：共享 gameplay 工具、交互、布娃娃、Smart Objects、Targeting 和跨模块集成。
- `GenericGameSystemEditor`：仅在 Editor 中加载；为主要 GGS Data Asset 和配置资产提供 Content Browser 创建入口。

维护时优先在行为所属模块内扩展，不要为了方便把跨模块逻辑塞进无关模块。不要包含其他模块的 `Private` 头文件。

## 仓库结构

- `Source/<Module>/Public`：导出的 C++ 头文件、Blueprint 可见 API、跨模块可用类型。
- `Source/<Module>/Private`：模块内部实现。模块私有细节应留在这里。
- `Source/<Module>/<Module>.Build.cs`：模块依赖声明。新增 include 或 UE 类型时，要同步检查依赖是否应放在 Public 或 Private。
- `Config/`：随插件分发的配置。
- `Resources/`：编辑器侧资源，例如插件图标。
- `Binaries/`、`Intermediate/`：构建产物。除非任务明确要求，不要手动编辑或提交由构建生成的内容。

## 命名和代码风格

- 保持 Unreal Engine 命名和反射习惯，尤其是 `UCLASS`、`USTRUCT`、`UFUNCTION`、`UPROPERTY` 的元数据。
- 保持现有前缀：
  - `GGS_`：核心玩法系统类型。
  - `GES_`：特效系统类型。
  - `GCMS_`：相机系统类型。
  - `GUIS_`：UI 系统类型。
  - `GSS_`：设置系统类型。
- 触碰 Public API 时，Blueprint 暴露、复制行为、类别、元数据和注释风格要与周围代码一致。
- 许多 Public 头文件使用中英双语注释。修改已有文档化 API 时匹配本地风格；不要添加重复或空泛注释。
- 代码修改保持小而聚焦。不要顺手格式化无关文件，也不要做与任务无关的重构。

## 依赖和内容约束

- 这是 code-only 插件。除非任务明确要求，不要新增 `Content/`、资产依赖或示例地图。
- 修改 `*.Build.cs` 时要特别谨慎：Unreal 的编译错误经常来自缺失模块依赖，而不是当前文件语法。
- 新增 Public 头文件如果暴露其他模块类型，优先把对应依赖放入 `PublicDependencyModuleNames`；仅实现内部使用的依赖放入 `PrivateDependencyModuleNames`。
- 不要引入新的第三方库、插件依赖或生成步骤，除非用户明确同意并且 README/配置同步更新。

## 高风险区域

- 反射宏、Blueprint API、复制属性、RPC、UObject 生命周期和组件所有权属于高风险修改。
- 修改 Gameplay Tag 驱动流程时，同时检查结构体定义、Tag Query、数据驱动入口和 Blueprint 调用路径。
- 修改 UI 逻辑时，同时检查子系统、Policy、Layout、Widget、输入路由和 CommonUI 激活/出栈行为。
- 修改相机逻辑时，同时检查 Camera Mode、Mode Stack、Blend、第三人称碰撞/穿透规避和控制输入。
- 修改交互、布娃娃、Smart Objects 或 Targeting 时，明确网络权限、Owner-only 行为、服务器/客户端职责和编辑器暴露方式。

## 常用入口

- 特效播放：`Source/GenericEffectsSystem/Public/Feedback/GES_ContextEffectComponent.h`
- 特效数据定义：`Source/GenericEffectsSystem/Public/Feedback/GES_ContextEffectsStructLibrary.h`
- 相机组件：`Source/GenericCameraSystem/Public/GCMS_CameraSystemComponent.h`
- 相机模式栈：`Source/GenericCameraSystem/Public/GCMS_CameraModeStack.h`
- 交互组件：`Source/GenericGameSystem/Public/Interaction/GGS_InteractionSystemComponent.h`
- 布娃娃组件：`Source/GenericGameSystem/Public/Ragdoll/GGS_RagdollComponent.h`
- UI 子系统：`Source/GenericUISystem/Public/UI/GUIS_GameUISubsystem.h`
- UI Policy：`Source/GenericUISystem/Public/UI/GUIS_GameUIPolicy.h`
- 设置注册表：`Source/GenericSettingsSystem/Public/Settings/GSS_GameSettingRegistry.h`
- 设置子系统：`Source/GenericSettingsSystem/Public/Settings/GSS_GameSettingsSubsystem.h`
- 通用设置 Provider：`Source/GenericSettingsSystemCommon/Public/Settings/`（Video、Audio、Input、Accessibility）
- 设置 UI：`Source/GenericSettingsSystemUI/Public/SettingsUI/GSS_GameSettingsMenu.h`

## 构建和验证

- 本仓库没有独立构建脚本，也没有内置自动化测试套件。
- 最可靠的验证方式是在安装了本插件的宿主 Unreal 项目中，用 Unreal Build Tool 构建宿主 Editor Target。
- Windows UE 5.7 路径：`G:\UE_5.7`；macOS UE 5.7 路径：`/Applications/UE_5.7`。
- Windows 宿主工程：`E:\GenericGame\GenericGame.uproject`；macOS 宿主工程：`/Users/kinson/Unreal/GenericGame/GenericGame.uproject`。
- 当前常用宿主构建命令按系统选择：

```powershell
& 'G:\UE_5.7\Engine\Build\BatchFiles\Build.bat' GenericGameEditor Win64 Development 'E:\GenericGame\GenericGame.uproject' -WaitMutex -NoHotReloadFromIDE
```

```bash
/Applications/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh GenericGameEditor Mac Development /Users/kinson/Unreal/GenericGame/GenericGame.uproject -WaitMutex -NoHotReloadFromIDE
```

- 如果新增或重命名反射类型、模块依赖或插件配置，必要时先重新生成项目文件。
- 如果本机没有宿主 UE 工程或 UE 5.7+，仍可完成代码修改，但最终回复必须明确说明未完成运行时验证，以及缺少的验证条件。
- 对玩法、UI、相机、特效行为的修改，除编译外还应说明最小编辑器冒烟测试步骤。

## 测试期望

- 只有在仓库已有相关测试位置，或用户明确要求时，才新增/更新自动化测试。
- 对小范围 C++ 修复，至少进行可用的静态检查、编译或文件级审阅。
- 对运行时行为，最终回复要说明实际执行的命令、结果，以及无法覆盖的风险。

## 文档同步

- 修改架构、Public API、安装方式、模块职责、插件依赖或用户可见工作流时，同步检查 `README.md`、`README_CN.md` 和本文件是否需要更新。
- 不要把 README 写成实现细节日志；README 面向使用者，本文件面向维护者和 Codex。

## Codex 工作方式

- 开始任务时先快速定位相关模块、Public/Private 边界和 `*.Build.cs`，再动手改代码。
- 优先使用 `rg` 搜索符号和调用点。
- 保留用户或其他工具已有修改，不要回滚无关变更。
- 手工编辑文件时使用补丁方式，保持改动可审阅。
- 最终回复用中文概述改了什么、验证了什么、还有什么没有验证。

## 排查 AGENTS.md 加载问题

- 如果 Codex 没读到本文件，检查当前工作目录是否在本仓库内。
- 如果读到了意外规则，检查 Codex Home 或上层目录是否有 `AGENTS.override.md`。
- 如果子目录规则没有生效，确认文件名是否为 `AGENTS.md` 或 `AGENTS.override.md`，以及 Codex 是否从该子目录或其下级目录启动。
- 如果说明过长导致截断，把特定模块规则拆到对应子目录的 `AGENTS.md`，并保留根文件为总览和通用约定。
