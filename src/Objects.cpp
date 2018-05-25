#define FREEGLUT_STATIC
#include "Objects.h"

Point::Point(GLfloat a, GLfloat b, GLfloat c)
{
	x = a;
	y = b;
	z = c;
}

Point::Point()
{
	x = y = z = 0;
}




Cuboid::Cuboid()
{

}
	
Cuboid::Cuboid(Point c, GLfloat h, GLfloat w, GLfloat t)
{
	bottomcenter = c;
	height = h;
	width = w;
	thickness = t;
}

void Cuboid::drawCuboid()
{
	glPushMatrix();
	glTranslatef(bottomcenter.x, bottomcenter.y, bottomcenter.z + height / 2);
//	glutSolidCylinder(width, height, 20, 20);
	glPopMatrix();
}




Cone::Cone() 
{

}
	
Cone::Cone(Point c, GLfloat h, GLfloat r)
{
	center = c;
	height = h;
	radius = r;
}

void Cone::drawCone()
{
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	glutSolidCone(radius, height, 20, 10);
	glPopMatrix();
}



void squareDoor::drawDoor()
{
	isopening = true;
	glPushMatrix();
	Point lmid(depth, -width / 4, height / 2);
	Point rmid(depth, width / 4, height / 2);
	Cuboid left(lmid, height, width / 2, thickness);
	Cuboid right(rmid, height, width / 2, thickness);
	left.drawCuboid();
	right.drawCuboid();
	glPopMatrix();
}




Tube::Tube(GLfloat n)
{
	nums = n;
}

void Tube::drawshade1()
{
	GLfloat angle =0;
	Point plank[4];
	plank[0] = Point(0, -len1, len1);
	plank[1] = Point(0, len1, len1);
	plank[2] = Point(-DEPTH_PER, len1, len1);
	plank[3] = Point(-DEPTH_PER, -len1, len1);
	glBegin(GL_POLYGON);
	glVertex3f((plank[0]).x, (plank[0]).y, plank[0].z);
	glVertex3f((plank[1]).x, (plank[1]).y, plank[1].z);
	glVertex3f((plank[2]).x, (plank[2]).y, plank[2].z);
	glVertex3f((plank[3]).x, (plank[3]).y, plank[3].z);
	glEnd();
	for (int i = 2; i <= 8; i++) {
		angle += 45;
		plank[0] = plank[1];
		plank[3] = plank[2];
		plank[1] = Point(0, ROOT2*len1*cos(angle), ROOT2*len1*sin(angle));
		plank[2] = Point(-DEPTH_PER, ROOT2*len1*cos(angle), ROOT2*len1*sin(angle));
		glBegin(GL_POLYGON);
		glVertex3f((plank[0]).x, (plank[0]).y, plank[0].z);
		glVertex3f((plank[1]).x, (plank[1]).y, plank[1].z);
		glVertex3f((plank[2]).x, (plank[2]).y, plank[2].z);
		glVertex3f((plank[3]).x, (plank[3]).y, plank[3].z);
		glEnd();
	}

}

void Tube::drawshade2()
{
	GLfloat angle = 0;
	Point plank[4];
	plank[0] = Point(0, -len2, len2);
	plank[1] = Point(0, len2, len2);
	plank[2] = Point(-DEPTH_PER, len2, len2);
	plank[3] = Point(-DEPTH_PER, -len2, len2);
	glBegin(GL_POLYGON);
	glVertex3f((plank[0]).x, (plank[0]).y, plank[0].z);
	glVertex3f((plank[1]).x, (plank[1]).y, plank[1].z);
	glVertex3f((plank[2]).x, (plank[2]).y, plank[2].z);
	glVertex3f((plank[3]).x, (plank[3]).y, plank[3].z);
	glEnd();
	for (int i = 2; i <= 8; i++) {
		angle += 45;
		plank[0] = plank[1];
		plank[3] = plank[2];
		plank[1] = Point(0, ROOT2*len2*cos(angle), ROOT2*len2*sin(angle));
		plank[2] = Point(-DEPTH_PER, ROOT2*len2*cos(angle), ROOT2*len2*sin(angle));
		glBegin(GL_POLYGON);
		glVertex3f((plank[0]).x, (plank[0]).y, plank[0].z);
		glVertex3f((plank[1]).x, (plank[1]).y, plank[1].z);
		glVertex3f((plank[2]).x, (plank[2]).y, plank[2].z);
		glVertex3f((plank[3]).x, (plank[3]).y, plank[3].z);
		glEnd();
	}
}

