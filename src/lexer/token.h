#pragma once

#include <vector>

enum TokenType {
    // structure
    LPAREN,         // (
    RPAREN,         // )
    LBRACKET,       // [
    RBRACKET,       // ]
    LBRACE,         // {
    RBRACE,         // }
    COLON,          // :
    SEMICOLON,      // ;
    DOT,            // .
    COMMA,          // ,
    POINTER,        // ->
    AT,             // @

    // assignment
    EQUALS,         // =
    TRIPLE_EQUALS,  // ===
    PLUS_PLUS,      // ++
    MINUS_MINUS,    // --
    PLUS_EQUALS,    // +=
    MINUS_EQUALS,   // -=
    ASTERISK_EQUALS,// *=
    SLASH_EQUALS,   // /=
    CARET_EQUALS,   // ^=
    PERCENT_EQUALS, // %=
    OR_EQUALS,      // |=
    AND_EQUALS,     // &=
    LSH_EQUALS,     // <<=
    RSH_EQUALS,     // >>=
    SWAP,           // <>

    // condition
    EQUALS_EQUALS,  // ==
    NOT_EQUALS,     // !=
    LTEQUALS,       // <=
    GTEQUALS,       // >=
    LESSTHAN,       // <
    GREATERTHAN,    // >
    EXCLAMATION,    // !
    QUESTION,       // ?
    OR,             // ||
    AND,            // &&

    // operators
    PLUS,           // +
    MINUS,          // -
    ASTERISK,       // *
    SLASH,          // /
    CARET,          // ^
    PERCENT,        // %

    // bitwise operation
    BIT_NOT,        // ~
    BIT_OR,         // |
    BIT_XOR,        // ><
    BIT_AND,        // &
    BIT_LSHIFT,     // <<
    BIT_RSHIFT,     // >>

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

    [[nodiscard]] std::string str() const;

    [[nodiscard]] TokenType getType() const;
    [[nodiscard]] std::string getValue() const;
    [[nodiscard]] size_t getLine() const;
    [[nodiscard]] size_t getStart() const;
    [[nodiscard]] size_t getEnd() const;

    [[nodiscard]] static std::string getTypeName(TokenType type);
    [[nodiscard]] static std::string getTypeValue(TokenType type);

private:
    TokenType tokenType;
    std::string value;
    size_t line, start, end;
};

