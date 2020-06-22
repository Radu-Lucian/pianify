# Ooura FFT
enable_language(C)
set(Ooura_fft_SOURCES ooura/fft4g.c)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")

add_library(Ooura_fft ${Ooura_fft_SOURCES})

# UnitTest++
if(Aquila_BUILD_TESTS)
    add_subdirectory(unittestpp)
endif()
