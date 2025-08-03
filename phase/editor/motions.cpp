/*
 motions.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Default motion implementation
 Copyright (c) 2025 maxvdec
*/

#include "editor.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

std::vector<Action> make_default_actions() {
    std::vector<Action> actions;
    std::vector<Action> move_actions = make_move_motions();
    actions.insert(actions.end(), move_actions.begin(), move_actions.end());
    std::vector<Action> edit_actions = make_edit_motions();
    actions.insert(actions.end(), edit_actions.begin(), edit_actions.end());
    std::vector<Action> search_actions = make_search_motions();
    actions.insert(actions.end(), search_actions.begin(), search_actions.end());
    return actions;
}

std::vector<Action> make_move_motions() {
    std::vector<Action> motions;
    motions.reserve(10);

    Action move_up = {
        .keys =
            {
                {'w', {Mode::Normal}},
                {KEY_UP, {Mode::Edit, Mode::Normal}},
            },
        .description = "Move cursor up",
        .command = [](Editor &editor) {
            auto [x, y] = get_cursor_pos();
            if (y > 0 && editor.editing_line > 0) {
                move(y - 1, x);
                editor.editing_line--;
                std::string buffer_contents = editor.buffer.contents();
                std::string current_line =
                    get_line(buffer_contents, editor.editing_line);
                int current_line_length = current_line.length();
                int new_x = std::min(editor.editing_x, current_line_length);

                int buffer_pos = 0;
                for (int i = 0; i < editor.editing_line; i++) {
                    std::string line = get_line(buffer_contents, i);
                    buffer_pos += line.length() + 1;
                }
                buffer_pos += new_x;

                editor.buffer.move_cursor(buffer_pos);
                editor.editing_x = new_x;
                move(y - 1, new_x + line_padding);
            }
            refresh();
        }};

    Action move_down = {
        .keys =
            {
                {'s', {Mode::Normal}},
                {KEY_DOWN, {Mode::Edit, Mode::Normal}},
            },
        .description = "Move cursor down",
        .command = [](Editor &editor) {
            auto [x, y] = get_cursor_pos();
            std::string buffer_contents = editor.buffer.contents();
            std::string next_line =
                get_line(buffer_contents, editor.editing_line + 1);

            if (!next_line.empty() ||
                editor.editing_line + 1 < count_lines(buffer_contents)) {
                editor.editing_line++;
                int next_line_length = next_line.length();
                int new_x = std::min(editor.editing_x, next_line_length);

                int buffer_pos = 0;
                for (int i = 0; i < editor.editing_line; i++) {
                    std::string line = get_line(buffer_contents, i);
                    buffer_pos += line.length() + 1;
                }
                buffer_pos += new_x;

                if (buffer_pos > editor.buffer.size()) {
                    buffer_pos = editor.buffer.size();
                }

                editor.buffer.move_cursor(buffer_pos);
                editor.editing_x = new_x;
                move(y + 1, new_x + line_padding);
            }
            refresh();
        }};

    Action move_left = {
        .keys =
            {
                {'a', {Mode::Normal}},
                {KEY_LEFT, {Mode::Edit, Mode::Normal}},
            },
        .description = "Move cursor left",
        .command = [](Editor &editor) {
            auto [x, y] = get_cursor_pos();

            if (editor.editing_x > 0) {
                editor.editing_x--;
                move(y, editor.editing_x + line_padding);
                editor.buffer.move_cursor(editor.buffer.cursor_pos() - 1);
            } else if (editor.editing_line > 0) {
                editor.editing_line--;
                std::string buffer_contents = editor.buffer.contents();
                std::string prev_line =
                    get_line(buffer_contents, editor.editing_line);
                editor.editing_x = prev_line.length();

                int buffer_pos = 0;
                for (int i = 0; i < editor.editing_line; i++) {
                    std::string line = get_line(buffer_contents, i);
                    buffer_pos += line.length() + 1;
                }
                buffer_pos += editor.editing_x;

                editor.buffer.move_cursor(buffer_pos);
                move(y - 1, editor.editing_x + line_padding);
            }
            refresh();
        }};

    Action move_right = {
        .keys =
            {
                {'d', {Mode::Normal}},
                {KEY_RIGHT, {Mode::Edit, Mode::Normal}},
            },
        .description = "Move cursor right",
        .command = [](Editor &editor) {
            auto [x, y] = get_cursor_pos();
            std::string buffer_contents = editor.buffer.contents();
            std::string current_line =
                get_line(buffer_contents, editor.editing_line);

            if (editor.editing_x < current_line.length()) {
                editor.editing_x++;
                move(y, editor.editing_x + line_padding);
                editor.buffer.move_cursor(editor.buffer.cursor_pos() + 1);
            } else {
                std::string next_line =
                    get_line(buffer_contents, editor.editing_line + 1);
                if (!next_line.empty() ||
                    editor.editing_line + 1 < count_lines(buffer_contents)) {
                    editor.editing_line++;
                    editor.editing_x = 0;

                    int buffer_pos = 0;
                    for (int i = 0; i < editor.editing_line; i++) {
                        std::string line = get_line(buffer_contents, i);
                        buffer_pos += line.length() + 1;
                    }

                    editor.buffer.move_cursor(buffer_pos);
                    move(y + 1, line_padding);
                }
            }
            refresh();
        }};

    Action move_end_of_line = {
        .keys =
            {
                {'r', {Mode::Normal}},
            },
        .description = "Move cursor to end of line",
        .command = [](Editor &editor) {
            auto [x, y] = get_cursor_pos();
            std::string buffer_contents = editor.buffer.contents();
            std::string current_line =
                get_line(buffer_contents, editor.editing_line);
            editor.editing_x = current_line.length();

            int buffer_pos = 0;
            for (int i = 0; i < editor.editing_line; i++) {
                std::string line = get_line(buffer_contents, i);
                buffer_pos += line.length() + 1;
            }
            buffer_pos += editor.editing_x;

            editor.buffer.move_cursor(buffer_pos);
            move(y, editor.editing_x + line_padding);
            refresh();
        }};

    Action move_begin_of_line = {
        .keys =
            {
                {'g', {Mode::Normal}},
            },
        .description = "Move cursor to end of line",
        .command = [](Editor &editor) {
            auto [x, y] = get_cursor_pos();
            std::string buffer_contents = editor.buffer.contents();
            std::string current_line =
                get_line(buffer_contents, editor.editing_line);
            editor.editing_x = 0;

            int buffer_pos = 0;
            for (int i = 0; i < editor.editing_line; i++) {
                std::string line = get_line(buffer_contents, i);
                buffer_pos += line.length() + 1;
            }

            editor.buffer.move_cursor(buffer_pos);
            move(y, editor.editing_x + line_padding);
            refresh();
        }};
    Action move_word = {.keys =
                            {
                                {'f', {Mode::Normal}},
                            },
                        .description = "Move to the beggining of the next word",
                        .command = [](Editor &editor) {
                            auto [x, y, _] = editor.get_next_word();
                            editor.editing_x = x;
                            editor.editing_line = y;
                            int buffer_pos =
                                screen_to_buffer(x, y, editor.buffer);
                            if (buffer_pos < 0) {
                                return;
                            }
                            editor.buffer.move_cursor(buffer_pos);
                            move(y, x + line_padding);
                            refresh();
                        }};
    Action move_end_word = {
        .keys =
            {
                {'e', {Mode::Normal}},
            },
        .description = "Move to the end of the next word",
        .command = [](Editor &editor) {
            auto [start_x, target_y, _] = editor.get_next_word();

            const std::string line =
                get_line(editor.buffer.contents(), target_y);

            if (start_x >= line.length() || line.empty()) {
                editor.editing_x = start_x;
                editor.editing_line = target_y;
                int buffer_pos =
                    screen_to_buffer(start_x, target_y, editor.buffer);
                if (buffer_pos >= 0) {
                    editor.buffer.move_cursor(buffer_pos);
                    move(target_y, start_x + line_padding);
                }
                refresh();
                return;
            }

            int end_x = start_x;

            if (!is_word_separator(line[end_x])) {
                while (end_x < line.length() &&
                       !is_word_separator(line[end_x])) {
                    end_x++;
                }
                if (end_x > start_x) {
                    end_x--;
                }
            }

            editor.editing_x = end_x;
            editor.editing_line = target_y;

            int buffer_pos = screen_to_buffer(end_x, target_y, editor.buffer);
            if (buffer_pos < 0) {
                return;
            }

            editor.buffer.move_cursor(buffer_pos);
            move(target_y, end_x + line_padding);
            refresh();
        }};
    motions.push_back(move_left);
    motions.push_back(move_right);
    motions.push_back(move_up);
    motions.push_back(move_down);
    motions.push_back(move_end_of_line);
    motions.push_back(move_begin_of_line);
    motions.push_back(move_word);
    motions.push_back(move_end_word);
    return motions;
}

