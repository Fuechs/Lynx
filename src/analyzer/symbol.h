#pragma once

#include <map>
#include <memory>

#include "../parser/type.h"

class Analyzer;

class Symbol {
public:
    using Ptr = std::shared_ptr<Symbol>;
    using Map = std::map<std::string, Ptr>;

    Symbol(std::shared_ptr<Analyzer> analyzer, std::string name, Type::Ptr type);
    virtual ~Symbol();

    [[nodiscard]] const Type::Ptr &getType() const { return type; }

    [[nodiscard]] virtual std::string str() const;

    [[nodiscard]] virtual constexpr bool isFunction() const { return false; }

protected:
    std::shared_ptr<Analyzer> analyzer;
    std::string name;
    Type::Ptr type;
};

class FunctionSymbol : public Symbol {
public:
    FunctionSymbol(const std::shared_ptr<Analyzer> &analyzer, const std::string &name,
        const FunctionType::Ptr &type, const std::vector<std::string> &parameterNames);
    ~FunctionSymbol() override;

    [[nodiscard]] const Type::Vec &getParameterTypes() const;

    [[nodiscard]] constexpr bool isFunction() const override { return true; }

private:
    std::vector<std::string> parameterNames;
};