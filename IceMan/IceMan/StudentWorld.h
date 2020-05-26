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
class Gold;

class StudentWorld : public GameWorld    //inherited from public gameworld
{
public:
	//constructor
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		player = nullptr;
		m_bouldersLeft = 0;
	}
	//destructor
	~StudentWorld();

	//function declarations
	void createIce();
	void createPlayer();
	void createSquirt(Iceman& man);
	void createBoulder(int num);
	//void createOil(int num);
	void createGold(int num);
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
	bool boulderInFront(const Actor& a);
	//bool boulderInTheWay(const Actor& a);
	bool icemanNearby(const Actor& a, int x, int y, double radius);

	int generateRandX();
	int generateRandY();
	bool distance(int x, int y); //returns true is distance between actors is far enough
	double radius(int x1, int y1, int x2, int y2);
	void deleteIce(int x, int y);
	void removeDead();


	void setDisplayText();
	std::string formatStats(unsigned int level, unsigned int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);

	//getters
	StudentWorld* getStudentWorld();
	int getBouldersLeft() const;
	int getGoldLeft() const;

	//setters
	void decBouldersLeft();
	void incBouldersLeft();
	void decGoldLeft();
	void incGoldLeft();
	void incIcemanGold();

	//don't call these
	virtual int init() {
		createIce();
		createPlayer();
		createBoulder(lvlBoulder());
		//createOil(lvlOil);
		createGold(lvlGold());
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp();


private:
	//Ice* ice; //purpose?
	//StudentWorld* world; //purpose?
	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW];
	std::unique_ptr<Iceman> player;
	std::vector<std::unique_ptr<Actor>> actors;
	int m_bouldersLeft;
	int m_goldleft;

};

#endif // STUDENTWORLD_H_
