#include <format>
#include <utility>

#include "expr.h"

#include <sstream>

#include "symbol.h"

// ASSIGNMENT EXPR

AssignmentExpr::AssignmentExpr(Ptr assignee, Ptr value)
: assignee(std::move(assignee)), value(std::move(value)) {}

AssignmentExpr::~AssignmentExpr() = default;

void AssignmentExpr::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr AssignmentExpr::getType(Analyzer::Ptr analyzer) const { return assignee->getType(analyzer); }

wyvern::Entity::Ptr AssignmentExpr::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Entity::Ptr L = assignee->generate(context);
    wyvern::Entity::Ptr R = value->generate(context);
    context->storeValue(L, R);
    return L;
}

std::string AssignmentExpr::str() const {
    return std::format("{} = {}", assignee->str(), value->str());
}

// BLOCK EXPR

BlockExpr::BlockExpr(Stmt::Vec stmts) : stmts(std::move(stmts)), yieldsValue(false) {}

BlockExpr::~BlockExpr() { stmts.clear(); }

void BlockExpr::analyze(Analyzer::Ptr analyzer) {
    analyzer->enterScope();

    for (auto &stmt : stmts)
        if (stmt)
            stmt->analyze(analyzer);

    analyzer->leaveScope();
}

Type::Ptr BlockExpr::getType(Analyzer::Ptr analyzer) const {
    // temporary
    if (stmts.back()->kind() == AST::Return)
        return stmts.back()->getType(analyzer);

    return nullptr;
}

wyvern::Entity::Ptr BlockExpr::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Entity::Ptr ret = context->getNull();

    for (const auto &stmt : stmts)
        ret = stmt->generate(context);

    return ret;
}

std::string BlockExpr::str() const {
    std::stringstream ss;
    ss << "{ ";

    if (stmts.size() == 1)
        ss << stmts[0]->str() << "; }";
    else {
        for (const auto &stmt : stmts)
            ss << "\n   " << stmt->str() << ";";
        ss << "\n}";
    }

    return ss.str();
}

// CALL EXPR

CallExpr::CallExpr(Ptr callee, Vec args) : callee(std::move(callee)), args(std::move(args)) {}

CallExpr::~CallExpr() = default;

void CallExpr::analyze(Analyzer::Ptr analyzer) {
    if (!callee)
        return;

    callee->analyze(analyzer);
    // TODO: standard values
    const auto &ftype = std::static_pointer_cast<FunctionType>(callee->getType(analyzer));
    const Type::Vec &params = ftype->getParameterTypes();

    for (size_t i = 0; i < args.size(); ++i) {
        args[i]->analyze(analyzer);

        // if parameters isn't a reference and arg is a pointer, insert dereference op
        if (!params[i]->isReference() && args[i]->getType(analyzer)->isPointer())
            args[i] = std::make_shared<UnaryExpr>(DEREF, args[i]);
    }
}

Type::Ptr CallExpr::getType(Analyzer::Ptr analyzer) const { return callee->getType(analyzer); }

wyvern::Entity::Ptr CallExpr::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Func::Ptr func = std::static_pointer_cast<wyvern::Func>(callee->generate(context));

    wyvern::Entity::Vec generated_args = {};
    for (const auto &arg : args)
        generated_args.push_back(arg->generate(context));

    return func->call(generated_args);
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

void BinaryExpr::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr BinaryExpr::getType(Analyzer::Ptr analyzer) const { return LHS->getType(analyzer); }

wyvern::Entity::Ptr BinaryExpr::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Entity::Ptr L = LHS->generate(context);
    wyvern::Entity::Ptr R = RHS->generate(context);

    switch (op) {
        case ADD: return context->binaryOp(wyvern::ADD, L, R);
        case SUB: return context->binaryOp(wyvern::SUB, L, R);
        case MUL: return context->binaryOp(wyvern::MUL, L, R);
        case DIV: return context->binaryOp(wyvern::DIV, L, R);
        case POW: {
            wyvern::Val::Ptr FL = context->typeCast(L, context->getFloatTy(64));
            wyvern::Val::Ptr FR = context->typeCast(R, context->getFloatTy(64));

            llvm::Function *powFunc = llvm::Intrinsic::getOrInsertDeclaration(context->getModule(), llvm::Intrinsic::pow, context->getFloatTy(64)->getTy());
            llvm::Value *ret = context->getBuilder()->CreateCall(powFunc, {FL->getValuePtr(), FR->getValuePtr()});
            wyvern::Val::Ptr ret_val = wyvern::Val::create(context, context->getFloatTy(64), ret);
            return context->typeCast(ret_val, context->getSignedTy(64));
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

void UnaryExpr::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr UnaryExpr::getType(Analyzer::Ptr analyzer) const { return expr->getType(analyzer); }

wyvern::Entity::Ptr UnaryExpr::generate(wyvern::Wrapper::Ptr context) {
    wyvern::Entity::Ptr gen = expr->generate(context);

    switch (op) {
        case ADDR: {
            // invalid if not a local
            auto local = std::dynamic_pointer_cast<wyvern::Local>(gen);
            return context->createValue(local->getTy(), local->getPtr());
        }

        case DEREF:
            if (auto local = std::dynamic_pointer_cast<wyvern::Local>(gen))
                return local->dereference(false);

            if (auto value = std::dynamic_pointer_cast<wyvern::Val>(gen))
                return value->dereference();

            if (auto arg = std::dynamic_pointer_cast<wyvern::Arg>(gen))
                return arg->dereference();

            std::cerr << "Attempted to dereference some Entity\n";

            return nullptr;

        case POST_INC: {
            // TODO: cater int type of 1 to the value that is added to
            auto value = context->typeCast(context->getInt(64, 1), context->getSignedTy(64));
            auto increment = context->binaryOp(wyvern::ADD, gen, value);
            if (gen->kind() == wyvern::Entity::LOCAL) {
                context->storeValue(gen, increment);
                return gen;
            }

            return increment;
        }

        default:
            return gen;
    }
}

std::string UnaryExpr::str() const {
    if (op == POST_DEC || op == POST_INC)
        return "(" + expr->str() + ")" + std::string(UnaryOpValue[op]);

    return std::string(UnaryOpValue[op]) + "(" + expr->str() + ")";
}


// SYMBOL EXPR

SymbolExpr::SymbolExpr(std::string name) : name(std::move(name)) {}

SymbolExpr::~SymbolExpr() { name.clear(); }

void SymbolExpr::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr SymbolExpr::getType(Analyzer::Ptr analyzer) const {
    return analyzer->lookup(name)->getType();
}

wyvern::Entity::Ptr SymbolExpr::generate(wyvern::Wrapper::Ptr context) {
    if (auto func = context->getFunc(name, false))
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

void ValueExpr::analyze(Analyzer::Ptr analyzer) {}

Type::Ptr ValueExpr::getType(Analyzer::Ptr analyzer) const { return value->getType(); }

wyvern::Entity::Ptr ValueExpr::generate(wyvern::Wrapper::Ptr context) { return value->generate(context); }

std::string ValueExpr::str() const { return value->str(); }