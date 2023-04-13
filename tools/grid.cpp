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
