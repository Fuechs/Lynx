/**
 * @file lynx.hpp
 * @brief base header
 * @version 0.1
 * @date 2022-11-03
 * 
 * @copyright Copyright (c) 2020-2025, Fuechs and Contributors.
 * 
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "util/color.hpp"
#include "util/io.hpp"

using std::cout, std::cerr, std::endl, std::exception, std::make_shared, std::make_unique,
        std::map, std::pair, std::reference_wrapper, std::shared_ptr, 
        std::string, std::stringstream, std::to_string, std::unique_ptr, std::vector;

using StringVec = vector<string>;

#define ERROR_TEST

// #define LYNX_BACKEND

#ifdef LYNX_BACKEND
    // do nothing
    #define LYNX_BC(code) code
#else
    // remove redundant (backend) code
    #define LYNX_BC(code)
#endif

// #define FUX_ANALYSER

#ifdef FUX_ANALYSER
    // do nothing
    #define FUX_AC(code) code
#else
    // remove redundant (analyser) code
    #define FUX_AC(code)
#endif

#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(_WIN32))
    #ifndef _WIN64
        #define FUX_WIN_INVALID
    #else
        #define FUX_WIN
        #include <windows.h>
    #endif
#elif defined(__linux__) 
    #define FUX_LINUX 
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
    #define FUX_UNIX
#elif defined(__APPLE__) && defined(__MACH__) 
    #define FUX_DARWIN
#else
    #define FUX_UNKNOWN_PLATFORM
#endif

class Source;

// compiler options / flags
struct FuxStruct {
    std::string mainFile = "";              // main file containing main() function
    std::string version = "0.1-dev";        // targeted version (standard is latest)
    std::string out = "a.out";              // name of output binary file (standard is 'a.out')
    StringVec libraries = {                 // library paths
        #if defined(FUX_LINUX)
            "/usr/include/fux/"
        #elif defined(FUX_DARWIN)
            "/usr/local/include/fux/"
        #endif
    }; 

    vector<Source *> sources = {};

    bool aggressive     = false; // aggressive errors
    bool compileOnly    = false; // compile only to llvm ir
    bool warnings       = true;  // throw warnings
    bool optimize       = true;  // optimize generated code
    bool werrors        = false;  // treat warnings as errors
    bool threading      = true;  // use threading 
    bool debug          = true;  // debug logs 
    size_t errorLimit   = 1000;  // amount of errors until stop
                                 // this will be decremented with every throw
                                 // when it hits zero, the compilation is halted
    std::string target  = "";    // targeted architecture / platform
                                 // e.g. arm64-apple-darwin22.2.0
                                 //      x86_64-unknown-linux-gnu

    bool errorThrown    = false; // wether an error was thrown
    bool warningThrown  = false; // wether a warning was thrown
};

extern FuxStruct fux;

template<typename T, typename A>
vector<T, A> operator+(const vector<T, A> &lhs, const vector<T, A> &rhs);

template<typename T, typename A>
vector<T, A> operator+(const vector<T, A> &lhs, const T &rhs);

string operator*(const string &lhs, size_t rhs);

// throw internal compiler error
void internalError(std::string message = "Unknown Error.");