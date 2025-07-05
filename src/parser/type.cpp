#include "type.h"

#include <utility>
#include "../analyzer/analyzer.h"

const char *TypeKindString[] = {
    "void",
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
    return std::make_shared<Type>(getKind(token.getValue()));
}

std::string Type::getKindValue(Kind kind) { return TypeKindString[kind]; }

Type::Kind Type::getKind(const std::string &kind) {
    // get index in TypeKindString and convert to Kind enum
    return static_cast<Kind>(std::find(TypeKindString, TypeKindString + std::size(TypeKindString) - 1, kind) - TypeKindString);
}

bool Type::operator==(const Type &comp) const {
    if (comp.kind == PTR) // if this was a pointer, PointerType's operator would apply
        return false;

    return kind == comp.kind;
}

void Type::analyze(const Analyzer::Ptr &analyzer) {

}

wyvern::Ty::Ptr Type::generate(const wyvern::Wrapper::Ptr &context) {
    switch (kind) {
        case VOID:  return context->getVoidTy();
        case U8:    return context->getUnsignedTy(8);
        case I32:   return context->getSignedTy(32);
        case I64:   return context->getSignedTy(64);
        case F64:   return context->getFloatTy(64);
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

std::string Type::str() const { return getKindValue(kind); }

// POINTER TYPE

PointerType::PointerType(Type::Ptr pointee) : Type(PTR), pointee(std::move(pointee)) {}

bool PointerType::operator==(const Type &comp) const {
    if (comp.getKind() != PTR)
        return false;

    return pointee == dynamic_cast<const PointerType *>(&comp)->pointee;
}

bool PointerType::operator==(const PointerType &comp) const { return pointee == comp.pointee; }

Type::Ptr PointerType::getPointee() const { return pointee; }

std::string PointerType::str() const { return pointee->str() + "*"; }