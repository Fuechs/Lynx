#pragma once

#include <memory>
#include <vector>

#include "token.h"
#include "../wyvern/src/wyvern.hpp"

class Analyzer;

class Type : public std::enable_shared_from_this<Type> {
public:
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;
    using Map = std::map<std::string, Ptr>;

    enum Kind {
        VOID,
        U8,
        I32,
        I64,
        F64,
        PTR,
        REF,
        FUNC,
        LITERAL,
        AUTO,
    };

    explicit Type(Kind kind);
    virtual ~Type();

    static Ptr create(const Token &token);

    static std::string getKindValue(Kind kind);
    static Kind getKind(const std::string &kind);

    virtual bool operator==(const Type &comp) const;

    virtual void analyze(const std::shared_ptr<Analyzer> &analyzer);
    virtual wyvern::Ty::Ptr generate(const wyvern::Wrapper::Ptr &context);

    Ptr getPointerTo(); // get PtrType to this type

    [[nodiscard]] virtual constexpr bool isInteger() const { return !isFloat(); }
    [[nodiscard]] virtual constexpr bool isFloat() const { return kind == F64; }
    [[nodiscard]] virtual constexpr bool isSigned() const { return isFloat() || kind == I32 || kind == I64; }
    [[nodiscard]] virtual constexpr bool isPointer() const { return false; }
    [[nodiscard]] virtual constexpr bool isReference() const { return false; }
    [[nodiscard]] virtual constexpr bool isFunction() const { return false; }

    [[nodiscard]] Kind getKind() const;

    [[nodiscard]] virtual std::string str() const;

protected:
    Kind kind;
};

class PointerType : public Type {
public:
    using Ptr = std::shared_ptr<PointerType>;
    using Vec = std::vector<Ptr>;

    explicit PointerType(Type::Ptr pointee);

    bool operator==(const Type &comp) const override;
    bool operator==(const PointerType &comp) const;

    [[nodiscard]] Type::Ptr getPointee() const;

    [[nodiscard]] constexpr bool isPointer() const override { return true; }

    [[nodiscard]] std::string str() const override;

private:
    Type::Ptr pointee;
};

class ReferenceType : public Type {
public:
    explicit ReferenceType(Type::Ptr referee);

    bool operator==(const Type &comp) const override;
    bool operator==(const ReferenceType &comp) const;

    [[nodiscard]] Type::Ptr getReferee() const;

    [[nodiscard]] constexpr bool isReference() const override { return true; }

    [[nodiscard]] std::string str() const override;

private:
    Type::Ptr referee;
};

class FunctionType : public Type {
public:
    using Ptr = std::shared_ptr<FunctionType>;

    explicit FunctionType(Type::Ptr returnType, Type::Vec parameterTypes);

    bool operator==(const Type &comp) const override;
    bool operator==(const FunctionType &comp) const;

    [[nodiscard]] constexpr const Type::Ptr &getReturnType() const { return returnType; }
    [[nodiscard]] constexpr const Type::Vec &getParameterTypes() const { return parameterTypes; }

    [[nodiscard]] constexpr bool isFunction() const override { return true; }

    [[nodiscard]] std::string str() const override;

private:
    Type::Ptr returnType;
    Type::Vec parameterTypes;
};