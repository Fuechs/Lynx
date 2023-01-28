/**
 * @file debug.cpp
 * @author fuechs
 * @brief debug function for every class
 * @version 0.1
 * @date 2022-12-24
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "color.hpp"

#include "../frontend/lexer/token.hpp"
#include "../frontend/lexer/lexer.hpp"
#include "../frontend/ast/ast.hpp"
#include "../frontend/parser/parser.hpp"
#include "../frontend/parser/value.hpp"
#include "../frontend/analyser/analyser.hpp"
#include "../frontend/error/error.hpp"
#ifdef FUX_BACKEND
#include "../backend/context/context.hpp"
#include "../backend/generator/generator.hpp"
#include "../backend/compiler/compiler.hpp"
#include "threading.hpp"
#endif

// * LEXER

void Lexer::debugPrint() {
    if (!fux.options.debugMode)
        return;
    
    cout << debugText << "Lexer:\n";
    for (Token &token : tokens)
        cout << token.str() << "\n";
}

// * PARSER

void NullExprAST::debugPrint() { cout << "null"; }

void BoolExprAST::debugPrint() { value->debugPrint(); }

void NumberExprAST::debugPrint() { value->debugPrint(); }

void CharExprAST::debugPrint() { value->debugPrint(); }

void StringExprAST::debugPrint() { value->debugPrint(); }

void VariableExprAST::debugPrint() { cout << name; }

void MemberExprAST::debugPrint() {
    base->debugPrint();
    cout << ".";
    member->debugPrint();
}

void UnaryExprAST::debugPrint() {
    cout << "(";
    if (op == UnaryOp::SINC || op == UnaryOp::SDEC) {
        expr->debugPrint();
        cout << UnaryOpValue(op);
    } else {
        cout << UnaryOpValue(op);
        expr->debugPrint();
    }
    cout << ")";
}

void BinaryExprAST::debugPrint() {
    cout << "(";
    LHS->debugPrint();
    if (op == BinaryOp::IDX) { // handle <expr>[<expr>] and <expr>[]
        if (!RHS) cout << "[]";
        else {        
            cout << "[";
            RHS->debugPrint();
            cout << "]";
        }
    } else {
        cout << " " << BinaryOpValue(op) << " ";
        RHS->debugPrint();
    }
    cout << ")";
}

void CallExprAST::debugPrint() { 
    callee->debugPrint();
    cout << "(";
    for (StmtPtr &arg : args) {
        arg->debugPrint();
        if (arg != args.back())
            cout << ", ";
    }
    cout << ")";
}

void TypeCastExprAST::debugPrint() {
    cout << "((";
    type.debugPrint(true);
    cout << ") ";
    expr->debugPrint();
    cout << ")";
}

void TernaryExprAST::debugPrint() {
    cout << "(";
    condition->debugPrint();
    cout << " ? ";
    thenExpr->debugPrint();
    cout << " : ";
    elseExpr->debugPrint();
    cout << ")";
}

void VariableDeclAST::debugPrint() {
    cout << symbol;
    type.debugPrint();
    if (value) {
        cout << " = ";
        value->debugPrint();
    }
}

void InbuiltCallAST::debugPrint() {
    cout << InbuiltsValue(callee) << " ";
    for (ExprPtr &arg : arguments) {
        arg->debugPrint();
        if (arg != arguments.back())
            cout << ", "; 
    }
}

void IfElseAST::debugPrint() {
    cout << "if (";
    condition->debugPrint();
    cout << ") ";
    thenBody->debugPrint();
    if (!elseBody)
        return;
    cout << ";\nelse ";
    elseBody->debugPrint();
} 

// TODO: correct indentation for nested blocks
void CodeBlockAST::debugPrint() {
    cout << "{\n";
    for (StmtPtr &stmt : body) {
        cout << "\t";
        stmt->debugPrint();
        cout << ";\n";
    }
    cout << "}";
}

void WhileLoopAST::debugPrint() {
    cout << "while (";
    condition->debugPrint();
    cout << ") ";
    if (body) body->debugPrint();
}

void ForLoopAST::debugPrint() {
    cout << "for (";
    if (initial) initial->debugPrint();
    if (forEach) {
        cout << " in ";
        if (iterator) iterator->debugPrint();
    } else {
        cout << "; ";
        if (condition) condition->debugPrint();
        cout << "; ";
        if (iterator) iterator->debugPrint();
    }

    cout << ") ";
    if (body) body->debugPrint();
}

void PrototypeAST::debugPrint() {
    symbol->debugPrint();
    cout << "(";
    for (StmtPtr &param : args) {
        param->debugPrint();
        if (param != *--args.end())
            cout << ", ";
    }
    cout << ")";
    type.debugPrint();
}

void FunctionAST::debugPrint() { 
    proto->debugPrint();
    cout << " ";
    body->debugPrint();
}

void RootAST::debugPrint() {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Root AST";
    for (StmtPtr &sub : program) {
        cout << "\n";
        sub->debugPrint();
        cout << ";";
    }
    cout << endl;
}

void Parser::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText<< "Parser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

void ValueStruct::debugPrint() {
    switch (type.kind) {
        case FuxType::BOOL:     cout << (__bool ? "true" : "false"); break;
        case FuxType::I8:       cout << __i8; break;
        case FuxType::U8:       cout << to_string(__u8); break;
        case FuxType::C8:       cout << "'" << __c8 << "'"; break;
        case FuxType::I16:      cout << __i16; break;
        case FuxType::U16:      cout << __u16; break;
        case FuxType::C16:      cout << "'" << to_string(__c16) << "'"; break;
        case FuxType::I32:      cout << __i32; break;
        case FuxType::U32:      cout << __u32; break;
        case FuxType::F32:      cout << __f32; break;
        case FuxType::I64:      cout << __i64; break;
        case FuxType::U64:      cout << __u64; break;
        case FuxType::F64:      cout << __f64; break;
        case FuxType::STR:      cout << '"' << __str << '"'; break;
        default:                cout << "???";
    }
}

// * ANALYSER

void Analyser::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Analyser";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * ERROR

void ErrorManager::debugPrint() { return; }

#ifdef FUX_BACKEND

// * CONTEXT

void FuxContext::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;
    
    cout << debugText << "FuxContext";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * GENERATOR

void Generator::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;

    cout << debugText << "Generator";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * COMPILER

void Compiler::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;
        
    cout << debugText << "Compiler";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

// * THREADING

void fuxThread::Thread::debugPrint(const string message) {  
    if (!fux.options.debugMode)
        return;
            
    cout << debugText << "Thread '" << name << "' (" << id << ")";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

void fuxThread::ThreadManager::debugPrint(const string message) {
    if (!fux.options.debugMode)
        return;
        
    cout << debugText << "ThreadManager";
    if (!message.empty())
        cout << ": " << message;
    cout << "\n";
}

#endif