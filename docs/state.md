# Language State

![total lines](https://aschey.tech/tokei/github/fuechs/fux?labelColor=151515&color=fcaa68&style=for-the-badge)

### Built with [![C++][cpp-badge]](https://isocpp.org/)&nbsp;[![LLVM][llvm-badge]](https://llvm.org/)

> **The compiler is currently undergoing a major rework. \
> See the `rework` branch for progress.**

Fux is currently still in early development. There is no working compiler available for this language. \
**Fux is not supposed to be a superset of C++.**

At the moment I'm concentrating on improving the syntax of Fux, because some aspects are still not clear. The compiler is being developed alongside this process. 

I've been inspired by [C++](https://isocpp.org/), [Python](https://python.org/), [Odin](https://odin-lang.org/), [Elixir](https://elixir-lang.org/) and [Carbon](https://github.com/carbon-language/carbon-lang).

## Roadmap

#### [...] Stage I - Turing Complete in C++

- [...] Syntax
- [&check;] Error System
- [&check;] Source Management
- [&check;] Threading
- [&check;] Lexer
- [&check;] Abstract Syntax Tree
- [...] Parser
    - [&check;] Expressions
    - [...] Statements
        - [&check;] Other Statements
        - [&cross;] Structs
        - [&cross;] Type Definitions
    - [...] Error Reporting
    - [&check;] Metadata
    - [&cross;] Threading
- [...] Analyser
    - [...] Scopes
    - [...] Lifetimes
    - [...] Symbols
        - [...] Table
        - [...] Variables
        - [...] Functions
        - [&cross;] Structs / Classes
        - [&cross;] Enumerations
        - [&cross;] Macros
        - [&cross;] Type Definitions
    - [...] Expressions
    - [...] Statements
- [...] Context [![LLVM][llvm-badge]](https://llvm.org/)
- [...] Generator [![LLVM][llvm-badge]](https://llvm.org/)
    - [...] [Eisdrache Wrapper](https://github.com/fuechs/eisdrache)
    - [...] Expressions
    - [...] Statements
- [&cross;] Optimizer [![LLVM][llvm-badge]](https://llvm.org/)
- [&cross;] Compiler [![LLVM][llvm-badge]](https://llvm.org/)

I consider the language turing complete once it is able to compile and run [Rule 110](../old_scr/examples/rule110.fux). To set a realistic goal though, it is not planned that Fux will support more complex structures, such as classes, as well as pythonic features at that point. 

###### [&cross;] Stage II - Core Package

###### [&cross;] Stage III - Alpha Release

[llvm-badge]: https://img.shields.io/badge/LLVM-4c1717?logo=llvm&logoColor=white
[cpp-badge]: https://img.shields.io/badge/C++-1a3b63?logo=cplusplus&logoColor=white