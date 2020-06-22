set(CURRENT_PROJECT Tests)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${CURRENT_PROJECT})

file(GLOB SRCS "${SRCROOT}/*.cpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")

add_executable(${CURRENT_PROJECT} ${SRCS})

target_compile_features(${CURRENT_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${CURRENT_PROJECT} 
	PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/InstrumentRecognizer
    ${PROJECT_SOURCE_DIR}/include/Converter
	${PROJECT_SOURCE_DIR}/third_party/Catch2
	${PROJECT_SOURCE_DIR}/config)

target_link_libraries(${CURRENT_PROJECT} Converter InstrumentRecognizer)