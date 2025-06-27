#include "type.h"

#include <utility>

const char *TypeKindString[] = {
    "u8",
    "i32",
    "i64",
    "f64",
    "ptr",
    "literal",
    "auto",
};

// TYPE

Type::Type(Kind kind) : kind(kind) {}

Type::~Type() = default;

Type::Ptr Type::create(const Token &token) {
    switch (Kind kind = getKind(token.getValue())) {
        case I32:
        case I64: return std::make_shared<IntType>(kind);
        case F64: return std::make_shared<FloatType>(kind);
        default:  return nullptr;
    }
}

std::string Type::getKindValue(Kind kind) { return TypeKindString[kind]; }

Type::Kind Type::getKind(const std::string &kind) {
    // get index in TypeKindString and convert to Kind enum
    return static_cast<Kind>(std::find(TypeKindString, TypeKindString + std::size(TypeKindString) - 1, kind) - TypeKindString);
}

wyvern::Ty::Ptr Type::generate(const wyvern::Wrapper::Ptr &context) {
    switch (kind) {
        case U8:  return context->getUnsignedTy(8);
        case I32: return context->getSignedTy(32);
        case I64: return context->getSignedTy(64);
        case F64: return context->getFloatTy(64);
        case PTR: {
            auto cast = std::static_pointer_cast<PointerType>(shared_from_this());
            return cast->getPointee()->generate(context)->getPtrTo();
        }
        case LITERAL: return context->getUnsignedPtrTy(8);
        case AUTO:
        default:
            return nullptr;
    }
}

Type::Ptr Type::getPointerTo() { return std::make_shared<PointerType>(shared_from_this()); }


Type::Kind Type::getKind() const { return kind; }

std::string Type::str() const { return ""; }

// INT TYPE

IntType::IntType(Kind kind) : Type(kind) {}

std::string IntType::str() const { return getKindValue(kind); }

// FLOAT TYPE

FloatType::FloatType(Kind kind) : Type(kind) {}

std::string FloatType::str() const { return getKindValue(kind); }

// POINTER TYPE

PointerType::PointerType(Type::Ptr pointee) : Type(PTR), pointee(std::move(pointee)) {}

Type::Ptr PointerType::getPointee() const { return pointee; }

std::string PointerType::str() const { return pointee->str() + "*"; }