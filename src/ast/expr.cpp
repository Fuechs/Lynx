#include <format>
#include <utility>

#include "expr.h"

// BINARY EXPR

BinaryExpr::BinaryExpr(const BinaryOp &op, Expr::Ptr LHS, Expr::Ptr RHS)
: op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

BinaryExpr::~BinaryExpr() = default;

std::string BinaryExpr::str() const {
    return "(" + LHS->str() + " " + BinaryOpValue(op) + " " + RHS->str() + ")";
}

// SYMBOL EXPR

SymbolExpr::SymbolExpr(std::string name) : name(std::move(name)) {}

SymbolExpr::~SymbolExpr() { name.clear(); }

std::string SymbolExpr::str() const { return name; }

// NUMBER EXPR

NumberExpr::NumberExpr(const std::string &value) {
    if (value.contains("."))
        this->value = std::make_shared<Value>(std::stod(value));
    else
        this->value = std::make_shared<Value>(std::stoll(value));
}

std::string NumberExpr::str() const { return value->str(); }