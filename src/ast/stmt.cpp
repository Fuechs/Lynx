#include <sstream>
#include "stmt.h"
#include "expr.h"

// STMT

Stmt::~Stmt() = default;

// ROOT

Root::Root() : program({}) {}

Root::~Root() { program.clear(); }

void Root::addStmt(Stmt::Ptr stmt) {
    program.push_back(std::move(stmt));
}

wyvern::Entity::Ptr Root::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Entity::Ptr ret = nullptr;

    for (const auto &stmt : program)
        if (!stmt)
            std::cerr << "Encountered null stmt during generation." << std::endl;
        else
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

wyvern::Entity::Ptr ReturnStmt::generate(wyvern::Wrapper::Ptr context) {
    return wyvern::Val::create(context, context->createRet(value->generate(context)));
}

std::string ReturnStmt::str() const { return "ret " + value->str(); }