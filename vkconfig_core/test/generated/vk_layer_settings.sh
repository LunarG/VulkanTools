#! Loader Settings:
export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_reference_1_2_1

#! Layers Settings:

#! reference layer
#! ==========================================
#! VK_LAYER_LUNARG_reference_1_2_1 - 1.2.170 (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)

#! toogle
#! ------------------------------------------
#! toogle (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  true or false
export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true

#! enum
#! ------------------------------------------
#! enum_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  enum case
export VK_LUNARG_REFERENCE_1_2_1_ENUM_REQUIRED_ONLY=value1

#! enum
#! ------------------------------------------
#! enum_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  enum case
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#enum
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_ENUM_WITH_OPTIONAL=value1

#! flags
#! ------------------------------------------
#! flags_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  flags case
export VK_LUNARG_REFERENCE_1_2_1_FLAGS_REQUIRED_ONLY=flag0,flag1

#! flags
#! ------------------------------------------
#! flags_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  flags case
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flags
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_FLAGS_WITH_OPTIONAL=flag0,flag1

#! String
#! ------------------------------------------
#! string_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  string
export VK_LUNARG_REFERENCE_1_2_1_STRING_REQUIRED_ONLY=A string

#! String
#! ------------------------------------------
#! string_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  string
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#string
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_STRING_WITH_OPTIONAL=A string

#! bool
#! ------------------------------------------
#! bool_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  true or false
export VK_LUNARG_REFERENCE_1_2_1_BOOL_REQUIRED_ONLY=true

#! bool
#! ------------------------------------------
#! bool_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  true or false
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#bool
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_BOOL_WITH_OPTIONAL=true

#! Load file
#! ------------------------------------------
#! load_file_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Load file path
export VK_LUNARG_REFERENCE_1_2_1_LOAD_FILE_REQUIRED_ONLY=./test.txt

#! Load file
#! ------------------------------------------
#! load_file_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Load file path
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#load_file
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_LOAD_FILE_WITH_OPTIONAL=./test.json

#! Save file
#! ------------------------------------------
#! save_file_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Save file path
export VK_LUNARG_REFERENCE_1_2_1_SAVE_FILE_REQUIRED_ONLY=./test.json

#! Save file
#! ------------------------------------------
#! save_file_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Save file path
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_file
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_SAVE_FILE_WITH_OPTIONAL=./test.json

#! Save folder
#! ------------------------------------------
#! save_folder_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Save folder path
export VK_LUNARG_REFERENCE_1_2_1_SAVE_FOLDER_REQUIRED_ONLY=./test

#! Save folder
#! ------------------------------------------
#! save_folder_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Save folder path
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_folder
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_SAVE_FOLDER_WITH_OPTIONAL=./test

#! Integer
#! ------------------------------------------
#! int_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Integer Description
export VK_LUNARG_REFERENCE_1_2_1_INT_REQUIRED_ONLY=76

#! Integer
#! ------------------------------------------
#! int_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Integer Description
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#int
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_INT_WITH_OPTIONAL=76

#! Float
#! ------------------------------------------
#! float_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Float Description
export VK_LUNARG_REFERENCE_1_2_1_FLOAT_REQUIRED_ONLY=76.500000

#! Float
#! ------------------------------------------
#! float_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Float Description
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#float
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_FLOAT_WITH_OPTIONAL=76.500

#! Frames
#! ------------------------------------------
#! frames_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Frames Description
export VK_LUNARG_REFERENCE_1_2_1_FRAMES_REQUIRED_ONLY=76-82,75

#! Frames
#! ------------------------------------------
#! frames_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  Frames Description
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#frames
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_FRAMES_WITH_OPTIONAL=76-82,75

#! List
#! ------------------------------------------
#! list_required_only (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  List description
export VK_LUNARG_REFERENCE_1_2_1_LIST_REQUIRED_ONLY=76,82,stringB,stringD

#! List
#! ------------------------------------------
#! list_with_optional (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  List description
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#list
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_REFERENCE_1_2_1_TOOGLE=true
export VK_LUNARG_REFERENCE_1_2_1_LIST_WITH_OPTIONAL=76,82,stringB,stringD

#! List
#! ------------------------------------------
#! list_empty (BETA) (WINDOWS_X86, WINDOWS_ARM, LINUX, MACOS)
#!  List description
#! For more information about the feature: https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#list
export VK_LUNARG_REFERENCE_1_2_1_LIST_EMPTY=

