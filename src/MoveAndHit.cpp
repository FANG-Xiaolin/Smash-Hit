#define FREEGLUT_STATIC
#include"MoveAndHit.h"

void DiamondsMove(Room& myroom, GLfloat lx)
{
	for (vector<Diamond>::iterator iter = myroom.this_room_Diamonds.begin(); iter != myroom.this_room_Diamonds.end(); iter++) 
	{
		if (iter->center.x < lx) {
			iter->var_z += 0.1*iter->direction;
			if (iter->var_z >= 4.0 || iter->var_z <= -4.0)iter->direction = -iter->direction;
		}
	}
}

void FlipSlabMove(Room& myroom, GLfloat lx)
{
	for (vector<FlipSlab>::iterator iter = myroom.this_room_FlipSlabs.begin(); iter != myroom.this_room_FlipSlabs.end(); iter++) {
		if (!iter->isbroken && iter->center.x < lx) {
			switch (iter->type)
			{
			case 1:
				iter->var_y += 0.15*iter->direction;
				if (iter->var_y >= 13.0 || iter->var_y <= -13.0)iter->direction = -iter->direction;
				break;
			case 2:
				iter->var_z += 0.13*iter->direction;
				if (iter->var_z >= 10.0 || iter->var_z <= -10.0)iter->direction = -iter->direction;
				break;
			default:
				break;
			}
		}
	}
}

void DoorMove(Room& myroom, GLfloat lx)
{
	if (myroom.this_room_door.isopening == GL_TRUE) {
		for (vector<FlipSlab>::iterator iter = myroom.this_room_door.mydoor.begin(); iter != myroom.this_room_door.mydoor.end(); iter++) {
			iter->ismoving = true;
			switch (iter->type)
			{
			case 1:iter->var_y += 0.15*iter->direction; break;
			case 2:iter->var_z += 0.15*iter->direction; break;
			default:
				break;
			}
			
		}
	}
}

void SwitchMove(Room& myroom, GLfloat lx)
{
	if (myroom.this_room_switch.isopening == GL_TRUE) {
		myroom.this_room_switch.var_y += 0.02;
	}
}

void BallMove(Room& myroom, GLfloat lx)
{
	for (vector<Ball>::iterator iter = myroom.this_room_balls.begin(); iter != myroom.this_room_balls.end(); iter++) 
	{
		if (!iter->isbroken)
		{
			iter->speed_z += iter->z_acc;
			iter->var_x -= iter->speed_x*iter->x_acc;
			iter->var_y += iter->speed_y*iter->y_acc;
			iter->var_z += iter->speed_z;
		}
	}
}

void Move(Room& myroom, GLfloat lx)
{
	BallMove(myroom,lx);
	DiamondsMove(myroom,lx);
	FlipSlabMove(myroom,lx);
	DoorMove(myroom,lx);
	SwitchMove(myroom,lx);
}

int checkHit(Ball& ball, Room& r, GLfloat lx, GLfloat ly, GLfloat lz)
{
	bool flagswit, flagdmd, flagbroken, flaglas;
	flagswit = flagdmd = flagbroken = flaglas = false;
	int i = 0;
	for (vector<FlipSlab>::iterator iter = r.this_room_FlipSlabs.begin(); iter != r.this_room_FlipSlabs.end(); iter++) {
		if (!iter->isbroken) {
			flagbroken = checkFlipSlab_ball(ball, *iter, lx);
			if (flagbroken) return (i+1) * 10;
		}
		i++;
	}
	i = 0;
	for (vector<Diamond>::iterator iter = r.this_room_Diamonds.begin(); iter != r.this_room_Diamonds.end(); iter++) {
		if (!iter->isbroken) {
			flagdmd = checkDiamond(ball, *iter, lx);
			if (flagdmd) {
				return -10*(i+1);
			} 
		}
		i++;
	}
	for (vector<LaserWall>::iterator iter = r.this_room_laserwall.begin(); iter != r.this_room_laserwall.end(); iter++) {
		if (iter->depth<=lx) {
			flaglas = checkLaserWall(ball, *iter, lx);
			if (flagdmd) {
				ball.isbroken = true;
				return 0;
			}
		}
	}
	flagswit = checkSwitch(ball, r.this_room_switch, lx);
	if (flagswit) return 1;
	checkDoor_ball(ball, r.this_room_door, lx);
	checkRounding(ball, r.left, r.right, r.ceil, r.ground);
	return 0;
}

bool checkFlipSlab_ball(Ball& ball, FlipSlab& fs, GLfloat lx)
{
	if (fs.center.x > lx)return false;
	if (fs.isbroken) return false;
	if (ball.center.x + ball.var_x - fs.center.x <= ball.radius) {
		if (fs.type == 1) {
			if (ball.center.y + ball.var_y - fs.center.y - fs.var_y <= fs.width / 2 && ball.center.y + ball.var_y - fs.center.y - fs.var_y >= -fs.width / 2) {
				if (ball.center.z + ball.var_z - fs.center.z <= fs.height / 2 && ball.center.z + ball.var_z - fs.center.z >= -fs.height / 2) {
					fs.isbroken = true;
					ball.x_acc = -0.8*ball.x_acc;
					fs.HitLoc.x = ball.center.x + ball.var_x;
					fs.HitLoc.y = ball.center.y + ball.var_y;
					fs.HitLoc.z = ball.center.z + ball.var_z;
					return true;			
				}	
			}
		}
		else {
			if (ball.center.z + ball.var_z - fs.center.z + fs.var_z <= fs.height / 2 && ball.center.z + ball.var_z - fs.center.z + fs.var_z >= -fs.height / 2) {
				if (ball.center.y + ball.var_y - fs.center.y <= fs.width / 2 && ball.center.y + ball.var_y - fs.center.y >= -fs.width / 2) {
					fs.isbroken = true;
					ball.x_acc = -0.8*ball.x_acc;
					fs.HitLoc.x = ball.center.x + ball.var_x;
					fs.HitLoc.y = ball.center.y + ball.var_y;
					fs.HitLoc.z = ball.center.z + ball.var_z;
					return true;
				}
			}
		}
	}
	return false;
}

