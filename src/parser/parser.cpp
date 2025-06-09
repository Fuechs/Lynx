#include "parser.h"

#include <utility>

Parser::Parser(Token::Vec tokens) : root(std::make_shared<Root>()), tokens(std::move(tokens)) {}

Root::Ptr Parser::parse() {
    it = tokens.begin();

    Stmt::Ptr stmt;
    while (it != tokens.end())
        if ((stmt = parseStmt())) { // check if stmt isn't null
            if (stmt->kind() != AST::Block) // expect ';' after stmt
                expect(SEMICOLON);
            root->addStmt(stmt);
        }

    return root;
}

Stmt::Ptr Parser::parseStmt() { return parseVariableStmt(); }

Stmt::Ptr Parser::parseVariableStmt() {
    if (*it == IDENTIFIER && (peek() == COLON || peek() == POINTER)) {
        std::string symbol = eat().getValue();
        Type::Ptr type = parseType();
        Expr::Ptr value = nullptr;

        if (eat(EQUALS))
            value = parseExpr();

        return std::make_shared<VariableStmt>(symbol, type, value);
    }

    return parseExpr();
}

Expr::Ptr Parser::parseExpr() { return parseAssignmentExpr(); }

Expr::Ptr Parser::parseAssignmentExpr() {
    Expr::Ptr LHS = parseBlockExpr();

    if (LHS && LHS->kind() == AST::Symbol && eat(EQUALS))
        return std::make_shared<AssignmentExpr>(LHS ,parseExpr());

    return LHS;
}

Expr::Ptr Parser::parseBlockExpr() {
    if (eat(LBRACE)) {
        Stmt::Vec stmts = {};

        while (!eat(RBRACE))
            if (it == tokens.end())
                std::cerr << "Expected '}' at line " << it->getLine() << ":" << it->getStart() << std::endl;
            else {
                stmts.push_back(parseStmt());
                if (stmts.back()->kind() != AST::Block)
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
        case IDENTIFIER: return std::make_shared<SymbolExpr>(eat().getValue());
        case NUMBER: return std::make_shared<NumberExpr>(eat().getValue());
        case LPAREN: {
            ++it;
            Expr::Ptr expr = parseExpr();
            expect(RPAREN);
            return expr;
        }

        default: {
            std::cerr << "Unexpected token '" << it->getValue() << "' at line " << it->getLine() << ":" << it->getStart() << std::endl;
            eat();
            return nullptr;
        }
    }
}

Type::Ptr Parser::parseType() {
    Type::Ptr type = nullptr;
    const bool reference = eat() == POINTER;

    if (*it == IDENTIFIER) {
        type = Type::create(eat());

        while (eat(ASTERISK))
            type = std::make_shared<PointerType>(type);
    } else
        type = std::make_shared<Type>(Type::AUTO);

    if (reference) // TODO: handle this
        std::cerr << "References aren't implemented yet." << std::endl;

    return type;
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

constexpr const Token &Parser::peek(int offset) const {
    return *(it+offset);
}

const Token &Parser::expect(TokenType type) {
    if (it == tokens.end() || *it != type)
        // TODO: proper errors
        std::cerr << "Expected '"+Token::getTypeValue(type)+"' at line " << it->getLine() << ":" << it->getStart() << std::endl;

    if (it != tokens.end())
        ++it;

    return *it;
}