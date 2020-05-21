#include "StudentWorld.h"
#include "GameController.h"
#include <string>
#include <memory>
#include <algorithm>
using namespace std;

const int TUNNEL_COL_START = 30;
const int TUNNEL_COL_END = 33;
const int TUNNEL_ROW = 4;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

void StudentWorld::cleanUp() {
	player.reset();


}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


//pg 19
int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	//updateDisplayText();

	//player->overlap(std::move(iceContainer), player);

	player->doSomething();
	if (player->getIsAlive() == true) {
		
		if (player->getOil() < lvlOil()) {
			//then do something for all actors in a for loop

			for (size_t i = 0; i < actors.size(); i++)
			{


			}
			if (player->getIsAlive() == false)
			{
				//GameController::getInstance().playSound(SOUND_PLAYER_ANNOYED); //this might be done by the actor that annoying the player
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
				
		}
		else {
			GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}
	else {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
		
	decLives();
	//TEMP///
	return GWSTATUS_CONTINUE_GAME;
	//return GWSTATUS_PLAYER_DIED;
}


void StudentWorld::createIce() {
	for (int row = 0; row < MAX_WINDOW; row++) {
		for (int col = 0; col < MAX_WINDOW - 4; col++) {
			if (row < TUNNEL_COL_START || row > TUNNEL_COL_END || col < TUNNEL_ROW) {
				iceContainer[row][col] = std::unique_ptr<Ice>(new Ice(row, col));
			}
		}
	}

}

void StudentWorld::createPlayer() {
	player = make_unique<Iceman>(this);
}

//Ice* StudentWorld::getIce(int x, int y) {
//	return iceContainer[x][y];
//}

StudentWorld* StudentWorld::getStudentWorld() //not used
{
	return this;
}

int StudentWorld::lvlBoulder() {
	return min(static_cast<int>(getLevel()) / 2 + 2, 9);
}

int StudentWorld::lvlGold() {
	return max(5 - static_cast<int>(getLevel()) / 2, 2);
}

int StudentWorld::lvlOil() {
	return min(2 + static_cast<int>(getLevel()), 21);
}

void StudentWorld::overlap() {
	int playerX = player->getX();
	int playerY = player->getY();

	for (int x = playerX; x < playerX + 4 ; x++) {
		for (int y = playerY; y < playerY + 4; y++) {
			if (iceContainer[x][y] != nullptr) {
				deleteIce(x, y);
				GameController::getInstance().playSound(SOUND_DIG);
			}
		}
	}
}

bool StudentWorld::overlapAt(int x, int y) {


	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (iceContainer[i][j]) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isRoomInFront(const Actor& a) {
	int x = a.getX();
	int y = a.getY();
	switch (a.getDirection()) {
	case GraphObject::Direction::left:
		if (!overlapAt(x - 4, y))
			return true;
		break;
	case GraphObject::Direction::right:
		if (!overlapAt(x + 4, y))
			return true;
		break;
	case GraphObject::Direction::down:
		if (!overlapAt(x, y - 4))
			return true;
		break;
	case GraphObject::Direction::up:
		if (!overlapAt(x, y + 4))
			return true;
		break;
	default: 
		return false;
	}
	return false;
}


//Iceman* StudentWorld::getPlayer() const {
//	return player;
//}

 


bool StudentWorld::iceInFront(const Actor &a) {
	int x = a.getX();
	int y = a.getY();
	switch (a.getDirection()) {
	case GraphObject::Direction::left:
		if (iceContainer[x - 1][y] || iceContainer[x - 1][y + 3])
			return true;
		break;
	case GraphObject::Direction::right:
		if (iceContainer[x + 4][y] || iceContainer[x + 4][y + 3])
			return true;
		break;
	case GraphObject::Direction::down:
		if (iceContainer[x][y - 1] || iceContainer[x + 3][y - 1])
			return true;
		break;
	case GraphObject::Direction::up:
		if (iceContainer[x][y + 4] || iceContainer[x + 3][y + 4])
			return true;
		break;
	default:
		return false;
	}
	return false;
}
void StudentWorld::deleteIce(int x, int y) {
	iceContainer[x][y].reset();
	iceContainer[x][y] = nullptr;
}