bool checkFlipSlab_camera(FlipSlab& fs, GLfloat lx, GLfloat ly, GLfloat lz)
{
	if (fs.center.x > lx)return false;
	if (fs.isbroken) return false;
	if (lx - fs.center.x <= THRESHOLD) {
		if ( fs.center.y - fs.var_y <= fs.width / 2 && fs.center.y - fs.var_y >= -fs.width / 2) {
			if (lz - fs.center.z - fs.var_z <= fs.height / 2 && lz - fs.center.z - fs.var_z >= -fs.height / 2) {
				fs.isbroken = true;
				return true;
			}
		}
	}
	return false;
}

bool checkRounding(Ball& ball, GLfloat left, GLfloat right, GLfloat ceil, GLfloat ground)
{
	if (ball.center.y + ball.var_y <= left + 2 * ball.radius) {
		ball.y_acc = -0.8*ball.y_acc;
		return true;
	}
	if (ball.center.y + ball.var_y >= right - 2 * ball.radius) {
		ball.y_acc = -0.8*ball.y_acc;
		return true;
	}
	if (ball.center.z + ball.var_z >= ceil - 2 * ball.radius) {
		ball.speed_z = -0.8*ball.speed_z;
		return true;
	}
	if (ball.center.z + ball.var_z <= ground + 2 * ball.radius) {
		ball.speed_z = -0.8*ball.speed_z;
		return true;
	}
	return false;
}

bool checkDiamond(Ball ball, Diamond& d, GLfloat lx)
{
	if (d.center.x > lx)return false;
	if (d.isbroken) return false;
	if (ball.center.x + ball.var_x - d.center.x <= 2*ball.radius) {
		if (ball.center.y + ball.var_y - d.center.y <= 2*ball.radius && ball.center.y + ball.var_y - d.center.y >= -2*ball.radius) {
			if (ball.center.z + ball.var_z - d.center.z - d.var_z <= 4*ball.radius && ball.center.z + ball.var_z -d.center.z - d.var_z >= -4*ball.radius) {
				ball.speed_y = 0.5*ball.speed_y;
				ball.isbroken = true;
				d.isbroken = true;
				return true;
			}
		}
	}
	return false;
}

bool checkSwitch(Ball& ball, openDoorSwitch& s, GLfloat lx)
{
	if (s.isopening == GL_TRUE) return false;
	if (ball.center.x + ball.var_x - s.center.x <= 3*ball.radius) {
		if (ball.center.y + ball.var_y - s.center.y <= 3*ball.radius && ball.center.y + ball.var_y - s.center.y >= -3*ball.radius) {
			if (ball.center.z + ball.var_z - s.center.z <= 3*ball.radius && ball.center.z + ball.var_z - s.center.z >= -3*ball.radius) {
				s.isopening = true;
				ball.x_acc = -0.95*ball.x_acc;
				return true;
			}
		}
	}
	return false;
}

bool checkDoor_camera(Door& d, GLfloat lx)/////////////////////////////////////////////////////
{
	//if(mode)
	if (lx <= THRESHOLD && lx > 0.1 &&d.isopening == false ) {
		d.isopening = true;
		return true;
	}
	return false;
}

bool checkDoor_ball(Ball& ball, Door& d, GLfloat lx)
{
	//for (vector<FlipSlab>::iterator iter = d.mydoor.begin(); iter != d.mydoor.end(); iter++) {
	//	checkFlipSlab
	//}
	if (ball.center.x + ball.var_x <= ball.radius && !d.isopening) {
		ball.x_acc = -0.85*ball.x_acc;
		return true;
	}
	return false;
}

bool checkLaserWall(Ball& ball, LaserWall& lw, GLfloat lx)
{
	if (lw.depth > lx)return false;
	if (ball.center.x + ball.var_x - lw.depth <= ball.radius) {
		for (int i = 0; i < 6; i++) {
			if (lw.lines[i].life > 0 && ball.center.z + ball.var_z - lw.lines[i].center.z <= ball.radius) {
				ball.isbroken = true;
				lw.lines[i].life--;
				return true;
			}
		}
	}
	return false;
}

bool checkLaserWall_camera(LaserWall& lw, GLfloat lx, GLfloat lz)
{
	if (lw.depth > lx)return false;
	if (lx - lw.depth <= THRESHOLD) {
		for (int i = 0; i < 6; i++) {
			if (lw.lines[i].life > 0 && lz - lw.lines[i].center.z <= THRESHOLD) {
				return true;
			}
		}
	}
	return false;
}