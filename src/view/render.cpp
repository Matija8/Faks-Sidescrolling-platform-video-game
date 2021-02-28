#include "./render.h"

#include "./image.h"

#include <GL/glut.h>

GLfloat tex1 = 12,
        tex2 = 6; // TODO

static auto bind_active_texture(unsigned texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

static auto unbind_active_texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

// TODO: Review z coord for quads.

static auto render_quad_facing_up(
    GLfloat x_left,
    GLfloat x_right,
    GLfloat y,
    GLfloat z_min,
    GLfloat z_max) -> void
{
    glBegin(GL_QUADS);

    glNormal3f(0, 1, 0);

    glTexCoord2f(0, 0);
    glVertex3f(x_left, y, z_max);

    glTexCoord2f(tex1, 0);
    glVertex3f(x_right, y, z_max);

    glTexCoord2f(tex1, tex2);
    glVertex3f(x_right, y, z_min);

    glTexCoord2f(0, tex2);
    glVertex3f(x_left, y, z_min);

    glEnd();
}

static auto render_quad_facing_front(
    GLfloat x_left,
    GLfloat x_right,
    GLfloat y_top,
    GLfloat y_bottom,
    GLfloat z) -> void
{
    glBegin(GL_QUADS);

    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(x_left, y_bottom, z);

    glTexCoord2f(tex1, 0);
    glVertex3f(x_right, y_bottom, z);

    glTexCoord2f(tex1, tex2);
    glVertex3f(x_right, y_top, z);

    glTexCoord2f(0, tex2);
    glVertex3f(x_left, y_top, z);

    glEnd();
}

static auto render_quad_facing_left(
    GLfloat x,
    GLfloat y_top,
    GLfloat y_bottom,
    GLfloat z_min,
    GLfloat z_max) -> void
{
    glBegin(GL_QUADS);

    glNormal3f(-1, 0, 0);

    glTexCoord2f(0, 0);
    glVertex3f(x, y_bottom, z_min);

    glTexCoord2f(tex1, 0);
    glVertex3f(x, y_bottom, z_max);

    glTexCoord2f(tex1, tex2);
    glVertex3f(x, y_top, z_max);

    glTexCoord2f(0, tex2);
    glVertex3f(x, y_top, z_min);

    glEnd();
}

static auto render_quad_facing_right(
    GLfloat x,
    GLfloat y_top,
    GLfloat y_bottom,
    GLfloat z_min,
    GLfloat z_max) -> void
{
    glBegin(GL_QUADS);

    glNormal3f(1, 0, 0);

    glTexCoord2f(0, 0);
    glVertex3f(x, y_bottom, z_max);

    glTexCoord2f(tex1, 0);
    glVertex3f(x, y_bottom, z_min);

    glTexCoord2f(tex1, tex2);
    glVertex3f(x, y_top, z_min);

    glTexCoord2f(0, tex2);
    glVertex3f(x, y_top, z_max);

    glEnd();
}

auto renderPlatform(
    unsigned platform_texture,
    Platform platform) -> void
{
    const GLfloat min_x = platform.x_left,
                  max_x = platform.x_right,
                  y = platform.y;

    bind_active_texture(platform_texture);

    render_quad_facing_up(min_x, max_x, y, -1, 2);

    render_quad_facing_front(min_x, max_x, y, y - 1, 2);

    render_quad_facing_left(min_x, y, y - 1, -1, 2);

    render_quad_facing_right(max_x, y, y - 1, -1, 2);

    unbind_active_texture();
}

auto renderFinishSign(
    unsigned wall_texture,
    Platform platform) -> void
{

    GLfloat min_x = platform.x_left,
            max_x = platform.x_right,
            floor_y = platform.y;

    glBindTexture(GL_TEXTURE_2D, wall_texture);

    render_quad_facing_front(min_x, max_x, floor_y + 2, floor_y, -0.5);

    render_quad_facing_up(min_x, max_x, floor_y + 2, -1, -0.5);

    render_quad_facing_left(min_x, floor_y + 2, floor_y, -1, -0.5);

    render_quad_facing_right(min_x, floor_y + 2, floor_y, -1, -0.5);

    unbind_active_texture();
}

auto renderBackground(
    unsigned background_texture,
    unsigned lava_texture,
    float min_x,
    float max_x,
    float min_y,
    float max_y,
    float far_z,
    float near_z) -> void
{
    bind_active_texture(background_texture);

    render_quad_facing_front(min_x, max_x, max_y, min_y, far_z);

    bind_active_texture(lava_texture);

    render_quad_facing_up(min_x, max_x, min_y, near_z, far_z);

    unbind_active_texture();
}

auto renderPlayer() -> void
{
    // TODO
    // GLfloat light_position[] = {1, 1, 1, 0};
    GLfloat no_material[] = {0, 0, 0, 1};
    // GLfloat material_ambient[] = {0.7, 0.7, 0.7, 1};
    // GLfloat material_ambient_heterogeneous[] = {0.8, 0.8, 0.2, 1};
    GLfloat material_diffuse[] = {0.1, 0.5, 0.8, 1};
    GLfloat material_specular[] = {1, 1, 1, 1};
    GLfloat no_shininess[] = {0};
    // GLfloat low_shininess[] = {5};
    GLfloat high_shininess[] = {100};
    GLfloat material_emission[] = {0.3, 0.2, 0.2, 0};

    GLdouble head_radius = 0.2;
    GLdouble body_height = 0.4; //Sirina tela je bh/2 = 0.2.

    glPushMatrix();
    glTranslatef(0, body_height / 2, 0);

    glPushMatrix(); //glava
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
    glColor3f(0.8, 0.1, 0.1);
    glTranslatef(0, head_radius * 2, 0);
    glutSolidSphere(head_radius, 50, 50);
    glPopMatrix();

    glPushMatrix(); //telo
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
    glColor3f(0, 0.5, 1);
    glutSolidCube(body_height);
    glPopMatrix();

    glPushMatrix(); //ruke
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
    glColor3f(0, 0, 1);
    glTranslatef(0, 0, -body_height);
    glScalef(1, 2, 1);
    glutSolidCube(body_height / 3);
    glPopMatrix();
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
    glTranslatef(0, 0, body_height);
    glScalef(1, 2, 1);
    glutSolidCube(body_height / 3);
    glPopMatrix();

    glPopMatrix();
}
