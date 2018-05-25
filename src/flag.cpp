#include <GL\glut.h>
#include <iostream>
#include <cmath>
#include <string>
#define pi 3.1415926
using namespace std;

void draw_star(const GLfloat *color, double x, double y, double R);//画星星,中心点坐标和半径
void color_triangle(const GLfloat *color, double x, double y, int length, int height);
void display(void);//完成图像

void display(void)
{
	int wide = 380, height = 200, x = 50, y = 50;
	double C, D, E, G, K, L;
	GLfloat blue[3] = { 0.0 / 255, 43.0 / 255, 127.0 / 255 };
	GLfloat red[3] = { 196.0 / 255, 30.0 / 255, 58.0 / 255 };
	GLfloat white[3] = { 1.0,1.0,1.0 };
	L = 1.0 / 13 * height;
	C = 7.0 / 13 * height;
	D = 2.0 / 5 * wide;
	E = C / 10;
	G = D / 12;
	K = 0.0616*height/2;//星星直径

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 13; i++)
	{
		if (i % 2 == 0)
		{
			color_triangle(red, x, y + i*L, wide, L);
		}
		else
		{
			color_triangle(white, x, y + i*L, wide, L);
		}
	}
	color_triangle(blue, x, y+height-C, D, C);
	for (int i = 0; i < 9; i++)
	{
		if (i % 2 == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				draw_star(white, x + (2 * j + 1)*G, y + height - (i + 1)*E, K);
			}
		}
		else
		{
			for (int j = 1; j <= 5; j++)
			{
				draw_star(white, x + (2 * j)*G, y + height - (i + 1)*E, K);
			}
		}
	}
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(x, y);
	glVertex2f(x, y + height);
	glVertex2f(x + wide, y + height);
	glVertex2f(x + wide, y);
	glEnd();
	glFlush();
}
void color_triangle(const GLfloat *color, double x, double y, int length, int height)
{
	GLfloat point[4][2];
	point[0][0] = point[3][0] = x; 
	point[0][1] = point[1][1] = y;
	point[1][0] = point[2][0] = x + length; 
	point[3][1] = point[2][1] = y + height;
	glColor3fv(color);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++)
	{
		glVertex2fv(point[i]);
	}
	glEnd();
}
void draw_star(const GLfloat *color,double x, double y, double K)
{
	GLfloat point[10][2];
	double theta;
	double length;
	double x1, y1;
	x1 = x - K*cos(18.0 / 360 * 2 * pi);
	y1 = y + K*sin(18.0 / 360 * 2 * pi);
	length = K* sin(36.0 / 360 * 2 * pi) / sin(126.0 / 360 * 2 * pi);
	point[0][0] = x1;
	point[0][1] = y1;
	int i;
	theta = 0;
	for (i = 1; i <= 9; i++)
	{
		if (i%2==1)
		{
			point[i][0] = point[i - 1][0] + length*cos(theta / 360 * 2 * pi);
			point[i][1] = point[i - 1][1] + length*sin(theta / 360 * 2 * pi);
			theta = theta + 72;
		}
		else
		{
			point[i][0] = point[i - 1][0] + length*cos(theta / 360 * 2 * pi);
			point[i][1] = point[i - 1][1] + length*sin(theta / 360 * 2 * pi);
			theta = theta - 144;
		}
	}
	glColor3fv(color);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < 5; i++)
	{
		glVertex2fv(point[(2 * i + 1)%10]);
		glVertex2fv(point[(2 * i + 2)%10]);
		glVertex2fv(point[(2 * i + 3)%10]);
	}
	glEnd();
	glBegin(GL_POLYGON);
	for (i = 0; i < 5; i++)
	{
		glVertex2fv(point[2 * i + 1]);
	}
	glEnd();
}
void ChangeSize(GLsizei w, GLsizei h)//备注，此函数为上网查询资料，摘录自博客
{
	if (h == 0)     h = 1;
	// 设置视区尺寸
	glViewport(0, 0, w, h);
	// 重置坐标系统
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 建立修剪空间的范围
	if (w <= h)
		glOrtho(0.0f, 250.0f, 0.0f, 250.0f*h / w, 1.0, -1.0);
	else
		glOrtho(0.0f, 250.0f*w / h, 0.0f, 250.0f, 1.0, -1.0);
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 400);//初始化window
	glutCreateWindow("USA_FLAG");

	glClearColor(1.0, 1.0, 1.0, 1.0);  // Set display-window color to white.刷新背景颜色，不影响画笔颜色
	glutDisplayFunc(display);

	glutReshapeFunc(ChangeSize);
	glMatrixMode(GL_PROJECTION);       // Set projection parameters.
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
	glutMainLoop();
	return 0;
}