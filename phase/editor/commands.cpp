/*
 commands.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Commands for doing various things
 Copyright (c) 2025 maxvdec
*/

#include "editor.hpp"
#include "lib.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

void Editor::command_window() {
    int h = 4, w = 40;
    int start_y = (LINES - h) / 2;
    int start_x = (COLS - w) / 2;

    WINDOW *modal = newwin(h, w, start_y, start_x);
    keypad(modal, true);
    box(modal, 0, 0);
    // draw_rounded_box(modal, h, w);
    set_cursor_line();
    wattron(modal, A_BOLD);
    mvwprintw(modal, 1, 2, "Command");
    wattroff(modal, A_BOLD);
    int palette = create_pair(COLOR_BRIGHT_RED, COLOR_DEFAULT);
    set_colorw(modal, palette);
    wattron(modal, A_ITALIC);
    mvwprintw(modal, 2, 2, "%s", current_command.c_str());
    wmove(modal, 2, 2 + current_command.length());
    wrefresh(modal);

    int line_padding = 2;

    int ch;

    while ((ch = wgetch(modal))) {
        if (ch == '\n')
            break;
        if (ch == 27) {
            current_command.clear();
            break;
        }
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (!current_command.empty()) {
                current_command.pop_back();
                int y, x;
                getyx(modal, y, x);
                mvwprintw(modal, y, line_padding + current_command.length(),
                          " ");
            }
        } else if (ch >= 32 && ch <= 126) {
            current_command.push_back(static_cast<char>(ch));
        }
        mvwprintw(modal, 2, 2, "%s", current_command.c_str());
        move(2, line_padding + current_command.length());
        wrefresh(modal);
    }
    wattroff(modal, A_ITALIC);
    remove_colorw(modal, palette);

    werase(modal);
    wrefresh(modal);
    delwin(modal);

    clear();
    refresh();
}

std::vector<Command> make_commands() {
    std::vector<Command> actions;
    Command save = {
        .name = {"w", "s", "save", "write"},
        .description = "Save the current file",
        .action = [](Editor &editor, std::string _) {
            std::string saved_path;

            if (editor.file_path.has_value()) {
                std::ofstream file(editor.file_path.value());
                if (file.is_open()) {
                    file << editor.buffer.contents();
                    file.close();
                } else {
                    mvprintw(LINES - 1, 0,
                             "Error: Could not open file for saving.");
                }

                try {
                    auto cwd = std::filesystem::current_path();
                    saved_path =
                        std::filesystem::relative(editor.file_path.value(), cwd)
                            .string();
                } catch (...) {
                    saved_path = editor.file_path.value().string();
                }
            } else {
                std::string new_file_name =
                    modal_input("Enter the path where to save the file.", "");
                if (!new_file_name.empty()) {
                    editor.file_path = new_file_name;
                    std::ofstream file(new_file_name);
                    if (file.is_open()) {
                        file << editor.buffer.contents();
                        file.close();
                    } else {
                        mvprintw(LINES - 1, 0,
                                 "Error: Could not open file for saving.");
                    }

                    try {
                        auto cwd = std::filesystem::current_path();
                        saved_path =
                            std::filesystem::relative(new_file_name, cwd)
                                .string();
                    } catch (...) {
                        saved_path = new_file_name;
                    }
                }
            }

            std::string size_str =
                std::to_string(editor.buffer.contents().length());
            std::string bytes_str =
                std::to_string(editor.buffer.contents().size());
            editor.buffer.has_input = false;
            editor.draw_to_state_bar("Saved " + size_str + " characters to " +
                                     saved_path + " (" + bytes_str +
                                     " bytes).");
        }};

    Command quit = {
        .name = {"q"},
        .description = "Quit the editor",
        .action = [](Editor &editor, std::string _) {
            if (editor.buffer.has_input) {
                std::string response = modal_input(
                    "Unsaved changes",
                    "Are you sure you "
                    "want to quit? Changes will not be saved. (y/n)",
                    "");
                if (response != "y" && response != "Y") {
                    return;
                }
            }
            endwin();
            exit(0);
        }};

    Command quit_force = {.name = {"qq"},
                          .description =
                              "Quit the editor and discard all changes",
                          .action = [](Editor &editor, std::string _) {
                              endwin();
                              exit(0);
                          }};

    actions.push_back(quit);
    actions.push_back(save);
    actions.push_back(quit_force);
    return actions;
}
