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
//////////////////////// ICE //////////////////
class Ice : public GraphObject   //grpah object on pg 24
{
public:
	Ice(int x, int y);
	~Ice();
};
//////////////////////// ICE MAN //////////////////
class Iceman : public Actor
{
public:
	Iceman(StudentWorld* world);
	~Iceman();
	virtual void doSomething();
	//setters

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

////////////////////// BOULDER //////////////////
class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int x, int y);
	~Boulder();
	virtual void doSomething();

private:

};

//+++++++ PROTESTORS +++++++
class NormalProtestor : public Actor
{
public:

private:

};

class HardcoreProtestor : public NormalProtestor
{
public:

private:

};
//------ PROTESTORS ------

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
