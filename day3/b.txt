#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;
using namespace std;

int main() {
    fs::path cwd = fs::current_path();
    string cmd;

    cout << "File Operations Console\n";
    cout << "Commands: touch <file>, rm <file>, cp <src> <dst>, mv <src> <dst>, exit\n\n";

    while (true) {
        cout << cwd << " $ ";
        getline(cin, cmd);

        if (cmd.rfind("touch",0)==0) {
            string a = cmd.size() > 6 ? cmd.substr(6) : "";
            if (a.empty()) cout << "touch: missing file\n";
            else ofstream(cwd/a).close();
        }
        else if (cmd.rfind("rm",0)==0) {
            string a = cmd.size() > 3 ? cmd.substr(3) : "";
            if (fs::exists(cwd/a)) fs::remove_all(cwd/a);
            else cout << "rm: file not found\n";
        }
        else if (cmd.rfind("cp",0)==0) {
            istringstream iss(cmd); string s, a, b; iss >> s >> a >> b;
            if (fs::exists(cwd/a))
                fs::copy(cwd/a, cwd/b, fs::copy_options::overwrite_existing);
            else cout << "cp: source not found\n";
        }
        else if (cmd.rfind("mv",0)==0) {
            istringstream iss(cmd); string s, a, b; iss >> s >> a >> b;
            if (fs::exists(cwd/a))
                fs::rename(cwd/a, cwd/b);
            else cout << "mv: source not found\n";
        }
        else if (cmd=="exit") break;
        else if (!cmd.empty()) cout << "Unknown command: " << cmd << "\n";
    }
}