std::vector<Action> make_edit_motions() {
    std::vector<Action> motions;
    motions.reserve(13);

    Action break_line = {
        .keys =
            {
                {'k', {Mode::Normal}},
            },
        .description = "Create a new line under the current line",
        .command = [](Editor &editor) {
            std::string buffer_contents = editor.buffer.contents();
            std::string current_line =
                get_line(buffer_contents, editor.editing_line);

            int buffer_pos = 0;
            for (int i = 0; i <= editor.editing_line; i++) {
                std::string line = get_line(buffer_contents, i);
                buffer_pos += line.length();
                if (i != editor.editing_line) {
                    buffer_pos += 1;
                }
            }

            editor.buffer.move_cursor(buffer_pos);
            editor.buffer.insert('\n');

            editor.editing_line += 1;
            editor.editing_x = 0;
            move(editor.editing_line, line_padding);
            editor.mode = Mode::Edit;
            editor.change_mode();
            refresh();
        }};
    Action toggle_case = {
        .keys =
            {
                {'u', {Mode::Normal}},
            },
        .description = "Toggle case of the character under the cursor",
        .command = [](Editor &editor) {
            std::string buffer_contents = editor.buffer.contents();
            if (editor.editing_x < 0 ||
                editor.editing_line >= count_lines(buffer_contents)) {
                return;
            }
            std::string current_line =
                get_line(buffer_contents, editor.editing_line);
            if (editor.editing_x >= current_line.length()) {
                return;
            }
            char &ch = current_line[editor.editing_x];
            if (isupper(ch)) {
                ch = tolower(ch);
            } else if (islower(ch)) {
                ch = toupper(ch);
            }
            editor.buffer.replace(editor.buffer.cursor_pos(),
                                  std::string(1, ch));
            move(editor.editing_line, editor.editing_x + line_padding);
            refresh();
        }};

    motions.push_back(break_line);
    motions.push_back(toggle_case);
    return motions;
}

