#This file can be used to build binder using preinstalled LLVM/Clang
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(binder CXX C)
SET(VERSION 1.0.0)
option(STATIC "Statically compile Binder. See `documentation/install.rst` for more information." OFF)
#This actually will work even with C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
#So far there are exceptions in config.cpp
set(LLVM_REQUIRES_EH ON)
set(USE_EXTERNAL_LLVM ON)
include("GNUInstallDirs")
set(LLVM_DIR_ORIG ${LLVM_DIR})
set(Clang_DIR_ORIG ${Clang_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
SET(CMAKE_MODULE_PATH_ORIG "${CMAKE_MODULE_PATH}")
set (CMAKE_MODULE_PATH "${LLVM_DIR};${Clang_DIR};${CMAKE_MODULE_PATH}")
option(BINDER_ENABLE_TEST      "Enables building of tests" OFF)
option(BINDER_MOCK_TEST        "Mock running of binder on tests" OFF)
option(BINDER_USE_PYTHON_IN_TEST  "Try to import python modules during tests" ON)
if (NOT DEFINED BINDER_TEST_PYTHON_VERSIONS)
  set(BINDER_TEST_PYTHON_VERSIONS "0,2.7,3")
endif()
MESSAGE(STATUS  "binder: BINDER_ENABLE_TEST=${BINDER_ENABLE_TEST} with BINDER_TEST_PYTHON_VERSIONS=${BINDER_TEST_PYTHON_VERSIONS}. Note that version 0 can be used to check the generated sources agains the references.")
MESSAGE(STATUS  "binder: BINDER_USE_PYTHON_IN_TEST=${BINDER_USE_PYTHON_IN_TEST}")
MESSAGE(STATUS  "binder: Using LLVM_DIR=${LLVM_DIR} to search for cmake configurations.")
MESSAGE(STATUS  "binder: Using Clang_DIR=${Clang_DIR} to search for cmake configurations.")
MESSAGE(STATUS  "binder: Using CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH} to search for cmake configurations.")
# find_package(Clang CONFIG QUIET PATHS /usr/lib64/cmake/clang /usr/lib/cmake/clang /usr/share/clang/cmake )

set(CONFIG_OUTPUT)
find_program(LLVM_CONFIG "llvm-config")
if(LLVM_CONFIG)
    message(STATUS "Found LLVM_CONFIG as ${LLVM_CONFIG}")
    set(CONFIG_COMMAND ${LLVM_CONFIG}
        "--assertion-mode"
        "--bindir"
        "--libdir"
        "--includedir"
        "--prefix"
        "--src-root")
    execute_process(COMMAND ${CONFIG_COMMAND}
                    RESULT_VARIABLE HAD_ERROR
                    OUTPUT_VARIABLE CONFIG_OUTPUT)
    if(NOT HAD_ERROR)
        string(REGEX REPLACE
               "[ \t]*[\r\n]+[ \t]*" ";"
               CONFIG_OUTPUT ${CONFIG_OUTPUT})
    else()
        string(REPLACE ";" " " CONFIG_COMMAND_STR "${CONFIG_COMMAND}")
        message(STATUS "${CONFIG_COMMAND_STR}")
        message(FATAL_ERROR "llvm-config failed with status ${HAD_ERROR}")
    endif()
else()
    message(FATAL_ERROR "llvm-config not found -- ${LLVM_CONFIG}")
endif()

list(GET CONFIG_OUTPUT 0 ENABLE_ASSERTIONS)
list(GET CONFIG_OUTPUT 1 TOOLS_BINARY_DIR)
list(GET CONFIG_OUTPUT 2 LIBRARY_DIR)
list(GET CONFIG_OUTPUT 3 INCLUDE_DIR)
list(GET CONFIG_OUTPUT 4 LLVM_OBJ_ROOT)
list(GET CONFIG_OUTPUT 5 MAIN_SRC_DIR)

set(LLVM_TOOLS_BINARY_DIR ${TOOLS_BINARY_DIR} CACHE PATH "Path to llvm/bin")
set(LLVM_LIBRARY_DIR ${LIBRARY_DIR} CACHE PATH "Path to llvm/lib")
set(LLVM_MAIN_INCLUDE_DIR ${INCLUDE_DIR} CACHE PATH "Path to llvm/include")
set(LLVM_BINARY_DIR ${LLVM_OBJ_ROOT} CACHE PATH "Path to LLVM build tree")
set(LLVM_MAIN_SRC_DIR ${MAIN_SRC_DIR} CACHE PATH "Path to LLVM source tree")

include_directories(${LLVM_MAIN_INCLUDE_DIR})
link_directories(${LLVM_LIBRARY_DIR})
add_definitions(-DLLVM_DIR="${LLVM_BINARY_DIR}")

find_package(Clang REQUIRED)

macro(add_clang_executable name)
  add_executable( ${name} ${ARGN} )
#  set_target_properties(${name} PROPERTIES FOLDER "Clang executables")
    message(STATUS "binder: We have cmake functions to update LLVM_COMPILE_FLAGS")
    if (LLVM_COMPILER_IS_GCC_COMPATIBLE)
      list(APPEND LLVM_COMPILE_FLAGS "-fno-rtti")
    elseif (MSVC)
      list(APPEND LLVM_COMPILE_FLAGS "/GR-")
    endif ()
endmacro(add_clang_executable)
include_directories(source)
add_subdirectory(source)
if (BINDER_ENABLE_TEST)
 if(${CMAKE_VERSION} VERSION_LESS "3.0.0")
   message("You are running cmake version ${CMAKE_VERSION}.")
   message("The testing suite will be disabled as it requires cmake 3.0.0 or higher.")
 else()
   ENABLE_TESTING()
   add_subdirectory(test)
 endif()
endif()
