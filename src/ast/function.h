#pragma once

#include "stmt.h"
#include "../analyzer/analyzer.h"

class FunctionParameter {
public:
    using Vec = std::vector<FunctionParameter>;

    explicit FunctionParameter(Type::Ptr type = nullptr, std::string symbol = "");
    ~FunctionParameter();

    void analyze(Analyzer::Ptr analyzer);
    Type::Ptr getType() const;
    [[nodiscard]] wyvern::Arg::Ptr generate(const wyvern::Wrapper::Ptr &context) const;

    [[nodiscard]] std::string str() const;

private:
    Type::Ptr type;
    std::string symbol;
};

class FunctionPrototype : public Stmt {
public:
    FunctionPrototype(std::string symbol, Type::Ptr type, FunctionParameter::Vec parameters = {});
    ~FunctionPrototype() override;

    void analyze(Analyzer::Ptr analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::FunctionPrototype; }
    [[nodiscard]] std::string str() const override;

protected:
    std::string symbol;
    Type::Ptr type;
    FunctionParameter::Vec parameters;
};

class Function : public FunctionPrototype {
public:
    Function(const std::string &symbol, const Type::Ptr &type, const FunctionParameter::Vec &parameters, Stmt::Ptr body);

    void analyze(Analyzer::Ptr analyzer) override;
    Type::Ptr getType(std::shared_ptr<Analyzer> analyzer) const override;
    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Function; }
    [[nodiscard]] std::string str() const override;

private:
    Stmt::Ptr body;
};
