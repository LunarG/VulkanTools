{
"Best Practices Validation" : {
"layer_options": {
    "VK_LAYER_KHRONOS_validation": {
        "debug_action": {
            "name": "Debug Action",
            "description": "This indicates what action is to be taken when a layer wants to report information",
            "type": "enum",
            "options": {
                "VK_DBG_LAYER_ACTION_IGNORE": "Ignore",
                "VK_DBG_LAYER_ACTION_LOG_MSG": "Log Message",
                "VK_DBG_LAYER_ACTION_CALLBACK": "Callback",
                "VK_DBG_LAYER_DEBUG_OUTPUT": "Debug Output",
                "VK_DBG_LAYER_ACTION_BREAK": "Break"
            },
            "default": "VK_DBG_LAYER_ACTION_LOG_MSG"
        },
        "report_flags": {
            "name": "Report Flags",
            "description": "This is a comma-delineated list of options telling the layer what types of messages it should report back",
            "type": "multi_enum",
            "options": {
                "info": "Info",
                "warn": "Warn",
                "perf": "Perf",
                "error": "Error",
                "debug": "Debug"
            },
            "default": [
                "error",
                "warn",
                "perf"
            ]
        },
        "log_filename": {
            "name": "Log Filename",
            "description": "Specifies the output filename",
            "type": "save_file",
            "default": "stdout"
        },
        "disables": {
            "read only": "true",
            "name": "Disables",
            "description": "Setting an option here will disable areas of validation",
            "type": "multi_enum",
            "options": {
                "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT": "Disable thread safety checks",
                "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT": "Disable stateless parameter checks",
                "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT": "Disable object lifetime validation",
                "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT": "Disable core validation checks",
                "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT": "Disable handle wrapping"
            },
            "default": ["VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT",
                        "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT",
                        "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT",
                        "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT",
                        "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT"]
        },
        "enables": {
            "read only": "true",
            "name": "Enables",
            "description": "Setting an option here will enable specialized areas of validation",
            "type": "multi_enum",
            "options": {
                "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT": "Enable GPU-Assisted Validation",
                "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT": "Reserve a descriptorSet binding slot for internal use",
                "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT": "Enable Best Practices warning checks",
                "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_EXT": "Enable Synchronization"
            },
            "default": ["VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT"]
        }
    }

    }
}
}
