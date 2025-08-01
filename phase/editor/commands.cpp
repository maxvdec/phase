/*
 commands.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Commands for doing various things
 Copyright (c) 2025 maxvdec
*/

#include "editor.hpp"
#include "utils.hpp"
#include <ncurses.h>

void Editor::command_window() {
    int h = 4, w = 40;
    int start_y = (LINES - h) / 2;
    int start_x = (COLS - w) / 2;

    WINDOW *modal = newwin(h, w, start_y, start_x);
    keypad(modal, true);
    box(modal, 0, 0);
    // draw_rounded_box(modal, h, w);
    set_cursor_line();
    wattron(modal, A_BOLD);
    mvwprintw(modal, 1, 2, "Command");
    wattroff(modal, A_BOLD);
    int palette = create_pair(COLOR_BRIGHT_RED, COLOR_DEFAULT);
    set_colorw(modal, palette);
    wattron(modal, A_ITALIC);
    mvwprintw(modal, 2, 2, "%s", current_command.c_str());
    wmove(modal, 2, 2 + current_command.length());
    wrefresh(modal);

    int line_padding = 2;

    int ch;

    while ((ch = wgetch(modal))) {
        if (ch == '\n' || ch == 27)
            break;
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (!current_command.empty()) {
                current_command.pop_back();
                int y, x;
                getyx(modal, y, x);
                mvwprintw(modal, y, line_padding + current_command.length(),
                          " ");
            }
        } else if (ch >= 32 && ch <= 126) {
            current_command.push_back(static_cast<char>(ch));
        }
        mvwprintw(modal, 2, 2, "%s", current_command.c_str());
        move(2, line_padding + current_command.length());
        wrefresh(modal);
    }
    wattroff(modal, A_ITALIC);
    remove_colorw(modal, palette);

    werase(modal);
    wrefresh(modal);
    delwin(modal);

    clear();
    refresh();
}
