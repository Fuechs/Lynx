#pragma once

#include "stmt.h"

class FunctionParameter {
public:
    using Vec = std::vector<FunctionParameter>;
};

class FunctionPrototype : public Stmt {
public:
    FunctionPrototype(std::string symbol, Type::Ptr type, FunctionParameter::Vec parameters = {});
    ~FunctionPrototype() override;

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

    wyvern::Entity::Ptr generate(wyvern::Wrapper::Ptr context) override;

    [[nodiscard]] constexpr AST kind() const override { return AST::Function; }
    [[nodiscard]] std::string str() const override;
private:
    Stmt::Ptr body;
};
