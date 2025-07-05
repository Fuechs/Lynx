#include "analyzer.h"

Analyzer::Analyzer(Root::Ptr root) : root(std::move(root)) {}

Analyzer::~Analyzer() = default;

void Analyzer::analyze() { root->analyze(shared_from_this()); }