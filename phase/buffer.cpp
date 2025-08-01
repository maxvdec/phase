/*
 buffer.cpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Buffer implementation with bug fixes
 Copyright (c) 2025 maxvdec
*/
#include "buffer.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

void Buffer::expand_gap(size_t min_size) {
    size_t gap_size = gap_end - gap_start;
    if (gap_size >= min_size)
        return;

    size_t new_gap_size = std::max(min_size, gap_size * 2 + 64);
    size_t new_size = buffer.size() + (new_gap_size - gap_size);
    std::vector<char> new_buffer(new_size);

    std::copy(buffer.begin(), buffer.begin() + gap_start, new_buffer.begin());

    size_t after_gap_count = buffer.size() - gap_end;
    std::copy(buffer.begin() + gap_end, buffer.end(),
              new_buffer.begin() + gap_start + new_gap_size);

    gap_end = gap_start + new_gap_size;
    buffer = std::move(new_buffer);
}

Buffer::Buffer(size_t initial_size) {
    buffer.resize(initial_size, '\0');
    gap_start = 0;
    gap_end = initial_size;
}

size_t Buffer::size() const { return buffer.size() - (gap_end - gap_start); }

void Buffer::move_cursor(size_t pos) {
    size_t logical_size = size();

    pos = std::min(pos, logical_size);

    if (pos < gap_start) {
        size_t dist = gap_start - pos;
        for (size_t i = 0; i < dist; i++) {
            buffer[gap_end - 1 - i] = buffer[gap_start - 1 - i];
        }
        gap_start = pos;
        gap_end -= dist;
    } else if (pos > gap_start) {
        size_t dist = pos - gap_start;
        size_t available_chars = logical_size - gap_start;
        dist = std::min(dist, available_chars);

        for (size_t i = 0; i < dist; i++) {
            buffer[gap_start + i] = buffer[gap_end + i];
        }
        gap_start += dist;
        gap_end += dist;
    }
}

void Buffer::insert(char c) {
    if (gap_start == gap_end) {
        expand_gap(64);
    }
    buffer[gap_start++] = c;
    has_input = true;
}

void Buffer::erase() {
    if (gap_start > 0) {
        gap_start--;
    }
}

void Buffer::print() const {
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gap_start && i < gap_end)
            continue;
        std::cout << buffer[i];
    }
    std::cout << std::endl;
}

size_t Buffer::cursor_pos() const { return gap_start; }

std::string Buffer::contents() const {
    std::string content;
    content.reserve(size());

    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gap_start && i < gap_end)
            continue;
        content += buffer[i];
    }
    return content;
}
