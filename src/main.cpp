// src/main.cpp
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>
#include <filesystem>

#include "console_input_manager.hpp"

// Window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FONT_SIZE = 48;

// Animation parameters
const float TYPING_SPEED = 0.08f;        // Time between each character appearing
const float CURSOR_BLINK_SPEED = 0.5f;   // Cursor blink speed
const float FADE_DURATION = 0.3f;        // Duration of fade-in effect
const float CURSOR_WIDTH = 3.0f;         // Width of the cursor
const float CURSOR_HEIGHT = 0.8f;        // Height of cursor relative to text

// Function to find font file
std::string findFontFile() {
    std::vector<std::string> possiblePaths = {
        "arial.ttf",
        "../arial.ttf",
        "build/bin/arial.ttf",
        "../build/bin/arial.ttf"
    };

    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }
    return "";
}

// Function to render text with professional typing animation
void renderDynamicText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, double time, int visibleChars) {
    if (text.empty() || !font) {
        return;
    }

    std::vector<SDL_Surface*> charSurfaces;
    std::vector<SDL_Texture*> charTextures;
    std::vector<SDL_Rect> charRects;
    std::vector<float> charAlphas;
    
    int textWidth = 0;
    int textHeight = 0;
    std::vector<int> charWidths;
    std::vector<int> charHeights;

    // Calculate total width and height of the text and store individual character dimensions
    for (size_t i = 0; i < text.length(); ++i) {
        if (i >= visibleChars) break;
        
        int minx, maxx, miny, maxy, advance;
        if (TTF_GlyphMetrics(font, text[i], &minx, &maxx, &miny, &maxy, &advance) != 0) {
            std::cerr << "Failed to get glyph metrics for character: " << text[i] << std::endl;
            continue;
        }
        charWidths.push_back(advance);
        charHeights.push_back(maxy - miny);
        textWidth += advance;
        textHeight = std::max(textHeight, maxy - miny);
    }

    // Create surfaces and textures for each character and calculate their positions
    int currentX = x - textWidth / 2; // Start X for centering
    for (size_t i = 0; i < text.length(); ++i) {
        if (i >= visibleChars) break;

        SDL_Color textColor = {255, 255, 255, 255}; // White color
        
        // Create surface for single character
        std::string singleChar = text.substr(i, 1);
        SDL_Surface* charSurface = TTF_RenderUTF8_Blended(font, singleChar.c_str(), textColor);
        if (!charSurface) {
            std::cerr << "Failed to render character surface: " << singleChar << std::endl;
            continue;
        }

        SDL_Texture* charTexture = SDL_CreateTextureFromSurface(renderer, charSurface);
        if (!charTexture) {
            std::cerr << "Failed to create texture for character: " << singleChar << std::endl;
            SDL_FreeSurface(charSurface);
            continue;
        }

        charSurfaces.push_back(charSurface);
        charTextures.push_back(charTexture);
        
        // Calculate fade-in alpha for this character
        float charTime = time - (i * TYPING_SPEED);
        float alpha = std::min(1.0f, charTime / FADE_DURATION);
        charAlphas.push_back(alpha);

        SDL_Rect charRect = {
            static_cast<int>(currentX),
            static_cast<int>(y - textHeight / 2), // Center vertically based on total text height
            charSurface->w,
            charSurface->h
        };

        charRects.push_back(charRect);
        currentX += charWidths[i]; // Move X for the next character based on glyph advance
    }

    // Render each character with fade-in effect
    for (size_t i = 0; i < charTextures.size(); ++i) {
        SDL_SetTextureAlphaMod(charTextures[i], static_cast<Uint8>(charAlphas[i] * 255));
        SDL_RenderCopy(renderer, charTextures[i], NULL, &charRects[i]);
    }

    // Render cursor if typing is not complete and there are visible characters
    if (visibleChars < text.length() && !charRects.empty()) {
        float cursorAlpha = (sin(time * M_PI * 2 / CURSOR_BLINK_SPEED) + 1.0f) * 0.5f;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, static_cast<Uint8>(cursorAlpha * 255));
        
        // Position cursor after the last visible character
        int cursorX = charRects.back().x + charRects.back().w; // Position after the last char
        
        SDL_Rect cursorRect = {
            cursorX,
            static_cast<int>(y - textHeight * CURSOR_HEIGHT / 2),
            static_cast<int>(CURSOR_WIDTH),
            static_cast<int>(textHeight * CURSOR_HEIGHT)
        };
        SDL_RenderFillRect(renderer, &cursorRect);
    }

    // Clean up
    for (size_t i = 0; i < charTextures.size(); ++i) {
        if (charTextures[i]) SDL_DestroyTexture(charTextures[i]);
    }
    for (size_t i = 0; i < charSurfaces.size(); ++i) {
        if (charSurfaces[i]) SDL_FreeSurface(charSurfaces[i]);
    }
}

int main(int argc, char* args[]) {
    ConsoleInputManager inputManager;
    std::string displayText;

    // Get initial input before showing window
    std::cout << "Enter your name: ";
    std::getline(std::cin, displayText);
    
    if (displayText.empty()) {
        displayText = "Anonymous";
    }

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

    // Find and load font
    std::string fontPath = findFontFile();
    if (fontPath.empty()) {
        std::cerr << "Could not find arial.ttf in any of the expected locations!" << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), FONT_SIZE);
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

    bool quit = false;
    SDL_Event e;
    Uint32 startTime = SDL_GetTicks();
    int visibleChars = 0;
    double lastCharTime = 0.0;

    // Game loop
    while (!quit) {
        // Process SDL Events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update typing animation
        double currentTime = (SDL_GetTicks() - startTime) / 1000.0;
        if (currentTime - lastCharTime >= TYPING_SPEED && visibleChars < displayText.length()) {
            visibleChars++;
            lastCharTime = currentTime;
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black background
        SDL_RenderClear(renderer);

        renderDynamicText(renderer, font, displayText, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, currentTime, visibleChars);

        SDL_RenderPresent(renderer);

        // Cap at 60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // Cleanup
    inputManager.stop();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}