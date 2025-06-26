#pragma once

#include <memory>
#include <string>

#include "type.h"
#include "../wyvern/src/wyvern.hpp"

class Value {
public:
    using Ptr = std::shared_ptr<Value>;

    explicit Value(const int64_t &value);
    explicit Value(const double &value);
    explicit Value(std::string value);

    ~Value();

    [[nodiscard]] const Type::Ptr &getType() const;
    [[nodiscard]] wyvern::Val::Ptr generate(const wyvern::Wrapper::Ptr &context) const;

    [[nodiscard]] std::string str() const;

private:
    Type::Ptr type;
    union {
        int64_t i64;
        double f64;
        std::string literal;
    };
};
