#pragma once

void funcMakeBlock(GLuint names, float min_x, float max_x, float floor_y);

void funcMakeBackground(GLuint name1, GLuint name2,float min_x, float max_x, float min_y, float max_y, float far_z, float near_z);

void funcMakePlayer(void);

void nivo(float** podaci,int* br, float* min_floor, float* max_floor);

void funcMakeFinishSign(GLuint name, float min_x, float max_x, float floor_y);
