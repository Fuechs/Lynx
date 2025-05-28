#pragma once

#include <memory>
#include <vector>

class Type {
public:
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;

    enum Kind {
        I64,
        F64,
        PTR,
    };

    explicit Type(Kind kind);
    virtual ~Type();

    [[nodiscard]] virtual constexpr bool isInteger() const { return false; }
    [[nodiscard]] virtual constexpr bool isFloat() const { return false; }
    [[nodiscard]] virtual constexpr bool isSigned() const { return false; }
    [[nodiscard]] virtual constexpr bool isPointer() const { return false;}

    [[nodiscard]] Kind getKind() const;

protected:
    Kind kind;
};

class IntType : public Type {
public:
    using Ptr = std::shared_ptr<IntType>;
    using Vec = std::vector<Ptr>;

    explicit IntType(Kind kind, bool _signed = false);

    [[nodiscard]] constexpr bool isInteger() const override { return true; }
    [[nodiscard]] constexpr bool isSigned() const override { return _signed; }

private:
    bool _signed;
};

class FloatType : public Type {
public:
    using Ptr = std::shared_ptr<FloatType>;
    using Vec = std::vector<Ptr>;

    explicit FloatType(Kind kind);

    [[nodiscard]] constexpr bool isFloat() const override { return true; }
    [[nodiscard]] constexpr bool isSigned() const override { return true; }
};

class PointerType : public Type {
public:
    using Ptr = std::shared_ptr<PointerType>;
    using Vec = std::vector<Ptr>;

    explicit PointerType(Type::Ptr pointee);

    [[nodiscard]] Type::Ptr getPointee() const;

    [[nodiscard]] constexpr bool isPointer() const override { return true;}

private:
    Type::Ptr pointee;
};