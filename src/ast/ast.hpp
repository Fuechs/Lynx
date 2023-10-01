/**
 * @file ast.hpp
 * @author fuechs
 * @brief fux abstract syntax tree header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"
#include "../llvmheader.hpp"
#include "../lexer/token.hpp"
#include "../parser/type.hpp"
#include "../parser/value.hpp"
#include "expr.hpp"
#include "op.hpp"
#include "func.hpp"
#include "macro.hpp"

typedef map<string, Fux::Type> ArgMap;

/// EXPRESSIONS ///

class NullExpr : public Expr {
public:    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;    
};

class BoolExpr : public Expr {
    ValueStruct *value;

public:
    BoolExpr(bool value) : value(new ValueStruct(value)) {}
    ~BoolExpr();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class NumberExpr : public Expr {
    ValueStruct *value;

public:
    NumberExpr(_u64 value) : value(new ValueStruct(value)) {}
    NumberExpr(_f64 value) : value(new ValueStruct(value)) {}
    ~NumberExpr();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class CharExpr : public Expr {
    ValueStruct *value;

public:
    template<typename T>
    CharExpr(T value) : value(new ValueStruct(value)) {}
    ~CharExpr();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class StringExpr : public Expr {
    ValueStruct *value;

public:
    StringExpr(string value) : value(new ValueStruct(value)) {}
    ~StringExpr();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;) 
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class RangeExpr : public Expr {
    Expr::Ptr begin; 
    Expr::Ptr end; 

public:
    RangeExpr(Expr::Ptr &begin, Expr::Ptr &end) 
    : begin(std::move(begin)), end(std::move(end)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)  
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class ArrayExpr : public Expr {
    Expr::Vec elements;
    
public:
    ArrayExpr(Expr::Vec &elements) : elements(std::move(elements)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;) 
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class SymbolExpr : public Expr {
    string name;

public:
    SymbolExpr(const string& name) : name(name) {}
    ~SymbolExpr() override;

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)  
    AST getASTType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

class CallExpr : public Expr {
    Expr::Ptr callee;
    Expr::Vec args;
    bool asyncCall;

public:
    // parser has to insert end position!
    CallExpr(const string &callee, Expr::Vec &args, bool asyncCall = false);
    [[deprecated("Is not able to copy the position of the callee.")]]
    CallExpr(Expr::Ptr &callee, Expr::Vec &args, bool asyncCall = false);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;) 
    AST getASTType() override;
    Fux::Type getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

class MemberExpr : public Expr {
    Expr::Ptr parent;
    string member;

public:
    MemberExpr(Expr::Ptr &parent, const Token &member);
    [[deprecated("Is not able to copy the position of the member.")]]
    MemberExpr(Expr::Ptr &parent, const string &member);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)    
    AST getASTType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

class UnaryExpr : public Expr {
    UnaryOp op;
    Expr::Ptr expr;

public:
    UnaryExpr(const Token &op, Expr::Ptr &expr, bool postOp = false);
    [[deprecated("Is not able to copy the position of the operator.")]]
    UnaryExpr(UnaryOp op, Expr::Ptr &expr);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)  
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class BinaryExpr : public Expr {
    BinaryOp op;
    Expr::Ptr LHS, RHS;

public:
    BinaryExpr(BinaryOp op, Expr::Ptr &LHS, Expr::Ptr &RHS = nullExpr);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)   
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class TypeCastExpr : public Expr {
    Fux::Type type;
    Expr::Ptr expr;

public:
    TypeCastExpr(Fux::Type type, Expr::Ptr &expr);
    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class TernaryExpr : public Expr {
    Expr::Ptr condition;
    Expr::Ptr thenExpr;
    Expr::Ptr elseExpr;

public:
    TernaryExpr(Expr::Ptr &condition, Expr::Ptr &thenExpr, Expr::Ptr &elseExpr);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;) 
    AST getASTType() override;
    Fux::Type getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

/// STATEMENTS ///

class NoOperationStmt : public Stmt {
public:
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;    
};

class VariableStmt : public Stmt {
    string symbol;
    Fux::Type type;
    Expr::Ptr value;

public:
    using Ptr = unique_ptr<VariableStmt>;

    VariableStmt(string symbol, Fux::Type type = Fux::Type(), Expr::Ptr &value = nullExpr) 
    : symbol(symbol), type(type), value(std::move(value)) {}
    ~VariableStmt() override;
    
    Expr::Ptr &getValue();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    Fux::Type getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

typedef unique_ptr<VariableStmt> VarDeclPtr; 

class InbuiltCallStmt : public Stmt {
    Inbuilts callee;
    Expr::Vec arguments;

public:
    InbuiltCallStmt(Inbuilts callee, Expr::Vec &arguments) 
    : callee(callee), arguments(std::move(arguments)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
};

class IfElseStmt : public Stmt {
    Expr::Ptr condition;
    Stmt::Ptr thenBody;
    Stmt::Ptr elseBody;

public:
    IfElseStmt(Expr::Ptr &condition, Stmt::Ptr &thenBody, Stmt::Ptr &elseBody = nullStmt)
    : condition(std::move(condition)), thenBody(std::move(thenBody)), elseBody(std::move(elseBody)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
};

class BlockStmt : public Stmt {
    Stmt::Vec body;

public:
    using Ptr = shared_ptr<BlockStmt>;

    BlockStmt() : body(Stmt::Vec()) {}
    BlockStmt(Stmt::Vec &body) : body(std::move(body)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(Stmt::Ptr &sub);
};

class WhileStmt : public Stmt {
    Expr::Ptr condition;
    Stmt::Ptr body;
    bool postCondition;

public:
    WhileStmt(Expr::Ptr &condition, Stmt::Ptr &body, bool postCondition = false)
    : condition(std::move(condition)), body(std::move(body)), postCondition(postCondition) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
};

class ForStmt : public Stmt {
    bool forEach;
    /*for (*/Stmt::Ptr initial; 
            Expr::Ptr condition; 
            Expr::Ptr iterator;//) {
        Stmt::Ptr body;
    // }
public:
    ForStmt(Stmt::Ptr &initial, Expr::Ptr &iterator, Stmt::Ptr &body)
    : forEach(true), initial(std::move(initial)), condition(nullptr), 
        iterator(std::move(iterator)), body(std::move(body)) {}
    ForStmt(Stmt::Ptr &initial, Expr::Ptr &condition, Expr::Ptr &iterator, Stmt::Ptr &body)
    : forEach(false), initial(std::move(initial)), condition(std::move(condition)),
        iterator(std::move(iterator)), body(std::move(body)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    void debugPrint(size_t indent = 0) override;
};

class EnumStmt : public Stmt {
    string symbol;
    StringVec elements;

public:
    EnumStmt(const string &symbol, StringVec elements = {})
    : symbol(symbol), elements(elements) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    Fux::Type getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override; 
};

class Root : public Stmt {
    Stmt::Vec program;

public:
    typedef shared_ptr<Root> Ptr;
    typedef vector<Ptr> Vec;

    Root() : program(Stmt::Vec()) {}        
    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(Stmt::Ptr &sub);
};