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
// _______________________________________________________________
//         |	                 |                               |
// BOULDER,SQUIRT(maybe)       PERSON                         GOODIES
//                               |                               |
//                           ___________                ____________________
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

void Actor::moveInDirection() {

	Direction d = this->getDirection();
	switch (d) {

	case GraphObject::Direction::left:
		moveTo(getX() - 1, getY());
		break;
	case GraphObject::Direction::right:
		moveTo(getX() + 1, getY());
		break;
	case GraphObject::Direction::down:
		moveTo(getX(), getY() - 1);
		break;
	case GraphObject::Direction::up:
		moveTo(getX(), getY() + 1);
		break;
	}
	return;
}

//////////////////////// ICE //////////////////
Ice::Ice(int row, int col) :GraphObject(IID_ICE, row, col, right, 0.25, 3) {
	setVisible(true);
}
Ice::~Ice() {};
//void Ice::doSomething(){};

//////////////////////// Squirt ///////////////
Squirt::Squirt(StudentWorld* world, int row, int col, GraphObject::Direction direction)
	: Actor(world, IID_WATER_SPURT, row, col, direction, 1.0, 0)
{
	m_travel_distance = 8;
}

void Squirt::doSomething() {

	if (m_travel_distance > 0 && !getWorld()->iceInFront(*this) && !getWorld()->boulderInFront(*this)) {
		//TODO: should not be able to pass through boulders
		moveInDirection();
		m_travel_distance--;
	}
	else {
		setisAlive(false);
	}
	return;
}

Squirt::~Squirt() {
	setisAlive(false);

}

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

	getWorld()->overlap(*this);

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
				if (!(getWorld()->iceInFront(*this)) && !(getWorld()->boulderInFront(*this))) {  //if there is ice in front, don't fire the water. TODO: same logic but for boulder
					if (getWorld()->isRoomInFront(*this))
					{
						getWorld()->createSquirt(*this);
					}
				}

			}
			break;
		case KEY_PRESS_LEFT:  //x-1
			if (getDirection() != left)
				setDirection(left);
			else if (getX() > 0 && !(getWorld()->boulderInFront(*this)))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT: //x+1
			if (getDirection() != right)
				setDirection(right);
			else if (getX() < MAX_WINDOW - 4 && !(getWorld()->boulderInFront(*this)))
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_DOWN:  //y-1
			if (getDirection() != down)
				setDirection(down);
			else if (getY() > 0 && !(getWorld()->boulderInFront(*this)))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_UP: //y+1
			if (getDirection() != up)
				setDirection(up);
			else if (getY() < MAX_WINDOW - 4 && !(getWorld()->boulderInFront(*this)))
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

int Iceman::getHP() const
{
	return m_HP;
}

int Iceman::getWaterAmnt() const
{
	return m_water_amnt;
}

int Iceman::getSonarAmnt() const
{
	return m_sonar_amnt;
}

int Iceman::getGoldAmnt() const
{
	return m_gold_amnt;
}

int Iceman::getOilAmnt() const {
	return m_oil_amnt;
}

//////////////////////// BOULDER //////////////////   pg31
Boulder::Boulder(StudentWorld* world, int x, int y)
	:Actor(world, IID_BOULDER, x, y, down, 1.0, 1) {
	m_state = stable;
	m_tick = 29;
}
Boulder::~Boulder() {}

void Boulder::setState(state x) {
	m_state = x;
}
Boulder:: state Boulder::getState() const {
	return m_state;
}
void Boulder::decrementTick() {
	m_tick--;
}
int Boulder::getTick() {
	return m_tick;
}
void Boulder::doSomething() {
	if (!this->getIsAlive()) { //if they are not alive
		return;
	}
	
	switch(this->getState()) {
		case Boulder::state::stable:
			//if there exists ice underneath the boulder, stay stable
			//if not change the state to waiting
			if (!getWorld()->iceInFront(*this)) { //if there is no ice underneath
				m_state = waiting;
			}
			//check if there is any ince in the 4 squares below it    pg 32
			break;
		case Boulder::state::waiting:
			if (m_tick == 0) {  // once 30 ticks have passed, change the state of the boudler to falling
				m_state = falling;
				GameController::getInstance().playSound(SOUND_FALLING_ROCK);

			}
			else {
				decrementTick();
			}
			break;
		case Boulder::state::falling:
				this->moveInDirection();
				//check after one tick whether there is ice, boulder, or out of border
				if (getWorld()->iceInFront(*this) || getWorld()->boulderInFront(*this)
					||getY() == 0)
				{ 
					this->setisAlive(false);
				}

			break;
	}


}
