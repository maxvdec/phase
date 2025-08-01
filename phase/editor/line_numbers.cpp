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
    for (int i = 0; i < LINES - 2; i++) {
        move(i, 0);
        clrtoeol();
    }

    std::string content = buffer.contents();
    std::vector<std::string> lines;
    size_t start = 0;
    size_t pos;
    while ((pos = content.find('\n', start)) != std::string::npos) {
        lines.push_back(content.substr(start, pos - start));
        start = pos + 1;
    }
    lines.push_back(content.substr(start));

    if (lines.empty()) {
        lines.push_back("");
    }

    int line_colors = create_pair(COLOR_BRIGHT_CYAN, COLOR_DEFAULT);

    std::string current_line =
        (editing_line < lines.size()) ? lines[editing_line] : "";
    int available_width = COLS - line_padding;
    int scroll_offset = 0;

    if (editing_x >= available_width) {
        scroll_offset = editing_x - available_width + 1;
    }

    for (int i = 0; i < LINES - 2; ++i) {
        set_color(line_colors);
        if (i < lines.size()) {
            mvprintw(i, 0, "%3d ┃", i + 1);
        } else {
            mvprintw(i, 0, "  * ┃");
        }
        remove_color(line_colors);

        if (i < lines.size()) {
            std::string line_text = lines[i];

            if (scroll_offset > 0) {
                if ((int)line_text.length() > scroll_offset) {
                    line_text = line_text.substr(scroll_offset);
                } else {
                    line_text = "";
                }
            }

            if ((int)line_text.length() > available_width) {
                line_text = line_text.substr(0, available_width);
            }

            if (!line_text.empty()) {
                mvprintw(i, line_padding, "%s", line_text.c_str());
            }
        }
    }

    int cursor_y = editing_line;
    int cursor_x;

    if (scroll_offset > 0) {
        cursor_x = COLS - 1;
    } else {
        cursor_x = line_padding + editing_x;
        if (cursor_x >= COLS) {
            cursor_x = COLS - 1;
        }
    }

    if (cursor_y >= LINES - 2) {
        cursor_y = LINES - 3;
    }
    if (cursor_y < 0) {
        cursor_y = 0;
    }

    move(cursor_y, cursor_x);
}
