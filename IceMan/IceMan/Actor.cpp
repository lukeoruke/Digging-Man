#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
const int ICEMAN_START_X = 30;
const int ICEMAN_START_Y = 60;

//////////////////////// ACTOR //////////////////
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, int depth)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	setVisible(true);
}
Actor::~Actor(){
}

//////////////////////// ICE //////////////////
Ice::Ice(int row, int col) :Actor(IID_ICE,row,col,right,0.25,3) {}
Ice::~Ice() {}
void Ice::doSomething(){};

//////////////////////// ICEMAN //////////////////     pg 27
Iceman::Iceman():Actor(IID_PLAYER,ICEMAN_START_X,ICEMAN_START_Y,right,1.0,0){}
Iceman::~Iceman(){}
void Iceman::doSomething() {


};