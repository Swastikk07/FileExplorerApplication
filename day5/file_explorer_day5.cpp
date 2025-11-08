#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
using namespace std;
namespace fs = std::filesystem;

void listFiles(const fs::path& p) {
    for (auto &f : fs::directory_iterator(p))
        cout << (fs::is_directory(f) ? "[DIR]  " : "       ") << f.path().filename().string() << "\n";
}

void showInfo(const fs::path& file) {
    if (!fs::exists(file)) { cout << "info: file not found\n"; return; }
    auto ftime = fs::last_write_time(file);
    auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + chrono::system_clock::now());
    time_t cftime = chrono::system_clock::to_time_t(sctp);
    cout << "File: " << file.filename() << "\nPath: " << fs::absolute(file)
         << "\nSize: " << fs::file_size(file) << " bytes\nLast Modified: "
         << put_time(localtime(&cftime), "%Y-%m-%d %H:%M:%S") << "\n";
    fs::perms p = fs::status(file).permissions();
    cout << "Permissions: "
         << ((p & fs::perms::owner_read)  != fs::perms::none ? "r" : "-")
         << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
         << ((p & fs::perms::owner_exec)  != fs::perms::none ? "x" : "-")
         << ((p & fs::perms::group_read)  != fs::perms::none ? "r" : "-")
         << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
         << ((p & fs::perms::group_exec)  != fs::perms::none ? "x" : "-")
         << ((p & fs::perms::others_read)  != fs::perms::none ? "r" : "-")
         << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
         << ((p & fs::perms::others_exec)  != fs::perms::none ? "x" : "-") << "\n";
}

void changePerms(const fs::path& file, const string& mode) {
    if (!fs::exists(file)) { cout << "chmod: file not found\n"; return; }
    fs::perms p = fs::perms::none;
    for (char c : mode) {
        if (c == 'r') p |= fs::perms::owner_read | fs::perms::group_read | fs::perms::others_read;
        if (c == 'w') p |= fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write;
        if (c == 'x') p |= fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec;
    }
    fs::permissions(file, p);
    cout << "Permissions updated for " << file << "\n";
}

int main() {
    fs::path cwd = fs::current_path();
    string line;

    cout << "File Explorer (Day 5)\nCommands: ls, cd <dir>, mkdir <dir>, touch <file>, info <file>, chmod <file> <rwx>, exit\n\n";

    while (true) {
        cout << cwd << " $ ";
        if (!getline(cin, line)) break;

        if (line == "ls") listFiles(cwd);
        else if (line.rfind("cd",0)==0) {
            string dir = line.size()>3 ? line.substr(3) : "";
            fs::path newPath = (dir=="..") ? cwd.parent_path() : cwd/dir;
            cwd = (fs::exists(newPath)&&fs::is_directory(newPath)) ? fs::canonical(newPath) : cwd;
        }
        else if (line.rfind("mkdir",0)==0) fs::create_directory(cwd/line.substr(6));
        else if (line.rfind("touch",0)==0) ofstream(cwd/line.substr(6)).close();
        else if (line.rfind("info",0)==0) showInfo(cwd/line.substr(5));
        else if (line.rfind("chmod",0)==0) {
            istringstream iss(line); string s, f, m; iss>>s>>f>>m; changePerms(cwd/f,m);
        }
        else if (line=="exit") break;
    }
}
