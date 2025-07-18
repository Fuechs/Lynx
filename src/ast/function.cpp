#include "function.h"

#include <sstream>
#include <utility>

/// PROTOTYPE

FunctionPrototype::FunctionPrototype(std::string symbol, FunctionType::Ptr type, std::vector<std::string> parameters)
: symbol(std::move(symbol)), type(std::move(type)), parameters(std::move(parameters)) {}

FunctionPrototype::~FunctionPrototype() {
    symbol.clear();
    parameters.clear();
}

void FunctionPrototype::analyze(Analyzer::Ptr analyzer) {
    analyzer->insert(symbol, std::make_shared<FunctionSymbol>(analyzer, symbol, type, parameters));
}

Type::Ptr FunctionPrototype::getType(std::shared_ptr<Analyzer> analyzer) const { return type; }

wyvern::Entity::Ptr FunctionPrototype::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Arg::Vec gen_args = {};
    const auto &types = type->getParameterTypes();

    for (size_t i = 0; i < parameters.size(); ++i)
        gen_args.push_back(wyvern::Arg::create(types[i]->generate(context), parameters[i]));

    return context->declareFunction(type->getReturnType()->generate(context), symbol, gen_args);
}

std::string FunctionPrototype::str() const {
    std::stringstream ss;

    ss << symbol << "(";

    if (!type) {
        for (const auto &p : parameters)
            ss << p << ", ";

        if (ss.str().ends_with(", "))
            ss.seekp(-2, std::ios_base::end); // remove last comma and space

        ss << ")";
        return ss.str();
    }

    const auto &types = type->getParameterTypes();

    for (size_t i = 0; i < parameters.size(); ++i)
        ss << parameters[i] << ": " << types[i]->str() << ", ";

    if (ss.str().ends_with(", "))
        ss.seekp(-2, std::ios_base::end); // remove last comma and space

    ss << ") -> " << type->getReturnType()->str();

    return ss.str();
}

/// FUNCTION

Function::Function(const std::string &symbol, const FunctionType::Ptr &type, const std::vector<std::string> &parameters, Stmt::Ptr body)
: FunctionPrototype(symbol, type, parameters), body(std::move(body)) {}

void Function::analyze(Analyzer::Ptr analyzer) {
    analyzer->insert(symbol, std::make_shared<FunctionSymbol>(analyzer, symbol, type, parameters));

    if (body)
        body->analyze(analyzer);
}

Type::Ptr Function::getType(std::shared_ptr<Analyzer> analyzer) const { return type; }

wyvern::Entity::Ptr Function::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Arg::Vec gen_args = {};
    const auto &types = type->getParameterTypes();

    for (size_t i = 0; i < parameters.size(); ++i)
        gen_args.push_back(wyvern::Arg::create(types[i]->generate(context), parameters[i]));

    wyvern::Func::Ptr func = context->declareFunction(type->getReturnType()->generate(context), symbol, gen_args, true);
    body->generate(context);
    return func;
}

std::string Function::str() const {
    std::stringstream ss;

    ss << symbol << "(";

    if (!type) {
        for (const auto &p : parameters)
            ss << p << ", ";

        if (ss.str().ends_with(", "))
            ss.seekp(-2, std::ios_base::end); // remove last comma and space

        ss << ")";
        return ss.str();
    }

    const auto &types = type->getParameterTypes();

    for (size_t i = 0; i < parameters.size(); ++i)
        ss << parameters[i] << ": " << types[i]->str() << ", ";

    if (ss.str().ends_with(", "))
        ss.seekp(-2, std::ios_base::end); // remove last comma and space

    ss << ") -> " << type->getReturnType()->str();

    ss << " " << body->str();

    return ss.str();
}