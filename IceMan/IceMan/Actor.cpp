#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
const int ICEMAN_START_X = 30;
const int ICEMAN_START_Y = 60;



//  GRAPH OBJ
//      |
//   ________
//   |      |
//  ICE	   ACTOR	
//           |
// ______________________________________________________
//         |	                 |                      |
// BOULDER,SQUIRT(maybe)       PERSON                  GOODIES
//                               |
//                           ___________
//                          |           |
//                       ICEMAN      Normal Protestor
//                                           |
//                                       HCPROTESTOR

//////////////////////// ACTOR //////////////////  pg 24
Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, int depth)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	m_isAlive = true;
	m_world = world;
	setVisible(true);
}
Actor::~Actor() {}
//void Actor :: doSomething() {}

StudentWorld* Actor::getWorld() const {
	return  m_world;
}

bool Actor::setisAlive(bool status) {
	return m_isAlive = status;
}
bool Actor::getIsAlive() {
	return m_isAlive;
}


//////////////////////// ICE //////////////////
Ice::Ice(int row, int col) :GraphObject(IID_ICE, row, col, right, 0.25, 3) {
	setVisible(true);
}
Ice::~Ice() {};
//void Ice::doSomething(){};

//////////////////////// ICEMAN //////////////////     pg 27
Iceman::Iceman(StudentWorld* world)
	:Actor(world, IID_PLAYER, ICEMAN_START_X, ICEMAN_START_Y, right, 1.0, 0)
{
	m_HP = 10;
	m_water_amnt = 5;
	m_sonar_amnt = 1;
	m_gold_amnt = 0;
	m_oil_amnt = 0;
}
Iceman::~Iceman() {}

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
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			setisAlive(false);
			break;
		case KEY_PRESS_SPACE:
			if (m_water_amnt > 0) {
				GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
				m_water_amnt--;
				if (!(getWorld()->iceInFront(*this))) {  //if there is ice in front, don't fire the water
					if (getWorld()->isRoomInFront(*this))
					{
						setVisible(false); //for testing purposes only
										   //TODO: Create Squirt Object (using players location and direction), then give it to StudentWorld to manage
					}
				}

			}
			break;
		case KEY_PRESS_LEFT:  //x-1
			if (getDirection() != left)
				setDirection(left);
			else if (getX() > 0)
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT: //x+1
			if (getDirection() != right)
				setDirection(right);
			else if (getX() < MAX_WINDOW - 4)
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_DOWN:  //y-1
			if (getDirection() != down)
				setDirection(down);
			else if (getY() > 0)
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_UP: //y+1
			if (getDirection() != up)
				setDirection(up);
			else if (getY() < MAX_WINDOW - 4)
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_TAB:
			if (m_gold_amnt > 0) {

				//TODO: Create GoldNugget Object
				m_gold_amnt--;
			}
			break;

		}
	}



}

int Iceman::getOil() const {
	return m_oil_amnt;
}

//////////////////////// BOULDER //////////////////   
Boulder::Boulder(StudentWorld* world, int x, int y)
	:Actor(world, IID_BOULDER, x, y, down, 1.0, 1) {
}
Boulder::~Boulder(){}

void Boulder::doSomething() {

}
