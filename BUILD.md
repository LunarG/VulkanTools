# Build Instructions

1. [Requirements](#requirements)
1. [Building Overview](#building-overview)
1. [Dependencies](#dependencies)
1. [Linux Build](#building-on-linux)
1. [Windows Build](#building-on-windows)
1. [MacOS build](#building-on-macos)
1. [Android Build](#building-for-android)

## Requirements

1. CMake >= 3.17.2
1. C++17 compatible toolchain
1. Git
1. Python >= 3.10
1. Qt 6.5

## Optional software packages:

- *[Qt 5](https://www.qt.io/download)* is required to build *[Vulkan Configurator]*(./vkconfig/vkconfig.md).
  - The Qt `bin` directory requires to be added to the `PATH` environment variable for *Qt* to be detected and Vulkan Configurator built.
  - If `Qt` is not directed, *[Vulkan Configurator]* build will be skipped.

## Building Overview

The following will be enough for most people, for more detailed instructions, see below.

```bash
git clone https://github.com/LunarG/VulkanTools.git
cd VulkanTools

cmake -S . -B build -D UPDATE_DEPS=ON -D BUILD_WERROR=ON -D BUILD_TESTS=ON -D CMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### Warnings as errors off by default!

By default `BUILD_WERROR` is `OFF`. The idiom for open source projects is to NOT enable warnings as errors.

System/language package managers have to build on multiple different platforms and compilers.

By defaulting to `ON` we cause issues for package managers since there is no standard way to disable warnings until CMake 3.24

Add `-D BUILD_WERROR=ON` to your workflow.

## Dependencies

Currently this repo has a custom process for grabbing C/C++ dependencies.

Keep in mind this repo predates tools like `vcpkg`, `conan`, etc. Our process is most similar to `vcpkg`.

By specifying `-D UPDATE_DEPS=ON` when configuring CMake we grab dependencies listed in [known_good.json](scripts/known_good.json).

All we are doing is streamlining `building`/`installing` the `known good` dependencies and helping CMake `find` the dependencies.

This is done via a combination of `Python` and `CMake` scripting.

Misc Useful Information:

- By default `UPDATE_DEPS` is `OFF`. The intent is to be friendly by default to system/language package managers.
- You can run `update_deps.py` manually but it isn't recommended for most users.

### How to test new dependency versions

Typically most developers alter `known_good.json` with the commit/branch they are testing.

Alternatively you can modify `CMAKE_PREFIX_PATH` as follows.

```sh
# Delete the CMakeCache.txt which will cache find_* results
rm build -rf/
cmake -S . -B build/ ... -D CMAKE_PREFIX_PATH=~/foobar/my_custom_glslang_install/ ...
```

## Building On Linux

### Linux Build Requirements

This repository is regularly built and tested on the two most recent Ubuntu LTS versions.

```bash
sudo apt-get install git build-essential python3 cmake

# Linux WSI system libraries
sudo apt-get install libwayland-dev xorg-dev

# Qt5 for vkconfig
sudo apt-get install qt6-default
```

## Building On Windows

### Windows Development Environment Requirements

- Windows 10+
- Visual Studio

### Visual Studio Generator

Run CMake to generate [Visual Studio project files](https://cmake.org/cmake/help/latest/guide/user-interaction/index.html#command-line-g-option).

```bash
# NOTE: By default CMake picks the latest version of Visual Studio as the default generator.
cmake -S . -B build

# Open the Visual Studio solution
cmake --open build
```

See the [CMake documentation](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators) for further information on Visual Studio generators.

NOTE: Windows developers don't have to develop in Visual Studio. Visual Studio just helps streamlining the needed C++ toolchain requirements (compilers, linker, etc).

## Building on MacOS

### MacOS Development Environment Requirements

- Xcode

NOTE: MacOS developers don't have to develop in Xcode. Xcode just helps streamlining the needed C++ toolchain requirements (compilers, linker, etc). Similar to Visual Studio on Windows.

### Xcode Generator

To create and open an Xcode project:

```bash
# Create the Xcode project
cmake -S . -B build -G Xcode

# Open the Xcode project
cmake --open build
```

See the [CMake documentation](https://cmake.org/cmake/help/latest/generator/Xcode.html) for further information on the Xcode generator.

## Building For Android

- CMake 3.21+
- NDK r25+
- Ninja 1.10+

### Android Build Requirements

- Download [Android Studio](https://developer.android.com/studio)
- Install (https://developer.android.com/studio/install)
- From the `Welcome to Android Studio` splash screen, add the following components using the SDK Manager:
  - SDK Platforms > Android 8.0 and newer (API Level 26 or higher)
  - SDK Tools > Android SDK Build-Tools
  - SDK Tools > Android SDK Platform-Tools
  - SDK Tools > Android SDK Tools
  - SDK Tools > NDK
  - SDK Tools > CMake

#### Add Android specifics to environment

NOTE: The following commands are streamlined for Linux but easily transferable to other platforms.
The main intent is setting 1 environment variable and ensuring the NDK and build tools are in the `PATH`.

```sh
# Set environment variable
# https://github.com/actions/runner-images/blob/main/images/linux/Ubuntu2204-Readme.md#environment-variables-2
export ANDROID_NDK_HOME=$ANDROID_SDK_ROOT/ndk/X.Y.Z

# (Optional if you have new enough version of CMake + Ninja)
export PATH=$ANDROID_SDK_ROOT/cmake/3.22.1/bin:$PATH

# Verify CMake/Ninja are in the path
which cmake
which ninja
```

### Android Build

1. Building libraries to package with your APK

Invoking CMake directly to build the binary is relatively simple.

See https://developer.android.com/ndk/guides/cmake#command-line for CMake NDK documentation.

```sh
# Build release binary for arm64-v8a
cmake -S . -B build \
  -D CMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
  -D ANDROID_PLATFORM=26 \
  -D CMAKE_ANDROID_ARCH_ABI=arm64-v8a \
  -D CMAKE_ANDROID_STL_TYPE=c++_static \
  -D ANDROID_USE_LEGACY_TOOLCHAIN_FILE=NO \
  -D CMAKE_BUILD_TYPE=Release \
  -D UPDATE_DEPS=ON \
  -G Ninja

cmake --build build

cmake --install build --prefix build/install
```

Then you just package the library into your APK under the appropriate lib directory based on the ABI:
https://en.wikipedia.org/wiki/Apk_(file_format)#Package_contents

Alternatively users can also use `scripts/android.py` to build the binaries.

Note: `scripts/android.py` will place the binaries in the `build-android/libs` directory.

```sh
# Build release binary for arm64-v8a
python3 scripts/android.py --config Release --app-abi arm64-v8a
```

`android.py` can also streamline building for multiple ABIs:

```sh
# Build release binaries for all ABIs
python3 scripts/android.py --config Release --app-abi 'armeabi-v7a arm64-v8a x86 x86_64'
```

Now you can upload the layer on to your Android device.

See Android developer documentation for more information on loading Vulkan layers:
https://developer.android.com/ndk/guides/graphics/validation-layer#load-layers

### Software Installation

After you have built your project you can install using CMake's install functionality.

CMake Docs:
- [Software Installation Guide](https://cmake.org/cmake/help/latest/guide/user-interaction/index.html#software-installation)
- [CLI for installing a project](https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project)

```sh
# EX: Installs Release artifacts into `build/install` directory.
# NOTE: --config is only needed for multi-config generators (Visual Studio, Xcode, etc)
cmake --install build/ --config Release --prefix build/install
```
