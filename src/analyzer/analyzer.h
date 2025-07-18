#pragma once

#include "symbol.h"
#include "../ast/stmt.h"

class Analyzer : public std::enable_shared_from_this<Analyzer> {
public:
    using Ptr = std::shared_ptr<Analyzer>;

    explicit Analyzer(Root::Ptr root);
    ~Analyzer();

    void analyze();

    Symbol::Ptr &lookup(const std::string &name);
    void insert(const std::string &name, Symbol::Ptr symbol);

    constexpr void enterScope() { scopes.emplace_back(); }
    constexpr void leaveScope() { scopes.pop_back(); }

private:
    Root::Ptr root;
    Symbol::Map globals;
    std::vector<Symbol::Map> scopes;
};
