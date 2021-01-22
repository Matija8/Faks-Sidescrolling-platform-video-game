#pragma once

#include "player.h"
#include "level.h"

auto on_display() -> void;
auto on_update() -> void;
auto platform_checking() -> void;
auto on_timer(int value) -> void;
auto on_reshape(int width, int height) -> void;
auto on_keyboard(unsigned char key, int x, int y) -> void;

auto init_entities(Platforms &platforms, Player &player) -> void;
