/*
 utils.hpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Utility functions for various uses
 Copyright (c) 2025 maxvdec
*/

#include "buffer.hpp"
#include <ncurses.h>
#include <string>
#include <tuple>
#include <vector>
constexpr int COLOR_DEFAULT = -1; // Uses terminal's default
constexpr int COLOR_BLACK_256 = 0;
constexpr int COLOR_RED_256 = 1;
constexpr int COLOR_GREEN_256 = 2;
constexpr int COLOR_YELLOW_256 = 3;
constexpr int COLOR_BLUE_256 = 4;
constexpr int COLOR_MAGENTA_256 = 5;
constexpr int COLOR_CYAN_256 = 6;
constexpr int COLOR_WHITE_256 = 7;

// Bright
constexpr int COLOR_BRIGHT_BLACK = 8;
constexpr int COLOR_BRIGHT_RED = 9;
constexpr int COLOR_BRIGHT_GREEN = 10;
constexpr int COLOR_BRIGHT_YELLOW = 11;
constexpr int COLOR_BRIGHT_BLUE = 12;
constexpr int COLOR_BRIGHT_MAGENTA = 13;
constexpr int COLOR_BRIGHT_CYAN = 14;
constexpr int COLOR_BRIGHT_WHITE = 15;

// Vivid
constexpr int COLOR_ORANGE = 202;
constexpr int COLOR_PINK = 205;
constexpr int COLOR_PURPLE = 93;
constexpr int COLOR_TURQUOISE = 51;
constexpr int COLOR_LIME = 118;
constexpr int COLOR_SKY_BLUE = 117;
constexpr int COLOR_VIOLET = 177;
constexpr int COLOR_INDIGO = 54;

// Grayscales
constexpr int COLOR_GRAY_1 = 238;
constexpr int COLOR_GRAY_2 = 242;
constexpr int COLOR_GRAY_3 = 246;
constexpr int COLOR_GRAY_4 = 250;
constexpr int COLOR_GRAY_5 = 254;

// Extras
constexpr int COLOR_SAND = 180; // yellowish-beige
constexpr int COLOR_MINT = 121;
constexpr int COLOR_CORAL = 203;
constexpr int COLOR_NAVY = 18;

int create_pair(int foreground, int background);
void set_color(int pair_num);
void set_colorw(WINDOW *win, int pair_num);
void remove_color(int pair_num);
void remove_colorw(WINDOW *win, int pair_num);
int find_color_approximation(int color256);

std::tuple<int, int> get_cursor_pos();

void set_cursor_block();
void set_cursor_line();
void set_cursor_underline();
void hide_cursor();
void show_cursor();

std::string get_line(std::string content, int nth);
int count_lines(std::string content);

void draw_rounded_box(WINDOW *win, int height, int width);

std::vector<std::string> split(std::string str, char delimiter);

bool is_word_separator(char c);
bool is_empty(char c);

int screen_to_buffer(int x, int y, Buffer &buffer);

extern int created_pairs;
