#include "type.h"

// INT TYPE

IntType::IntType(size_t bits) : bits(bits) {}

IntType::~IntType() = default;

Type::Kind IntType::getKind() const override { return Kind::INT; }