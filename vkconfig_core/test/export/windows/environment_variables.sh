#! Loader Settings:
#! For now, the Vulkan Loader doesn't fully support the same behavior with environment variables than what's supported with Vulkan Configurator...
#! The Vulkan Loader doesn't support fully ordering layers with environment variables.
export VK_LOADER_LAYERS_ENABLE=VK_LAYER_KHRONOS_validation

#! Layers Settings:

#! VK_LAYER_NV_optimus 1.3.295


#! VK_LAYER_RENDERDOC_Capture 1.3.131


#! VK_LAYER_VALVE_steam_fossilize 1.3.207


#! VK_LAYER_VALVE_steam_overlay 1.3.207


#! VK_LAYER_KHRONOS_validation 1.4.321

#! Fine Grained Locking - fine_grained_locking
#! ==========================================
#!  Enable fine grained locking for Core Validation, which should improve
#!  performance in multithreaded applications. This setting allows the
#!  optimization to be disabled for debugging.
export VK_KHRONOS_VALIDATION_FINE_GRAINED_LOCKING=true

#! Core - validate_core
#! ==========================================
#!  The main, heavy-duty validation checks. This may be valuable early in the
#!  development cycle to reduce validation output while correcting
#!  parameter/object usage errors.
export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true

#! Image Layout - check_image_layout
#! ==========================================
#!  Check that the layout of each image subresource is correct whenever it is
#!  used by a command buffer. These checks are very CPU intensive for some
#!  applications.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true
export VK_KHRONOS_VALIDATION_CHECK_IMAGE_LAYOUT=true

#! Command Buffer State - check_command_buffer
#! ==========================================
#!  Check that all Vulkan objects used by a command buffer have not been
#!  destroyed. These checks can be CPU intensive for some applications.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true
export VK_KHRONOS_VALIDATION_CHECK_COMMAND_BUFFER=true

#! Object in Use - check_object_in_use
#! ==========================================
#!  Check that Vulkan objects are not in use by a command buffer when they are
#!  destroyed.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true
export VK_KHRONOS_VALIDATION_CHECK_OBJECT_IN_USE=true

#! Query - check_query
#! ==========================================
#!  Checks for commands that use VkQueryPool objects.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true
export VK_KHRONOS_VALIDATION_CHECK_QUERY=true

#! Shader - check_shaders
#! ==========================================
#!  This will validate the contents of the SPIR-V which can be CPU intensive
#!  during application start up. This does internal checks as well as calling
#!  spirv-val.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true
export VK_KHRONOS_VALIDATION_CHECK_SHADERS=true

#! Caching - check_shaders_caching
#! ==========================================
#!  Creates an internal instance of VK_EXT_validation_cache and upon
#!  vkDestroyInstance, will cache the shader validation so sequential usage of
#!  the validation layers will be skipped.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_CORE=true
#! - export VK_KHRONOS_VALIDATION_CHECK_SHADERS=true
export VK_KHRONOS_VALIDATION_CHECK_SHADERS_CACHING=true

#! Disable spirv-val - debug_disable_spirv_val
#! ==========================================
#!  Allows normal shader validation to run, but removes just spirv-val for
#!  performance reasons
export VK_KHRONOS_VALIDATION_DEBUG_DISABLE_SPIRV_VAL=false

#! Handle Wrapping - unique_handles
#! ==========================================
#!  Handle wrapping checks. Disable this feature if you are experiencing crashes
#!  when creating new extensions or developing new Vulkan objects/structures.
export VK_KHRONOS_VALIDATION_UNIQUE_HANDLES=true

#! Object Lifetime - object_lifetime
#! ==========================================
#!  Object tracking checks. This may not always be necessary late in a
#!  development cycle.
export VK_KHRONOS_VALIDATION_OBJECT_LIFETIME=true

#! Stateless Parameter - stateless_param
#! ==========================================
#!  Stateless parameter checks. This may not always be necessary late in a
#!  development cycle.
export VK_KHRONOS_VALIDATION_STATELESS_PARAM=true

#! Thread Safety - thread_safety
#! ==========================================
#!  Thread checks. In order to not degrade performance, it might be best to run
#!  your program with thread-checking disabled most of the time, enabling it
#!  occasionally for a quick sanity check or when debugging difficult application
#!  behaviors.
export VK_KHRONOS_VALIDATION_THREAD_SAFETY=true

#! Synchronization - validate_sync
#! ==========================================
#!  Check for resource access conflicts caused by missing or incorrectly used
#!  synchronization operations.
export VK_KHRONOS_VALIDATION_VALIDATE_SYNC=false

#! Submit time validation - syncval_submit_time_validation
#! ==========================================
#!  Enable synchronization validation on the boundary between submitted command
#!  buffers. This also validates accesses from presentation operations. This
#!  option can incur a significant performance cost.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_SYNC=true
#! export VK_KHRONOS_VALIDATION_SYNCVAL_SUBMIT_TIME_VALIDATION=true (Commented out as the set of dependences is not satisfied)

#! Shader accesses heuristic - syncval_shader_accesses_heuristic
#! ==========================================
#!  Take into account memory accesses performed by the shader based on SPIR-V
#!  static analysis. Warning: can produce false-positives, can ignore certain
#!  types of accesses.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_SYNC=true
#! export VK_KHRONOS_VALIDATION_SYNCVAL_SHADER_ACCESSES_HEURISTIC=false (Commented out as the set of dependences is not satisfied)

#! Extra properties - syncval_message_extra_properties
#! ==========================================
#!  Append a section of key-value properties to the error message. Useful for
#!  filtering errors.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_SYNC=true
#! export VK_KHRONOS_VALIDATION_SYNCVAL_MESSAGE_EXTRA_PROPERTIES=false (Commented out as the set of dependences is not satisfied)

