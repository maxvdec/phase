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

void Editor::draw_line_numbers() {
    auto [x, y] = get_cursor_pos();
    int line_colors = create_pair(COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    set_color(line_colors);
    for (int i = 1; i < LINES - 1; i++) {
        mvprintw(i, 0, "*");
    }

    std::vector<std::string> lines;
    std::string content = buffer.contents();
    size_t pos = 0;
    while ((pos = content.find('\n')) != std::string::npos) {
        lines.push_back(content.substr(0, pos));
        content.erase(0, pos + 1);
    }

    lines.push_back(content); // Add the last line if it exists

    for (int i = 0; i < lines.size() && i < LINES - 1; i++) {
        std::string line_number = std::to_string(i + 1);
        mvprintw(i, 0, "%s", line_number.c_str());
        mvprintw(i, 2, "%s", lines[i].c_str());
    }

    move(y, x);
    refresh();
    remove_color(line_colors);
}
