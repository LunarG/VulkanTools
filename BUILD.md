# Build Instructions
This document contains the instructions for building this repository on Linux and Windows.

This repository contains Vulkan development tools, such as additional layers and VkTrace trace/replay utilities,
supplementing the loader and validation layer core components found at https://github.com/KhronosGroup.

## Get Additional Components

The public repository for the the LunarG VulkanTools is hosted at https://github.com/LunarG.

If you intend to contribute, the preferred work flow is to fork the repo,
create a branch in your forked repo, do the work,
and create a pull request on GitHub to integrate that work back into the repo.

## Linux System Requirements
These additional packages are needed for building the components in this repo.

### Ubuntu

Ubuntu 16.04 LTS and 18.04 have been tested with this repo.

Continuous integration tools use [CMake 3.12.4](https://github.com/Kitware/CMake/releases/tag/v3.12.4) for Linux

```
# Dependencies from included submodule components
sudo apt-get install git cmake build-essential bison libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev libxcb-randr0-dev
# Additional dependencies for this repo:
sudo apt-get install wget autotools-dev libxcb-keysyms1 libxcb-keysyms1-dev libxcb-ewmh-dev
# If performing 32-bit builds, you will also need:
sudo apt-get install libc6-dev-i386 g++-multilib
```

On Ubuntu 18.04 LTS or newer, you may build VkConfig only if you also install several
additional Qt dependencies:

```
# Qt Dependencies for building VkConfig (not required if you don't want VkConfig)
sudo apt-get install qt5-default qtwebengine5-dev
```

### Fedora Core

Fedora Core 28 and 29 were tested with this repo.

Continuous integration tools use [CMake 3.12.4](https://github.com/Kitware/CMake/releases/tag/v3.12.4) for Linux

Additional package dependencies include:

```
# Dependencies from included submodule components
sudo dnf install git cmake @development-tools glm-devel \
                 libpng-devel wayland-devel libpciaccess-devel \
                 libX11-devel libXpresent libxcb xcb-util libxcb-devel libXrandr-devel \
                 xcb-util-keysyms-devel xcb-util-wm-devel
# Qt Dependencies for building VkConfig (not required if you don't want VkConfig)
sudo dnf install qt qt5-qtwebengine-devel
```

## Download the repository

To create your local git repository of VulkanTools:
```
cd YOUR_DEV_DIRECTORY

# Clone the VulkanTools repo
git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git

# Enter the folder containing the cloned source
cd VulkanTools

# This will perform some initialization and ensure subcomponents are built:
./update_external_sources.sh    # linux
./update_external_sources.bat   # windows
```

## Updating the Repository After a Pull

The VulkanTools repository contains a submodule named jsoncpp. You may occasionally have to update the source in that submodules.
You will know this needs to be performed when you perform a pull, and you check the status of your tree with `git status` and something similar to the following shows:

```
(master *)] $ git status
On branch master
Your branch is up-to-date with 'origin/master'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   submodules/jsoncpp (new commits)

no changes added to commit (use "git add" and/or "git commit -a")
```

To resolve this, simply update the sub-module using:

```
git submodule update --recursive
```

Then, update the external sources as before:

```
# This will perform required subcomponent operations.
./update_external_sources.sh    # linux
./update_external_sources.bat   # windows
```

Now, you should be able to continue building as normal.


## Repository Dependencies
This repository attempts to resolve some of its dependencies by using
components found from the following places, in this order:

1. CMake or Environment variable overrides (e.g., -DVULKAN_HEADERS_INSTALL_DIR)
1. LunarG Vulkan SDK, located by the `VULKAN_SDK` environment variable
1. System-installed packages, mostly applicable on Linux

Dependencies that cannot be resolved by the SDK or installed packages must be
resolved with the "install directory" override and are listed below. The
"install directory" override can also be used to force the use of a specific
version of that dependency.

### Vulkan-Headers

This repository has a required dependency on the
[Vulkan Headers repository](https://github.com/KhronosGroup/Vulkan-Headers).
You must clone the headers repository and build its `install` target before
building this repository. The Vulkan-Headers repository is required because it
contains the Vulkan API definition files (registry) that are required to build
the validation layers. You must also take note of the headers' install
directory and pass it on the CMake command line for building this repository,
as described below.

### Vulkan-Loader

The tools in this repository depend on the Vulkan loader.

A loader can be used from an installed LunarG SDK, an installed Linux package,
or from a driver installation on Windows.

If a loader is not available from any of these methods and/or it is important
to use a loader built from a repository, then you must build the
[Vulkan-Loader repository](https://github.com/KhronosGroup/Vulkan-Loader.git)
with its install target. Take note of its install directory location and pass
it on the CMake command line for building this repository, as described below.

### Vulkan-ValidationLayers
The tools in this repository depend on the Vulkan validation layers.

Validation layers can be used from an installed LunarG SDK, an installed Linux
package, or from a driver installation on Windows.

If the validation layers are not available from any of these methods and/or
it is important to use the validation layers built from a repository, then you
must build the
[Vulkan-ValidationLayers repository](https://github.com/KhronosGroup/Vulkan-ValidationLayers.git)
with its install target. Take note of its install directory location and pass
it on the CMake command line for building this repository, as described below.

### Vulkan-Tools

The tests in this repository depend on the Vulkan-Tools repository, which is
hosted under Khronos' GitHub account and differentiated in name by a hyphen.
The tests use Vulkan Info and the mock ICD from Vulkan-Tools.

You may build the
[Vulkan-Tools repository](https://github.com/KhronosGroup/Vulkan-Tools.git)
with its install target. Take note of its build directory location and set
the VULKAN\_TOOLS\_BUILD\_DIR environment variable to the appropriate path.

If you do not intend to run the tests, you do not need Vulkan-Tools.

### Build and Install Directories

A common convention is to place the build directory in the top directory of
the repository with a name of `build` and place the install directory as a
child of the build directory with the name `install`. The remainder of these
instructions follow this convention, although you can use any name for these
directories and place them in any location.

### Building Dependent Repositories with Known-Good Revisions

There is a Python utility script, `scripts/update_deps.py`, that you can use
to gather and build the dependent repositories mentioned above. This program
also uses information stored in the `scripts/known-good.json` file to checkout
dependent repository revisions that are known to be compatible with the
revision of this repository that you currently have checked out.

Here is a usage example for this repository:

    git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git
    cd VulkanTools
    mkdir build
    cd build
    ../scripts/update_deps.py
    cmake -C helper.cmake ..
    cmake --build .


#### Notes

- You may need to adjust some of the CMake options based on your platform. See
  the platform-specific sections later in this document.
- The `update_deps.py` script fetches and builds the dependent repositories in
  the current directory when it is invoked. In this case, they are built in
  the `build` directory.
- The `build` directory is also being used to build this
  (Vulkan-Tools) repository. But there shouldn't be any conflicts
  inside the `build` directory between the dependent repositories and the
  build files for this repository.
- The `--dir` option for `update_deps.py` can be used to relocate the
  dependent repositories to another arbitrary directory using an absolute or
  relative path.
- The `update_deps.py` script generates a file named `helper.cmake` and places
  it in the same directory as the dependent repositories (`build` in this
  case). This file contains CMake commands to set the CMake `*_INSTALL_DIR`
  variables that are used to point to the install artifacts of the dependent
  repositories. You can use this file with the `cmake -C` option to set these
  variables when you generate your build files with CMake. This lets you avoid
  entering several `*_INSTALL_DIR` variable settings on the CMake command line.
- If using "MINGW" (Git For Windows), you may wish to run
  `winpty update_deps.py` in order to avoid buffering all of the script's
  "print" output until the end and to retain the ability to interrupt script
  execution.
- Please use `update_deps.py --help` to list additional options and read the
  internal documentation in `update_deps.py` for further information.


## VkConfig

VkConfig has additional requirements beyond the rest of the source in this
repository.
Because of that, if one or more of those dependencies is not properly installed,
you may get a warning during CMake generation like the following:

```
WARNING: vkconfig will be excluded because Qt5 was not found.
-- Configuring done
-- Generating done
```

This is usually caused by missing Qt dependencies.
Make sure the following items are installed for proper building of VkConfig:

* Qt5
* Qt5 Web Engine Widgets

Please note that not building VkConfig is purely fine as well and will not
impact the generation of any other targets.


## Linux Build

This build process builds all items in the VulkanTools repository

Example debug build:
```
cd VulkanTools  # cd to the root of the VulkanTools git repository
cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=build/install -DVULKAN_HEADERS_INSTALL_DIR=absolute_path_to_install_directory -DVULKAN_LOADER_INSTALL_DIR=absolute_path_to_install_directory -DVULKAN_VALIDATIONLAYERS_INSTALL_DIR=absolute_path_to_install_directory
cd dbuild
make -j8
```

## Windows System Requirements

Windows 7+ with additional required software packages:

- Microsoft Visual Studio 2015 Professional or 2017 Professional.  Note: it is possible that lesser/older versions may work, but not guaranteed.
- CMake: Continuous integration tools use [CMake 3.12.2](https://github.com/Kitware/CMake/releases/tag/v3.12.2) for Windows
  - In order to build the VkTrace tools, you need at least version 3.0.
  - Tell the installer to "Add CMake to the system PATH" environment variable.
- Python 3 (from https://www.python.org/downloads).  Notes:
  - Select to install the optional sub-package to add Python to the system PATH environment variable.
  - Need python3.3 or later to get the Windows py.exe launcher that is used to get python3 rather than python2 if both are installed on Windows
- Git (from http://git-scm.com/download/win).
  - Tell the installer to allow it to be used for "Developer Prompt" as well as "Git Bash".
  - Tell the installer to treat line endings "as is" (i.e. both DOS and Unix-style line endings).

Optional software packages:

- Cygwin (from https://www.cygwin.com/).  Notes:
  - Cygwin provides some Linux-like tools, which are valuable for obtaining the source code, and running CMake.
    Especially valuable are the BASH shell and git packages.
  - If you do not wish to use Cygwin, there are other shells and environments that can be used.
    You can also use a Git package that does not come from Cygwin.

## Windows Build

Cygwin is used in order to obtain a local copy of the Git repository, and to run the CMake command that creates Visual Studio files.
Visual Studio is used to build the software, and will re-run CMake as appropriate.

To build all Windows targets (e.g. in a "Developer Command Prompt for VS2015" window):
```
cd VulkanTools  # cd to the root of the VulkanTools git repository
cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=build/install -DVULKAN_HEADERS_INSTALL_DIR=absolute_path_to_install_directory -DVULKAN_LOADER_INSTALL_DIR=absolute_path_to_install_directory -DVULKAN_VALIDATIONLAYERS_INSTALL_DIR=absolute_path_to_install_directory
cmake --build dbuild --config Debug --target install
```

At this point, you can use Windows Explorer to launch Visual Studio by double-clicking on the "VULKAN.sln" file in the \build folder.
Once Visual Studio comes up, you can select "Debug" or "Release" from a drop-down list.
You can start a build with either the menu (Build->Build Solution), or a keyboard shortcut (Ctrl+Shift+B).
As part of the build process, Python scripts will create additional Visual Studio files and projects,
along with additional source files.
All of these auto-generated files are under the "build" folder.

Vulkan programs must be able to find and use the Vulkan-1.dll library.
Make sure it is either installed in the C:\Windows\System32 folder,
or the PATH environment variable includes the folder that it is located in.

### Windows 64-bit Installation Notes
If you plan on creating a Windows Install file (done in the windowsRuntimeInstaller sub-directory) you will need to build for both 32-bit
and 64-bit Windows since both versions of EXEs and DLLs exist simultaneously on Windows 64.

To do this, simply create and build the release versions of each target:
```
cd VulkanTools  # cd to the root of the Vulkan git repository
mkdir build
cd build
cmake -G "Visual Studio 14 Win64" ..
msbuild ALL_BUILD.vcxproj /p:Platform=x64 /p:Configuration=Release
mkdir build32
cd build32
cmake -G "Visual Studio 14" ..
msbuild ALL_BUILD.vcxproj /p:Platform=x86 /p:Configuration=Release
```

## Android Build
Install the required tools for Linux and Windows covered above, then add the
following.

### Android Studio
- Install 2.1 or later version of [Android Studio](http://tools.android.com/download/studio/stable)
- From the "Welcome to Android Studio" splash screen, add the following components using Configure > SDK Manager:
  - SDK Tools > Android NDK

#### Add NDK to path

On Linux:
```
export PATH=$HOME/Android/Sdk/ndk-bundle:$PATH
```
On Windows:
```
set PATH=%LOCALAPPDATA%\Android\sdk\ndk-bundle;%PATH%
```
On OSX:
```
export PATH=$HOME/Library/Android/sdk/ndk-bundle:$PATH
```
### Additional OSX System Requirements
Tested on OSX version 10.11.4

Continuous integration tools use [CMake 3.11.3](https://github.com/Kitware/CMake/releases/tag/v3.11.3) for MacOS

 Setup Homebrew and components
- Follow instructions on [brew.sh](http://brew.sh) to get homebrew installed.
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
- Ensure Homebrew is at the beginning of your PATH:
```
export PATH=/usr/local/bin:$PATH
```
- Add packages with the following (may need refinement)
```
brew install cmake python python3 git
```

### Build steps for Android
Use the following to ensure the Android build works.
#### Linux
Follow the setup steps for Linux, then from your terminal:
```
cd build-android
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(nproc)
```
#### OSX
Follow the setup steps for OSX above, then from your terminal:
```
cd build-android
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(sysctl -n hw.ncpu)
```
#### Windows
Follow the setup steps for Windows above, then from Developer Command Prompt for VS2015:
```
cd build-android
update_external_sources_android.bat
android-generate.bat
ndk-build
```

## Android usage
This documentation is preliminary and needs to be beefed up.

See the [vktracereplay.sh](https://github.com/LunarG/VulkanTools/blob/master/build-android/vktracereplay.sh) file for a working example of how to use vktrace/vkreplay and screenshot layers.

Two additional scripts have been added to facilitate tracing and replaying any APK.  Note that these two scripts do not install anything for you, so make sure your target APK, vktrace, vktrace_layer, and vkreplay all use the same ABI.
```
./create_trace.sh --serial 0123456789 --abi armeabi-v7a --package com.example.CubeWithLayers  --activity android.app.NativeActivity
adb install --abi armeabi-v7a
./replay_trace.sh --serial 0123456789 --tracefile com.example.CubeWithLayers0.vktrace
```
An example of using the scripts on Linux and macOS:
```
./build_vktracereplay.sh
./vktracereplay.sh \
 --serial 12345678 \
 --abi armeabi-v7a \
 --apk ../demos/android/cube-with-layers/bin/cube-with-layers.apk \
 --package com.example.CubeWithLayers \
 --frame 50
```
And on Windows:
```
build_vktracereplay.bat ^
vktracereplay.bat ^
 --serial 12345678 ^
 --abi armeabi-v7a ^
 --apk ..\demos\android\cube-with-layers\bin\NativeActivity-debug.apk ^
 --package com.example.CubeWithLayers ^
 --frame 50
```
### vkjson_info
Currently vkjson_info is only available as an executable for devices with root access.

To use, simply push it to the device and run it.  The resulting json file will be found in:
```
/sdcard/Android/<output>.json
```
A working example can be found in [devsim_layer_test_anroid.sh](https://github.com/LunarG/VulkanTools/blob/master/build-android/devsim_layer_test_android.sh)
