#pragma once

#include "../lexer/token.h"
#include "../ast/expr.h"

class Parser {
public:
    explicit Parser(Token::Vec tokens);

    Root::Ptr parse();

    Stmt::Ptr parseStmt();

    Expr::Ptr parseExpr();
    Expr::Ptr parseBlockExpr();
    Expr::Ptr parseAdditiveExpr();
    Expr::Ptr parseMultiplicativeExpr();
    Expr::Ptr parsePowerExpr();
    Expr::Ptr parsePrimaryExpr();

private:
    // advance to the next token and return the current
    constexpr const Token &eat();
    // advance to the next token and return true if the current token is of the given type
    constexpr bool eat(TokenType type);
    // advance to the next token if the token is of the given type, throw error otherwise
    const Token &expect(TokenType type);

    Root::Ptr root;
    Token::Vec tokens;
    Token::Vec::const_iterator it;
};
