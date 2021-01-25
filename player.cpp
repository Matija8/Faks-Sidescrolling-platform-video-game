#include "player.h"

#include <iostream>

auto Player::move_left() -> void
{
    // x_acc = -LATERAL_ACCELERATION;
    // is_stopping = false;
    x_velocity = -DEFAULT_X_VELOCITY;
}

auto Player::move_right() -> void
{
    // x_acc = LATERAL_ACCELERATION;
    // is_stopping = false;
    x_velocity = DEFAULT_X_VELOCITY;
}

auto Player::stop_x() -> void
{
    // if (x_velocity == 0)
    //     return;
    // x_acc = x_velocity > 0 ? -LATERAL_DECCELERATION : LATERAL_DECCELERATION;
    // is_stopping = true;
    x_velocity = 0;
}

auto Player::jump() -> void
{
    if (is_in_air)
        return;

    y_velocity = JUMP_SPEED;
    is_in_air = true;
}

auto Player::move(const Platforms &platforms, const double dt) -> void
{
    move_on_x_axis(platforms, dt);
    move_on_y_axis(platforms, dt);
}

auto Player::move_on_x_axis(const Platforms &platforms, const double dt) -> void
{

    // const double old_x_velocity = x_velocity;
    // x_velocity += x_acc * dt;
    // if (old_x_velocity * x_velocity < 0 && is_stopping)
    // {
    //     x_velocity = 0;
    //     x_acc = 0;
    // }

    // x_velocity = std::min(x_velocity, MAX_LATERAL_PLAYER_SPEED);
    // x_velocity = std::max(x_velocity, -MAX_LATERAL_PLAYER_SPEED);

    const double x_coord_old = x_coord;
    x_coord += x_velocity * dt;

    bool is_colliding = false;
    for (auto &p : platforms)
    {
        if (x_coord + PLAYER_EDGE > p.x_left && x_coord - PLAYER_EDGE < p.x_right && p.y < y_coord + PLAYER_HEIGHT && p.y > y_coord)
        {
            is_colliding = true;
            break;
        }
    }

    if (is_colliding)
        x_coord = x_coord_old;
}

auto Player::move_on_y_axis(const Platforms &platforms, const double dt) -> void
{
    bool is_above_platform = false;
    double highest_platform_below_player_y = -10000;
    for (auto &p : platforms)
    {
        if (x_coord + PLAYER_EDGE >= p.x_left && x_coord - PLAYER_EDGE <= p.x_right && y_coord >= p.y)
        {
            is_above_platform = true;
            highest_platform_below_player_y = std::max(highest_platform_below_player_y, p.y);
        }
    }

    y_velocity -= GRAVITY * dt;
    y_velocity = std::max(y_velocity, -TERMINAL_FALL_VELOCITY);

    y_coord += y_velocity * dt;

    const bool is_on_a_platform = is_above_platform && y_coord <= highest_platform_below_player_y;
    if (is_on_a_platform)
    {
        y_coord = highest_platform_below_player_y;
        y_velocity = 0;
    }
    is_in_air = !is_on_a_platform;
}