void Tube::drawTube()
{
	glPushMatrix();
	for (int i = 0; i < nums; i++) {
		drawshade1();
		glTranslatef(-DEPTH_PER, 0, 0);
		drawshade2();
		glTranslatef(-DEPTH_PER, 0, 0);
	}
	glPopMatrix();
}




FlipSlab::FlipSlab()
{
	var_y = 0.0;
	var_z = 0.0;
	Point a(0, 0, 0);
	center = a;
	direction = -1;
	isbroken = false;
	ismoving = false;
	readobj("object3.obj", myFlipSlab);
}

FlipSlab::FlipSlab(Point a,object fs, GLint t)
{
	var_y = 0.0;
	var_z = 0.0;
	center = a;
	direction = -1;
	isbroken = false;
	ismoving = false;
	myFlipSlab = fs;
	type = t;
	if (type == 1) {
		width = 11.35;
		height = 24.35;
	}
	if (type == 2) {
		width = 18.65;
		height = 13.5;
	}
}

void FlipSlab::drawFlipSlab()
{
	if (isbroken == false) {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		if (ismoving) {
			switch (type){
			case 1:
				glTranslatef(0.0, var_y, 0.0);
			case 2:
				glTranslatef(0.0, 0.0, var_z);
			default:
				break;
			}
		}
		drawobj(myFlipSlab);
		glPopMatrix();
		return;
	}
	else {
		return;
	}
}

void FlipSlab::brokeFlipSlab()
{

}




openDoorSwitch::openDoorSwitch()
{

}

openDoorSwitch::openDoorSwitch(Point a, object left, object right)
{
	isopening = false;
	center = a;
	lefthalf = left;
	righthalf = right;
	var_y = 0.0;
}

void openDoorSwitch::drawSwitch()
{
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	if (isopening) {
		glPushMatrix();
		glTranslatef(0.0, -var_y, 0.0);
		drawobj(lefthalf);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0, var_y, 0.0);
		drawobj(righthalf);
		glPopMatrix();
	}
	else {
		drawobj(lefthalf);
		drawobj(righthalf);
	}
	glPopMatrix();
}




Ball::Ball()
{

}
//参数用你自己的，调一下！
Ball::Ball(Point a, GLfloat radius, GLfloat spdy_factor,GLfloat spdz_factor)
{
	center = a;
	this->radius = radius;
	var_x =	var_y = var_z = 0;
	speed_x = 1.25;
	speed_y = spdy_factor*speed_x;
	speed_z = spdz_factor*speed_x;
	x_acc = y_acc = 1.0;
	z_acc = -0.005;
	//myball = myb;
	isbroken = false;
}

void Ball::drawBall(GLuint textureID)
{
	static mysphere sphere;
	if (!isbroken) {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glTranslatef(var_x, var_y, var_z);
		sphere.textureID = textureID;
		sphere.draw(radius);
		glPopMatrix();
	}
}




Door::Door()
{

}

Door::Door(GLint type, GLfloat depth, vector<FlipSlab> door_component)//////////////////////////////////
{
	isopening = isHit = false;
	this->type = type;
	this->depth = depth;
	mydoor = door_component;

}

void Door::drawDoor()
{
	for (vector<FlipSlab>::iterator iter = mydoor.begin(); iter != mydoor.end(); iter++) {
		iter->drawFlipSlab();
	}
}




