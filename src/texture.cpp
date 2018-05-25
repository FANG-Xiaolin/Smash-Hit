#define FREEGLUT_STATIC
#include "Glee\GLee.h"
#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <string>       
#include <fstream>
#include <cstdio>
#include "texture.h"
using namespace std;
bool loadmytexture(string filename, GLuint& textureID)
{

	ifstream file;
	file.open(filename, ifstream::binary);
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	int height, width, total;
	if (!file)
	{
		cerr << "Error::bmpLoader, could not open bmp file:"
			<< filename << " for reading." << endl;
		return false;
	}
	file.seekg(0);
	file.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

	file.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

	height = infoHeader.biHeight;
	width = infoHeader.biWidth;
	total = height*width;
	cout << height << " " << width << " " << total << endl;
	GLubyte *pixel = new GLubyte[total * 3];
	memset(pixel, 0, total * 3);
	file.read((char*)pixel, total * 3);

	file.close();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixel);
	delete[] pixel;
	glDisable(GL_TEXTURE_2D);
	return true;
}

bool  loadsky(GLuint &skytextureID)
{
	ifstream file;
	glBindTexture(GL_TEXTURE_CUBE_MAP, skytextureID);
	const string filename[6] = { "sky_px.bmp", "sky_nx.bmp", "sky_py.bmp", "sky_ny.bmp", "sky_pz.bmp", "sky_nz.bmp" };
	GLenum cube[6] =
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
	for (int i = 0; i < 6; i++)
	{
		file.open(filename[i], ifstream::binary);
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;
		int height, width, total;
		if (!file)
		{
			cerr << "Error::bmpLoader, could not open bmp file:"
				<< filename << " for reading." << endl;
			return false;
		}
		file.seekg(0);
		file.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

		file.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

		height = infoHeader.biHeight;
		width = infoHeader.biWidth;
		total = height*width;
		cout << filename[i] << " " << height << " " << width << " " << total << endl;
		GLubyte *pixel = new GLubyte[total * 3];
		memset(pixel, 0, total * 3);
		file.read((char*)pixel, total * 3);
		file.close();
		/*for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) cout << (int)pixel[3 * (i*width + j)+2] << endl;*/
		glTexImage2D(cube[i], 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixel);

		delete[] pixel;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV, GL_REPLACE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}