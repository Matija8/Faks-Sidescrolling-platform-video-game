#pragma once

#include <string>
#include <vector>

struct Platform
{
    double x_left,
        x_right,
        y;
};

using Platforms = std::vector<Platform>;

auto get_platforms(const std::string& file_with_level_data) -> std::vector<Platform>;

auto get_min_y_val_from_platforms(const std::vector<Platform>& platforms) -> double;

auto get_max_y_val_from_platforms(const std::vector<Platform>& platforms) -> double;

auto get_y_vals_from_platforms(
    const std::vector<Platform>& platforms) -> std::vector<double>;

// TODO: Move to game.

auto lose_game() -> void;

auto win_game() -> void;

auto exit_game() -> void;
