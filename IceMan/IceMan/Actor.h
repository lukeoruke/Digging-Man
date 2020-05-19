#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, int depth);
	StudentWorld* getWorld() const;
	void setIsAlive(bool status);
	bool getIsAlive();
	~Actor();
	virtual void doSomething() = 0;
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
	//void overlap(std::unique_ptr<Ice> iceField[60][60], Iceman* player);
private:
	int m_HP;
	int m_water_amnt;
	int m_sonar_amnt;
	int m_gold_amnt;
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
