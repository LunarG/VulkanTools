# Vulkan Ecosystem Tools

The repository contains the following Vulkan Tools:
- [Vulkan Configurator](vkconfig/README.md)
- [`VK_LAYER_LUNARG_api_dump`, `VK_LAYER_LUNARG_screenshot` and `VK_LAYER_LUNARG_monitor` layers](layersvt/README.md)
- [Vulkan Installation Analyzer](via/README.md)

These tools have binaries included within the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/).

## Removed Tools

VkTrace and VkReplay have been deprecated and replaced by [gfxreconstruct](https://github.com/LunarG/gfxreconstruct).
Both VkTrace and VkReplay have been removed from VulkanTools and can now be found in the [vktrace](https://github.com/LunarG/vktrace) archive.
Both these tools are also no longer part of the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/).

Vulkan-Layer-Factory is deprecated and has been archived in its own repository which is available [here](https://github.com/LunarG/Vulkan-Layer-Factory).

## Contributing

If you intend to contribute, the preferred work flow is for you to develop your contribution
in a fork of this repo in your GitHub account and then submit a pull request.
Please see the [CONTRIBUTING](CONTRIBUTING.md) file in this repository for more details

## How to Build and Run

[BUILD.md](BUILD.md) includes directions for building all the components and running the tests.

## Version Tagging Scheme

Updates to this repository which correspond to a new Vulkan specification release are tagged using the following format: `v<`_`version`_`>` (e.g., `v1.3.266`).

**Note**: Marked version releases have undergone thorough testing but do not imply the same quality level as SDK tags. SDK tags follow the `vulkan-sdk-<`_`version`_`>.<`_`patch`_`>` format (e.g., `vulkan-sdk-1.3.266.0`).

This scheme was adopted following the `1.3.266` Vulkan specification release.

## License
This work is released as open source under a [Apache-style license](LICENSE.txt) from Khronos including a Khronos copyright.

## Acknowledgements
While this project has been developed primarily by LunarG, Inc., there are many other companies and individuals making this possible: Valve Corporation, funding project development.
