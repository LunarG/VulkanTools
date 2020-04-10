{
    "Lightweight Validation": {
        "VK_LAYER_KHRONOS_validation": {
            "Read Only": {
            "Disables": "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT,VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT,VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT"
            },
           "User" : {
           "debug_action": "VK_DBG_LAYER_ACTION_LOG_MSG",
           "report_flags": [
           "error",
           "warn",
           "perf" ],
           "log_filename": "stdout",
           "Enables": []
           }
       }
    }    
}
