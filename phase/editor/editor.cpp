/*
 editor.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Main editor functionality
 Copyright (c) 2025 maxvdec
*/

#include "editor.hpp"
#include "buffer.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

void Editor::start_with_file(std::filesystem::path file_path) {
    if (!std::filesystem::exists(file_path)) {
        printw("File does not exist: %s\n", file_path.string().c_str());
        return;
    }

    std::ifstream file(file_path);
    if (!file.is_open()) {
        printw("Failed to open file: %s\n", file_path.string().c_str());
        return;
    }

    std::string contents = std::string(std::istreambuf_iterator<char>(file),
                                       std::istreambuf_iterator<char>());
    Buffer buffer = Buffer(contents.size() + 1);
    for (char c : contents) {
        buffer.insert(c);
    }

    this->buffer = std::move(buffer);

    file.close();
}

void Editor::change_mode() {
    if (mode == Mode::Normal) {
        set_cursor_block();
    } else {
        set_cursor_line();
    }
    auto [x, y] = get_cursor_pos();
    mvprintw(LINES - 1, 0, "Mode:");
    attron(A_BOLD);
    int palette;
    if (mode == Mode::Normal) {
        palette = create_pair(COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
        set_color(palette);
        printw(" Normal ");
        remove_color(palette);
    } else {
        palette = create_pair(COLOR_BRIGHT_RED, COLOR_DEFAULT);
        set_color(palette);
        printw(" Edit   ");
        remove_color(palette);
    }
    attroff(A_BOLD);
    printw(" | Row: %d | Col: %d ", y + 1, x + 1);
    move(y, x); // Move cursor back to its position
    refresh();
}

void Editor::editor_flow() {
    std::string buffer_contents = buffer.contents();
    std::string first_line =
        buffer_contents.substr(0, buffer_contents.find('\n'));
    int length = first_line.length();
    move(0, length + 2);
    buffer.move_cursor(length);
    change_mode(); // Initial mode display
    while (true) {
        int ch = getch();
        if (ch == 'q' && mode == Mode::Normal) {
            break;
        } else if (ch == 27) {
            if (mode == Mode::Normal) {
                mode = Mode::Edit;
                change_mode();
            } else {
                mode = Mode::Normal;
                change_mode();
            }
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (mode == Mode::Edit && !buffer.contents().empty()) {
                buffer.erase();
            }
        } else if (ch == KEY_RESIZE) {
            clear();
            draw_line_numbers();
        } else if (mode == Mode::Edit) {
            buffer.insert(static_cast<char>(ch));
        }
        this->draw_line_numbers();
    }
}
