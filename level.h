#pragma once

#include <vector>
#include <string>

struct Platform
{
    double x_left,
        x_right,
        y;
};

struct Level
{
    std::vector<Platform> podaci;
    double min_floor,
        max_floor;
};

auto get_level(const std::string &file_with_level_data) -> Level;

auto lose_game() -> void;

auto win_game() -> void;

auto exit_game() -> void;
