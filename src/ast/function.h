#pragma once

#include "stmt.h"
#include "../analyzer/analyzer.h"

class FunctionPrototype : public Stmt {
public:
    FunctionPrototype(std::string symbol, FunctionType::Ptr type, std::vector<std::string> parameters = {});
    ~FunctionPrototype() override;

    void analyze(Analyzer::Ptr analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::FunctionPrototype; }
    [[nodiscard]] std::string str() const override;

protected:
    std::string symbol;
    FunctionType::Ptr type;
    std::vector<std::string> parameters;
};

class Function : public FunctionPrototype {
public:
    Function(const std::string &symbol, const FunctionType::Ptr &type, const std::vector<std::string> &parameters, Stmt::Ptr body);

    void analyze(Analyzer::Ptr analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Function; }
    [[nodiscard]] std::string str() const override;

private:
    Stmt::Ptr body;
};