Diamond::Diamond()
{
	var_z=(int)(rand() / 2147483647.0) * 4;
	center.x = center.y = center.z = 0;
	direction = 1;
	isbroken = false;
	//readobj("object1.obj", mydiamond);////////////////////////////////////////////////////////////////////////
}

Diamond::Diamond(Point a, object dmd)
{
	var_z = (int)(rand()*1.0 /RAND_MAX*4);
	center = a;
	direction = (rand() % 2 == 0) ? -1 : 1;
	isbroken = false;
	mydiamond = dmd;////////////////////////////////////////////////////////////////////////
}

void Diamond::drawDiamond()
{
	if (!isbroken) 
	{
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glTranslatef(0.0, 0.0, var_z);
		drawobj(mydiamond);
		glPopMatrix();
	}
}

BrokenDiamond::BrokenDiamond()
{
	//Point a(0, 0, 0);
	//center = a;
	//for (int i = 0; i < 6; i++)
	//{
	//	BrokenGlass[i].center = a;
	//	BrokenGlass[i].xi = 0;
	//	BrokenGlass[i].yi = 0;
	//	BrokenGlass[i].zi = 0;
	//	BrokenGlass[i].xg = 0;
	//	BrokenGlass[i].yg = 0;
	//	BrokenGlass[i].zg = 0;
	//	readobj("object3.obj", BrokenGlass[i].myBrokenFlipSlab);		//需要更改为对应的
	//}
}

BrokenDiamond::BrokenDiamond(Point a, Point crash, vector<object> objs)
{
	time = 0;
	this->center = a;
	this->crashP = crash;
	GLfloat length = 3.0;
	GLfloat width = 7.0;
	GLfloat tempy = a.y - length * 1 / 4;
	GLfloat tempz = a.z + width * 1 / 4;

	BrokenGlass[0].center.y = tempy;
	BrokenGlass[0].center.z = tempz;
	BrokenGlass[1].center.y = tempy + length / 2;
	BrokenGlass[1].center.z = tempz;
	BrokenGlass[2].center.y = tempy;
	BrokenGlass[2].center.z = tempz - width / 2;
	BrokenGlass[3].center.y = tempy + length / 2;
	BrokenGlass[3].center.z = tempz - width / 2;

	for (int i = 0; i < 4; i++)
	{
		BrokenGlass[i].center.x = a.x;
		BrokenGlass[i].xi = 0;
		BrokenGlass[i].yi = 0;
		BrokenGlass[i].zi = 0;
		BrokenGlass[i].xg = 0;
		BrokenGlass[i].yg = 0;
		BrokenGlass[i].zg = 0;
		BrokenGlass[i].myBrokenFlipSlab = objs[i];		//需要更改为对应的
	}

}

void BrokenDiamond::velocityBrokenDiamond()
{
	//设定速度和加速度
	for (int i = 0; i < 4; i++)
	{
		//重力加速度
		BrokenGlass[i].zg = -3.0;

		//初始速度
		BrokenGlass[i].xi = center.x;
		BrokenGlass[i].yi = 0.003 / (BrokenGlass[i].center.y - crashP.y);
		BrokenGlass[i].zi = 0.003 / (BrokenGlass[i].center.z - crashP.z);
	}
}

void BrokenDiamond::drawbrokeDiamond()
{
	time++;
	if (time >= 50) return;
	velocityBrokenDiamond();
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		//画整一块玻璃碰撞瞬间的初始位置
		glTranslatef(BrokenGlass[i].center.x, BrokenGlass[i].center.y, BrokenGlass[i].center.z);
		if (time < 50)
		{
			glTranslatef(0.0 , BrokenGlass[i].yi * time * 0.003, (BrokenGlass[i].zi + BrokenGlass[i].zg) * time * 0.03);
			glRotatef(BrokenGlass[i].xi * time * 65, 0.5, 0.5, 1.0);
			drawobj(BrokenGlass[i].myBrokenFlipSlab);
		}
		else
		{
			glPopMatrix();
			break;
		}
		glPopMatrix();
	}

}

