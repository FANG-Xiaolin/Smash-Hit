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
	int s;//�����ĵ����Ŀ��3-4
	GLuint Index[4][3];//����,0Ϊ����������1Ϊ����������2Ϊ��������
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
	string name;//��������
	mRGB ka_rgb;//������
	mRGB kd_rgb;//�����
	mRGB ks_rgb;//���淴���
	mRGB tf_rgb;//�˹�͸����
	GLuint illum;//����ģ��
	GLfloat d;//����ϵ��
	GLfloat Ns;//����ָ��������ָ��ֵ����ֵԽ����߹�Խ�ܼ���һ��ȡֵ��Χ��0~1000��
	GLfloat Ni;//�����ʣ���ȡֵΪ1.0������ͨ�������ʱ�򲻷���������������������Ϊ1.5
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