/*
 line_numbers.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Line number logic
 Copyright (c) 2025 maxvdec
*/

#include "editor.hpp"
#include "utils.hpp"
#include <ncurses.h>
#include <string>
#include <vector>

int line_padding = 6;

void Editor::draw_line_numbers() {
    auto [x, y] = get_cursor_pos();
    for (int i = 0; i < LINES - 1; i++) {
        move(i, 0);
        clrtoeol();
    }

    int line_colors = create_pair(COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    set_color(line_colors);

    std::string content = buffer.contents();
    std::vector<std::string> lines;
    size_t start = 0;
    size_t pos;

    while ((pos = content.find('\n', start)) != std::string::npos) {
        lines.push_back(content.substr(start, pos - start));
        start = pos + 1;
    }
    lines.push_back(content.substr(start));

    for (int i = 0; i < lines.size() && i < LINES - 1; ++i) {
        set_color(line_colors);
        mvprintw(i, 0, "%3d | ", i + 1);
        remove_color(line_colors);
        mvprintw(i, line_padding, "%s", lines[i].c_str());
    }

    set_color(line_colors);
    for (int i = lines.size(); i < LINES - 1; ++i) {
        mvprintw(i, 0, "  * |", i + 1);
    }
    remove_color(line_colors);

    move(y, x); // Restore cursor
    refresh();
    remove_color(line_colors);
}
