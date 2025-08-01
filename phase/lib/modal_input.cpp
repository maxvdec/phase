/*
 modal_input.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Modal input functions
 Copyright (c) 2025 maxvdec
*/

#include "lib.hpp"
#include "utils.hpp"
#include <ncurses.h>
#include <string>

std::string modal_input(std::string title, std::string description,
                        std::string default_value) {
    auto [x, y] = get_cursor_pos();
    int h = 6, w = COLS - 4;
    int start_y = (LINES - h) / 2;
    int start_x = (COLS - w) / 2;

    WINDOW *modal = newwin(h, w, start_y, start_x);
    keypad(modal, true);
    box(modal, 0, 0);
    set_cursor_line();
    wattron(modal, A_BOLD);
    mvwprintw(modal, 1, 2, "%s", title.c_str());
    wattroff(modal, A_BOLD);
    mvwprintw(modal, 2, 2, "%s", description.c_str());
    int palette = create_pair(COLOR_BRIGHT_RED, COLOR_DEFAULT);
    set_colorw(modal, palette);
    wattron(modal, A_ITALIC);
    mvwprintw(modal, 2, 2, "%s", default_value.c_str());
    wmove(modal, 3, 2 + default_value.length());
    wrefresh(modal);

    int line_padding = 2;

    int ch;

    std::string final = default_value;

    while ((ch = wgetch(modal))) {
        if (ch == '\n' || ch == 27)
            break;
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (!final.empty()) {
                final.pop_back();
                int y, x;
                getyx(modal, y, x);
                mvwprintw(modal, y, line_padding + final.length(), " ");
            }
        } else if (ch >= 32 && ch <= 126) {
            final.push_back(static_cast<char>(ch));
        }
        mvwprintw(modal, 3, 2, "%s", final.c_str());
        move(2, line_padding + final.length());
        wrefresh(modal);
    }
    wattroff(modal, A_ITALIC);
    remove_colorw(modal, palette);

    werase(modal);
    wrefresh(modal);
    delwin(modal);

    clear();
    move(y, x);
    refresh();

    return final;
}

int modal_char(std::string title, std::string description, int default_value) {
    int h = 2, w = 40;
    int start_y = (LINES - h) / 2;
    int start_x = (COLS - w) / 2;

    WINDOW *modal = newwin(h, w, start_y, start_x);
    keypad(modal, true);
    box(modal, 0, 0);
    set_cursor_line();
    wattron(modal, A_BOLD);
    mvwprintw(modal, 1, 2, "Command");
    wattroff(modal, A_BOLD);
    int palette = create_pair(COLOR_BRIGHT_RED, COLOR_DEFAULT);
    set_colorw(modal, palette);
    wattron(modal, A_ITALIC);
    wrefresh(modal);

    int line_padding = 2;
    hide_cursor();

    move(2, line_padding);
    int ch;

    while ((ch = wgetch(modal))) {
    }
    move(1, line_padding + 1);
    wattroff(modal, A_ITALIC);
    remove_colorw(modal, palette);

    werase(modal);
    wrefresh(modal);
    delwin(modal);

    show_cursor();

    clear();
    refresh();

    return ch;
}
