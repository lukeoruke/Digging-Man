#include "StudentWorld.h"
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

//Ice* StudentWorld::getIce(int x, int y) const{
//	iceContainer[x][y].get();
//	return iceContainer[x][y].get;
//}
void StudentWorld::overlap() {
	int playerX = getPlayer()->getX();
	int playerY = getPlayer()->getY();

	for (int x = playerX; playerX < x + 4; playerX++) {
		for (int y = playerY; playerY < y + 4; playerY++) {
			if (iceContainer[x][y] != nullptr) {
				iceContainer[x][y].reset();
				iceContainer[x][y] = nullptr;
			}
		}
	}
}
void StudentWorld::deleteIce(int x, int y){
	 iceContainer[x][y].reset();
}
///////////////////ICEMAN/////////////////
void StudentWorld::createPlayer() {
	player = new Iceman(this);
}
Iceman* StudentWorld::getPlayer() const {
	return player;
}

//not used yet
StudentWorld* StudentWorld::getStudentWorld() {
	return this;
}


