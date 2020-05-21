#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>

const int MAX_WINDOW = 64;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Ice;
class Iceman;
class Boulder;

class StudentWorld : public GameWorld    //inherited from public gameworld
{
public:
	//constructor
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		player = nullptr;
	}
	//destructor
	~StudentWorld();

	//function declrations
	void createIce();
	void createPlayer();
	void createBoulder(int create);
	//void createProtestor();
	//void createHardcoreProtestor();
	//std::unique_ptr<Iceman> getPlayer() const; 

	//methods
	int lvlBoulder(); //returns amount of boulders in current level
	int lvlGold(); //returns amount of gold in current level
	int lvlOil(); //returns amount of oil in current level
	void overlap(const Actor& a);
	bool overlapAt(int x, int y);
	bool isRoomInFront(const Actor& a); //returns true if there is room for an object in front of player
	bool iceInFront(const Actor& a);
	void deleteIce(int x, int y);
	int generateRandX();
	int generateRandY();


	StudentWorld* getStudentWorld();


	virtual int init() {
		createIce();
		createPlayer();
		createBoulder(lvlBoulder());
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp();


private:
	Ice* ice; //purpose?
	StudentWorld* world; //purpose?
						 //Iceman* player; //purpose?

	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW];
	std::unique_ptr<Iceman> player;
	std::vector<Actor*> actors;
	Boulder* boulder;

};

#endif // STUDENTWORLD_H_
