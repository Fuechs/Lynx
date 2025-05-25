#include <regex>
#include <iostream>
#include <sstream>
#include "lexer.h"

const std::vector<std::pair<TokenType, std::regex>> patterns = {
    {NUMBER, std::regex(R"(\d*\.\d+|\d+)")}, // needs to be top priority

    {PLUS, std::regex(R"(\+)")},
    {MINUS, std::regex(R"(-)")},
    {ASTERISK, std::regex(R"(\*)")},
    {SLASH, std::regex(R"(\/)")},

    {IDENTIFIER, std::regex(R"([a-zA-Z_]\w*)")}
};

Lexer::Lexer(const std::string &source) : source(source) {}

Lexer::~Lexer() = default;

Token::Vec Lexer::lex() const {
    Token::Vec tokens = {};
    std::string current_line;
    std::istringstream stream(source);
    size_t line = 1;

    while (std::getline(stream, current_line)) {
        size_t pos = 0;
        
        while (pos < current_line.length()) {
            // Skip whitespace
            while (pos < current_line.length() && std::isspace(current_line[pos]))
                pos++;
            
            if (pos >= current_line.length())
                break;

            bool matched = false;
            for (const auto &[tokenType, pattern] : patterns) {
                std::string remaining = current_line.substr(pos);
                std::smatch match;
                try {
                    if (std::regex_search(remaining, match, pattern, std::regex_constants::match_continuous)) {
                        tokens.emplace_back(tokenType, match.str(), line, pos, pos + match.length() - 1);
                        pos += match.length();
                        matched = true;
                        break;
                    }
                } catch (const std::regex_error& e) {
                    std::cerr << "Regex error: " << e.what() << std::endl;
                }
            }

            if (!matched) {
                std::cerr << "Invalid character at line " << line << ", position " << pos 
                         << " (ASCII: " << static_cast<int>(static_cast<unsigned char>(current_line[pos])) 
                         << ")" << std::endl;
                pos++;
            }
        }
        line++;
    }

    return tokens;
}