#define FREEGLUT_STATIC
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <iostream>
#include <string>       
using namespace std;
bool loadmytexture(string filename, GLuint& textureID);
bool  loadsky(GLuint &skytextureID);
#endif