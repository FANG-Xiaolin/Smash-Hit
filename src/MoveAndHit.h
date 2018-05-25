#pragma once
#define FREEGLUT_STATIC
#include"Objects.h"


const float THRESHOLD = 0.5; //Åö×²¼ì²â

void DiamondsMove(Room& myroom ,GLfloat lx); 
void FlipSlabMove(Room& myroom, GLfloat lx);
void DoorMove(Room& myroom, GLfloat lx);
void SwitchMove(Room& myroom, GLfloat lx);
void BallMove(Room& myroom, GLfloat lx);
void Move(Room& myroom, GLfloat lx);

int  checkHit(Ball& ball, Room& r, GLfloat lx, GLfloat ly, GLfloat lz);//0-hit nothing  important -5-hit flipslab -10-camera hit door 1-hit door switch 3-hit diamond
bool checkFlipSlab_ball(Ball& ball, FlipSlab& fs, GLfloat lx);
bool checkFlipSlab_camera(FlipSlab& fs, GLfloat lx, GLfloat ly, GLfloat lz);
bool checkRounding(Ball& ball, GLfloat left, GLfloat right, GLfloat ceil, GLfloat ground);//0- hit nothing, 1-hit left,2-hit right, 3-hit ceil, 4-hit ground
bool checkDiamond(Ball ball, Diamond& d, GLfloat lx);
bool checkSwitch(Ball& ball, openDoorSwitch& s, GLfloat lx);
bool checkDoor_camera(Door& d, GLfloat lx);
bool checkDoor_ball(Ball& ball, Door& d, GLfloat lx);
bool checkLaserWall(Ball& ball, LaserWall& lw, GLfloat lx);
bool checkLaserWall_camera(LaserWall& lw, GLfloat lx, GLfloat lz);
//////////////////////////////ÂþÓÎ×´Ì¬ÏÂµÄÅö×²¼ì²â

