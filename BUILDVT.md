# Build Instructions
This document contains the instructions for building this repository on Linux and Windows.

This repository contains additional layers and the VkTrace trace/replay tools, supplementing the
loader and validation layer core components found at https://github.com/KhronosGroup.

This repository previously contained a sample Intel Vulkan driver that has since been deprecated.
The final stable version of that source code is available from this repo by checking out the git
tag "Vulkan-ICD-Snapshot". This source code is no longer supported.

## Git the Bits

The public repository for the the LunarG VulkanTools is hosted at https://github.com/LunarG.

If you intend to contribute, the preferred work flow is to fork the repo,
create a branch in your forked repo, do the work,
and create a pull request on GitHub to integrate that work back into the repo.

## Linux System Requirements
Ubuntu 14.04.3 LTS, 14.10, 15.04,15.10, and 16.04 LTS have been tested with this repo.

These additional packages are needed for building the components in this repo.
```
# Dependencies from the LoaderAndValidationLayers repo:
sudo apt-get install git cmake build-essential bison libx11-xcb-dev libxkbcommon-dev libmirclient-dev libwayland-dev libxrandr-dev
# Additional dependencies for this repo:
sudo apt-get install wget autotools-dev libxcb-keysyms1 libxcb-keysyms1-dev
# If performing 32-bit builds, you'll also need:
sudo apt-get install libc6-dev-i386 g++-multilib
```

## Clone the Repository

Note that the Vulkan-LoaderAndValidationLayers repo content is included within the VulkanTools repo.

To create your local git repository of VulkanTools:
```
cd YOUR_DEV_DIRECTORY
git clone git@github.com:LunarG/VulkanTools.git
cd VulkanTools
# This will fetch and build glslang and spriv-tools
./update_external_sources.sh         # linux
./update_external_sources.bat --all  # windows
```

## Linux Build

This build process builds vktrace and the LVL tests.

Example debug build:
```
cd YOUR_DEV_DIRECTORY/VulkanTools  # cd to the root of the VulkanTools git repository
cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug
cd dbuild
make
```

## Windows System Requirements

Windows 7+ with additional required software packages:

