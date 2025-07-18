cmake_minimum_required(VERSION 3.15)
project(Lynx VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs
        Support
        Core
        IRReader
        AsmParser
        BitReader
        BinaryFormat
        Remarks
        TargetParser
        BitstreamReader
        AllTargetsAsmParsers
        AllTargetsCodeGens
        AllTargetsDescs
        AllTargetsInfos
        ExecutionEngine
        MC
        MCParser
)

set(SOURCES
        src/analyzer/analyzer.cpp
        src/analyzer/symbol.cpp
        src/ast/expr.cpp
        src/ast/function.cpp
        src/ast/stmt.cpp
        src/lexer/lexer.cpp
        src/lexer/token.cpp
        src/parser/parser.cpp
        src/parser/type.cpp
        src/parser/value.cpp
        src/util/io.cpp
        src/wyvern/src/wyvern.cpp
        src/main.cpp
)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/analyzer
        ${PROJECT_SOURCE_DIR}/src/ast
        ${PROJECT_SOURCE_DIR}/src/lexer
        ${PROJECT_SOURCE_DIR}/src/parser
        ${PROJECT_SOURCE_DIR}/src/util
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(Lynx PRIVATE ${llvm_libs})