Room::Room()
{

}

Room::Room(string addr,vector<object> objs)
{
	string obj, info;
	obj = addr + ".obj";
	info = addr + ".txt";
	readobj(obj, myroom);
	readRoomInfo(info, objs);
}

void Room::readRoomInfo(string info, vector<object> objs)
{
	GLfloat a, b, c;
	GLint type;
	ifstream roominfo(info);
	roominfo >> length;//长度
	roominfo >> a >> b >> c;//门开关位置
	Point doorswitch(a, b, c);
	roominfo >> this->left >> this->right >> this->ceil >> this->ground;//左右上下墙壁
	roominfo >> a;
	while (a != -1.0) {
		roominfo >> b >> c;
		Point loc(a, b, c);
		Diamond newdiamond(loc,objs[DIAMOND]);
		this_room_Diamonds.push_back(newdiamond);
		roominfo >> a;
	}
	roominfo >> a;
	while (a != -2.0) {
		roominfo >> b >> c >> type;
		Point loc(a, b, c);
		if (type == 1) {
			FlipSlab newflipslab(loc, objs[FLSBlr], type);
			newflipslab.ismoving = true;
			this_room_FlipSlabs.push_back(newflipslab);
		}
		else if (type == 2) {
			FlipSlab newflipslab(loc, objs[FLSBud], type);
			newflipslab.ismoving = true;
			this_room_FlipSlabs.push_back(newflipslab);
		}
		roominfo >> a;
	}
	roominfo >> type;
	vector<FlipSlab> doorcompo;
	Point tmpcenter;
	if (type == 1) {
		FlipSlab newfrac1(tmpcenter, objs[3], 1);
		doorcompo.push_back(newfrac1);
		FlipSlab newfrac2(tmpcenter, objs[4], 1);
		newfrac2.direction = -newfrac1.direction;
		doorcompo.push_back(newfrac2);
	}
	else if (type == 2) {
		FlipSlab newfrac1(tmpcenter, objs[5], 2);
		newfrac1.direction = -newfrac1.direction;
		doorcompo.push_back(newfrac1);
		FlipSlab newfrac2(tmpcenter, objs[6], 2);
		newfrac2.direction = -newfrac1.direction;
		doorcompo.push_back(newfrac2);
	}
	else {
		FlipSlab newfrac1(tmpcenter, objs[3], 1);
		doorcompo.push_back(newfrac1);
		FlipSlab newfrac2(tmpcenter, objs[4], 1);
		newfrac2.direction = -newfrac1.direction;
		doorcompo.push_back(newfrac2);
		FlipSlab newfrac3(tmpcenter, objs[5], 2);
		newfrac3.direction = -newfrac3.direction;
		doorcompo.push_back(newfrac3);
		FlipSlab newfrac4(tmpcenter, objs[6], 2);
		newfrac4.direction = -newfrac3.direction;
		doorcompo.push_back(newfrac4);
	}
	Door tmpdoor(type, length, doorcompo);
	this_room_door = tmpdoor;
	openDoorSwitch tmpswitch(doorswitch, objs[7], objs[8]);
	this_room_switch = tmpswitch;
	myball = objs[9];
	Particles tmpswitchfantasy(doorswitch, objs[10]);
	this_room_switch_fantasy = tmpswitchfantasy;
	GLfloat laserdepth;
	roominfo >> laserdepth;
	while (laserdepth != -3.0) {
		Point laser_init_depth;
		Laser tmplaser(laser_init_depth, objs[11]);
		LaserWall tmplaserwall(laserdepth, right - left, ceil - ground, tmplaser);
		this_room_laserwall.push_back(tmplaserwall);
		roominfo >> laserdepth;
	}
}

void Room::init()
{
	for (vector<Diamond>::iterator iter = this_room_Diamonds.begin(); iter != this_room_Diamonds.end(); iter++) {
		iter->isbroken = false;
	}
	for (vector<FlipSlab>::iterator iter = this_room_FlipSlabs.begin(); iter != this_room_FlipSlabs.end(); iter++) {
		iter->isbroken = false;
	}
	this_room_balls.clear();
	this_room_door.isopening = false;
	this_room_switch.isopening = false;
}




