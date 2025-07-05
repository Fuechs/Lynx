#pragma once

#include <memory>
#include <vector>
#include "../wyvern/src/wyvern.hpp"
#include "../parser/type.h"

class Analyzer;
class Expr;

enum class AST {
    Stmt,
    Root,
    FunctionPrototype,
    Function,
    Variable,
    Return,
    Expr,
    Assignment,
    Block,
    Call,
    Binary,
    Unary,
    Symbol,
    Number,
    Literal,
};

class Stmt {
public:
    using Ptr = std::shared_ptr<Stmt>;
    using Vec = std::vector<Ptr>;

    virtual ~Stmt();

    virtual void analyze(std::shared_ptr<Analyzer> analyzer) = 0;
    virtual Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const = 0;
    virtual wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) = 0;

    [[nodiscard]] virtual constexpr AST kind() const { return AST::Stmt; }
    [[nodiscard]] virtual std::string str() const = 0;

    constexpr bool isExpr() const { return kind() >= AST::Expr; }
};

class Root : public Stmt {
public:
    using Ptr = std::shared_ptr<Root>;

    Root();
    ~Root() override;

    void addStmt(Stmt::Ptr stmt);

    void analyze(std::shared_ptr<Analyzer> analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Root; }
    [[nodiscard]] std::string str() const override;

private:
    Vec program;
};

class VariableStmt : public Stmt {
public:
    explicit VariableStmt(std::string symbol, Type::Ptr type = nullptr, std::shared_ptr<Expr> value = nullptr);
    ~VariableStmt() override;

    void analyze(std::shared_ptr<Analyzer> analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Variable; }
    [[nodiscard]] std::string str() const override;

private:
    std::string symbol;
    Type::Ptr type;
    std::shared_ptr<Expr> value;
};

class ReturnStmt : public Stmt {
public:
    explicit ReturnStmt(std::shared_ptr<Expr> value);
    ~ReturnStmt() override;

    void analyze(std::shared_ptr<Analyzer> analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Return; }
    [[nodiscard]] std::string str() const override;

private:
    std::shared_ptr<Expr> value;
};