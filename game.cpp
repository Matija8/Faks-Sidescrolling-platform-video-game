#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#include "constants.h"
#include "error_handling.h"
#include "functions.h"
#include "level.h"
#include "image.h"
#include "window.h"

static GLuint names[4]; // Teksture.
static float lava_floor;
static bool animation_ongoing = true;

static float time_in_air = 0; // Vreme.

static double max_floor,
    min_floor;

static size_t what_is_d = 0;
static int d_change_flag = 1;
static double current_floor_y, // Vrednost y-koordinate podloge ispod igraca (ne nuzno poda).
    current_right_x,           // Pozicije ivica trenutnog bloka.
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

struct Player
{
    auto moveLeft()
    {
        x_velocity = -DEFAULT_X_VELOCITY;
    }

    auto moveRight()
    {
        x_velocity = DEFAULT_X_VELOCITY;
    }

    auto stopX()
    {
        x_velocity = 0;
    }

public:
    double x_coord = 0;
    double x_velocity = 0;
    double y_coord;
    double y_velocity = JUMP_SPEED;
    bool is_jumping = false;
    bool is_moving = false;
    bool is_falling = false;
    bool is_above_platform = true;
};

static Player player;
static Camera camera;
static AppWindow window;
static Level level;

static void initialize_lights(void);
static void initialize_textures(void);
static void on_display(void);
static void on_timer(int value);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);

static auto exit_game() -> void
{
    exit(EXIT_SUCCESS);
}

static auto lose_game() -> void
{
    std::cout << "GAME OVER! Play again?\n";
    exit_game();
}

static auto win_game() -> void
{
    std::cout << "You WIN!\n";
    exit_game();
}

auto &debug_log = std::cout;

auto main(int argc, char **argv) -> int
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Game Running");

    window.initWindow();

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    level = get_level(".nivo.txt");
    min_floor = level.min_floor,
    max_floor = level.max_floor,
    player.y_coord = level.podaci[0].y; //Igrac pocinje na visini prve podloge.

    initialize_textures();
    initialize_lights();
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);

    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    glutMainLoop();

    return 0;
}

static auto move_on_y_axis(const double dt = 1) -> void
{

    if (!player.is_jumping && (!player.is_above_platform || player.y_coord > current_floor_y)) // have a margin of error for floor hover.
    {

        player.is_falling = true;
        time_in_air += dt / 5; // scale speeds
        player.y_velocity = GRAVITY * time_in_air;
        // p.y_velocity += GRAVITY * dt / 5;

        player.y_velocity = std::min(player.y_velocity, TERMINAL_Y_VELOCITY);

        player.y_coord -= player.y_velocity * dt;
    }
    else
    {
        player.is_falling = false;
        time_in_air = 0;
    }

    // TODO: Get delta time and use it for moving.

    if (player.is_jumping && !player.is_falling)
    {
        time_in_air += dt / 5; // This is time spent falling!
        player.y_velocity -= GRAVITY * time_in_air;
        player.y_coord += player.y_velocity * time_in_air;
        if (player.y_velocity <= 0)
        {
            player.is_falling = true;
            player.is_jumping = false;
            time_in_air = 0;
        }
    }
    else
    {
        if (player.is_above_platform && player.y_coord <= current_floor_y)
        {
            player.is_falling = false;
            player.is_jumping = false;
            time_in_air = 0;
            player.y_coord = current_floor_y;
            player.y_velocity = JUMP_SPEED;
        }
    }

    // debug_log << p.y_velocity << "\n";
}

static void on_keyboard(unsigned char key, int x, int y)
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
        player.moveLeft();
        break;

    case 'D':
    case 'd':
        player.moveRight();
        break;

    case 'W':
    case 'w':
        if (!player.is_falling)
        {
            // debug_log << "Not falling!\n";
            player.is_jumping = true;
            time_in_air = 0;
        }
        break;

    case 'S':
    case 's':
        player.stopX();
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
        window.toggleFullScreen();
    }
}

static void on_timer(int value)
{
    const double dt = 1;

    if (value != TIMER_ID)
        return;

    if (player.y_coord <= lava_floor)
    {
        lose_game();
    }

    // Move on x axis.
    player.x_coord += player.x_velocity * dt;

    move_on_y_axis(dt);

    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
}

//Kod preuzet sa vezbi asistenta Ivana Cukica. Primer Cube.
static void on_reshape(int width, int height)
{
    assertIsTrueElseThrow(width >= 0 && height >= 0);

    window.onReshape(width, height);
}

static void on_display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.x = player.x_coord; // Camera is focused on player?

    camera.y = std::max(camera.y, player.y_coord - 1);
    camera.y = std::min(camera.y, player.y_coord + 1);

    gluLookAt(
        camera.x, camera.y + 2, 10, // eye (x,y,z)
        camera.x, camera.y, 0,      // center
        0, 1, 0);                   // Vector up (x,y,z)

    //Pravimo podloge.

    for (size_t i = 0; i < level.podaci.size(); i++)
    {
        funcMakeBlock(names[1], level.podaci[i]);
        if (i == level.podaci.size() - 1)
        {
            funcMakeFinishSign(names[0], level.podaci[i]);
        }
    }

    //inicijalizacija podloge;

    if (what_is_d >= 0 && what_is_d < level.podaci.size() && d_change_flag == 1)
    {
        if (what_is_d == level.podaci.size() - 1)
        {
            last_right_x = current_right_x;
            current_left_x = next_left_x;
            current_right_x = level.podaci[what_is_d].x_right;
            current_floor_y = level.podaci[what_is_d].y;
            next_left_x = current_right_x + 1000;

            d_change_flag = 0;
        }
        else
        {
            next_left_x = level.podaci[what_is_d + 1].x_left;
            current_left_x = level.podaci[what_is_d].x_left;
            current_right_x = level.podaci[what_is_d].x_right;
            current_floor_y = level.podaci[what_is_d].y;
            next_floor = level.podaci[what_is_d + 1].y;

            if (what_is_d == 0)
            {
                last_right_x = current_left_x - 10; // TODO
                last_floor = lava_floor;
            }
            else
            {
                last_right_x = level.podaci[what_is_d - 1].x_right;
                last_floor = level.podaci[what_is_d - 1].y;
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

    if (what_is_d == (level.podaci.size() - 1) && player.y_coord == current_floor_y)
    {
        win_game();
    }

    //Model igraca.

    glPushMatrix();
    glTranslatef(player.x_coord, player.y_coord, 1);
    funcMakePlayer();
    glPopMatrix();

    //Dodajemo pozadinu.

    lava_floor = min_floor - 0.5;

    funcMakeBackground(names[2],
                       names[3],
                       level.podaci[0].x_left - 20, level.podaci.back().x_right + 30,
                       lava_floor, max_floor + 10, -6, 8);

    glutSwapBuffers();
}

//Teksture. Namerno odvojene od ostatka koda.
//kod preuzet sa vezbi asistenta Ivana Cukica i modifikovan.

static void initialize_textures(void)
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image *image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(4, names);

    /* Kreira se tekstura zid. */
    image_read(image, BMP_WALL);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se tekstura trava. */
    image_read(image, BMP_GRASS);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se tekstura pozadina. */
    image_read(image, BMP_BACKGROUND);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se tekstura lava. */
    image_read(image, BMP_LAVA);

    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//Osvetljenje. Namerno odvojene od ostatka koda.
//kod preuzet sa vezbi asistenta Ivana Cukica i modifikovan.

static void initialize_lights(void)
{
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = {1, 10, 5, 0};

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
