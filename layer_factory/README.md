# Vulkan Layer Factory

## Overview

The Vulkan Layer Factory is a framework based on the canonical Vulkan layer model that
facilitates the implementation of Vulkan Layers. The layer factory hides the majority of the
loader-layer interface, layer boilerplate, setup and initialization, and complexities
of layer development.

A complete layer with the attendant support files can be produced by simply creating a
subdirectory in the layer\_factory directory and adding in a simple header file
and then running cmake. This layer can be used just as any other Vulkan layer.


### Layer Factory sample code

The base installation of the layer factory contains some sample layers, including the Vulkan
Assistant Layer and the Starter Layer. The Starter Layer in particular is meant to serve as
an example of a very simple layer implementation.


### Create a Factory Layer


Step 1: Create a subdirectory in the layer\_factory directory using a name that will serve as the base
for the final layer

    For example, a subdirectory named 'starter\_layer' will produce VkLayer\_starter\_layer.dll/libVkLayer\_starter\_layer.so and be loaded as VK_LAYER_LUNARG_starter_layer

Step 2: Add your state tracker file(s) to the subdirectory.

    This can be a single header file, or mulitple header and source files.

Step 3: Create or copy an 'interceptor\_objects.h' file into your new directory

    This should include the header file for each of the included interceptors:

    #include "whatever_you_called_your_layers_header_file.h"

Step 4: Run CMake and build.

    A *.json and *.def file (for Windows) will also be created with your layer binary.
    These files end up in the existing layers binary directory, and will be picked up
    by the usual VK_LAYERS_PATH environment variable.

## Using Layers

1. Build VK loader using normal steps (cmake and make)
2. Place the layer library (dll or so) in the same directory as your VK test or application

       This is required for the Loader to be able to scan and enumerate your library.
       Alternatively, use the `VK_LAYER_PATH` environment variable to specify where the layer libraries reside.

3. Specify which layers to activate using environment variables.

    `export VK\_INSTANCE\_LAYERS=VK\_LAYER\_LUNARG\_standard\_validation:VK\_LAYER\_LUNARG\_starter\_layer`


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
            MemAllocLevel() : layer_factory(this) {
                _number_mem_objects = 0;
                _total_memory = 0;
                _present_count = 0;
            };

            // Intercept memory allocation calls and increment counter
            VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory) {
                _number_mem_objects++;
                _total_memory += pAllocateInfo->allocationSize;
                _mem_size_map[*pMemory] = pAllocateInfo->allocationSize;
                return VK_SUCCESS;
            };

            // Intercept free memory calls and update totals
            void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
                if (memory != VK_NULL_HANDLE) {
                    _number_mem_objects--;
                    VkDeviceSize this_alloc = _mem_size_map[memory];
                    _total_memory -= this_alloc;
                }
            }

            VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
                _present_count++;
                if (_present_count >= display_rate) {
                    _present_count = 0;
                    std::stringstream message;
                    message << "Memory Allocation Count: " << _number_mem_objects << "\n";
                    message << "Total Memory Allocation Size: " << _total_memory << "\n\n";
                    Information(message.str());
                }
                return VK_SUCCESS;
            }

        private:
            // Counter for the number of currently active memory allocations
            uint32_t _number_mem_objects;
            VkDeviceSize _total_memory;
            uint32_t _present_count;
            std::unordered_map<VkDeviceMemory, VkDeviceSize> _mem_size_map;
    };

    MemAllocLevel high_water_mark;

### Current known issues

 * CMake MUST be run to pick up and interpret new or deleted factory layers.


