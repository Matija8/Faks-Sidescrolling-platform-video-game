#pragma once

#include "constants.h"

struct Player
{
    auto move_left() -> void;

    auto move_right() -> void;

    auto stop_x() -> void;

    auto jump() -> void;

    auto move_on_y_axis(const double dt = 1) -> void;

public:
    double x_coord = 0,
           x_velocity = 0,
           y_coord,
           y_velocity = JUMP_SPEED; // TODO

    bool is_jumping = false,
         is_moving = false,
         is_falling = false,
         is_above_platform = true;
};
