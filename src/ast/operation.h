#pragma once

#include <string>
#include "../eisdrache/src/eisdrache.hpp"

enum BinaryOp {
    ADD,    // addition
    SUB,    // subtraction
    MUL,    // multiplication
    DIV,    // division
    POW,    // power
};

enum UnaryOp {
    DEREF,   // dereference
};

inline const char *BinaryOpValue[] = {
    "+",
    "-",
    "*",
    "/",
    "^",
    "/",
    "^",
};

inline const char *UnaryOpValue[] = {
    "*",
};