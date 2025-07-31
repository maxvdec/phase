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
#include <string>

enum class Mode {
    Normal,
    Edit,
};

class Editor {
    Mode mode = Mode::Normal;

  public:
    Buffer buffer;

    void draw_line_numbers();
    void start_with_file(std::filesystem::path file_path);
    void editor_flow();
    void change_mode();
};

#endif
