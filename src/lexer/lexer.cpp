#include <regex>
#include <iostream>
#include <sstream>
#include "lexer.h"

const std::vector<std::pair<TokenType, std::regex>> patterns = {
    // sorted after length due to priority when matching

    // undefined length
    {NUMBER, std::regex(R"(\d*\.\d+|\d+)")},
    {IDENTIFIER, std::regex(R"([a-zA-Z_]\w*)")},

    // three characters
    {TRIPLE_EQUALS, std::regex(R"(===)")},
    {LSH_EQUALS, std::regex(R"(<<=)")},
    {RSH_EQUALS, std::regex(R"(>>=)")},

    // two characters
    {POINTER, std::regex(R"(->)")},
    {PLUS_PLUS, std::regex(R"(\+\+)")},
    {MINUS_MINUS, std::regex(R"(--)")},
    {PLUS_EQUALS, std::regex(R"(\+=)")},
    {MINUS_EQUALS, std::regex(R"(-=)")},
    {ASTERISK_EQUALS, std::regex(R"(\*=)")},
    {SLASH_EQUALS, std::regex(R"(/=)")},
    {CARET_EQUALS, std::regex(R"(\^=)")},
    {PERCENT_EQUALS, std::regex(R"(%=)")},
    {OR_EQUALS, std::regex(R"(\|=)")},
    {AND_EQUALS, std::regex(R"(&=)")},
    {SWAP, std::regex(R"(<>)")},
    {EQUALS_EQUALS, std::regex(R"(==)")},
    {NOT_EQUALS, std::regex(R"(!=)")},
    {LTEQUALS, std::regex(R"(<=)")},
    {GTEQUALS, std::regex(R"(>=)")},
    {OR, std::regex(R"(\|\|)")},
    {AND, std::regex(R"(&&)")},
    {BIT_XOR, std::regex(R"(><)")},
    {BIT_LSHIFT, std::regex(R"(<<)")},
    {BIT_RSHIFT, std::regex(R"(>>)")},

    // one character
    {LPAREN, std::regex(R"(\()")},
    {RPAREN, std::regex(R"(\))")},
    {LBRACKET, std::regex(R"(\[)")},
    {RBRACKET, std::regex(R"(\])")},
    {LBRACE, std::regex(R"(\{)")},
    {RBRACE, std::regex(R"(\})")},
    {COLON, std::regex(R"(:)")},
    {SEMICOLON, std::regex(R"(;)")},
    {DOT, std::regex(R"(\.)")},
    {COMMA, std::regex(R"(,)")},
    {AT, std::regex(R"(@)")},
    {EQUALS, std::regex(R"(\=)")},
    {LESSTHAN, std::regex(R"(<)")},
    {GREATERTHAN, std::regex(R"(>)")},
    {EXCLAMATION, std::regex(R"(!)")},
    {QUESTION, std::regex(R"(\?)")},
    {PLUS, std::regex(R"(\+)")},
    {MINUS, std::regex(R"(-)")},
    {ASTERISK, std::regex(R"(\*)")},
    {SLASH, std::regex(R"(\/)")},
    {CARET, std::regex(R"(\^)")},
    {PERCENT, std::regex(R"(%)")},
    {BIT_NOT, std::regex(R"(~)")},
    {BIT_OR, std::regex(R"(\|)")},
    {BIT_AND, std::regex(R"(&)")},

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
            std::cout << "Trying to match " << current_line.substr(pos) << std::endl;
            for (const auto &[tokenType, pattern] : patterns) {
                std::string remaining = current_line.substr(pos);
                std::smatch match;

                try {
                    if (std::regex_search(remaining, match, pattern, std::regex_constants::match_continuous)) {
                        tokens.emplace_back(tokenType, match.str(), line, pos + 1, pos + match.length());
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