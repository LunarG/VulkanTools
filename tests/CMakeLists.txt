set(apidump_test_script "${CMAKE_CURRENT_LIST_DIR}/apidump_test.sh")
if (WIN32)
    set(apidump_test_script "${CMAKE_CURRENT_LIST_DIR}/apidump_test.ps1")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${apidump_test_script} ${CMAKE_CURRENT_BINARY_DIR})
