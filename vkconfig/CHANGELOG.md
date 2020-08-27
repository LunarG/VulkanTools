<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright © 2015-2020 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width=400 /></p>

# Release notes

## [Vulkan Configurator current](https://github.com/LunarG/VulkanTools)

### Improvements:
- Add layer configuration description #1105
- Add selected path from dialog window cache #1107
- Add test framework #1100
- Add button to browse working directory #1099
- Add version in the title bar and show <ACTIVE> state #1109
- Add default settings state in the layer configuration settings #1076
- Make Standard Validation, the default configuration #1076

### Fixes:
- Fix disabled configurations can't be reenabled or removed
- Fix display of custom layer paths #1101
- Fix GUI layer configurations layout #1101
- Fix inconsistant display of separators in path #1107
- Fix display of empty entries in the custom path dialog when opening Json files that are not layer json files #1103
- Fix log file default location to HOME to ensure write access #1083
- Fix log file not written to #1109

## [Vulkan Configurator 1.2.148-1001](https://github.com/LunarG/VulkanTools/releases/tag/sdk-1.2.148.1) - 2020-08-08

### Fixes:
- Fix Shader Printf built-in configuration that was setup for GPU-Assisted validation isntead
- Fix layer configurations selection on Linux
- Fix debug output setting not working

## [Vulkan Configurator 1.2.148-1000](https://github.com/LunarG/VulkanTools/releases/tag/sdk-1.2.148.0) - 2020-08-03

### Features:
- Design GUI
- Add built-in Vulkan layers configurations
- Add user-defined Vulkan layers configurations
- Add import and export of Vulkan layers configuration
- Add VK_LAYER_KHRONOS_validation settings presets
- Add message filtering using VUID and message IDs
- Add an option to make layers overriding persistent on exit
- Add an option to override only a selected list of applications
- Add an application launcher
- Add startup Vulkan status
