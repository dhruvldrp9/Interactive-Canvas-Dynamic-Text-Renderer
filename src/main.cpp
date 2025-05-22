// src/main.cpp
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For std::max
#include <thread>    // For std::this_thread::sleep_for
#include <chrono>    // For std::chrono::milliseconds

#include "console_input_manager.hpp" // Include our new manager

// Window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FONT_SIZE = 48; // Base font size

// Global variable for the text to display, will be updated by console input
std::string g_displayText = "Type your name!";

// Function to render text with changing color and dynamic sizing
void renderDynamicText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, double time) {
    if (text.empty()) {
        return; // Nothing to render if text is empty
    }

    // Calculate color components based on time for a dynamic effect
    Uint8 r = static_cast<Uint8>(128 + 127 * std::sin(time * 0.5));
    Uint8 g = static_cast<Uint8>(128 + 127 * std::sin(time * 0.7));
    Uint8 b = static_cast<Uint8>(128 + 127 * std::sin(time * 0.9));

    SDL_Color textColor = {r, g, b, 255}; // R, G, B, Alpha

    // Create a surface from the text
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    // Get texture dimensions
    int text_w, text_h;
    SDL_QueryTexture(textTexture, NULL, NULL, &text_w, &text_h);

    // Calculate scaling factor to fit text within the screen width, with some padding
    double scale_factor_w = (SCREEN_WIDTH * 0.9) / text_w; // 90% of screen width
    double scale_factor_h = (SCREEN_HEIGHT * 0.9) / text_h; // 90% of screen height
    double scale_factor = std::min(scale_factor_w, scale_factor_h); // Use the smaller factor to ensure it fits both ways

    // Apply scaling, ensuring it doesn't shrink too much if the text is short
    // And also ensure it doesn't grow beyond the original font size if not needed
    scale_factor = std::min(scale_factor, 1.0); // Don't scale up beyond original size for short text
    scale_factor = std::max(scale_factor, 0.5); // Don't scale down too much either

    text_w = static_cast<int>(text_w * scale_factor);
    text_h = static_cast<int>(text_h * scale_factor);


    // Calculate position to center the text
    SDL_Rect renderQuad = { x - text_w / 2, y - text_h / 2, text_w, text_h };

    // Render texture to screen
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

int main(int argc, char* args[]) {
    // Create an instance of our console input manager
    ConsoleInputManager inputManager;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Interactive Canvas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Set renderer draw color to black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    // Load font
    TTF_Font* font = TTF_OpenFont("/home/gabbar/Projects/Interactive Canvas Dynamic Text Renderer/build/bin/arial.ttf", FONT_SIZE); // Using a fixed FONT_SIZE
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Start the console input manager thread
    inputManager.start();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Timer for dynamic effects
    Uint32 startTime = SDL_GetTicks();

    // Game loop
    while (!quit) {
        // --- Process Console Input ---
        std::string newLine;
        if (inputManager.getNewLine(newLine)) {
            g_displayText = newLine; // Update global display text
            if (g_displayText.empty()) {
                g_displayText = "Type your name!"; // Default if empty string entered
            }
            std::cout << "Displaying: '" << g_displayText << "'\n";
        }

        // --- Process SDL Events ---
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // --- Rendering ---
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black background
        SDL_RenderClear(renderer);

        double time = (SDL_GetTicks() - startTime) / 1000.0; // Time in seconds

        renderDynamicText(renderer, font, g_displayText, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, time);

        SDL_RenderPresent(renderer);

        // Add a small delay to prevent busy-waiting if no events or input,
        // and to keep CPU usage down. Roughly 60 FPS target if vsync isn't perfect.
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // Cleanup resources
    inputManager.stop(); // Stop the input thread gracefully
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}