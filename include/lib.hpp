/*
 lib.hpp
 As part of the Phase project
 Created by Max Van den Eynde in 2025
 --------------------------------------------------
 Description: Simple API for managing UI and the edtior
 Copyright (c) 2025 maxvdec
*/

#ifndef LIB_HPP
#define LIB_HPP

#include <string>

std::string modal_input(std::string title, std::string description,
                        std::string default_value = "");
int modal_char(std::string title, std::string description,
               int default_value = 0);

#endif // LIB_HPP
