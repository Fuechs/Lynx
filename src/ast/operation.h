#pragma once
#include <string>

enum class BinaryOp {
    ADD,    // addition
    SUB,    // subtraction
    MUL,    // multiplication
    DIV,    // division
};

std::string BinaryOpValue(const BinaryOp &op);