- Microsoft Visual Studio 2013 Professional.  Note: it is possible that lesser/older versions may work, but that has not been tested.
- CMake (from http://www.cmake.org/download/).  Notes:
  - In order to build the VkTrace tools, you need at least version 3.0.
  - Tell the installer to "Add CMake to the system PATH" environment variable.
- Python 3 (from https://www.python.org/downloads).  Notes:
  - Select to install the optional sub-package to add Python to the system PATH environment variable.
  - Need python3.3 or later to get the Windows py.exe launcher that is used to get python3 rather than python2 if both are installed on Windows
- Git (from http://git-scm.com/download/win).
  - Note: If you use Cygwin, you can normally use Cygwin's "git.exe".  However, in order to use the "update_external_sources.bat" script, you must have this version.
  - Tell the installer to allow it to be used for "Developer Prompt" as well as "Git Bash".
  - Tell the installer to treat line endings "as is" (i.e. both DOS and Unix-style line endings).
- glslang is required for tests.
  - You can download and configure it (in a peer directory) here: https://github.com/KhronosGroup/glslang/blob/master/README.md
  - A windows batch file has been included that will pull and build the correct version.  Run it from Developer Command Prompt for VS2013 like so:
    - update_external_sources.bat --build-glslang

Optional software packages:

- Cygwin (from https://www.cygwin.com/).  Notes:
  - Cygwin provides some Linux-like tools, which are valuable for obtaining the source code, and running CMake.
    Especially valuable are the BASH shell and git packages.
  - If you don't want to use Cygwin, there are other shells and environments that can be used.
    You can also use a Git package that doesn't come from Cygwin.

## Windows Build

Cygwin is used in order to obtain a local copy of the Git repository, and to run the CMake command that creates Visual Studio files.  Visual Studio is used to build the software, and will re-run CMake as appropriate.

To build all Windows targets (e.g. in a "Developer Command Prompt for VS2013" window):
```
cd VulkanTools  # cd to the root of the VulkanTools git repository
mkdir build
cd build
cmake -G "Visual Studio 12 Win64" ..
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
If you plan on creating a Windows Install file (done in the windowsRuntimeInstaller sub-directory) you will need to build for both 32-bit and 64-bit Windows since both versions of EXEs and DLLs exist simultaneously on Windows 64.

To do this, simply create and build the release versions of each target:
```
cd VulkanTools  # cd to the root of the Vulkan git repository
mkdir build
cd build
cmake -G "Visual Studio 12 Win64" ..
msbuild ALL_BUILD.vcxproj /p:Platform=x64 /p:Configuration=Release
mkdir build32
cd build32
cmake -G "Visual Studio 12" ..
msbuild ALL_BUILD.vcxproj /p:Platform=x86 /p:Configuration=Release
```
## Android Build
Install the required tools for Linux and Windows covered above, then add the
following.
### Android Studio
- Install 2.1 or later verion of [Android Studio](http://tools.android.com/download/studio/stable)
- From the "Welcome to Android Studio" splash screen, add the following components using Configure > SDK Manager:
  - SDK Tools > Android NDK

#### Add NDK to path

On Linux:
```
export PATH=$HOME/Android/sdk/ndk-bundle:$PATH
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
Follow the setup steps for Windows above, then from Developer Command Prompt for VS2013:
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
 --apk ../demos/android/cube-with-layers/bin/NativeActivity-debug.apk \
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
### api_dump
To enable, make the following changes to vk_layer_settings.txt
```
-lunarg_api_dump.file = FALSE
+lunarg_api_dump.file = TRUE

-lunarg_api_dump.log_filename = stdout
+lunarg_api_dump.log_filename = /sdcard/Android/vk_apidump.txt
```
Then:
```
adb push vk_layer_settings.txt /sdcard/Android
```
And run your application with the following layer enabled:
```
VK_LAYER_LUNARG_api_dump
```
### screenshot
To enable, set a property that contains target frame:
```
adb shell setprop debug.vulkan.screenshot <framenumber>
```
For production builds, be sure your application has access to read and write to external storage by adding the following to AndroidManifest.xml:
```
<!-- This allows writing log files to sdcard -->
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
```
You may also need to grant it access with package manager:
```
adb shell pm grant com.example.Cube android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.Cube android.permission.WRITE_EXTERNAL_STORAGE
```
Run your application with the following layer enabled:
```
VK_LAYER_LUNARG_screenshot
```
Result screenshot will be in:
```
/sdcard/Android/<framenumber>.ppm
```
### device_simulation
To enable, set a property with the path of configuration file to load:
```
adb shell setprop debug.vulkan.devsim.filepath <path/to/DevSim/JSON/configuration/file>
```
Example of a DevSim JSON configuration file: [tiny1.json](https://github.com/LunarG/VulkanTools/blob/master/layersvt/device_simulation_examples/tiny1.json)

Optional: set properties to enable debugging output and exit-on-error:
```
adb shell setprop debug.vulkan.devsim.debugenable 1
adb shell setprop debug.vulkan.devsim.exitonerror 1
```
Run your application with the following layer enabled:
```
VK_LAYER_LUNARG_device_simulation
```
### vkjson_info
Currently vkjson_info is only available as an executable for devices with root access.

To use, simply push it to the device and run it.  The resulting json file will be found in:
```
/sdcard/Android/<output>.json
```
A working example can be found in [devsim_layer_test_anroid.sh](https://github.com/LunarG/VulkanTools/blob/master/build-android/devsim_layer_test_android.sh)
### vktrace
To record a trace on Android, enable port forwarding from the device to the host:
```
adb reverse localabstract:vktrace tcp:34201
```
Start up vktrace on the host in server mode:
```
vktrace -v full -o cube.vktrace
```
Run your application with the following layer enabled:
```
VK_LAYER_LUNARG_vktrace
```
The trace will be recorded on the host.
### vkreplay
To replay a trace, push the trace to your device
```
adb push cube.vktrace /sdcard/cube.vktrace
```
Grant vkreplay the ability to read it
```
adb shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE
```
And start the native activity
```
adb shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/cube.vktrace"
```
