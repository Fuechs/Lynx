/**
 * @file subject.cpp
 * @author fuechs
 * @brief Error Subject Implementation
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "subject.hpp"
#include "../util/source.hpp"
#include "renderer.hpp"

/// SUBJECT ///

Subject::Subject(Metadata meta, Underline::Ptr primary, Underline::Ptr secondary, BlockHighlight::Ptr highlight, 
    Comment::Vec comments, Suggestion::Vec suggestions, Vec references, Ptr traceback) 
: meta(meta), primary(primary), secondary(secondary), highlight(highlight), comments(comments), 
    suggestions(suggestions), references(references), traceback(traceback) 
{
    if (highlight && (primary || secondary || !comments.empty()))
        assert(!"Cannot render both highlight and underline or comment in an error.");
}

Subject::~Subject() {
    comments.clear();
    suggestions.clear();
    references.clear();
}

string Subject::print() {
    size_t padding = to_string(meta.lstLine).size() + 4;
    stringstream ss;

    size_t line = 0, col = 0;

    Source *src = nullptr;
    for (Source *_ : fux.sources)
        if (meta.file == _->filePath) {
            src = _;
            break;
        }
    
    primary->dashed = false;
    line = primary->line;
    col = primary->start;

    using LineMap = map<size_t, pair<Underline::Vec, Comment::Vec>>;
//                   line number              markings

    LineMap lines = {};

    // figure out which lines have markings or suggestions
    for (Comment::Ptr &mark : comments) 
        lines[mark->line].second.push_back(mark);

    lines[primary->line].first.push_back(primary);
    lines[secondary->line].first.push_back(secondary);

    // previous line
    #define prev() (--lineIter)++ 

    // figure out which lines inbetween markings should be printed 
    for (LineMap::iterator lineIter = lines.begin(); lineIter != lines.end(); lineIter++) 
        if (lineIter != lines.begin())
            // if gap is <= 3, add missing lines
            for (size_t i = prev()->first; (lineIter->first - i <= 3) && i < lineIter->first; i++)
                if (!lines.contains(i))
                    lines[i] = {}; 

    #undef prev

    if (line == 0 || col == 0) {
        line = lines.begin()->second.first.front()->line;
        col = lines.begin()->second.first.front()->start;
    }

    ss << CC::BLUE << SC::BOLD << string(padding - 2, ' ') << ">>> " << CC::DEFAULT 
        << meta.file << ':' << line << ':' << col << '\n'
        << Renderer(src, lines, suggestions).render(padding);

    return ss.str();
}