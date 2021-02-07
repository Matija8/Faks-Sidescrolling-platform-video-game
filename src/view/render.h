#pragma once

#include "../model/platforms.h"

auto renderPlatform(
    unsigned platform_texture_index,
    Platform platform) -> void;

auto renderFinishSign(
    unsigned texture_number,
    Platform platform) -> void;

auto renderBackground(
    unsigned name1,
    unsigned name2,
    float min_x,
    float max_x,
    float min_y,
    float max_y,
    float far_z,
    float near_z) -> void;

auto renderPlayer() -> void;
