#include <format>
#include <utility>

#include "expr.h"

#include <sstream>

// ASSIGNMENT EXPR

AssignmentExpr::AssignmentExpr(Ptr assignee, Ptr value)
: assignee(std::move(assignee)), value(std::move(value)) {}

AssignmentExpr::~AssignmentExpr() = default;

Eisdrache::Entity::Ptr AssignmentExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Local::Ptr L = std::static_pointer_cast<Eisdrache::Local>(assignee->generate(context));
    Eisdrache::Local::Ptr R = std::static_pointer_cast<Eisdrache::Local>(value->generate(context));
    context->storeValue(L, R);
    return L;
}

std::string AssignmentExpr::str() const {
    return std::format("{} = {}", assignee->str(), value->str());
}

// BLOCK EXPR

BlockExpr::BlockExpr(Stmt::Vec stmts) : stmts(std::move(stmts)), yieldsValue(false) {}

BlockExpr::~BlockExpr() { stmts.clear(); }

Eisdrache::Entity::Ptr BlockExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Local::Ptr ret = context->getNull();

    for (const auto &stmt : stmts)
        ret = std::static_pointer_cast<Eisdrache::Local>(stmt->generate(context));

    return ret;
}

std::string BlockExpr::str() const {
    std::stringstream ss;
    ss << "{ ";

    if (stmts.size() == 1)
        ss << stmts[0]->str() << "; }";
    else {
        for (const auto &stmt : stmts)
            ss << "\n" << stmt->str() << ";";
        ss << "\n}";
    }

    return ss.str();
}

// CALL EXPR

CallExpr::CallExpr(Ptr callee, Vec args) : callee(std::move(callee)), args(std::move(args)) {}

CallExpr::~CallExpr() = default;

Eisdrache::Entity::Ptr CallExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Func::Ptr func = std::static_pointer_cast<Eisdrache::Func>(callee->generate(context));

    Eisdrache::ValueVec args_value = {};
    for (const auto &arg : args) {
        Eisdrache::Local::Ptr arg_local = std::static_pointer_cast<Eisdrache::Local>(arg->generate(context));
        args_value.push_back(arg_local->getValuePtr());
    }

    return func->call(args_value);
}

std::string CallExpr::str() const {
    std::stringstream ss;
    ss << callee->str() << "(";

    for (const auto &arg : args) {
        ss << arg->str();
        if (arg != args.back())
            ss << ", ";
    }

    ss << ")";
    return ss.str();
}

// BINARY EXPR

BinaryExpr::BinaryExpr(const BinaryOp &op, Ptr LHS, Ptr RHS)
: op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

BinaryExpr::~BinaryExpr() = default;

Eisdrache::Entity::Ptr BinaryExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Local::Ptr L = std::static_pointer_cast<Eisdrache::Local>(LHS->generate(context));
    Eisdrache::Local::Ptr R = std::static_pointer_cast<Eisdrache::Local>(RHS->generate(context));


    switch (op) {
        case ADD: return context->binaryOp(Eisdrache::ADD, L, R);
        case SUB: return context->binaryOp(Eisdrache::SUB, L, R);
        case MUL: return context->binaryOp(Eisdrache::MUL, L, R);
        case DIV: return context->binaryOp(Eisdrache::DIV, L, R);
        case POW: {
            Eisdrache::Local::Ptr FL = context->typeCast(L, context->getFloatTy(64));
            Eisdrache::Local::Ptr FR = context->typeCast(R, context->getFloatTy(64));

            llvm::Function *powFunc = llvm::Intrinsic::getOrInsertDeclaration(context->getModule(), llvm::Intrinsic::pow, context->getFloatTy(64)->getTy());
            llvm::Value *ret = context->getBuilder()->CreateCall(powFunc, {FL->getValuePtr(), FR->getValuePtr()});
            Eisdrache::Local::Ptr ret_local = context->getCurrentParent()->addLocal(std::make_shared<Eisdrache::Local>(context, context->getFloatTy(64), ret));
            return context->typeCast(ret_local, context->getSignedTy(64));
        }
        default:            return context->getNull();
    }
}

std::string BinaryExpr::str() const {
    return "(" + LHS->str() + " " + BinaryOpValue[op]  + " " + RHS->str() + ")";
}

// UNARY EXPR

UnaryExpr::UnaryExpr(const UnaryOp &op, Ptr expr) : op(op), expr(std::move(expr)) {}

UnaryExpr::~UnaryExpr() = default;

Eisdrache::Entity::Ptr UnaryExpr::generate(Eisdrache::Ptr context) {
    Eisdrache::Entity::Ptr gen = expr->generate(context);

    switch (op) {
        case DEREF: return std::static_pointer_cast<Eisdrache::Local>(gen)->dereference();
        default:    return gen;
    }
}

std::string UnaryExpr::str() const {
    return std::string(UnaryOpValue[op]) + "(" + expr->str() + ")";
}


// SYMBOL EXPR

SymbolExpr::SymbolExpr(std::string name) : name(std::move(name)) {}

SymbolExpr::~SymbolExpr() { name.clear(); }

Eisdrache::Entity::Ptr SymbolExpr::generate(Eisdrache::Ptr context) {
    if (auto func = context->getFunc(name))
        return func;

    return (*context->getCurrentParent())[name];
}

std::string SymbolExpr::str() const { return name; }

// VALUE EXPR

ValueExpr::ValueExpr(const Token &token) {
    switch (token.getType()) {
        case NUMBER:
            if (token.getValue().find('.') != std::string::npos)
                value = std::make_shared<Value>(std::stod(token.getValue()));
            else
                value = std::make_shared<Value>(std::stoll(token.getValue()));
            break;

        case LITERAL:
            value = std::make_shared<Value>(token.getValue());
            break;

        default:
            std::cerr << "Unhandled token type in ValueExpr\n";
            value = nullptr;
    }
}

Eisdrache::Entity::Ptr ValueExpr::generate(Eisdrache::Ptr context) { return value->generate(context); }

std::string ValueExpr::str() const { return value->str(); }