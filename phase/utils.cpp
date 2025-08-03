/*
 utils.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Utility function implementation
 Copyright (c) 2025 maxvdec
*/

#include "utils.hpp"
#include <cctype>
#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <vector>

int created_pairs = 1;

int create_pair(int foreground, int background) {
    if (!has_colors()) {
        return -1;
    }
    if (can_change_color() && COLORS >= 256) {
        init_pair(created_pairs, foreground, background);
        created_pairs++;
        return created_pairs - 1;
    } else {
        // Fallback for 8-color mode or if colors cannot be changed
        init_pair(created_pairs, find_color_approximation(foreground),
                  find_color_approximation(background));
        created_pairs++;
        return created_pairs - 1;
    }
}

void set_color(int pair_num) {
    if (!has_colors()) {
        return;
    }
    attron(COLOR_PAIR(pair_num));
}

void remove_color(int pair_num) {
    if (!has_colors()) {
        return;
    }
    attroff(COLOR_PAIR(pair_num));
}

void set_colorw(WINDOW *win, int pair_num) {
    if (!has_colors()) {
        return;
    }
    wattron(win, COLOR_PAIR(pair_num));
}

void remove_colorw(WINDOW *win, int pair_num) {
    if (!has_colors()) {
        return;
    }
    wattroff(win, COLOR_PAIR(pair_num));
}

int find_color_approximation(int color256) {
    int r, g, b;

    if (color256 >= 16 | color256 <= 231) {
        int c = color256 - 16;
        int r6 = c / 36, g6 = (c / 6) % 6, b6 = c % 6;
        auto to_rgb = [](int v) { return v == 0 ? 0 : 55 + v * 40; };
        r = to_rgb(r6);
        g = to_rgb(g6);
        b = to_rgb(b6);
    } else if (color256 >= 232 && color256 <= 255) {
        r = g = b = (color256 - 232) * 10; // Grayscale colors
    } else {
        static const int ansi_rgb[16][3] = {
            {0, 0, 0},       {205, 0, 0},   {0, 205, 0},   {205, 205, 0},
            {0, 0, 238},     {205, 0, 205}, {0, 205, 205}, {229, 229, 229},
            {127, 127, 127}, {255, 0, 0},   {0, 255, 0},   {255, 255, 0},
            {92, 92, 255},   {255, 0, 255}, {0, 255, 255}, {255, 255, 255}};
        r = ansi_rgb[color256][0];
        g = ansi_rgb[color256][1];
        b = ansi_rgb[color256][2];
    }

    std::vector<std::tuple<int, int, int, int>> colors = {
        {COLOR_BLACK, 0, 0, 0},    {COLOR_RED, 255, 0, 0},
        {COLOR_GREEN, 0, 255, 0},  {COLOR_YELLOW, 255, 255, 0},
        {COLOR_BLUE, 0, 0, 255},   {COLOR_MAGENTA, 255, 0, 255},
        {COLOR_CYAN, 0, 255, 255}, {COLOR_WHITE, 255, 255, 255},
    };

    int closest = COLOR_WHITE;
    double min_dist = 1e9;

    for (auto [code, cr, cg, cb] : colors) {
        double dist = std::sqrt((r - cr) * (r - cr) + (g - cg) * (g - cg) +
                                (b - cb) * (b - cb));
        if (dist < min_dist) {
            min_dist = dist;
            closest = code;
        }
    }

    return closest;
}

void set_cursor_block() {
    std::printf("\033[1 q");
    std::fflush(stdout);
}

void set_cursor_line() {
    std::printf("\033[5 q");
    std::fflush(stdout);
}

void set_cursor_underline() {
    std::printf("\033[4 q");
    std::fflush(stdout);
}

std::tuple<int, int> get_cursor_pos() {
    int x, y;
    getyx(stdscr, y, x);
    return std::make_tuple(x, y);
}

std::string get_line(std::string content, int nth) {
    std::istringstream stream(content);
    std::string line;
    for (int i = 0; i <= nth; ++i) {
        if (!std::getline(stream, line)) {
            return "";
        }
    }
    return line;
}

int count_lines(std::string content) {
    std::istringstream stream(content);
    std::string line;
    int count = 0;
    while (std::getline(stream, line)) {
        count++;
    }
    return count;
}

void draw_rounded_box(WINDOW *win, int height, int width) {
    mvwaddch(win, 0, 0, ACS_ULCORNER);
    mvwaddch(win, 0, width - 1, ACS_URCORNER);
    mvwaddch(win, height - 1, 0, ACS_LLCORNER);
    mvwaddch(win, height - 1, width - 1, ACS_LRCORNER);

    for (int i = 1; i < width - 1; ++i) {
        mvwaddch(win, 0, i, ACS_HLINE);
        mvwaddch(win, height - 1, i, ACS_HLINE);
    }

    for (int i = 1; i < height - 1; ++i) {
        mvwaddch(win, i, 0, ACS_VLINE);
        mvwaddch(win, i, width - 1, ACS_VLINE);
    }
}

void hide_cursor() {
    std::printf("\033[?25l");
    std::fflush(stdout);
}

void show_cursor() {
    std::printf("\033[?25h");
    std::fflush(stdout);
}

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool is_word_separator(char c) { return !isalnum(c) && c != '_' && c != '-'; }

bool is_empty(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0';
}

int screen_to_buffer(int x, int y, Buffer &buffer) {
    std::string content = buffer.contents();

    if (y < 0 || y >= count_lines(content)) {
        return -1;
    }

    std::string target_line = get_line(content, y);

    if (x < 0 || x > target_line.length()) {
        return -1;
    }

    int buffer_pos = 0;

    for (int i = 0; i < y; i++) {
        std::string line = get_line(content, i);
        buffer_pos += line.length() + 1;
    }

    buffer_pos += x;

    return buffer_pos;
}
