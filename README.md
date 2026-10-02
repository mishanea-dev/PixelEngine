# PixelEngine

PixelEngine is a C++ game-engine playground built on [raylib](https://www.raylib.com/) and [Dear ImGui](https://github.com/ocornut/imgui). It currently contains a small executable that opens a raylib window and an ImGui editor panel.

## Quick start

Requirements:

- CMake 3.24 or newer
- A C++17 compiler (MinGW-w64, Clang, or Visual Studio)
- Git, because CMake downloads the pinned dependencies on the first configure

From the repository root, configure and build with Visual Studio 2022:

```powershell
cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64
cmake --build build-vs --config Debug
.\build-vs\bin\Debug\pixel_engine.exe
```

If you prefer MinGW-w64, use:

```powershell
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_C_COMPILER=C:\msys64\ucrt64\bin\gcc.exe `
  -DCMAKE_CXX_COMPILER=C:\msys64\ucrt64\bin\g++.exe
cmake --build build-mingw
.\build-mingw\bin\pixel_engine.exe
```
