cmake_minimum_required(VERSION 3.28)

project(tuim VERSION 1.0.0 LANGUAGES CXX)

###############################################
#  Compile executables in debug by default    #
###############################################
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

###############################################
#  Options                                    #
###############################################
option(TUIM_BUILD_TESTING "Build the testing suite" ON)
option(TUIM_BUILD_EXAMPLES "Build the examples" ON)
option(TUIM_RUN_TESTS_POST_BUILD "Automatically run tests after compiling" ON)

###############################################
#  Global C++ configuration                   #
###############################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################
#  backward-cpp                               #
###############################################
include(FetchContent)
FetchContent_Declare(backward
    GIT_REPOSITORY https://github.com/bombela/backward-cpp
    GIT_TAG master
)
FetchContent_MakeAvailable(backward)
set_target_properties(backward PROPERTIES FOLDER "vendor")

###############################################
#  Target: library                            #
###############################################
add_library(tuim INTERFACE)
add_library(tuim::tuim ALIAS tuim)

target_include_directories(tuim INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

###############################################
#  Target: tests                              #
###############################################
if(TUIM_BUILD_TESTING)
    FetchContent_Declare(
      doctest
      GIT_REPOSITORY https://github.com/doctest/doctest
      GIT_TAG v2.4.12
    )
    FetchContent_MakeAvailable(doctest)

    enable_testing()

    add_executable(tests tests/tests.cpp)
    add_executable(tuim::tests ALIAS tests)

    if (UNIX)
        target_compile_definitions(tests INTERFACE
            BACKWARD_HAS_DW=1
            BACKWARD_HAS_BFD=1
            BACKWARD_HAS_DWARF=1
        )
    endif()

    target_link_libraries(tests PRIVATE tuim::tuim)
    target_link_libraries(tests PUBLIC Backward::Object doctest::doctest)

    # Standard CTest registration
    add_test(NAME AllTests COMMAND tests)

    # Automation: Run after compilation
    if(TUIM_RUN_TESTS_POST_BUILD)
        add_custom_command(TARGET tests POST_BUILD
            COMMAND tests
            COMMENT "Running unit tests..."
        )
    endif()
endif()

###############################################
#  Target: examples                           #
###############################################
if(TUIM_BUILD_EXAMPLES)
    file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
    foreach(SOURCE_FILE ${EXAMPLE_SOURCES})
        get_filename_component(EXAMPLE_NAME ${SOURCE_FILE} NAME_WE)

        add_executable(${EXAMPLE_NAME} ${SOURCE_FILE})
        add_executable(tuim::${EXAMPLE_NAME} ALIAS ${EXAMPLE_NAME})

        if (UNIX)
            target_compile_definitions(${EXAMPLE_NAME} INTERFACE
                BACKWARD_HAS_DW=1
                BACKWARD_HAS_BFD=1
                BACKWARD_HAS_DWARF=1
            )
        endif()

        target_link_libraries(${EXAMPLE_NAME} PRIVATE tuim::tuim)
        target_link_libraries(${EXAMPLE_NAME} PUBLIC Backward::Object)
    endforeach()
endif()