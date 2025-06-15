#include <iostream>

#include "io.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

int main() {
    const std::string source = readFile("src/test/test.lynx");
    const Lexer lexer(source);
    const Token::Vec tokens = lexer.lex();

    for (auto &token : tokens)
        std::cout << token.str() << '\n';

    Parser parser(tokens);
    const Root::Ptr root = parser.parse();
    std::cout << root->str() << '\n';

    Eisdrache::initialize();
    Eisdrache::Ptr eisdrache = Eisdrache::create("Lynx Compiler");

    Eisdrache::Func::Ptr puts = eisdrache->declareFunction(eisdrache->getSignedTy(64),
        "puts", {eisdrache->getUnsignedPtrTy(8)});
    puts->addAttr(llvm::Attribute::NoCapture, 0);


    eisdrache->declareFunction(eisdrache->getSignedTy(64), "main",
        {{"argc", eisdrache->getSizeTy()}, {"argv", eisdrache->getUnsignedPtrPtrTy(8)}}, true);
    root->generate(eisdrache);
    eisdrache->createRet(eisdrache->getInt(64, 0));
    eisdrache->dump("src/test/test.ll");

    return 0;
}
