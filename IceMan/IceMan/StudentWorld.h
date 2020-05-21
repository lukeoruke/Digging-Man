#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>
#include <vector>


const int MAX_WINDOW = 64;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Ice;
class Actor;
class Iceman;



class StudentWorld : public GameWorld    //inherited from public gameworld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		player = nullptr;
	}
	~StudentWorld();

	void createIce();
	void createPlayer();
	void createBoulder();
	void createSquirt();

	void createNormalProtestor();
	void createHardcoreProtestor();

	//std::unique_ptr<Ice> getIce(int x, int y);
	//Iceman* getPlayer() const;
	StudentWorld* getStudentWorld();
	
	int lvlBoulder(); //returns amount of boulders in current level
	int lvlGold(); //returns amount of gold in current level
	int lvlOil(); //returns amount of oil in current level
	void overlap(); //deletes ice if player overlaps
	bool overlapAt(int x, int y); //returns true if there is an overlap (coordinates start at bottom left of wherever we want to check)
	bool isRoomInFront(const Actor& a); //returns true if there is room for an object in front of player
	bool iceInFront(const Actor& a);
	void deleteIce(int x, int y);

	virtual int init() {
		createIce();
		createPlayer();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp();

private:
	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW];
	std::unique_ptr<Iceman> player;
	std::vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