BrokenFlipSlab::BrokenFlipSlab()
{
	//Point a(0, 0, 0);
	//center = a;
	//length = 0;
	//width = 0;
	//for (int i = 0; i < 16; i++)
	//{
	//	BrokenGlass[i].center = a;
	//	BrokenGlass[i].xi = 0;
	//	BrokenGlass[i].yi = 0;
	//	BrokenGlass[i].zi = 0;
	//	BrokenGlass[i].xg = 0;
	//	BrokenGlass[i].yg = 0;
	//	BrokenGlass[i].zg = 0;
	//	readobj("object3.obj", BrokenGlass[i].myBrokenFlipSlab);		//需要更改为对应的
	//}
}

BrokenFlipSlab::BrokenFlipSlab(Point a, GLfloat length, GLfloat width,vector<object> brokenfs)
{
	//cout << "newwwww" << endl;
	time = 0;
	this->center = a;
	this->length = length;
	this->width = width;
	GLfloat tempy = a.y - length * 3 / 8;
	GLfloat tempz = a.z + width * 3 / 8;
	for (int i = 0; i < 16; i++)
	{
		BrokenGlass[i].center.x = a.x;
		BrokenGlass[i].center.y = tempy + (i % 4) * length / 4;
		BrokenGlass[i].center.z = tempz - (i / 4) * width / 4;
		BrokenGlass[i].xi = 0;
		BrokenGlass[i].yi = 0;
		BrokenGlass[i].zi = 0;
		BrokenGlass[i].xg = 0;
		BrokenGlass[i].yg = 0;
		BrokenGlass[i].zg = 0;
		BrokenGlass[i].myBrokenFlipSlab=brokenfs[i];		
	}
}

void BrokenFlipSlab::velocityBrokenFlipSlab()
{
	//设定速度和加速度
	for (int i = 0; i < 16; i++)
	{
		//重力加速度
		BrokenGlass[i].zg = -0.1;

		//初始速度
		if ((abs(crashP.y - BrokenGlass[i].center.y)<2.1*length/4)  && (abs(crashP.z - BrokenGlass[i].center.z)< 2.1 * width/4))
		{
			BrokenGlass[i].xi = -0.5 / (abs(crashP.y - BrokenGlass[i].center.y) + abs(crashP.z - BrokenGlass[i].center.z));
			BrokenGlass[i].yi = 0.3 / (BrokenGlass[i].center.y - crashP.y);
			BrokenGlass[i].zi = 0.3 / (BrokenGlass[i].center.z - crashP.z);
		}
		else
		{
			BrokenGlass[i].xi = 0;
			BrokenGlass[i].yi = 0;
			BrokenGlass[i].zi = 0;
		}

	}
}

void BrokenFlipSlab::drawbrokeFlipSlab()
{
	time++;
	if (time >= 50) return;
	velocityBrokenFlipSlab();
	//static GLint time = 0;
	for (int i = 0; i < 16; i++)
	{
		glPushMatrix();
		//画整一块玻璃碰撞瞬间的初始位置
		glTranslatef(BrokenGlass[i].center.x, BrokenGlass[i].center.y, BrokenGlass[i].center.z);
		if (time < 200)
		{
			//time++;
			//cout << time << endl;
			if(time%10)
			glTranslatef(BrokenGlass[i].xi * time*0.3, BrokenGlass[i].yi * time *0.3, BrokenGlass[i].zi * time*0.3 + BrokenGlass[i].zg * time*0.3);
			glRotatef( BrokenGlass[i].zi * time * 10, 0.2, 0.4, 0.77);
			//cout << to_string(i) << "  ddddd " << BrokenGlass[i].yi << endl;
			drawobj(BrokenGlass[i].myBrokenFlipSlab);
		}
		else
		{
			glPopMatrix();
			break;
		}
		glPopMatrix();
	}
	//time++;
}




