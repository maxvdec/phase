/*
 editor.hpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Editor rendering logic interface
 Copyright (c) 2025 maxvdec
*/

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "buffer.hpp"
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class Mode {
    Normal,
    Edit,
};

struct Action;
struct Command;

std::vector<Action> make_default_actions();

std::vector<Action> make_move_motions();
std::vector<Action> make_edit_motions();

std::vector<Command> make_commands();

class Editor {
    std::vector<Action> motions;
    std::vector<Command> commands;
    std::unordered_map<std::string, int> palettes;
    std::string current_command;

  public:
    Mode mode = Mode::Normal;
    std::optional<std::filesystem::path> file_path;
    Buffer buffer;
    int editing_line = 0;
    int editing_x = 0;
    std::string current_file;
    bool should_quit = false;

    void draw_line_numbers();
    void start_with_file(std::filesystem::path file_path);
    void editor_flow();
    void change_mode();
    void command_window();
    void draw_to_state_bar(std::string text, int palette = -1);

    Editor();
};

typedef std::function<void(Editor &)> EditorCommand;

struct ActionTrigger {
    int key;
    std::vector<Mode> modes;
};

struct Action {
    std::vector<ActionTrigger> keys;
    std::string description;
    EditorCommand command;
};

typedef std::function<void(Editor &, std::string)> ActionCommand;

struct Command {
    std::vector<std::string> name;
    std::string description;
    ActionCommand action;
};

extern int line_padding;

#endif
