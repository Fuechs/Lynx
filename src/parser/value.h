#pragma once

#include <memory>
#include <string>

#include "type.h"
#include "../eisdrache/src/eisdrache.hpp"

class Value {
public:
    using Ptr = std::shared_ptr<Value>;

    explicit Value(const int64_t &value);
    explicit Value(const double &value);

    ~Value();

    [[nodiscard]] const Type::Ptr &getType() const;
    [[nodiscard]] llvm::Eisdrache::Local &getLLVMValue(llvm::Eisdrache::Ptr context) const;

    [[nodiscard]] std::string str() const;

private:
    Type::Ptr type;
    union {
        int64_t i64;
        double f64;
    };
};
