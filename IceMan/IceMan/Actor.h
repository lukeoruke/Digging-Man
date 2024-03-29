#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <algorithm>
#include <vector>

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, int depth);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool annoy(unsigned int amt) = 0;
	//setters
	bool setDead();
	//getters
	StudentWorld* getWorld() const;
	bool getIsAlive();
	//methods
	void moveInDirection();
private:
	StudentWorld* m_world;
	bool m_isAlive;
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICE//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Ice : public GraphObject   //grpah object on pg 24
{
public:
	Ice(int x, int y);
	~Ice();
};

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////BOULDER//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int x, int y);
	~Boulder();
	virtual void doSomething();
	virtual bool annoy(unsigned int amt);
	enum state { stable, waiting, falling };
	void setStateBoulder(state x);
	state getStateBoulder() const;

private:
	state m_state;
	int m_BoulderTick;
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////GOODIES//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Goodies : public Actor {
public:
	Goodies(StudentWorld* world, int startX, int startY, int imageID, int soundToPlay, bool activateOnPlayer,
		bool activateOnProtester);
	virtual ~Goodies();
	virtual bool annoy(unsigned int amt);
	enum goodieState { permanent, temporary };

	//getters
	int getItemSound() const;
	bool getIsActiveOnPlayer() const;
	bool getIsActiveOnProtester() const;
	goodieState getItemState() const;
	int getitemTick() const;

	//setters
	void setItemState(bool state);
	void setitemTicks(int ticks);
	void decreaseTick();

private:
	goodieState goodie_state;
	int goodie_sound;
	int m_ticks;
	bool is_active_on_player;
	bool is_active_on_protest;
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////GOLD/////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Gold : public Goodies
{
public:
	Gold(StudentWorld* world, int startX, int startY, bool temporary, bool activateOnPlayer, bool activateOnProtester);
	~Gold();
	virtual void doSomething();
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////OIL//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////   pg34
class Oil : public Goodies {
public:
	Oil(StudentWorld* world, int startX, int startY);
	~Oil();
	virtual void doSomething();
};
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////SONAR///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// pg 21
class Sonar : public Goodies {
public:
	Sonar(StudentWorld* world, int x, int y, bool state, int tick);
	~Sonar();
	virtual void doSomething();
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////WATER////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Water : public Goodies {
public:
	Water(StudentWorld* world, int x, int y, bool state, int tick);
	~Water();
	virtual void doSomething();
};

class Squirt : public Actor
{
public:
	Squirt(StudentWorld* world, int row, int col, Direction direction);
	~Squirt();
	virtual void doSomething();
	virtual bool annoy(unsigned int amt);

private:
	int m_travel_distance;
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PERSON///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class Person : public Actor {
public:
	Person(StudentWorld* world, int imageID, int startX, int startY, Direction dir, unsigned int health);
	virtual ~Person();
	//TODO: implement virtual functions for abstract class
	//setters

	virtual void gainGold() = 0;
	virtual bool annoy(unsigned int amt);

	//getters
	int getHP() const;

protected:
	unsigned int m_HP;

private:

};

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICEMAN///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Iceman : public Person
{
public:
	Iceman(StudentWorld* world);
	~Iceman();
	virtual void doSomething();
	//setters
	virtual void gainGold();
	void gainOilIceman();
	void gainSonarIceman();
	void gainWaterIceman();

	//getters
	int getWaterAmnt() const;
	int getSonarAmnt() const;
	int getGoldAmnt() const;
	int getOilAmnt() const;

private:
	int m_water_amnt;
	int m_sonar_amnt;
	int m_gold_amnt;
	int m_oil_amnt;
};


/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////PROTESTERS///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
class Protester : public Person {
public:
	Protester(StudentWorld* world, int x, int y, int imageID, unsigned int health);
	virtual ~Protester() = 0;
	//virtual bool annoy(unsigned int amt);

	//setters

	//getters
	bool getIsLeaving();
	//bool oppositeDirection();
	void moveProtester();
	void pickRandDirection(int protesterX, int protesterY);
	//bool cannotMove();
protected:

	bool m_leaveState; //starts out in a Not leave the oil state (false)
	int m_ticksWait;
	int rest_state;
	int m_distancetoTravel;
	int m_shout;
	int  m_perpendicular_tick;
	//std::queue<gridQueue> tree;
	std::vector<GraphObject::Direction> stepsToLeave;

};

class RegularProtester : public Protester
{
public:
	RegularProtester(StudentWorld* world, int x, int y);
	virtual bool annoy(unsigned int amt);
	void doSomething();
	void gainGold();
	int numSquaresToMoveInCurrentDirection();

private:

};

class HardcoreProtester : public Protester
{
public:
	HardcoreProtester(StudentWorld* world, int x, int y);
	virtual bool annoy(unsigned int amt);
	void gainGold();
	void doSomething();
	int numSquaresToMoveInCurrentDirection();
protected:


private:

};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
