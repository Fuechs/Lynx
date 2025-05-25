#pragma once

#include "../lexer/token.h"
#include "../ast/expr.h"

class Parser {
public:
    explicit Parser(const Token::Vec &tokens);

    RootAST::Ptr parse();

    StmtAST::Ptr parseStmt();

    Expr::Ptr parseExpr();
    Expr::Ptr parseAdditiveExpr();
    Expr::Ptr parseMultiplicativeExpr();
    Expr::Ptr parsePrimaryExpr();

private:
    constexpr const Token &eat();

    RootAST::Ptr root;
    Token::Vec tokens;
    Token::Vec::const_iterator it;
};
