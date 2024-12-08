cmake_minimum_required(VERSION  3.13)

project(WaveAnalyzer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES 
sysio/csv_handler.cpp
sysio/csv_handler.hpp
signal/processing.cpp 
signal/processing.hpp 
common.hpp
main.cpp
)

set(FFT_DIR "${CMAKE_SOURCE_DIR}/3rdparty/fftw-3.3.5-dll64/")
set(FFT_LIB "${CMAKE_SOURCE_DIR}/3rdparty/fftw-3.3.5-dll64/libfftw3-3.lib")
message(${FFT_LIB})
add_executable(WaveAnalyzer ${SOURCES})

target_include_directories(WaveAnalyzer PRIVATE
    ${FFT_DIR}
    ${CMAKE_SOURCE_DIR}/sysio
    ${CMAKE_SOURCE_DIR}/signal
	
)

target_link_libraries(WaveAnalyzer ${FFT_LIB} )

# Add the FFTW DLL to the output folder after the build
# add_custom_command(TARGET WaveAnalyzer POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${FFT_DIR}/libfftw3-3.dll  # Source path to the DLL
#     $<TARGET_FILE_DIR:WaveAnalyzer>/libfftw3-3.dll  # Destination path (same directory as executable)
# )

