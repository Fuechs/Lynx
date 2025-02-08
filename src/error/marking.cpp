/**
 * @file marking.cpp
 * @author fuechs
 * @brief Error Marking Implementation
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "marking.hpp"

Underline::Underline(size_t line, size_t start, size_t end, string message) 
: dashed(true), line(line), start(start), end(end), size(0), message(message) {}

Underline::~Underline() { message.clear(); }

string Underline::print(size_t padding, size_t &cursor, size_t size) {
    stringstream ss;

    ss << (dashed ? CC::BLUE : CC::RED);

    for (; cursor <= end; cursor++) {
        if (cursor == start && this->size > size) 
            ss << '|';
        else if (size == 0 && cursor >= start && cursor <= end)
            ss << (start == end ? '^' : (dashed ? '-' : '~'));
        else if (size != 0 && this->size == size && cursor < start)
            ss << ' '; 
        else if (size == 0)
            ss << ' ';
    }

    if (this->size == size) {
        if (this->size == 0)
            ss << ' ';
        ss << message;
    }

    return ss.str();
}

BlockHighlight::BlockHighlight(size_t start, size_t end, string message) 
: start(start), end(end), message(message) {}

BlockHighlight::~BlockHighlight() { message.clear(); }

Comment::Comment(size_t line, size_t col, string message) 
: line(line), col(col), message(message) {}

Comment::~Comment() { message.clear(); }

string Comment::print() {
    return string(col - 1, ' ') + CC::GREEN + SC::BOLD + "; " + message + "\n" + SC::RESET;
}
 
Comment::Vec operator+(const Comment::Ptr &lhs, const Comment::Ptr &rhs) { return {lhs, rhs}; }