#pragma once

#include <memory>
#include <vector>
#include "../eisdrache/src/eisdrache.hpp"
#include "../parser/type.h"

using llvm::Eisdrache;
class Expr;

enum class AST {
    Stmt,
    Root,
    Variable,
    Expr,
    Assignment,
    Block,
    Binary,
    Symbol,
    Number,
};

class Stmt {
public:
    using Ptr = std::shared_ptr<Stmt>;
    using Vec = std::vector<Ptr>;

    virtual ~Stmt();

    virtual Eisdrache::Local &generate(Eisdrache::Ptr context) = 0;

    [[nodiscard]] virtual constexpr AST kind() const { return AST::Stmt; }
    [[nodiscard]] virtual std::string str() const = 0;
};

class Root : public Stmt {
public:
    using Ptr = std::shared_ptr<Root>;

    Root();
    ~Root() override;

    void addStmt(Stmt::Ptr stmt);

    Eisdrache::Local &generate(Eisdrache::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Root; }
    [[nodiscard]] std::string str() const override;

private:
    Vec program;
};

class VariableStmt : public Stmt {
public:
    explicit VariableStmt(std::string symbol, Type::Ptr type = nullptr, std::shared_ptr<Expr> value = nullptr);
    ~VariableStmt() override;

    Eisdrache::Local &generate(Eisdrache::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Variable; }
    [[nodiscard]] std::string str() const override;

private:
    std::string symbol;
    Type::Ptr type;
    std::shared_ptr<Expr> value;
};