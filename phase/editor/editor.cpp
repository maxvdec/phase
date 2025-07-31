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
    printw(" | Row: %d | Col: %d | Line: %d ", y + 1, x + 1, editing_line + 1);
    move(y, x); // Move cursor back to its position
    refresh();
}

void Editor::editor_flow() {
    std::string buffer_contents = buffer.contents();
    int length = get_line(buffer_contents, editing_line).length();
    move(0, length + line_padding);
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
                auto [x, y] = get_cursor_pos();
                if (x > line_padding) {
                    move(y, x - 1);
                } else if (y > 0) {
                    buffer_contents = buffer.contents();
                    std::string last_line =
                        get_line(buffer_contents, editing_line - 1);
                    move(y - 1, last_line.length() + line_padding);
                    editing_line--;
                }
            }
        } else if (ch == KEY_RESIZE) {
            clear();
            draw_line_numbers();
        } else if (ch == '\n') {
            buffer.insert('\n');
            auto [x, y] = get_cursor_pos();
            move(y + 1, line_padding);
            editing_line++;
        } else if (mode == Mode::Edit) {
            buffer.insert(static_cast<char>(ch));
            auto [x, y] = get_cursor_pos();
            move(y, x + 1);
        }
        refresh();
        this->draw_line_numbers();
    }
}

Editor::Editor() { motions = make_default_motions(); }
