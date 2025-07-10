#include "function.h"

#include <sstream>
#include <utility>

/// PARAMETER

FunctionParameter::FunctionParameter(Type::Ptr type, std::string symbol)
: type(std::move(type)), symbol(std::move(symbol)) {}

FunctionParameter::~FunctionParameter() { symbol.clear(); }

void FunctionParameter::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr FunctionParameter::getType() const { return type; }

wyvern::Arg::Ptr FunctionParameter::generate(const wyvern::Wrapper::Ptr &context) const {
    return wyvern::Arg::create(type->generate(context), symbol);
}

std::string FunctionParameter::str() const {
    if (symbol.empty())
        return type->str();

    return symbol + ": " + type->str();
}

/// PROTOTYPE

FunctionPrototype::FunctionPrototype(std::string symbol, Type::Ptr type, FunctionParameter::Vec parameters)
: symbol(std::move(symbol)), type(std::move(type)), parameters(std::move(parameters)) {}

FunctionPrototype::~FunctionPrototype() {
    symbol.clear();
    parameters.clear();
}

void FunctionPrototype::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr FunctionPrototype::getType(std::shared_ptr<Analyzer> analyzer) const { return type; }

wyvern::Entity::Ptr FunctionPrototype::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Arg::Vec parameters = {};

    for (const auto &param : this->parameters)
        parameters.push_back(param.generate(context));

    return context->declareFunction(type->generate(context), symbol, parameters);
}

std::string FunctionPrototype::str() const {
    std::stringstream ss;

    ss << symbol << "(";

    for (const auto &param : parameters)
        ss << param.str();

    ss << ")";

    if (type)
        ss << " -> " << type->str();

    return ss.str();
}

/// FUNCTION

Function::Function(const std::string &symbol, const Type::Ptr &type, const FunctionParameter::Vec &parameters, Stmt::Ptr body)
: FunctionPrototype(symbol, type, parameters), body(std::move(body)) {}

void Function::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr Function::getType(std::shared_ptr<Analyzer> analyzer) const { return type; }

wyvern::Entity::Ptr Function::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Arg::Vec parameters = {};

    for (const auto &param : this->parameters)
        parameters.push_back(param.generate(context));

    wyvern::Func::Ptr func = context->declareFunction(type->generate(context), symbol, parameters, true);
    body->generate(context);
    return func;
}

std::string Function::str() const {
    std::stringstream ss;

    ss << symbol << "(";

    for (const auto &param : parameters)
        ss << param.str();

    ss << ")";

    if (type)
        ss << " -> " << type->str();

    ss << " " << body->str();

    return ss.str();
}