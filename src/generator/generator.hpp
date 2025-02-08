/**
 * @file generator.hpp
 * @author fuechs
 * @brief fux ir generator header
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

#ifdef FUX_BACKEND
#include "../llvmheader.hpp"
#include "../ast/ast.hpp"

class Generator {
public:
    Generator(Root::Ptr &root, Eisdrache *eisdrache) 
    : root(std::move(root)), eisdrache(eisdrache) {}

    void generate();
    
    static Eisdrache::Ty *getType(Eisdrache *eisdrache, Fux::Type type);

private:
    Root::Ptr root;
    Eisdrache *eisdrache;

    void debugPrint(const string message);
};

#endif