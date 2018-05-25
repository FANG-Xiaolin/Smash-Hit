#define FREEGLUT_STATIC
#ifndef LIGHTANDSHADOW_H
#define LIGHTANDSHADOW_H
#include <GL/glut.h>
#include <iostream>
#include "Maths/Maths.h"
static GLfloat window_width = 1000;
static GLfloat window_height = 600;

static double theta = 90, phi = 00;//球坐标参数
static GLdouble center[3] = { -200, 0, 20 };//视觉中心
static GLdouble eye[3] = { 200.0, 0.0, -7.0 };//照相机初始位置
static GLdouble direct[3] = { 0.0, 0.0, 1.0 };//照相机头方向
static GLfloat all_ambient[4][4] = { 0.1, 0.1, 0.1, 1.0,
								     0.3, 0.1, 0.1, 1.0,
								     0.1, 0.3, 0.1, 1.0,
								     0.1, 0.1, 0.3, 1.0,
								    };//全局环境光
static GLuint ambient_num = 1;
static GLfloat sun_ambient[4] = { 0.3, 0.3, 0.3, 1.0};
static GLfloat sun_diffuse[4] = { 0.8, 0.8, 0.8, 1.0};
static GLfloat sun_position[4] = { 200.0, 0.0, 5.0, 1.0 };
static GLfloat sun_specular[4] = { 0.8, 0.8, 0.8, 1.0};

static GLfloat lowAmbient[4] = { 0.1, 0.1, 0.1, 1 };
static GLfloat lowDiffuse[4] = { 0.2, 0.2, 0.2, 1 };
static GLfloat lowSpecular[4] = { 0.0, 0.0, 0.0, 0.0 };

static MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
static MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;
static GLuint shadowtexture;

static GLuint shadowSize = 512;

extern void shadowMatrixInit();


#endif