# Interactive Canvas Dynamic Text Renderer

A C++ application that creates an interactive canvas where users can input text that is rendered with dynamic colors and scaling effects. The application uses SDL2 and SDL2_ttf for graphics and text rendering.

## Features

- Real-time text rendering with dynamic color effects
- Automatic text scaling to fit the window
- Interactive console input for text updates
- Smooth animations and transitions
- Cross-platform support (Windows, Linux, macOS)

## Prerequisites

- CMake (version 3.15 or higher)
- C++17 compatible compiler
- SDL2 development libraries
- SDL2_ttf development libraries
- Arial font file (included in the project)

## Building the Project

1. Clone the repository:
```bash
git clone [repository-url]
cd Interactive-Canvas-Dynamic-Text-Renderer
```

2. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```

3. Configure and build the project:
```bash
cmake ..
cmake --build .
```

The executable will be created in the `build/bin` directory.

## Running the Application

1. Navigate to the build directory:
```bash
cd build/bin
```

2. Run the executable:
```bash
./interactive_canvas
```

3. Type your text in the console and press Enter to see it rendered on the canvas.

## Controls

- Type text in the console and press Enter to update the display
- Close the window to exit the application

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── src/
│   ├── main.cpp           # Main application code
│   ├── console_input_manager.cpp  # Console input handling
│   └── console_input_manager.hpp  # Console input manager header
└── arial.ttf              # Font file
```

## Dependencies

- SDL2: Graphics and window management
- SDL2_ttf: Text rendering
- C++17 Standard Library

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. 