#include "player.h"

#include <iostream>

extern double current_floor_y;

extern double time_in_air;

auto Player::move_left() -> void
{
    x_velocity = -DEFAULT_X_VELOCITY;
}

auto Player::move_right() -> void
{
    x_velocity = DEFAULT_X_VELOCITY;
}

auto Player::stop_x() -> void
{
    x_velocity = 0;
}

auto Player::jump() -> void
{

    if (!is_falling)
    {
        is_jumping = true;
        time_in_air = 0;
    }
}

auto Player::move_on_y_axis(const double dt) -> void
{

    if (!is_jumping && (!is_above_platform || y_coord > current_floor_y)) // TODO: have a margin of error for floor hover.
    {

        is_falling = true;
        time_in_air += dt / 5; // scale speeds
        y_velocity = GRAVITY * time_in_air;
        // p.y_velocity += GRAVITY * dt / 5;

        y_velocity = std::min(y_velocity, TERMINAL_Y_VELOCITY);

        y_coord -= y_velocity * dt;
    }
    else
    {
        is_falling = false;
        time_in_air = 0;
    }

    // TODO: Get delta time and use it for moving.

    if (is_jumping && !is_falling)
    {
        time_in_air += dt / 5; // This is time spent falling!
        y_velocity -= GRAVITY * time_in_air;
        y_coord += y_velocity * time_in_air;
        if (y_velocity <= 0)
        {
            is_falling = true;
            is_jumping = false;
            time_in_air = 0;
        }
    }
    else
    {
        if (is_above_platform && y_coord <= current_floor_y)
        {
            is_falling = false;
            is_jumping = false;
            time_in_air = 0;
            y_coord = current_floor_y;
            y_velocity = JUMP_SPEED;
        }
    }

    // debug_log << p.y_velocity << "\n";
}