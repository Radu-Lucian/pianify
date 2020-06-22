set(CURRENT_PROJECT GUI)
set(INCROOT ${PROJECT_SOURCE_DIR}/include/${CURRENT_PROJECT})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${CURRENT_PROJECT})

file(GLOB INCS "${INCROOT}/*.h")
file(GLOB SRCS "${SRCROOT}/*.cpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")

find_package(Qt5 COMPONENTS Core Widgets Multimedia PrintSupport REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${Pianify_SOURCE_DIR}/design")

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Multimedia_DEFINITIONS})

if(WIN32)
	add_executable(${CURRENT_PROJECT} WIN32 ${SRCS} ${INCS})
else()
	add_executable(${CURRENT_PROJECT} ${SRCS} ${INCS})
endif()

target_compile_features(${CURRENT_PROJECT} PRIVATE cxx_std_17)
target_include_directories(${CURRENT_PROJECT} PRIVATE 
    ${Qt5Multimedia_INCLUDES}
    ${PROJECT_SOURCE_DIR}/third_party/aquila
    ${PROJECT_SOURCE_DIR}/third_party/midifile/include
    ${PROJECT_SOURCE_DIR}/include/InstrumentRecognizer
    ${PROJECT_SOURCE_DIR}/include/Converter
    ${PROJECT_SOURCE_DIR}/include/DatasetHelper
    ${PROJECT_SOURCE_DIR}/third_party/AudioFile
    ${PROJECT_SOURCE_DIR}/third_party/QCustomPlot
    ${PROJECT_SOURCE_DIR}/include/${CURRENT_PROJECT}
	${PROJECT_SOURCE_DIR}/config
    )
target_link_libraries(${CURRENT_PROJECT} AudioFile DatasetHelper InstrumentRecognizer Converter Qt5::Widgets Qt5::Core QCustomPlot Aquila Qt5::Multimedia)

add_custom_command(TARGET ${CURRENT_PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${CURRENT_PROJECT}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${CURRENT_PROJECT}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${CURRENT_PROJECT}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Multimedia> $<TARGET_FILE_DIR:${CURRENT_PROJECT}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::PrintSupport> $<TARGET_FILE_DIR:${CURRENT_PROJECT}>
    )