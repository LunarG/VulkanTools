<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

Copyright &copy; 2015-2019 LunarG, Inc.

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/


# VK\_LAYER\_LUNARG\_api\_dump
The `VK_LAYER_LUNARG_api_dump` utility layer prints API calls, parameters, and values to the identified output stream.

`VK_LAYER_LUNARG_api_dump` has several settings available that can be set using `vkconfig`
or through an Android layer settings file.

| `vkconfig` Setting Name | Layer Settings Name | Default Value | Description |
| ----------------------- | ------------------- | ------------- | ----------- |
| Output to File          | `lunarg_api_dump.file` | `FALSE`    | Whether to send output to a file, or to stdout |
| Log Filename            | `lunarg_api_dump.log_filename` | `vk_apidump.txt` | The name of a filename where to store the output.  Not used unless **Output to File** is set to `TRUE` |
| Detailed Output         | `lunarg_api_dump.detailed` | `TRUE` | If `TRUE`, dump all function parameters and values; otherwise, only dump function signatures |
| Flush                   | `lunarg_api_dump.flush` | `TRUE` | Whether to force an I/O flush after writing each output line |
| Hide Addresses          | `lunarg_api_dump.no_addr` | `FALSE` | If `TRUE`, replace all addresses with the static string `address` |
| Indent Size             | `lunarg_api_dump.indent_size` | 4 | How many spaces to use for each level of indentation |
| Name Size               | `lunarg_api_dump.name_size`   | 32 | The minimum field length of a function name |
| Output Format           | `lunarg_api_dump.output_format` |  `Text` | `Text` for normal text output, or `Html` for HTML-formatted output |
| Show Shader             | `lunarg_api_dump.show_shader` | `FALSE` | Currently unused |
| Show Types              | `lunarg_api_dump.show_types`  | `TRUE`  | Whether to show type names|
| Type Size               | `lunarg_api_dump.type_size`   | 0  | The minimum field length of a type name, if **Show Types** is `TRUE`; unused if **Show Types** is `FALSE`|
| Use Spaces              | `lunarg_api_dump.use_spaces`  | `TRUE` | If `TRUE`, use spaces to separate fields; if `FALSE`, use tabs|


### Android
To enable, make the following changes to `vk_layer_settings.txt`:
```
-lunarg_api_dump.file = FALSE
+lunarg_api_dump.file = TRUE

-lunarg_api_dump.log_filename = stdout
+lunarg_api_dump.log_filename = /sdcard/Android/vk_apidump.txt
```
Then:
```
adb push vk_layer_settings.txt /sdcard/Android
```
