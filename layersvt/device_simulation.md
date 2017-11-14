# VK_LAYER_LUNARG_device_simulation (DevSim)
The goal of this layer is to simplify application testing on a wide range of simulated device capabilities, without requiring an actual physical copy of every device.

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

Please report issues to the [GitHub VulkanTools repository](https://github.com/LunarG/VulkanTools/issues) and include "DevSim" in the title text.

## Layer name
`VK_LAYER_LUNARG_device_simulation`

## DevSim Layer operation and configuration
At application startup, during vkCreateInstance(), the DevSim layer initializes its internal tables from the actual physical device in the system, then loads its configuration file, which specifies override values to apply to those internal tables.

How the JSON configuration values are applied depends on whether the top-level section begins with "ArrayOf" or not.
* If the section is not an array, values are applied if they appear in the JSON; if a value is not present in the JSON, the previous value is not modified.
Therefore not every parameter needs to be specified, only a sparse set of values that need to be changed.
* If the section defines an array (i.e.: begins with "ArrayOf"), then all previous contents of that array is cleared, and the JSON must specify all values of each desired array element.

The JSON fileformat consumed by the DevSim layer is specified by a JSON schema, the canonical URI of which is "https://schema.khronos.org/vulkan/devsim_1_0_0.json#"

The top-level sections of a configuration file are specified by the DevSim JSON schema, and are processed as follows:
* `$schema` - Mandatory.  Must be the URI string referencing the JSON schema.
* `comments` - Optional.  May contain arbitrary comments, description, copyright, etc.
* `VkPhysicalDeviceProperties` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceFeatures` - Optional.  Only values specified in the JSON will be modified.
* `VkPhysicalDeviceMemoryProperties` - Optional.  Only values specified in the JSON will be modified.
* `ArrayOfVkQueueFamilyProperties` - Optional.  If present, all values of all elements must be specified.
* `ArrayOfVkFormatProperties` - Optional.  If present, all values of all elements must be specified.
* The remaining top-level sections of the schema are not yet supported by DevSim.

The schema permits additional top-level sections to be optionally included in configuration files;
any additional top-level sections will be ignored by DevSim.

The schema defines basic range checking for common Vulkan data types, but it cannot detect if a particular configuration makes no sense.
If a configuration defines capabilities beyond what the actual device is natively capable of providing, the results are undefined.
DevSim has some simple checking of configuration values and writes debug messages (if enabled) for values that are incompatible with the capabilities of the actual device.

This version of DevSim currently supports only Vulkan v1.0.
If the application requests an unsupported version of the Vulkan API, DevSim will emit an error message.
If you wish DevSim to terminate on errors, set the `VK_DEVSIM_EXIT_ON_ERROR` environment variable (see below).

## Example of a DevSim JSON configuration file
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

## Environment variables used by DevSim layer.

* `VK_DEVSIM_FILENAME` - Name of one or more configuration file(s) to load.
  _Added in v1.2.1:_ This variable can have a delimited list of files to be loaded.  On Windows, the delimiter is `;` else it is `:`.
  Files are loaded in order.  Later files can override settings from earlier files.
* `VK_DEVSIM_DEBUG_ENABLE` - A non-zero integer enables debug message output.
* `VK_DEVSIM_EXIT_ON_ERROR` - A non-zero integer enables exit-on-error.

## Example using the DevSim layer
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
* ${VulkanTools}/tests/devsim_layer_test.sh - a test runner script.
* ${VulkanTools}/tests/devsim_test1.json - an example configuration file, containing bogus test data.

## Device configuration data from vulkan.gpuinfo.org
A large and growing database of device capabilities is available at https://vulkan.gpuinfo.org/

That device data can be downloaded in JSON format, compatible with the DevSim JSON schema.

A JSON index of the available device records can be queried with https://vulkan.gpuinfo.org/api/v2/devsim/getreportlist.php

That index includes URLs to download the specific device records in DevSim-compatible format, for example https://vulkan.gpuinfo.org/api/v2/devsim/getreport.php?id=1456

As mentioned above, attempting to use a configuration file that does not fit within the capabilities of the underlying device may produce undefined results.
Downloaded device records should be reviewed to determine that its capabilities can be simulated by the underlying device.

## JSON validation
The DevSim layer itself does very little sanity checking of the configuration file, so those files should be validated to the schema using a separate tool, such as the following web-based validators.
1. http://www.jsonschemavalidator.net/
1. https://jsonschemalint.com/#/version/draft-04/markup/json
1. https://json-schema-validator.herokuapp.com/

## Other Resources
1. http://json.org/
1. http://json-schema.org/

