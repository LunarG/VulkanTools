
if (NOT WIN32)
    # extra setup for out-of-tree builds
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        add_custom_target(vt_test-dir-symlinks ALL
            COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/vlf_test.sh
            COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/apidump_test.sh
            VERBATIM
            )
        set_target_properties(vt_test-dir-symlinks PROPERTIES FOLDER ${VULKANTOOLS_TARGET_FOLDER})
    endif()
else()
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vlf_test.ps1 VKVLFTEST)
        FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/apidump_test.ps1 VKAPIDUMPTEST)
        add_custom_target(vt_test-dir-symlinks ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VKVLFTEST} vlf_test.ps1
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VKAPIDUMPTEST} apidump_test.ps1
            VERBATIM
            )
    endif()
endif()
