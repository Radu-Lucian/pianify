set(INCROOT ${PROJECT_SOURCE_DIR}/third_party/QCustomPlot)
set(SRCROOT ${PROJECT_SOURCE_DIR}/third_party/QCustomPlot)

# all source files
file(GLOB INCS "${INCROOT}/*.h")
file(GLOB SRCS "${SRCROOT}/*.cpp")

#set(Qt5_DIR "C:/Qt/5.9.2/msvc2017_64/lib/cmake/Qt5")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Pianify_SOURCE_DIR}/lib")

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${Pianify_SOURCE_DIR}/design")

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
include_directories("${Pianify_SOURCE_DIR}/bin")

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5PrintSupport_DEFINITIONS})
add_definitions(-DQCUSTOMPLOT_COMPILE_LIBRARY)

add_library(QCustomPlot SHARED ${SRCS} ${INCS})

target_link_libraries(QCustomPlot)

qt5_use_modules(QCustomPlot Widgets PrintSupport)