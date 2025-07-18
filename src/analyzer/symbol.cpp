#include "symbol.h"

#include <ranges>
#include <utility>

#include "analyzer.h"

// SYMBOL

Symbol::Symbol(Analyzer::Ptr analyzer, std::string name, Type::Ptr type)
: analyzer(std::move(analyzer)), name(std::move(name)), type(std::move(type)) {}

Symbol::~Symbol() { name.clear(); }

std::string Symbol::str() const { return "$"+name + " " + type->str(); }

// FUNCTION SYMBOL

FunctionSymbol::FunctionSymbol(const Analyzer::Ptr &analyzer, const std::string &name,
    const FunctionType::Ptr &type, const std::vector<std::string> &parameterNames)
: Symbol(analyzer, name, type), parameterNames(parameterNames) {}

FunctionSymbol::~FunctionSymbol() {
    name.clear();
    parameterNames.clear();
}

const Type::Vec &FunctionSymbol::getParameterTypes() const {
    return std::static_pointer_cast<FunctionType>(type)->getParameterTypes();
}
