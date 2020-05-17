


#include <QString>

struct TABLE_ENTRY {
    QString         setting;
    QString         description;
    QString         url;
};

TABLE_ENTRY lookupTable[] = {
    { "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT",
      "API usage validation checks: Validate the status of descriptor sets, command buffers, shader modules, pipeline states, renderpass usage, synchronization, dynamic states, and many other types of valid usage. ",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/core_checks.md"},

     { "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE",
      "Commandbuffer state core validation checks.",
      "" },

    { "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE",
      "Object-in-use state core validation checks.",
      ""},

    { "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET",
      "Core validation checks to verify descriptor sets are not in-use.",
      "" },

    { "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION",
      "Query state core validation checks.",
     "" },

    { "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE",
      "Push constant range core validation checks.",
      "" },

    { "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION",
      "Image layout validation core validation checks.",
      "" },

    { "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT",
      "Shader validation-related core validation checks.",
      "" },

    { "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT",
      "Thread-safety external synchronization checks. Checks performed include ensuring that only one thread at a time uses an object in free-threaded API calls.",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/thread_safety.md" },

    { "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT",
      "Stateless API parameter validation checks. This option checks the validity of structures ; the validity of enumerated type values ; null pointer conditions ; properties and limits requirements. ",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/stateless_validation.md" },

    { "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT",
      "Object lifetimes core validation checks. Validate that only known objects are referenced and destroyed, that lookups are performed only on objects being tracked and that objects are correctly freed or destroyed.",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/object_lifetimes.md" },

    { "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT",
      "Handle-wrapping which ensures unique object handles",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/handle_wrapping.md" },

    { "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT",
      "Enables shader instrumentation for additional diagnostic validation checks.",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/gpu_validation.md" },

    { "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT",
      "Modifies the value of the VkPhysicalDeviceLimits::maxBoundDescriptorSets property to return a value one less than the actual device's value to \"reserve\" a descriptor set binding slot for use by shader-based validation. This option is likely only of interest to applications that dynamically adjust their descriptor set bindings to adjust for the limits of the device.",
      "" },

    { "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT",
      "Enables the shader Debug Printf functionality. It allows developers to debug their shader code by \"printing\" any values of interest to the debug callback or stdout. ",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/debug_printf.md" },

    { "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT",
      "Vulkan Best Practices checks. It is intended to highlight potential performance issues, questionable usage patterns, common mistakes, and items not specifically prohibited by the Vulkan specification but that may lead to application problems.",
      "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/best_practices.md" },

    { "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM",
      "ARM GPU-specific Best Practices checks. It is intended to highlight usage patterns that are performance issues on ARM GPUs but that are not prohibited by the Vulkan specification or issues on other vendor GPUs.",
      "" },
};





