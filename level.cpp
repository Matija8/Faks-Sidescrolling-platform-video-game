#include "level.h"

#include <iostream>
#include <fstream>

auto level() -> Level
{
    Level l;

    std::ifstream f(".nivo.txt");

    if (!f)
        throw std::logic_error("TODO Message");

    // fscanf(f, "%d", &l.n); //prvo vidimo broj podataka
    f >> l.number_of_platforms;
    if (l.number_of_platforms < 2)
        throw std::logic_error("TODO Message");

    l.min_floor = 10000;
    l.max_floor = -100000; // TODO

    for (unsigned i = 0; i < l.number_of_platforms; i++)
    {
        double min_platform_x_coord,
            max_platform_x_coord,
            platform_y_coord;

        f >> min_platform_x_coord >>
            max_platform_x_coord >>
            platform_y_coord;

        // l.podaci[i * 3] = min_platform_x_coord;
        // l.podaci[i * 3 + 1] = max_platform_x_coord;
        // l.podaci[i * 3 + 2] = platform_y_coord;

        l.podaci.push_back({min_platform_x_coord, max_platform_x_coord, platform_y_coord});

        l.min_floor = std::min(platform_y_coord, l.min_floor);
        l.max_floor = std::max(platform_y_coord, l.max_floor);
    }

    return l;
}