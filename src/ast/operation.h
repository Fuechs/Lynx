#pragma once

#include <string>
#include "../eisdrache/src/eisdrache.hpp"

enum class BinaryOp {
    ADD,    // addition
    SUB,    // subtraction
    MUL,    // multiplication
    DIV,    // division
    POW,    // power
};

std::string BinaryOpValue(const BinaryOp &op);