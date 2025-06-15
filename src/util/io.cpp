#include <iostream>
#include <fstream>
#include <sstream>
#include "io.h"

std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "could not open file '" << filePath << "'\n";
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string escapeSequences(std::string value) {
    std::string buffer;

    for (size_t i = 0; i < value.size(); i++)
        if (value.at(i) == '\\')
            switch (value.at(++i)) {
                case 'a':   buffer += '\a'; break;  // alert
                case 'b':   buffer += '\b'; break;  // backspace
                case 'f':   buffer += '\f'; break;  // form feed
                case 'n':   buffer += '\n'; break;  // new line
                case 'r':   buffer += '\r'; break;  // carriage return
                case 't':   buffer += '\t'; break;  // horizontal tab
                case 'v':   buffer += '\v'; break;  // vertical tab
                case '\'':  buffer += '\''; break;  // single quote
                case '\"':  buffer += '\"'; break;  // double quote
                case '\?':  buffer += '\?'; break;  // question mark
                case '\\':  buffer += '\\'; break;  // backslash
                default:    break;                  // remove invalid escape sequences
            }
        else
            buffer += value.at(i);

    return buffer;
}

std::string unescapeSequences(const std::string &value) {
    std::string buffer;

    for (char i : value)
        switch (i) {
            case '\a':  buffer += "\\a"; break;
            case '\b':  buffer += "\\b"; break;
            case '\f':  buffer += "\\f"; break;
            case '\n':  buffer += "\\n"; break;
            case '\r':  buffer += "\\r"; break;
            case '\t':  buffer += "\\t"; break;
            case '\v':  buffer += "\\v"; break;
            case '\'':  buffer += "\\'"; break;
            case '\"':  buffer += "\\\""; break;
            case '\?':  buffer += "\\?"; break;
            case '\\':  buffer += "\\\\"; break;
            default:    buffer += i; break;
        }

    return buffer;
}