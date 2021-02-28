#include "./render.h"

#include "./image.h"

#include <GL/glut.h>

//podaci za materijal
GLfloat light_position[] = {1, 1, 1, 0};
GLfloat no_material[] = {0, 0, 0, 1};
GLfloat material_ambient[] = {0.7, 0.7, 0.7, 1};
GLfloat material_ambient_heterogeneous[] = {0.8, 0.8, 0.2, 1};
GLfloat material_diffuse[] = {0.1, 0.5, 0.8, 1};
GLfloat material_specular[] = {1, 1, 1, 1};
GLfloat no_shininess[] = {0};
GLfloat low_shininess[] = {5};
GLfloat high_shininess[] = {100};
GLfloat material_emission[] = {0.3, 0.2, 0.2, 0};

// TODO: Remove duplication in making quads.

static auto bind_active_texture(unsigned texture_index)
{
    glBindTexture(GL_TEXTURE_2D, texture_index);
}

static auto unbind_active_texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto renderPlatform(
    unsigned platform_texture_index,
    Platform platform) -> void
{

    const GLfloat min_x = platform.x_left,
                  max_x = platform.x_right,
                  y = platform.y;

    bind_active_texture(platform_texture_index);

    //pravimo podlogu

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, y, -1);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, y, 2);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, y, 2);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, y, -1);
    glEnd();

    //pravimo prednju ivicu podloge

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, y, 2);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, y - 1, 2);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, y - 1, 2);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, y, 2);
    glEnd();

    //leva ivica

    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);

    glTexCoord2f(12, 6);
    glVertex3f(min_x, y, 2);

    glTexCoord2f(12, 0);
    glVertex3f(min_x, y - 1, 2);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, y - 1, -1);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, y, -1);
    glEnd();

    //desna ivica

    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, y, 2);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, y - 1, 2);

    glTexCoord2f(0, 0);
    glVertex3f(max_x, y - 1, -1);

    glTexCoord2f(0, 6);
    glVertex3f(max_x, y, -1);
    glEnd();

    unbind_active_texture();
}

auto renderFinishSign(
    unsigned wall_texture_index,
    Platform platform) -> void
{

    GLfloat min_x = platform.x_left,
            max_x = platform.x_right,
            floor_y = platform.y;

    glBindTexture(GL_TEXTURE_2D, wall_texture_index);

    // Front
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, floor_y, -0.5);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, floor_y, -0.5);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, floor_y + 2, -0.5);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, floor_y + 2, -0.5);
    glEnd();

    // Top
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, floor_y + 2, -0.5);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, floor_y + 2, -0.5);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, floor_y + 2, -1);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, floor_y + 2, -1);
    glEnd();

    // Left
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);

    glTexCoord2f(12, 6);
    glVertex3f(min_x, floor_y, -0.5);

    glTexCoord2f(12, 0);
    glVertex3f(min_x, floor_y + 2, -0.5);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, floor_y + 2, -1);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, floor_y, -1);
    glEnd();

    // Right
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, floor_y, -0.5);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, floor_y + 2, -0.5);

    glTexCoord2f(0, 0);
    glVertex3f(max_x, floor_y + 2, -1);

    glTexCoord2f(0, 6);
    glVertex3f(max_x, floor_y, -1);
    glEnd();

    unbind_active_texture();
}

void renderBackground(
    unsigned name1, unsigned name2, float min_x, float max_x,
    float min_y, float max_y, float far_z, float near_z)
{

    // Make background

    bind_active_texture(name1);

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, max_y, far_z);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, min_y, far_z);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, min_y, far_z);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, max_y, far_z);
    glEnd();

    //Make lava

    bind_active_texture(name2);

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);

    glTexCoord2f(12, 6);
    glVertex3f(max_x, min_y, near_z);

    glTexCoord2f(12, 0);
    glVertex3f(max_x, min_y, far_z);

    glTexCoord2f(0, 0);
    glVertex3f(min_x, min_y, far_z);

    glTexCoord2f(0, 6);
    glVertex3f(min_x, min_y, near_z);
    glEnd();

    unbind_active_texture();
}

void renderPlayer(void)
{
    // TODO

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

    glColor3f(0, 0, 0); // TODO: This becomes lava!?
}