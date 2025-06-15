#pragma once

#include <string>

std::string readFile(const std::string &path);

// escape all escape sequences in string
std::string escapeSequences(std::string value);

// unescape all escape sequences in string
std::string unescapeSequences(const std::string &value);