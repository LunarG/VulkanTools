<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright &copy; 2015-2025 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

<p align="center"><img src="./images/vulkan_configurator.png" width=400 /></p>

--------------
## Using Vulkan Configurator

Part of the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home), *Vulkan Configurator* is a tool designed to help Vulkan application developers configuring their Vulkan system: The *Vulkan Loader*, *Vulkan Layers* and *Vulkan drivers*.

It's a graphical user interface (GUI) that may be launched from the console using `vkconfig-gui` and a command line tool `vkconfig --gui`. *Vulkan Configurator* may be used with command line arguments to configure Vulkan Loader. Use `vkconfig --help` in the console for more information.

* **[Manual](https://github.com/KhronosGroup/Vulkan-Utility-Libraries/blob/main/docs/Configuring-Vulkan-Layers-Whitepaper.pdf)**: *Configuring Vulkan Layers* whitepaper for detailed usage information and system architechture.
* **[Change Log](https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/CHANGELOG.md)**: The history of *Vulkan Configurator* releases.
* **[Bug reports](https://github.com/LunarG/VulkanTools/issues)**: Open a GitHub issue when you encounter a bug.
* **[Contributing](https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/CONTRIBUTING.md)**: Submit a fix or a feature to *Vulkan Configurator*.

--------------
## Platforms Support

| OS      | Windows            | Linux               | macOS              |
| --------| ------------------ | ------------------- | ------------------ |
| Support | :heavy_check_mark: | :heavy_check_mark:  | :heavy_check_mark: |

--------------
## Vulkan Configurator UI overview

Vulkan Configurator is designed around multiple tabs, each dedicated to specific usages.

### Vulkan Layers Configuration tab

This tab allows configuring Vulkan layers. This includes enabling and specifying the Vulkan layers order on the Vulkan developers matching. Each layer can be configure using the Vulkan Layers settings.

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/1-layers.png" width=1200 /></p>

The context menu of the layers configuration allows generating layer settings files of all types: documentation, `vk_layer_settings.txt`, environment variables scripts and `VK_EXT_layer_settings` C++ helper library.

### Vulkan Layers Paths tab

This tab allows adding additional Vulkan layers, potentially Vulkan versions of the same Vulkan layer.

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/2-layers-path.png" width=1200 /></p>

### Vulkan Drivers tab

This tab allows adding Vulkan drivers but also either sorting or forcing Vulkan physical devices to all Vulkan applications.

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/3-drivers.png" width=1200 /></p>

### Vulkan Applications tab

This tab is an application launcher enabling to quickly test Vulkan system changes.

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/4-applications.png" width=1200 /></p>

### Vulkan Diagnostics tab

This tab allows generating multiple log, including Vulkan loader log, Vulkan loader configuration, Vulkaninfo, etc. These logs can be exported and shared for examples to submit issues on Vulkan github repository.

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/5-diagnostics.png" width=1200 /></p>

### Vulkan Documentation tab

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/6-documentation.png" width=1200 /></p>

### Vulkan Preferences tab

<p align="center"><img src="https://github.com/LunarG/VulkanTools/blob/main/vkconfig_gui/images/7-preferences.png" width=1200 /></p>
