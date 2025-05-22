// src/console_input_manager.cpp
#include "console_input_manager.hpp"
#include <iostream>

ConsoleInputManager::ConsoleInputManager() : m_running(false) {
    // Constructor initializes members, but doesn't start the thread yet.
}

ConsoleInputManager::~ConsoleInputManager() {
    stop(); // Ensure thread is stopped on destruction
}

void ConsoleInputManager::start() {
    if (!m_running) {
        m_running = true;
        // Start the thread, passing the member function and 'this' pointer
        m_inputThread = std::thread(&ConsoleInputManager::inputThreadLoop, this);
        std::cout << "Console input manager started. Type your name and press Enter:\n";
    }
}

void ConsoleInputManager::stop() {
    if (m_running) {
        m_running = false; // Signal the thread to stop
        // Notify the condition variable in case the thread is waiting,
        // though for std::getline it usually won't be blocked indefinitely after signal.
        m_cv.notify_all();
        if (m_inputThread.joinable()) {
            m_inputThread.join(); // Wait for the thread to finish
        }
        std::cout << "Console input manager stopped.\n";
    }
}

void ConsoleInputManager::inputThreadLoop() {
    // This loop runs in the separate thread
    while (m_running) {
        std::string line;
        // std::getline blocks until enter is pressed.
        // This is why we run it in a separate thread.
        if (std::getline(std::cin, line)) {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_inputQueue.push(line);
            m_cv.notify_one(); // Notify the main thread that new input is available
        } else {
            // Handle EOF or error on stdin, might indicate program termination
            m_running = false;
        }
    }
}

bool ConsoleInputManager::getNewLine(std::string& line) {
    std::lock_guard<std::mutex> lock(m_queueMutex); // Protect the queue
    if (!m_inputQueue.empty()) {
        line = m_inputQueue.front();
        m_inputQueue.pop();
        return true;
    }
    return false;
}