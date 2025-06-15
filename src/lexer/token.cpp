#include "token.h"

#include <utility>

const char *tokenTypeNames[] = {
    "lparen",
    "rparen",
    "lbracket",
    "rbracket",
    "lbrace",
    "rbrace",
    "colon",
    "semicolon",
    "dot",
    "comma",
    "pointer",
    "at",

    "equals",
    "triple_equals",
    "plus_plus",
    "minus_minus",
    "plus_equals",
    "minus_equals",
    "asterisk_equals",
    "slash_equals",
    "caret_equals",
    "percent_equals",
    "or_equals",
    "and_equals",
    "lsh_equals",
    "rsh_equals",
    "swap",

    "equals_equals",
    "not_equals",
    "ltequals",
    "gtequals",
    "lessthan",
    "greaterthan",
    "exclamation",
    "question",
    "or",
    "and",

    "plus",
    "minus",
    "asterisk",
    "slash",
    "caret",
    "percent",

    "bit_not",
    "bit_or",
    "bit_xor",
    "bit_and",
    "bit_lshift",
    "bit_rshift",

    "identifier",
    "number",
    "literal",
};

const char *tokenTypeValues[] = {
    "(",
    ")",
    "[",
    "]",
    "{",
    "}",
    ":",
    ";",
    ".",
    ",",
    "->",
    "@",

    "=",
    "===",
    "++",
    "--",
    "+=",
    "-=",
    "*=",
    "/=",
    "^=",
    "%=",
    "|=",
    "&=",
    "<<=",
    ">>=",
    "<>",

    "==",
    "!=",
    "<=",
    ">=",
    "<",
    ">",
    "!",
    "?",
    "||",
    "&&",

    "+",
    "-",
    "*",
    "/",
    "^",
    "%",

    "~",
    "|",
    "><",
    "&",
    "<<",
    ">>",

    "identifier",
    "number",
    "literal",
};

Token::Token(const TokenType &tokenType, std::string value, size_t line, size_t start, size_t end)
: tokenType(tokenType), value(std::move(value)), line(line), start(start), end(end) {}

Token::~Token() { value.clear(); }

bool Token::operator==(TokenType type) const { return this->tokenType == type; }

std::string Token::str() const {
    return "<" + std::string(tokenTypeNames[tokenType]) + ", '" + value
        + "' at line "+std::to_string(line)+":"+std::to_string(start)+"-"+std::to_string(end)+">";
}

TokenType Token::getType() const { return tokenType; }

std::string Token::getValue() const { return value; }

size_t Token::getLine() const { return line; }

size_t Token::getStart() const { return start; }

size_t Token::getEnd() const { return end; }

std::string Token::getTypeName(TokenType type) { return tokenTypeNames[type]; }

std::string Token::getTypeValue(TokenType type) { return tokenTypeValues[type]; }