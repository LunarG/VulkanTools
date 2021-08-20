cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 11)

function(DevsimTest NAME)
	set(TEST_FILE ./${NAME}.cpp)
    set(TEST_NAME devsim_${NAME})
    
    file(GLOB FILES_JSON ./*.json)

    add_executable(${TEST_NAME} ${TEST_FILE} ${FILES_JSON} devsim_test_helper.h devsim_test_helper.cpp ${JSONCPP_SOURCE_DIR}/jsoncpp.cpp)
    target_link_libraries(${TEST_NAME} gtest gtest_main Vulkan::Vulkan)
    target_compile_definitions(${TEST_NAME} PUBLIC TEST_SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
    target_compile_definitions(${TEST_NAME} PUBLIC TOP_SOURCE_PATH="${CMAKE_SOURCE_DIR}")
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction(DevsimTest)

DevsimTest(test_array_combination_modes)
DevsimTest(test_physical_device_extended_property_modification)
DevsimTest(test_physical_device_main_property_modification)