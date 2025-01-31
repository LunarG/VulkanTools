<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright &copy; 2015-2025 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width=400 /></p>

[Vulkan layers](https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderInterfaceArchitecture.md#layers) allow application developers to add functionality to Vulkan applications without modifying the application itself, e.g.: validating API usages, dumping API entry points or generating screenshots of specified frames.

Configuring layers means multiple tasks:
* Enabling layers, selecting which layer will be executed
* Ordering layers, choosing the order of the layer execution between the Vulkan application and the Vulkan driver.
* Configuring each layer capabilities

[Vulkan layers can be configured using three different methods](https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html) to match specific Vulkan developers' workflows:
* Using Vulkan Loader and Vulkan Layers environment variables.
* Using the Vulkan API: `vkCreateInstance()` and the `VK_EXT_layer_settings` extension since *Vulkan 1.3.272*.
* Using Vulkan Loader settings and the `vk_layer_settings.txt` file using *Vulkan Configurator* since *Vulkan 1.3.284*.

*Vulkan Configurator* allows configuring [Vulkan layers](https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderInterfaceArchitecture.md#layers) used by Vulkan applications at runtime.

A Vulkan application may configure layers when creating a Vulkan Instance or using environment variables. This layers configuration may be overridden using *Vulkan Configurator* globally or for a selected list of Vulkan applications.

*Vulkan Configurator 3* requires *Vulkan Loader 1.4.304* or newer to run.

* **[Change Log](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/CHANGELOG.md)**: The history of *Vulkan Configurator* releases.
* **[Bug reports](https://github.com/LunarG/VulkanTools/issues)**: Open a GitHub issue when you encounter a bug.
* **[Contributing](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/CONTRIBUTING.md)**: Submit a fix or a feature to *Vulkan Configurator*.

--------------
## Platforms Support

| OS      | Windows            | Linux               | macOS              | iOS     | Android |
| --------| ------------------ | ------------------- | ------------------ | ------- | ------- |
| Support | :heavy_check_mark: | :heavy_check_mark:  | :heavy_check_mark: | :x:     | :x:     |

--------------
## Downloads

*Vulkan Configurator* is delivered with the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home).

--------------
## Using the Vulkan Configurator

Vulkan Configurator is mainly a graphical user interface (GUI) that may be launched from the console using `vkconfig_gui` and a command line tool `vkconfig --gui`.

The tool is distributed differently, depending on the platform:
- Ubuntu packages: Upon installing the `lunarg-vkconfig` package.
- Linux tarball: The vkconfig executable comes pre-built; be sure Qt is installed to run it.
  - On Ubuntu 20.04 Qt can be installed by running `sudo apt install qt5-default`
- Windows: The tool will be present on the start menu, in the Vulkan SDK menu.
- macOS: The tool is provided as an application bundle. Run it by double-clicking the bundle from a Finder window.

Vulkan Configurator may be used with command line arguments to override layers. Use `vkconfig --help` in the console for more information.

![Vulkan Configurator Animated Presentation](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/images/presentation.gif)

--------------
## Terminology

***[Vulkan Layer](https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderInterfaceArchitecture.md#layers)***: A layer is an optional library that can intercept Vulkan functions on their way from the Vulkan application down to the Vulkan drivers. Multiple layers can be chained together to use multiple layer functionalities simultaneously.

***Vulkan Layers Configuration***: A collection of Vulkan Layers executed in [a specific order](#vulkan-layers-execution-order-overview) with specific settings for each layer.

***[Vulkan Explicit Layers vs Implicit Layer](https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderApplicationInterface.md#implicit-vs-explicit-layers)***: An explicit layer has to be explicitly activated by the user from source in `vkCreateInstance`, using Vulkan Configurator or `VK_INSTANCE_LAYERS` environment variable. Implicit layers are enabled by their existence on the system by default.

***[Vulkan Meta-Layer](https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderLayerInterface.md#override-meta-layer)***: Meta-layers are a special kind of layer which is only available through the desktop [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader). While usual layers are associated with one particular library, a meta-layer is actually a collection layer which contains an ordered list of other layers called *component layers*. 

***Vulkan Override Layer***: The Vulkan Override Layer is an implicit meta-layer found on the system with the name `VK_LAYER_LUNARG_override`. It is the mechanism used by *Vulkan Configurator* to override Vulkan applications layers. This layer contains:
- The ordered list of layers to activate
- The list of layers to exclude from execution
- The list of paths to executables that the layers override applies to. If this list is empty, the override is applied to every application upon startup.

***Vulkan Layer settings***: Per-layer settings loaded by each layer library and stored in the `vk_layer_settings.txt` file. This file is located either next to the Vulkan application executable or set globally and applied to all Vulkan applications thanks to *Vulkan Configurator*. These settings are described [here for VK_LAYER_KHRONOS_validation](https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/main/layers/vk_layer_settings.txt) and [here for other layers created by LunarG](https://github.com/LunarG/VulkanTools/blob/main/layersvt/vk_layer_settings.txt).

https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html

--------------
## Vulkan Layers execution order overview

[ ![Vulkan Loader](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/images/vulkan_layers_order_small.png) ](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/images/vulkan_layers_order.png)

--------------
## Vulkan Loader and Layers implementation overview

[ ![Vulkan Loader](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/images/vulkan_loader_640px.png) ](https://github.com/LunarG/VulkanTools/blob/main/vkconfig/images/vulkan_loader.png)

For detailed information, read the [Architecture of the Vulkan Loader Interfaces](https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderInterfaceArchitecture.md) document.

## Vulkan Loader and Layers Settings

The loader settings are stored into the `vk_loader_settings.json` file. 

The layers settings are stored into the `vk_layer_settings.txt` file and read directly by the layers.

When *Vulkan Configurator* is used to override layers, the Vulkan application local `vk_layer_settings.txt` file is ignored by layer ecosystem convention.

*Vulkan Configurator* does not make any system-wide changes to a system, but it does make user-specific changes. These changes are documented below:

### Linux and macOS

Unix systems store files in the following paths:

- `$HOME/.local/share/vulkan/loader_settings.d/vk_loader_settings.json` specifies the enabled layers and their execution order to the Vulkan Loader
- `$HOME/.local/share/vulkan/settings.d/vk_layer_settings.txt` tells Vulkan layers which settings to use

### Windows

Windows systems store files in the following paths:

- `%HOME%\AppData\Local\LunarG\vulkan\vk_loader_settings.json` specifies the enabled layers and their execution order to the Vulkan Loader
- `%HOME%\AppData\Local\LunarG\vkconfig\override\vk_layer_settings.txt` tells Vulkan layers which settings to use

Windows system has registry entries in the following locations:

- `HKEY_CURRENT_USER\Software\Khronos\Vulkan\ImplicitLayers` which locates `vk_loader_settings.json`
- `HKEY_CURRENT_USER\Software\Khronos\Vulkan\Settings` which locates the global `vk_layer_settings.txt`


