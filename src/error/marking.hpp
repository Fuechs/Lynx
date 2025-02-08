/**
 * @file marking.hpp
 * @author fuechs
 * @brief Error Marking Header
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

struct Underline {
    using Ptr = shared_ptr<Underline>;
    using Vec = vector<Ptr>;

    Underline(size_t line = 0, size_t start = 0, size_t end = 0, string message = "");
    ~Underline();

    string print(size_t padding, size_t &cursor, size_t size);

    // false: (red) ~~~~~~
    // true : (blue) ------
    bool dashed;
/*           
      start       end
       |          |
[line] foo_bar_foo; 
       ~~~~~~~~~~~~
*/
    size_t line, start, end, size;
                                /* foo_bar;
                                   |~~~~~~
                                   | <- size = 3
                                   | 
                                   <message>
                                */
/*
    foo_bar_foo;
    ~~~~~~~~~~~~ The message.

(if the space right from the underline is taken:)
    foo_bar_foo;
    |~~~~~~~~~~~ <taken>
    The message.
*/
    string message;
};

struct BlockHighlight {
    using Ptr = shared_ptr<BlockHighlight>;
    using Vec = vector<Ptr>;

    BlockHighlight(size_t start = 0, size_t end = 0, string message = "");
    ~BlockHighlight();

    size_t start, end; // first and last line
    string message;

    /*
              |  ____________
      [start] | | foo_bar++; 
          ... | | ...
        [end] | | buzz(); 
              | \__ <message> 
    */
};

struct Comment {
    using Ptr = shared_ptr<Comment>;
    using Vec = vector<Ptr>;

    Comment(size_t line = 0, size_t col = 0, string message = "");
    ~Comment();

    string print();

/*
        {
            foo
            bar
        }
[line]  ; <message>
        |
       col 
*/
    size_t line, col;
    string message;
};

Comment::Vec operator+(const Comment::Ptr &lhs, const Comment::Ptr &rhs);