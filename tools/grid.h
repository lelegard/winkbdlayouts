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
    Grid(const WString& margin = L"", const WString& spacing = L" ");

    // Clear content.
    void clear() { _lines.clear(); }

    // Add one line.
    void addLine(const Line& line) { _lines.push_back(line); }

    // Add one column on last line.
    void addColumn(const WString& text);

    // Add underlines under previous line.
    void addUnderlines(const Line& first_colums = Line(), wchar_t underline = L'-');

    // Remove empty lines or columns. Exclude some non-significant initial columns or lines.
    void removeEmptyLines(size_t header_columns_count = 0, bool trim = false);
    void removeEmptyColumns(size_t header_lines_count = 0, bool trim = false);

    // Set attributes.
    void setMargin(const WString& margin) { _margin = margin; }
    void setMargin(size_t width) { _margin = WString(width, L' '); }
    void setSpacing(const WString& spacing) { _spacing = spacing; }
    void setSpacing(size_t width) { _spacing = WString(width, L' '); }

    // Print the grid. All columns are aligned on their maximum width.
    void print(std::ostream& out);

private:
    std::list<Line> _lines;
    WString         _margin;
    WString         _spacing;
};
