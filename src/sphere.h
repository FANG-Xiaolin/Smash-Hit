#define FREEGLUT_STATIC
#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glut.h>
#include <iostream>
 
struct mysphere
{
	GLfloat position[3];
	GLfloat radius;
	GLfloat sphere_ambient[4];
	GLfloat sphere_diffuse[4];
	GLfloat sphere_specular[4];
	GLfloat sphere_emission[4];
	GLfloat sphere_shininess;
	GLuint textureID;
	mysphere():
		sphere_shininess(100),
		radius(1.5)
	{
		textureID = 0;
		position[0] = position[1] = position[2] = 0;
		sphere_ambient[0] = sphere_ambient[1] = sphere_ambient[2] = 0.4f;
		sphere_ambient[3] = 1.0;
		sphere_specular[0] = sphere_specular[1] = sphere_specular[2] = 1.0f;
		sphere_specular[3] = 1.0;
		sphere_diffuse[0] = sphere_diffuse[1] = sphere_diffuse[2] = 1.0f;
		sphere_diffuse[3] = 1.0;
		sphere_emission[0] = sphere_emission[1] = sphere_emission[2] = 1.0f;
		sphere_emission[3] = 1.0;
	}

	void setradius(GLdouble r){ radius = r; }
	void draw(GLfloat radius,GLdouble theta=0,GLdouble phi=0);
	void draw(GLdouble theta=0, GLdouble phi=0);
};

#endif