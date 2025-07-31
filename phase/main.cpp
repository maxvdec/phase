
#include "editor.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale.h>
#include <ncurses.h>
#include <string>

int main() {
    // Initialize the screen
    initscr();
    setlocale(LC_ALL, "");
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    std::filesystem::path file_path =
        std::string(CMAKE_PATH) + "/tests/test1.txt";

    Editor editor;
    editor.start_with_file(file_path);
    editor.draw_line_numbers();
    editor.editor_flow();

    getch();

    endwin();

    editor.buffer.print();
    return 0;
}
