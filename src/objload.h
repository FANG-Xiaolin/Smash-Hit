#define FREEGLUT_STATIC
#ifndef OBJLOAD_H
#define OBJLOAD_H

#include <GL/glut.h>
#include <cmath>
#include<cstdio>
#include <iostream>
#include <string>       
using namespace std;

struct face
{
	int s;//组成面的点的数目，3-4
	GLuint Index[4][3];//索引,0为坐标索引，1为纹理索引，2为法线索引
	face()
	{
		s = 0;
	}
	GLuint mtlindex;
};
struct mRGB
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	mRGB()
	{
		r = g = b = 0;
	}
};
struct mtl
{
	string name;//材料名称
	mRGB ka_rgb;//环境光
	mRGB kd_rgb;//漫射光
	mRGB ks_rgb;//镜面反射光
	mRGB tf_rgb;//滤光透射率
	GLuint illum;//光照模型
	GLfloat d;//渐隐系数
	GLfloat Ns;//反射指数，反射指数值，该值越高则高光越密集，一般取值范围在0~1000。
	GLfloat Ni;//折射率，若取值为1.0，光在通过物体的时候不发生弯曲。玻璃的折射率为1.5
	mtl ()
	{
		name = "";
		illum = 0;
		d = 1;
		Ns = 1;
		Ni = 1.5;
	}
};
struct object
{
	int v_num;
	int vn_num;
	int vt_num;
	int mtl_num;
	GLfloat **vArr;
	GLfloat **vnArr;
	GLfloat **vtArr;
	char* texturename;
	GLuint textureID;
	mtl *objmtl;
	int face_num;
	face *objface;
	object()
	{
		texturename = NULL;
		textureID = 0;
		v_num = vn_num = vt_num = face_num = mtl_num = 0;
	}
};

bool readmtl(string addr,object& obj);
bool readobj(string addr,object& obj);
void deleteobj(object& obj);
void drawobj(object obj);
#endif