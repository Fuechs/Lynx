/**
 * @file llvmheader.hpp
 * @author fuechs
 * @brief llvm header include file
 * @version 0.1
 * @date 2022-11-27
 * 
 * @copyright Copyright (c) 2020-2025, Fuechs and Contributors. 
 * 
 */

#pragma once

#include "lynx.hpp"

#ifdef LYNX_BACKEND

#include "eisdrache/eisdrache.hpp"

using llvm::Eisdrache;

#endif

// type definitions for generating objects
typedef int8_t      _i8;
typedef uint8_t     _u8;
typedef char        _c8;
typedef int16_t     _i16;
typedef uint16_t    _u16;
typedef wchar_t     _c16;
// typedef _Float16    _f16;
typedef int32_t     _i32;
typedef uint32_t    _u32;
typedef float       _f32;
typedef int64_t     _i64;
typedef uint64_t    _u64;
typedef double      _f64;