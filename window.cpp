#include "window.h"

#include <GL/glut.h>

auto AppWindow::initWindow() noexcept -> void
{
    glutPositionWindow(300, 150);
    glutReshapeWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    toggleFullScreen();
}

auto AppWindow::toggleFullScreen() noexcept -> void
{
    const bool was_fullscreen = m_is_fullscreen;
    m_is_fullscreen = !m_is_fullscreen;

    if (!was_fullscreen)
    {
        glutFullScreen();
    }
    else
    {
        glutReshapeWindow(m_width, m_height);
    }
}

auto AppWindow::onReshape(unsigned width, unsigned height) -> void
{
    if (!m_is_fullscreen)
        setDimensions(width, height);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width / height, 1, 60);
    glMatrixMode(GL_MODELVIEW);
}

auto AppWindow::setDimensions(unsigned width, unsigned height) noexcept -> void
{
    m_width = width;
    m_height = height;
}
