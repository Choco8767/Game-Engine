# Game Engine

**A simple Game Engine built with C++ and Vulkan.**

![C++](https://img.shields.io/badge/C++-20-blue.svg)
![Vulkan](https://img.shields.io/badge/Vulkan-1.3-red.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg)

<br>

## Prerequisites

- [ ] [Ninja](https://github.com/ninja-build/ninja/releases)
- [ ] [Vulkan SDK (v1.3+)](https://vulkan.lunarg.com/sdk/home)
- [ ] [CMake (v3.26+)](https://cmake.org/download/)
- [ ] [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)
- [ ] [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) and [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) Extensions for VSCode

<br>

## Installation

**1. Clone the Repository**

Run this command to clone the repository and navigate to it:

```bash
git clone https://github.com/Choco8767/Game-Engine
cd Game-Engine
```

<br>

**2. Install C++ Dependencies**

Set up `vcpkg`:

- Ensure `vcpkg` is installed.
- Create a new System Environment Variable named `VCPKG_ROOT` and set its value to your `vcpkg` installation path (e.g., `C:\dev\vcpkg`).
- Optional - Add your vcpkg folder to your system `PATH` to use the command via Command Line.

<br>

Now configure and install dependencies for the project.

**Note:** If you just set the `VCPKG_ROOT` variable, you must restart VSCode for it to take effect.

<br>

Then, via Command Palette:

- Press `CTRL + Shift + P` to open Command Palette
- Run `CMake: Select Configure Preset` and pick `x64 Release` (or x86, depending on your architecture)
- Run `CMake: Configure`

<br>

Or, via Command Line:

- Open Specifically your `Developer PowerShell for VS` in the `Root Project Directory`
- Run `cmake --list-presets`
- Run `cmake --preset x64-release` (or x86, depending on your architecture)


<br>

**3. Build and Install**

Run these commands in order to build the project for both configurations:

```bash
# Build and Install (Release)
cmake --build out/build/x64-release --target install
```
```bash
# Build and Install (Debug)
cmake --build out/build/x64-debug --target install
```

<br>

## Running

**Run Release Version**

Assuming you are in the root of the project:

```bash
# 64-Bit Architecture
cd out/install/x64-release

# 32-Bit Architecture
# cd out/install/x86-release

./Game-Engine.exe  # OS Dependent
```

<br>

**Run Debug Version**

Assuming you are in the root of the project:

```bash
# 64-Bit Architecture
cd out/install/x64-debug

# 32-Bit Architecture
# cd out/install/x86-debug

./Game-Engine.exe  # OS Dependent
```
