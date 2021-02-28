#pragma once

constexpr double JUMP_SPEED = 0.2,
                 GRAVITY = 0.01,
                 DEFAULT_X_VELOCITY = 0.1,
                 TERMINAL_FALL_VELOCITY = 1.5,
                 LATERAL_ACCELERATION = 0.015,
                 LATERAL_DECCELERATION = 0.01,
                 MAX_LATERAL_PLAYER_SPEED = 0.13;

constexpr int TIMER_ID = 0;
constexpr unsigned TIMER_INTERVAL = 20;

const char* const BMP_WALL = "Assets/wall.bmp";
const char* const BMP_GRASS = "Assets/grass.bmp";
const char* const BMP_BACKGROUND = "Assets/background.bmp";
const char* const BMP_LAVA = "Assets/lava.bmp";
