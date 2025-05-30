#include <sstream>
#include "stmt.h"

Stmt::~Stmt() = default;

Root::Root() : program({}) {}

Root::~Root() { program.clear(); }

void Root::addStmt(Stmt::Ptr stmt) {
    program.push_back(std::move(stmt));
}

Eisdrache::Local &Root::generate(llvm::Eisdrache::Ptr context) {
    Eisdrache::Local *ret = nullptr;

    for (const auto &stmt : program)
        ret = &stmt->generate(context);

    return *ret; // might be temporary, added this for basic testing
}

std::string Root::str() const {
    std::stringstream ss;

    for (const auto &stmt : program)
        ss << stmt->str() << '\n';

    return ss.str();
}