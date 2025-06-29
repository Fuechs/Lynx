#include "function.h"

#include <utility>

/// PARAMETER...

/// PROTOTYPE

FunctionPrototype::FunctionPrototype(std::string symbol, Type::Ptr type, FunctionParameter::Vec parameters)
: symbol(std::move(symbol)), type(std::move(type)), parameters(std::move(parameters)) {}

FunctionPrototype::~FunctionPrototype() {
    symbol.clear();
    parameters.clear();
}

wyvern::Entity::Ptr FunctionPrototype::generate(wyvern::Wrapper::Ptr context) {
    // TODO: generate parameters
    wyvern::Arg::Vec parameters = {};
    return context->declareFunction(type->generate(context), symbol, parameters);
}

std::string FunctionPrototype::str() const { return symbol + "()" + type->str(); }

/// FUNCTION

Function::Function(const std::string &symbol, const Type::Ptr &type, const FunctionParameter::Vec &parameters, Stmt::Ptr body)
: FunctionPrototype(symbol, type, parameters), body(std::move(body)) {}

wyvern::Entity::Ptr Function::generate(wyvern::Wrapper::Ptr context) {
    // TODO: generate parameters
    wyvern::Arg::Vec parameters = {};
    wyvern::Func::Ptr func = context->declareFunction(type->generate(context), symbol, parameters, true);
    body->generate(context);
    return func;
}

std::string Function::str() const { return symbol + "() " + type->str() + " " + body->str(); }