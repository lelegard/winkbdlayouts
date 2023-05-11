//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Display a grid of row / columns.
//
//----------------------------------------------------------------------------

#pragma once
#include "grid.h"


//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

Grid::Grid(const WString& margin, const WString& spacing) :
    _lines(),
    _margin(margin),
    _spacing(spacing)
{
}


//----------------------------------------------------------------------------
// Add one column on last line.
//----------------------------------------------------------------------------

void Grid::addColumn(const WString& text)
{
    if (_lines.empty()) {
        _lines.push_back(Line());
    }
    _lines.back().push_back(text);
}


//----------------------------------------------------------------------------
// Add underlines under previous line.
//----------------------------------------------------------------------------

void Grid::addUnderlines(const Line& first_colums, wchar_t underline)
{
    if (!_lines.empty()) {
        const Line& prev(_lines.back());
        Line next(first_colums);
        while (next.size() < prev.size()) {
            next.push_back(WString(prev[next.size()].length(), underline));
        }
        _lines.push_back(next);
    }
}


//----------------------------------------------------------------------------
// Remove empty lines or columns. Exclude some non-significant initial columns or lines.
//----------------------------------------------------------------------------

void Grid::removeEmptyLines(size_t header_columns_count, bool trim)
{
    for (auto it = _lines.begin(); it != _lines.end(); ) {
        if (trim) {
            for (WString& cell : *it) {
                Trim(cell);
            }
        }
        bool remove = true;
        for (size_t i = header_columns_count; remove && i < it->size(); ++i) {
            remove = (*it)[i].empty();
        }
        if (remove) {
            it = _lines.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Grid::removeEmptyColumns(size_t header_lines_count, bool trim)
{
    size_t col = 0;
    bool more_col = true;

    // Loop on columns;
    while (more_col) {

        // First pass: check if the column shall be kept or removed.
        more_col = false;
        bool remove = true;
        size_t line_index = 0;
        for (auto& line : _lines) {
            for (WString& cell : line) {
                Trim(cell);
            }
            if (line_index++ >= header_lines_count && col < line.size() && remove) {
                more_col = true;
                remove = line[col].empty();
            }
        }

        // Second pass: remove the column if necessary.
        if (remove) {
            for (auto& line : _lines) {
                if (col < line.size()) {
                    // line.erase(line.begin() + col) does not compile with VS 17.5.5 ????
                    Line::iterator it(line.begin());
                    std::advance(it, col);
                    line.erase(it);
                }
            }
        }
        else {
            ++col;
        }
    }
}


//----------------------------------------------------------------------------
// Print the grid. All columns are aligned on their maximum width.
//----------------------------------------------------------------------------

void Grid::print(std::ostream& out)
{
    std::vector<size_t> widths;

    // Compute columns widths.
    for (const auto& row : _lines) {
        for (size_t i = 0; i < row.size(); ++i) {
            const size_t w = row[i].length();
            if (widths.size() <= i) {
                widths.push_back(w);
            }
            else if (widths[i] < w) {
                widths[i] = w;
            }
        }
    }

    // Then print the grid.
    for (const auto& row : _lines) {
        out << _margin;
        for (size_t i = 0; i < row.size(); ++i) {
            out << row[i];
            if (i < row.size() - 1) {
                out << std::string(widths[i] - row[i].length(), ' ') << _spacing;
            }
        }
        out << std::endl;
    }
}
