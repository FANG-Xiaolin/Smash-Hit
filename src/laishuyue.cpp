//碎玻璃
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
	Point center;
	GLfloat length, width;
	BrokenGlass BrokenGlass[16];
	BrokenFlipSlab();
	BrokenFlipSlab(Point a, GLfloat length, GLfloat width);
	void velocityBrokenFlipSlab(Point crashP);
	void drawbrokeFlipSlab(Point crashP);
};

BrokenFlipSlab::BrokenFlipSlab()
{
	Point a(0, 0, 0);
	center = a;
	length = 0;
	width = 0;
	for (int i = 0; i < 16; i++)
	{
		BrokenGlass[i].center = a;
		BrokenGlass[i].xi = 0;
		BrokenGlass[i].yi = 0;
		BrokenGlass[i].zi = 0;
		BrokenGlass[i].xg = 0;
		BrokenGlass[i].yg = 0;
		BrokenGlass[i].zg = 0;
		readobj("object3.obj", BrokenGlass[i].myBrokenFlipSlab);		//需要更改为对应的
	}
}

BrokenFlipSlab::BrokenFlipSlab(Point a, GLfloat length, GLfloat width)
{
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
		readobj("object3.obj", BrokenGlass[i].myBrokenFlipSlab);		//需要更改为对应的
	}
}

void BrokenFlipSlab::velocityBrokenFlipSlab(Point crashP)
{
	//设定速度和加速度
	for (int i = 0; i < 16; i++)
	{
		//重力加速度
		BrokenGlass[i].zg = -3.0;

		//初始速度
		if ((abs(crashP.y - BrokenGlass[i].center.y) + abs(crashP.z - BrokenGlass[i].center.z))< 1.0)
		{
			BrokenGlass[i].xi = -30.0 / (abs(crashP.y - BrokenGlass[i].center.y) + abs(crashP.z - BrokenGlass[i].center.z));
			BrokenGlass[i].yi = 3.0 / (BrokenGlass[i].center.y - crashP.y);
			BrokenGlass[i].zi = 3.0 / (BrokenGlass[i].center.z - crashP.z);
		}
		else
		{
			BrokenGlass[i].xi = 0;
			BrokenGlass[i].yi = 0;
			BrokenGlass[i].zi = 0;
		}
		
	}
}

//直接调用这个就可以了
void BrokenFlipSlab::drawbrokeFlipSlab(Point crashP)
{
	velocityBrokenFlipSlab(crashP);
	static GLint time = 0;
	for (int i = 0; i < 16; i++)
	{
		glPushMatrix();
		//画整一块玻璃碰撞瞬间的初始位置
		glTranslatef(BrokenGlass[i].center.x, BrokenGlass[i].center.y, BrokenGlass[i].center.z);
		if (time < 30)
		{
			glTranslatef(BrokenGlass[i].xi * time, BrokenGlass[i].yi * time * 3, BrokenGlass[i].zi * time + BrokenGlass[i].zg * time);
			glRotatef(33.0 + BrokenGlass[i].xi * time * 65, 0.5, 0.5, 1.0);
		}
		else
		{
			time = 0;
		}
		drawobj(BrokenGlass[i].myBrokenFlipSlab);
		glPopMatrix();
	}
	time++;
}


//碎钻石
class BrokenDiamond
{
public:
	Point center;
	BrokenGlass BrokenGlass[6];
	BrokenDiamond();
	BrokenDiamond(Point a);
	void velocityBrokenDiamond(Point crashP);
	void drawbrokeDiamond(Point crashP);
};

class Diamond
{
public:
	GLfloat var_z;
	Point center;
	GLint direction;//goes up -> 1 , goes down-> -1
	GLboolean ishere;
	object mydiamond;
	Diamond();
	Diamond(Point a, object dmd);
	void drawDiamond();
	void brokeDiamond();
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
	Particle particle[100];
	Particles(Point a);
	void DrawParticles();
};

//初始化粒子
Particles::Particles(Point a)
{
	GLint loop;
	for (loop = 0; loop < 50; loop++)
	{
		particle[loop].active = true;
		particle[loop].life = 1.0f;// 所有的粒子生命值为最大

		particle[loop].x = a.x;
		particle[loop].y = a.y;
		particle[loop].z = a.z;

		//随机生成fade衰减速率
		particle[loop].fade = 0.003f;// 随机生成衰减速率
		//particle[loop].fade = GLfloat(rand() % 100) / 1000.0f + 0.003f;// 随机生成衰减速率
		
		//对应速度
		particle[loop].xi = GLfloat((rand() % 50) - 26.0f);// 随机生成X轴方向速度
		particle[loop].yi = GLfloat((rand() % 50) - 25.0f);// 随机生成Y轴方向速度
		particle[loop].zi = GLfloat((rand() % 50) - 25.0f);// 随机生成Z轴方向速度
	
		//对应加速度
		particle[loop].xg = 0.0f;// 设置X轴方向加速度为0
		particle[loop].yg = -0.8f;//  设置Y轴方向加速度为-0.8
		particle[loop].zg = 0.0f;//  设置Z轴方向加速度为0

		readobj("object1.obj", particle[loop].myParticle);

	}
}

