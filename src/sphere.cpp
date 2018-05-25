#define FREEGLUT_STATIC
#include "Glee\GLee.h"
#include <GL/glut.h>
#include <iostream>
#include "sphere.h"
using namespace std;
void mysphere::draw(GLfloat radius, GLdouble theta, GLdouble phi)
{
	GLint last_textureID;
	this->radius = radius;
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, sphere_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sphere_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sphere_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sphere_shininess);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glMatrixMode(GL_TEXTURE);
	//glPushMatrix();
	//MATRIX4X4 m;
	//获取照相机的位置，并进行反转,形成正确的反射纹理
	//glGetFloatv(GL_MODELVIEW_MATRIX, m);
	//m.Invert();
	//glMultMatrixf(m);
	glutSolidSphere(radius, 40, 32);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glPopMatrix();

}
void mysphere::draw(GLdouble theta, GLdouble phi)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glRotated(-phi, 0, 1, 0);
	glPopMatrix();
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glMaterialfv(GL_FRONT, GL_AMBIENT, sphere_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sphere_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sphere_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sphere_shininess);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	//glPopMatrix(); //glMatrixMode(GL_MODELVIEW);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glutSolidSphere(radius, 40, 32);
	//glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

