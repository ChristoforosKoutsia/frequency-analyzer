cmake_minimum_required(VERSION  3.13)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.3/mingw_64")

project(SpectraForge VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# This is required for Qt to work properly with CMake
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
##############################################33


file(GLOB_RECURSE GUI_SOURCES
    "${CMAKE_SOURCE_DIR}/gui/*.cpp"
    "${CMAKE_SOURCE_DIR}/gui/*.hpp"
)

set(SOURCES 
${GUI_SOURCES}
sysio/csv_handler.cpp
sysio/csv_handler.hpp
signal/processing.cpp 
signal/processing.hpp
gui/main_gui.cpp
common.hpp
main.cpp
)

set(FFT_DIR "${CMAKE_SOURCE_DIR}/3rdparty/fftw-3.3.5-dll64/")
set(FFT_LIB "${CMAKE_SOURCE_DIR}/3rdparty/fftw-3.3.5-dll64/libfftw3-3.lib")
message(${FFT_LIB})
add_executable(SpectraForge ${SOURCES})

target_include_directories(SpectraForge PRIVATE
    ${FFT_DIR}
    ${CMAKE_SOURCE_DIR}/sysio
    ${CMAKE_SOURCE_DIR}/signal
    ${CMAKE_SOURCE_DIR}/gui/Widgets/include
    ${CMAKE_SOURCE_DIR}/gui
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/gui/Platform/include
)

message("FFT_LIB: ${FFT_LIB}")

# Find the Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)


target_link_libraries(SpectraForge Qt6::Widgets Qt6::Charts ${FFT_LIB} )

# Add the FFTW DLL to the output folder after the build
 add_custom_command(TARGET SpectraForge POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy_if_different
     ${FFT_DIR}/libfftw3-3.dll  # Source path to the DLL
     $<TARGET_FILE_DIR:SpectraForge>/libfftw3-3.dll  # Destination path (same directory as executable)
 )


# Copy styles folder
add_custom_command(TARGET SpectraForge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/styles $<TARGET_FILE_DIR:SpectraForge>/styles
)

# Copy icons folder
add_custom_command(TARGET SpectraForge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/icons $<TARGET_FILE_DIR:SpectraForge>/icons
)


# Make the application only executable and NOT a console application
set_target_properties(SpectraForge PROPERTIES WIN32_EXECUTABLE TRUE)
