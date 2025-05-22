# CMakeLists.txt

# Minimum required CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR) # SDL2 might need 3.15+ for modern find_package

# Project name and C++ standard
project(InteractiveCanvas LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source files for our executable
set(SOURCE_FILES
    src/main.cpp
    src/console_input_manager.cpp # <-- Add our new source file here!
)

# --- Find SDL2 and SDL2_ttf ---

# Set a hint for CMake if SDL2/SDL2_ttf are not in standard locations
# For Windows, you might set this to the parent directory where you extracted SDL2-devel/SDL2_ttf-devel
# e.g., set(SDL2_DIR "C:/Libraries/SDL2-devel-2.28.5-VC")
#      set(SDL2TTF_DIR "C:/Libraries/SDL2_ttf-devel-2.20.2-VC")
# You can also pass these as command-line arguments to cmake:
# cmake .. -DSDL2_DIR="C:/path/to/SDL2" -DSDL2TTF_DIR="C:/path/to/SDL2_ttf"

# Find SDL2 (Core components)
find_package(SDL2 REQUIRED)

# Find SDL2_ttf
find_package(SDL2_ttf REQUIRED)

# --- Find Threads (for std::thread) ---
# Modern CMake often finds this automatically, but explicit is good practice.
# Requires C++11 or later.
find_package(Threads REQUIRED)


# --- Add Executable ---
add_executable(interactive_canvas ${SOURCE_FILES})

# --- Link Libraries and Set Include Directories ---

# Link SDL2, SDL2_ttf, and Threads.
# SDL2 and SDL2_ttf's CMake config files usually provide INTERFACE_INCLUDE_DIRECTORIES
# and LINK_LIBRARIES, which are automatically propagated when using the imported targets.
target_link_libraries(interactive_canvas PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_ttf::SDL2_ttf Threads::Threads)

# Add our own src directory to include paths so main.cpp can find console_input_manager.hpp
target_include_directories(interactive_canvas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# --- Handle SDL2 specific properties for certain platforms (important for Windows) ---
if(WIN32)
    target_compile_definitions(interactive_canvas PRIVATE SDL_MAIN_HANDLED)
endif()

# Set output directory for executables
set_target_properties(interactive_canvas PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Optional: Add a custom command to copy the font file to the build directory
# This ensures the executable can find 'arial.ttf' if it's placed next to your CMakeLists.txt
# or in a known resource folder. Adjust the source path as needed.
# If your font is in 'interactive_canvas/res/arial.ttf'
# configure_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/res/arial.ttf"
#    "${CMAKE_BINARY_DIR}/bin/arial.ttf"
#    COPYONLY
#)
# Or if it's in the main source directory:
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/arial.ttf")
    add_custom_command(TARGET interactive_canvas POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/arial.ttf"
            "$<TARGET_FILE_DIR:interactive_canvas>/arial.ttf"
        COMMENT "Copying arial.ttf to build directory"
    )
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/arial.ttf")
    add_custom_command(TARGET interactive_canvas POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/src/arial.ttf"
            "$<TARGET_FILE_DIR:interactive_canvas>/arial.ttf"
        COMMENT "Copying arial.ttf to build directory"
    )
else()
    message(WARNING "arial.ttf not found in source or src directory. Please ensure it's accessible to the executable.")
endif()

message(STATUS "Executable 'interactive_canvas' will be built in ${CMAKE_BINARY_DIR}/bin.") 