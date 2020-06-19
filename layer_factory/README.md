<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>

Copyright &copy; 2015-2020 LunarG, Inc.

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

# Vulkan Layer Factory

## Overview

The Vulkan Layer Factory is a framework based on the canonical Vulkan layer model that
facilitates the implementation of Vulkan Layers. The layer factory hides the majority of the
loader-layer interface, layer boilerplate, setup and initialization, and complexities
of layer development.

A complete layer with the attendant support files can be produced by simply creating a
subdirectory in the layer\_factory directory and adding in a simple header file
and then running cmake. This layer can be used just as any other Vulkan layer.

The Vulkan Layer Factory framework produces 'Factory Layers' comprising one or more
'interceptor' objects. Interceptor objects override functions to be called before (PreCallApiName)
or after (PostCallApiName) each Vulkan entrypoint of interest. Each interceptor is independent
of all others within a Factory Layer, and their call order is not guaranteed.

### Layer Factory sample code

The base installation of the layer factory contains some sample layers, including
the Demo layer and the Starter Layer. The Starter Layer in particular is meant to serve as
an example of a very simple layer implementation.


### Create a Factory Layer


Step 1: Create a subdirectory in the layer\_factory directory using a name that will serve as the base
for the final layer

    For example, a subdirectory named 'starter\_layer' will produce VkLayer\_starter\_layer.dll/libVkLayer\_starter\_layer.so and be loaded as VK_LAYER_LUNARG_starter_layer

Step 2: Add your state tracker file(s) to the subdirectory.

    This can be a single header file, or multiple header and source files.

Step 3: Create or copy an 'interceptor\_objects.h' file into your new directory

    This should include the header file for each of the included interceptors:

    #include "whatever_you_called_your_layers_header_file.h"

Step 4: Run CMake and build.

    CMake will discover all Factory Layer subdirectories in layer_factory each time it is run.
    A *.json and *.def file (for Windows) will also be created with your layer binary.
    These files end up in the existing layers binary directory, and will be picked up
    by the usual VK_LAYERS_PATH environment variable.

    Note that adding or removing a layer_factory subdirectory requires re-running CMake in order to
    properly recognize the additions/deletions.

## Using Layers

1. Build VK loader using normal steps (cmake and make)
2. Place the layer library (dll or so) in the same directory as your VK test or application

       This is required for the Loader to be able to scan and enumerate your library.
       Alternatively, use the `VK_LAYER_PATH` environment variable to specify where the layer libraries reside.

3. Specify which layers to activate using environment variables.

    `export VK\_INSTANCE\_LAYERS=VK\_LAYER\_KHRONOS\_validation:VK\_LAYER\_LUNARG\_starter\_layer`


### Layer Factory Features

The layer factory provides helper functions for layer authors to simplify layer tasks. These include some
simpler output functions, debug facilities, and simple global intercept functions.


Output Helpers:

Interceptors can use base-class defined output helpers for simpler access to Debug Report Extension output.
These include Information(), Warning(), Performance\_Warning(), and Error(), corresponding to the
VkDebugReportFlagBitsEXT enumerations. Alternatively, the standard layer-provided log\_msg() call can be used
directly, as can printf for standard-out or OutputDebugString for Windows.

Debug Helpers:

A BreakPoint() helper can be used in an intercepted function which will generate a break in a Windows or Linux
debugger.

Global Intercept Helpers:

There are two global intercept helpers, PreCallApiFunction() and PostCallApiFunction(). Overriding these virtual
functions in your intercepter will result in them being called for EVERY API call.

### Details

By creating a child framework object, the factory will generate a full layer and call any overridden functions
in your interceptor.

Here is a simple, and complete, interceptor (the starter\_layer). This layer intercepts the memory allocate and free
functions, tracking the number and total size of device memory allocations. The QueuePresent() function is also intercepted, and
results are outputted on every 60th frame.  Note that the function signatures are identical to those in the specification.

In this example, there is a single interceptor in which the child object is named 'MemAllocLevel' and is instantiated as
'high\_water\_mark'. An layer can contain many interceptors as long as the instantiated object names are unique within that layer.


    #pragma once
    #include <sstream>
    #include <unordered_map>

    static uint32_t display_rate = 60;

    class MemAllocLevel : public layer_factory {
        public:
            // Constructor for interceptor
            MemAllocLevel() : layer_factory(this), number_mem_objects_(0), total_memory_(0), present_count_(0) {};

            // Intercept memory allocation calls and increment counter
            VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory) {
                number_mem_objects_++;
                total_memory_ += pAllocateInfo->allocationSize;
                mem_size_map_[*pMemory] = pAllocateInfo->allocationSize;
                return VK_SUCCESS;
            };

            // Intercept free memory calls and update totals
            void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
                if (memory != VK_NULL_HANDLE) {
                    number_mem_objects_--;
                    VkDeviceSize this_alloc = mem_size_map_[memory];
                    total_memory_ -= this_alloc;
                }
            }

            VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
                present_count_++;
                if (present_count_ >= display_rate) {
                    present_count_ = 0;
                    std::stringstream message;
                    message << "Memory Allocation Count: " << number_mem_objects_ << "\n";
                    message << "Total Memory Allocation Size: " << total_memory_ << "\n\n";
                    Information(message.str());
                }
                return VK_SUCCESS;
            }

        private:
            // Counter for the number of currently active memory allocations
            uint32_t number_mem_objects_;
            VkDeviceSize total_memory_;
            uint32_t present_count_;
            std::unordered_map<VkDeviceMemory, VkDeviceSize> mem_size_map_;
    };

    MemAllocLevel memory_allocation_stats;

### Current known issues

 * CMake MUST be run to pick up and interpret new or deleted factory layers.


