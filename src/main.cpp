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

    wyvern::Wrapper::initialize();
    wyvern::Wrapper::Ptr context = wyvern::Wrapper::create("Lynx Compiler");

    root->generate(context);
    context->getFunc("puts")->addAttr(llvm::Attribute::NoCapture, 0);
    context->dump("src/test/test.ll");

    return 0;
}
