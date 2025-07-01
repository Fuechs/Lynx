#pragma once

#include <memory>
#include <vector>

#include "token.h"
#include "../wyvern/src/wyvern.hpp"

class Type : public std::enable_shared_from_this<Type> {
public:
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;

    enum Kind {
        U8,
        C8,
        I32,
        I64,
        F64,
        PTR,
        LITERAL,
        AUTO,
    };

    explicit Type(Kind kind);
    virtual ~Type();

    static Ptr create(const Token &token);

    static std::string getKindValue(Kind kind);
    static Kind getKind(const std::string &kind);

    virtual wyvern::Ty::Ptr generate(const wyvern::Wrapper::Ptr &context);

    Ptr getPointerTo(); // get PtrType to this type

    [[nodiscard]] virtual constexpr bool isInteger() const { return false; }
    [[nodiscard]] virtual constexpr bool isFloat() const { return false; }
    [[nodiscard]] virtual constexpr bool isSigned() const { return false; }
    [[nodiscard]] virtual constexpr bool isPointer() const { return false; }
    [[nodiscard]] virtual constexpr bool isReference() const { return false; }

    [[nodiscard]] Kind getKind() const;

    [[nodiscard]] virtual std::string str() const;

protected:
    Kind kind;
};

// TODO: this class is unnecessary
class IntType : public Type {
public:
    using Ptr = std::shared_ptr<IntType>;
    using Vec = std::vector<Ptr>;

    explicit IntType(Kind kind);

    [[nodiscard]] constexpr bool isInteger() const override { return true; }
    [[nodiscard]] constexpr bool isSigned() const override { return kind == I64; }

    [[nodiscard]] std::string str() const override;
};

// TODO: this class is unnecessary
class FloatType : public Type {
public:
    using Ptr = std::shared_ptr<FloatType>;
    using Vec = std::vector<Ptr>;

    explicit FloatType(Kind kind);

    [[nodiscard]] constexpr bool isFloat() const override { return true; }
    [[nodiscard]] constexpr bool isSigned() const override { return true; }

    [[nodiscard]] std::string str() const override;
};

class PointerType : public Type {
public:
    using Ptr = std::shared_ptr<PointerType>;
    using Vec = std::vector<Ptr>;

    explicit PointerType(Type::Ptr pointee);

    [[nodiscard]] Type::Ptr getPointee() const;

    [[nodiscard]] constexpr bool isPointer() const override { return true; }

    [[nodiscard]] std::string str() const override;

private:
    Type::Ptr pointee;
};