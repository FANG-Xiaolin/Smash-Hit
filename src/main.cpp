#define FREEGLUT_STATIC
#include "GLee/GLee.h"
#include <ctime>
#include "objload.h"
#include "sphere.h"
#include "texture.h"
#include "lightandshadow.h"
#include "Maths/Maths.h"
#include "Objects.h"
#include "MoveAndHit.h"

#define WALK 0
#define GAME 1
#define MOVING_SPEED 0.2
#define fExtent 300
using namespace std;

static clock_t last = 0, current = 0;//last time and current time
bool mouseflag1, mouseflag2; //flag1 -> shoot the ball or not  ..  flag2 -> WALK mode .. 
GLint prex, prey;
bool transition = false;//diplay two rooms at the same time or not
GLint mode = GAME;
static int mouse_up = 1;
static double center_length = 0;
static int left_balls = 50;
GLuint texture_ID, skytextureID, skybox[6];

Room curroom, nextroom;
GLint nextroom_num;
vector<object> objects;
vector<Room> rooms;
vector<object> brokenglasseslr;
vector<object> brokenglassesud;
vector<object> brokendiamondslist;
void Shoot(GLint sx, GLint sy, GLdouble* MVmat, GLdouble* PJmat, GLint *VIEWpt);
void drawRoom(Room& room);
void drawsky();


void drawsky()
{
	glPushMatrix();
	glDisable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_2D);
	//up  
	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0, 1);
	glVertex3f(fExtent, fExtent, fExtent);
	glTexCoord2f(0, 0);
	glVertex3f(fExtent, -fExtent, fExtent);
	glTexCoord2f(1, 0);
	glVertex3f(-fExtent, -fExtent, fExtent);
	glTexCoord2f(1, 1);
	glVertex3f(-fExtent, fExtent, fExtent);
	glEnd();

	//down
	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 0);
	glVertex3f(-fExtent, fExtent, -fExtent);
	glTexCoord2f(1, 1);
	glVertex3f(-fExtent, -fExtent, -fExtent);
	glTexCoord2f(0, 1);
	glVertex3f(fExtent, -fExtent, -fExtent);
	glTexCoord2f(0, 0);
	glVertex3f(fExtent, fExtent, -fExtent);
	glEnd();


	//front  
	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0, 0);
	glVertex3f(fExtent, fExtent, -fExtent);
	glTexCoord2f(0, 1);
	glVertex3f(fExtent, -fExtent, -fExtent);
	glTexCoord2f(1, 1);
	glVertex3f(fExtent, -fExtent, fExtent);
	glTexCoord2f(1, 0);
	glVertex3f(fExtent, fExtent, fExtent);
	glEnd();


	//bock 
	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 1);
	glVertex3f(-fExtent, fExtent, fExtent);
	glTexCoord2f(0, 1);
	glVertex3f(-fExtent, -fExtent, fExtent);
	glTexCoord2f(0, 0);
	glVertex3f(-fExtent, -fExtent, -fExtent);
	glTexCoord2f(1, 0);
	glVertex3f(-fExtent, fExtent, -fExtent);
	glEnd();

	//left 
	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 1);
	glVertex3f(-fExtent, fExtent, fExtent);
	glTexCoord2f(1, 0);
	glVertex3f(-fExtent, fExtent, -fExtent);
	glTexCoord2f(0, 0);
	glVertex3f(fExtent, fExtent, -fExtent);
	glTexCoord2f(0, 1);
	glVertex3f(fExtent, fExtent, fExtent);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0, 0);
	glVertex3f(-fExtent, -fExtent, -fExtent);
	glTexCoord2f(0, 1);
	glVertex3f(-fExtent, -fExtent, fExtent);
	glTexCoord2f(1, 1);
	glVertex3f(fExtent, -fExtent, fExtent);
	glTexCoord2f(1, 0);
	glVertex3f(fExtent, -fExtent, -fExtent);
	glEnd();
	glDisable(GL_TEXTURE_2D);//关闭纹理  

	glPopMatrix();
}

