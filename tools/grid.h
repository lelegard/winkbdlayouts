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
#include "strutils.h"

class Grid
{
public:
    // A line in the grid.
    typedef WStringVector Line;

    // Constructor.
    Grid(const std::wstring& margin = L"", const std::wstring& spacing = L" ");

    // Clear content.
    void clear() { _lines.clear(); }

    // Add one line.
    void addLine(const Line& line) { _lines.push_back(line); }

    // Add one column on last line.
    void addColumn(const std::wstring& text);

    // Add underlines under previous line.
    void addUnderlines(const Line& first_colums = Line(), wchar_t underline = L'-');

    // Set attributes.
    void setMargin(const std::wstring& margin) { _margin = margin; }
    void setMargin(size_t width) { _margin = std::wstring(width, L' '); }
    void setSpacing(const std::wstring& spacing) { _spacing = spacing; }
    void setSpacing(size_t width) { _spacing = std::wstring(width, L' '); }

    // Print the grid. All columns are aligned on their maximum width.
    void print(std::ostream& out);

private:
    std::list<Line> _lines;
    std::wstring    _margin;
    std::wstring    _spacing;
};
