#pragma once

#include <vector>

struct Platform
{
    double x_left,
        x_right,
        y;
};

struct Level
{
    // float *podaci;
    std::vector<Platform> podaci;
    unsigned number_of_platforms;
    double min_floor,
        max_floor;
};

auto level() -> Level;
