// src/console_input_manager.hpp
#ifndef CONSOLE_INPUT_MANAGER_HPP
#define CONSOLE_INPUT_MANAGER_HPP

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

/**
 * @brief Manages reading lines from the console in a separate thread.
 * Provides a non-blocking way to get newly entered lines.
 */
class ConsoleInputManager {
public:
    ConsoleInputManager();
    ~ConsoleInputManager();

    // Starts the input reading thread
    void start();
    // Stops the input reading thread
    void stop();

    // Checks if there's a new line available and retrieves it.
    // Returns true if a new line was retrieved, false otherwise.
    bool getNewLine(std::string& line);

private:
    void inputThreadLoop(); // The function executed by the thread

    std::atomic<bool> m_running;          // Flag to control thread execution
    std::thread m_inputThread;            // The thread object
    std::mutex m_queueMutex;             // Mutex to protect the message queue
    std::queue<std::string> m_inputQueue; // Queue for new lines from console
    std::condition_variable m_cv;         // For signaling new input (not strictly needed for non-blocking read, but good practice)
};

#endif // CONSOLE_INPUT_MANAGER_HPP