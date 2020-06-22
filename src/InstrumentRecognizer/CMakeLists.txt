set(CURRENT_PROJECT InstrumentRecognizer)
set(INCROOT ${PROJECT_SOURCE_DIR}/include/${CURRENT_PROJECT})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${CURRENT_PROJECT})

file(GLOB INCS "${INCROOT}/*.h" "${INCROOT}/*.hpp")
file(GLOB SRCS "${SRCROOT}/*.cpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")

add_library(${CURRENT_PROJECT} SHARED ${SRCS} ${INCS})

target_compile_features(${CURRENT_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${CURRENT_PROJECT}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include/${CURRENT_PROJECT}
	${PROJECT_SOURCE_DIR}/config
    PUBLIC
    ${PROJECT_SOURCE_DIR}/third_party/tiny_dnn)
