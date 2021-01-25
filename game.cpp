#include "game.h"

#include <iostream>

#include <GL/glut.h>

#include "constants.h"
#include "error_handling.h"
#include "functions.h"
#include "level.h"
#include "player.h"
#include "textures.h"
#include "window.h"

// TODO: Create main.cpp file. Move all to src folder.

GLuint names[4]; // Teksture.
static float lava_floor;
static bool animation_ongoing = true;

struct Camera
{
    double x = 0;
    double y = 0.3;
};

static Player player;
static Camera camera;
static AppWindow window;
static Platforms platforms;

auto &debug_log = std::cout;

// TODO: Pull out main into seperate file (main.cpp).

auto main(int argc, char **argv) -> int
{
    init_entities(platforms, player);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Game Running");

    window.init_window();

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    initialize_textures();
    initialize_lights();
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);

    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    glutMainLoop();

    return 0;
}

auto init_entities(Platforms &platforms, Player &player) -> void
{
    platforms = get_platforms(".level.txt");
    assertIsTrueElseThrow(platforms.size() > 0);

    player.y_coord = platforms[0].y;
    player.x_coord = (platforms[0].x_left + platforms[0].x_right) / 2;
}

auto on_keyboard(unsigned char key, int x, int y) -> void
{
    (void)x;
    (void)y;

    // debug_log << "Key: " << key << " pressed\n";

    switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
        exit_game();
        break;

    case 'A':
    case 'a':
        player.move_left();
        break;

    case 'D':
    case 'd':
        player.move_right();
        break;

    case 'W':
    case 'w':
        player.jump();
        break;

    case 'S':
    case 's':
        player.stop_x();
        break;

    case 'J':
    case 'j':
        camera.y -= 0.1;
        break;

    case 'K':
    case 'k':
        camera.y += 0.1;
        break;

    case 'F':
    case 'f':
        window.toggle_fullscreen();
        break;

    case 'P':
    case 'p':
        animation_ongoing = !animation_ongoing;
        break;
    }
}

auto on_timer(int value) -> void
{

    if (value != TIMER_ID)
        return;

    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);

    if (!animation_ongoing)
        return;

    const double dt = 1;

    on_update(dt);
    on_render();
}

auto on_update(const double dt) -> void
{

    if (player.y_coord <= lava_floor)
    {
        lose_game();
    }

    static const Platform &last_platform = platforms.back();

    if (player.y_coord == last_platform.y && player.x_coord - PLAYER_EDGE >= last_platform.x_left && player.x_coord + PLAYER_EDGE <= last_platform.x_right)
    {
        win_game();
    }

    player.move(platforms, dt);
}

auto on_render() noexcept -> void
{

    glutPostRedisplay();
}

auto on_reshape(int width, int height) -> void
{
    assertIsTrueElseThrow(width >= 0 && height >= 0);

    window.on_reshape(width, height);
}
auto on_display() -> void
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.x = player.x_coord;

    camera.y = std::max(camera.y, player.y_coord - 1);
    camera.y = std::min(camera.y, player.y_coord + 1);

    gluLookAt(
        camera.x, camera.y + 2, 10, // eye (x,y,z)
        camera.x, camera.y, 0,      // center
        0, 1, 0);                   // Vector up (x,y,z)

    //Dodajemo pozadinu.

    static const double min_floor = get_min_y_val_from_platforms(platforms),
                        max_floor = get_max_y_val_from_platforms(platforms);

    lava_floor = min_floor - 0.5;

    funcMakeBackground(
        names[2],
        names[3],
        platforms[0].x_left - 100, platforms.back().x_right + 100,
        lava_floor, max_floor + 10, -6, 8);

    //Pravimo podloge.

    for (Platform platform : platforms)
    {
        funcMakeBlock(names[1], platform);
    }

    funcMakeFinishSign(names[0], platforms.back());

    //Model igraca.

    glPushMatrix();
    glTranslatef(player.x_coord, player.y_coord, 1);
    funcMakePlayer();
    glPopMatrix();

    glutSwapBuffers();
}
