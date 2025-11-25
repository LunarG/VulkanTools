# VK_LAYER_LUNARG_reference_1_2_1

## reference layer

The main, comprehensive Khronos validation layer.

Vulkan is an Explicit API, enabling direct control over how GPUs actually work. By design, minimal error checking is done inside a Vulkan driver. Applications have full control and responsibility for correct operation. Any errors in how Vulkan is used can result in a crash. 

The Khronos Validation Layer can be enabled to assist development by enabling developers to verify their applications correctly use the Vulkan API.

## Layer Properties

- API Version: 1.4.333
- Implementation Version: Build 76
- Layer Manifest: `VK_LAYER_LUNARG_reference_1_2_1.json`
  - File Format: 1.2.1
  - Layer Binary: `VkLayer_reference.dll`
- Variables:
  - `vk_layer_settings.txt` namespace: `lunarg_reference_1_2_1`
  - Environment Variable prefix: `VK_LUNARG_REFERENCE_1_2_1_`
  - Android system property prefix: `adb shell setprop debug.vulkan.lunarg_reference_1_2_1.`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`
- Number of Layer Settings: 12
- Number of Layer Presets: 11

## Layer Settings Overview

|Label|Variables Key|Type|Default Value|Platforms|
|---|---|---|---|---|
| [toogle](#toogle) | `toogle` | `BOOL` | `true` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [enum](#enum) | `enum_required_only` | `ENUM` | `value1` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [flags](#flags) | `flags_required_only` | `FLAGS` | `flag0,flag1` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [String](#string) | `string_required_only` | `STRING` | `A string` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [bool](#bool) | `bool_required_only` | `BOOL` | `true` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [Load file](#load-file) | `load_file_required_only` | `LOAD_FILE` | `./test.txt` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [Save file](#save-file) | `save_file_required_only` | `SAVE_FILE` | `./test.json` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [Save folder](#save-folder) | `save_folder_required_only` | `SAVE_FOLDER` | `./test` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [Integer](#integer) | `int_required_only` | `INT` | `76` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [Float](#float) | `float_required_only` | `FLOAT` | `76.500000` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [Frames](#frames) | `frames_required_only` | `FRAMES` | `76-82,75` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |
| [List](#list) | `list_required_only` | `LIST` | `L,R,stringB,stringD` | `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID` |

## Layer Settings Details

### toogle

true or false.

- Key: `toogle`
- Type: `BOOL`
- Default Value: `true`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>VkBool32 data[] = { VK_TRUE };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "toogle", VK_LAYER_SETTING_TYPE_BOOL32_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.toogle = true</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true<br>export VK_REFERENCE_1_2_1_TOOGLE=true<br>export VK_TOOGLE=true<br>export VK_REF_TOGGLE=true</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.toogle true<br>adb shell setprop debug.vulkan.reference_1_2_1.toogle true<br>adb shell setprop debug.vulkan.toogle true</pre> |

### enum

enum case.

- Key: `enum_required_only`
- Type: `ENUM`
- Default Value: `value1`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

|Enum Value|Label|Description|Platforms|
|---|---|---|---|
| `value0` |Value0|My value0|`WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`|
| `value1` |Value1|My value1|`WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`|
| `value2` |Value2|My value2|`WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`|

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "value1" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "enum_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.enum_required_only = value1</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_ENUM_REQUIRED_ONLY=value1<br>export VK_REFERENCE_1_2_1_ENUM_REQUIRED_ONLY=value1<br>export VK_ENUM_REQUIRED_ONLY=value1</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.enum_required_only value1<br>adb shell setprop debug.vulkan.reference_1_2_1.enum_required_only value1<br>adb shell setprop debug.vulkan.enum_required_only value1</pre> |

### flags

flags case.

- Key: `flags_required_only`
- Type: `FLAGS`
- Default Value: `flag0,flag1`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

|Flags|Label|Description|Platforms|
|---|---|---|---|
| `flag0` |Flag0|My flag0|`WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`|
| `flag1` |Flag1|My flag1|`WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`|
| `flag2` |Flag2|My flag2|`WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`|

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "flag0,flag1" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "flags_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.flags_required_only = flag0,flag1</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_FLAGS_REQUIRED_ONLY=flag0,flag1<br>export VK_REFERENCE_1_2_1_FLAGS_REQUIRED_ONLY=flag0,flag1<br>export VK_FLAGS_REQUIRED_ONLY=flag0,flag1</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.flags_required_only flag0,flag1<br>adb shell setprop debug.vulkan.reference_1_2_1.flags_required_only flag0,flag1<br>adb shell setprop debug.vulkan.flags_required_only flag0,flag1</pre> |

### String

string.

- Key: `string_required_only`
- Type: `STRING`
- Default Value: `A string`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "A string" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "string_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.string_required_only = A string</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_STRING_REQUIRED_ONLY=A string<br>export VK_REFERENCE_1_2_1_STRING_REQUIRED_ONLY=A string<br>export VK_STRING_REQUIRED_ONLY=A string</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.string_required_only A string<br>adb shell setprop debug.vulkan.reference_1_2_1.string_required_only A string<br>adb shell setprop debug.vulkan.string_required_only A string</pre> |

### bool

true or false.

- Key: `bool_required_only`
- Type: `BOOL`
- Default Value: `true`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>VkBool32 data[] = { VK_TRUE };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "bool_required_only", VK_LAYER_SETTING_TYPE_BOOL32_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.bool_required_only = true</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_BOOL_REQUIRED_ONLY=true<br>export VK_REFERENCE_1_2_1_BOOL_REQUIRED_ONLY=true<br>export VK_BOOL_REQUIRED_ONLY=true</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.bool_required_only true<br>adb shell setprop debug.vulkan.reference_1_2_1.bool_required_only true<br>adb shell setprop debug.vulkan.bool_required_only true</pre> |

### Load file

Load file path.

- Key: `load_file_required_only`
- Type: `LOAD_FILE`
- Default Value: `./test.txt`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "./test.txt" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "load_file_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.load_file_required_only = ./test.txt</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_LOAD_FILE_REQUIRED_ONLY=./test.txt<br>export VK_REFERENCE_1_2_1_LOAD_FILE_REQUIRED_ONLY=./test.txt<br>export VK_LOAD_FILE_REQUIRED_ONLY=./test.txt</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.load_file_required_only ./test.txt<br>adb shell setprop debug.vulkan.reference_1_2_1.load_file_required_only ./test.txt<br>adb shell setprop debug.vulkan.load_file_required_only ./test.txt</pre> |

### Save file

Save file path.

- Key: `save_file_required_only`
- Type: `SAVE_FILE`
- Default Value: `./test.json`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "./test.json" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "save_file_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.save_file_required_only = ./test.json</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_SAVE_FILE_REQUIRED_ONLY=./test.json<br>export VK_REFERENCE_1_2_1_SAVE_FILE_REQUIRED_ONLY=./test.json<br>export VK_SAVE_FILE_REQUIRED_ONLY=./test.json</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.save_file_required_only ./test.json<br>adb shell setprop debug.vulkan.reference_1_2_1.save_file_required_only ./test.json<br>adb shell setprop debug.vulkan.save_file_required_only ./test.json</pre> |

### Save folder

Save folder path.

- Key: `save_folder_required_only`
- Type: `SAVE_FOLDER`
- Default Value: `./test`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "./test" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "save_folder_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.save_folder_required_only = ./test</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_SAVE_FOLDER_REQUIRED_ONLY=./test<br>export VK_REFERENCE_1_2_1_SAVE_FOLDER_REQUIRED_ONLY=./test<br>export VK_SAVE_FOLDER_REQUIRED_ONLY=./test</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.save_folder_required_only ./test<br>adb shell setprop debug.vulkan.reference_1_2_1.save_folder_required_only ./test<br>adb shell setprop debug.vulkan.save_folder_required_only ./test</pre> |

### Integer

Integer Description.

- Key: `int_required_only`
- Type: `INT`
- Default Value: `76`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>int32_t data[] = { 76 };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "int_required_only", VK_LAYER_SETTING_TYPE_INT32_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.int_required_only = 76</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_INT_REQUIRED_ONLY=76<br>export VK_REFERENCE_1_2_1_INT_REQUIRED_ONLY=76<br>export VK_INT_REQUIRED_ONLY=76</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.int_required_only 76<br>adb shell setprop debug.vulkan.reference_1_2_1.int_required_only 76<br>adb shell setprop debug.vulkan.int_required_only 76</pre> |

### Float

Float Description.

- Key: `float_required_only`
- Type: `FLOAT`
- Default Value: `76.500000`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>float data[] = { 76.500000 };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "float_required_only", VK_LAYER_SETTING_TYPE_FLOAT32_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.float_required_only = 76.500000</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_FLOAT_REQUIRED_ONLY=76.500000<br>export VK_REFERENCE_1_2_1_FLOAT_REQUIRED_ONLY=76.500000<br>export VK_FLOAT_REQUIRED_ONLY=76.500000</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.float_required_only 76.500000<br>adb shell setprop debug.vulkan.reference_1_2_1.float_required_only 76.500000<br>adb shell setprop debug.vulkan.float_required_only 76.500000</pre> |

### Frames

Frames Description.

- Key: `frames_required_only`
- Type: `FRAMES`
- Default Value: `76-82,75`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>uint32_t data[] = { 76-82,75 };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "frames_required_only", VK_LAYER_SETTING_TYPE_UINT32_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.frames_required_only = 76-82,75</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_FRAMES_REQUIRED_ONLY=76-82,75<br>export VK_REFERENCE_1_2_1_FRAMES_REQUIRED_ONLY=76-82,75<br>export VK_FRAMES_REQUIRED_ONLY=76-82,75</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.frames_required_only 76-82,75<br>adb shell setprop debug.vulkan.reference_1_2_1.frames_required_only 76-82,75<br>adb shell setprop debug.vulkan.frames_required_only 76-82,75</pre> |

### List

List description.

- Key: `list_required_only`
- Type: `LIST`
- Default Value: `L,R,stringB,stringD`
- Platforms: `WINDOWS_X86`, `WINDOWS_ARM`, `LINUX`, `ANDROID`

| Methods: | |
| --- | --- |
| `VK_EXT_layer_settings`: | <pre>const char* data[] = { "L,R,stringB,stringD" };<br>VkLayerSettingEXT setting {<br>&emsp;"VK_LAYER_LUNARG_reference_1_2_1", "list_required_only", VK_LAYER_SETTING_TYPE_STRING_EXT,<br>&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |
| `vk_layer_settings.txt`: | <pre>lunarg_reference_1_2_1.list_required_only = L,R,stringB,stringD</pre> |
| Environment variables: | <pre>export VK_LUNARG_REFERENCE_1_2_1_LIST_REQUIRED_ONLY=L,R,stringB,stringD<br>export VK_REFERENCE_1_2_1_LIST_REQUIRED_ONLY=L,R,stringB,stringD<br>export VK_LIST_REQUIRED_ONLY=L,R,stringB,stringD</pre> |
| Android system properties: | <pre>adb shell setprop debug.vulkan.lunarg_reference_1_2_1.list_required_only L,R,stringB,stringD<br>adb shell setprop debug.vulkan.reference_1_2_1.list_required_only L,R,stringB,stringD<br>adb shell setprop debug.vulkan.list_required_only L,R,stringB,stringD</pre> |

## Layer Presets

### Default



#### Preset Setting Values:
- toogle: true
- enum: value1
- enum: value1
- flags: flag0,flag1
- flags: flag0,flag1
- String: A string
- String: A string
- bool: true
- bool: true
- Load file: ./test.txt
- Load file: ./test.json
- Save file: ./test.json
- Save file: ./test.json
- Save folder: ./test
- Save folder: ./test
- Integer: 76
- Integer: 76
- Float: 76.500000
- Float: 76.500
- Frames: 76-82,75
- Frames: 76-82,75
- List: 76,82,stringB,stringD
- List: 76,82,stringB,stringD
- List: 

### Preset Enum

Description Enum

#### Preset Setting Values:
- enum: value2
- enum: value2

### Preset Flags

Description Flags

#### Preset Setting Values:
- flags: flag0,flag2
- flags: flag0,flag2

### Preset String

Description String

#### Preset Setting Values:
- String: Required Only
- String: With Optional

### Preset Bool

Description Bool

#### Preset Setting Values:
- bool: true
- bool: false

### Preset Load File

Description Load File

#### Preset Setting Values:
- Load file: ./text.log
- Load file: ./text.log

### Preset Save File

Description Save File

#### Preset Setting Values:
- Save file: ./text.log
- Save file: ./text.log

### Preset Save Folder

Description Save Folder

#### Preset Setting Values:
- Save folder: ./text.log
- Save folder: ./text.log

### Preset Int

Description Int

#### Preset Setting Values:
- Integer: 75
- Integer: 77

### Preset Frames

Description Frames

#### Preset Setting Values:
- Frames: 13-17,24-32
- Frames: 13-17,24,32

### Preset List

Description List

#### Preset Setting Values:
- List: stringA
- List: stringA

