/**
 * @file parser.cpp
 * @author fuechs
 * @brief fux parser
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "parser.hpp"

RootPtr Parser::parse() {
    // lexing
    tokens = lexer->lex();
    lexer->debugPrint();
    
    current = tokens.begin();

    StmtPtr branch;
    while (notEOF()) 
        if ((branch = parseStmt())) // check for nullptr in case of error
            root->addSub(branch);
    
    return std::move(root);
}

StmtPtr Parser::parseStmt() {
    StmtPtr stmt = parseIfElseStmt();
    expect(SEMICOLON);
    return std::move(stmt);
}

StmtPtr Parser::parseIfElseStmt() {
    StmtPtr stmt;

    if (current->type == KEY_IF) {
        eat();
        expect(LPAREN, ILLEGAL_BRACKET_MISMATCH);
        ExprPtr condition = parseExpr();
        expect(LPAREN, MISSING_BRACKET);
        StmtPtr thenBody = parseStmt(); 
        if (current->type == KEY_ELSE) {
            eat();
            StmtPtr elseBody = parseStmt(); 
            stmt = make_unique<IfElseAST>(condition, thenBody, elseBody);
        } else
            stmt = make_unique<IfElseAST>(condition, thenBody);
    } else
        stmt = parsePutsStmt();

    return stmt;
}

StmtPtr Parser::parsePutsStmt() {
    StmtPtr call;
    if (current->type == KEY_PUTS) {
        eat();
        ExprPtr arg = parseExpr();
        call = make_unique<PutsCallAST>(arg);
    } else 
        call = parseVariableDeclStmt();

    return call;
}

StmtPtr Parser::parseVariableDeclStmt() {
    // TODO: parse storage modifiers
    FuxType::AccessList access = {FuxType::PUBLIC};
    // TODO: parse pointer depth
    size_t pointerDepth = 0;

    const string symbol = eat().value;
    FuxType type = FuxType();

    // TODO: check for this in parseStmt
    if (check(COLON)) {
        type = parseType(pointerDepth, access);
        
        if (check(EQUALS)) { // =
            if (!type)
                type = FuxType(FuxType::AUTO, 0, access);
        } else if (check(TRIPLE_EQUALS)) { // ===
            if (!type) {
                access.push_back(FuxType::CONSTANT);
                type = FuxType(FuxType::AUTO, pointerDepth, access);
            } else
                type.access.push_back(FuxType::CONSTANT);
        }
    } else if (current->type == EQUALS || current->type == TRIPLE_EQUALS) { // assignment 
        --current; // get back to identifier
        return parseAssignmentExpr();
    } else
        return parseExpr();

    ExprPtr value = parseExpr();
    return make_unique<VariableDeclAST>(symbol, type, value);
}

ExprPtr Parser::parseExpr() { return parseAssignmentExpr(); }

ExprPtr Parser::parseAssignmentExpr() { 
    ExprPtr dest = parseCallExpr();

    if (check(EQUALS)) {
        ExprPtr value = parseExpr();
        return make_unique<AssignmentExprAST>(dest, value);
    } 
    
    if (check(TRIPLE_EQUALS)) {
        ExprPtr value = parseExpr();
        return make_unique<AssignmentExprAST>(dest, value, true);
    }
    
    return std::move(dest);
}

ExprPtr Parser::parseCallExpr() { return parseAdditiveExpr(); } // ! skipping logicalexpr here

ExprPtr Parser::parseLogicalExpr() {
    ExprPtr LHS = parseComparisonExpr();

    while (current->type == AND || current->type == OR) {
        TokenType logical = eat().type;
        ExprPtr RHS = parseComparisonExpr();
        LHS = make_unique<LogicalExprAST>(logical, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseComparisonExpr() {
    ExprPtr LHS = parseExpr();

    while (current->type >= EQUALS_EQUALS && current->type <= GTEQUALS) {
        TokenType comp = eat().type;
        ExprPtr RHS = parseExpr();
        LHS = make_unique<ComparisonExprAST>(comp, LHS, RHS);
    }

    return LHS;
} 

ExprPtr Parser::parseAdditiveExpr() {
    ExprPtr LHS = parseMultiplicativeExpr();

    while (current->type == PLUS || current->type == MINUS) {
        char op = current->value.front(); // get '+' or '-' 
        ++current;
        ExprPtr RHS = parseMultiplicativeExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parseMultiplicativeExpr() {
    ExprPtr LHS = parsePowerExpr();

    while (current->type == ASTERISK || current->type == SLASH || current->type == PERCENT) {
        char op = current->value.front(); // get '*', '/', '%' 
        ++current;
        ExprPtr RHS = parsePowerExpr();
        LHS = make_unique<BinaryExprAST>(op, LHS, RHS);
    }

    return LHS;
}

ExprPtr Parser::parsePowerExpr() { 
    ExprPtr LHS = parseUnaryExpr();

    while (check(CARET)) {
        ExprPtr RHS = parseUnaryExpr();
        LHS = make_unique<BinaryExprAST>('^', LHS, RHS);
    }    

    return LHS;
}

ExprPtr Parser::parseUnaryExpr() { return parsePrimaryExpr(); }

ExprPtr Parser::parsePrimaryExpr() {
    Token that = eat();
    // TODO: unary epxr
    switch (that.type) {
        case NUMBER:        return make_unique<NumberExprAST, _i64>(stoll(that.value));
        case FLOAT:         return make_unique<NumberExprAST, _f64>(stod(that.value));
        case IDENTIFIER:    {
            ExprPtr primary = make_unique<VariableExprAST>(that.value);

            if (!check(DOT)) 
                return primary;
            
            ExprPtr member = parsePrimaryExpr(); 
            return make_unique<MemberExprAST>(primary, member);
        }
        case LPAREN: {
            ExprPtr expr = parseExpr();
            expect(RPAREN, MISSING_BRACKET);
            return expr;
        }
        default: {        
            stringstream message;
            message 
                << "unexpected token " << TokenTypeString[that.type]
                << " '" << that.value << "' while parsing primary expression";
            error->createError(UNEXPECTED_TOKEN, that, message.str());
            return parsePrimaryExpr();
        }
    }
}

FuxType Parser::parseType(_i64 pointerDepth, FuxType::AccessList access) {
    if (!current->isType()) 
        return FuxType(); // = NO_TYPE; will be checked by analyser
    
    Token typeToken = eat();
    if (check(ARRAY_BRACKET))
        return FuxType::createArray((FuxType::Kind) typeToken.type, pointerDepth, access, typeToken.value);
    else if (check(LBRACKET)) {
        ExprPtr size = parseExpr(); 
        expect(RBRACKET, MISSING_BRACKET);
        return FuxType::createArray((FuxType::Kind) typeToken.type, pointerDepth, access, typeToken.value, size);
    } else
        return FuxType::createStd((FuxType::Kind) typeToken.type, pointerDepth, access, typeToken.value);
}

Token Parser::eat() {
    if (current->type == _EOF)
        return *current;
    return *current++;
}

Token Parser::expect(TokenType type, ErrorType errType) {
    Token curTok = eat();
    if (curTok.type != type) {
        stringstream err;
        err 
            << "expected " << TokenTypeString[type] 
            << " '" << TokenTypeValue[type] << "', "
            << "got " << TokenTypeString[curTok.type] 
            << " '" << curTok.value << "' instead";
        error->createError(errType, curTok, err.str());
        return Token();
    }

    return curTok;
}

Token Parser::peek(size_t steps) { return *(current + steps); }

bool Parser::check(TokenType type) {
    if (current->type != type) 
        return false;
    
    ++current;
    return true;
}

constexpr bool Parser::notEOF() { return current->type != _EOF; }