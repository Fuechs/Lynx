#include "parser.h"

#include <thread>
#include <utility>

#include "function.h"

Parser::Parser(Token::Vec tokens) : root(std::make_shared<Root>()), tokens(std::move(tokens)) {}

Root::Ptr Parser::parse() {
    it = tokens.begin();

    Stmt::Ptr stmt;
    while (it != tokens.end())
        if ((stmt = parseStmt())) { // check if stmt isn't null
            if (stmt->kind() != AST::Block
                && stmt->kind() != AST::Function) // expect ';' after stmt
                expect(SEMICOLON);
            root->addStmt(stmt);
        }

    return root;
}

Stmt::Ptr Parser::parseStmt() { return parseFunctionStmt(); }

Stmt::Ptr Parser::parseFunctionStmt() {
    if (*it == IDENTIFIER && peek() == LPAREN) {
        auto start = it++; // start at the lparen

        // skip any other parens
        for (size_t depth = 0;;) {
            if      (eat(LPAREN)) ++depth;
            else if (eat(RPAREN)) --depth;
            else if (it == tokens.end()) {
                std::cerr << "Expected ')' at line " << it->getLine() << ":" << it->getStart() << std::endl;
                return nullptr;
            } else eat();

            // it doesn't check whether depth is < 0
            // as the function parameters will be parsed
            // later and any error doesn't matter yet
            if (depth <= 0)
                break;
        }

        if (*it != COLON && *it != POINTER) { // not a function declaration but a function call
            it =  start;
            // We have to call parseExpr() instead of parseCallExpr() to handle situations like this one:
            // someCall() = x;
            return parseExpr();
        }

        it = start;
        std::string symbol = eat().getValue();
        eat(LPAREN);

        FunctionParameter::Vec parameters = {};
        if (*it != RPAREN)
            do {
                parameters.push_back(parseFunctionParameter());
            } while (eat(COMMA));
        expect(RPAREN);

        Type::Ptr type = parseType();

        if (*it == SEMICOLON)
            return std::make_shared<FunctionPrototype>(symbol, type, parameters);

        Stmt::Ptr body;
        if (*it == LBRACE)
            body = parseBlockExpr();
        else {
            body = parseStmt();
            expect(SEMICOLON);
        }
        return std::make_shared<Function>(symbol, type, parameters, body);
    }

    return parseVariableStmt();
}

Stmt::Ptr Parser::parseVariableStmt() {
    if (*it == IDENTIFIER && (peek() == COLON || peek() == POINTER)) {
        std::string symbol = eat().getValue();
        Type::Ptr type = parseType();
        Expr::Ptr value = nullptr;

        if (eat(EQUALS))
            value = parseExpr();

        return std::make_shared<VariableStmt>(symbol, type, value);
    }

    return parseReturnStmt();
}

Stmt::Ptr Parser::parseReturnStmt() {
    if (it->getValue() == "ret") {
        eat();
        return std::make_shared<ReturnStmt>(parseExpr());
    }

    return parseExpr();
}


Expr::Ptr Parser::parseExpr() { return parseAssignmentExpr(); }

Expr::Ptr Parser::parseAssignmentExpr() {
    Expr::Ptr LHS = parseCallExpr();

    if (LHS && eat(EQUALS))
        return std::make_shared<AssignmentExpr>(LHS ,parseExpr());

    return LHS;
}


Expr::Ptr Parser::parseCallExpr() {
    Expr::Ptr callee = parseBlockExpr();

    if (eat(LPAREN)) {
        Expr::Vec args = {};

        if (!eat(RPAREN)) {
            do args.push_back(parseExpr()); while (eat(COMMA));
            expect(RPAREN);
        }

        return std::make_shared<CallExpr>(callee, args);
    }

    return callee;
}

