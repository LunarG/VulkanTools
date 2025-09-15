## Vulkan Configurator 3.4.0 - October 2025
[Vulkan SDK main](https://github.com/LunarG/VulkanTools/tree/main)

### Features:
- Add Vulkan drivers tab
  - Add force physical device
  - Add additional Vulkan drivers
- Add log search bar to Application Launcher tab
- Add code export to `VK_EXT_layer_settings` extension code for Vulkan application of layers configurations
- Add command line `settings` argument to generate:
  - `VK_EXT_layer_settings` extension code for Vulkan application of layers configurations
  - Environment variables script of layers configurations
  - `vk_layer_settings.txt` layers settings file
- Add an automatic default layer preset, to reset the layer settings to default

### Improvements:
- Clean up Vulkan Configurator command line `help`

### Fixes:
- Fix invalid downloaded SDK filename
- Fix Vulkan Loader not sending log on first application run
- Fix layer settings cursor update #2475

### Deprecation:
- Deprecate the synchronization configuration

## Vulkan Configurator 3.3.0 - July 2025
[Vulkan SDK 1.4.321.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.321)

### Features:
- Add support for layer-defined messages
- Add support for external `vk_layer_settings.txt` files
- Add environment variables export of loader configurations

### Improvements:
- Improve layer settings documentation
  - Add layer setting context menu to access setting documentation
  - Add detailed description layer setting field
  - Add display of deprecated setting information in generated layer documentation
  - Add optional information about which setting replaced a deprecated setting
  - Add layer setting dependences to generated `vk_layer_settings.txt`
- Add theme mode control in preference tab
  - Add custom alternate rows color

### Fixes:
  - Fix crashes when `QT_QPA_PLATFORM` is set
  - Fix invalid new SDK notifications on patch releases
  - Disable SDK release notification on Windows ARM due to a Qt 6.8.2 bug

## Vulkan Configurator 3.2.0 - May 2025
[Vulkan SDK 1.4.313.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.313)

### Features:
- Add dark mode, following the OS theme
- Add muliple diagnostic modes
- Add diagnostic logs export
- Add search in diagnostic tab

### Improvements:
- Add Vulkan Header version used to build in about tab
- Refactor layer version combobox
- Clean up UI layout
- Add button to remove missing layers
- Improved generated layer documentation
  - Add links within the documentation to reach detailed part of a setting documentation
  - Clarify `VK_EXT_layer_settings` variable names
  - Hide `advanced` settings, used only by layer developers
  - Add setting dependences section
  - Add support for Android system property prefix, eg `debug.vvl.validate_core` instead of `debug.vulkan.khronos_validation.validate_core`
  - Fix broken generated layer doc links

### Fixes:
- Fix layer settings all display with the layer development status
- Fix reset to default when stay in system tray is checked

## Vulkan Configurator 3.1.0 - March 2025
[Vulkan SDK 1.4.309.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.309)

### Features:
- Add `vkconfig loader` commands to import, export, delete and use stored loader configurations
- Add new Vulkan SDK notification and download

### Improvements:
- Add button to remove missing layers
- Add loader messages only configuration
- Open created files on export
- Add button to open launcher log file
- Add `vulkaninfo` as default application on macOS
- Enable text editing the executable path
- Add layer manifest validation log saving option
- Add ignoring invalid layer manifest when invalid until the file is modified

### Fixes:
- Fix application arguments and environment variables splitting
- Fix layer settings crash when reordering layers
- Fix executable launcher log speed issue

---
## Vulkan Configurator 3.0.0 - February 2025
[Vulkan SDK 1.4.304.1](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.304.1)

### Features:
- Improve layers loading and selection:
  * Add loading of multiple versions of the same layer
  * Add explicit selection of the layer version used by a configuration
  * No longer load and unload layer manifest when switching configurations
  * Json validation caching based on layer manifest modified date and time
  * Add layers manifest reloading
  * Per-layer enabling of layer settings
- Improve layers ordering:
  * Allow ordering of all (implicit layers, unknown layers) layers executed by Vulkan application
  * Allow ordering layers independently from enabling them
  * Allow ordering of layers using drag and drop
- Improve executable list:
  * Add storing multiple set of options per executable
  * Add per-application layers configuration
- Add *Vulkan Loader* logging support:
  * Add per-executable *Vulkan Loader* loader configuration
  * Add *Vulkan Loader* logging outside of application launcher
  * Add *Vulkan Loader* selection of each logging message type
- Split GUI and command line into two separate executables
- Add system diagnostic

### Improvements:
- Almost all Vulkan Configurator data is stored in a `$HOME` directory JSON file
- Fix default applications (`vkcube`) during SDK upgrade
- Fix upgrade of built-in configuration that got updated using layers configuration versioning
- Add the list of all availalble environment variables for each settings in documentation generation

### Deprecation:
- Requires Vulkan Loader 1.4.304 or newer

---
## Vulkan Configurator 2.6.3 - January 2025
[Vulkan SDK 1.4.304.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.304.0)
- Final build

---
## Vulkan Configurator 2.6.3 - January 2025
[Vulkan SDK 1.4.304.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.4.304.0)

### Deprecation:
- Final build

---
## Vulkan Configurator 2.6.2 - October 2024
[Vulkan SDK 1.3.296.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.296.0)

### Improvements:
- Show message warnings when overriding implicit layer, either disabling or enabling
- Removing layers override when closing OS session

### Fixes:
- Fix flag settings dependencies
- Fix filesystem settings update
- Fix overridden implicit layer ordering
- Fix renaming configuration with the same name but different cases

---
## Vulkan Configurator 2.6.1 - July 2024
[Vulkan SDK 1.3.290.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.290.0)

### Features:
- Redesign main window UI around tabs
- Per-Application layers configurations
- Full ordering of Vulkan layers
- Split GUI and command line tool

### Deprecation:
- Requires Vulkan Loader 1.3.261 or newer on Linux and macOS
- Requires Vulkan Loader 1.3.284 or newer on Windows.
- Highly recommand Validation layer 1.3.250 or newer.

---
## Vulkan Configurator 2.6.0 - May 2024
[Vulkan SDK 1.3.283.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.283.0)

### Features:
- Add system tray
- Add mode to disable all Vulkan layers

### Improvements:
- Add 'status' to give an explicit control to print the Vulkan status log
- Add support for using Vulkan Configurator environment variables in the UI
- Better present implicit layers, showing "Implicitly On" or "Implicitly Off" depending on the environment variable
- Add double click on configuration name to edit a layers configuration

### Fixes:
- Fix clear log when changing `Vulkan Loader Messages` type when `Clear log on action` is unchecked
- Fix spaces in application launcher command-line 'path' arguments
- Fix 'Use of application list' check box not being saved
- Clean up UI update

---
## Vulkan Configurator 2.5.6 - March 2024
[Vulkan SDK 1.3.280.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.280.0)

### Fixes:
- Fix confusing synchronization built-in configuration

---
## Vulkan Configurator 2.5.5 - January 2024
[Vulkan SDK 1.3.275.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.275.0)

### Improvements:
- Add `LOAD_FOLDER` setting type

### Fixes:
- Remove default settings values from built-in layer configurations
- Fix doc generator using default environment variable names

---
## Vulkan Configurator 2.5.4 - May 2023
[Vulkan SDK 1.3.250.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.250.0)

### Improvements:
- Display `deviceUUID` and `driverUUID` in the "Development Status" when available for each physical device

---
## Vulkan Configurator 2.5.3 - March 2023
[Vulkan SDK 1.3.243.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.243.0)

### Improvements:
- Enabling fully generic layer settings for the validation layer settings

---
## Vulkan Configurator 2.5.2 - December 2022
[Vulkan SDK 1.3.236.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.236.0)

### Improvements:
- Add `warn_on_robust_oob` validation layer setting handling

---
## Vulkan Configurator 2.5.1 - October 2022
[Vulkan SDK 1.3.231.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.231.0)

### Features:
- Add ENUM setting type children settings #1746

### Improvements:
- Allow disabling the validation layer settings built-in UI #1746

### Fixes:
- Fix override layer version when some layers are excluded or missing #1743

---
## Vulkan Configurator 2.5.0 - August 2022
[Vulkan SDK 1.3.224.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.224.0)

### Features:
- Add per layers-configuration user-defined layers paths #1711

### Improvements:
- Add reordering of configurations when renaming a configuration
- Improve handling of missing layers

---
## Vulkan Configurator 2.4.6 - June 2022
[Vulkan SDK 1.3.216.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.216.0)

### Features:
- Add `VK_KHR_portability_enumeration` extension support

### Improvements:
- Allow using multiple layers built with different Vulkan header version when Loader 212 or newer is installed on the system

---
## Vulkan Configurator 2.4.5 - April 2022
[Vulkan SDK 1.3.211.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.211.0)

### Features:
- Add `VK_ADD_LAYER_PATH` Vulkan Loader variable handling
- Remove built-in support of `VK_LAYER_LUNARG_device_simulation` layer #1628
  - Use `VK_LAYER_KHRONOS_profiles` layer instead

### Improvements:
- Improve the Portability built-in configuration to check only what's necessary, reducing validation overhead
- Refinement of the layers window UI design

---
## Vulkan Configurator 2.4.4 - March 2022
[Vulkan SDK 1.3.204.1](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.204.1)

### Features:
- Add markdown documentation export
- Add `vk_layer_settings.txt` documentation export
- Add command line support of HTML, markdown and `vk_layer_settings.txt` documentation exports

### Improvements:
- Add button to create a new layers configuration

### Fixes:
- Fix layer presets that could be modified by accident #1653
- Fix loading of profiles file from the UI #1653

---
## Vulkan Configurator 2.4.3 - February 2022
[Vulkan SDK 1.3.204.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.3.204.0)

### Improvements:
- Upgrade the built-in "Portability" to use the Profiles layer
- Display the development status of a setting next to the setting label in the UI
- Display a warning when using multiple layers built with different Vulkan Header version
  - It's a requirement by the Vulkan Loader

### Fixes:
- Fix ANY dependence when using FLAGS setting type

---
## Vulkan Configurator 2.4.2 - November 2021
[Vulkan SDK 1.2.196.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.196.0)

### Improvements:
- Discard applied layers configuration to ensure the layers never crash Vulkan Configurator

### Fixes:
- Fix 'Edit Layers' button being enabled when there is no active configuration #1616

---
## Vulkan Configurator 2.4.1 - September 2021
[Vulkan SDK 1.2.189.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.189.0)

### Fixes:
- Fix user-defined layer settings not loaded, for older Vulkan API than 1.2.176
- Fix settings written in 'vk_layer_settings.txt' even when the dependence is not met #1582

---
## Vulkan Configurator 2.4.0 - July 2021
[Vulkan SDK 1.2.182.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.182.0)

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
- Add link to [Vulkan Guide layers](https://github.com/KhronosGroup/Vulkan-Guide/blob/main/chapters/development_tools.md#vulkan-layers) list within the help menu
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

---
## Vulkan Configurator 2.3.0 - May 2021
[Vulkan SDK 1.2.176.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.176.0)

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

---
## Vulkan Configurator 2.2.0 - March 2021
[Vulkan SDK 1.2.170.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.170.0)

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

---
## Vulkan Configurator 2.1.0 - January 2021
[Vulkan SDK 1.2.162.1](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.162.1)

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

---
## Vulkan Configurator 2.0.3 - December 2020
[Vulkan SDK 1.2.162.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.162.0)

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

---
## Vulkan Configurator 2.0.2 - October 2020
[Vulkan SDK 1.2.154.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.154.0)

### Features:
- Add multiple debug actions simultaneously #1119
- Add better representation of the layers execution order #1181

### Improvements:
- Streamline and clean up GUI
- Add layers configuration description #1105
- Add selected path from dialog window cache #1107
- Add unit test framework #1100
- Add button to browse working directory #1099
- Add version in the title bar and show `<ACTIVE>` state #1109
- Add default settings state in the layer configuration settings #1076
- Make *Validation - Standard*, the default configuration #1076
- Update `vkcube` to use the version from the same *Vulkan SDK* than *Vulkan Configurator* #1112
- Expose `duplicate_message_limit` setting from `VK_LAYER_KHRONOS_validation` #1176

### Fixes:
- Fix disabled configurations that can't be reenabled or removed
- Fix display of custom layer paths #1101
- Fix inconsistent display of separators in paths #1107
- Fix display of empty entries in the custom path dialog when opening Json files that are not layer json files #1103
- Fix log file default location to `HOME` to ensure write access #1083
- Fix log file not written to #1109
- Fix `debug_output` setting displayed on Linux and macOS but not available #1086
- Fix `numeric_bool` setting display #1166

---
## Vulkan Configurator 2.0.1 - August 2020
[Vulkan SDK 1.2.148.1](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.148.1)

### Fixes:
- Fix *Shader Printf* built-in configuration that was setup for *GPU-Assisted* validation instead
- Fix layer configurations selection on Linux
- Fix debug output setting not working

---
## Vulkan Configurator 2.0.0 - August 2020
[Vulkan SDK 1.2.148.0](https://github.com/LunarG/VulkanTools/tree/vulkan-sdk-1.2.148.0)

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