#! Debug Printf only preset - printf_only_preset
#! ==========================================
#!  A single, quick setting to turn on only DebugPrintf and turn off everything
#!  else
export VK_KHRONOS_VALIDATION_PRINTF_ONLY_PRESET=false

#! Debug Printf - printf_enable
#! ==========================================
#!  Enable DebugPrintf and will print anything use NonSemantic.DebugPrintf in
#!  their SPIR-V
export VK_KHRONOS_VALIDATION_PRINTF_ENABLE=false

#! Redirect Printf messages to stdout - printf_to_stdout
#! ==========================================
#!  Enable redirection of Debug Printf messages from the debug callback to stdout
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_PRINTF_ENABLE=true
#! export VK_KHRONOS_VALIDATION_PRINTF_TO_STDOUT=true (Commented out as the set of dependences is not satisfied)

#! Printf verbose - printf_verbose
#! ==========================================
#!  Will print out handles, instruction location, position in command buffer, and
#!  more
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_PRINTF_ENABLE=true
#! export VK_KHRONOS_VALIDATION_PRINTF_VERBOSE=false (Commented out as the set of dependences is not satisfied)

#! Printf buffer size - printf_buffer_size
#! ==========================================
#!  Set the size in bytes of the buffer per draw/dispatch/traceRays to hold the
#!  messages
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_PRINTF_ENABLE=true
#! export VK_KHRONOS_VALIDATION_PRINTF_BUFFER_SIZE=1024 (Commented out as the set of dependences is not satisfied)

#! GPU Assisted Validation - gpuav_enable
#! ==========================================
#!  Enable validation that cannot be done the CPU and needs hooks into the GPU
#!  execution
export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=false

#! Safe Mode - gpuav_safe_mode
#! ==========================================
#!  Will have GPU-AV try and prevent crashes, but will be much slower to
#!  validate. If using Safe Mode, consider using selective shader
#!  instrumentation, to only instrument the shaders/pipelines causing issues.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_SAFE_MODE=false (Commented out as the set of dependences is not satisfied)

#! Force on robustness features - gpuav_force_on_robustness
#! ==========================================
#!  This will enable all possible robustness features for the app at device
#!  creation time. This can be used to quickly detect if with robustness, your
#!  issue disappears. GPU-AV will also skip validating things already covered by
#!  robustness, so turning on should reduce GPU-AV performance overhead.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_FORCE_ON_ROBUSTNESS=false (Commented out as the set of dependences is not satisfied)

#! Shader instrumentation - gpuav_shader_instrumentation
#! ==========================================
#!  Instrument shaders to validate descriptors, descriptor indexing, buffer
#!  device addresses and ray queries. Warning: will considerably slow down shader
#!  executions.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true (Commented out as the set of dependences is not satisfied)

#! Enable instrumenting shaders selectively - gpuav_select_instrumented_shaders
#! ==========================================
#!  Select which shaders to instrument by passing a VkValidationFeaturesEXT
#!  struct with GPU-AV enabled in the VkShaderModuleCreateInfo pNext or using a
#!  regex matching a shader/pipeline debug name. Because this only validates the
#!  selected shaders, it will allow GPU-AV to run much faster.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_SELECT_INSTRUMENTED_SHADERS=false (Commented out as the set of dependences is not satisfied)

#! Shader/pipeline name regexes - gpuav_shaders_to_instrument
#! ==========================================
#!  Any shader or pipeline library debug name (set with
#!  vkSetDebugUtilsObjectNameEXT) fully matching any listed regular expression
#!  will be instrumented when creating pipelines with those shaders or libraries.
#!  Regex grammar: Modified ECMAScript. No support for shader objects yet.
#!  Warning: instrumentation being performed at final pipeline creation time, if
#!  shaders modules have already been destroyed at this step (possible when using
#!  pipeline libraries), they won't be found by this regex selection system. In
#!  this case, consider naming pipeline libraries instead.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SELECT_INSTRUMENTED_SHADERS=true
#! export VK_KHRONOS_VALIDATION_GPUAV_SHADERS_TO_INSTRUMENT= (Commented out as the set of dependences is not satisfied)

#! Descriptors indexing - gpuav_descriptor_checks
#! ==========================================
#!  Enable descriptors and buffer out of bounds validation when using descriptor
#!  indexing
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_DESCRIPTOR_CHECKS=true (Commented out as the set of dependences is not satisfied)

#! Post process descriptor indexing - gpuav_post_process_descriptor_indexing
#! ==========================================
#!  Track which descriptor indexes were used in shader to run normal validation
#!  afterwards
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_POST_PROCESS_DESCRIPTOR_INDEXING=true (Commented out as the set of dependences is not satisfied)

#! Out of bounds buffer device addresses - gpuav_buffer_address_oob
#! ==========================================
#!  Check for invalid access using buffer device address
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_BUFFER_ADDRESS_OOB=true (Commented out as the set of dependences is not satisfied)

#! Deprecated and disabled setting - gpuav_max_buffer_device_addresses (DEPRECATED)
#! ==========================================

#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_BUFFER_ADDRESS_OOB=true
#! export VK_KHRONOS_VALIDATION_GPUAV_MAX_BUFFER_DEVICE_ADDRESSES=10000 (Commented out as the set of dependences is not satisfied)

#! RayQuery SPIR-V instructions - gpuav_validate_ray_query
#! ==========================================
#!  Enable shader instrumentation on OpRayQueryInitializeKHR
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_VALIDATE_RAY_QUERY=true (Commented out as the set of dependences is not satisfied)

