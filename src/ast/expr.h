#pragma once

#include <memory>
#include <vector>

#include "stmt.h"
#include "operation.h"
#include "./parser/value.h"

class Expr : public Stmt {
public:
    using Ptr = std::shared_ptr<Expr>;
    using Vec = std::vector<Ptr>;
};

class BlockExpr : public Expr {
public:
    explicit BlockExpr(Stmt::Vec stmts);
    ~BlockExpr() override;

    Eisdrache::Local &generate(Eisdrache::Ptr context) override;

    [[nodiscard]] std::string str() const override;

private:
    Stmt::Vec stmts;
    bool yieldsValue; // is the block supposed to yield a value (expression)?
};

class BinaryExpr : public Expr {
public:
    using Ptr = std::shared_ptr<BinaryExpr>;

    BinaryExpr(const BinaryOp &op, Expr::Ptr LHS, Expr::Ptr RHS);
    ~BinaryExpr() override;

    Eisdrache::Local &generate(Eisdrache::Ptr context) override;

    [[nodiscard]] std::string str() const override;

private:
    BinaryOp op;
    Expr::Ptr LHS, RHS;
};

class SymbolExpr : public Expr {
public:
    explicit SymbolExpr(std::string name);
    ~SymbolExpr() override;

    [[nodiscard]] std::string str() const override;

private:
    std::string name;
};

class NumberExpr : public Expr {
public:
    explicit NumberExpr(const std::string &value);

    Eisdrache::Local &generate(Eisdrache::Ptr context) override;

    [[nodiscard]] std::string str() const override;

private:
    Value::Ptr value;
};