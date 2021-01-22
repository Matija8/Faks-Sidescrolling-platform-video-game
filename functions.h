#pragma once

#include "level.h"

auto funcMakeBlock(
    GLuint names,
    Platform platform) -> void;

auto funcMakeFinishSign(
    GLuint name,
    Platform platform) -> void;

auto funcMakeBackground(
    GLuint name1,
    GLuint name2,
    float min_x,
    float max_x,
    float min_y,
    float max_y,
    float far_z,
    float near_z) -> void;

auto funcMakePlayer(void) -> void;
