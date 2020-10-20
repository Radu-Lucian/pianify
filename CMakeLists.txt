cmake_minimum_required(VERSION 3.11)

project(Pianify VERSION 1.0.1 LANGUAGES CXX)

option(USE_BOOST "Do you wish to build Boost? Enabling this option will also build the Client project." OFF)
option(BUILD_GUI "Do you want to build the graphical user interface?" ON)
option(BUILD_CLI "Build the command line interface?" OFF)

if(BUILD_GUI)
	set(Qt5_DIR "C:/Qt/5.15.0/msvc2019_64/lib/cmake/Qt5")
endif()

if(USE_BOOST)
	set(FETCHCONTENT_SOURCE_DIR_BOOST "C:/boost/boost_1_71_0/boost_1_71_0")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(AUDIO_SAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/audio_samples")
set(AUDIO_TEST_SAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/audio_test_samples")
set(MIDI_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/audio_samples/midi_generated")
set(DESIGN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/design")
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(MODELS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/model")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/directories.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config/directories.h)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options("-bigobj")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_options("/bigobj")
endif()

if (UNIX)
	set(CMAKE_CXX_FLAGS -pthread)
endif (UNIX)

add_subdirectory(src)
add_subdirectory(third_party)