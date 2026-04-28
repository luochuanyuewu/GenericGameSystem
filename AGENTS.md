# AGENTS.md

## Project Overview

- `GenericGameSystem` is a code-only Unreal Engine 5.7+ plugin repository.
- The plugin is split into four runtime modules under `Source/`:
  - `GenericEffectsSystem`: context-driven VFX/SFX playback using Gameplay Tags and Niagara.
  - `GenericCameraSystem`: camera mode stacking, blending, and third-person camera behaviors.
  - `GenericUISystem`: CommonUI-based UI framework, layout policy, extension points, and input routing.
  - `GenericGameSystem`: shared gameplay utilities, interaction, ragdoll, Smart Objects, and integration glue.
- The repository does not include a playable game project or plugin content. `GenericGameSystem.uplugin` sets `CanContainContent` to `false`.

## Repository Layout

- `Source/<Module>/Public`: exported headers and Blueprint-facing APIs.
- `Source/<Module>/Private`: implementation files. Keep module internals here.
- `Config/`: plugin config files packaged with the plugin.
- `Resources/`: editor-facing plugin assets such as the icon.
- `README.md` and `README_CN.md`: human-facing overview and installation notes.

## Working Agreements

- Preserve module boundaries. Do not include another module's `Private` headers.
- Prefer extending the existing module that owns the behavior instead of adding cross-cutting utility code to unrelated modules.
- Keep UE naming conventions consistent with the repository:
  - `GGS_` for core gameplay system types
  - `GES_` for effects system types
  - `GCMS_` for camera system types
  - `GUIS_` for UI system types
- When editing public APIs, keep Blueprint exposure, replication behavior, and metadata aligned with the surrounding code.
- Many public headers use bilingual comments. Match the local style when touching existing documented APIs, but do not add redundant comments.
- This plugin is code-first. Do not add new content folders or asset dependencies unless the task explicitly requires them.

## Build And Validation

- There is no standalone build script or automated test suite in this repository.
- To validate code changes, build the plugin through a host Unreal project that has this plugin installed and UE 5.7+ available.
- Typical validation paths:
  - Regenerate project files if new reflected types or module dependencies are added.
  - Build the host editor target with Unreal Build Tool.
  - Open the host project and smoke-test the affected feature in editor when behavior changes touch gameplay, UI, camera, or effects.
- If no host project is available in the current workspace, make the code change, explain what was not validated, and avoid claiming runtime verification.

## Editing Guidance

- Prefer minimal, surgical changes that fit the existing architecture.
- For new gameplay features, start from the owning component or subsystem rather than adding global helpers.
- Keep `Public` and `Private` file placement aligned: exported declarations in `Public`, implementation in `Private`.
- When adding replicated state or RPCs, update both declaration and implementation and verify owner-only versus full replication behavior intentionally.
- When changing Gameplay Tag driven flows, inspect the associated structs, tag queries, and Blueprint entry points so behavior stays data-driven.
- When changing UI code, check both subsystem/policy flow and the widget layer using it.

## Testing Expectations

- Add or update automated coverage only when the repository already has a relevant test location or the user explicitly asks for it.
- At minimum, perform the strongest available validation for the touched area and report the exact command or limitation in the final response.
- For runtime features that cannot be exercised here, describe the smallest editor smoke test a teammate should run.

## Common Starting Points

- Effects playback: `Source/GenericEffectsSystem/Public/Feedback/GES_ContextEffectComponent.h`
- Effects data definitions: `Source/GenericEffectsSystem/Public/Feedback/GES_ContextEffectsStructLibrary.h`
- Camera entry point: `Source/GenericCameraSystem/Public/GCMS_CameraSystemComponent.h`
- Camera mode stack: `Source/GenericCameraSystem/Public/GCMS_CameraModeStack.h`
- Interaction system: `Source/GenericGameSystem/Public/Interaction/GGS_InteractionSystemComponent.h`
- Ragdoll system: `Source/GenericGameSystem/Public/Ragdoll/GGS_RagdollComponent.h`
- UI subsystem: `Source/GenericUISystem/Public/UI/GUIS_GameUISubsystem.h`
- UI policy: `Source/GenericUISystem/Public/UI/GUIS_GameUIPolicy.h`

## Safety And Review Notes

- Treat reflection macros, replication, UObject ownership, and Blueprint API changes as high-risk edits.
- Be careful when changing module dependencies in `*.Build.cs`; Unreal compile failures often surface from missing module declarations rather than code syntax alone.
- Avoid stale documentation: if you update architecture, public API names, or workflow assumptions, update this file and the relevant README when needed.

## Final Response Expectations

- Summarize what changed in plain language.
- Call out validation actually performed.
- If validation was not possible, say so explicitly and name the missing dependency, usually a host UE project or local engine build.
