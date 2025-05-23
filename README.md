# Interactive Canvas Dynamic Text Renderer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A C++ application that creates an interactive canvas for dynamically rendering text with modern typing and animation effects. Utilizing the power of SDL2 for graphics and SDL2_ttf for high-quality text rendering, this project demonstrates how to build a simple yet visually appealing graphical application that responds to user input.

## ✨ Features

- **Dynamic Text Rendering**: Render text on a graphical canvas in real-time.
- **Typing Animation**: Text appears character by character with a smooth typing effect.
- **Fade-in Effect**: Each character fades in as it appears, creating a polished look.
- **Blinking Cursor**: A realistic blinking cursor indicates the typing position.
- **Console Input**: Enter the text to be displayed directly through the console before the canvas appears.
- **Automatic Centering**: Text is automatically centered on the canvas.
- **Cross-Platform**: Designed to be compatible with Windows, Linux, and macOS.

## 🛠️ Prerequisites

Before building and running the application, ensure you have the following installed:

*   **CMake**: Version 3.15 or higher. Used for managing the build process.
    *   Download: [https://cmake.org/download/](https://cmake.org/download/)
*   **C++17 Compiler**: A compiler that supports the C++17 standard (e.g., GCC, Clang, MSVC).
*   **SDL2 Development Libraries**: The core SDL2 library.
    *   Installation instructions vary by OS:
        *   **Debian/Ubuntu**: `sudo apt-get install libsdl2-dev`
        *   **Fedora**: `sudo dnf install SDL2-devel`
        *   **macOS (using Homebrew)**: `brew install sdl2`
        *   **Windows**: Download from the [SDL website](https://www.libsdl.org/download-2.0.php) and follow their installation guide.
*   **SDL2_ttf Development Libraries**: The SDL2 TrueType Font rendering library.
    *   Installation instructions vary by OS:
        *   **Debian/Ubuntu**: `sudo apt-get install libsdl2-ttf-dev`
        *   **Fedora**: `sudo dnf install SDL2_ttf-devel`
        *   **macOS (using Homebrew)**: `brew install sdl2_ttf`
        *   **Windows**: Download from the [SDL_ttf website](https://www.libsdl.org/projects/SDL_ttf/release-2.0.html) and follow their installation guide.
*   **Arial Font File**: A `arial.ttf` font file is required and is included in this repository.

## 🏗️ Building the Project

Follow these steps to build the application:

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/Interactive-Canvas-Dynamic-Text-Renderer.git
    cd Interactive-Canvas-Dynamic-Text-Renderer
    ```
    *(Replace `https://github.com/your-username/Interactive-Canvas-Dynamic-Text-Renderer.git` with the actual repository URL)*

2.  **Create a build directory and navigate into it:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure and build the project using CMake:**
    ```bash
    cmake ..
    make
    ```
    This will compile the source code and create the executable.

## 🚀 Running the Application

1.  **Navigate to the directory containing the executable:**
    ```bash
    cd build/bin
    ```

2.  **Run the executable:**
    ```bash
    ./interactive_canvas
    ```

3.  **Enter your desired text** in the console when prompted and press Enter. The interactive canvas window will then appear, displaying your text with the typing animation.

## 🎮 Controls

*   Type text in the console and press `Enter` to input the name before the canvas appears.
*   Close the application window to exit.

## 📁 Project Structure

```
Interactive-Canvas-Dynamic-Text-Renderer/
├── CMakeLists.txt          # CMake build configuration file
├── LICENSE                 # Project license file (MIT License)
├── README.md               # Project documentation (this file)
├── src/
│   ├── main.cpp           # Main application source code
│   ├── console_input_manager.cpp  # Source for console input handling
│   └── console_input_manager.hpp  # Header for console input manager
└── arial.ttf              # TrueType font file used for rendering
```

## 👋 Contributing

Contributions are welcome! If you'd like to contribute, please follow these steps:

1.  Fork the repository.
2.  Create a new branch for your feature or bug fix.
3.  Make your changes and commit them with clear messages.
4.  Push your changes to your fork.
5.  Create a pull request to the `main` branch of this repository.

Please ensure your code adheres to the project's coding style and that all tests pass.

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details. 