#! Out of bounds vertex attribute fetching - gpuav_vertex_attribute_fetch_oob
#! ==========================================
#!  Validate that no vertex attribute fetching is out of bonds
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_SHADER_INSTRUMENTATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_VERTEX_ATTRIBUTE_FETCH_OOB=true (Commented out as the set of dependences is not satisfied)

#! Buffer content validation - gpuav_buffers_validation
#! ==========================================
#!  Validate buffers containing parameters used in indirect Vulkan commands, or
#!  used in copy commands
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_BUFFERS_VALIDATION=true (Commented out as the set of dependences is not satisfied)

#! Indirect draws parameters - gpuav_indirect_draws_buffers
#! ==========================================
#!  Validate buffers containing draw parameters used in indirect draw commands.
#!  Includes vkCmdDrawMeshTasks* calls as well
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_BUFFERS_VALIDATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_INDIRECT_DRAWS_BUFFERS=true (Commented out as the set of dependences is not satisfied)

#! Indirect dispatches parameters - gpuav_indirect_dispatches_buffers
#! ==========================================
#!  Validate buffers containing dispatch parameters used in indirect dispatch
#!  commands
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_BUFFERS_VALIDATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_INDIRECT_DISPATCHES_BUFFERS=true (Commented out as the set of dependences is not satisfied)

#! Indirect trace rays parameters - gpuav_indirect_trace_rays_buffers
#! ==========================================
#!  Validate buffers containing ray tracing parameters used in indirect ray
#!  tracing commands
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_BUFFERS_VALIDATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_INDIRECT_TRACE_RAYS_BUFFERS=true (Commented out as the set of dependences is not satisfied)

#! Buffer copies - gpuav_buffer_copies
#! ==========================================
#!  Validate copies involving a VkBuffer. Right now only validates copy buffer to
#!  image.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_BUFFERS_VALIDATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_BUFFER_COPIES=true (Commented out as the set of dependences is not satisfied)

#! Index buffers - gpuav_index_buffers
#! ==========================================
#!  Validate that indexed draws do not fetch indices outside of the bounds of the
#!  index buffer.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! - export VK_KHRONOS_VALIDATION_GPUAV_BUFFERS_VALIDATION=true
#! export VK_KHRONOS_VALIDATION_GPUAV_INDEX_BUFFERS=true (Commented out as the set of dependences is not satisfied)

#! Reserve Descriptor Set Binding Slot - gpuav_reserve_binding_slot (DEPRECATED)
#! ==========================================
#!  Specifies that the validation layers reserve a descriptor set binding slot
#!  for their own use. The layer reports a value for
#!  VkPhysicalDeviceLimits::maxBoundDescriptorSets that is one less than the
#!  value reported by the device. If the device supports the binding of only one
#!  descriptor set, the validation layer does not perform GPU-assisted
#!  validation.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_RESERVE_BINDING_SLOT=true (Commented out as the set of dependences is not satisfied)

#! Linear Memory Allocation Mode - gpuav_vma_linear_output (DEPRECATED)
#! ==========================================
#!  Use VMA linear memory allocations for GPU-AV output buffers instead of
#!  finding best place for new allocations among free regions to optimize memory
#!  usage. Enabling this setting reduces performance cost but disabling this
#!  method minimizes memory usage.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_VMA_LINEAR_OUTPUT=true (Commented out as the set of dependences is not satisfied)

#! Disable all of GPU-AV - gpuav_debug_disable_all
#! ==========================================
#!  Acts as a VkValidationFeatureDisableEXT to override the
#!  VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT passed by the user
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_DEBUG_DISABLE_ALL=false (Commented out as the set of dependences is not satisfied)

#! Validate instrumented shaders - gpuav_debug_validate_instrumented_shaders
#! ==========================================
#!  Run spirv-val after doing shader instrumentation
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_DEBUG_VALIDATE_INSTRUMENTED_SHADERS=false (Commented out as the set of dependences is not satisfied)

#! Dump instrumented shaders - gpuav_debug_dump_instrumented_shaders
#! ==========================================
#!  Will dump the instrumented shaders (before and after) to working directory
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_DEBUG_DUMP_INSTRUMENTED_SHADERS=false (Commented out as the set of dependences is not satisfied)

#! Limit how many time a pass can instrument the SPIR-V - gpuav_debug_max_instrumentations_count
#! ==========================================
#!  Zero is same as unlimited
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_DEBUG_MAX_INSTRUMENTATIONS_COUNT=0 (Commented out as the set of dependences is not satisfied)

#! Print SPIR-V instrumentation info - gpuav_debug_print_instrumentation_info
#! ==========================================
#!  Prints verbose information about the instrumentation of the SPIR-V
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_GPUAV_ENABLE=true
#! export VK_KHRONOS_VALIDATION_GPUAV_DEBUG_PRINT_INSTRUMENTATION_INFO=false (Commented out as the set of dependences is not satisfied)

#! Best Practices - validate_best_practices
#! ==========================================
#!  Outputs warnings related to common misuse of the API, but which are not
#!  explicitly prohibited by the specification.
export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES=false

#! ARM-specific best practices - validate_best_practices_arm
#! ==========================================
#!  Outputs warnings for spec-conforming but non-ideal code on ARM GPUs.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES=true
#! export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES_ARM=false (Commented out as the set of dependences is not satisfied)

#! AMD-specific best practices - validate_best_practices_amd
#! ==========================================
#!  Outputs warnings for spec-conforming but non-ideal code on AMD GPUs.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES=true
#! export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES_AMD=false (Commented out as the set of dependences is not satisfied)

#! IMG-specific best practices - validate_best_practices_img
#! ==========================================
#!  Outputs warnings for spec-conforming but non-ideal code on Imagination GPUs.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES=true
#! export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES_IMG=false (Commented out as the set of dependences is not satisfied)

