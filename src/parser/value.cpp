#include "value.h"

Value::Value(const int64_t &value) :    type(ValueType::I64), i64(value) {}
Value::Value(const double &value) :     type(ValueType::F64), f64(value) {}

Value::~Value() {}

std::string Value::str() const {
    switch (type) {
        case ValueType::I64: return std::to_string(i64);
        case ValueType::F64: return std::to_string(f64);
        default: return "INVALID_VALUE";
    }
}