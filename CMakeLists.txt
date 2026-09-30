cmake_minimum_required(VERSION 3.28)

project(PixelEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

# ---- Global settings -------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # use standard C++20, not MSVC/GCC dialects

# Put all binaries in one folder so DLLs sit next to the .exe that needs them.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Make Visual Studio's Solution Explorer show a folder tree instead of a flat list.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ---- Warnings --------------------------------------------------------------
# An INTERFACE library is a way to bundle settings and apply them to many targets.
add_library(pe_warnings INTERFACE)
if(MSVC)
    target_compile_options(pe_warnings INTERFACE /W4 /permissive-)
else()
    target_compile_options(pe_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()

# ---- Dependencies ----------------------------------------------------------
find_package(SDL3 CONFIG REQUIRED)

# ---- Subdirectories --------------------------------------------------------
add_subdirectory(engine/pe_core)
add_subdirectory(sandbox)

# Make `sandbox` the target that runs when you press F5 in Visual Studio.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
