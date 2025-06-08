#pragma once

#include <memory>
#include <vector>
#include "../eisdrache/src/eisdrache.hpp"

using llvm::Eisdrache;

enum class AST {
    Stmt,
    Root,
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