Particles::Particles()
{

}
//初始化粒子
Particles::Particles(Point a, object component)
{
	GLint loop;
	center = a;
	for (loop = 0; loop < 10; loop++)
	{
		//particle[loop].active = true;
		particle[loop].life = 1.0f;// 所有的粒子生命值为最大

		particle[loop].x = a.x;
		particle[loop].y = a.y;
		particle[loop].z = a.z;

		//随机生成fade衰减速率
		//particle[loop].fade = 0.1;// 随机生成衰减速率
		particle[loop].fade = (GLfloat(rand() % 2 + 1) / 4.0f) / 1.5;// 随机生成衰减速率

																	 //对应速度
																	 //particle[loop].xi = GLfloat(0.01*((rand() % 5) - 2.60f));// 随机生成X轴方向速度
		particle[loop].xi = 0;// 随机生成X轴方向速度
		particle[loop].yi = GLfloat(0.05*((rand() % 5) - 2.50f));// 随机生成Y轴方向速度
		particle[loop].zi = GLfloat(0.05*((rand() % 5) - 2.50f));// 随机生成Z轴方向速度

																 //对应加速度
		particle[loop].xg = 0.0f;// 设置X轴方向加速度为0
								 //particle[loop].yg = -0.08f;//  设置Y轴方向加速度为-0.8
		particle[loop].yg = 0.0f;//  设置Y轴方向加速度为-0.8
		particle[loop].zg = 0.0f;//  设置Z轴方向加速度为0

								 //readobj("object1.obj", particle[loop].myParticle);
		particle[loop].myParticle = component;
	}
}

void Particles::DrawParticles()
{
	GLint loop;
	//GLfloat zoom = -40.0f;// 沿Z轴缩放
	GLfloat slowdown = 0.2f;// 减速粒子
	GLfloat xspeed = 0.1;// X方向的速度
	GLfloat yspeed = 0.1;// Y方向的速度
	for (loop = 0; loop<10; loop++)// 循环所有的粒子
	{
		glPushMatrix();
		if (particle[loop].life >= 0.0f)// 如果粒子为激活的
		{
			//当前粒子坐标
			GLfloat x = particle[loop].x;// 返回X轴的位置
			GLfloat y = particle[loop].y;// 返回Y轴的位置
			GLfloat z = particle[loop].z;
			//GLfloat z = particle[loop].z + zoom;// 返回Z轴的位置

			//更新粒子坐标
			particle[loop].x = center.x;
			particle[loop].y += particle[loop].yi / (slowdown);
			particle[loop].z += particle[loop].zi / (slowdown);

			//更新粒子速度
			//particle[loop].xi += particle[loop].xg;
			particle[loop].yi += particle[loop].yg;
			particle[loop].zi += particle[loop].zg;

			//更新粒子的生命值
			particle[loop].life -= particle[loop].fade;
		}

		else if (particle[loop].life < 0.0f)// 如果粒子生命值小于0
		{

			particle[loop].life = 1.0f;// 产生一个新的粒子
									   //particle[loop].fade = 0.05;// 随机生成衰减速率
			particle[loop].fade = (GLfloat(rand() % 2 + 1) / 4.0f) / 1.5;// 随机生成衰减速率

			particle[loop].x = center.x;// 新粒子出现在屏幕的中央
			particle[loop].y = center.y;
			particle[loop].z = center.z;


			//particle[loop].xi = xspeed + GLfloat((rand() % 60) - 32.0f);// 随机生成粒子速度
			particle[loop].xi = 0;// 随机生成X轴方向速度
			particle[loop].yi = GLfloat(0.05*((rand() % 5) - 2.50f));// 随机生成Y轴方向速度
			particle[loop].zi = GLfloat(0.05*((rand() % 5) - 2.50f));// 随机生成Z轴方向速度
		}
		glTranslatef(particle[loop].x, particle[loop].y, particle[loop].z);
		drawobj(particle[loop].myParticle);
		glPopMatrix();
	}
}