void Particles::DrawParticles()
{
	GLint loop;
	//GLfloat zoom = -40.0f;// 沿Z轴缩放
	GLfloat slowdown = 2.0f;// 减速粒子
	GLfloat xspeed = 1.0;// X方向的速度
	GLfloat yspeed = 1.0;// Y方向的速度
	for (loop = 0; loop<50; loop++)// 循环所有的粒子
	{
		glPushMatrix();
		if (particle[loop].active)// 如果粒子为激活的
		{
			//当前粒子坐标
			GLfloat x = particle[loop].x;// 返回X轴的位置
			GLfloat y = particle[loop].y;// 返回Y轴的位置
			GLfloat z = particle[loop].z;
			//GLfloat z = particle[loop].z + zoom;// 返回Z轴的位置

			//更新粒子坐标
			particle[loop].x = 0.0;
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
			particle[loop].fade = GLfloat(rand() % 100) / 1000.0f + 0.003f;// 随机生成衰减速率

			particle[loop].x = 0.0f;// 新粒子出现在屏幕的中央
			particle[loop].y = 0.0f;
			particle[loop].z = 0.0f;

			//particle[loop].xi = xspeed + GLfloat((rand() % 60) - 32.0f);// 随机生成粒子速度
			particle[loop].yi = yspeed + GLfloat((rand() % 60) - 30.0f);
			particle[loop].zi = GLfloat((rand() % 60) - 30.0f);
		}
		glTranslatef(particle[loop].x, particle[loop].y, particle[loop].z);
		drawobj(particle[loop].myParticle);
		glPopMatrix();
	}
}


//激光
class Light
{
public:
	Particle light[100];
	Light(Point a, object objlight);
	void DrawLight();
};

//初始化粒子
Light::Light(Point a, object objlight)
{
	GLint loop;
	for (loop = 0; loop < 20; loop++)
	{
		light[loop].active = true;
		light[loop].life = 1.0f;// 所有的粒子生命值为最大

		light[loop].x = a.x;
		light[loop].y = a.y;
		light[loop].z = a.z;

		//随机生成fade衰减速率
		light[loop].fade = 0.0001f;// 随机生成衰减速率
									 //particle[loop].fade = GLfloat(rand() % 100) / 1000.0f + 0.003f;// 随机生成衰减速率

									 //对应速度
		light[loop].xi = 0.0;// 随机生成X轴方向速度
		light[loop].yi = GLfloat((rand() % 50) - 25.0f);// 随机生成Y轴方向速度
		light[loop].zi = 0.0;// 随机生成Z轴方向速度

														   //对应加速度
		light[loop].xg = 0.0f;// 设置X轴方向加速度为0
		light[loop].yg = -0.8f;//  设置Y轴方向加速度为-0.8
		light[loop].zg = 0.0f;//  设置Z轴方向加速度为0

		light[loop].myParticle = objlight;
		//readobj("object3.obj", light[loop].myParticle);

	}
}

void Light::DrawLight()
{
	GLint loop;
	//GLfloat zoom = -40.0f;// 沿Z轴缩放
	GLfloat slowdown = 2.0f;// 减速粒子
	GLfloat xspeed = 1.0;// X方向的速度
	GLfloat yspeed = 1.0;// Y方向的速度
	for (loop = 0; loop<20; loop++)// 循环所有的粒子
	{
		glPushMatrix();
		if (light[loop].active)// 如果粒子为激活的
		{
			//当前粒子坐标
			GLfloat x = light[loop].x;// 返回X轴的位置
			GLfloat y = light[loop].y;// 返回Y轴的位置
			GLfloat z = light[loop].z;
			//GLfloat z = particle[loop].z + zoom;// 返回Z轴的位置

			//更新粒子坐标
			light[loop].x = 0.0;
			light[loop].y += light[loop].yi / (slowdown);
			light[loop].z += light[loop].zi / (slowdown);

			//更新粒子速度
			//particle[loop].xi += particle[loop].xg;
			light[loop].yi += light[loop].yg;
			//particle[loop].zi += particle[loop].zg;

			//更新粒子的生命值
			light[loop].life -= light[loop].fade;
		}

		else if (light[loop].life < 0.0f)// 如果粒子生命值小于0
		{
			light[loop].life = 1.0f;// 产生一个新的粒子
			light[loop].fade = GLfloat(rand() % 100) / 1000.0f + 0.003f;// 随机生成衰减速率

			light[loop].x = 0.0f;// 新粒子出现在屏幕的中央
			light[loop].y = 0.0f;
			light[loop].z = 0.0f;

			//particle[loop].xi = xspeed + GLfloat((rand() % 60) - 32.0f);// 随机生成粒子速度
			light[loop].yi = yspeed + GLfloat((rand() % 60) - 30.0f);
			light[loop].zi = GLfloat((rand() % 60) - 30.0f);
		}
		glTranslatef(light[loop].x, light[loop].y, light[loop].z);
		drawobj(light[loop].myParticle);
		glPopMatrix();
	}
}
