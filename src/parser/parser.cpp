#include "parser.h"

#include <utility>

Parser::Parser(Token::Vec tokens) : root(std::make_shared<Root>()), tokens(std::move(tokens)) {}

Root::Ptr Parser::parse() {
    it = tokens.begin();

    while (it != tokens.end())
        root->addStmt(parseStmt());

    return root;
}

Stmt::Ptr Parser::parseStmt() { return parseExpr(); }

Expr::Ptr Parser::parseExpr() { return parseBlockExpr(); }

Expr::Ptr Parser::parseBlockExpr() {
    if (eat(LBRACE)) {
        Stmt::Vec stmts = {};

        while (!eat(RBRACE))
            if (it == tokens.end())
                std::cerr << "Expected '}' at line " << it->getLine() << ":" << it->getStart() << std::endl;
            else {
                stmts.push_back(parseStmt());
                expect(SEMICOLON);
            }

        return std::make_shared<BlockExpr>(stmts);
    }

    return parseAdditiveExpr();
}

Expr::Ptr Parser::parseAdditiveExpr() {
    Expr::Ptr LHS = parseMultiplicativeExpr();

    while (*it == PLUS || *it == MINUS) {
        auto op = eat() == PLUS ? BinaryOp::ADD : BinaryOp::SUB;
        LHS = std::make_shared<BinaryExpr>(op, LHS, parseMultiplicativeExpr());
    }

    return LHS;
}

Expr::Ptr Parser::parseMultiplicativeExpr() {
    Expr::Ptr LHS = parsePowerExpr();

    while (*it == ASTERISK || *it == SLASH) {
        auto op = eat() == ASTERISK ? BinaryOp::MUL : BinaryOp::DIV;
        LHS = std::make_shared<BinaryExpr>(op, LHS, parsePowerExpr());
    }

    return LHS;
}

Expr::Ptr Parser::parsePowerExpr() {
    Expr::Ptr LHS = parsePrimaryExpr();

    while (eat(CARET))
        LHS = std::make_shared<BinaryExpr>(BinaryOp::POW, LHS, parsePrimaryExpr());

    return LHS;
}


Expr::Ptr Parser::parsePrimaryExpr() {
    switch (it->getType()) {
        // FIXME: case IDENTIFIER: return std::make_shared<SymbolExpr>(eat().getValue());
        case NUMBER: return std::make_shared<NumberExpr>(eat().getValue());
        case LPAREN: {
            ++it;
            Expr::Ptr expr = parseExpr();
            expect(RPAREN);
            return expr;
        }

        default: return nullptr;
    }
}

constexpr const Token &Parser::eat() {
    if (it != tokens.end())
        return *it++;

    return *it;
}

constexpr bool Parser::eat(TokenType type) {
    if (it != tokens.end() && *it == type) {
        ++it;
        return true;
    }

    return false;
}

const Token &Parser::expect(TokenType type) {
    if (it == tokens.end() || *it != type)
        // TODO: proper errors
        std::cerr << "Expected '"+Token::getTypeValue(type)+"' at line " << it->getLine() << ":" << it->getStart() << std::endl;

    if (it != tokens.end())
        ++it;

    return *it;
}