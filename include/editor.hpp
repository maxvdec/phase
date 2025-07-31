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
#include <string>
#include <unordered_map>
#include <vector>

enum class Mode {
    Normal,
    Edit,
};

struct Action;

std::vector<Action> make_default_actions();

std::vector<Action> make_move_motions();

class Editor {
    Mode mode = Mode::Normal;
    std::vector<Action> motions;
    std::unordered_map<std::string, int> palettes;

  public:
    Buffer buffer;
    int editing_line = 0;
    int editing_x = 0;

    void draw_line_numbers();
    void start_with_file(std::filesystem::path file_path);
    void editor_flow();
    void change_mode();

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

extern int line_padding;

#endif
