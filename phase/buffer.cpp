/*
 buffer.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Buffer implementation
 Copyright (c) 2025 maxvdec
*/

#include "buffer.hpp"
#include <cassert>
#include <iostream>
#include <string>

void Buffer::expand_gap(size_t min_size) {
    size_t gap_size = gap_end - gap_start;
    if (gap_size >= min_size)
        return;

    size_t new_size = buffer.size() + min_size;
    std::vector<char> new_buffer(new_size);

    for (size_t i = 0; i < gap_start; ++i) {
        new_buffer[i] = buffer[i];
    }

    size_t after_gap_count = buffer.size() - gap_end;
    for (size_t i = 0; i < after_gap_count; ++i) {
        new_buffer[new_size - after_gap_count + i] = buffer[gap_end + i];
    }

    gap_end = new_size - after_gap_count;
    buffer = std::move(new_buffer);
}

Buffer::Buffer(size_t initial_size) {
    buffer.resize(initial_size, '\0');
    gap_start = 0;
    gap_end = initial_size;
}

size_t Buffer::size() const { return buffer.size() - (gap_end - gap_start); }

void Buffer::move_cursor(size_t pos) {
    assert(pos <= this->size());
    if (pos < gap_start) {
        size_t dist = gap_start - pos;
        for (size_t i = 0; i < dist; i++) {
            buffer[gap_end - 1 - i] = buffer[gap_start - 1 - i];
            buffer[gap_start - 1 - i] = '\0';
        }
        gap_start = pos;
        gap_end -= dist;
    } else if (pos > gap_start) {
        size_t dist = pos - gap_start;
        for (size_t i = 0; i < dist; i++) {
            buffer[gap_start + i] = buffer[gap_end + i];
            buffer[gap_end + i] = '\0';
        }
        gap_start += dist;
        gap_end += dist;
    }
}

void Buffer::insert(char c) {
    if (gap_start == gap_end) {
        expand_gap();
    }
    buffer[gap_start++] = c;
}

void Buffer::erase() {
    if (gap_start > 0) {
        gap_start--;
        buffer[gap_start] = '\0';
    }
}

void Buffer::print() const {
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gap_start && i < gap_end)
            continue; // skip gap
        std::cout << buffer[i];
    }
    std::cout << std::endl;
}

size_t Buffer::cursor_pos() const { return gap_start; }

std::string Buffer::contents() const {
    std::string content;
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gap_start && i < gap_end)
            continue; // skip gap
        content += buffer[i];
    }
    return content;
}
