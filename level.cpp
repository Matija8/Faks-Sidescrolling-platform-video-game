#include "level.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "error_handling.h"

auto get_platforms(const std::string &file_with_level_data) -> std::vector<Platform>
{
    std::vector<Platform> platforms;

    std::ifstream f(file_with_level_data);

    if (!f)
        throw std::logic_error("TODO Message");

    size_t number_of_platforms;
    f >> number_of_platforms;
    if (number_of_platforms < 2)
        throw std::logic_error("TODO Message");

    for (size_t i = 0; i < number_of_platforms; i++)
    {
        double min_platform_x_coord,
            max_platform_x_coord,
            platform_y_coord;

        f >> min_platform_x_coord >>
            max_platform_x_coord >>
            platform_y_coord;

        platforms.push_back({min_platform_x_coord, max_platform_x_coord, platform_y_coord});
    }

    return platforms;
}

auto get_min_y_val_from_platforms(const Platforms &platforms) -> double
{
    assertIsTrueElseThrow(platforms.size() > 0);
    const std::vector<double> platform_y_vals = get_y_vals_from_platforms(platforms);
    return *(std::min_element(platform_y_vals.begin(), platform_y_vals.end()));
}

auto get_max_y_val_from_platforms(const Platforms &platforms) -> double
{
    assertIsTrueElseThrow(platforms.size() > 0);
    const std::vector<double> platform_y_vals = get_y_vals_from_platforms(platforms);
    return *(std::max_element(platform_y_vals.begin(), platform_y_vals.end()));
}

auto get_y_vals_from_platforms(
    const Platforms &platforms) -> std::vector<double>
{
    std::vector<double> platform_y_vals(platforms.size());
    std::transform(
        platforms.begin(), platforms.end(),
        platform_y_vals.begin(),
        [](const Platform &platform) noexcept -> double {
            return platform.y;
        });

    return platform_y_vals;
}

auto lose_game() -> void
{
    std::cout << "GAME OVER! Play again?\n";
    exit_game();
}

auto win_game() -> void
{
    std::cout << "You WIN!\n";
    exit_game();
}

auto exit_game() -> void
{
    exit(EXIT_SUCCESS);
}
