#pragma once

#include "../model/player.h"
#include "../model/platforms.h"

auto on_display() -> void;

auto on_update(const double dt) -> void;

auto on_render() noexcept -> void;

auto on_timer(int value) -> void;

auto on_reshape(int width, int height) -> void;

auto on_keyboard(unsigned char key, int x, int y) -> void;

auto init_entities(Platforms &platforms, Player &player) -> void;
