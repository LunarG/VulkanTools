# VK\_LAYER\_LUNARG\_api\_dump
The `VK_LAYER_LUNARG_api_dump` utility layer prints API calls, parameters, and values to the identified output stream.

`VK_LAYER_LUNARG_api_dump` has three custom settings that can be set to `TRUE` or `FALSE` values:


| Setting       | Description                                                     |
| ------------- |---------------------------------------------------------------- |
| `lunarg_api_dump.detailed`   | if `TRUE` (the default), dump all function parameters and values; if `FALSE`, dump only function signatures        |
| `lunarg_api_dump.file`       | dump to file; otherwise dump to `stdout`                          |
| `lunarg_api_dump.no_addr`    | if `TRUE`, replace all addresses with static string "`address`" |
| `lunarg_api_dump.flush`      | if `TRUE`, force I/O flush after every line                         |

### Android
To enable, make the following changes to vk_layer_settings.txt
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
