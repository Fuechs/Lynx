#include "analyzer.h"

#include <ranges>

Analyzer::Analyzer(Root::Ptr root) : root(std::move(root)) {}

Analyzer::~Analyzer() {
    globals.clear();
    scopes.clear();
}

void Analyzer::analyze() {
    root->analyze(shared_from_this());

    for (const auto &[name, symbol] : globals)
        std::cout << name << ": " << symbol->str() << '\n';
}

Symbol::Ptr &Analyzer::lookup(const std::string &name) {
    for (Symbol::Map &scope : scopes | std::views::reverse)
        if (scope.contains(name))
            return scope[name];

    if (globals.contains(name))
        return globals[name];

    throw std::invalid_argument("Symbol not found: $" + name);
}

void Analyzer::insert(const std::string &name, Symbol::Ptr symbol) {
    std::cout << "Inserting symbol: " << symbol->str() << '\n';

    if (scopes.empty())
        globals[name] = std::move(symbol);
    else
        scopes.back()[name] = std::move(symbol);
}