# Build Instructions
This document contains the instructions for building this repository on Linux and Windows.

This repository contains *Vulkan* development tools and layers,
supplementing the loader and validation layer core components found at https://github.com/KhronosGroup.

## System Requirements

### Windows System Requirements

Windows 7+ with additional required software packages:

- Microsoft Visual Studio 2017 or 2019: it is possible that lesser/older versions may work, but not guaranteed.
- [CMake 3.10.2](https://cmake.org/files/v3.10/cmake-3.10.2-win64-x64.zip) is recommended.
  - Tell the installer to "Add CMake to the system `PATH`" environment variable.
- Python 3 (from https://www.python.org/downloads).  Notes:
  - Select to install the optional sub-package to add Python to the system `PATH` environment variable.
  - Need python3.3 or later to get the Windows `py.exe` launcher that is used to get `python3` rather than `python2` if both are installed on Windows
- Git (from http://git-scm.com/download/win).
  - Tell the installer to allow it to be used for "Developer Prompt" as well as "Git Bash".
  - Tell the installer to treat line endings "as is" (i.e. both DOS and Unix-style line endings).

Optional software packages:

- *[Qt 5](https://www.qt.io/download)* is required to build *[Vulkan Configurator]*(./vkconfig/vkconfig.md).
  - The Qt `bin` directory requires to be added to the `PATH` environment variable for *Qt* to be detected and Vulkan Configurator built.
  - If `Qt` is not directed, *[Vulkan Configurator]* build will be skipped.
  - Vulkan Configurator requires at least Qt 5.5 with the binaries corresponding to the Visual Studio toolset.
- *Cygwin* (from https://www.cygwin.com/).
  - *Cygwin* provides some Linux-like tools, which are valuable for obtaining the source code, and running CMake.
    Especially valuable are the *BASH* shell and git packages.
  - If you do not wish to use *Cygwin*, there are other shells and environments that can be used.
    You can also use a Git package that does not come from *Cygwin*.

### Ubuntu System Requirements

Ubuntu 18.04 LTS and 20.04 have been tested with this repo.

[CMake 3.10.2](https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.tar.gz) is recommended.

```
# Dependencies from included submodule components
sudo apt-get install git build-essential bison libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev libxcb-randr0-dev

# Additional dependencies for this repo:
sudo apt-get install wget autotools-dev libxcb-keysyms1 libxcb-keysyms1-dev libxcb-ewmh-dev

# If performing 32-bit builds, you will also need:
sudo apt-get install libc6-dev-i386 g++-multilib
```

Optional software packages:

On Ubuntu 18.04 LTS or newer, you may build [Vulkan Configurator](./vkconfig/README.md) only if you also install several
additional Qt dependencies:

```
sudo apt-get install qt5-default
```

### Fedora Core System Requirements

Fedora Core 28 and 29 were tested with this repo.

[CMake 3.10.2](https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.tar.gz) is recommended.

Additional package dependencies include:

```
# Dependencies from included submodule components
sudo dnf install git @development-tools glm-devel \
                 libpng-devel wayland-devel libpciaccess-devel \
                 libX11-devel libXpresent libxcb xcb-util libxcb-devel libXrandr-devel \
                 xcb-util-keysyms-devel xcb-util-wm-devel
```

Optional software packages:

You may build [Vulkan Configurator](./vkconfig/README.md) only if you also install several additional Qt dependencies:
```
sudo dnf install qt
```

### macOS System Requirements

macOS 10.11.4 have been tested with this repo.

- [CMake 3.10.2](https://cmake.org/files/v3.10/cmake-3.10.2-Darwin-x86_64.tar.gz) is recommended.

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
brew install python python3 git
```

### Android Additional System Requirements
Install the required tools for Linux and Windows covered above, then add the
following.

#### Android Studio
- Install 2.1 or later version of [Android Studio](https://developer.android.com/studio)
- From the "Welcome to Android Studio" splash screen, add the following components using Configure > SDK Manager:
  - SDK Tools > Android NDK

#### Add NDK to path

On Windows:
```
set PATH=%LOCALAPPDATA%\Android\sdk\ndk-bundle;%PATH%
```
On Linux:
```
export PATH=$HOME/Android/Sdk/ndk-bundle:$PATH
```
On macOS:
```
export PATH=$HOME/Library/Android/sdk/ndk-bundle:$PATH
```

## Getting Started Build Instructions

### 64-bit Windows Build 
```
    git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git
    cd VulkanTools
    mkdir build
    .\update_external_sources.bat
    cd build
    ..\scripts\update_deps.py --arch x64
    cmake -A x64 -C helper.cmake ..
    cmake --build . --parallel
```

### 32-bit Windows Build 
```
    git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git
    cd VulkanTools
    mkdir build
     .\update_external_sources.bat
    cd build
    ..\scripts\update_deps.py --arch Win32
    cmake -A Win32 -C helper.cmake ..
    cmake --build . --parallel
```

### Windows Unit Tests

```
ctest -C Debug --output-on-failure --parallel 16
ctest -C Release  --output-on-failure --parallel 16
```

### Linux and macOS Build
```
    git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git
    cd VulkanTools
    mkdir build
    ./update_external_sources.sh
    cd build
    ../scripts/update_deps.py
    cmake -C helper.cmake ..
    cmake --build . --parallel
```

### Linux and macOS Unit Tests

```
ctest --parallel 8 --output-on-failure
```

### Android Build
Use the following to ensure the Android build works.

#### Android Build from Windows
From Developer Command Prompt for VS2015:
```
cd build-android
update_external_sources_android.bat
android-generate.bat
ndk-build
```

#### Android Build from Linux
From your terminal:
```
cd build-android
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(nproc)
```

#### Android Build from macOS 
From your terminal:
```
cd build-android
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(sysctl -n hw.ncpu)
```

#### vkjson_info
Currently vkjson_info is only available as an executable for devices with root access.

To use, simply push it to the device and run it.  The resulting json file will be found in:
```
/sdcard/Android/<output>.json
```

## The VulkanTools repository

### Cloning the repository and updaing subcomponents

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

### Updating the Repository After a Pull

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

### Repository Dependencies
This repository attempts to resolve some of its dependencies by using
components found from the following places, in this order:

1. CMake or Environment variable overrides (e.g., -DVULKAN_HEADERS_INSTALL_DIR)
1. LunarG Vulkan SDK, located by the `VULKAN_SDK` environment variable
1. System-installed packages, mostly applicable on Linux

Dependencies that cannot be resolved by the SDK or installed packages must be
resolved with the "install directory" override and are listed below. The
"install directory" override can also be used to force the use of a specific
version of that dependency.

#### Vulkan-Headers

This repository has a required dependency on the
[Vulkan Headers repository](https://github.com/KhronosGroup/Vulkan-Headers).
You must clone the headers repository and build its `install` target before
building this repository. The Vulkan-Headers repository is required because it
contains the Vulkan API definition files (registry) that are required to build
the validation layers. You must also take note of the headers' install
directory and pass it on the CMake command line for building this repository,
as described below.

#### Vulkan-Loader

The tools in this repository depend on the Vulkan loader.

A loader can be used from an installed LunarG SDK, an installed Linux package,
or from a driver installation on Windows.

If a loader is not available from any of these methods and/or it is important
to use a loader built from a repository, then you must build the
[Vulkan-Loader repository](https://github.com/KhronosGroup/Vulkan-Loader.git)
with its install target. Take note of its install directory location and pass
it on the CMake command line for building this repository, as described below.

#### Vulkan-ValidationLayers
The tools in this repository depend on the Vulkan validation layers.

Validation layers can be used from an installed LunarG SDK, an installed Linux
package, or from a driver installation on Windows.

If the validation layers are not available from any of these methods and/or
it is important to use the validation layers built from a repository, then you
must build the
[Vulkan-ValidationLayers repository](https://github.com/KhronosGroup/Vulkan-ValidationLayers.git)
with its install target. Take note of its install directory location and pass
it on the CMake command line for building this repository, as described below.

#### Khronos Vulkan-Tools

The tests in this repository depend on the Vulkan-Tools repository, which is
hosted under Khronos' GitHub account and differentiated in name by a hyphen.
The tests use Vulkan Info and the mock ICD from Vulkan-Tools.

You may build the
[Vulkan-Tools repository](https://github.com/KhronosGroup/Vulkan-Tools.git)
with its install target. Take note of its build directory location and set
the VULKAN\_TOOLS\_BUILD\_DIR environment variable to the appropriate path.

If you do not intend to run the tests, you do not need Vulkan-Tools.

## Advanced Build and Install Directories

A common convention is to place the build directory in the top directory of
the repository with a name of `build` and place the install directory as a
child of the build directory with the name `install`. The remainder of these
instructions follow this convention, although you can use any name for these
directories and place them in any location.

### Getting Dependent Repositories using `update_deps.py` script

The Python utility script, `scripts/update_deps.py`, can be use to gather and
build the dependent repositories mentioned above. This script uses information 
stored in the `scripts/known_good.json` file to check out dependent repository 
revisions that are known to be compatible with the revision of this repository 
that you currently have checked out. As such, this script is useful as a 
quick-start tool for common use cases and default configurations.

### Advanced Windows Build

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

#### Windows 64-bit Installation Notes
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

### Advanced Linux Build

This build process builds all items in the VulkanTools repository

Example debug build:
```
cd VulkanTools  # cd to the root of the VulkanTools git repository
cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=build/install -DVULKAN_HEADERS_INSTALL_DIR=absolute_path_to_install_directory -DVULKAN_LOADER_INSTALL_DIR=absolute_path_to_install_directory -DVULKAN_VALIDATIONLAYERS_INSTALL_DIR=absolute_path_to_install_directory
cd dbuild
make -j8
```

### Notes

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
