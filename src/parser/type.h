#pragma once

#include <memory>
#include <vector>

#include "token.h"
#include "../eisdrache/src/eisdrache.hpp"

using llvm::Eisdrache;

class Type {
public:
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;

    enum Kind {
        I64,
        F64,
        PTR,
        AUTO,
    };

    explicit Type(Kind kind);
    virtual ~Type();

    static Ptr create(const Token &token);

    static std::string getKindValue(Kind kind);
    static Kind getKind(const std::string &kind);

    // TODO: virtual Eisdrache::Local &generate(Eisdrache::Ptr context);

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

class IntType : public Type {
public:
    using Ptr = std::shared_ptr<IntType>;
    using Vec = std::vector<Ptr>;

    explicit IntType(Kind kind);

    [[nodiscard]] constexpr bool isInteger() const override { return true; }
    [[nodiscard]] constexpr bool isSigned() const override { return kind == I64; }

    [[nodiscard]] std::string str() const override;
};

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