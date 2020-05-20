#include "StudentWorld.h"
#include "GameController.h"
#include <string>
using namespace std;
#include <memory>

const int TUNNEL_COL_START = 30;
const int TUNNEL_COL_END = 33;
const int TUNNEL_ROW = 4;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}




int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	//player->overlap(std::move(iceContainer), player);

	//pg 19
	player->doSomething();

	//if (iceman is alive) {
	//	if (he did not win) {
	//		then do something for all actors in a for loop
	//	}
	//}

	decLives();
	//TEMP///
	return GWSTATUS_CONTINUE_GAME;
	//return GWSTATUS_PLAYER_DIED;
}


// check if we have to use the respecitve Destructors aka  ~
void StudentWorld::cleanUp() {
	player.reset();
	for (int row = 0; row < MAX_WINDOW; row++) {
		for (int col = 0; col < MAX_WINDOW - 4; col++) {
			if (row < TUNNEL_COL_START || row > TUNNEL_COL_END || col < TUNNEL_ROW) {
				iceContainer[row][col].reset();
			}
		}
	}

}

StudentWorld::~StudentWorld() {
	cleanUp();
}
///////////////////ICE/////////////////
void StudentWorld::createIce() {
	for (int row = 0; row < MAX_WINDOW; row++) {
		for (int col = 0; col < MAX_WINDOW-4; col++) {
			if (row < TUNNEL_COL_START || row > TUNNEL_COL_END || col < TUNNEL_ROW) {
				iceContainer[row][col] = std::unique_ptr<Ice>(new Ice(row, col));
			}
		}
	}

}
void StudentWorld::overlap() {

	int playerX = player->getX();
	int playerY = player->getY();

	for (int x = playerX; x < playerX + 4; x++) {
		for (int y = playerY; y < playerY + 4; y++) {
			if (iceContainer[x][y] != nullptr) {
				iceContainer[x][y].reset();
				iceContainer[x][y] = nullptr;
				GameController::getInstance().playSound(SOUND_DIG);
			}
		}
	}
}
bool StudentWorld::iceInFront() {
	switch (player->getDirection()) {
	case GraphObject::Direction::left:
		if (iceContainer[player->getX() - 1][player->getY()])
			return true;
		break;
	case GraphObject::Direction::right:
		if (iceContainer[player->getX() + 4][player->getY()])
			return true;
		break;
	case GraphObject::Direction::down:
		if (iceContainer[player->getX()][player->getY() - 1])
			return true;
		break;
	case GraphObject::Direction::up:
		if (iceContainer[player->getX()][player->getY() + 4])
			return true;
		break;
	default:
		return false;
	}
}

void StudentWorld::deleteIce(int x, int y) {
	iceContainer[x][y].reset();
}
///////////////////ICEMAN/////////////////
void StudentWorld::createPlayer() {
	player = std::unique_ptr<Iceman>(new Iceman(this));
}
//Iceman* StudentWorld::getPlayer() const {
//	return player;
//}

//not used yet
StudentWorld* StudentWorld::getStudentWorld() {
	return this;
}


