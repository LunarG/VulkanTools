<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width="263" height="113" /></p>
<p align="left">Copyright © 2015-2021 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width="400" /></p>

# Release notes

## [Vulkan Configurator 2.4.0 for Vulkan SDK 1.2.X.0](https://github.com/LunarG/VulkanTools/tree/master) - July 2021

### Features:
- Add support for `VK_LAYER_SETTINGS_PATH` override for 'vk_layer_settings.txt' path
- Add 'vk_layer_settings.txt' path in the 'Vulkan Development Status'

### Improvements:
- Add link to [Vulkan Guide layers](https://github.com/KhronosGroup/Vulkan-Guide/blob/master/chapters/development_tools.md#vulkan-layers) list within the help menu

## [Vulkan Configurator 2.3.0 for Vulkan SDK 1.2.176.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.176.0) - Mai 2021

### Features:
- Add Vulkan loader debug message to application launcher #1372
- Redesigned of the full layer settings UI #1420

### Improvements:
- Add display of layer execution order in the setting tree #1390
- Add layer settings UI inputs checking
- Add API dump presets #1414
- Add validation layer VUIDs loading from Vulkan Header to keep it up to date #1434
- Improve 'High Validation Layer Overhead' warning UI

### Fixes:
- Hide 'Callback' from validation layer 'Debug Action' debug action that is NOOP
- Fix crash when SDK path is included in 'override_paths' of 'VkLayer_override.json'
- Fix default value of validation layer 'Check Out of Bounds #1435
- Fix duplicate configurations #1445
- Fix layer reordering crash #1449
- Fix Vulkan application crash when updating the Vulkan loader
- Fix User-Defined layer path window 'Ok' button being disabled incorrectly
- Fix blank setting tree after using the user-defined layer path window #1377
- Fix 'Reset To Default' than wasn't bringing back notification pop-ups
- Fix creating a new 'Configuration' can be cancelled

## [Vulkan Configurator 2.2.0 for Vulkan SDK 1.2.170.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.170.0) - March 2021

### Features:
- Add *Synchronization* default configuration #1304
- Add *Portability* default configuration on *macOS* #1307
- Add per-configuration reset in layers configuration context menu
- Add per-configuration reset in layers window
- Expose `modify_extension_list` and `modify_memory_flags` settings from devsim

### Improvements:
- Add display of `VK_LAYER_PATH` content in 'Vulkan Development Status'
- Rename 'Custom Path' by 'User-Defined Path'
- Add extension filters to load and save file layer settings #1317
- Improve in application documentation of settings #1332
- Display the layer status in the 'Vulkan Development Status' #1332
- Refresh 'Vulkan Development Status' when the configuration changed #1366
- Streamline 'configuration tree' UI usage #1370
- Add 'Reload Default Configuration' to retrieve missing default configurations #1370
- Add 'reset' command line argument to reset the platform layers configurations
- Add possible recovery by resetting to default when a crash happened during last run

### Fixes:
- Fix export and import path being truncated when the path as '.' character
- Fix crash when loading a JSON file as if it's a JSON layer file but it's not #1330
- Fix validation layer 'printf buffer size' setting exposed as a 'bool' instead of a 'int' #1338
- Fix crash when using older `VK_LAYER_KHRONOS_validation` versions
- Fix *Vulkan Info* and *Vulkan Installation Analysis* dialog no longer running with overridden layers
- Fix preset changed causing the setting tree to be rebuild #1300

## [Vulkan Configurator 2.1.0 for Vulkan SDK 1.2.162.1](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.162.1) - January 2021

### Features:
- Refactor built-in configurations #1247
- Add layer settings presets #1271
- Add *Portability* default configuration on Linux and Windows #1279

### Improvements:
- In case of crash or user interruption, remove the layers override #1278
- Improve layers support since version 1.1.130
- Improve layer configurations compatibilities between versions
- Add more tool tips #1296
- Display the 'Vulkan Development Status' each time the layers paths are changed

### Fixes:
- Expose missing `VK_LAYER_LUNARG_gfxreconstruct` 'Log Detailed' setting

## [Vulkan Configurator 2.0.3 for Vulkan SDK 1.2.162.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.162.0) - December 2020

### Features:
- Add command line arguments to manage layers override #1213

### Improvements:
- Test with *VS2015*, *VS2017* and *VS2019*
- Test with *Qt 5.9* and *Qt 5.15* including *QMake* build
- Test manually using a [use cases based template](https://docs.google.com/document/d/1z0WqfMp2IBko1fvDICkjDE_3JKnf8SrU5APQTqKRR-U/edit)

### Fixes:
- Fix manual layers ordering #1214
- Fix layers override update when changing layers management options #1225
- Fix reset button in layers window #1227
- Fix warning about old loader when trying to use application list #1230
- Fix initialization of added applications on macOS #1249 

## [Vulkan Configurator 2.0.2 for Vulkan SDK 1.2.154.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.154.0) - October 2020

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
- Make *Validation - Standard*, the default configuration #1076
- Update `vkcube` to use the version from the same *Vulkan SDK* than *Vulkan Configurator* #1112
- Expose `duplicate_message_limit` setting from `VK_LAYER_KHRONOS_validation` #1176

### Fixes:
- Fix disabled configurations that can't be reenabled or removed
- Fix display of custom layer paths #1101
- Fix inconsistant display of separators in paths #1107
- Fix display of empty entries in the custom path dialog when opening Json files that are not layer json files #1103
- Fix log file default location to `HOME` to ensure write access #1083
- Fix log file not written to #1109
- Fix `debug_output` setting displayed on Linux and macOS but not avaiable #1086
- Fix `numeric_bool` setting display #1166

## [Vulkan Configurator 2.0.1 for Vulkan SDK 1.2.148.1](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.148.1) - August 2020 (revision 1)

### Fixes:
- Fix *Shader Printf* built-in configuration that was setup for *GPU-Assisted* validation isntead
- Fix layer configurations selection on Linux
- Fix debug output setting not working

## [Vulkan Configurator 2.0.0 for Vulkan SDK 1.2.148.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.148.0) - August 2020

### Features:
- Design GUI
- Add built-in Vulkan layers configurations
- Add user-defined Vulkan layers configurations
- Add import and export of Vulkan layers configuration
- Add `VK_LAYER_KHRONOS_validation` settings presets
- Add message filtering using VUID and message IDs
- Add an option to make layers overriding persistent on exit
- Add an option to override only a selected list of applications
- Add an application launcher
- Add startup Vulkan status
