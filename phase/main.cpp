
#include "editor.hpp"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

int main() {
    // Initialize the screen
    initscr();
    start_color();
    use_default_colors();

    Editor editor;
    editor.draw_line_numbers();

    getch();

    endwin();
    return 0;
}
