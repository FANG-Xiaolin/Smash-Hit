#define FREEGLUT_STATIC
#include <GL/glut.h>
#include <cmath>
#include<cstdio>
#include <iostream>
#include <string>       
#include <fstream>
#include <sstream>
#include <algorithm>
#include "objload.h"
#include "texture.h"
using namespace std;
bool readobj(string addr, object& obj)
{
	ifstream file;
	file.open(addr);
	cout << addr << endl;
	if (!file)
	{
		cerr << "Error::ObjLoader, could not open obj file:"
			<< addr << " for reading." << endl;
		return false;
	}
	//else cout << "open file success" << endl;
	string line;
	string mtladdr;
	while (getline(file, line))
	{
		if (line.substr(0, 2) == "vt") // 顶点纹理坐标
			obj.vt_num++;
		else if (line.substr(0, 2) == "vn")//法线
			obj.vn_num++;
		else if (line.substr(0, 1) == "v")//点
			obj.v_num++;
		else if (line.substr(0, 1) == "f") // 面数据
			obj.face_num++;
		else if (line.substr(0, 6) == "mtllib")
		{
			istringstream s(line.substr(6));
			s >> mtladdr;
		}
	}
	file.close();
	if (!readmtl(mtladdr, obj))
	{
		cerr << "Error::ObjLoader, could not read mtl file:"
			<< addr << endl;
		return false;
	}//read mtl
	file.open(addr);
	int i;
	obj.vArr = new GLfloat*[obj.v_num];
	for (i = 0; i < obj.v_num; i++) obj.vArr[i] = new GLfloat[3];
	obj.vtArr = new GLfloat*[obj.vt_num];
	for (i = 0; i < obj.vt_num; i++) obj.vtArr[i] = new GLfloat[2];
	obj.vnArr = new GLfloat*[obj.vn_num];
	for (i = 0; i < obj.vn_num; i++) obj.vnArr[i] = new GLfloat[3];
	obj.objface = new face[obj.face_num];
	int vnow = 0, vtnow = 0, vnnow = 0, fnow = 0;
	int mtlnow = 0;
	/*cout << "get number success" << endl;
	cout << obj.v_num << endl;
	cout << obj.vn_num << endl;
	cout << obj.vt_num << endl;
	cout << obj.face_num << endl;*/
	while (getline(file, line))
	{
		//cout << line << endl;
		if (line.substr(0, 2) == "vt") // 顶点纹理坐标数据
		{
			istringstream s(line.substr(3));
			s >> obj.vtArr[vtnow][0];
			s >> obj.vtArr[vtnow][1];
			//cout << obj.vtArr[vtnow][0] << " " << obj.vtArr[vtnow][1]<<endl;
			vtnow++;
		}
		else if (line.substr(0, 2) == "vn") // 顶点法向量数据
		{
			istringstream s(line.substr(3));
			s >> obj.vnArr[vnnow][0];
			s >> obj.vnArr[vnnow][1];
			s >> obj.vnArr[vnnow][2];
			vnnow++;
		}
		else if (line.substr(0, 1) == "v") // 顶点位置数据
		{
			istringstream s(line.substr(2));
			s >> obj.vArr[vnow][0];
			s >> obj.vArr[vnow][1];
			s >> obj.vArr[vnow][2];
			vnow++;
		}
		else if (line.substr(0, 1) == "f") // 面数据
		{
			istringstream vtns(line.substr(2));//整行输入
			string vtn;//每一个形如2/3/4的东西，2为顶点，3位纹理，4为法线，均为索引
			int k = 0;//第k个点
			while (vtns >> vtn) // 处理一行中多个顶点属性
			{
				//cout << vtn << endl;
				replace(vtn.begin(), vtn.end(), '/', ' ');//algorithm中的函数
														  //cout << vtn << endl;
				istringstream ivtn(vtn);
				if (vtn.find("  ") != string::npos) // 没有纹理数据
				{
					cerr << "Error:ObjLoader, no texture data found within file:"
						<< addr << endl;
					return false;
				}
				ivtn >> obj.objface[fnow].Index[k][0]
					>> obj.objface[fnow].Index[k][1]
					>> obj.objface[fnow].Index[k][2];
				obj.objface[fnow].Index[k][0]--;
				obj.objface[fnow].Index[k][1]--;
				obj.objface[fnow].Index[k][2]--;//索引都要-1，默认点索引从1开始，我们从0开始
				k++;
			}
			obj.objface[fnow].mtlindex = mtlnow;
			obj.objface[fnow].s = k;//3或者4
			fnow++;//面数+1
		}
		else if (line.substr(0, 6) == "usemtl")
		{
			ifstream s(line.substr(7));
			string name;
			s >> name;
			for (int j = 0; j < obj.mtl_num; j++)
			{
				if (name == obj.objmtl[j].name)
				{
					//cout << name << " " << obj.objmtl[j].name << endl;
					mtlnow = j;
					break;
				}
			}
		}
	}
	file.close();
	return true;
}
bool readmtl(string addr, object& obj)
{
	ifstream file;
	string line;
	file.open(addr);
	if (!file)
	{
		cerr << "Error::ObjLoader, could not open obj file:"
			<< addr << " for reading." << endl;
		return false;
	}
	while (getline(file, line))
	{
		if (line.substr(0, 6) == "newmtl") 
			obj.mtl_num++;
	}
	file.close();
	obj.objmtl = new mtl[obj.mtl_num];
	file.open(addr);
	int k = -1;
	while (getline(file, line))
	{
		if (line.substr(0, 6) == "newmtl")
		{
			k++;
			istringstream s(line.substr(7));
			s >> obj.objmtl[k].name;
			//cout << obj.objmtl[k].name << endl;
		}
		else if (line.substr(0, 2) == "Ka")
		{
			istringstream s(line.substr(3));
			s >> obj.objmtl[k].ka_rgb.r;
			s >> obj.objmtl[k].ka_rgb.g;
			s >> obj.objmtl[k].ka_rgb.b;
		}
		else if (line.substr(0, 2) == "Kd")
		{
			istringstream s(line.substr(3));
			s >> obj.objmtl[k].kd_rgb.r;
			s >> obj.objmtl[k].kd_rgb.g;
			s >> obj.objmtl[k].kd_rgb.b;
		}
		else if (line.substr(0, 2) == "Ks")
		{
			istringstream s(line.substr(3));
			s >> obj.objmtl[k].ks_rgb.r;
			s >> obj.objmtl[k].ks_rgb.g;
			s >> obj.objmtl[k].ks_rgb.b;
		}
		else if (line.substr(0, 2) == "Tf")
		{
			istringstream s(line.substr(3));
			s >> obj.objmtl[k].tf_rgb.r;
			s >> obj.objmtl[k].tf_rgb.g;
			s >> obj.objmtl[k].tf_rgb.b;
		}
		else if (line.substr(0, 5) == "illum")
		{
			istringstream s(line.substr(6));
			s >> obj.objmtl[k].illum;
		}
		else if (line.substr(0, 1) == "d")
		{
			istringstream s(line.substr(2));
			s >> obj.objmtl[k].d;
		}
		else if (line.substr(0, 2) == "Ns")
		{
			istringstream s(line.substr(3));
			s >> obj.objmtl[k].Ns;
		}
		else if (line.substr(0, 2) == "Ni")
		{
			istringstream s(line.substr(3));
			s >> obj.objmtl[k].Ni;
		}
		else if (line.substr(0, 3) == "map")
		{

			obj.texturename = new char[20];
			istringstream s(line.substr(7));
			s >> obj.texturename;
			cout << obj.texturename << endl;
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &obj.textureID);
			loadmytexture(obj.texturename, obj.textureID);
			glDisable(GL_TEXTURE_2D);
		}
	}
	return true;
}
void deleteobj(object& obj)
{
	int i = 0;
	for (i = 0; i < obj.v_num; i++) delete[] obj.vArr[i];
	delete[] obj.vArr;
	for (i = 0; i < obj.vt_num; i++) delete[] obj.vtArr[i];
	delete[] obj.vtArr;
	for (i = 0; i < obj.vn_num; i++) delete[] obj.vnArr[i];
	delete[] obj.vnArr;
	delete[] obj.objface;
}
void drawobj(object obj)
{
	int i = 0;
	GLint last_texture2D;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture2D);
	if (obj.textureID != 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, obj.textureID);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	glPushMatrix();
	glDisable(GL_COLOR_MATERIAL);

	for (i = 0; i < obj.face_num; i++)
	{
		int point_num = obj.objface[i].s;
		GLfloat obj_mtl_ambient[4] = { obj.objmtl[obj.objface[i].mtlindex].ka_rgb.r, obj.objmtl[obj.objface[i].mtlindex].ka_rgb.g,
			obj.objmtl[obj.objface[i].mtlindex].ka_rgb.b, 1.0f };
		GLfloat obj_mtl_diffuse[4] = { obj.objmtl[obj.objface[i].mtlindex].kd_rgb.r, obj.objmtl[obj.objface[i].mtlindex].kd_rgb.g,
			obj.objmtl[obj.objface[i].mtlindex].kd_rgb.b, 1.0f };
		GLfloat obj_mtl_specular[4] = { obj.objmtl[obj.objface[i].mtlindex].ks_rgb.r, obj.objmtl[obj.objface[i].mtlindex].ks_rgb.g,
			obj.objmtl[obj.objface[i].mtlindex].ks_rgb.b, 1.0f };
		GLfloat obj_mtl_emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat obj_mtl_shininess = 127;

		glMaterialfv(GL_FRONT, GL_AMBIENT, obj_mtl_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, obj_mtl_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, obj_mtl_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, obj_mtl_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, obj_mtl_shininess);			//Material.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (obj.objface[i].s == 3)
			glBegin(GL_TRIANGLES);
		else glBegin(GL_POLYGON);

		for (int k = 0; k < obj.objface[i].s; k++)
		{

			if (obj.textureID != 0)
			{
				glTexCoord2fv(obj.vtArr[obj.objface[i].Index[k][1]]);
			}
			glNormal3fv(obj.vnArr[obj.objface[i].Index[k][2]]);
			glVertex3fv(obj.vArr[obj.objface[i].Index[k][0]]);
		}
		glEnd();

	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, last_texture2D);
}

