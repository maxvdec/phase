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

std::vector<Motion> make_default_motions() {
    std::vector<Motion> motions;
    motions.reserve(2);

    Motion move_up = {
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

    Motion move_down = {
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

    Motion move_left = {
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

    Motion move_right = {
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

    Motion move_end_of_line = {
        .keys =
            {
                {'e', {Mode::Normal}},
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
    motions.push_back(move_left);
    motions.push_back(move_right);
    motions.push_back(move_up);
    motions.push_back(move_down);
    motions.push_back(move_end_of_line);
    return motions;
}
