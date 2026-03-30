#pragma once

#include <string>
#include <vector>

// read until newline, handles partial reads (TCP can split data across packets)
std::string readLine(int fd);

// split by whitespace, same as command::split but for protocol layer
std::vector<std::string> tokenize(const std::string& line);
