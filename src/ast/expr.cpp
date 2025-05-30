#include <format>
#include <utility>

#include "expr.h"

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
        // TODO: BinaryOp::POW
        default:            return context->getNull();
    }
}

std::string BinaryExpr::str() const {
    return "(" + LHS->str() + " " + BinaryOpValue(op) + " " + RHS->str() + ")";
}

// SYMBOL EXPR

SymbolExpr::SymbolExpr(std::string name) : name(std::move(name)) {}

SymbolExpr::~SymbolExpr() { name.clear(); }

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