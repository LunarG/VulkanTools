# devsim_info

This directory is a filesystem stub, to give a home for upcoming development.

The `devsim_info` tool writes the current system's Vulkan configuration information to stdout, in a format conforming to the
[devsim schema](https://schema.khronos.org/vulkan/devsim_1_0_0.json#).
That configuration info can then be loaded as a config file by the 
[device simulation layer](https://github.com/LunarG/VulkanTools/blob/master/layersvt/device_simulation.md).

At present (Aug 2017), the only generator of devsim schema-compliant data is http://vulkan.gpuinfo.org
This tool adds the capability to local systems, without the necessity of querying that online database.
The `devsim_info` will also provide features to simplify the creation of regression tests, by permitting selection of subsets of the config for use in comparing with gold result files.

Requirements and rationale for creating this tool are captured in more detail at
[VT-47](https://lunarg.atlassian.net/browse/VT-47)

