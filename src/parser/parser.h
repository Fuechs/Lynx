#pragma once

#include "../ast/function.h"
#include "../lexer/token.h"
#include "../ast/expr.h"

class Parser {
public:
    explicit Parser(Token::Vec tokens);

    Root::Ptr parse();

    Stmt::Ptr parseStmt();
    Stmt::Ptr parseFunctionStmt();
    Stmt::Ptr parseVariableStmt();
    Stmt::Ptr parseReturnStmt();

    Expr::Ptr parseExpr();
    Expr::Ptr parseAssignmentExpr();
    Expr::Ptr parseCallExpr();
    Expr::Ptr parseBlockExpr();
    Expr::Ptr parseAdditiveExpr();
    Expr::Ptr parseMultiplicativeExpr();
    Expr::Ptr parsePowerExpr();
    Expr::Ptr parseAddressOfExpr();
    Expr::Ptr parseDereferenceExpr();
    Expr::Ptr parseIncrementDecrementExpr();
    Expr::Ptr parsePrimaryExpr();

    Type::Ptr parseType(bool parameter = false);
    std::pair<std::string, Type::Ptr> parseFunctionParameter();

private:
    // advance to the next token and return the current
    constexpr const Token &eat();
    // advance to the next token and return true if the current token is of the given type
    constexpr bool eat(TokenType type);
    // advance to the next token and return true if the current token has the given value
    constexpr bool eat(const std::string &value);
    // advance to the next token if the current token is of the given type, throw error otherwise
    const Token &expect(TokenType type);
    // peek to the next token, further or back (does not check for EOF)
    [[nodiscard]] constexpr const Token &peek(int offset = 1) const;

    Root::Ptr root;
    Token::Vec tokens;
    Token::Vec::const_iterator it;
};
