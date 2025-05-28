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
    const RootAST::Ptr root = parser.parse();
    std::cout << root->str() << '\n';

    const Type::Ptr ty = std::make_shared<IntType>(Type::I64);
    std::cout << ty->isInteger() << std::endl;

    return 0;
}
