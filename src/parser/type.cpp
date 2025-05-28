#include "type.h"

#include <utility>

// TYPE

Type::Type(Kind kind) : kind(kind) {}

Type::~Type() = default;

Type::Kind Type::getKind() const { return kind; }

// INT TYPE

IntType::IntType(Kind kind, bool _signed) : Type(kind), _signed(_signed) {}

// FLOAT TYPE

FloatType::FloatType(Kind kind) : Type(kind) {}

// POINTER TYPE

PointerType::PointerType(Type::Ptr pointee) : Type(PTR), pointee(std::move(pointee)) {}

Type::Ptr PointerType::getPointee() const { return pointee; }