#pragma once

#include <memory>
#include <vector>

#include "stmt.h"
#include "../lexer/token.h"
#include "operation.h"

using Value = double; // temporary

class Expr : public StmtAST {
public:
    using Ptr = std::shared_ptr<Expr>;
    using Vec = std::vector<Ptr>;
};

class BinaryExpr : public Expr {
public:
    using Ptr = std::shared_ptr<BinaryExpr>;

    BinaryExpr(const BinaryOp &op, const Expr::Ptr &LHS, const Expr::Ptr &RHS);
    ~BinaryExpr() override;

    std::string str() const override;

private:
    BinaryOp op;
    Expr::Ptr LHS, RHS;
};

class NumberExpr : public Expr {
public:
    explicit NumberExpr(const std::string &value);

    std::string str() const override;

private:
    Value value;
};