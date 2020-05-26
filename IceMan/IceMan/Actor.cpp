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
//                    _______________________           ____________________
//                    |          |          |           |        |         |
//               ICEMAN      Norm Prot.  HC Prot.      


///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ACTOR//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////  pg 24
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

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICE//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
Ice::Ice(int row, int col) :GraphObject(IID_ICE, row, col, right, 0.25, 3) {
	setVisible(true);
}
Ice::~Ice() {};
//void Ice::doSomething(){};

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////SQUIRT//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICEMAN//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////     pg 27
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
				if (!(getWorld()->iceInFront(*this)) && !(getWorld()->boulderInTheWay(*this))) {  //if there is ice in front, don't fire the water. same logic but for boulder
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
			else if (getX() > 0 && !(getWorld()->boulderInTheWay(*this)))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT: //x+1
			if (getDirection() != right)
				setDirection(right);
			else if (getX() < MAX_WINDOW - 4 && !(getWorld()->boulderInTheWay(*this)))
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_DOWN:  //y-1
			if (getDirection() != down)
				setDirection(down);
			else if (getY() > 0 && !(getWorld()->boulderInTheWay(*this)))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_UP: //y+1
			if (getDirection() != up)
				setDirection(up);
			else if (getY() < MAX_WINDOW - 4 && !(getWorld()->boulderInTheWay(*this)))
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_TAB:
			if (m_gold_amnt > 0) {
				m_gold_amnt--;
				// create gold object
			}
			break;

		}
	}



}
void Iceman::gainGoldIceman() {
	m_gold_amnt++;
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

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////BOULDER//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////   pg31
Boulder::Boulder(StudentWorld* world, int x, int y)
	:Actor(world, IID_BOULDER, x, y, down, 1.0, 1) {
	m_state = stable;
	m_BoulderTick = 29;
	getWorld()->incBouldersLeft();
}
Boulder::~Boulder() {
	getWorld()->decBouldersLeft();
}

void Boulder::setStateBoulder(state x) {
	m_state = x;
}
Boulder::state Boulder::getStateBoulder() const {
	return m_state;
}
//int Boulder::decrementTick() {
//	return m_boulder--;
//}
//int Boulder::getTick() {
//	return m_tick;
//}
void Boulder::doSomething() {
	if (!this->getIsAlive()) { //if they are not alive
		return;
	}

	switch (this->getStateBoulder()) {
	case Boulder::state::stable:
		//if there exists ice underneath the boulder, stay stable
		//if not change the state to waiting
		if (!getWorld()->iceInFront(*this)) { //if there is no ice underneath
			m_state = waiting;
		}
		//check if there is any ince in the 4 squares below it    pg 32
		break;
	case Boulder::state::waiting:
		if (m_BoulderTick == 0) {  // once 30 ticks have passed, change the state of the boudler to falling
			m_state = falling;
			GameController::getInstance().playSound(SOUND_FALLING_ROCK);

		}
		else {
			m_BoulderTick--;     //decrement the tick by one
		}
		break;
	case Boulder::state::falling:
		this->moveInDirection();
		//check after one tick whether there is ice, boulder, or out of border
		if (getWorld()->iceInFront(*this) || getWorld()->boulderInFront(*this)
			|| getY() == 0)
		{
			this->setisAlive(false);
		}

		break;
	}


}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////GOLD//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////   pg34

Gold::Gold(StudentWorld* world, int x, int y)
	: Actor(world, IID_GOLD, x, y, right, 1.0, 2) {
	setVisible(false);
	getWorld()->incGoldLeft();
	m_GoldTick = 20; // how long does the gold nugget last for once its created?
}

Gold::~Gold(){
	getWorld()->decGoldLeft();
}

void Gold::doSomething() {
	if (!this->getIsAlive()) { //if they are not alive
		return;
	}

	if (!this->isVisible()) {  //if the gold is currently not visible
		int goldX = this->getX();
		int goldY = this->getY();

		//if the ice man is in range of the gold, make it visible
		if (getWorld()->icemanNearby(*this, goldX, goldY, 4.0)) {
			this->setVisible(true);
			return;
		}
	}

	if (this->isVisible()) {  //if the gold is visible to the player  pg35, #3
		int goldX = this->getX();
		int goldY = this->getY();
		

		if (getWorld()->icemanNearby(*this, goldX, goldY, 3.0)) {
			this->setisAlive(false); // if the iceman is 3 units away, set the gold to dead
			GameController::getInstance().playSound(SOUND_GOT_GOODIE);
			getWorld()->incIcemanGold();
		}
	}

}