#! NVIDIA-specific best practices - validate_best_practices_nvidia
#! ==========================================
#!  Outputs warnings for spec-conforming but non-ideal code on NVIDIA GPUs.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES=true
#! export VK_KHRONOS_VALIDATION_VALIDATE_BEST_PRACTICES_NVIDIA=false (Commented out as the set of dependences is not satisfied)

#! Debug Action - debug_action
#! ==========================================
#!  Specifies what action is to be taken when a layer reports information
export VK_KHRONOS_VALIDATION_DEBUG_ACTION=VK_DBG_LAYER_ACTION_LOG_MSG

#! Log Filename - log_filename
#! ==========================================
#!  Specifies the output filename
export VK_KHRONOS_VALIDATION_LOG_FILENAME=stdout

#! Message Severity - report_flags
#! ==========================================
#!  Comma-delineated list of options specifying the types of messages to be
#!  reported
export VK_KHRONOS_VALIDATION_REPORT_FLAGS=error

#! Limit Duplicated Messages - enable_message_limit
#! ==========================================
#!  Enable limiting of duplicate messages.
export VK_KHRONOS_VALIDATION_ENABLE_MESSAGE_LIMIT=true

#! Max Duplicated Messages - duplicate_message_limit
#! ==========================================
#!  Maximum number of times any single validation message should be reported.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_VALIDATION_ENABLE_MESSAGE_LIMIT=true
export VK_KHRONOS_VALIDATION_DUPLICATE_MESSAGE_LIMIT=10

#! Mute Message VUIDs - message_id_filter
#! ==========================================
#!  List of VUIDs and VUID identifiers which are to be IGNORED by the validation
#!  layer
export VK_KHRONOS_VALIDATION_MESSAGE_ID_FILTER=

#! JSON - message_format_json
#! ==========================================
#!  Display Validation as JSON (VkDebugUtilsMessengerCallbackDataEXT::pMessage
#!  will contain JSON)
export VK_KHRONOS_VALIDATION_MESSAGE_FORMAT_JSON=false

#! Display Application Name - message_format_display_application_name
#! ==========================================
#!  Useful when running multiple instances to know which instance the message is
#!  from.
export VK_KHRONOS_VALIDATION_MESSAGE_FORMAT_DISPLAY_APPLICATION_NAME=false


#! VK_LAYER_LUNARG_api_dump 1.4.321

#! Output Range - output_range
#! ==========================================
#!  Comma separated list of frames to output or a range of frames with a start,
#!  count, and optional interval separated by a dash. A count of 0 will output
#!  every frame after the start of the range. Example: "5-8-2" will output frame
#!  5, continue until frame 13, dumping every other frame. Example: "3,8-2" will
#!  output frames 3, 8, and 9.
export VK_LUNARG_API_DUMP_OUTPUT_RANGE=0-0

#! Output Format - output_format
#! ==========================================
#!  Specifies the format used for output; can be HTML, JSON, or  Text (default --
#!  outputs plain text)
export VK_LUNARG_API_DUMP_OUTPUT_FORMAT=text

#! Pre-dump the parameters. - pre_dump
#! ==========================================
#!  Setting this to true causes parameters to calls be logged before the function
#!  as well as after the function.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_API_DUMP_OUTPUT_FORMAT=text
export VK_LUNARG_API_DUMP_PRE_DUMP=false

#! Output to File - file
#! ==========================================
#!  Setting this to true indicates that output should be written to file instead
#!  of stdout
export VK_LUNARG_API_DUMP_FILE=false

#! Log Filename - log_filename
#! ==========================================
#!  Specifies the file to dump to when output files are enabled
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_API_DUMP_FILE=true
#! export VK_LUNARG_API_DUMP_LOG_FILENAME=stdout (Commented out as the set of dependences is not satisfied)

#! Log Flush After Write - flush
#! ==========================================
#!  Setting this to true causes IO to be flushed after each API call that is
#!  written
export VK_LUNARG_API_DUMP_FLUSH=true

#! Name Size - name_size
#! ==========================================
#!  The number of characters the name of a variable should consume, assuming more
#!  are not required
export VK_LUNARG_API_DUMP_NAME_SIZE=32

#! Show Types - show_types
#! ==========================================
#!  Dump types in addition to values
export VK_LUNARG_API_DUMP_SHOW_TYPES=true

#! Type Size - type_size
#! ==========================================
#!  The number of characters the name of a type should consume, assuming more are
#!  not required
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_API_DUMP_SHOW_TYPES=true
export VK_LUNARG_API_DUMP_TYPE_SIZE=0

#! Show Timestamp - timestamp
#! ==========================================
#!  Show the timestamp of function calls since start in microseconds
export VK_LUNARG_API_DUMP_TIMESTAMP=false

#! Show Shader - show_shader
#! ==========================================
#!  Dump the shader binary code in pCode
export VK_LUNARG_API_DUMP_SHOW_SHADER=false

#! Show Parameter Details - detailed
#! ==========================================
#!  Dump parameter details in addition to API calls
export VK_LUNARG_API_DUMP_DETAILED=true

#! Hide Addresses - no_addr
#! ==========================================
#!  Dump "address" in place of hex addresses
export VK_LUNARG_API_DUMP_NO_ADDR=false

#! Use Spaces - use_spaces
#! ==========================================
#!  Setting this to true causes all tab characters to be replaced with spaces
export VK_LUNARG_API_DUMP_USE_SPACES=true

#! Indent Size - indent_size
#! ==========================================
#!  Specifies the number of spaces that a tab is equal to
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_API_DUMP_USE_SPACES=true
export VK_LUNARG_API_DUMP_INDENT_SIZE=4

