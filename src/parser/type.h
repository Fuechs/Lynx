#pragma once

#include <memory>
#include <vector>

class Type {
public:
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;

    enum Kind {
        INT,
        FLOAT,
    };

    Type() = default;
    virtual ~Type() = default;

    [[nodiscard]] virtual Kind getKind() const = 0;
};

class IntType : public Type {
public:
    using Ptr = std::shared_ptr<IntType>;
    using Vec = std::vector<Ptr>;

    explicit IntType(size_t bits);
    ~IntType() override;

    [[nodiscard]] Kind getKind() const override;

private:
    size_t bits{};
};