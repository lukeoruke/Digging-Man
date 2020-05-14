#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject
{
	public:
		Actor(int imageID, int startX, int startY, Direction dir, double size, int depth);
		~Actor();
};

class Ice : public Actor   //grpah object on pg 24
{
public:
	Ice(int x, int y);
	~Ice();
	void doSomething();
};

class Iceman : public Actor
{
	Iceman();
	~Iceman();
	void doSomething();

};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
