#include <iostream>

#include "lexer/lexer.h"
#include "parser/parser.h"

int main() {
    const std::string source = "123+420/69-30*2";
    const Lexer lexer(source);
    const Token::Vec tokens = lexer.lex();


    for (auto &token : tokens)
        std::cout << token.str() << '\n';

    Parser parser(tokens);
    const RootAST::Ptr root = parser.parse();
    std::cout << root->str() << '\n';

    return 0;
}
