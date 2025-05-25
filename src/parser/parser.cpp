#include "parser.h"

Parser::Parser(const Token::Vec &tokens) : root(std::make_shared<RootAST>()), tokens(tokens) {}

RootAST::Ptr Parser::parse() {
    it = tokens.begin();

    while (it != tokens.end())
        root->addStmt(parseStmt());

    return root;
}

StmtAST::Ptr Parser::parseStmt() { return parseExpr(); }

Expr::Ptr Parser::parseExpr() { return parseAdditiveExpr(); }

Expr::Ptr Parser::parseAdditiveExpr() {
    Expr::Ptr LHS = parseMultiplicativeExpr();

    while (*it == PLUS || *it == MINUS) {
        auto op = eat() == PLUS ? BinaryOp::ADD : BinaryOp::SUB;
        LHS = std::make_shared<BinaryExpr>(op, LHS, parseMultiplicativeExpr());
    }

    return LHS;
}

Expr::Ptr Parser::parseMultiplicativeExpr() {
    Expr::Ptr LHS = parsePrimaryExpr();

    while (*it == ASTERISK || *it == SLASH) {
        auto op = eat() == ASTERISK ? BinaryOp::MUL : BinaryOp::DIV;
        LHS = std::make_shared<BinaryExpr>(op, LHS, parsePrimaryExpr());
    }

    return LHS;
}

Expr::Ptr Parser::parsePrimaryExpr() {
    if (*it == NUMBER)
        return std::make_shared<NumberExpr>(eat().getValue());

    return nullptr;
}

constexpr const Token &Parser::eat() {
    if (it != tokens.end())
        return *it++;

    return *it;
}