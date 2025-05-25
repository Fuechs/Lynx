#pragma once

#include <vector>

enum TokenType {
    // structure
    LPAREN,         // (
    RPAREN,         // )

    // operators
    PLUS,           // +
    MINUS,          // -
    ASTERISK,       // *
    SLASH,          // /

    // literals
    IDENTIFIER,
    NUMBER,
};

class Token {
public:
    using Vec = std::vector<Token>;

    Token(const TokenType &tokenType, const std::string &value, size_t line, size_t start, size_t end);
    ~Token();

    bool operator==(TokenType type) const;

    std::string str() const;

    TokenType getType() const;
    std::string getValue() const;

private:
    TokenType tokenType;
    std::string value;
    size_t line, start, end;
};

