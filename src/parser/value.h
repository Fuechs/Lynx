#pragma once

#include <memory>
#include <string>

#include "type.h"

class Value {
public:
    using Ptr = std::shared_ptr<Value>;

    explicit Value(const int64_t &value);
    explicit Value(const double &value);

    ~Value();

    [[nodiscard]] std::string str() const;

private:
    Type::Ptr type;
    union {
        int64_t i64;
        double f64;
    };
};