#! Show Thread and Frame - show_thread_and_frame
#! ==========================================
#!  Show the thread and frame of each function called
export VK_LUNARG_API_DUMP_SHOW_THREAD_AND_FRAME=true


#! VK_LAYER_LUNARG_crash_diagnostic 1.4.321

#! Watchdog timeout (ms) - watchdog_timeout_ms
#! ==========================================
#!  If set to a non-zero number, a watchdog thread will be created. This will
#!  trigger if the application fails to submit new commands within a set time (in
#!  milliseconds) and a log will be created as if the a lost device error was
#!  encountered.
export VK_LUNARG_CRASH_DIAGNOSTIC_WATCHDOG_TIMEOUT_MS=30000

#! Output Path - output_path
#! ==========================================
#!  The directory where dump files and shader binaries are written.
export VK_LUNARG_CRASH_DIAGNOSTIC_OUTPUT_PATH=

#! Dump queue submissions - dump_queue_submits
#! ==========================================
#!  Control which queue submissions are included in the dump.
export VK_LUNARG_CRASH_DIAGNOSTIC_DUMP_QUEUE_SUBMITS=running

#! Dump command buffers - dump_command_buffers
#! ==========================================
#!  Control which command buffers are included in the dump.
export VK_LUNARG_CRASH_DIAGNOSTIC_DUMP_COMMAND_BUFFERS=running

#! Dump commands - dump_commands
#! ==========================================
#!  Control which commands are included in the dump.
export VK_LUNARG_CRASH_DIAGNOSTIC_DUMP_COMMANDS=running

#! Dump shaders - dump_shaders
#! ==========================================
#!  Control of shader dumping.
export VK_LUNARG_CRASH_DIAGNOSTIC_DUMP_SHADERS=off

#! Message Severity - message_severity
#! ==========================================
#!  Comma-delineated list of options specifying the types of log messages to be
#!  reported
export VK_LUNARG_CRASH_DIAGNOSTIC_MESSAGE_SEVERITY=error

#! Log file name - log_file
#! ==========================================
#!  none = no logging, stderr or stdout = to the console, otherwise an absolute
#!  or relative path
export VK_LUNARG_CRASH_DIAGNOSTIC_LOG_FILE=stderr

#! Enable Tracing - trace_on
#! ==========================================
#!  All Vulkan API calls intercepted by the layer will be logged to the console.
export VK_LUNARG_CRASH_DIAGNOSTIC_TRACE_ON=false

#! Enable semaphore log tracing. - trace_all_semaphores
#! ==========================================
#!  Semaphore events will be logged to console.
export VK_LUNARG_CRASH_DIAGNOSTIC_TRACE_ALL_SEMAPHORES=false

#! Synchronize commands - sync_after_commands
#! ==========================================
#!  Add pipeline barriers after instrumented commands.
export VK_LUNARG_CRASH_DIAGNOSTIC_SYNC_AFTER_COMMANDS=false

#! Instrument all commands - instrument_all_commands
#! ==========================================
#!  All commands will be instrumented.
export VK_LUNARG_CRASH_DIAGNOSTIC_INSTRUMENT_ALL_COMMANDS=false

#! Track semaphores - track_semaphores
#! ==========================================
#!  Enable semaphore tracking.
export VK_LUNARG_CRASH_DIAGNOSTIC_TRACK_SEMAPHORES=true


#! VK_LAYER_LUNARG_gfxreconstruct 1.4.321

#! Hotkey Capture Trigger - capture_trigger
#! ==========================================
#!  Specify a hotkey (any one of F1-F12, TAB, CONTROL) that will be used to
#!  start/stop capture. Example: F3 will set the capture trigger to F3 hotkey.
#!  One capture file will be generated for each pair of start/stop hotkey
#!  presses. Default is: Empty string (hotkey capture trigger is disabled).
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_TRIGGER=

#! Hotkey Capture Frame Limit - capture_trigger_frames
#! ==========================================
#!  Specify a limit on the number of frames to be captured via hotkey. Example: 1
#!  will capture exactly one frame when the trigger key is pressed. Default is:
#!  Empty string (no limit)
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_TRIGGER_FRAMES=

#! Capture Specific Frames - capture_frames
#! ==========================================
#!  Specify one or more comma-separated frame ranges to capture. Each range will
#!  be written to its own file. A frame range can be specified as a single value,
#!  to specify a single frame to capture, or as two hyphenated values, to specify
#!  the first and last frame to capture. Frame ranges should be specified in
#!  ascending order and cannot overlap. Note that frame numbering is 1-based
#!  (i.e. the first frame is frame 1). Example: 200,301-305 will create two
#!  capture files, one containing a single frame and one containing five frames.
#!  Default is: Empty string (all frames are captured).
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_FRAMES=

#! Capture File Name - capture_file
#! ==========================================
#!  Path to use when creating the capture file. Default is: gfxrecon_capture.gfxr
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_FILE=gfxrecon_capture.gfxr

#! Capture File Name with Timestamp - capture_file_timestamp
#! ==========================================
#!  Add a timestamp (yyyymmddThhmmss) postfix to the capture file name.
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_FILE_TIMESTAMP=true

#! Capture File Flush After Write - capture_file_flush
#! ==========================================
#!  Flush output stream after each packet is written to the capture file. Default
#!  is: false.
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_FILE_FLUSH=false

#! Compression Format - capture_compression_type
#! ==========================================
#!  Compression format to use with the capture file. Valid values are: LZ4, ZLIB,
#!  ZSTD, and NONE. Default is: LZ4
export VK_LUNARG_GFXRECONSTRUCT_CAPTURE_COMPRESSION_TYPE=LZ4