Expr::Ptr Parser::parseBlockExpr() {
    if (eat(LBRACE)) {
        Stmt::Vec stmts = {};

        while (!eat(RBRACE))
            if (it == tokens.end())
                std::cerr << "Expected '}' at line " << it->getLine() << ":" << it->getStart() << std::endl;
            else {
                stmts.push_back(parseStmt());
                if (stmts.back()->isExpr() && *it != SEMICOLON) { // convert trailing expr to return stmt
                    auto expr = std::static_pointer_cast<Expr>(stmts.back());
                    stmts.pop_back();
                    stmts.push_back(std::make_shared<ReturnStmt>(expr));
                } else if (stmts.back()->kind() != AST::Block)
                    expect(SEMICOLON);
            }

        return std::make_shared<BlockExpr>(stmts);
    }

    return parseAdditiveExpr();
}

Expr::Ptr Parser::parseAdditiveExpr() {
    Expr::Ptr LHS = parseMultiplicativeExpr();

    while (*it == PLUS || *it == MINUS) {
        auto op = eat() == PLUS ? ADD : SUB;
        LHS = std::make_shared<BinaryExpr>(op, LHS, parseMultiplicativeExpr());
    }

    return LHS;
}

Expr::Ptr Parser::parseMultiplicativeExpr() {
    Expr::Ptr LHS = parsePowerExpr();

    while (*it == ASTERISK || *it == SLASH) {
        auto op = eat() == ASTERISK ? MUL : DIV;
        LHS = std::make_shared<BinaryExpr>(op, LHS, parsePowerExpr());
    }

    return LHS;
}

Expr::Ptr Parser::parsePowerExpr() {
    Expr::Ptr LHS = parseAddressOfExpr();

    while (eat(CARET))
        LHS = std::make_shared<BinaryExpr>(POW, LHS, parseAddressOfExpr());

    return LHS;
}

Expr::Ptr Parser::parseAddressOfExpr() {
    if (eat(BIT_AND))
        return std::make_shared<UnaryExpr>(ADDR, parseAddressOfExpr());

    return parseDereferenceExpr();
}

Expr::Ptr Parser::parseDereferenceExpr() {
    if (eat(ASTERISK))
        return std::make_shared<UnaryExpr>(DEREF, parseDereferenceExpr());

    return parseIncrementDecrementExpr();
}

Expr::Ptr Parser::parseIncrementDecrementExpr() {
    Expr::Ptr LHS;

    if (*it == MINUS_MINUS || *it == PLUS_PLUS)
        LHS = std::make_shared<UnaryExpr>(eat() == MINUS_MINUS ? PRE_DEC : PRE_INC, parseIncrementDecrementExpr());
    else
        LHS = parsePrimaryExpr();

    while (*it == MINUS_MINUS || *it == PLUS_PLUS)
        LHS = std::make_shared<UnaryExpr>(eat() == MINUS_MINUS ? POST_DEC : POST_INC, LHS);

    return LHS;
}

Expr::Ptr Parser::parsePrimaryExpr() {
    switch (it->getType()) {
        case IDENTIFIER: return std::make_shared<SymbolExpr>(eat().getValue());
        case NUMBER:
        case LITERAL: return std::make_shared<ValueExpr>(eat());
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

Type::Ptr Parser::parseType(bool expectColon) {
    Type::Ptr type = nullptr;
    const bool reference = expectColon ? eat() == POINTER : eat(POINTER);

    if (*it == IDENTIFIER) {
        type = Type::create(eat());

        while (eat(ASTERISK))
            type = type->getPointerTo();
    } else
        type = std::make_shared<Type>(Type::AUTO);

    if (reference) // TODO: handle this
        std::cerr << "References aren't implemented yet." << std::endl;

    return type;
}

FunctionParameter Parser::parseFunctionParameter() {
    if (peek() == COLON || peek() == POINTER) { // parameter has a name
        std::string symbol = expect(IDENTIFIER).getValue();
        Type::Ptr type = parseType();
        return FunctionParameter(type, symbol);
    }

    return FunctionParameter(parseType(false));
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
        return *it++;

    return *it;
}