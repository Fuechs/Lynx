#include <format>
#include <utility>

#include "expr.h"

#include <sstream>

// ASSIGNMENT EXPR

AssignmentExpr::AssignmentExpr(Ptr assignee, Ptr value)
: assignee(std::move(assignee)), value(std::move(value)) {}

AssignmentExpr::~AssignmentExpr() = default;

Eisdrache::Local &AssignmentExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Local &L = assignee->generate(context);
    Eisdrache::Local &R = value->generate(context);
    context->storeValue(L, R);
    return L;
}

std::string AssignmentExpr::str() const {
    return std::format("{} = {}", assignee->str(), value->str());
}

// BLOCK EXPR

BlockExpr::BlockExpr(Stmt::Vec stmts) : stmts(std::move(stmts)), yieldsValue(false) {}

BlockExpr::~BlockExpr() { stmts.clear(); }

Eisdrache::Local &BlockExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Local *ret = &context->getNull();

    for (const auto &stmt : stmts)
        ret = &stmt->generate(context);

    return *ret;
}

std::string BlockExpr::str() const {
    std::stringstream ss;
    ss << "{ ";

    if (stmts.size() == 1)
        ss << stmts[0]->str() << "; }";
    else {
        for (const auto &stmt : stmts)
            ss << "\n" << stmt->str() << ";";
        ss << "\n}";
    }

    return ss.str();
}


// BINARY EXPR

BinaryExpr::BinaryExpr(const BinaryOp &op, Expr::Ptr LHS, Expr::Ptr RHS)
: op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

BinaryExpr::~BinaryExpr() = default;

Eisdrache::Local &BinaryExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Local &L = LHS->generate(context);
    Eisdrache::Local &R = RHS->generate(context);

    switch (op) {
        case BinaryOp::ADD: return context->binaryOp(Eisdrache::ADD, L, R);
        case BinaryOp::SUB: return context->binaryOp(Eisdrache::SUB, L, R);
        case BinaryOp::MUL: return context->binaryOp(Eisdrache::MUL, L, R);
        case BinaryOp::DIV: return context->binaryOp(Eisdrache::DIV, L, R);
        case BinaryOp::POW: {
            Eisdrache::Local &FL = context->typeCast(L, context->getFloatTy(64));
            Eisdrache::Local &FR = context->typeCast(R, context->getFloatTy(64));

            llvm::Function *powFunc = llvm::Intrinsic::getOrInsertDeclaration(context->getModule(), llvm::Intrinsic::pow, context->getFloatTy(64)->getTy());
            llvm::Value *ret = context->getBuilder()->CreateCall(powFunc, {FL.getValuePtr(), FR.getValuePtr()});
            Eisdrache::Local &ret_local = context->getCurrentParent().addLocal(Eisdrache::Local(context, context->getFloatTy(64), ret));
            return context->typeCast(ret_local, context->getSignedTy(64));
        }
        default:            return context->getNull();
    }
}

std::string BinaryExpr::str() const {
    return "(" + LHS->str() + " " + BinaryOpValue(op) + " " + RHS->str() + ")";
}

// SYMBOL EXPR

SymbolExpr::SymbolExpr(std::string name) : name(std::move(name)) {}

SymbolExpr::~SymbolExpr() { name.clear(); }

Eisdrache::Local &SymbolExpr::generate(Eisdrache::Ptr context) {
    return context->getCurrentParent()[name];
}

std::string SymbolExpr::str() const { return name; }

// NUMBER EXPR

NumberExpr::NumberExpr(const std::string &value) {
    if (value.find('.') != std::string::npos)
        this->value = std::make_shared<Value>(std::stod(value));
    else
        this->value = std::make_shared<Value>(std::stoll(value));
}

Eisdrache::Local &NumberExpr::generate(Eisdrache::Ptr context) { return value->getLLVMValue(context); }

std::string NumberExpr::str() const { return value->str(); }