# GenericGameSystem Plugin

此文件为 Claude Code (claude.ai/code) 在 GenericGameSystem 插件中工作时提供指导。

## 🎯 快速概览

**GenericGameSystem (GGS)** 是虚幻引擎 5.7+ 的综合性多模块插件，提供基础游戏系统功能，包括上下文特效系统、高级相机管理、UI 扩展和交互框架。

**版本**: 1.6 | **作者**: YuewuDev | **文档**: https://www.yuewu.dev/en/wiki | **支持**: https://discord.com/invite/xMRXAB2

---

## ⚡ 核心概念（必读）

### 1. 上下文特效系统 (GenericEffectsSystem)
- **GameplayTag 驱动**: 基于 GameplayTag 的 VFX/SFX 播放
- **Niagara 集成**: 高级粒子效果支持
- **高效池化**: 特效池化管理，性能优化
- **GAS 集成**: 与 GameplayAbilities 无缝集成

### 2. 相机管理系统 (GenericCameraSystem)
- **多模式支持**: 支持多种相机模式和流畅过渡
- **第三人称/第一人称**: 内置常用视角实现
- **目标锁定**: 目标跟踪和锁定系统
- **Enhanced Input**: 输入系统集成

### 3. UI 扩展系统 (GenericUISystem)
- **CommonUI 扩展**: 增强的 UI 组件和控件
- **输入路由**: 智能输入处理和动作路由
- **数据驱动**: 数据表驱动的 UI 系统
- **本地化支持**: 内置本地化功能

### 4. 交互系统 (GenericGameSystem)
- **交互框架**: 游戏对象的交互管理
- **通用工具**: 跨模块集成层和辅助函数
- **模块化**: 独立模块设计，按需使用

---

## 🚀 快速开始

### 播放上下文特效
获取 UGES_ContextEffectComponent，创建 FGES_ContextEffectInput 配置标签和位置，调用 PlayContextEffectsWithInput()

### 设置相机模式
获取 UGCMS_CameraSystemComponent，使用 PushCameraMode() 推送新相机模式，使用 PopCameraMode() 弹出当前模式

### 设置交互组件
获取 UGGS_InteractionComponent，配置交互范围和标签，绑定 OnInteract 委托处理交互。详细实现见插件文档或相关技能

---

## 📦 模块架构

### 模块组织
```
GenericGameSystem/
├── GenericEffectsSystem (运行时)
│   ├── Feedback/             # 特效反馈系统
│   ├── Context/              # 上下文特效处理
│   └── Niagara/              # Niagara 集成
│
├── GenericCameraSystem (运行时)
│   ├── CameraModes/          # 相机模式实现
│   ├── Components/           # 相机系统组件
│   └── Targeting/            # 目标锁定系统
│
├── GenericUISystem (运行时)
│   ├── Widgets/              # UI 控件扩展
│   ├── InputRouting/         # 输入路由
│   └── Localization/         # 本地化支持
│
└── GenericGameSystem (运行时)
    ├── Interaction/          # 交互框架
    ├── Utilities/            # 通用工具
    └── Integration/          # 跨模块集成
```

### 依赖关系
**必需插件**:
- `Niagara` - 粒子特效系统
- `CommonUI` - UI 框架
- `EnhancedInput` - 输入系统
- `ModularGameplay` - 模块化架构
- `TargetingSystem` - 目标选择系统
- `SmartObjects` - 智能对象系统
- `GameplayBehaviors` - AI 行为系统

---

## 🔑 关键文件路径（快速查找）

### 特效系统
- `GenericEffectsSystem/Public/Feedback/GES_ContextEffectComponent.h` - 上下文特效组件
- `GenericEffectsSystem/Public/Context/GES_ContextEffectDefinition.h` - 特效定义

### 相机系统
- `GenericCameraSystem/Public/GCMS_CameraSystemComponent.h` - 相机系统组件
- `GenericCameraSystem/Public/CameraModes/GCMS_CameraModeBase.h` - 相机模式基类
- `GenericCameraSystem/Public/Targeting/GCMS_TargetingComponent.h` - 目标锁定组件

### UI 系统
- `GenericUISystem/Public/Widgets/GGS_ExtensionWidget.h` - UI 扩展控件
- `GenericUISystem/Public/InputRouting/GGS_InputRouterComponent.h` - 输入路由组件

### 交互系统
- `GenericGameSystem/Public/Interaction/GGS_InteractionComponent.h` - 交互组件
- `GenericGameSystem/Public/Interaction/GGS_InteractableInterface.h` - 交互接口

---

## ⚠️ 关键注意事项

### 特效系统
- ✅ **标签配置**: 正确配置 GameplayTag 以触发特效
- ✅ **池管理**: 使用对象池避免频繁创建/销毁
- ✅ **性能**: 控制同屏特效数量

### 相机系统
- ✅ **模式切换**: 使用 Push/Pop 管理相机模式栈
- ✅ **目标验证**: 目标锁定前验证目标有效性
- ✅ **输入处理**: 正确配置相机输入优先级

### UI 系统
- ✅ **输入路由**: 确保输入路由正确配置
- ✅ **本地化**: 使用数据表进行本地化
- ✅ **性能**: 避免频繁的 UI 更新

### 交互系统
- ✅ **范围检查**: 合理设置交互范围
- ✅ **标签过滤**: 使用标签过滤可交互对象
- ✅ **回调处理**: 正确处理交互回调

---

## 🌐 网络复制

主要组件的网络复制支持：

| 组件 | 复制支持 | 说明 |
|------|---------|------|
| `UGES_ContextEffectComponent` | ✅ | 特效播放同步 |
| `UGCMS_CameraSystemComponent` | ✅ | 相机模式同步 |
| `UGGS_InteractionComponent` | ✅ | 交互状态同步 |

---

## 🐛 常见问题快速诊断

### 特效不播放？
1. 检查 GameplayTag 配置是否正确
2. 验证特效资产是否存在
3. 确认组件已正确初始化
4. 查看游戏调试器中的特效系统类别

### 相机不切换？
1. 验证相机模式类是否正确
2. 检查模式栈状态
3. 确认相机组件激活状态
4. 查看相机系统调试信息

### 交互不响应？
1. 检查交互范围设置
2. 验证交互标签配置
3. 确认回调正确绑定
4. 检查目标对象是否实现了交互接口

---

## 📖 相关资源

- **官方文档**: https://www.yuewu.dev/wiki/a1d2yzik8sZuC3aj3Il3X
- **社区支持**: https://discord.com/invite/xMRXAB2
- **Fab 市场**: https://www.fab.com/listings/98b2c4a0-9520-4d6b-8bc2-86d5c82612ca
- **演示地图**: `Content/GenericDemo/` (系统相关演示)

---

## 相关技能

- `/ue-niagara` - UE5 Niagara 特效系统
- `/ue-camera-system` - UE5 相机系统
- `/ue-commonui` - CommonUI 框架
- `/update-memory` - 更新插件记忆

---

## 何时更新记忆

运行 `/update-memory` 技能当：
- 发现新的架构模式或最佳实践
- 修正 Claude 对代码的误解
- API 发生重大变更
- 新功能添加或模块更新