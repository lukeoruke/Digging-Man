#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <queue>

const int MAX_WINDOW = 64;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Ice;
class Iceman;
class Boulder;
class Gold;
class Protestor;
struct gridQueue {
public:
	gridQueue(int x, int y);
	int getGridX();
	int getGridY();

private:
	int m_x;
	int m_y;
};
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
	void createOil(int num);
	void createGold(int num);
	void createSonar();
	void createWater();
	void createProtestor();
	//void createHardcoreProtestor();
	//std::unique_ptr<Iceman> getPlayer() const; 

	//methods
	int lvlBoulder(); //returns amount of boulders in current level
	int lvlGold(); //returns amount of gold in current level
	int lvlOil(); //returns amount of oil in current level

	void overlap(const Actor& a);
	bool overlapAt(int x, int y);

	bool annoyNearbyPeople(const Actor& a, unsigned int hp);
	bool isRoomInFront(const Actor& a); //returns true if there is room for an object in front of player
	bool iceInFront(const Actor& a);
	bool boulderInFront(const Actor& a);
	bool boulderInTheWay(const Actor& a);
	bool icemanNearby(const Actor& a, int x, int y, double radius);
	void placeGold(int x, int y);
	void createNewItem();
	void useSonar();
	//added
	bool icemanInSight(int x, int y);
	double protestorRadius(int x, int y);
	GraphObject::Direction getIcemanDirection();
	GraphObject::Direction faceIceman(int x, int y);
	bool canReachIceman(int x, int y);
	bool canTurn(int x, int y, GraphObject::Direction r);
	GraphObject::Direction makeTurn(int x, int y, GraphObject::Direction r);
	GraphObject::Direction leaveField(int x, int y);
	void findPath(int x, int y);
	void createGrid();
	GraphObject:: Direction pickPath(int  proX, int proY, int d);

	int generateRandX();
	int generateRandY();
	bool distance(int x, int y); //(for creating new actor objects) returns true if euclidean distance between calling actor and a coordinate pair is greater than 6
	double radius(int x1, int y1, int x2, int y2); //takes two sets of coordinates and returns the euclidean distance between them
	void deleteIce(int x, int y);
	void removeDead();

	void setDisplayText();
	std::string formatStats(unsigned int level, unsigned int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);
	std::string ZeroPadNumber(int num);
	std::string SpacePadNumber(int num, int pad);
	//getters
	StudentWorld* getStudentWorld();
	int getBouldersLeft() const;
	int getGoldLeft() const;
	int getOilLeft() const;
	int getSonarLeft() const;
	int getWaterLeft() const;
	int getSonarWaterTick();


	//setters
	void decBouldersLeft();
	void incBouldersLeft();
	void decGoldLeft();
	void incGoldLeft();
	void incIcemanItem(char item);
	void incOilLeft();
	void decOilLeft();
	void incSonarLeft();
	void decSonarLeft();
	void decWaterLeft();
	void incWaterLeft();


	//don't call these
	virtual int init() {
		createIce();
		createPlayer();
		createBoulder(lvlBoulder());
		createOil(lvlOil());
		createGold(lvlGold());
		createProtestor();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp();


private:
	//Ice* ice; //purpose?
	//StudentWorld* world; //purpose?
	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW];
	int grid[MAX_WINDOW][MAX_WINDOW];
	std::unique_ptr<Iceman> player;
	std::vector<std::unique_ptr<Actor>> actors;
	int m_bouldersLeft;
	int m_goldleft;
	int m_oilleft;
	int m_sonarleft;
	int m_waterleft;
	std::queue<gridQueue> tree;
	std::vector<GraphObject::Direction> stepsToLeave;
};


#endif // STUDENTWORLD_H_


