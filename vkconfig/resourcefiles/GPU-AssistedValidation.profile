{
    "GPU-Assisted Validation": {
        "VK_LAYER_KHRONOS_validation": {
            "Read Only": {
            "Enables": "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT",
            "Disables": "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT,VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT,VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT,VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT,VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT"
            },
            "User" : {
            "debug_action": "VK_DBG_LAYER_ACTION_LOG_MSG",
            "report_flags": [
            "error",
            "warn",
            "perf" ],
            "log_filename": "stdout"
            }
        }
    }
}