void drawRoom(Room& room)
{
	drawobj(room.myroom);

	for (vector<Diamond>::iterator iter = room.this_room_Diamonds.begin(); iter != room.this_room_Diamonds.end(); iter++) 
	{
		if (!iter->isbroken&&iter->center.x <= eye[0]) iter->drawDiamond();
	}
	for (vector<FlipSlab>::iterator iter = room.this_room_FlipSlabs.begin(); iter != room.this_room_FlipSlabs.end(); iter++) 
	{
		if (iter->center.x <= eye[0]) 
		{
			if (!iter->isbroken) 
			{
				if (&room == &curroom) 
				{
					GLboolean flagfs = checkFlipSlab_camera(*iter, eye[0], eye[1], eye[2]);
					if (flagfs) {
						left_balls -= 5;
						cout << to_string(left_balls) << endl;
					}
				}
				iter->drawFlipSlab();
			}
		}
	}
	for (vector<LaserWall>::iterator iter = room.this_room_laserwall.begin(); iter != room.this_room_laserwall.end(); iter++)
	{
		if (iter->depth <= eye[0])
		{
			iter->drawLaserWall();
		}
	}
	for (vector<BrokenFlipSlab>::iterator iter = room.this_room_brokenFlipSlabs.begin(); iter != room.this_room_brokenFlipSlabs.end(); iter++) {
		if (iter->center.x  <= eye[0] ) {
			iter->drawbrokeFlipSlab();
		}
	}
	for (vector<BrokenDiamond>::iterator iter = room.this_room_brokenDiamonds.begin(); iter != room.this_room_brokenDiamonds.end(); iter++) {
		if (iter->center.x <= eye[0]) {
			iter->drawbrokeDiamond();
		}
	}
	glActiveTexture(GL_TEXTURE2);
	for (vector<Ball>::iterator iter = room.this_room_balls.begin(); iter != room.this_room_balls.end();) {
		if (iter->center.x + iter->var_x < eye[0] && iter->center.x + iter->var_x >= 0) {
			int hit = checkHit(*iter, curroom, eye[0], eye[1], eye[2]);//0-hit nothing important -5-hit flipslab -10-camera hit door 3-hit diamond
			if (hit != 0) {
				if (hit < 10) {
					if (hit <=-10) {
						//////////////////////////////////////broken diamond
						//Point brokencenter()
						Point brokencenter(room.this_room_Diamonds[hit / -10 - 1].center.x, room.this_room_Diamonds[hit / -10 - 1].center.y , room.this_room_Diamonds[hit / -10 - 1].center.z + room.this_room_Diamonds[hit / -10 - 1].var_z);
						Point hitloc(room.this_room_Diamonds[hit / -10 - 1].center.x, room.this_room_Diamonds[hit / -10 - 1].center.y, room.this_room_Diamonds[hit / -10 - 1].center.z + room.this_room_Diamonds[hit / -10 - 1].var_z+1);
						BrokenDiamond newbroken(brokencenter, hitloc, brokendiamondslist);
						room.this_room_brokenDiamonds.push_back(newbroken);
						left_balls += 3;
					}
					else {
						left_balls += hit;
						cout << to_string(left_balls) << endl;
					}
				}
				else {
					if (room.this_room_FlipSlabs[hit / 10 - 1].type == 1) {
						Point brokencenter(room.this_room_FlipSlabs[hit / 10 - 1].center.x, room.this_room_FlipSlabs[hit / 10 - 1].center.y + room.this_room_FlipSlabs[hit / 10 - 1].var_y, room.this_room_FlipSlabs[hit / 10 - 1].center.z + room.this_room_FlipSlabs[hit / 10 - 1].var_z);
						BrokenFlipSlab newbroken(brokencenter, room.this_room_FlipSlabs[hit / 10 - 1].width, room.this_room_FlipSlabs[hit / 10 - 1].height, brokenglasseslr);
						newbroken.crashP = room.this_room_FlipSlabs[hit / 10 - 1].HitLoc;
						room.this_room_brokenFlipSlabs.push_back(newbroken);
					}
					else {
						Point brokencenter(room.this_room_FlipSlabs[hit / 10 - 1].center.x, room.this_room_FlipSlabs[hit / 10 - 1].center.y + room.this_room_FlipSlabs[hit / 10 - 1].var_y, room.this_room_FlipSlabs[hit / 10 - 1].center.z - room.this_room_FlipSlabs[hit / 10 - 1].var_z);
						BrokenFlipSlab newbroken(brokencenter, room.this_room_FlipSlabs[hit / 10 - 1].width, room.this_room_FlipSlabs[hit / 10 - 1].height, brokenglassesud);
						newbroken.crashP = room.this_room_FlipSlabs[hit / 10 - 1].HitLoc;
						room.this_room_brokenFlipSlabs.push_back(newbroken);
					}
				}
			}
			iter->drawBall(skytextureID);
		}
		iter++;
	}
	glActiveTexture(GL_TEXTURE0);
	room.this_room_door.drawDoor();
	room.this_room_switch.drawSwitch();
	if (!room.this_room_switch.isopening) room.this_room_switch_fantasy.DrawParticles();
	//bool isshake = checkDoor(room.this_room_door, eye[0]);
	//if (isshake) shakeRoom(room);///////////////////////////////////////////////////
}

