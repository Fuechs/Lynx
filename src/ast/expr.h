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

class AssignmentExpr : public Expr {
public:
    explicit AssignmentExpr(Ptr assignee, Ptr value);
    ~AssignmentExpr() override;

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Assignment; }
    [[nodiscard]] std::string str() const override;

private:
    Ptr assignee, value;
};

class BlockExpr : public Expr {
public:
    explicit BlockExpr(Stmt::Vec stmts);
    ~BlockExpr() override;

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Block; }
    [[nodiscard]] std::string str() const override;

private:
    Stmt::Vec stmts;
    bool yieldsValue; // is the block supposed to yield a value (expression)?
};

class CallExpr : public Expr {
public:
    CallExpr(Ptr callee, Vec args);
    ~CallExpr() override;

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Call; }
    [[nodiscard]] std::string str() const override;

private:
    Ptr callee;
    Vec args;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(const BinaryOp &op, Ptr LHS, Ptr RHS);
    ~BinaryExpr() override;

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Binary; }
    [[nodiscard]] std::string str() const override;

private:
    BinaryOp op;
    Ptr LHS, RHS;
};

class UnaryExpr : public Expr {
public:
    UnaryExpr(const UnaryOp &op, Ptr expr);
    ~UnaryExpr() override;

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Unary; }
    [[nodiscard]] std::string str() const override;

private:
    UnaryOp op;
    Ptr expr;
};

class SymbolExpr : public Expr {
public:
    explicit SymbolExpr(std::string name);
    ~SymbolExpr() override;

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Symbol; }
    [[nodiscard]] std::string str() const override;

private:
    std::string name;
};

class ValueExpr : public Expr {
public:
    explicit ValueExpr(const Token &token);

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Number; }
    [[nodiscard]] std::string str() const override;

private:
    Value::Ptr value;
};