#include <format>

#include "expr.h"

#include <format>

BinaryExpr::BinaryExpr(const BinaryOp &op, const Expr::Ptr &LHS, const Expr::Ptr &RHS)
: op(op), LHS(LHS), RHS(RHS) {}

BinaryExpr::~BinaryExpr() {}

std::string BinaryExpr::str() const {
    return "(" + LHS->str() + " " + BinaryOpValue(op) + " " + RHS->str() + ")";
}

NumberExpr::NumberExpr(const std::string &value) : value(std::stod(value)) {}

std::string NumberExpr::str() const {
    std::string s = std::to_string(value);
    if (s.find('.') != std::string::npos) {
        s = s.substr(0, s.find_last_not_of('0') + 1);
        if (s.back() == '.')
            s.pop_back();
    }
    return s;
}