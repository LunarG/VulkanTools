# Build Instructions
This document contains the instructions for building this repository on Linux and Windows.

This repository contains Vulkan development tools, such as additional layers and VkTrace trace/replay utilities,
supplementing the loader and validation layer core components found at https://github.com/KhronosGroup.

## Index

1. [Required Tools](#required-tools)
2. [Retrieving the Source](#retrieving-the-source)
3. [Building](#building)

<br/>
## Required Tools

Before you can begin, you will need to install the appropriate tools for your platform.
More details about individual platform requirements can be found in the appropriate sections below.

* [Linux System Requirements](#linux-system-requirements)
* [Windows System Requirements](#windows-system-requirements)
* [Requirements for Building for Android](#android-requirements)

<br/>
## Retrieving the Source

The public repository for the the LunarG VulkanTools is hosted at https://github.com/LunarG.

If you intend to contribute, the preferred work flow is to fork the repo,
create a branch in your forked repo, do the work,
and create a pull request on GitHub to integrate that work back into the repo.


### Clone the Repository

Note that the [Vulkan-LoaderAndValidationLayers repo](https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers) content is included within the VulkanTools repo via a Git Submodule.

To create your local git repository of VulkanTools:
```
cd YOUR_DEV_DIRECTORY

# Clone the VulkanTools repo
git clone --recurse-submodules git@github.com:LunarG/VulkanTools.git

# Enter the folder containing the cloned source
cd VulkanTools

# Update the submodules that are a part of the tree:
git submodule update --init --recursive

# This will perform some initialization and ensure the subcomponents are built:
./update_external_sources.sh    # linux
./update_external_sources.bat   # windows
```

### Updating the Repository After a Pull

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

	modified:   submodules/Vulkan-LoaderAndValidationLayers (new commits)

no changes added to commit (use "git add" and/or "git commit -a")
```

To resolve this, simply update the sub-module using:

```
git submodule update --recursive
```

Then, update the external sources as before:

```
# This will perform some initialization and ensure the subcomponents are built:
./update_external_sources.sh    # linux
./update_external_sources.bat   # windows
```

Now, you should be able to continue building as normal.

#### Updating the Submodule Is Not Working!

Occasionally, updating the submodule will fail.
In some cases, you may do a `git status` and it still shows your submodule is
out of date.

Perform a `git diff` to see what's going on.
If your sub-module commit ID has "-dirty" on the end, this indicates you have
changes in your submodule that it doesn't like.
To clear it, change into the submodule directory and clear any issues there.

For example, if the issue was with the Vulkan-LoaderAndValidationLayers
submodule, do the following steps:

```
# Go into the submodule directory
cd submodules/Vulkan-LoaderAndValidationLayers

# Find out what the Git is not happy about
git status

# Clean up what you need to (git reset --hard HEAD, or manually deleting
# additional files using `rm` or `del`).
git reset --hard HEAD

# Go back up
cd ../..

# Now update the submodule
git submodule update --recursive
```

Typically, this may be because some built file remains in the submodule folder
and Git wants you to remove those files before it updates.


<br/>
## Building

Building varies based on your current platform and your desired target.
The following sections have been put together to discuss building for the various
targets:

* [Linux](#building-on-linux)
* [Windows](#building-on-windows)
* [Android](#building-for-android)

<br/>
### Building on Linux

#### Linux System Requirements

Linux has some additional requirements before a build can begin.

##### Ubuntu

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

##### Fedora Core

Fedora 23 and newer has been used to build the repo.
These additional packages are needed for building the components in this repo and can be installed using `dnf`
if they are not already present on your system:

```
# General build utilities
sudo dnf install autoconf automake bison cmake @development-tools git glibc-devel wget

# Windowing system items
sudo dnf install libwayland-client libwayland-server libX11-devel libxcb libxcb-devel libxkbcommon libxkbcommon-devel libXrandr libXrandr-devel wayland-devel xcb-util-devel xcb-util-keysyms xcb-util-keysyms-devel

# If you require the ability to target 32-bit builds, you will also need:
sudo dnf install gcc-c++.i686 glibc-devel.i686 libX11-devel.i686
```

#### Linux Build Steps

This build process builds all items in the VulkanTools repository

Example debug build:
```
cd YOUR_DEV_DIRECTORY/VulkanTools  # cd to the root of the VulkanTools git repository
cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug
cd dbuild
make -j8
```
<br/>

### Building on Windows

#### Windows System Requirements

Windows 7+ with additional required software packages:

- Microsoft Visual Studio 2015 or newer.
  - **Note:** it is possible that lesser/older versions may work, but not guaranteed.
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

#### Windows Build Steps

Cygwin is used in order to obtain a local copy of the Git repository, and to run the CMake command that creates Visual Studio files.
Visual Studio is used to build the software, and will re-run CMake as appropriate.

To build all Windows targets (e.g. in a "Developer Command Prompt for VS2015" window):
```
cd VulkanTools  # cd to the root of the VulkanTools git repository
mkdir build
cd build
cmake -G "Visual Studio 14 Win64" ..
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

##### Windows 64-bit Installation Notes
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

<br/>
### Building for Android
Install the required tools for Linux and Windows covered above, then add the
following.

#### Android Requirements

* Install 2.1 or later version of [Android Studio](http://tools.android.com/download/studio/stable)
* Once installed, install at least the following additional components:
  * Android 6.0 (Marshmallow) SDK
  * NDK

There are two ways to install the additional components:
1. During installation from the "Welcome to Android Studio" dialog
  * Select "Configure" and choose "SDK Manager"
  * Select "SDK Platforms" individual Android SDKs or "SDK Tools" to install the NDK
2. After installation from inside of Android studio:
  * Choose "File" menu and select "Settings" to open dialog box
  * In dialog box, on left side, expand "Appearance & Behavior", then expand "System Settings" and choose "Android SDK"
  * Select "SDK Platforms" individual Android SDKs or "SDK Tools" to install the NDK
  * Click "Apply" to install

##### Environment Variables

After installation is done, you will need to add the following environment variables to your system:

###### Linux:

```
export ANDROID_SDK_HOME=$HOME/Android/Sdk/
export ANDROID_NDK_HOME=$ANDROID_SDK_HOME/ndk-bundle/
export PATH=$ANDROID_SDK_HOME/platform-tools:$ANDROID_SDK_HOME/tools:$ANDROID_SDK_HOME/build-tools/27.0.3:$ANDROID_SDK_HOME/ndk-bundle:$PATH
```

###### Windows:

```
set ANDROID_SDK_HOME=%LOCALAPPDATA%\Android\sdk\
set ANDROID_NDK_HOME=%ANDROID_SDK_HOME%\ndk-bundle\
set PATH=%ANDROID_SDK_HOME%\platform-tools;%ANDROID_SDK_HOME%\tools;%ANDROID_SDK_HOME%\build-tools\27.0.3;%ANDROID_SDK_HOME%\ndk-bundle;%PATH%
```

###### OSX:
```
export ANDROID_SDK_HOME=$HOME/Library/Android/sdk
export ANDROID_NDK_HOME=$ANDROID_SDK_HOME/ndk-bundle/
export PATH=$ANDROID_SDK_HOME/platform_tools:$ANDROID_SDK_HOME/tools:$ANDROID_SDK_HOME/build-tools/27.0.3:$ANDROID_SDK_HOME/ndk-bundle:$PATH
```

** NOTE **: On some platforms, these may have automatically been done for you as part of the install.
But it is best to check and verify.

You may also have to define `JAVA_HOME` if it is not defined.
The location of the JRE used by Android Studio varies based on the installation platform.  Once you find
the location, you will also have to add %JAVA_HOME%/bin to your path.

##### Additional OSX System Requirements
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


#### Build steps for Android
Use the following to ensure the Android build works.

##### Linux
Follow the setup steps for Linux, then from your terminal:

```
cd build-android
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(nproc)
```

##### OSX
Follow the setup steps for OSX above, then from your terminal:

```
cd build-android
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(sysctl -n hw.ncpu)
```

##### Windows
Follow the setup steps for Windows above, then from Developer Command Prompt for VS2015:

```
cd build-android
update_external_sources_android.bat
android-generate.bat
ndk-build
```

### Android usage
This documentation is preliminary and needs to be beefed up.

See the [vktracereplay.sh](https://github.com/LunarG/VulkanTools/blob/master/build-android/vktracereplay.sh) file for a working example of how to use vktrace/vkreplay and screenshot layers.

Two additional scripts have been added to facilitate tracing and replaying any APK.  Note that these two scripts do not install anything for you, so make sure your target APK, vktrace, vktrace_layer, and vkreplay all use the same ABI.

```
./create_trace.sh --serial 0123456789 --abi armeabi-v7a --package com.example.CubeWithLayers  --activity android.app.NativeActivity
adb install --abi armeabi-v7a
./replay_trace.sh --serial 0123456789 --tracefile com.example.CubeWithLayers0.vktrace
```

#### Using the Scripts

##### Linux/OSX
An example of using the scripts on Linux and OSX:

```
./build_vktracereplay.sh
./vktracereplay.sh \
 --serial 12345678 \
 --abi armeabi-v7a \
 --apk ../demos/android/cube-with-layers/bin/cube-with-layers.apk \
 --package com.example.CubeWithLayers \
 --frame 50
```

##### Windows
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

#### vkjson_info
Currently vkjson_info is only available as an executable for devices with root access.

To use, simply push it to the device and run it.  The resulting json file will be found in:

```
/sdcard/Android/<output>.json
```
A working example can be found in [devsim_layer_test_anroid.sh](https://github.com/LunarG/VulkanTools/blob/master/build-android/devsim_layer_test_android.sh)
