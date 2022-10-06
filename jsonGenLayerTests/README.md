# Json Generation Layer Test

This document is an overview of the file structure of this repo and how to run the test.

# System Requirements

- Python 3 (from https://www.python.org/downloads).  Notes:
  - Select to install the optional sub-package to add Python to the system `PATH` environment variable.
  - Need python3.3 or later to get the Windows `py.exe` launcher that is used to get `python3` rather than `python2` if both are installed on Windows
- Install latest VulkanSDK
  - Download from https://vulkan.lunarg.com/sdk/home
- Install latest Vulkan driver.

# Building unit test
- The cmake files are located at the jsonGenLayerTests
- Generate the build artifacts via cmake. This should generate the executable that is used by jsonGenLayerTests.py

# Environment Setup (Windows)

- Setting up Json Generation Layer -
  - Set "VK_JSON_FILE_PATH" to a custom directory
    - The generated Json files will be dumped into this directory.
    - Copy VkLayer_json_gen.dll and VkLayer_json_gen.json from the layer
      build directory to this path.
  - Add the VkLayer_json_gen.json in ${VK_JSON_FILE_PATH} into the Explicit Layers.
    - Use "regedit" to open Registry Editor, navigate to ExplicitLayers and add it there.

# File Structure
```
- jsonGenLayerTests
  - GoldenJsonFiles       // Contains golden json files used for comparing the generated ones
  - shaders               // Contains GLSL and SPIRV shaders
  - CMakeLists.txt
  - jsonGenLayerTests.py  // Script which runs the layer enabled vulkan app and compares the
                          // generated json files with the golden.
  - main.cpp              // Source file of the Vulkan app.
  - README.md             // This file
```

# How to Run the test

Command to run -
```
python .\jsonGenLayerTests.py <path_to_jsonGenLayerTest_build_dir> --shaders <path_to_jsonSpv_dir>

Example:
python .\jsonGenLayerTests.py "C:\Git\vksc_layer\vulkantools-lunarg\jsonGenLayerTests" --shaders .\shaders
```

# Updating Golden Json Files

If the differences with the Golden Json Files are expected, then the golden json files
should be updated to the generated golden files.

```
cp ${VK_JSON_FILE_PATH}/JSON_GEN_LAYER_TESTS_* GoldenJsonFiles/
```

# Instructions on creating SPIRV shaders
## On Windows
```
<path_to_VulkanSDK>/x.x.x.x/Bin32/glslc.exe ./glsl/shader.vert -o vert.spv
<path_to_VulkanSDK>/x.x.x.x/Bin32/glslc.exe ./glsl/shader.frag -o frag.spv
```
## On Linux
```
<path_to_VulkanSDK>/x.x.x.x/x86_64/bin/glslc shader.vert -o vert.spv
<path_to_VulkanSDK>/x.x.x.x/x86_64/bin/glslc shader.frag -o frag.spv
```
