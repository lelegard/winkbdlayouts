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
#include "platform.h"

class Grid
{
public:
    // A line in the grid.
    typedef std::vector<std::string> Line;

    // Constructor.
    Grid(const std::string& margin = "", const std::string& spacing = " ");

    // Clear content.
    void clear() { _lines.clear(); }

    // Add one line.
    void addLine(const Line& line) { _lines.push_back(line); }

    // Add one column on last line.
    void addColumn(const std::string& text);

    // Add underlines under previous line.
    void addUnderlines(const Line& first_colums = Line(), char underline = '-');

    // Set attributes.
    void setMargin(const std::string& margin) { _margin = margin; }
    void setMargin(size_t width) { _margin = std::string(width, ' '); }
    void setSpacing(const std::string& spacing) { _spacing = spacing; }
    void setSpacing(size_t width) { _spacing = std::string(width, ' '); }

    // Print the grid. All columns are aligned on their maximum width.
    void print(std::ostream& out);

private:
    std::list<Line> _lines;
    std::string     _margin;
    std::string     _spacing;
};
