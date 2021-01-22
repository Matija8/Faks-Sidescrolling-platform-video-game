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
};

auto get_platforms(const std::string &file_with_level_data) -> std::vector<Platform>;

auto lose_game() -> void;

auto win_game() -> void;

auto exit_game() -> void;
