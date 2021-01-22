#include "level.h"

#include <iostream>
#include <fstream>
#include <string>

auto get_level(const std::string &file_with_level_data) -> Level
{
    Level level;

    std::ifstream f(file_with_level_data);

    if (!f)
        throw std::logic_error("TODO Message");

    f >> level.number_of_platforms;
    if (level.number_of_platforms < 2)
        throw std::logic_error("TODO Message");

    level.min_floor = 10000;
    level.max_floor = -100000; // TODO

    for (unsigned i = 0; i < level.number_of_platforms; i++)
    {
        double min_platform_x_coord,
            max_platform_x_coord,
            platform_y_coord;

        f >> min_platform_x_coord >>
            max_platform_x_coord >>
            platform_y_coord;

        level.podaci.push_back({min_platform_x_coord, max_platform_x_coord, platform_y_coord});

        level.min_floor = std::min(platform_y_coord, level.min_floor);
        level.max_floor = std::max(platform_y_coord, level.max_floor);
    }

    return level;
}