#! Memory Tracking Mode - memory_tracking_mode
#! ==========================================
#!  Specifies the memory tracking mode to use for detecting modifications to
#!  mapped Vulkan memory objects. Available options are: page_guard, assisted,
#!  and unassisted.
export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard

#! Page Guard Copy on Map - page_guard_copy_on_map
#! ==========================================
#!  When the page_guard memory tracking mode is enabled, copies the content of
#!  the mapped memory to the shadow memory immediately after the memory is
#!  mapped.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard
export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_COPY_ON_MAP=true

#! Page Guard Separate Read Tracking - page_guard_separate_read
#! ==========================================
#!  When the page_guard memory tracking mode is enabled, copies the content of
#!  pages accessed for read from mapped memory to shadow memory on each read. Can
#!  overwrite unprocessed shadow memory content when an application is reading
#!  from and writing to the same page.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard
export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_SEPARATE_READ=true

#! Page Guard External Memory - page_guard_external_memory
#! ==========================================
#!  When the page_guard memory tracking mode is enabled, use the
#!  VK_EXT_external_memory_host extension to eliminate the need for shadow memory
#!  allocations. For each memory allocation from a host visible memory type, the
#!  capture layer will create an allocation from system memory, which it can
#!  monitor for write access, and provide that allocation to vkAllocateMemory as
#!  external memory. Only available on Windows.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard
export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_EXTERNAL_MEMORY=false

#! Page Guard Persistent Memory - page_guard_persistent_memory
#! ==========================================
#!  When the page_guard memory tracking mode is enabled, this option changes the
#!  way that the shadow memory used to detect modifications to mapped memory is
#!  allocated. The default behavior is to allocate and copy the mapped memory
#!  range on map and free the allocation on unmap. When this option is enabled,
#!  an allocation with a size equal to that of the object being mapped is made
#!  once on the first map and is not freed until the object is destroyed. This
#!  option is intended to be used with applications that frequently map and unmap
#!  large memory ranges, to avoid frequent allocation and copy operations that
#!  can have a negative impact on performance. This option is ignored when
#!  GFXRECON_PAGE_GUARD_EXTERNAL_MEMORY is enabled.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard
#! - export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_EXTERNAL_MEMORY=false
export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_PERSISTENT_MEMORY=false

#! Page Guard Align Buffer Sizes - page_guard_align_buffer_sizes
#! ==========================================
#!  When the page_guard memory tracking mode is enabled, this option overrides
#!  the Vulkan API calls that report buffer memory properties to report that
#!  buffer sizes and alignments must be a multiple of the system page size. This
#!  option is intended to be used with applications that perform CPU writes and
#!  GPU writes/copies to different buffers that are bound to the same page of
#!  mapped memory, which may result in data being lost when copying pages from
#!  the page_guard shadow allocation to the real allocation. This data loss can
#!  result in visible corruption during capture. Forcing buffer sizes and
#!  alignments to a multiple of the system page size prevents multiple buffers
#!  from being bound to the same page, avoiding data loss from simultaneous CPU
#!  writes to the shadow allocation and GPU writes to the real allocation for
#!  different buffers bound to the same page. This option is only available for
#!  the Vulkan API.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard
#! - export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_EXTERNAL_MEMORY=false
export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_ALIGN_BUFFER_SIZES=true

#! Page Guard unblock SIGSEGV from thread's signal mask - page_guard_unblock_sigsegv
#! ==========================================
#!  When the page_guard memory tracking mode is enabled, this option overrides
#!  enables SIGSEGV in thread's signal mask in the case that it has been
#!  disabled.
#! This setting requires ALL of the following values:
#! - export VK_LUNARG_GFXRECONSTRUCT_MEMORY_TRACKING_MODE=page_guard
#! - export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_EXTERNAL_MEMORY=false
export VK_LUNARG_GFXRECONSTRUCT_PAGE_GUARD_UNBLOCK_SIGSEGV=false

#! Level - log_level
#! ==========================================
#!  Specify the minimum severity level of messages to log. Options are: debug,
#!  info, warning, error, and fatal. The specified level and all higher levels
#!  will be enabled for logging. For example, choosing the warning level will
#!  also enable the error and fatal levels.
export VK_LUNARG_GFXRECONSTRUCT_LOG_LEVEL=info

#! Log Name and Line Number - log_detailed
#! ==========================================
#!  Include name and line number from the file responsible.
export VK_LUNARG_GFXRECONSTRUCT_LOG_DETAILED=false

#! Apply additional indentation - log_allow_indents
#! ==========================================
#!  Apply additional indentation formatting to log messages.
export VK_LUNARG_GFXRECONSTRUCT_LOG_ALLOW_INDENTS=false

#! Log Output to Console / stdout - log_output_to_console
#! ==========================================
#!  Log messages will be written to stdout.
export VK_LUNARG_GFXRECONSTRUCT_LOG_OUTPUT_TO_CONSOLE=true

#! Trigger Debug Break on Error - log_break_on_error
#! ==========================================
#!  Trigger a debug break when logging an error.
export VK_LUNARG_GFXRECONSTRUCT_LOG_BREAK_ON_ERROR=false

#! Log Output to Debug Console - log_output_to_os_debug_string
#! ==========================================
#!  Windows only option. Log messages will be written to the Debug Console with
#!  OutputDebugStringA
export VK_LUNARG_GFXRECONSTRUCT_LOG_OUTPUT_TO_OS_DEBUG_STRING=false

#! Log File - log_file
#! ==========================================
#!  When set, log messages will be written to a file at the specified path.
#!  Default is: Empty string (file logging disabled).
export VK_LUNARG_GFXRECONSTRUCT_LOG_FILE=

