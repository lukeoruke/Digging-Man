#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>

const int MAX_WINDOW = 64;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Ice;
class Iceman;

class StudentWorld : public GameWorld    //inherited from public gameworld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}
	//function declrations
	void createIce();
	void createPlayer();
	Iceman* getPlayer() const;
	//Ice* getIce(int x, int y) const;
	void overlap();
	StudentWorld* getStudentWorld();
	void deleteIce(int x, int y);

	virtual int init(){
		//std::vector<Actor> Actors;   //pg 16
		createIce();
		createPlayer();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp()
	{
	}

private:
	Ice* ice;
	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW-4];
	Iceman* player;
	StudentWorld* world;
};

#endif // STUDENTWORLD_H_
