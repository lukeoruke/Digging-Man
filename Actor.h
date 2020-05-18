#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor : public GraphObject
{
	public:
		Actor(int imageID, int startX, int startY, Direction dir, double size, int depth);
		~Actor();
};
//////////////////////// ICE //////////////////
class Ice : public Actor   //grpah object on pg 24
{
public:
	Ice(int x, int y);
	~Ice();
	void doSomething();
};
//////////////////////// ICE MAN //////////////////
class Iceman : public Actor
{
public:
	Iceman(StudentWorld* world);
	~Iceman();
	void doSomething();
	StudentWorld* getWorld();
	//void overlap(std::unique_ptr<Ice> iceField[60][60], Iceman* player);
private:
	int m_HP;
	int m_water_amnt;
	int m_sonar_amnt;
	int m_gold_amnt;
	StudentWorld* m_world;
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
