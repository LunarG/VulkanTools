<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>

Copyright &copy; 2015-2022 LunarG, Inc.

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/


# VK\_LAYER\_LUNARG\_api\_dump
The `VK_LAYER_LUNARG_api_dump` utility layer prints API calls, parameters,
and values to the identified output stream.

<br></br>


## Configuring the API Dump Layer

For an overview of how to configure layers, refer to the [Layers Overview and Configuration](https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_configuration.html) document.

The API Dump Layer settings are documented in the [Layer Details](https://vulkan.lunarg.com/doc/sdk/latest/windows/api_dump_layer.html#user-content-layer-details) section below.

The API Dump Layer can also be enabled and configured using vkconfig. See the [vkconfig](https://vulkan.lunarg.com/doc/sdk/latest/windows/vkconfig.html) documentation for more information.


## Enabling the API Dump Layer

### Desktop (Linux/Windows/MacOS)

You must add the location of the `VK_LAYER_LUNARG_api_dump.json` file and corresponding
`VkLayer_api_dump` library to your `VK_LAYER_PATH` in order for the Vulkan loader to be able
to find the layer.

Then, you must also enable the layer in one of two ways:

 * Directly in your application using the layer's name during vkCreateInstance
 * Indirectly by using the `VK_INSTANCE_LAYERS` environment variable.

#### Setting `VK_LAYER_PATH`

**Windows**

If your source was located in: `C:\my_folder\vulkantools` and your build folder was build64, then you would add it to the layer path in the following way:

    set VK_LAYER_PATH=C:\my_folder\vulkantools\build64\layersvt\Debug;%VK_LAYER_PATH%

**Linux/MacOS**

If your source was located in: `/my_folder/vulkantools` and your build folder was build, then you would add it to the layer path in the following way:

    export VK_LAYER_PATH=/my_folder/vulkantools/build/layersvt:$VK_LAYER_PATH

#### Enabling the layer with `VK_INSTANCE_LAYERS`

To force the layer to be enabled for Vulkan applications, you can set the `VK_INSTANCE_LAYERS` environment variable in the following way:

**Windows**

    set VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_api_dump

**Linux/MacOS**

    export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_api_dump

<br></br>

### Android

#### Permissions

You may need to grant permissions to your application to write to system storage, even if it normally does not.
This depends on whether or not you modify API Dump to output to a file or just leave it to output to text.

If you're building with Android Studio, you do this by:

 * Click on "Run" in the menu
 * Choose "Edit Configurations..."
 * In the dialog box, look for the "Install Flags:" text box
 * Enter -g
 * Click "Apply"

If this does not work, you may still require enabling permissions for your application from the settings menu.

Failure to do so will result in your application crashing during vkCreateInstance since the layer will attempt,
but fail, to create the capture file.

#### Globally Enabling the Layer

Use ADB to enable the layer for your project by:

    adb shell "setprop debug.vulkan.layers 'VK_LAYER_LUNARG_api_dump'"

When done, disable the layer using:

    adb shell "setprop debug.vulkan.layers ''"

<br></br>

### Applying Environment Settings on Android

On Android, you must use properties to set the layer environment variables.
The format of the properties to set takes the following form:

    debug. + (lower-case environment variable with 'vk_' stripped)

The easiest way to set a property is from the ADB shell:

    adb shell "setprop <property_name> '<property_value>'"

**For example:**

To set the API Dump output log filename, which on desktop uses `VK_APIDUMP_LOG_FILENAME`
set the following property:

    debug.apidump_log_filename

Which you can set in the following way:

    adb shell "setprop debug.apidump_log_filename '/sdcard/Android/vk_apidump.txt'"

<br></br>


## Layer Options

The options for this layer are specified in VK_LAYER_LUNARG_api_dump.json. The option details are in [api_dump_layer.html](https://vulkan.lunarg.com/doc/sdk/latest/windows/api_dump_layer.html#user-content-layer-details).