#! Log File Flush After Write - log_file_flush_after_write
#! ==========================================
#!  Flush the log file to disk after each write when true.
export VK_LUNARG_GFXRECONSTRUCT_LOG_FILE_FLUSH_AFTER_WRITE=false

#! Log File Keep Open - log_file_keep_open
#! ==========================================
#!  Keep the log file open between log messages when true, or close and reopen
#!  the log file for each message when false.
export VK_LUNARG_GFXRECONSTRUCT_LOG_FILE_KEEP_OPEN=true

#! Log File Overwrite - log_file_create_new
#! ==========================================
#!  Specifies that log file initialization should overwrite an existing file when
#!  true, or append to an existing file when false.
export VK_LUNARG_GFXRECONSTRUCT_LOG_FILE_CREATE_NEW=true


#! VK_LAYER_LUNARG_monitor 1.4.321


#! VK_LAYER_LUNARG_screenshot 1.4.321

#! Frames - frames
#! ==========================================
#!  Specifies list of frames to output as screenshots. It is specified as a
#!  comma-separated list of frames or a range of frames with a start, count, and
#!  optional interval separated by a dash. Setting the variable to "all" will
#!  output every frame. Example: "5-8-2" will output frame 5, continue until
#!  frame 13, dumping every other frame. Example: "3,8-2" will output frames 3,
#!  8, and 9. If it is not set or it set to an empty string, no screenshots are
#!  created.
export VK_LUNARG_SCREENSHOT_FRAMES=

#! Directory - dir
#! ==========================================
#!  Specifies the directory in which to create the screenshot files. If it is not
#!  set or is set to an empty string, the files will be created in the current
#!  working directory.
export VK_LUNARG_SCREENSHOT_DIR=

#! Format - format
#! ==========================================
#!  Specify a color space for the output. If it is not set, set to null, or set
#!  to USE_SWAPCHAIN_COLORSPACE the format will be set to use the same color
#!  space as the swapchain object.
export VK_LUNARG_SCREENSHOT_FORMAT=USE_SWAPCHAIN_COLORSPACE


#! VK_LAYER_KHRONOS_profiles 1.4.321

#! Force Device - force_device
#! ==========================================
#!  On system with multiple physical devices, force the use of one specific
#!  physical device.
export VK_KHRONOS_PROFILES_FORCE_DEVICE=FORCE_DEVICE_OFF

#! Device UUID - force_device_uuid
#! ==========================================
#!  Device UUID of the forced physical devices
export VK_KHRONOS_PROFILES_FORCE_DEVICE_UUID=

#! Device Name - force_device_name
#! ==========================================
#!  Device Name of the forced physical devices
export VK_KHRONOS_PROFILES_FORCE_DEVICE_NAME="${VP_PHYSICAL_DEVICES}"

#! Emulate a Vulkan Profile - profile_emulation
#! ==========================================
#!  Emulate a Vulkan Profile on the Developer System
export VK_KHRONOS_PROFILES_PROFILE_EMULATION=true

#! Loading Directory - profile_dirs
#! ==========================================
#!  Select the location all the profiles.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_PROFILE_EMULATION=true
export VK_KHRONOS_PROFILES_PROFILE_DIRS=

#! Name - profile_name
#! ==========================================
#!  Name of the profile specified by the profile file to use.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_PROFILE_EMULATION=true
export VK_KHRONOS_PROFILES_PROFILE_NAME=${VP_DEFAULT}

#! Schema Validation - profile_validation
#! ==========================================
#!  Validate the profile files against the Vulkan SDK profile schema if the file
#!  is found.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_PROFILE_EMULATION=true
export VK_KHRONOS_PROFILES_PROFILE_VALIDATION=false

#! Simulate Profile Capabilities - simulate_capabilities
#! ==========================================
#!  Control of the simulated capabilities of the Vulkan physical device from the
#!  selected Vulkan Profile.
export VK_KHRONOS_PROFILES_SIMULATE_CAPABILITIES=SIMULATE_API_VERSION_BIT,SIMULATE_FEATURES_BIT,SIMULATE_PROPERTIES_BIT

#! Unspecified Features - default_feature_values
#! ==========================================
#!  Feature values when not specified in the select Vulkan profiles.
export VK_KHRONOS_PROFILES_DEFAULT_FEATURE_VALUES=DEFAULT_FEATURE_VALUES_DEVICE

#! Emulate VK_KHR_portability_subset - emulate_portability
#! ==========================================
#!  Emulate the VK_KHR_portability_subset extension on the device.
export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true

#! constantAlphaColorBlendFactors - constantAlphaColorBlendFactors
#! ==========================================
#!  Indicates whether this implementation supports constant alpha Blend Factors
#!  used as source or destination color Blending.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_CONSTANTALPHACOLORBLENDFACTORS=false

#! events - events
#! ==========================================
#!  Indicates whether this implementation supports synchronization using Events
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_EVENTS=false

#! imageViewFormatReinterpretation - imageViewFormatReinterpretation
#! ==========================================
#!  Indicates whether this implementation supports a VkImageView being created
#!  with a texel format containing a different number of components, or a
#!  different number of bits in each component, than the texel format of the
#!  underlying VkImage.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_IMAGEVIEWFORMATREINTERPRETATION=false

#! imageViewFormatSwizzle - imageViewFormatSwizzle
#! ==========================================
#!  Indicates whether this implementation supports remapping format components
#!  using VkImageViewCreateInfo::components.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_IMAGEVIEWFORMATSWIZZLE=false

