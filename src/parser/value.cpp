#include "value.h"

Value::Value(const int64_t &value) :    type(std::make_shared<IntType>(Type::I64)), i64(value) {}
Value::Value(const double &value) :     type(std::make_shared<FloatType>(Type::F64)), f64(value) {}

Value::~Value() {}

std::string Value::str() const {
    switch (type->getKind()) {
        case Type::I64: return std::to_string(i64);
        case Type::F64: return std::to_string(f64);
        default:        return "INVALID_VALUE";
    }
}