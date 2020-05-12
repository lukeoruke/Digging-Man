#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, int depth)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	setVisible(true);
}
Actor::~Actor(){
}

//////////////////////// ICE //////////////////
Ice::Ice(int row, int col) :Actor(IID_ICE,row,col,right,0.25,3) {
}
Ice::~Ice() {}
void Ice::doSomething(){};
