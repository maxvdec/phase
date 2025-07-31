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
#include <vector>

enum class Mode {
    Normal,
    Edit,
};

struct Motion;

std::vector<Motion> make_default_motions();

class Editor {
    Mode mode = Mode::Normal;
    std::vector<Motion> motions;

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

struct MotionTrigger {
    int key;
    std::vector<Mode> modes;
};

struct Motion {
    std::vector<MotionTrigger> keys;
    std::string description;
    EditorCommand command;
};

extern int line_padding;

#endif
