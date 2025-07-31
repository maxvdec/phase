/*
 utils.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Utility function implementation
 Copyright (c) 2025 maxvdec
*/

#include "utils.hpp"
#include <iostream>
#include <ncurses.h>
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
