#include <sstream>
#include "stmt.h"
#include "expr.h"
#include "../analyzer/analyzer.h"

// STMT

Stmt::~Stmt() = default;

// ROOT

Root::Root() : program({}) {}

Root::~Root() { program.clear(); }

void Root::addStmt(Stmt::Ptr stmt) {
    program.push_back(std::move(stmt));
}

void Root::analyze(Analyzer::Ptr analyzer) {
    auto it = program.begin();
    while (it != program.end()) {
        if (*it) (*it)->analyze(analyzer);
        else {
            std::cerr << "Encountered null stmt during analysis." << std::endl;
            program.erase(it);
        }

        ++it;
    }
}

Type::Ptr Root::getType(Analyzer::Ptr) const { return nullptr; }

wyvern::Entity::Ptr Root::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Entity::Ptr ret = nullptr;

    for (const auto &stmt : program)
        ret = stmt->generate(context);

    return ret; // might be temporary, added this for basic testing
}

std::string Root::str() const {
    std::stringstream ss;

    for (const auto &stmt : program)
        if (!stmt)
            ss << "NULL_AST;\n";
        else
            ss << stmt->str() << ";\n";

    return ss.str();
}

// VARIABLE STMT

VariableStmt::VariableStmt(std::string symbol, Type::Ptr type, std::shared_ptr<Expr> value)
: symbol(std::move(symbol)), type(std::move(type)), value(std::move(value)) {}

VariableStmt::~VariableStmt() { symbol.clear(); }

void VariableStmt::analyze(Analyzer::Ptr analyzer) {
    if (type) type->analyze(analyzer);
    if (value) value->analyze(analyzer);

    if (*value->getType(analyzer) != *type) {
        // insert type cast
    }

    analyzer->insert(symbol, std::make_shared<Symbol>(analyzer, symbol, type));
}

Type::Ptr VariableStmt::getType(Analyzer::Ptr analyzer) const { return type; }

wyvern::Entity::Ptr VariableStmt::generate(wyvern::Wrapper::Ptr context) {
    auto val = value ? value->generate(context) : nullptr;
    return context->declareLocal(type->generate(context), symbol, val);
}

std::string VariableStmt::str() const {
    std::stringstream ss;
    ss << symbol;

    if (type) {
        if (!type->isReference())
            ss << ": ";

        ss << type->str();
    }

    if (value)
        ss << (type ? " = " : " := ") << value->str();

    return ss.str();
}

// RETURN STMT

ReturnStmt::ReturnStmt(Expr::Ptr value) : value(std::move(value)) {}

ReturnStmt::~ReturnStmt() = default;

void ReturnStmt::analyze(Analyzer::Ptr analyzer) {
    if (value) value->analyze(analyzer);
}

Type::Ptr ReturnStmt::getType(Analyzer::Ptr analyzer) const { return value->getType(analyzer); }

wyvern::Entity::Ptr ReturnStmt::generate(wyvern::Wrapper::Ptr context) {
    if (!value)
        return wyvern::Val::create(context, context->createRetVoid());

    return wyvern::Val::create(context, context->createRet(value->generate(context)));
}

std::string ReturnStmt::str() const {
    return "ret" + (value ? " " + value->str() : "");
}