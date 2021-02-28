#pragma once

#include "../model/platforms.h"

auto renderPlatform(
    unsigned platform_texture,
    Platform platform) -> void;

auto renderFinishSign(
    unsigned texture,
    Platform platform) -> void;

auto renderBackground(
    unsigned background_texture,
    unsigned lava_texture,
    float min_x,
    float max_x,
    float min_y,
    float max_y,
    float far_z,
    float near_z) -> void;

auto renderPlayer() -> void;
