#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Function to list files in current directory
void listFiles(const fs::path& path) {
    for (auto &entry : fs::directory_iterator(path)) {
        std::cout << (fs::is_directory(entry) ? "[DIR]  " : "       ")
                  << entry.path().filename().string() << "\n";
    }
}

// Function to search for a file recursively
void searchFile(const fs::path& root, const std::string& filename) {
    bool found = false;
    for (auto &entry : fs::recursive_directory_iterator(root)) {
        if (entry.path().filename() == filename) {
            std::cout << "Found: " << entry.path() << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "No file named '" << filename << "' found.\n";
    }
}

int main() {
    fs::path cwd = fs::current_path();
    std::string line;

    std::cout << "File Explorer (Day 4) – File Search\n";
    std::cout << "Commands: ls, cd <dir>, mkdir <dir>, search <filename>, exit\n\n";

    while (true) {
        std::cout << cwd << " $ ";
        if (!std::getline(std::cin, line)) break;

        if (line == "ls") {
            listFiles(cwd);
        }
        else if (line.rfind("cd", 0) == 0) {
            std::string dir = line.size() > 3 ? line.substr(3) : "";
            fs::path newPath = (dir == "..") ? cwd.parent_path() : cwd / dir;
            if (fs::exists(newPath) && fs::is_directory(newPath)) {
                cwd = fs::canonical(newPath);
            } else {
                std::cout << "cd: " << dir << ": No such directory\n";
            }
        }
        else if (line.rfind("mkdir", 0) == 0) {
            std::string dir = line.size() > 6 ? line.substr(6) : "";
            if (dir.empty()) {
                std::cout << "mkdir: missing directory name\n";
            } else if (fs::exists(cwd / dir)) {
                std::cout << "mkdir: directory already exists\n";
            } else {
                fs::create_directory(cwd / dir);
            }
        }
        else if (line.rfind("search", 0) == 0) {
            std::string filename = line.size() > 7 ? line.substr(7) : "";
            if (filename.empty()) {
                std::cout << "search: missing file name\n";
            } else {
                searchFile(cwd, filename);
            }
        }
        else if (line == "exit") {
            break;
        }
        else if (!line.empty()) {
            std::cout << "Unknown command: " << line << "\n";
        }
    }

    std::cout << "Exiting File Explorer.\n";
    return 0;
}
