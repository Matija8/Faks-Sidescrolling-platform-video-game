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
    // float *podaci;
    std::vector<Platform> podaci;
    unsigned number_of_platforms;
    double min_floor,
        max_floor;
};

auto get_level(const std::string &file_with_level_data) -> Level;
