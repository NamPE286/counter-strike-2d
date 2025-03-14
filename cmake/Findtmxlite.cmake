#Findtmxlite.cmake
set(VCPKG_PATH "${USER_PROFILE}/vcpkg")
set(VCPKG_INCLUDE_DIR "${VCPKG_PATH}/installed/${VCPKG_TARGET_TRIPLET}/include")
set(VCPKG_LIBRARY_DIR "${VCPKG_PATH}/installed/${VCPKG_TARGET_TRIPLET}/lib")

# Set up tmxlite search

# Find path to tmxlite header files
find_path(tmxlite_INCLUDE_DIR NAMES tmxlite/Map.hpp PATHS VCPKG_INCLUDE_DIR)

# Find the tmxlite library
find_library(tmxlite_LIBRARY NAMES tmxlite PATHS VCPKG_LIBRARY_DIR)

# Create an imported target for tmxlite
add_library(tmxlite INTERFACE)

# Specify the path to the tmxlite header files
target_include_directories(tmxlite INTERFACE ${tmxlite_INCLUDE_DIR})

# Link the tmxlite library to the target
target_link_libraries(tmxlite INTERFACE ${tmxlite_LIBRARY})