void Shoot(GLint sx, GLint sy, GLdouble* MVmat, GLdouble* PJmat, GLint *VIEWpt)
{
	GLdouble x, y, z;
	gluUnProject((GLdouble)sx, (GLdouble)sy, 1.0, MVmat, PJmat, VIEWpt, &x, &y, &z);
	glPushMatrix();
	GLdouble ratio1 = -(eye[1] - y) / (eye[0] - x);//y和x的坐标比
	GLdouble ratio2 = (eye[2] - z) / (eye[0] - x);//z和x的坐标比
	Point tmpp(eye[0], eye[1], eye[2]);
	Ball newball(tmpp, 1, ratio1, ratio2);
	curroom.this_room_balls.push_back(newball);
	left_balls--;
	return;
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bool flagdr = checkDoor_camera(curroom.this_room_door, eye[0]);
	if (flagdr == true)
	{
		left_balls -= 10;

	}
	for (vector<LaserWall>::iterator iter = curroom.this_room_laserwall.begin(); iter != curroom.this_room_laserwall.end(); iter++)
	{
		if (iter->depth <= eye[0])
		{
			bool flaglaser = checkLaserWall_camera(*iter, eye[0], eye[2]);
			if (flaglaser) left_balls -= 3;
		}
	}
	
	/*glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	drawsky()*/;

	/*Draw our shadow, use shadow mapping
	* draw for 3 times
	*/
	/****************************************1st**************************************/
	glActiveTexture(GL_TEXTURE1);//阴影贴图
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);
	glViewport(0, 0, shadowSize, shadowSize);//Use viewport the same size as the shadow map
	glCullFace(GL_FRONT);//Draw back faces into the shadow map
	glShadeModel(GL_FLAT);	//Disable color writes, and use flat shading for speed
	glColorMask(0, 0, 0, 0);
	glActiveTexture(GL_TEXTURE0);
	drawRoom(curroom);
	if (transition == true)
	{
		if (curroom.this_room_door.isopening)
		{
			glPushMatrix();
			glTranslatef(-nextroom.length, 0.0, 0.0);
			drawRoom(nextroom);
			glPopMatrix();
		}
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowtexture);//Read the depth buffer into the shadow map texture
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowSize, shadowSize);

	glCullFace(GL_BACK);//restore states
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	/****************************************2nd**************************************/
	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraViewMatrix);
	glViewport(0, 0, window_width, window_height);

	glLightfv(GL_LIGHT1, GL_POSITION, sun_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lowAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lowDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, black);//Use dim light to represent shadowed areas
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glActiveTexture(GL_TEXTURE0);
	drawRoom(curroom);
	//drawobj(obj);
	if (transition == true)
	{
		if (curroom.this_room_door.isopening)
		{
			glPushMatrix();
			glTranslatef(-nextroom.length, 0.0, 0.0);
			drawRoom(nextroom);
			glPopMatrix();
		}
	}
	glActiveTexture(GL_TEXTURE1);
	/****************************************3rd**************************************/
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun_specular);//Draw with bright light

													//Calculate texture matrix for projection
													//This matrix takes us from eye space to the light's clip space
													//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
	MATRIX4X4 textureMatrix = biasMatrix*lightProjectionMatrix*lightViewMatrix;//Set up texture coordinate generation.

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	glBindTexture(GL_TEXTURE_2D, shadowtexture);//Bind & enable shadow map texture
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);//Shadow comparison should generate an INTENSITY result
	glAlphaFunc(GL_GEQUAL, 0.99f);//Set alpha test to discard false comparisons
	glEnable(GL_ALPHA_TEST);

	glActiveTexture(GL_TEXTURE0);
	drawRoom(curroom);
	//drawobj(obj);
	if (transition == true)
	{
		if (curroom.this_room_door.isopening)
		{
			glPushMatrix();
			glTranslatef(-nextroom.length, 0.0, 0.0);
			drawRoom(nextroom);
			glPopMatrix();
		}
	}
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);//Disable textures and texgen

	glDisable(GL_LIGHTING);//Restore other states
	glDisable(GL_ALPHA_TEST);
	glActiveTexture(GL_TEXTURE0);
	drawsky();
	glActiveTexture(GL_TEXTURE1);

	/*Draw our windows
	*
	*//****************************************draw windows**************************************/
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (mode == GAME) {
		glRasterPos2f(-1.0f, 0.9f);
		string rest_balls_string = "Life : ";
		rest_balls_string = rest_balls_string + to_string(left_balls);
		for (unsigned int i = 0; i<rest_balls_string.length(); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, rest_balls_string[i]);//Print text
	}
	else if (mode == WALK) {
		glRasterPos2f(-1.0f, 0.9f);
		string rest_balls_string = "GAME OVER!\n";
		for (unsigned int i = 0; i<rest_balls_string.length(); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, rest_balls_string[i]);//Print text
	}



	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();//reset matrices


	glFinish();
	glutSwapBuffers();

}
void myIdle(void)
{
	if (mode == GAME) 
	{
		if (left_balls <= 0) {
			mode = WALK;
			return;
		}
		eye[0] -= MOVING_SPEED;
		if (eye[0] <= 30.0 || curroom.this_room_door.isopening == GL_TRUE)
		{
			if (transition == false) 
			{
				nextroom_num = rand() % rooms.size();
				nextroom = rooms[nextroom_num];
				transition = true;
			}
			if (eye[0] <= 0.05) 
			{
				//curroom.init();
				curroom = nextroom;
				eye[0] = nextroom.length - eye[0];
				transition = false;
			}
		}
		else if (curroom.this_room_door.isopening == GL_FALSE && curroom.this_room_switch.isopening == GL_TRUE) 
		{
			curroom.this_room_door.isopening = GL_TRUE;
		}
	}
	Move(curroom,eye[0]);
	Move(nextroom,eye[0]);
	current = clock();
	if ((current-last) / static_cast<double>(CLK_TCK)> 1.0/200)//控制帧数在60帧
	{
		last = current;
		shadowMatrixInit();
		glutPostRedisplay();
	}
	
}

