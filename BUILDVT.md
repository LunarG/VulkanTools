# Build Instructions
These instructions are for Linux and Windows.

This repository contains addtional layers, tests, and the VkTrace trace/replay tools, supplementing the
loader and validation layer core components found at https://github.com/KhronosGroup.

For Linux, this repository also contains a sample Intel Vulkan driver that is being deprecated.
Instead of using this driver, it is suggested that you contact your graphics device manufacturer
to obtain a Vulkan driver for your GPU hardware.

## Git the Bits

The public repository for the the LunarG VulkanTools is hosted at https://github.com/LunarG.

If you intend to contribute, the preferred work flow is to fork the repo,
create a branch in your forked repo, do the work,
and create a pull request on GitHub to integrate that work back into the repo.

## Linux System Requirements
Ubuntu 14.04.3 LTS, 14.10, 15.04 and 15.10 have been tested with this repo.

These packages are used for building the components in this repo.
```
sudo apt-get install git subversion cmake libgl1-mesa-dev freeglut3-dev libglm-dev libmagickwand-dev qt5-default libpciaccess-dev libpthread-stubs0-dev libudev-dev bison graphviz libpng-dev
sudo apt-get build-dep mesa
```

If you are using the sample Intel Vulkan driver in this repo, you will have to ensure that
the DRI3 extension is active in your X Server.
Follow the
[DRI3 Instructions](dri3.md)
to prepare your X Server.

## Clone the Repository

Note that the Vulkan-LoaderAndValidationLayers repo content is included within the VulkanTools repo.

To create your local git repository of VulkanTools:
```
cd YOUR_DEV_DIRECTORY
git clone git@github.com:LunarG/VulkanTools.git
cd VulkanTools
# This will fetch and build glslang and spriv-tools
# On Linux, it will also fetch and build LunarGLASS
./update_external_sources.sh         # linux
./update_external_sources.bat --all  # windows
```

## Linux Build

This build process builds the icd, vktrace and the tests.

Example debug build:
```
cd YOUR_DEV_DIRECTORY/VulkanTools  # cd to the root of the VulkanTools git repository
cmake -H. -Bdbuild -DCMAKE_BUILD_TYPE=Debug
cd dbuild
make
```

## Linux Test

The test executables can be found in the dbuild/tests directory. The tests use the Google
gtest infrastructure. Tests available so far:
- vkbase: Test basic entry points
- vk_blit_tests: Test VK Blits (copy, clear, and resolve)
- vk_image_tests: Test VK image related calls needed by render_test
- vk_render_tests: Render a single triangle with VK. Triangle will be in a .ppm in
the current directory at the end of the test.

There are also a few shell and Python scripts that run test collections (eg,
`run_all_tests.sh`).


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
- Image Magick is used by the tests to compare images (from http://www.imagemagick.org/script/binary-releases.php)
  - Install each a 32-bit and a 64-bit version, as the 64-bit installer does not install the 32-bit libraries and tools.
    - Here are some helpful links (warning they may go out of date with a newer ImageMagick).
      - 64-bit: http://www.imagemagick.org/download/binaries/ImageMagick-6.9.3-2-Q16-x64-dll.exe
      - 32-bit: http://www.imagemagick.org/download/binaries/ImageMagick-6.9.3-2-Q16-x86-dll.exe
  - For each of the installs, be sure to check box to "Install development headers and libraries"
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

Example debug x64 build (e.g. in a "Developer Command Prompt for VS2013" window):
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

Vulkan programs must be able to find and use the Vulkan-1.dll libary.
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

