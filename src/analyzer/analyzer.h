#pragma once

#include "../ast/stmt.h"

class Analyzer : public std::enable_shared_from_this<Analyzer> {
public:
    using Ptr = std::shared_ptr<Analyzer>;

    explicit Analyzer(Root::Ptr root);
    ~Analyzer();

    void analyze();

private:
    Root::Ptr root;
};
