#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main() {
    std::cout << "=== File Explorer Application (Day 1) ===\n";

    // Current working directory
    fs::path current_path = fs::current_path();
    std::cout << "Current Directory: " << current_path << "\n\n";

    // List all files and directories
    std::cout << "Contents:\n";
    for (const auto &entry : fs::directory_iterator(current_path)) {
        if (entry.is_directory())
            std::cout << "[DIR]  " << entry.path().filename().string() << "\n";
        else
            std::cout << "[FILE] " << entry.path().filename().string() << "\n";
    }

    return 0;
}
