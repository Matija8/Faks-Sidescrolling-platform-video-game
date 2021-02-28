#pragma once

#include "./platforms.h" // TODO: Remove knowledge of platforms from player.h.
#include "constants.h"

constexpr double PLAYER_EDGE = 0.2,
                 PLAYER_HEIGHT = 1.8;

struct Player
{
    auto move_left() -> void;

    auto move_right() -> void;

    auto stop_x() -> void;

    auto jump() -> void;

    auto move(const Platforms& platforms, const double dt) -> void;

    auto move_on_x_axis(const Platforms& platforms, const double dt) -> void;

    auto move_on_y_axis(const Platforms& platforms, const double dt) -> void;

public:
    double x_coord = 0,
           x_velocity = 0,
           x_acc = 0,
           y_coord = 0,
           y_velocity = 0;

    bool is_in_air = false,
         is_stopping = true;
};
