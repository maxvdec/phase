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

void Editor::draw_line_numbers() {
    int line_colors = create_pair(COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    set_color(line_colors);
    for (int i = 1; i < LINES; i++) {
        mvprintw(i, 0, "*");
    }
    move(0, 0);
    refresh();
    remove_color(line_colors);
}
