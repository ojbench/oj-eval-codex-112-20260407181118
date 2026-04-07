// Implementation for Problem 112 - shell 模拟
// Only the allowed headers may be used as per the template
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;

namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;

        static bool is_space(char c) {
            return c == ' ';
        }
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) : _argc(0), _argv(nullptr) {
            // Parse the command by spaces (single space separator).
            // Input is guaranteed to not end with a space.
            // Consecutive spaces should be treated as separators and skipped.
            vector<string> tokens;
            tokens.reserve(8);
            size_t i = 0, n = cmd.size();
            while (i < n) {
                // skip leading spaces between tokens
                while (i < n && is_space(cmd[i])) ++i;
                if (i >= n) break;
                size_t j = i;
                while (j < n && !is_space(cmd[j])) ++j;
                tokens.emplace_back(cmd.substr(i, j - i));
                i = j;
            }

            _argc = static_cast<int>(tokens.size());
            if (_argc == 0) {
                _argv = nullptr;
                return;
            }

            // Allocate argv array (size _argc)
            _argv = new char*[_argc + 1];
            for (int k = 0; k < _argc; ++k) {
                const string& s = tokens[k];
                // Allocate C-string with null terminator
                char* buf = new char[s.size() + 1];
                std::memcpy(buf, s.c_str(), s.size() + 1);
                _argv[k] = buf;
            }
            _argv[_argc] = nullptr;
        }
        ~arguments() {
            // Free allocated memory to avoid leaks
            if (_argv) {
                for (int i = 0; i < _argc; ++i) {
                    delete[] _argv[i];
                }
                delete[] _argv;
                _argv = nullptr;
            }
            _argc = 0;
        }
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}
