#include "protocol.h"
#include <unistd.h>
#include <sstream>

std::string readLine(int fd) {
    static std::string buffer;
    while (true) {
        size_t pos = buffer.find('\n');
        if (pos != std::string::npos) {
            std::string line = buffer.substr(0, pos);
            buffer = buffer.substr(pos + 1);
            return line;
        }
        char buf[256];
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n <= 0) {
            std::string result = buffer;
            buffer.clear();
            return result;
        }
        buffer.append(buf, n);
    }
}

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (ss >> token)
        tokens.push_back(token);
    return tokens;
}
