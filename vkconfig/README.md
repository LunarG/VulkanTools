<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright &copy; 2015-2022 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width=400 /></p>

*Vulkan Configurator* allows overriding the [layers configuration](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md#layers) used by Vulkan applications at runtime.

A Vulkan application may configure layers when creating a Vulkan Instance. This layers configuration may be overridden using *Vulkan Configurator* globally or for a selected list of Vulkan applications.

Finally *Vulkan Configurator* allows using layers from user-defined directories.

* **[Change Log](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/CHANGELOG.md)**: The history of *Vulkan Configurator* releases.
* **[Bug reports](https://github.com/LunarG/VulkanTools/issues)**: Open a GitHub issue when you encounter a bug.
* **[Roadmap](https://github.com/LunarG/VulkanTools/projects/2)**: Follow *Vulkan Configurator* future developments.
* **[Contributing](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/CONTRIBUTING.md)**: Submit a fix or a feature to *Vulkan Configurator*.

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

Vulkan Configurator is a graphical user interface (GUI) that may be launched from the console using `vkconfig`.

The tool is distributed differently, depending on the platform:
- Ubuntu packages: Upon installing the `lunarg-vkconfig` package, the tools will be available from the command line as `vkconfig`.
- Linux tarball: The vkconfig executable comes pre-built; be sure Qt is installed to run it. Run vkconfig on the command line.
  - On Ubuntu 20.04 Qt can be installed by running `sudo apt install qt5-default`
- Windows: The tool will be present on the start menu, in the Vulkan SDK menu. User can also invoke from a command line.
- macOS: The tool is provided as an application bundle. Run it by double-clicking the bundle from a Finder window.

Vulkan Configurator may be used with command line arguments to averride layers. Use `vkconfig --help` in the console for more information.

![Vulkan Configurator Animated Presentation](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/presentation.gif)


### Selecting an Application

The typical use-case for VkConfig is to make changes and run applications while the tool is still open so
that any changes made are not carried over when the tool exits.
Because of this, VkConfig has an application launcher built in which allows you to select which
application to run.

![Application Selection](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vkconfig_applications_collapsed.png)

Select between any previously-defined applications by clicking the down-facing error on the right of
the application line.
The application selection can also be expanded so the application name, executable path, command-line
arguments, and even the log file can be edited.
To edit these fields in-place, expand the application area by simply expanding the arrow to the left of
the word "Application".

![Application Selection Expanded](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vkconfig_applications_expanded.png)

To add or further edit an application in the launcher, click the 3 periods (`...`) to the far right
of the application field.
This will expand to an application dialog where applications can be added, edited, or removed.

![Application Dialog](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vkconfig_applications_dialog.png)


### Applications Listed by Name

Applications are now listed by name so the same application can be added with different names and
different command-line arguments.

For example, "Rise of the Tomb Raider" and "Half-Life 2" are both available through Valve's Steam
store.
In order to run both application's, the `steam` executable must first be run, but with different
arguments.
Because applications are now listed by name, it is now possible to add both to the Application list so
they can be triggered individually.
This removes the need to edit the command-line when switching between them previously.

This may also prove useful if running the same application with one or more different command-line
arguments.


--------------
## Terminology

***[Vulkan Layer](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md#layers)***: A layer is an optional library that can intercept Vulkan functions on their way from the Vulkan application down to the Vulkan drivers. Multiple layers can be chained together to use multiple layer functionalities simultaneously.

***Vulkan Layers Configuration***: A collection of Vulkan Layers executed in [a specific order](#vulkan-layers-execution-order-overview) with specific settings for each layer.

***[Vulkan Explicit Layers vs Implicit Layer](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md#implicit-vs-explicit-layers)***: An explicit layer has to be explicitly activated by the user from source in `vkCreateInstance`, using Vulkan Configurator or `VK_INSTANCE_LAYERS` environment variable. Implicit layers are enabled by their existence on the system by default.

***[Vulkan Meta-Layer](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md#meta-layers)***: Meta-layers are a special kind of layer which is only available through the desktop [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader). While usual layers are associated with one particular library, a meta-layer is actually a collection layer which contains an ordered list of other layers called *component layers*. 

***Vulkan Override Layer***: The Vulkan Override Layer is an implicit meta-layer found on the system with the name `VK_LAYER_LUNARG_override`. It is the mechanism used by *Vulkan Configurator* to override Vulkan applications layers. This layer contains:
- The ordered list of layers to activate
- The list of layers to exclude from execution
- The list of paths to executables that the layers override applies to. If this list is empty, the override is applied to every application upon startup.

***Vulkan Layer settings***: Per-layer settings loaded by each layer library and stored in the `vk_layer_settings.txt` file. This file is located either next to the Vulkan application executable or set globally and applied to all Vulkan applications thanks to *Vulkan Configurator*. These settings are described [here for VK_LAYER_KHRONOS_validation](https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/layers/vk_layer_settings.txt) and [here for other layers created by LunarG](https://github.com/LunarG/VulkanTools/blob/master/layersvt/vk_layer_settings.txt).

--------------
## Vulkan Layers execution order overview

[ ![Vulkan Loader](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vulkan_layers_order_small.png) ](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vulkan_layers_order.png)

--------------
## Vulkan Loader and Layers implementation overview

[ ![Vulkan Loader](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vulkan_loader_640px.png) ](https://github.com/LunarG/VulkanTools/blob/master/vkconfig/images/vulkan_loader.png)

For detailed information, read the [Architecture of the Vulkan Loader Interfaces](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md) document.

## Vulkan Layers Settings

### `vk_layer_settings.txt`

The layer settings are stored into the `vk_layer_settings.txt` file and read directly by the layers.

When *Vulkan Configurator is used to override layers, the Vulkan application local `vk_layer_settings.txt` file is ignored by layer ecosystem convention.

*Vulkan Configurator* does not make any system-wide changes to a system, but it does make user-specific changes.
These changes are documented below:

#### Linux and macOS

Unix systems store files in the following paths:

- `$HOME/.local/share/vulkan/implicit_layer.d/VkLayer_override.json` tells a Vulkan application which layers to use
- `$HOME/.local/share/vulkan/settings.d/vk_layer_settings.txt` tells Vulkan layers which settings to use

#### Windows

Windows systems store files in the following paths:

- `%HOME%\AppData\Local\LunarG\vkconfig\override\VkLayerOverride.json` tells a Vulkan application which layers to use
- `%HOME%\AppData\Local\LunarG\vkconfig\override\vk_layer_settings.txt` tells Vulkan layers which settings to use

In addition, Windows system create registry entries in the following locations:

- `HKEY_CURRENT_USER\Software\Khronos\Vulkan\ImplicitLayers` will have an entry that points to the JSON file above
- `HKEY_CURRENT_USER\Software\Khronos\Vulkan\Settings` will have an entry that points to the text file above
- `HKEY_CURRENT_USER\Software\LunarG\vkconfig` stores the application settings for `vkconfig`

### Environment Variables (Deprecated)

OS Environment Variables are the legacy approach to configure layers however they should be considered deprecated in favor of using `vk_layer_settings.txt` either locally, next to the Vulkan Applications or globally, generated from *Vulkan Configurator*.