void shadowMatrixInit()
{
	sun_position[0] = eye[0]+10;
	center[2] = eye[2];
	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)window_width / (GLfloat)window_height, 1.0f, 3000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], direct[0], direct[1], direct[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(60, 1.0f, 1.0f, 3000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(sun_position[0], sun_position[1], sun_position[2], center[0], center[1], center[2], direct[0], direct[1], direct[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
}
bool init()
{
	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		return false;
	}
	glActiveTexture(GL_TEXTURE0);//普通贴图
	string addr;
	objects.clear();
	rooms.clear();
	ifstream inobj("objectsList.txt");//存放xxx.obj文件列表
	inobj >> addr;
	while (addr != "END") {
		object newobject;
		readobj(addr, newobject);
		objects.push_back(newobject);
		inobj >> addr;
	}
	inobj.close();

	ifstream inroom("roomsList.txt");
	inroom >> addr;
	while (addr != "END") { //addr是房间名前缀，不用加.txt/.obj，里面会处理，存放房间名列表
		Room newroom(addr, objects);
		rooms.push_back(newroom);
		inroom >> addr;
	}
	inroom.close();

	ifstream inbroken("lrparts.txt");
	inbroken >> addr;
	while (addr != "END") {
		object brokenpiece;
		readobj(addr, brokenpiece);
		brokenglasseslr.push_back(brokenpiece);
		brokenglassesud.push_back(brokenpiece);
		inbroken >> addr;
	}

	ifstream inbrokendiamonds("brokenDiamonds.txt");
	inbrokendiamonds >> addr;
	while (addr != "END") {
		object brokenpiece;
		readobj(addr, brokenpiece);
		brokendiamondslist.push_back(brokenpiece);
		inbrokendiamonds >> addr;
	}

	nextroom = curroom = rooms[rand() % rooms.size()];
	//sky box
	glEnable(GL_TEXTURE_2D);
	glGenTextures(6, skybox);
	loadmytexture("sky_px.bmp", skybox[0]);
	loadmytexture("sky_nx.bmp", skybox[1]);
	loadmytexture("sky_py.bmp", skybox[2]);
	loadmytexture("sky_ny.bmp", skybox[3]);
	loadmytexture("sky_pz.bmp", skybox[4]);
	loadmytexture("sky_nz.bmp", skybox[5]);
	glDisable(GL_TEXTURE_2D);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, all_ambient[ambient_num]);
	/*Init
	*
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//Load identity modelview


	glShadeModel(GL_SMOOTH);//Shading states
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);//Depth states
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);//We use glScale when drawing the scene


	glEnable(GL_NORMALIZE);


	glGenTextures(1, &shadowtexture);//Create the shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowSize, shadowSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//Use the color as the ambient and diffuse material
	glEnable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 40.0f); //White specular material color, shininess 16
	glPushMatrix();

	shadowMatrixInit();//Calculate & save matrices
	glPopMatrix();

	glActiveTexture(GL_TEXTURE2);//skytexture
	glEnable(GL_TEXTURE_CUBE_MAP);

	glGenTextures(1, &skytextureID);
	if (!loadsky(skytextureID)) cout << "load sky fails" << endl;;
	cout << "skytextureID= " << skytextureID << endl;
	glDisable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);

	return(true);
}
void mousemove(int x, int y)
{
	//if(mouseflag1) shoot the ball!
	if (mouseflag2)
	{
		eye[2]+= (y - prey) / 3.0;
		eye[1] -= (x - prex) / 3.0;;
		prex = x;
		prey = y;
	}
	shadowMatrixInit();

}
void MouseUp(GLint button, GLint state, GLint x, GLint y)		 //Mouse click callback function.
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		mode = WALK;
		mouseflag2 = TRUE;
		prex = x;
		prey = y;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		mouseflag2 = FALSE;
	}

	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		GLdouble a[16];
		GLdouble b[16];
		GLint c[4];
		glGetDoublev(GL_MODELVIEW_MATRIX, a);
		glGetDoublev(GL_PROJECTION_MATRIX, b);
		glGetIntegerv(GL_VIEWPORT, c);
		Shoot(x, y, a, b, c);
		mouseflag1 = TRUE;
	}
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP) {
		mouseflag1 = FALSE;
	}
	shadowMatrixInit();
}
void keyboardCB(unsigned char key, int x, int y)
{
	if (mode == GAME) {
		if (key == 'p') mode = WALK;
	}
	if (mode == WALK) {
		switch (key)
		{
		case 'f': eye[0] -= 4.5; break;
		case 'b': eye[0] += 4.5; break;
		case 'w': if (eye[2] <= rooms[0].ceil) eye[2] += 2.5; break;
		case 's': if (eye[2] >= rooms[0].ground) eye[2] -= 2.5; break;
		case 'a': if (eye[1] >= rooms[0].left)  eye[1] -= 2.5; break;
		case 'd': if (eye[1] <= rooms[0].right) eye[1] += 2.5; break;
		}
	}
	shadowMatrixInit();
}
void reshape(int w, int h)
{
	//Update the camera's projection matrix
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(60.0f, (float)w / h, 1.0f, 3000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glPopMatrix();
}
int main(int argc, char *argv[])
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Smash Hit");
	if (!init())
		return 0;
	glutDisplayFunc(myDisplay);
	glutMouseFunc(MouseUp);
	glutMotionFunc(mousemove);
	glutKeyboardFunc(keyboardCB);
	glutIdleFunc(myIdle);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}