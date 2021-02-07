#pragma once

struct AppWindow
{
private:
    static constexpr unsigned DEFAULT_WIDTH = 1200, DEFAULT_HEIGHT = 800;

public:
    auto init_window() noexcept -> void;
    auto toggle_fullscreen() noexcept -> void;
    auto on_reshape(unsigned width, unsigned height) noexcept -> void;

private:
    auto setDimensions(unsigned width, unsigned height) noexcept -> void;

private:
    unsigned m_width = DEFAULT_WIDTH,
             m_height = DEFAULT_HEIGHT;
    bool m_is_fullscreen = false;
};
