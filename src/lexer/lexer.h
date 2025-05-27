#pragma once

#include <string>
#include "token.h"

class Lexer {
public:
     explicit Lexer(const std::string &source);
     ~Lexer();

     Token::Vec lex() const;

private:
    const std::string &source;
};