#! imageView2DOn3DImage - imageView2DOn3DImage
#! ==========================================
#!  Indicates whether this implementation supports a VkImage being created with
#!  the VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT flag set, permitting a 2D or 2D
#!  array image view to be created on a 3D VkImage.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_IMAGEVIEW2DON3DIMAGE=false

#! multisampleArrayImage - multisampleArrayImage
#! ==========================================
#!  Indicates whether this implementation supports a VkImage being created as a
#!  2D array with multiple samples per texel.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_MULTISAMPLEARRAYIMAGE=false

#! mutableComparisonSamplers - mutableComparisonSamplers
#! ==========================================
#!  Indicates whether this implementation allows descriptors with comparison
#!  samplers to be updated.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_MUTABLECOMPARISONSAMPLERS=false

#! pointPolygons - pointPolygons
#! ==========================================
#!  Indicates whether this implementation supports Rasterization using a point
#!  Polygon Mode.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_POINTPOLYGONS=false

#! samplerMipLodBias - samplerMipLodBias
#! ==========================================
#!  Indicates whether this implementation supports setting a mipmap LOD bias
#!  value when creating a sampler.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_SAMPLERMIPLODBIAS=false

#! separateStencilMaskRef - separateStencilMaskRef
#! ==========================================
#!  Indicates whether this implementation supports separate front and back
#!  Stencil Test reference values.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_SEPARATESTENCILMASKREF=false

#! shaderSampleRateInterpolationFunctions - shaderSampleRateInterpolationFunctions
#! ==========================================
#!  Indicates whether this implementation supports fragment shaders which use the
#!  InterpolationFunction capability.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_SHADERSAMPLERATEINTERPOLATIONFUNCTIONS=false

#! tessellationIsolines - tessellationIsolines
#! ==========================================
#!  Indicates whether this implementation supports isoline output from the
#!  Tessellation stage of a graphics pipeline. This member is only meaningful if
#!  tessellationShader are supported.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_TESSELLATIONISOLINES=false

#! triangleFans - triangleFans
#! ==========================================
#!  Indicates whether this implementation supports Triangle Fans primitive
#!  topology.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_TRIANGLEFANS=false

#! vertexAttributeAccessBeyondStride - vertexAttributeAccessBeyondStride
#! ==========================================
#!  Indicates whether this implementation supports accessing a vertex input
#!  attribute beyond the stride of the corresponding vertex input binding.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_EMULATE_PORTABILITY=true
export VK_KHRONOS_PROFILES_VERTEXATTRIBUTEACCESSBEYONDSTRIDE=false

#! minVertexInputBindingStrideAlignment - minVertexInputBindingStrideAlignment
#! ==========================================
#!  Indicates whether this implementation supports accessing a vertex input
#!  attribute beyond the stride of the corresponding vertex input binding.
#! This setting requires ALL of the following values:
#! - export VK_KHRONOS_PROFILES_VERTEXATTRIBUTEACCESSBEYONDSTRIDE=false
export VK_KHRONOS_PROFILES_MINVERTEXINPUTBINDINGSTRIDEALIGNMENT=4

#! Exclude Device Extensions - exclude_device_extensions
#! ==========================================
#!  Removes the listed device extensions from being reported by the Vulkan
#!  physical device.
export VK_KHRONOS_PROFILES_EXCLUDE_DEVICE_EXTENSIONS=

#! Exclude Formats - exclude_formats
#! ==========================================
#!  Removes the format feature flags from being reported by the Vulkan physical
#!  device.
export VK_KHRONOS_PROFILES_EXCLUDE_FORMATS=

#! Debug Actions - debug_actions
#! ==========================================
#!  This indicates what action is to be taken when a layer wants to report
#!  information
export VK_KHRONOS_PROFILES_DEBUG_ACTIONS=DEBUG_ACTION_STDOUT_BIT

#! Log Filename - debug_filename
#! ==========================================
#!  Specifies the output filename
export VK_KHRONOS_PROFILES_DEBUG_FILENAME=profiles_layer_log.txt

#! Clear Log at Launch - debug_file_clear
#! ==========================================
#!  Discard the content of the log file between each layer run
export VK_KHRONOS_PROFILES_DEBUG_FILE_CLEAR=true

#! Fail on Error - debug_fail_on_error
#! ==========================================
#!  If the device doesn't support the capabilities of the selected Profile, the
#!  layer fail to load. When enabled, vkEnumeratePhysicalDevices will fail when
#!  the selected Profile is not supported.
export VK_KHRONOS_PROFILES_DEBUG_FAIL_ON_ERROR=false

#! Message Types - debug_reports
#! ==========================================
#!  This is a comma-delineated list of options telling the layer what types of
#!  messages it should report back
export VK_KHRONOS_PROFILES_DEBUG_REPORTS=DEBUG_REPORT_NOTIFICATION_BIT,DEBUG_REPORT_WARNING_BIT,DEBUG_REPORT_ERROR_BIT


#! VK_LAYER_KHRONOS_shader_object 1.4.321

#! Force Enable - force_enable
#! ==========================================
#!  Force the layer to be active even if the underlying driver also implements
#!  the shader object extension.
export VK_KHRONOS_SHADER_OBJECT_FORCE_ENABLE=false

#! Disable Pipeline Pre-Caching - disable_pipeline_pre_caching
#! ==========================================
#!  Disable the layer from pre-caching pipelines, reducing the memory overhead.
export VK_KHRONOS_SHADER_OBJECT_DISABLE_PIPELINE_PRE_CACHING=false


#! VK_LAYER_KHRONOS_synchronization2 1.4.321

#! Force Enable - force_enable
#! ==========================================
#!  Force the layer to be active even if the underlying driver also implements
#!  the synchonization2 extension.
export VK_KHRONOS_SYNCHRONIZATION2_FORCE_ENABLE=false

