<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

Copyright &copy; 2015-2019 LunarG, Inc.

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/


# VK\_LAYER\_LUNARG\_api\_dump
The `VK_LAYER_LUNARG_api_dump` utility layer prints API calls, parameters,
and values to the identified output stream.
It has several settings that can be adjusted by either environment variables
or by using the `vk_layer_settings.txt` file.

<br></br>


## Enabling the Layer

### Desktop (Linux/Windows/MacOS)

You must add the location of the generated `VK_LAYER_LUNARG_api.json` file and corresponding
`VkLayer_api_dump` library to your `VK_LAYER_PATH` in order for the Vulkan loader to be able
to find the layer.

Then, you must also enable the layer in one of two ways:

 * Directly in your application using the layer's name during vkCreateInstance
 * Indirectly by using the `VK_INSTANCE_LAYERS` environment variable.

#### Setting the `VK_LAYER_PATH`

**Windows**

If your source was located in: `C:\my_folder\vulkantools` and your build folder was build64, then you would add it to the layer path in the following way:

    set VK_LAYER_PATH=C:\my_folder\vulkantools\build64\layersvt\Debug;%VK_LAYER_PATH%

**Linux/MacOS**

If your source was located in: `/my_folder/vulkantools` and your build folder was build, then you would add it to the layer path in the following way:

    export VK_LAYER_PATH=/my_folder/vulkantools/build/layersvt:$VK_LAYER_PATH

Forcing the layer with `VK_INSTANCE_LAYERS`

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


## API Dump Options

Setting  | Environment Variable | Settings File Value | Default | Description
-------- | -------------------- | ------------------- | ------- | -----------
Enable File Output | None: Implicitly defined when filename defined | `lunarg_api_dump.file` | Not Set | Force output of API Dump layer to be written to a file instead of `stdout`.
Output File Name | `VK_APIDUMP_LOG_FILENAME` | `lunarg_api_dump.log_filename` | stdout | The name (and path) of the output file to save the generated content to.
Detailed Output | `VK_APIDUMP_DETAILED` | `lunarg_api_dump.detailed` | true | Generate more detailed output of the commands including parameters and values.  If `false` only output function signature.
No Addresses/Handles | `VK_APIDUMP_NO_ADDR` | `lunarg_api_dump.no_addr` | false | Generate output without addresses or handles (which can vary run to run. Instead use the placeholder value "address".
Flush After Every Command | `VK_APIDUMP_FLUSH` | `lunarg_api_dump.flush` | true | Flush after every API command's output
Output format | `VK_APIDUMP_OUTPUT_FORMAT` | `lunarg_api_dump.output_format` | `text` | Output the API Dump information as a text file (`text`), an HTML-formated file (`html`), or a json file (`json`).
Selective Output Range | `VK_APIDUMP_OUTPUT_RANGE` | `lunarg_api_dump.output_range` | `0-0` | Only output frames within the specified range. Given by a comma separated list of frames or a range with a start, count, and optional interval separated by dashes. A count of 0 will output every frame after the start of the range. Example: "5-8-2" will output frame 5, continue until frame 13, dumping every other frame. Example: "3,8-2" will output frames 3, 8, and 9.
Show Timestamps | `VK_APIDUMP_TIMESTAMP` | `lunarg_api_dump.show_timestamp` | false | Show the timestamp of function calls since start in microseconds

### Settings Priority

If you have a setting defined in both the Settings File as well as an Environment
Variable, the Environment Variable value will **always** override the Settings File
value.
This is intended to let you dynamically change settings without having to adjust
the Settings File.

<br></br>


### Applying Environment Settings on Android

On Android, you must use properties to set the environment variables.
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

### Settings File Specific Settings

At this time, certain settings are only available to be set in the `vk_layer_settings.txt` file.  These are:

Setting  | Settings File Value | Default | Description
-------- | ------------------- | ------- | -----------
Indent Size | `lunarg_api_dump.indent_size` | 4 | Set the indent size for writing out parameters and values for each command.  Only valid for `text` format and `stdout` writing.
Name Size | `lunarg_api_dump.name_size` | 32 | Set the max length to assume for written names.  This is intended to allow cleaner indenting by reserving space for names shorter than this length.  A value of 0 means no additional spacing applied.  Only valid when "Use Spaces" is enabled.
Show Shader | `lunarg_api_dump.show_shader` | false | Output the contents of any shader file loaded.
Show Types | `lunarg_api_dump.show_types` | true | Output the types for each setting.
Type Size | `lunarg_api_dump.type_size` | 0 | Set the max length to assume for written types.  This is intended to allow cleaner indenting by reserving space for types shorter than this length.  A value of 0 means no additional spacing applied.  Only valid when "Use Spaces" is enabled.
Use Spaces| `lunarg_api_dump.use_spaces` | true | Attempt to use additional white space to produce a cleaner/easier-to-read output.
Show Thread And Frame | `lunarg_api_dump.show_thread_and_frame` | true | Show the thread and frame of each function called.
