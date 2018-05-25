#define FREEGLUT_STATIC
#include "Glee/GLee.h"
#include <GL/glut.h>
#include <iostream>
#include "lightandshadow.h"

/*void shadowMatrixInit()
{

	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glLoadIdentity();
	gluPerspective(75, (GLfloat)window_width / (GLfloat)window_height, 1.0f, 30000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], direct[0], direct[1], direct[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(75, 1.0f, 1.0f, 30000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(sun_position[0], sun_position[1], sun_position[2], center[0], center[1], center[2], direct[0], direct[1], direct[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
}*/
