# Build Instructions
This document contains the instructions for building this repository on Linux and Windows.

This repository contains Vulkan development tools, such as additional layers and VkTrace trace/replay utilities,
supplementing the loader and validation layer core components found at https://github.com/KhronosGroup.

## Git the Bits

The public repository for the the LunarG VulkanTools is hosted at https://github.com/LunarG.

If you intend to contribute, the preferred work flow is to fork the repo,
create a branch in your forked repo, do the work,
and create a pull request on GitHub to integrate that work back into the repo.

## Linux System Requirements
Ubuntu 14.04.3 LTS, 14.10, 15.04,15.10, and 16.04 LTS have been tested with this repo.

These additional packages are needed for building the components in this repo.
```
# Dependencies from included submodule components
sudo apt-get install git cmake build-essential bison libx11-xcb-dev libxkbcommon-dev libmirclient-dev libwayland-dev libxrandr-dev
# Additional dependencies for this repo:
sudo apt-get install wget autotools-dev libxcb-keysyms1 libxcb-keysyms1-dev
# If performing 32-bit builds, you will also need:
sudo apt-get install libc6-dev-i386 g++-multilib
```


# [Vulkan-Loader](https://github.com/KhronosGroup/Vulkan-Loader.git)
  - Building the VulkanTools repository requires linking to the Vulkan Loader Library (vulkan-1.dll).
    Locating the library for this repo can be done in two different ways:
      -  The Vulkan SDK can be installed. In this case, cmake should be able to locate the loader repo through the VulkanSDK
         environment variable
      -  The library can be built from the [Vulkan-Loader](https://github.com/KhronosGroup/Vulkan-Loader.git) repository.
         In this case, the following option should be used on the cmake command line:
             LOADER_REPO_ROOT=c:\developement\Vulkan-Loader
         and use absolute (not relative) paths, like so:
             cmake -DLOADER_REPO_ROOT=c:\absolute_path_to\Vulkan-Loader ....

# [glslang](https://github.com/KhronosGroup/glslang)
  - Ensure that the 'update_glslang_sources.py' script has been run, and the repository successfully built.
  - Follow the build instructions in the glslang repository, including INSTALL_PREFIX and 'make install', noting the install dir location.
      Indicate the location of the glslang components by using the following cmake option:
             cmake -DGLSLANG_INSTALL_DIR=c:\absolute_path_to\glslang\location_of\install ....



## Clone the Repository

Note that the [Vulkan-ValidationLayers repo](https://github.com/KhronosGroup/Vulkan-ValidationLayers) content is included within the VulkanTools repo via a Git Submodule.

To create your local git repository of VulkanTools:
```
cd YOUR_DEV_DIRECTORY

# Clone the VulkanTools repo
git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git

# Enter the folder containing the cloned source
cd VulkanTools

# Update the submodules that are a part of the tree:
git submodule update --init --recursive

# This will perform some initialization and ensure subcomponents are built:
./update_external_sources.sh    # linux
./update_external_sources.bat   # windows
```

## Updating the Repository After a Pull

Since the VulkanTools repo now contains Git Submodules, you may occasionally have
to update the source in those submodules.
You will know this needs to be performed when you perform a pull, and you check the
status of your tree with `git status` and something similar to the following shows:

```
(master *)] $ git status
On branch master
Your branch is up-to-date with 'origin/master'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   submodules/Vulkan-ValidationLayers (new commits)

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


## Linux Build

This build process builds all items in the VulkanTools repository

Example debug build:
```
cd YOUR_DEV_DIRECTORY/VulkanTools  # cd to the root of the VulkanTools git repository
if the SDK is installed and the accompanying setup-env.sh script has been run (setting up the VULKAN_SDK environment variable),
    cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug -DGLSLANG_INSTALL_DIR=c:\absolute_path_to\glslang\location_of\install
or, if a specific Vulkan Loader library is desired, point to it like so:
    cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug -DGLSLANG_INSTALL_DIR=c:\absolute_path_to\glslang\location_of\install -DLOADER_REPO_ROOT=c:\absolute_path_to\loader

cd dbuild
make -j8
```

## Windows System Requirements

Windows 7+ with additional required software packages:

- Microsoft Visual Studio 2015 Professional or 2017 Professional.  Note: it is possible that lesser/older versions may work, but not guaranteed.
- CMake (from http://www.cmake.org/download/).  Notes:
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
mkdir build
cd build
If the SDK is installed,
    cmake -DGLSLANG_INSTALL_DIR=/absolute_path_to/glslang/location_of/install -G "Visual Studio 14 Win64" ..
or, if a specific Vulkan Loader library is desired, point to it like so:
    cmake -DGLSLANG_INSTALL_DIR=/absolute_path_to/glslang/location_of/install -DLOADER_REPO_ROOT=/absolute_path_to/loader -G "Visual Studio 14 Win64" ..
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
