#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

//////////////////////// ICE //////////////////
class Ice : public GraphObject   //grpah object on pg 24
{
public:
	Ice(int x, int y);
	~Ice();
};

//+++++++ ACTOR ++++++++
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, int depth);
	~Actor();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	void setIsAlive(bool status);
	bool getIsAlive() const;
private:
	StudentWorld* m_world;
	bool m_isAlive;
};

//++++++++ PERSON ++++++++

//////////////////////// ICE MAN //////////////
class Iceman : public Actor
{
public:
	Iceman(StudentWorld* world);
	//~Iceman();
	virtual void doSomething();
	int getOil() const;
	//void overlap(std::unique_ptr<Ice> iceField[60][60], Iceman* player);
private:
	int m_HP;
	int m_water_amnt;
	int m_sonar_amnt;
	int m_gold_amnt;
	int m_oil_amnt;
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
