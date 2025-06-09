#include "value.h"

Value::Value(const int64_t &value) :    type(std::make_shared<IntType>(Type::I64)), i64(value) {}
Value::Value(const double &value) :     type(std::make_shared<FloatType>(Type::F64)), f64(value) {}

Value::~Value() {}

const Type::Ptr &Value::getType() const { return type; }

llvm::Eisdrache::Local &Value::generate(llvm::Eisdrache::Ptr context) const {
    using llvm::Eisdrache;

    Eisdrache::Local local;

    switch (type->getKind()) {
        case Type::I64: local = Eisdrache::Local(context, context->getSignedTy(64), context->getInt(64, i64)); break;
        case Type::F64: local = Eisdrache::Local(context, context->getFloatTy(64), context->getFloat(f64)); break;
        default:        local = Eisdrache::Local(context);
    }

    return context->getCurrentParent().addLocal(local);
}

std::string Value::str() const {
    switch (type->getKind()) {
        case Type::I64: return std::to_string(i64);
        case Type::F64: return std::to_string(f64);
        default:        return "INVALID_VALUE";
    }
}