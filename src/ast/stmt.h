#pragma once

#include <memory>
#include <vector>

class StmtAST {
public:
    using Ptr = std::shared_ptr<StmtAST>;
    using Vec = std::vector<Ptr>;

    virtual ~StmtAST();

    virtual std::string str() const = 0;
};

class RootAST : public StmtAST {
public:
    using Ptr = std::shared_ptr<RootAST>;

    RootAST();
    ~RootAST() override;

    void addStmt(StmtAST::Ptr stmt);
    std::string str() const override;

private:
    Vec program;
};