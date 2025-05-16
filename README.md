# Vulkan Ecosystem Tools

The repository contains the following Vulkan Tools:
- [Vulkan Configurator](vkconfig/README.md)
- [`VK_LAYER_LUNARG_api_dump`, `VK_LAYER_LUNARG_screenshot` and `VK_LAYER_LUNARG_monitor` layers](layersvt/README.md)

These tools have binaries included within the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/).

## Contributing

If you intend to contribute, the preferred work flow is for you to develop your contribution
in a fork of this repo in your GitHub account and then submit a pull request.
Please see the [CONTRIBUTING](CONTRIBUTING.md) file in this repository for more details

## How to Build and Run

[BUILD.md](BUILD.md) includes directions for building all the components and running the tests.


## Generated Code

Some of the source code in this repository is generated in `layersvt/generated/`, from data descriptive headers in the Vulkan-Headers repository.

## How to generate the code

- Linux:
```bash
scripts/generate_source.py external/Vulkan-Headers/[config]/[architecture]/registry/
     where
config is, for example, 'Debug' or 'Release', and architecture is '64' or '32'.
```

- Windows Powershell:
```powershell
pwsh -Command { python3 scripts/generate_source.py external/Vulkan-Headers/[config]/[arch]/registry/ }
```

- Windows Command:
```cmd
cmd /C "python3 scripts/generate_source.py external/Vulkan-Headers/[config]/[arch]/registry/"
```

When making change to the `scripts/` folder, make sure to run `generate_source.py` and check in both the changes to
`scripts/` and `layersvt/generated/` in any PR. (Code generation does **not** happen automatically at build time.)

## CMake helper

A helper CMake target `vt_codegen` is also provided to simplify the invocation of `scripts/generate_source.py` from the build directory:

```bash
cmake -S . -B build -D VT_CODEGEN=ON
cmake --build build --target vt_codegen
```

## Version Tagging Scheme

Updates to this repository which correspond to a new Vulkan specification release are tagged using the following format: `v<`_`version`_`>` (e.g., `v1.3.266`).

**Note**: Marked version releases have undergone thorough testing but do not imply the same quality level as SDK tags. SDK tags follow the `vulkan-sdk-<`_`version`_`>.<`_`patch`_`>` format (e.g., `vulkan-sdk-1.3.266.0`).

This scheme was adopted following the `1.3.266` Vulkan specification release.

## Removed Tools

- *Vulkan Installation Analyzer* (vkvia) has been deprecated and replaced by *Vulkan Configurator* diagnostics tab. The last available version was released with [Vulkan SDK 1.4.309](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.309).
- `VkTrace` and `VkReplay` have been deprecated, removed from VulkanTools and replaced by [gfxreconstruct](https://github.com/LunarG/gfxreconstruct). They can now be found in the [vktrace](https://github.com/LunarG/vktrace) archive. Both these tools are also no longer part of the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/).
- *Vulkan-Layer-Factory* is deprecated and has been archived in its own repository which is available [here](https://github.com/LunarG/Vulkan-Layer-Factory).

## License
This work is released as open source under a [Apache-style license](LICENSE.txt) from Khronos including a Khronos copyright.

## Acknowledgements
While this project has been developed primarily by LunarG, Inc., there are many other companies and individuals making this possible: Valve Corporation, funding project development.
