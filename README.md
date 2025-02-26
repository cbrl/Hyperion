# Hyperion
A 3D game engine featuring physically-based lighting with forward and deferring rendering pipelines,
and an entity-component system.

## Compiler Requirements
This project requires a version of MSVC with C++20 support. Support for modules in MSVC is a work
in progress, and this project still causes encounters compiler errors as of MSVC 19.44 (Visual
Studio 17.14).

## Required vcpkg Packages
- assimp
- imgui
- imguizmo
- directxtk
- directxtex
- nlohmann-json
- spdlog
- boost-regex
