#include "game.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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

double time_in_air = 0; // Vreme.

double current_floor_y; // Vrednost y-koordinate podloge ispod igraca (ne nuzno poda).

static size_t what_is_d = 0;
static int d_change_flag = 1;
static double current_right_x, // Pozicije ivica trenutnog bloka.
    current_left_x,
    next_left_x, // Pozicije ivica sledeceg/proslog bloka.
    last_right_x,
    next_floor, // Vrednost y-koordinate sledece podloge.
    last_floor;

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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Game Running");

    window.init_window();

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    init_entities(platforms, player);

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
    platforms = get_platforms(".nivo.txt");
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
    }
}

auto on_timer(int value) -> void
{

    if (value != TIMER_ID)
        return;

    // TODO
    on_update();
    // on_render()

    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
}

auto on_update() -> void
{

    if (player.y_coord <= lava_floor)
    {
        lose_game();
    }

    platform_checking();

    const double dt = 1;

    // Move on x axis.
    player.x_coord += player.x_velocity * dt;

    player.move_on_y_axis(dt);
}

auto on_reshape(int width, int height) -> void
{
    assertIsTrueElseThrow(width >= 0 && height >= 0);

    window.on_reshape(width, height);
}

auto platform_checking() -> void
{ // TODO

    if (what_is_d >= 0 && what_is_d < platforms.size() && d_change_flag == 1)
    {
        if (what_is_d == platforms.size() - 1)
        {
            last_right_x = current_right_x;
            current_left_x = next_left_x;
            current_right_x = platforms[what_is_d].x_right;
            current_floor_y = platforms[what_is_d].y;
            next_left_x = current_right_x + 1000;

            d_change_flag = 0;
        }
        else
        {
            next_left_x = platforms[what_is_d + 1].x_left;
            current_left_x = platforms[what_is_d].x_left;
            current_right_x = platforms[what_is_d].x_right;
            current_floor_y = platforms[what_is_d].y;
            next_floor = platforms[what_is_d + 1].y;

            if (what_is_d == 0)
            {
                last_right_x = current_left_x - 10; // TODO
                last_floor = lava_floor;
            }
            else
            {
                last_right_x = platforms[what_is_d - 1].x_right;
                last_floor = platforms[what_is_d - 1].y;
            }

            d_change_flag = 0;
        }
        /*printf("promena vred: floor %.0f, last edge %.0f, left %.0f, right %.0f, next edge %.0f\n",
            current_floor, last_right_x, current_left_x, current_right_x, next_left_x);*/
    }

    // Provere propadanja.Leva ivica igraca je x_cam-0.2.Desna ivica igraca je x_cam-0.2.

    //Na trenutnoj podlozi smo.

    if (player.x_coord + 0.2 >= current_left_x && player.x_coord - 0.2 <= current_right_x && player.y_coord >= current_floor_y)
    {
        player.is_above_platform = true;
    }

    const bool is_falling_left = player.x_coord + 0.2 < current_left_x && player.x_coord - 0.2 > last_right_x,
               is_falling_right = player.x_coord - 0.2 > current_right_x && player.x_coord + 0.2 < next_left_x;

    if (is_falling_left || is_falling_right)
    {
        player.is_above_platform = false;
    }

    //provera da li se zakucavamo u trenutnu podlogu kad propadamo.

    if (player.x_coord + 0.2 >= current_left_x && player.x_coord - 0.2 <= current_right_x && player.y_coord < current_floor_y)
    {

        if (player.y_coord > current_floor_y - 1.8)
        {

            if (player.x_velocity > 0 && player.x_coord < current_left_x + 0.2)
                player.x_coord = current_left_x - 0.2;

            if (player.x_velocity < 0 && player.x_coord > current_right_x - 0.2)
                player.x_coord = current_right_x + 0.2;
        }
    }

    //Stigli smo do sledece podloge.

    if (player.x_coord + 0.2 >= next_left_x && player.y_coord >= next_floor)
    {
        d_change_flag = 1;
        what_is_d++;
    }
    if (player.x_coord + 0.2 >= next_left_x && player.y_coord < next_floor)
    {
        if (player.y_coord > next_floor - 1.8)
        { //Igrac je visok 0.8. Podloga je visine 1.
            player.x_coord = next_left_x - 0.2;
        }
    }

    //Vratili smo se na prethodnu podlogu.

    if (player.x_coord - 0.2 <= last_right_x && player.y_coord >= last_floor)
    {
        assertIsTrueElseThrow(what_is_d > 0);
        d_change_flag = 1;
        what_is_d--;
    }
    if (player.x_coord - 0.2 <= last_right_x && player.y_coord < last_floor)
    {
        if (player.y_coord > last_floor - 1.8)
        {
            player.x_coord = last_right_x + 0.2;
        }
    }

    //Stigli smo na kraj.

    if (what_is_d == (platforms.size() - 1) && player.y_coord == current_floor_y)
    {
        win_game();
    }
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
        platforms[0].x_left - 20, platforms.back().x_right + 30,
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
