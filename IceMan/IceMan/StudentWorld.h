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
	//void createBoulder();
	//void createProtestor();
	//void createHardcoreProtestor();
	//std::unique_ptr<Iceman> getPlayer() const; 
	void overlap();
	bool iceInFront();
	StudentWorld* getStudentWorld();
	void deleteIce(int x, int y);

	virtual int init(){
		//std::vector<Actor> Actors;   //pg 16
		createIce();
		createPlayer();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp();
	~StudentWorld();

private:
	Ice* ice;
	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW];
	/*Iceman* player;*/
	std::unique_ptr<Iceman> player;
	StudentWorld* world;
};

#endif // STUDENTWORLD_H_
