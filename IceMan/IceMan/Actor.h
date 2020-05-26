#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, int depth);
	~Actor();
	virtual void doSomething() = 0;
	//setters
	bool setisAlive(bool status);
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
//////////////////////////////////////////PERSON///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class Person : public Actor {
public:
	bool hitByBoulder();
	//TODO: implement virtual functions for abstract class
	//setters

	//getters
	int getHP() const;

private:
	int m_HP;
};

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICEMAN//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Iceman : public Actor
{
public:
	Iceman(StudentWorld* world);
	~Iceman();
	virtual void doSomething();
	//setters
	void gainGoldIceman();

	//getters
	int getHP() const;
	int getWaterAmnt() const;
	int getSonarAmnt() const;
	int getGoldAmnt() const;
	int getOilAmnt() const;

private:
	int m_HP;
	int m_water_amnt;
	int m_sonar_amnt;
	int m_gold_amnt;
	int m_oil_amnt;
};

class Squirt : public Actor
{
public:
	Squirt(StudentWorld* world, int row, int col, Direction direction);
	~Squirt();
	virtual void doSomething();

private:
	int m_travel_distance;
};

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////BOULDER//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int x, int y);
	~Boulder();
	virtual void doSomething();
	enum state { stable, waiting, falling };
	void setStateBoulder(state x);
	state getStateBoulder() const;

private:
	state m_state;
	int m_BoulderTick;
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////GOLD//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class Gold : public Actor
{
public:
	Gold(StudentWorld* world, int x, int y, int z);
	~Gold();
	virtual void doSomething();
	enum goldState {permanent, temporary};
	void setStateGold(goldState x);
	goldState getStateGold() const;
private:
	goldState gold_state;
	int m_GoldTick;
};


///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PROTESTORS//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class NormalProtestor : public Person
{
public:

private:

};

class HardcoreProtestor : public Person
{
public:

private:

};
//------ PROTESTORS ------

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
