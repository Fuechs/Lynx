#include <sstream>
#include "stmt.h"

StmtAST::~StmtAST() {}

RootAST::RootAST() : program({}) {}

RootAST::~RootAST() { program.clear(); }

void RootAST::addStmt(StmtAST::Ptr stmt) {
    program.push_back(std::move(stmt));
}

std::string RootAST::str() const {
    std::stringstream ss;

    for (auto &stmt : program)
        ss << stmt->str() << '\n';

    return ss.str();
}