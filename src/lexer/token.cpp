#include "token.h"

const char *tokenTypeNames[] = {
    "lparen",
    "rparen",

    "plus",
    "minus",
    "asterisk",
    "slash",

    "identifier",
    "number",
};

Token::Token(const TokenType &tokenType, const std::string &value, size_t line, size_t start, size_t end)
: tokenType(tokenType), value(value), line(line), start(start), end(end) {}

Token::~Token() { value.clear(); }

bool Token::operator==(TokenType type) const { return this->tokenType == type; }

std::string Token::str() const {
    return "<" + std::string(tokenTypeNames[tokenType]) + ", '" + value
        + "' at line "+std::to_string(line)+":"+std::to_string(start)+"-"+std::to_string(end)+">";
}

TokenType Token::getType() const { return tokenType; }

std::string Token::getValue() const { return value; }