Laser::Laser()
{

}

Laser::Laser(Point a, object objlight)
{
	center = a;
	Laserparticle = objlight;
	GLint loop;
	for (loop = 0; loop < 20; loop++)
	{
		//laser[loop].active = true;
		laser[loop].life = 1.0f;// 所有的粒子生命值为最大

		laser[loop].x = a.x;
		laser[loop].y = a.y;
		laser[loop].z = a.z;

		//随机生成fade衰减速率
		laser[loop].fade = 0.01f;// 随机生成衰减速率
								   //particle[loop].fade = GLfloat(rand() % 100) / 1000.0f + 0.003f;// 随机生成衰减速率

								   //对应速度
		laser[loop].xi = 0.0;// 随机生成X轴方向速度
		//laser[loop].yi = 5.0;
		laser[loop].yi = GLfloat((rand() % 50) - 25.0f) * 10;// 随机生成Y轴方向速度
		laser[loop].zi = 0.0;// 随机生成Z轴方向速度

							 //对应加速度
		//laser[loop].xg = 0.0f;// 设置X轴方向加速度为0
		//laser[loop].yg = 0.0f;//  设置Y轴方向加速度为-0.8
		//laser[loop].zg = 0.0f;//  设置Z轴方向加速度为0

		laser[loop].myParticle = objlight;
		//readobj("object3.obj", light[loop].myParticle);

	}
}

void Laser::DrawLaser()
{
	if (life > 0) {
		GLint loop;
		//GLfloat zoom = -40.0f;// 沿Z轴缩放
		GLfloat slowdown = 2.0f;// 减速粒子
		//GLfloat xspeed = 0.01;// X方向的速度
		GLfloat yspeed = 0.01;// Y方向的速度
		for (loop = 0; loop<20; loop++)// 循环所有的粒子
		{
			glPushMatrix();
			if (laser[loop].life >=0.0f)// 如果粒子为激活的
			{
				//当前粒子坐标
				GLfloat x = laser[loop].x;// 返回X轴的位置
				GLfloat y = laser[loop].y;// 返回Y轴的位置
				GLfloat z = laser[loop].z;
				//GLfloat z = particle[loop].z + zoom;// 返回Z轴的位置

				//更新粒子坐标
				laser[loop].x = center.x;
				laser[loop].y += 0.1*laser[loop].yi / (slowdown);
				laser[loop].z += laser[loop].zi / (slowdown);

				//更新粒子速度
				//particle[loop].xi += particle[loop].xg;
				//laser[loop].yi += laser[loop].yg;
				//particle[loop].zi += particle[loop].zg;

				//更新粒子的生命值
				laser[loop].life -= laser[loop].fade;
			}

			else if (laser[loop].life < 0.0f)// 如果粒子生命值小于0
			{
				laser[loop].life = 1.0f;// 产生一个新的粒子
				////laser[loop].fade = GLfloat(rand() % 100) / 1000.0f + 0.003f;// 随机生成衰减速率
				//laser[loop].fade = 0.1;
				laser[loop].fade = 0.01;// 随机生成衰减速率
				laser[loop].x = center.x;// 新粒子出现在屏幕的中央
				laser[loop].y = center.y;
				laser[loop].z = center.z;

				//particle[loop].xi = xspeed + GLfloat((rand() % 60) - 32.0f);// 随机生成粒子速度
				/*laser[loop].yi = yspeed + GLfloat((rand() % 60) - 30.0f);
				laser[loop].zi = GLfloat((rand() % 60) - 30.0f);*/
				//laser[loop].yi = 5.0;// 随机生成Y轴方向速度
				laser[loop].yi = yspeed + GLfloat((rand() % 60) - 30.0f);
				laser[loop].zi = 0;// 随机生成Z轴方向速度
			}
			glTranslatef(laser[loop].x, laser[loop].y, laser[loop].z);
			drawobj(laser[loop].myParticle);
			glPopMatrix();
		}
	}
}

