# CMakeLists.txt

# Minimum required CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Project name and C++ standard
project(InteractiveCanvas LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source files for our executable
set(SOURCE_FILES
    src/main.cpp
    src/console_input_manager.cpp
)

# --- Find SDL2 (Core components) ---
# This part should still work, as SDL2 core is usually found.
find_package(SDL2 REQUIRED)

# --- Manual Configuration for SDL2_ttf ---
# Define variables to hold the paths you found
set(SDL2TTF_INCLUDE_DIR "/usr/include/SDL2") # <--- REPLACE WITH YOUR ACTUAL PATH
set(SDL2TTF_LIB_DIR "/usr/lib/x86_64-linux-gnu") # <--- REPLACE WITH YOUR ACTUAL PATH

# Find the SDL2_ttf library by name in the specified directory
find_library(SDL2TTF_LIBRARY
    NAMES SDL2_ttf # Common names for the library file
    PATHS ${SDL2TTF_LIB_DIR}
    REQUIRED
)

# Check if SDL2_ttf_LIBRARY was found
if(NOT SDL2TTF_LIBRARY)
    message(FATAL_ERROR "Could not find libSDL2_ttf.so in ${SDL2TTF_LIB_DIR}")
endif()

# --- Find Threads (for std::thread) ---
find_package(Threads REQUIRED)


# --- Add Executable ---
add_executable(interactive_canvas ${SOURCE_FILES})

# --- Link Libraries and Set Include Directories ---

# Link SDL2, SDL2_ttf (manually found), and Threads.
# SDL2::SDL2main and SDL2::SDL2 come from find_package(SDL2)
# SDL2TTF_LIBRARY is our manually found library
target_link_libraries(interactive_canvas PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    ${SDL2TTF_LIBRARY} # <-- Use our manually found library variable
    Threads::Threads
)

# Add our own src directory to include paths
target_include_directories(interactive_canvas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add the manually found SDL2_ttf include directory
target_include_directories(interactive_canvas PRIVATE ${SDL2TTF_INCLUDE_DIR}) # <-- Add this line!

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
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/arial.ttf")
    add_custom_command(TARGET interactive_canvas POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/arial.ttf" # <-- Source path
            "$<TARGET_FILE_DIR:interactive_canvas>/arial.ttf" # <-- Destination path
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