#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
const int ICEMAN_START_X = 30;
const int ICEMAN_START_Y = 60;



								///GRAPH OBJ
								///    |
							//////________
							/////|      |
							////ICE		Actor	
							////		 |
							////_____________________
							////|		|            |
						  ////ICEMAN   PEOPLE	    EXC

//////////////////////// ACTOR //////////////////  pg 24
Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, int depth)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	m_world = world;
	setVisible(true);
}
//void Actor :: doSomething() {}

StudentWorld* Actor::getWorld() const{
	return  m_world;
}

bool Actor::setisAlive(bool status) {
	return m_isAlive = status;
}
bool Actor::getisAlive() {
	return m_isAlive;
}
Actor::~Actor(){
}
//////////////////////// ICE //////////////////
Ice::Ice(int row, int col) :GraphObject(IID_ICE,row,col,right,0.25,3) {
	setVisible(true);
}
Ice::~Ice() {}
//void Ice::doSomething(){};

//////////////////////// ICEMAN //////////////////     pg 27
Iceman::Iceman(StudentWorld* world)
	:Actor(world,IID_PLAYER, ICEMAN_START_X, ICEMAN_START_Y, right, 1.0, 0)
{
	m_HP = 10;
	m_water_amnt = 5;
	m_sonar_amnt = 1;
	m_gold_amnt = 0;
}
Iceman::~Iceman(){}

//first attempt to delete Ice
//bool Iceman::overlap(std::unique_ptr<Ice> iceField[64][60], Iceman* player) {
//	double playerSize = player->getSize();
//	double iceSize = iceField[0][0]->getSize(); 
//
//	if (playerSize > iceSize) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}

void Iceman::doSomething() {
	//check if the iceman is alive
	if (m_HP <= 0) {
		setisAlive(false);
		return;
	} 
	getWorld()->overlap();
	//pg30
	int ch;
	if (getWorld()->getKey(ch) == true) {
		switch(ch)
		{
			case KEY_PRESS_LEFT:
				setDirection(left);
				if (getX() > 0)
					moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_RIGHT:
				setDirection(right);
				if (getX() < MAX_WINDOW - 4)
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_DOWN:
				setDirection(down);
				if (getY() > 0)
					moveTo(getX(), getY() - 1);
				break;
			case KEY_PRESS_UP:
				setDirection(up);
				if (getY() < MAX_WINDOW - 4)
					moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_SPACE:
				break;
			case KEY_PRESS_TAB:
				break;
				
		}
	}
	
	  

};
