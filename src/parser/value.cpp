#include "value.h"

#include <utility>
#include "../util/io.h"

Value::Value(const int32_t &value) :    type(std::make_shared<IntType>(Type::I32)), i32(value) {}
Value::Value(const int64_t &value) :    type(std::make_shared<IntType>(Type::I64)), i64(value) {}
Value::Value(const double &value) :     type(std::make_shared<FloatType>(Type::F64)), f64(value) {}
Value::Value(std::string value) :       type(std::make_shared<Type>(Type::LITERAL)), literal(std::move(value)) {}

Value::~Value() {}

const Type::Ptr &Value::getType() const { return type; }

wyvern::Val::Ptr Value::generate(const wyvern::Wrapper::Ptr &context) const {
    switch (type->getKind()) {
        // TODO: need a proper function to get signed values
        case Type::I32:     return wyvern::Val::create(context, context->getSignedTy(32), context->getBuilder()->getInt32(i32));
        case Type::I64:     return wyvern::Val::create(context, context->getSignedTy(64), context->getBuilder()->getInt64(i64));
        case Type::F64:     return context->getFloat(64);
        case Type::LITERAL: return context->getLiteral(escapeSequences(literal));
        default:            return context->getNull();
    }
}

std::string Value::str() const {
    switch (type->getKind()) {
        case Type::I32:     return std::to_string(i32);
        case Type::I64:     return std::to_string(i64);
        case Type::F64:     return std::to_string(f64);
        case Type::LITERAL: return '"'+literal+'"';
        default:            return "INVALID_VALUE";
    }
}