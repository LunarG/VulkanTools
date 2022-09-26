<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width="263" height="113" /></p>
<p align="left">Copyright © 2015-2022 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width="400" /></p>

# Release notes

## [Vulkan Configurator 2.5.1](https://github.com/LunarG/VulkanTools/tree/master) - October 2022

### Features:
- Add ENUM setting type children settings #1746

### Improvements:
- Allow disabling the validation layer settings built-in UI #1746

### Fixes:
- Fix override layer version when some layers are excluded or missing #1743

## [Vulkan Configurator 2.5.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.3.224.0) - August 2022

### Features:
- Add per layers-configuration user-defined layers paths #1711

### Improvements:
- Add reordering of configurations when renaming a configuration
- Improve handling of missing layers

## [Vulkan Configurator 2.4.6](https://github.com/LunarG/VulkanTools/tree/sdk-1.3.216.0) - June 2022

### Features:
- Add `VK_KHR_portability_enumeration` extension support

### Improvements:
- Allow using multiple layers built with different Vulkan header version when Loader 212 or newer is installed on the system

## [Vulkan Configurator 2.4.5](https://github.com/LunarG/VulkanTools/tree/sdk-1.3.211.0) - April 2022

### Features:
- Add `VK_ADD_LAYER_PATH` Vulkan Loader variable handling
- Remove built-in support of `VK_LAYER_LUNARG_device_simulation` layer #1628
  - Use `VK_LAYER_KHRONOS_profiles` layer instead

### Improvements:
- Improve the Portability built-in configuration to check only what's necessary, reducing validation overhead
- Refinement of the layers window UI design

## [Vulkan Configurator 2.4.4](https://github.com/LunarG/VulkanTools/tree/sdk-1.3.204.1) - March 2022

### Features:
- Add markdown documentation export
- Add `vk_layer_settings.txt` documentation export
- Add command line support of HTML, markdown and `vk_layer_settings.txt` documentation exports

### Improvements:
- Add button to create a new layers configuration

### Fixes:
- Fix layer presets that could be modified by accident #1653
- Fix loading of profiles file from the UI #1653

## [Vulkan Configurator 2.4.3](https://github.com/LunarG/VulkanTools/tree/sdk-1.3.204.0) - February 2022

### Improvements:
- Upgrade the built-in "Portability" to use the Profiles layer
- Display the development status of a setting next to the setting label in the UI
- Display a warning when using multiple layers built with different Vulkan Header version
  - It's a requirement by the Vulkan Loader

### Fixes:
- Fix ANY dependence when using FLAGS setting type

## [Vulkan Configurator 2.4.2](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.196.0) - November 2021

### Improvements:
- Discard applied layers configuration to ensure the layers never crash Vulkan Configurator

### Fixes:
- Fix 'Edit Layers' button being enabled when there is no active copnfiguration #1616

## [Vulkan Configurator 2.4.1](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.189.0) - September 2021

### Fixes:
- Fix user-defined layer settings not loaded, for older Vulkan API than 1.2.176
- Fix settings written in 'vk_layer_settings.txt' even when the dependence is not met #1582

## [Vulkan Configurator 2.4.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.182.0) - July 2021

### Features:
- Add support for `VK_LAYER_SETTINGS_PATH` override for 'vk_layer_settings.txt' path
- Add 'vk_layer_settings.txt' path in the 'Vulkan Development Status'
- Add ${VK_LOCAL} and ${VK_APPDATA} built-in variables
- Add 'expanded' property to layer manifest settings and flags #1497
- Add settings environment variables checking #1498
- Add settings tree context menu #1501
- Add layer generated user documentation #1510
- Add build date in the title bar

### Improvements:
- Add link to [Vulkan Guide layers](https://github.com/KhronosGroup/Vulkan-Guide/blob/master/chapters/development_tools.md#vulkan-layers) list within the help menu
- Update built-in VUIDs to the list included in SDK 1.2.176 #1511
- Refactor the layer window to expose more layer documentation #1519
- Add shader caching setting to validation built-in UI #1552
- Add indirect parameter setting to validation built-in UI #1522
- Improve user-defined paths dialog and workflow #1523
- Improve UI labels #1551

### Fixes:
- Fix message box with no title displayed on macOS #1547
- Hide excluded layers in configuration that are missing, these may not be supported on the platform #1551
- Fix crash when renaming a configuration from the layers window #1551
- Fix validation layer bool setting written in 'vk_layer-setting.txt' #1551
- Fix upgrade of 'vkcube' path when the path is no longer valid

## [Vulkan Configurator 2.3.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.176.0) - Mai 2021

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

## [Vulkan Configurator 2.2.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.170.0) - March 2021

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

## [Vulkan Configurator 2.1.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.162.1) - January 2021

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

## [Vulkan Configurator 2.0.3](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.162.0) - December 2020

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

## [Vulkan Configurator 2.0.2](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.154.0) - October 2020

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

## [Vulkan Configurator 2.0.1](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.148.1) - August 2020 (revision 1)

### Fixes:
- Fix *Shader Printf* built-in configuration that was setup for *GPU-Assisted* validation isntead
- Fix layer configurations selection on Linux
- Fix debug output setting not working

## [Vulkan Configurator 2.0.0](https://github.com/LunarG/VulkanTools/tree/sdk-1.2.148.0) - August 2020

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
