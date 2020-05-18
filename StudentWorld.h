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

class StudentWorld : public GameWorld    //inherited from public gameworld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}
	void createIce();
	void createPlayer(StudentWorld* world);
	virtual int init(){
		createIce();
		createPlayer(world);
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		

		//player->overlap(std::move(iceContainer), player);
		 player->doSomething();


		decLives();
		//TEMP///
		return GWSTATUS_CONTINUE_GAME;
		//return GWSTATUS_PLAYER_DIED;
	}

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
