#pragma once
#define FREEGLUT_STATIC
#include "objload.h"
#include "sphere.h"
#include<vector>
#include<list>
#include<iterator>
#include<fstream>
using namespace std; 

#define ROOM_WIDTH 10
#define DEPTH_PER 20
#define ROOT2 1.41421
#define Pi 3.1415926
#define DEG_TO_RAD 0.017453

#define DIAMOND 0
#define FLSBlr 1
#define FLSBud 2

class Point
{
public:
	GLfloat x, y, z;
	Point(GLfloat a, GLfloat b, GLfloat c);
	Point();
};

class Cuboid
{
public:
	Point bottomcenter;
	GLfloat height;
	GLfloat width;
	GLfloat thickness;
	GLboolean ishere;
	Cuboid();
	Cuboid(Point c, GLfloat h, GLfloat w, GLfloat t);
	void drawCuboid(); 
};

class Cone
{
public:
	Point center; // coords of center of base of tetrahedron
	GLfloat radius;
	GLfloat height;
	GLboolean ishere;
	Cone();
	Cone(Point c, GLfloat h, GLfloat r);
	void drawCone();
};

class squareDoor //this can easily implement horizontal and vertical opening doors
{
public:
	GLfloat height, width, depth, thickness;
	GLboolean isopening;
	void openDoor();
	void drawDoor();
};

class Tube
{
public:
	GLfloat nums;
	GLfloat len1 = 25.0;
	GLfloat len2 = 15.0;
	Tube(GLfloat n);
	void drawTube();
	void drawshade1();
	void drawshade2();
};

class openDoorSwitch
{
public:
	Point center;
	GLboolean isopening;
	GLfloat var_y;
	object lefthalf;
	object righthalf;
	openDoorSwitch();
	openDoorSwitch(Point a,object left,object right);
	void drawSwitch();
};

//粒子结构：荧光和激光都使用这个class
class Particle// 创建粒子数据结构
{
public:
	GLboolean active;// 是否激活
	GLfloat life;// 粒子生命
	GLfloat fade;// 衰减速度

	GLfloat x;// X 位置
	GLfloat y;// Y 位置
	GLfloat z;// Z 位置

	GLfloat xi;// X 方向
	GLfloat yi;// Y 方向
	GLfloat zi;// Z 方向

	GLfloat xg;// X 方向重力加速度
	GLfloat yg;// Y 方向重力加速度
	GLfloat zg;// Z 方向重力加速度

	object myParticle;
};

//荧光
class Particles
{
public:
	Point center;
	Particle particle[100];
	Particles();
	Particles(Point a, object component);
	void DrawParticles();
};

//激光
class Laser
{
public:
	Point center;
	object Laserparticle;
	Particle laser[100];
	GLint life = 2;
	Laser();
	Laser(Point a, object objlight);
	void DrawLaser();
};

class LaserWall
{
public:
	GLfloat depth, width, height;
	GLint num;
	Laser lines[6];
	LaserWall();
	LaserWall(GLfloat d, GLfloat w, GLfloat h, Laser l);
	void drawLaserWall();
};

class Ball
{
public:
	Ball();
	Ball(Point a,GLfloat radius, GLfloat spdy_factor, GLfloat spdz_factor);
	Point center;
	GLfloat radius;
	GLfloat var_x, var_y, var_z, speed_x, speed_z, speed_y;
	GLfloat z_acc = -0.049;
	GLfloat x_acc, y_acc;
	//object myball;
	GLboolean isbroken;
	GLint isHit;
	void drawBall(GLuint textureID);
	mysphere sphere;
};

class FlipSlab
{
public:
	GLint type;//1--左右动， 2--上下动
	Point center;
	GLfloat var_y, var_z, moving_range;
	GLboolean isbroken, ismoving;
	GLint direction;//goes right -> 1 , goes left-> -1
	object myFlipSlab;
	GLfloat width, height;
	Point HitLoc;
	FlipSlab();
	FlipSlab(Point a, object fs, GLint type);
	void drawFlipSlab();
	void brokeFlipSlab();
};

class BrokenGlass
{
public:
	Point center;
	GLfloat xi;
	GLfloat yi;
	GLfloat zi;
	GLfloat xg;
	GLfloat yg;
	GLfloat zg;
	object myBrokenFlipSlab;
};

class BrokenFlipSlab
{
public:
	Point center, crashP;
	GLint time;
	GLfloat length, width;
	BrokenGlass BrokenGlass[16];
	BrokenFlipSlab();
	BrokenFlipSlab(Point a, GLfloat length, GLfloat width, vector<object> brokenfs);
	void velocityBrokenFlipSlab();
	void drawbrokeFlipSlab();
};

class Diamond
{
public:
	GLfloat var_z;
	Point center;
	GLint direction;//goes up -> 1 , goes down-> -1
	GLboolean isbroken;
	object mydiamond;
	Diamond();
	Diamond(Point a, object dmd);
	void drawDiamond();
	//void brokeDiamond();
};

class BrokenDiamond/////////////////////////////////////////////////////////
{
public:
	GLint time;
	Point center, crashP;
	BrokenGlass BrokenGlass[6];
	BrokenDiamond();
	BrokenDiamond(Point a,Point crash,vector<object> objs);
	void velocityBrokenDiamond();
	void drawbrokeDiamond();
};

class Door
{
public:
	GLboolean isopening, isHit;
	GLfloat depth;
	GLint type;
	vector<FlipSlab> mydoor;
	Door();
	Door(GLint type, GLfloat depth, vector<FlipSlab> door_component);//type: 1--两片门左右开 2--两片门上下 3--上下+左右
	void drawDoor();
};

class Room
{
public:
	Room();
	Room(string addr, vector<object> objs);
	GLfloat length;
	GLfloat left, right, ceil, ground;
	object myroom;
	object myball;
	void readRoomInfo(string info, vector<object> objs);
	vector<object> this_room_objects;
	vector<Diamond> this_room_Diamonds;
	vector<BrokenDiamond> this_room_brokenDiamonds;
	vector<FlipSlab> this_room_FlipSlabs;
	vector<BrokenFlipSlab> this_room_brokenFlipSlabs;
	vector<Ball> this_room_balls;
	vector<LaserWall> this_room_laserwall;
	Door this_room_door;
	openDoorSwitch this_room_switch;
	Particles this_room_switch_fantasy;
	void init();
};

