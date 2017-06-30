# Device Simulation Layer (DevSim)
The Device Simulation Layer is a Vulkan layer that can modify the results of Vulkan PhysicalDevice queries based on a configuration file,
thus simulating a different device than is actually in a system.
A goal is to simplify application testing for a wide range of simulated devices, without actually requiring a physical copy of every device.

The DevSim layer loads a configuration file at application startup, which specifies the capabilities of the simulated device.

## NOTICE
This is a "version 0" prerelease of the DevSim layer.
Behaviors may change before a general "version 1.x" public release, particularly the configuration fileformat.

## DISCLAIMER
No warranty that a simulated device accurately reproduces capabilities of an actual device.

## Layer Name
`VK_LAYER_LUNARG_device_simulation`

## File Format
In this version 0 release, the fileformat consumed by DevSim is the format produced by [vkjson_info](https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/tree/master/libs/vkjson).

The following vkjson_info sections are interpreted by DevSim for overrides:
* `properties`
* `features`

Other sections of the vkjson_info output are ignored.

## Environment variables used by DevSim layer.

* `VK_DEVSIM_FILENAME` - name of the configuration file to load.
* `VK_DEVSIM_DEBUG_ENABLE` - a non-zero integer will enable debugging output.
* `VK_DEVSIM_EXIT_ON_ERROR` - a non-zero integer will enable exit-on-error.

## Example using the DevSim layer
```bash
# Configure bash to find the Vulkan SDK.
source $VKSDK/setup_env.sh

# Set loader parameters to find and load the DevSim layer from your local VulkanTools build.
export VK_INSTANCE_LAYERS="VK_LAYER_LUNARG_device_simulation"
export VK_LAYER_PATH="${VulkanTools}/build/layersvt"

# Specify the simulated device's configuration file.
export VK_DEVSIM_FILENAME="${VulkanTools}/tests/devsim_test1.json"

# Run a Vulkan application through the DevSim layer.
vkjson_info
# That app writes the simulated device's configuration to an output file.
```
See also:
* ${VulkanTools}/tests/devsim_layer_test.sh - a test runner script.
* ${VulkanTools}/tests/devsim_test1.json - an example configuration file, containing bogus test data.

