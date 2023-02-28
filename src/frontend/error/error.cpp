/**
 * @file error.cpp
 * @author fuechs
 * @brief fux error manager 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "error.hpp"

ErrorManager::ErrorManager() : _errors(ParseError::Vec()), 
    errorCount(0), warningCount(0), sources(SourceMap()) {}

ErrorManager::~ErrorManager() {
    _errors.clear();
    sources.clear();
}

void ErrorManager::addSourceFile(const string &fileName, const vector<string> &sourceLines) { sources[fileName] = sourceLines; }

void ErrorManager::createError(const string &fileName, ParseError::Type type, const Token &token, string message, string info, bool aggressive) {
    Metadata meta = Metadata(&fileName, &sources.at(fileName), token.line, token.line, token.start, token.end);
    _errors.push_back(ParseError(aggressive ? (ParseError::FlagVec) {ParseError::AGGRESSIVE} : ParseError::FlagVec(), 
        type, message, (ParseError::SUBJ_STRCT) {meta, info, "", 0}));
}

void ErrorManager::createWarning(const string &fileName, ParseError::Type type, const Token &token, string message, string info, bool aggressive) {
    ParseError::FlagVec flags = {ParseError::WARNING};
    if (aggressive)
        flags.push_back(ParseError::AGGRESSIVE);
    Metadata meta = Metadata(&fileName, &sources.at(fileName), token.line, token.line, token.start, token.end);
    _errors.push_back(ParseError(flags, type, message, (ParseError::SUBJ_STRCT) {meta, info, "", 0}));
}

void ErrorManager::createError(const string &fileName, ParseError::Type type, size_t fstLine, size_t lstLine, string message, string info, bool aggressive) {
    Metadata meta = Metadata(&fileName, &sources.at(fileName), fstLine, lstLine, 1);
    _errors.push_back(ParseError(aggressive ? (ParseError::FlagVec) {ParseError::AGGRESSIVE} : ParseError::FlagVec(), 
        type, message, (ParseError::SUBJ_STRCT) {meta, info, "", 0}));
}

void ErrorManager::createRefError(const string &fileName, ParseError::Type type, const Token &token, const Token &refToken, string message, string info, string refInfo, bool aggressive) {
    ParseError::FlagVec flags = {ParseError::REFERENCE};
    if (aggressive)
        flags.push_back(ParseError::AGGRESSIVE);
    Metadata subj_meta = Metadata(&fileName, &sources.at(fileName), token.line, token.line, token.start, token.end);
    Metadata ref_meta = Metadata(&fileName, &sources.at(fileName), refToken.line, refToken.line, refToken.start, refToken.end);
    _errors.push_back(ParseError(flags, type, message, 
        (ParseError::SUBJ_STRCT) {subj_meta, info, "", 0}, (ParseError::SUBJ_STRCT) {ref_meta, refInfo, "", 0}));
}

void ErrorManager::createRefWarning(const string &fileName, ParseError::Type type, const Token &token, const Token &refToken, string message, string info, string refInfo, bool aggressive) {
    ParseError::FlagVec flags = {ParseError::REFERENCE, ParseError::WARNING};
    if (aggressive)
        flags.push_back(ParseError::AGGRESSIVE);
    Metadata subj_meta = Metadata(&fileName, &sources.at(fileName), token.line, token.line, token.start, token.end);
    Metadata ref_meta = Metadata(&fileName, &sources.at(fileName), refToken.line, refToken.line, refToken.start, refToken.end);
    _errors.push_back(ParseError(flags, type, message, 
        (ParseError::SUBJ_STRCT) {subj_meta, info, "", 0}, (ParseError::SUBJ_STRCT) {ref_meta, refInfo, "", 0}));
}


void ErrorManager::addHelp(string message) {
    _errors.back().addNote("Help: "+message);
}

void ErrorManager::addNote(string message) {
    _errors.back().addNote("Note: "+message);
}

void ErrorManager::report() {
    for (ParseError &pe : _errors)
        pe.report();
}

size_t ErrorManager::errors() { return errorCount; }

size_t ErrorManager::warnings() { return warningCount; }