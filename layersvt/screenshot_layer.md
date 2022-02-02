<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright © 2015-2022 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

# VK\_LAYER\_LUNARG\_screenshot
The `VK_LAYER_LUNARG_screenshot` layer records frames to image files. The layer can easily be enabled and configured using the [Vulkan Configurator](https://vulkan.lunarg.com/doc/sdk/latest/windows/vkconfig.html) included with the Vulkan SDK. On desktops (Linux/Windows/MacOS), you can also manually enable the layer by setting the enviroment variable VK_INSTANCE_LAYERS to VK_LAYER_LUNARG_screenshot.

## Android

Frame numbers can be specified with the debug.vulkan.screenshot property:

```
adb shell setprop debug.vulkan.screenshot <framenumbers>
```

The directory in which to create the image files can be specified with the debug.vulkan.dir property:

```
adb shell setprop debug.vulkan.dir <directory>
```
If debug.vulkan.dir is not set or it is set to an empty string, the value of debug.vulkan.dir will default to "/sdcard/Android".

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

The options for this layer are specified in VK_LAYER_LUNARG_screenshot.json. The option details are in [screenshot_layer.html](https://vulkan.lunarg.com/doc/sdk/latest/windows/screenshot_layer.html#user-content-layer-details).
