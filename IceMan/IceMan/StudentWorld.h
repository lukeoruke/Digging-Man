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
class Protester;
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
		m_goldleft = 0;
		m_oilleft = 0;
		m_sonarleft = 0;
		m_protestersleft = 0;
		m_waterleft = 0;
		m_ticksLastProtester = 0;
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
	void createProtester();
	void createHProtester();

	//methods
	int lvlBoulder(); //returns amount of boulders in current level
	int lvlGold(); //returns amount of gold in current level
	int lvlOil(); //returns amount of oil in current level
	int lvlProtestors(); //returns max amount of protestors in current level

	void overlap(const Actor& a);
	bool overlapAt(int x, int y);

	bool protesterFoundGold(const Actor& a);
	void annoyIceman(unsigned int hp);
	bool annoyNearbyPeople(const Actor& a, unsigned int hp);
	bool isRoomInFront(const Actor& a); //returns true if there is room for an object in front of player
	bool iceInFront(const Actor& a);
	bool boulderInFront(const Actor& a);
	bool boulderInTheWay(const Actor& a, int where);
	bool icemanNearby(const Actor& a, int x, int y, double radius);
	void placeGold(int x, int y);
	void createNewItem();
	void createNewProtester();
	void useSonar();
	//added
	bool icemanInSight(int x, int y);
	double protesterRadius(int x, int y);
	GraphObject::Direction getIcemanDirection();
	GraphObject::Direction faceIceman(int x, int y);
	bool isFacingIceman(const Actor& a);
	bool canReachIceman(int x, int y);
	bool canTurn(int x, int y, GraphObject::Direction r);
	GraphObject::Direction makeTurn(int x, int y, GraphObject::Direction r);
	std::vector<GraphObject::Direction>  leaveField(int x, int y);
	int findPath(int x, int y);
	void createGrid();
	GraphObject::Direction pickPath(int  proX, int proY, int d);

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
	int getProtestersLeft() const;
	int getSonarWaterTick();
	int getProtesterTick();


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
	void incProtestersLeft();
	void decProtestersLeft();


	//don't call these
	virtual int init() {
		createIce();
		createPlayer();
		createBoulder(lvlBoulder());
		createOil(lvlOil());
		createGold(lvlGold());
		//createProtester();
		//createHProtester();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move();

	virtual void cleanUp();


private:

	std::unique_ptr<Ice> iceContainer[MAX_WINDOW][MAX_WINDOW];
	int grid[MAX_WINDOW][MAX_WINDOW];
	std::unique_ptr<Iceman> player;
	std::vector<std::unique_ptr<Actor>> actors;
	int m_bouldersLeft;
	int m_goldleft;
	int m_oilleft;
	int m_sonarleft;
	int m_waterleft;
	int m_protestersleft;
	std::queue<gridQueue> tree;
	std::vector<GraphObject::Direction> stepsToLeave;
	int m_ticksLastProtester;
};


#endif // STUDENTWORLD_H_
