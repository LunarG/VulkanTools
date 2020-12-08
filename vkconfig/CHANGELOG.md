<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width="263" height="113" /></p>
<p align="left">Copyright © 2015-2020 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width="400" /></p>

# Release notes

## [Vulkan Configurator 2.0.3 for Vulkan SDK 1.2.162](https://github.com/LunarG/VulkanTools/releases/tag/sdk-1.2.162.0) - December 2020

### Features:
- Add command line arguments to manage layers override #1213

### Improvements:
- Test with VS2015, VS2017 and VS2019
- Test with Qt 5.9 and Qt 5.15 including QMake build
- Test manually using a [use cases based template](https://docs.google.com/document/d/1z0WqfMp2IBko1fvDICkjDE_3JKnf8SrU5APQTqKRR-U/edit)

### Fixes:
- Fix manual layers ordering #1214
- Fix layers override update when changing layers management options #1225
- Fix reset button in layers window #1227
- Fix warning about old loader when trying to use application list #1230
- Fix initialization of added applications on macOS #1249 

## [Vulkan Configurator 2.0.2 for Vulkan SDK 1.2.154.0](https://github.com/LunarG/VulkanTools/releases/tag/sdk-1.2.154.0) - October 2020

### Features:
- Add multiple debug actions simulaneously #1119
- Add better representation of the layers execution order #1181

### Improvements:
- Streamline and clean up GUI
- Add layers configuration description #1105
- Add selected path from dialog window cache #1107
- Add unit test framework #1100
- Add button to browse working directory #1099
- Add version in the title bar and show <ACTIVE> state #1109
- Add default settings state in the layer configuration settings #1076
- Make "Validation - Standard", the default configuration #1076
- Update vkcube to use the version from the same Vulkan SDK than Vulkan Configurator #1112
- Expose `duplicate_message_limit` setting from VK_LAYER_KHRONOS_validation #1176

### Fixes:
- Fix disabled configurations that can't be reenabled or removed
- Fix display of custom layer paths #1101
- Fix inconsistant display of separators in paths #1107
- Fix display of empty entries in the custom path dialog when opening Json files that are not layer json files #1103
- Fix log file default location to HOME to ensure write access #1083
- Fix log file not written to #1109
- Fix `debug_output` setting displayed on Linux and macOS but not avaiable #1086
- Fix `numeric_bool` setting display #1166

### Known issues:
- *Using Vulkan Configurator is exclusive from using layer environment variables. Choose one or the other method.*
- Layers will use the override layer settings and ignore the local file with no warning to the user.
- Layers settings fields are not checked for syntax errors.
- Layer changes are not autodetected. If changes are made to the layer environment, *Vulkan Configurator* must be restarted to take into account these changes.
- On *macOS*, *vkcube* and *vkcubepp* need to be run at least once to get past the security checks before they can be used from *Vulkan Configurator*.
- The screenshot layer can crash if you do not specify a working directory that is writable by the current user.

## [Vulkan Configurator 2.0.1 for Vulkan SDK 1.2.148.1](https://github.com/LunarG/VulkanTools/releases/tag/sdk-1.2.148.1) - August 2020 (revision 1)

### Fixes:
- Fix Shader Printf built-in configuration that was setup for GPU-Assisted validation isntead
- Fix layer configurations selection on Linux
- Fix debug output setting not working

### Known issues:
- Minor GUI formating issues may still occur on some Linux distributions (Fedora particularly)
- Layers will use the override layer settings and ignore the local file with no warning to the user.
- Layer paths may not be duplicated in the layer override json file. They currently are.
- Layer execution order express in the "Select Layers" window is not accurate, only forced on layers can be ordered.
- Layers settings fields are not checked for syntax errors.
- The user can't reorder the layers configurations in the list.
- Vulkaninfo output is now sorted alphabetically.
- On Linux, when run for the first time from the command line, vkcube may not be located and added to the application launcher automatically.
- A version or build # needs to be added to the about box.
- Layer changes are not autodetected. If changes are made to the layer environment, vkconfig must be restarted.
- On macOS, vkcube and vkcubepp need to be run at least once to get past the security checks before they can be used from vkconfig.
- The screenshot layer can crash if you do not specify a working directory that is writable by the current user.

## [Vulkan Configurator 2.0.0 for Vulkan SDK 1.2.148.0](https://github.com/LunarG/VulkanTools/releases/tag/sdk-1.2.148.0) - August 2020

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