std::vector<Action> make_search_motions() {
    std::vector<Action> motions;
    motions.reserve(5);
    Action create_mark = {
        .keys =
            {
                {'m', {Mode::Normal}},
            },
        .description = "Place a mark at the current cursor position",
        .command = [](Editor &editor) {
            for (int i = 0; i < editor.marks.size(); i++) {
                if (editor.marks[i].editor_x == editor.editing_x &&
                    editor.marks[i].editor_y == editor.editing_line) {
                    editor.draw_to_state_bar("Removed mark " +
                                             std::to_string(i) +
                                             " at current position.");
                    editor.marks.erase(editor.marks.begin() + i);
                    return;
                }
            }
            editor.marks.push_back(Mark{editor.editing_x, editor.editing_line});
            editor.draw_to_state_bar("Marked current position as mark " +
                                     std::to_string(editor.marks.size() - 1) +
                                     ".");
        }};
    Action mark_previous = {
        .keys =
            {
                {',', {Mode::Normal}},
            },
        .description = "Go to the previous mark",
        .command = [](Editor &editor) {
            if (editor.marks.empty()) {
                editor.draw_to_state_bar("No marks set.",
                                         editor.palettes["edit"]);
                return;
            }
            if (editor.current_mark < 0) {
                editor.current_mark = editor.marks.size() - 1;
            } else {
                editor.current_mark--;
            }
            if (editor.current_mark > editor.marks.size() - 1) {
                editor.current_mark = 0;
            } else if (editor.current_mark < 0) {
                editor.current_mark = editor.marks.size() - 1;
            }
            Mark &mark = editor.marks[editor.current_mark];
            editor.editing_x = mark.editor_x;
            editor.editing_line = mark.editor_y;
        }};
    Action mark_forward = {
        .keys =
            {
                {'.', {Mode::Normal}},
            },
        .description = "Go to the next mark",
        .command = [](Editor &editor) {
            if (editor.marks.empty()) {
                editor.draw_to_state_bar("No marks set.",
                                         editor.palettes["edit"]);
                return;
            }
            if (editor.current_mark < 0) {
                editor.current_mark = 0;
            } else {
                editor.current_mark++;
            }
            if (editor.current_mark > editor.marks.size() - 1) {
                editor.current_mark = editor.marks.size() - 1;
            } else if (editor.current_mark < 0) {
                editor.current_mark = 0;
            }
            Mark &mark = editor.marks[editor.current_mark];
            editor.editing_x = mark.editor_x;
            editor.editing_line = mark.editor_y;
        }};
    motions.push_back(create_mark);
    motions.push_back(mark_previous);
    motions.push_back(mark_forward);
    return motions;
}