LaserWall::LaserWall()
{

}

LaserWall::LaserWall(GLfloat d, GLfloat w, GLfloat h, Laser l)
{
	depth = d;
	width = w;
	height = h;
	GLfloat offset = h / 6;
	for (int i = 0; i < 6; i++) {
		l.center.x = depth;
		l.center.y = -width / 2;
		l.center.z = -offset*(i + 1);
		lines[i] = l;
	}
}

void LaserWall::drawLaserWall()
{
	for (int i = 0; i < 6; i++) {
		if(lines[i].life > 0)
			lines[i].DrawLaser();
	}
}




















//
//void Room::drawRoom()
//{
//	drawobj(myroom);
//	for (vector<Diamond>::iterator iter = this_room_Diamonds.begin(); iter != this_room_Diamonds.end(); iter++) {
//		iter->drawDiamond();
//	}
//	for (vector<FlipSlab>::iterator iter = this_room_FlipSlabs.begin(); iter != this_room_FlipSlabs.end(); iter++) {
//		iter->drawFlipSlab();
//	}
//	for (vector<Ball>::iterator iter = this_room_balls.begin(); iter != this_room_balls.end(); iter++) {
//		iter->drawBall();
//	}
//	this_room_door.drawDoor();
//	this_room_switch.drawSwitch();
//}


//
//Room::Room()
//{
//
//}
//
//Room::Room(GLfloat d)
//{
//	depth = d;
//}
//
//
//void Room::drawWall(Point * Wall)
//{
//	glBegin(GL_POLYGON);
//	glVertex3f(Wall[0].x, Wall[0].y, Wall[0].z);
//	glVertex3f(Wall[1].x, Wall[1].y, Wall[1].z);
//	glVertex3f(Wall[2].x, Wall[2].y, Wall[2].z);
//	glVertex3f(Wall[3].x, Wall[3].y, Wall[3].z);
//	glEnd();
//}
//
//void Room::drawRoom()
//{
//	glPushMatrix();
//	Point* wall = (Point *)malloc(sizeof(Point) * 4);
//
//	//wall[0] = Point(0, 20 / 2, 20 / 2);
//	//wall[1] = Point(0, 20/ 2, -20 / 2);
//	//wall[2] = Point(0, -20 / 2, -20 / 2);
//	//wall[3] = Point(0, -20 / 2, 20 / 2);
//	//drawWall(wall);
//
//
//	wall[0] = Point(0, ROOM_WIDTH / 2, ROOM_WIDTH / 2);
//	wall[1] = Point(-depth, ROOM_WIDTH / 2, ROOM_WIDTH / 2);
//	wall[2] = Point(-depth, -ROOM_WIDTH / 2, ROOM_WIDTH / 2);
//	wall[3] = Point(0, -ROOM_WIDTH / 2, ROOM_WIDTH / 2);
//	drawWall(wall);
//
//	wall[0] = wall[3];
//	wall[1] = wall[2];
//	wall[2] = Point(-depth, -ROOM_WIDTH / 2, -ROOM_WIDTH / 2);
//	wall[3] = Point(0, -ROOM_WIDTH / 2, -ROOM_WIDTH / 2);
//	drawWall(wall);
//
//	wall[0] = wall[3];
//	wall[1] = wall[2];
//	wall[2] = Point(-depth, ROOM_WIDTH / 2, -ROOM_WIDTH / 2);
//	wall[3] = Point(0, ROOM_WIDTH / 2, -ROOM_WIDTH / 2);
//	drawWall(wall);
//
//	wall[0] = wall[3];
//	wall[1] = wall[2];
//	wall[2] = Point(-depth, ROOM_WIDTH / 2, ROOM_WIDTH / 2);
//	wall[3] = Point(0, ROOM_WIDTH / 2, ROOM_WIDTH / 2);
//	drawWall(wall);
//
//	free(wall);
//	glPopMatrix();
//}

