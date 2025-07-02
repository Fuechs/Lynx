#pragma once

enum BinaryOp {
    ADD,    // addition
    SUB,    // subtraction
    MUL,    // multiplication
    DIV,    // division
    POW,    // power
};

enum UnaryOp {
    ADDR,     // address of
    DEREF,    // dereference
    PRE_INC,  // pre increment
    PRE_DEC,  // pre decrement
    POST_INC, // post increment
    POST_DEC, // post decrement
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
    "&",
    "*",
    "++",
    "--",
    "++",
    "--"
};