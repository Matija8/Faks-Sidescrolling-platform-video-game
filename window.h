#pragma once

struct AppWindow
{
private:
    static constexpr unsigned DEFAULT_WIDTH = 1200, DEFAULT_HEIGHT = 800;

public:
    auto initWindow() noexcept -> void;
    auto toggleFullScreen() noexcept -> void;
    auto onReshape(unsigned width, unsigned height) -> void;

private:
    auto setDimensions(unsigned width, unsigned height) noexcept -> void;

private:
    unsigned m_width = DEFAULT_WIDTH,
             m_height = DEFAULT_HEIGHT;
    bool m_is_fullscreen = false;
};
