#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>

const int ROW = 30;
const int COL = 30;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Ice;

class StudentWorld : public GameWorld    //inherited from public gameworld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}
	void createIce();
	virtual int init(){
		createIce();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	virtual void cleanUp()
	{
	}

private:
	Ice* makeIce;
	//std::list<Ice*> iceContainer;
	std::unique_ptr<Ice> iceContainer[COL][ROW];
};

#endif // STUDENTWORLD_H_
