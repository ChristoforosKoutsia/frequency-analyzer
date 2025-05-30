#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QuickControls2Imagine" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::QuickControls2Imagine APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::QuickControls2Imagine PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libQt6QuickControls2Imagine.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Qt6::QuickControls2ImagineStyleImpl;Qt6::Qml;Qt6::Core"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6QuickControls2Imagine.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::QuickControls2Imagine )
list(APPEND _cmake_import_check_files_for_Qt6::QuickControls2Imagine "${_IMPORT_PREFIX}/lib/libQt6QuickControls2Imagine.a" "${_IMPORT_PREFIX}/bin/Qt6QuickControls2Imagine.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
