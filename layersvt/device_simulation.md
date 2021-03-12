<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright © 2015-2021 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

# VK\_LAYER\_LUNARG\_device\_simulation

## Overview

### Extend your Vulkan test coverage with the LunarG Device Simulation Layer
The LunarG Device Simulation layer helps test across a wide range of hardware capabilities without requiring a physical copy of every device. It can be applied without modifying any application binaries, and in a fully automated fashion. The Device Simulation layer (aka DevSim) is a Vulkan layer that can override the values returned by your application’s queries of the GPU. DevSim uses a JSON text configuration file to make your application see a different driver/GPU than is actually in your system. This capability is useful to verify that your application both a) properly queries the limits from Vulkan, and b) obeys those limits.

The DevSim layer library is available pre-built in the Vulkan SDK, and continues to evolve. DevSim works for all Vulkan platforms (Linux, Windows, macOS, and Android), and is open-source software hosted on GitHub.The DevSim layer can be enabled and configured using the [Vulkan Configurator](https://vulkan.lunarg.com/doc/sdk/latest/windows/vkconfig.html) included with the Vulkan SDK.

The role of DevSim is to "simulate" a less-capable Vulkan implementation by constraining the features and resources of a more-capable implementation. Note that the actual device in your machine should be more capable than that which you are simulating. DevSim does not add capabilities to your existing Vulkan implementation by "emulating" additional capabilities with software; e.g. DevSim cannot add geometry shader capability to an actual device that doesn’t already provide it. Also, DevSim does not "enforce" the features being simulated. For enforcement, you would continue to use the Validation Layers as usual, in conjunction with DevSim.

### Using DevSim
DevSim supports a flexible configuration file format using JSON syntax. The configuration file format is defined by a formal JSON schema available on the Khronos website, so any configuration file may be verified to be correct using freely available JSON validators. Browsing through the schema file, you can see the extent of parameters that are available for your configuration. As a convenience, DevSim supports loading multiple JSON files, so your configuration data can be split among separate files for modularity as desired.

### Android
To enable, use a setting with the path of configuration file to load:
```
adb shell settings put global debug.vulkan.devsim.filepath <path/to/DevSim/JSON/configuration/file>
```
Example of a DevSim JSON configuration file: [tiny1.json](https://github.com/LunarG/VulkanTools/blob/master/layersvt/device_simulation_examples/tiny1.json)

Optional: use settings to enable debugging output and exit-on-error:
```
adb shell settings put global debug.vulkan.devsim.debugenable 1
adb shell settings put global debug.vulkan.devsim.exitonerror 1
```

### How DevSim Works
DevSim builds its internal data tables by querying the capabilities of the underlying actual device, then applying each of the configuration files “on top of” those tables. Therefore you only need to specify the features you wish to modify from the actual device; tweaking a single feature is easy. Here’s an example of  a valid configuration file for changing only the maximum permitted viewport size:

```json
{
   "$schema": "https://schema.khronos.org/vulkan/devsim_1_0_0.json#",
   "VkPhysicalDeviceProperties": {
       "limits": { "maxViewportDimensions": [1024, 1024] }
   }
}
```

### Simulating Entire Real-World Devices
If you instead wish to simulate entire real-world devices, LunarG has collaborated with the [Vulkan Hardware Database](https://vulkan.gpuinfo.org/) to make their data compatible with the DevSim schema. You can download device configurations from the website in JSON format, and use those configuration files directly with DevSim.

# Technical Details

The Device Simulation Layer is a Vulkan layer that can modify the results of Vulkan PhysicalDevice queries based on a JSON configuration file, thus simulating some of the capabilities of device by overriding the capabilities of the actual device under test.

Please note that this device simulation layer "simulates", rather than "emulates", another device.
By that we mean that the layer cannot add emulated capabilities that do not already exist in the system's underlying actual device;
DevSim will not enable a less-capable device to emulate a more-capable device.
This layer enables a more-capable device to simulate a less-capable device by removing some of the capabilities reported by queries;
you can change or reduce what is already present in the underlying actual implementation.

Application code can be tested to verify it responds correctly to the capabilities reported by the simulated device.
That could include:
* Properly querying the capabilities of the device.
* Properly complying with the limits reported from the device.
* Verifying all necessary capabilities are reported present, rather than assuming they are available.
* Exercising fall-back code paths, if optional capabilities are not available.

The DevSim layer does not enforce the capabilities returned by queries.
The application is still responsible for querying and complying with the capabilities reported.
If an application erroneously attempts to overcommit a resource, or use a disabled feature, the DevSim layer will not generate errors.
To detect such errors, use the Vulkan Validation layers, as with any non-simulated device.

The DevSim layer will work with other Vulkan layers, such as Validation.
When configuring the order of the layers list, the DevSim layer should be "last";
i.e.: closest to the driver, farthest from the application.
That allows the Validation layer to see the results of the DevSim layer, and permit Validation to enforce the simulated capabilities.

Please report issues to [LunarG's VulkanTools GitHub repository](https://github.com/LunarG/VulkanTools/issues) and include "DevSim" in the title text.

### Layer name
`VK_LAYER_LUNARG_device_simulation`

### DevSim Layer operation and configuration
At application startup, during vkCreateInstance(), the DevSim layer initializes its internal tables from the actual physical device in the system, then loads its configuration file, which specifies override values to apply to those internal tables.

How the JSON configuration values are applied depends on whether the top-level section begins with "ArrayOf" or not.
* If the section is not an array, values are applied if they appear in the JSON; if a value is not present in the JSON, the previous value is not modified.
Therefore not every parameter needs to be specified, only a sparse set of values that need to be changed.
* If the section defines an array (i.e.: begins with "ArrayOf"), then all previous contents of that array are cleared, and the JSON must specify all values of each desired array element.

JSON file formats consumed by the DevSim layer are specified by one of the JSON schemas in the table below.

| Schema Use | Canonical URI |
|:----------:|:-------------:|
| Vulkan v1.0 | https://schema.khronos.org/vulkan/devsim_1_0_0.json# |
| Vulkan v1.1 | https://schema.khronos.org/vulkan/devsim_1_1_0.json# |
| VK_KHR_portability_subset | https://schema.khronos.org/vulkan/devsim_VK_KHR_portability_subset-provisional-1.json# |
| VK_KHR_8bit_storage | https://schema.khronos.org/vulkan/devsim_VK_KHR_8bit_storage_1.json# |
| VK_KHR_16bit_storage | https://schema.khronos.org/vulkan/devsim_VK_KHR_16bit_storage_1.json# |
| VK_KHR_buffer_device_address | https://schema.khronos.org/vulkan/devsim_VK_KHR_buffer_device_address_1.json# |
| VK_KHR_maintenance2 | https://schema.khronos.org/vulkan/devsim_VK_KHR_maintenance2_1.json# |
| VK_KHR_maintenance3 | https://schema.khronos.org/vulkan/devsim_VK_KHR_maintenance3_1.json# |
| VK_KHR_multiview | https://schema.khronos.org/vulkan/devsim_VK_KHR_multiview_1.json# |
| VK_KHR_sampler_ycbcr_conversion | https://schema.khronos.org/vulkan/devsim_VK_KHR_sampler_ycbcr_conversion_1.json# |
| VK_KHR_variable_pointers | https://schema.khronos.org/vulkan/devsim_VK_KHR_variable_pointers_1.json# |

Usually you will be using configuration files validated with the Vulkan v1.1 schema.

The top-level sections of such configuration files are processed as follows:
* `$schema` - Mandatory.  Must be the URI string referencing the JSON schema.
* `comments` - Optional.  May contain arbitrary comments, description, copyright, etc.
* `VkPhysicalDeviceProperties` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceMaintenance3Properties` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceMultiviewProperties` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDevicePointClippingProperties` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceFeatures` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDevice16BitStorageFeatures` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceMultiviewFeatures` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceSamplerYcbcrConversionFeatures` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceVariablePointersFeatures` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceMemoryProperties` - Optional.  Only values specified in the JSON will be modified.
* `ArrayOfVkQueueFamilyProperties` - Optional.  If present, all values of all elements must be specified.
* `ArrayOfVkFormatProperties` - Optional.  If present, all values of all elements must be specified.
* `ArrayOfVkExtensionProperties` - Optional.  If present, all values of all elements must be specified. Modifies the list returned by `vkEnumerateDeviceExtensionProperties`.
* The remaining top-level sections of the schema are not yet supported by DevSim.

The schema permits additional top-level sections to be optionally included in configuration files;
any additional top-level sections will be ignored by DevSim.

The schemas define basic range checking for common Vulkan data types, but they cannot detect whether a particular configuration makes no sense.
If a configuration defines capabilities beyond what the actual device is natively capable of providing, the results are undefined.
DevSim has some simple checking of configuration values and writes debug messages (if enabled) for values that are incompatible with the capabilities of the actual device.

This version of DevSim currently supports Vulkan v1.0 and v1.1.
If the application requests an unsupported version of the Vulkan API, DevSim will emit an error message.
If you wish DevSim to terminate on errors, set the `VK_DEVSIM_EXIT_ON_ERROR` environment variable (see below).

### Example of a DevSim JSON configuration file
```json
{
    "$schema": "https://schema.khronos.org/vulkan/devsim_1_0_0.json#",
    "comments": {
        "info": "You may add whatever you wish to this comments section",
        "filename": "layersvt/device_simulation_examples/tiny1.json",
        "desc": "A tiny and valid DevSim JSON config file",
        "author": "mikew@lunarg.com"
    },
    "VkPhysicalDeviceProperties": {
        "deviceName": "override viewport capabilties",
        "limits": {
            "maxViewports": 1,
            "maxViewportDimensions": [1024, 1024]
        }
    }
}
```

### `VK_KHR_portability_subset` Emulation

The DevSim layer provides the ability to emulate the `VK_KHR_portability_subset` extension on devices that do not implement this extension.
This feature allows users to test their application with limitations found on non-conformant Vulkan implementations.
To turn on this feature, set the `VK_DEVSIM_EMULATE_PORTABILITY_SUBSET_EXTENSION` environment variable (or the corresponding vk_layer_settings.txt option `lunarg_device_simulation.emulate_portability`) to a positive integer.
DevSim config files that use this feature should validate to the portability specific schema https://schema.khronos.org/vulkan/devsim_VK_KHR_portability_subset-provisional-1.json#.

### DevSim Layer Options

| Environment Variable | `vk_layer_settings.txt` Option | Android Option | Default | Description |
|:--------------------:|:----------------------------:|:--------------:|:-----------:|:--------------:|
| `VK_DEVSIM_FILENAME` | `lunarg_device_simulation.filename` | `debug.vulkan.devsim.filepath` | Not Set | Name of one or more configuration file(s) to load. _Added in v1.2.1:_ This variable can have a delimited list of files to be loaded.  On Windows, the delimiter is `;` else it is `:`. Files are loaded in order.  Later files can override settings from earlier files.|
| `VK_DEVSIM_DEBUG_ENABLE` | `lunarg_device_simulation.debug_enable` | debug.vulkan.devsim.debugenable | false | Enables debug message output. |
| `VK_DEVSIM_EXIT_ON_ERROR` | `lunarg_device_simulation.exit_on_error` | debug.vulkan.devsim.exitonerror | false | Enables exit-on-error. |
| `VK_DEVSIM_EMULATE_PORTABILITY_SUBSET_EXTENSION` | `lunarg_device_simulation.emulate_portability` | debug.vulkan.devsim.emulateportability | true | Enables emulation of the `VK_KHR_portability_subset` extension. |
| `VK_DEVSIM_MODIFY_EXTENSION_LIST` | `lunarg_device_simulation.modify_extension_list` | debug.vulkan.devsim.modifyextensionlist | false | Enables modification of the device extensions list from the JSON config file. |
| `VK_DEVSIM_MODIFY_MEMORY_FLAGS` | `lunarg_device_simulation.modify_memory_flags` | debug.vulkan.devsim.modifymemoryflags | false | Enables modification of the device memory heap flags and memory type flags from the JSON config file. |

**Note:** Environment variables take precedence over `vk_layer_settings.txt` options.

### Example using the DevSim layer using Linux environment variables
```bash
# Configure bash to find the Vulkan SDK.
source $VKSDK/setup-env.sh

# Set loader parameters to find and load the DevSim layer from your local VulkanTools build.
export VK_LAYER_PATH="${VulkanTools}/build/layersvt"
export VK_INSTANCE_LAYERS="VK_LAYER_LUNARG_device_simulation"

# Specify the simulated device's configuration file.
export VK_DEVSIM_FILENAME="${VulkanTools}/layersvt/device_simulation_examples/tiny1.json"
# A list of files could look like:
# export VK_DEVSIM_FILENAME="/home/foo/first.json:/home/foo/second.json"

# Enable verbose messages from the DevSim layer.
export VK_DEVSIM_DEBUG_ENABLE="1"

# Run a Vulkan application through the DevSim layer.
vulkaninfo
# Compare the results with that app running without the DevSim layer.
```
See also
* [${VulkanTools}/tests/devsim_layer_test.sh](https://github.com/LunarG/VulkanTools/blob/master/tests/devsim_layer_test.sh) - a test runner script.
* [${VulkanTools}/tests/devsim_test1.json](https://github.com/LunarG/VulkanTools/blob/master/tests/devsim_test1_in.json) - an example configuration file, containing bogus test data.

### Device configuration data from vulkan.gpuinfo.org
A large and growing database of device capabilities is available at https://vulkan.gpuinfo.org/

That device data can be downloaded in JSON format, compatible with the DevSim JSON schema.

A JSON index of the available device records can be queried with https://vulkan.gpuinfo.org/api/v2/devsim/getreportlist.php

That index includes URLs to download the specific device records in DevSim-compatible format, for example https://vulkan.gpuinfo.org/api/v2/devsim/getreport.php?id=1456

As mentioned above, attempting to use a configuration file that does not fit within the capabilities of the underlying device may produce undefined results.
Downloaded device records should be reviewed to determine that their capabilities can be simulated by the underlying device.

### Device configuration data from the local system
Vulkan Info can write its output in a format compatible the DevSim JSON schema,
so the configuration of the local system can be captured.
Use `vulkaninfo -j` to generate output in JSON format and redirect to a file, which can be consumed directly by DevSim.
See the Vulkan Info documentation for further details.

### Device configuration data from an iOS device
Vulkan Info can be built and run for iOS using the source and XCode project available from the [Vulkan-Tools repository](https://github.com/KhronosGroup/Vulkan-Tools/tree/master/vulkaninfo). For details on running and extracting the json files see the [Vulkan Info documentation](https://vulkan.lunarg.com/doc/sdk/latest/mac/vulkaninfo.html). An [Apple Developer Program](https://developer.apple.com/programs/) membership is required to deploy Vulkan Info to an iOS hardware device.

### JSON validation
The DevSim layer itself does very little sanity-checking of the configuration file, so those files should be validated to the schema using a separate tool, such as the following web-based validators.
1. http://www.jsonschemavalidator.net/
2. https://json-schema-validator.herokuapp.com/
3. https://jsonschemalint.com/#/version/draft-04/markup/json/

### Other Resources
1. http://json.org/
2. http://json-schema.org/
