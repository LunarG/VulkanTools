set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    include(GNUInstallDirs)
endif()

function(vkConfigTest NAME)
	set(TEST_FILE ./${NAME}.cpp)
    set(TEST_NAME vkconfig_${NAME})

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} vkconfig_core gtest gtest_main Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
    if(WIN32)
        target_link_libraries(${TEST_NAME} Cfgmgr32)
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction(vkConfigTest)

vkConfigTest(test_util)
vkConfigTest(test_version)
vkConfigTest(test_command_line)
vkConfigTest(test_layer_setting)
vkConfigTest(test_layer_type)


