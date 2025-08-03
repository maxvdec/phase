/*
 buffer.hpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Implementation of a Gap Buffer
 Copyright (c) 2025 maxvdec
*/

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

class Buffer {
  private:
    std::vector<char> buffer;
    size_t gap_start;
    size_t gap_end;

    void expand_gap(size_t min_size = 10);

  public:
    Buffer(size_t initial_size = 50);
    bool has_input = false;

    size_t size() const;
    void move_cursor(size_t pos);
    void insert(char c);
    void erase();
    void print() const;
    size_t cursor_pos() const;
    std::string contents() const;
    void replace(size_t pos, const std::string &str);
};

#endif
