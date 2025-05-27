#include "operation.h"

std::string BinaryOpValue(const BinaryOp &op) {
    switch (op) {
        case BinaryOp::ADD: return "+";
        case BinaryOp::SUB: return "-";
        case BinaryOp::MUL: return "*";
        case BinaryOp::DIV: return "/";
        default:            return "";
    }
}
