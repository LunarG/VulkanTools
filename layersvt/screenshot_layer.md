<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright &copy; 2015-2022 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

# VK\_LAYER\_LUNARG\_screenshot
The `VK_LAYER_LUNARG_screenshot` layer records frames to image files.

## Configuring the Screenshot Layer

For an overview of how to configure layers, refer to the [Layers Overview and Configuration](https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_configuration.html) document.

The Screenshot Layer settings are documented in the [Layer Details](https://vulkan.lunarg.com/doc/sdk/latest/windows/screenshot_layer.html#user-content-layer-details) section below.

The Screenshot Layer can also be enabled and configured using the [Vulkan Configurator](https://vulkan.lunarg.com/doc/sdk/latest/windows/vkconfig.html) included with the Vulkan SDK.


## Android

Frame numbers can be specified with the debug.vulkan.screenshot property:

```
adb shell setprop debug.vulkan.screenshot <framenumbers>
```

The directory in which to create the image files can be specified with the debug.vulkan.screenshot.dir property:

```
adb shell setprop debug.vulkan.screenshot.dir <directory>
```
If debug.vulkan.screenshot.dir is not set or it is set to an empty string, the value of debug.vulkan.screenshot.dir will default to "/sdcard/Android".

For production builds, if the files are to be written to external storage, make sure your application is able to read and write external storage by adding the following to AndroidManifest.xml:

```xml
<!-- This allows writing log files to sdcard -->
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
```

You may also need to grant it access with package manager.  For example, using the
Vulkan Cube demo from [Khronos/Vulkan-Tools](https://github.com/KhronosGroup/Vulkan-Tools):

```
adb shell pm grant com.example.VkCube android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.VkCube android.permission.WRITE_EXTERNAL_STORAGE
```
## Layer Options

The options for this layer are specified in VK_LAYER_LUNARG_screenshot.json. The layer option details are documented in the [screenshot layer documentation](https://vulkan.lunarg.com/doc/sdk/latest/windows/screenshot_layer.html#user-content-layer-details).
