// Example program:
// Using SDL3 to create an application window

#include <SDL3/SDL.h>

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL3 window",                  // window title
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;// src/main.cpp
        #include <SDL3/SDL.h>
        #include "managers/TrafficManager.h"
        #include "managers/FileHandler.h"
        #include "visualization/Renderer.h"
        #include <iostream>
        #include <chrono>
        
        class Simulator {
        private:
            TrafficManager trafficManager;
            Renderer renderer;
            bool running;
        
            void processInput() {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_EVENT_QUIT:
                            running = false;
                            break;
                        case SDL_EVENT_KEY_DOWN:
                            // For SDL3, we use scancode instead of keysym
                            if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                                running = false;
                            }
                            break;
                    }
                }
            }
        
            void update(float deltaTime) {
                trafficManager.update(deltaTime);
            }
        
            void render() {
                renderer.render(trafficManager);
            }
        
        public:
            Simulator() : running(false) {}
        
            bool initialize() {
                if (!renderer.initialize()) {
                    std::cerr << "Failed to initialize renderer" << std::endl;
                    return false;
                }
        
                // Clear any existing vehicle data
                FileHandler fileHandler;
                fileHandler.clearLaneFiles();
        
                running = true;
                return true;
            }
        
            void run() {
                auto lastUpdateTime = std::chrono::high_resolution_clock::now();
        
                while (running) {
                    auto currentTime = std::chrono::high_resolution_clock::now();
                    float deltaTime = std::chrono::duration<float>(currentTime - lastUpdateTime).count();
                    lastUpdateTime = currentTime;
        
                    processInput();
                    update(deltaTime);
                    render();
        
                    // Cap frame rate at ~60 FPS
                    if (deltaTime < 0.016f) {
                        SDL_Delay(static_cast<uint32_t>((0.016f - deltaTime) * 1000));
                    }
                }
            }
        
            void cleanup() {
                renderer.cleanup();
            }
        };
        
        int main(int argc, char* argv[]) {
            (void)argc; // Suppress unused parameter warning
            (void)argv; // Suppress unused parameter warning
        
            Simulator simulator;
        
            if (!simulator.initialize()) {
                std::cerr << "Failed to initialize simulator" << std::endl;
                return 1;
            }
        
            std::cout << "Traffic Simulator Started\n";
            std::cout << "Press ESC to exit\n";
        
            simulator.run();
            simulator.cleanup();
        
            return 0;
        }
    }

    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }

        // Do game logic, present a frame, etc.
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}