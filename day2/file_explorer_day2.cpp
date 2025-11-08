#include <iostream> 
#include <filesystem> 
#include <string> 
#include <sstream> 

namespace fs = std::filesystem; 

void listFiles(const fs::path& path) { 
    std::cout << "\nContents of: " << path << "\n"; 
    try { 
        for (const auto& entry : fs::directory_iterator(path)) { 
            std::cout << (fs::is_directory(entry) ? "[DIR]  " : "       "); 
            std::cout << entry.path().filename().string() << "\n"; 
        } 
    } catch (const fs::filesystem_error& e) { 
        std::cerr << "Error: " << e.what() << "\n"; 
    } 
} 

int main() { 
    fs::path cwd = fs::current_path(); 
    std::string line; 

    std::cout << "Interactive File Explorer (Day 2)\n"; 
    std::cout << "Type commands: ls, cd <dir>, pwd, exit\n\n"; 

    while (true) { 
        std::cout << cwd << " $ "; 
        if (!std::getline(std::cin, line)) break; 

        std::istringstream iss(line); 
        std::string cmd; 
        iss >> cmd; 

        if (cmd == "ls") { 
            listFiles(cwd); 
        } else if (cmd == "pwd") { 
            std::cout << cwd << "\n"; 
        } else if (cmd == "cd") { 
            std::string dir; 
            if (!(iss >> dir)) { 
                std::cout << "cd: missing operand\n"; 
                continue;    
            } 
            fs::path newPath = (dir == "..") ? cwd.parent_path() : cwd / dir; 
            if (fs::exists(newPath) && fs::is_directory(newPath)) { 
                cwd = fs::canonical(newPath); 
            } else { 
                std::cout << "cd: " << dir << ": No such directory\n"; 
            } 
        } else if (cmd == "exit") { 
            break; 
        } else if (cmd.empty()) { 
            continue; 
        } else { 
            std::cout << "Unknown command: " << cmd << "\n"; 
        } 
    } 

    std::cout << "Exiting File Explorer.\n"; 
    return 0; 
}
