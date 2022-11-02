/**
 * @file lexer.cpp
 * @author fuechs
 * @brief fux lexer 
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "lexer.hpp"

TokenList Lexer::lex() {
    while (idx < source.length()) {
        getToken();

        switch (currentToken.type) {
            case IDENTIFIER:
                while (getIdent())
                    advance();
                break;

            case STRING:
                getString();
                break;

            case NUMBER:
                getNumber();
                break;

            default: break;
        }

        endToken();
        // update line and column for next token
        currentToken.col = col;
        currentToken.line = line; 
    }

    currentToken.type = _EOF;
    currentToken.value = TokenTypeValue[_EOF];
    endToken();

    return tokens;
}

void Lexer::parseLines() {
    stringstream line;

    for (int i = 0; i < source.length(); i++) {
        if (source[i] == '\n') {
            lines.add(line.str());
            line.str("");
        } else
            line << source[i];
    }

    if (!line.str().empty())
        lines.add(line.str());
}

char Lexer::peek(int offset) {
    if ((idx + offset - 1) < source.length())
        return source[idx+offset];
    return NULL;
}

void Lexer::advance(int steps) {
    idx += steps;
    col += steps;
}

void Lexer::resetPos() {
    col = 1;
    ++line;
}

char Lexer::current() {
    return source[idx];
}

void Lexer::getToken() {
    currentToken.value = current();

    switch (currentToken.value.c_str()[0])  {

        case '(': 
            currentToken.type = LPAREN;
            advance();
            break;

        case ')': 
            currentToken.type = RPAREN;
            advance();
            break;
        
        case '{': 
            currentToken.type = LBRACE;
            advance();
            break;
        
        case '}': 
            currentToken.type = RBRACE;
            advance();
            break;
        
        case '[': 
            currentToken.type = LBRACKET;
            advance();
            break;

        case ']': 
            currentToken.type = RBRACKET;
            advance();
            break;

        case ':':
            if (peek() == ':') {
                currentToken.type = COLON_COLON;
                currentToken.value = "::";
                advance(2);
                break;
            }

            currentToken.type = COLON;
            advance();
            break;
        
        case ';':
            currentToken.type = SEMICOLON;
            advance();
            break;
        
        case '.':
            if (isdigit(peek())) {
                currentToken.type = NUMBER;
                advance();
                break;
            }

            if (peek() == '.' && peek(2) == '.') {
                currentToken.type = TRIPLE_DOT;
                currentToken.value = "...";
                advance(3);
                break;
            }

            currentToken.type = DOT;
            advance();
            break;
        
        case ',':
            currentToken.type = COMMA;
            advance();
            break;
        
        case '=':
            if (peek() == '=') {
                if (peek(2) == '=') {
                    currentToken.type = TRIPLE_EQUALS;
                    currentToken.value = "===";
                    advance(3);
                    break;
                }

                currentToken.type = EQUALS_EQUALS;
                currentToken.value = "==";
                advance(2);
                break;
            }
            currentToken.type = EQUALS;
            advance();
            break;
        
        case '<':
            if (peek() == '<') {
                currentToken.type = LSHIFT;
                currentToken.value = "<<";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = LTEQUALS;
                currentToken.value = "<=";
                advance(2);
                break;
            }

            currentToken.type = LESSTHAN;
            advance();
            break;
        
        case '>':
            if (peek() == '>') {
                currentToken.type = RSHIFT;
                currentToken.value = ">>";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = GTEQUALS;
                currentToken.value = ">=";
                advance(2);
                break;
            }

            currentToken.type = GREATERTHAN;
            advance();
            break;
        
        case '!':
            if (peek() == '=') {
                currentToken.type = NOT_EQUALS;
                currentToken.value = "!=";
                advance(2);
                break;
            }

            currentToken.type = EXCLAMATION;
            advance();
            break;
        
        case '?':
            currentToken.type = QUESTION;
            advance();
            break;
        
        case '+':
            if (peek() == '+') {
                currentToken.type = PLUS_PLUS;
                currentToken.value = "++";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = PLUS_EQUALS;
                currentToken.value = "+=";
                advance(2);
                break;
            }

            currentToken.type = PLUS;
            advance();
            break;
        
        case '-':
            if (peek() == '-') {
                currentToken.type = MINUS_MINUS;
                currentToken.value = "--";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = MINUS_EQUALS;
                currentToken.value = "-=";
                advance(2);
                break;
            }

            if (peek() == '>') {
                currentToken.type = RPOINTER;
                currentToken.value = "->";
                advance(2);
                break;
            }

            currentToken.type = MINUS;
            advance();
            break;
        
        case '*':
            if (peek() == '=') {
                currentToken.type = ASTERISK_EQUALS;
                currentToken.value = "*=";
                advance(2);
                break;
            }

            currentToken.type = ASTERISK;
            advance(2);
            break;
        
        case '/':
            if (skipComment())
                break;

            if (peek() == '=') {
                currentToken.type = SLASH_EQUALS;
                currentToken.value = "/=";
                advance(2);
                break;
            }

            currentToken.type = SLASH;
            advance();
            break;
        
        case '^':
            if (peek() == '=') {
                currentToken.type = CARET_EQUALS;
                currentToken.value = "^=";
                advance(2);
                break;
            }

            currentToken.type = CARET;
            advance();
            break;
        
        case '%':
            if (peek() == '=') {
                currentToken.type = PERCENT_EQUALS;
                currentToken.value = "%=";
                advance(2);
                break;
            }

            currentToken.type = PERCENT;
            advance();
            break;
    
        case '|':
            if (peek() == '|') {
                currentToken.type = OR;
                currentToken.value = "||";
                advance(2);
                break;
            }

            if (peek() == '|=') {
                currentToken.type = OR_EQUALS;
                currentToken.value = "|=";
                advance(2);
                break;
            }

            currentToken.type = BIT_OR;
            advance();
            break;

        case '&':
            if (peek() == '&') {
                currentToken.type = AND;
                currentToken.value = "&&";
                advance(2);
                break;
            }

            if (peek() == '=') {
                currentToken.type = AND_EQUALS;
                currentToken.value = "&=";
                advance(2);
                break;
            }

            currentToken.type = BIT_AND;
            advance();
            break;
        
        case ' ':
        case '\t':
        case '\r':
            advance();
            break;
        
        case '\n':
            advance();
            resetPos();
            break;
        
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            currentToken.type = NUMBER;
            advance();
            break;
        
        case '\'':
            currentToken.type = CHAR;
            if (!isalpha(peek()))
                errors->createNewError(EXPECTED_CHAR_LITERAL_EOF, line, col, "expected actual character");
            else {
                advance();
                currentToken.value = current();
                if (peek() != '\'')
                    errors->createNewError(ILLEGAL_CHAR_LITERAL_FORMAT, line, col, "expected ending quote");
                else
                    advance();
            }
            break;
         
        case '"':
            currentToken.type = STRING;
            currentToken.value = "";
            break;
        
        default:
            currentToken.type = IDENTIFIER;
            break;

    }
}

void Lexer::endToken() {
    if (currentToken.type != NONE)
        tokens.push_back(currentToken);
    
    currentToken.type = NONE;
    currentToken.value.erase();
}

bool Lexer::getIdent() {
    if ((isalpha(current())
    ||  isdigit(current())
    ||  current() == '_')
    &&  idx < source.length()) {
        currentToken.value.push_back(current());
        return true;
    }
    
    return false;
}

void Lexer::getString() {
    advance();
    if (idx >= source.length())
        errors->createNewError(EXPECTED_STRING_LITERAL_EOF, line, col, "unterminated string");
    
    while (current() != '"') {
        currentToken.value.push_back(current());
        advance();

        if (idx >= source.length())
            errors->createNewError(EXPECTED_STRING_LITERAL_EOF, line, col, "unterminated string");
    }

    advance();
}

void Lexer::getNumber() {

    while ((isdigit(current()) || current() == '_') && idx < source.length()) {
        if (current() != '_')
            currentToken.value.push_back(current());
        advance();
    }

    if (idx >= source.length())
        return;

    if (current() == '.') {
        advance();

        if (idx >= source.length())
            errors->createNewError(ILLEGAL_NUMBER_FORMAT, line, col, "character '.' with no following digits");
        else if (current() == '.')
            errors->createNewError(ILLEGAL_NUMBER_FORMAT, line, col, "invalid character '.'");
        else
            currentToken.value.push_back(current());
        
        while ((isdigit(current()) || current() == '_') && idx < source.length()) {
            if (current() != '_')
                currentToken.value.push_back(current());
            advance();
        }
    }

}

bool Lexer::skipComment() {
    if (peek() == '/') {
        while (idx < source.length() && current() != '\n')
            advance();
        advance();
        resetPos();
        return true;
    }

    if (peek() == '*') {
        while (idx < source.length() 
        && current() != '/' 
        && peek(2) != '*') {
            if (current() == '\n') {
                resetPos();
                --idx;
            }
            advance();
        }
        
        if (idx >= source.length())
            errors->createNewError(UNEXPECTED_EOF, line, col, "expected multi-line comment to end");
        else
            advance(2);
        
        return true;
    }

    return false;
}