#include "StudentWorld.h"
#include <string>
using namespace std;
#include <memory>

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

void StudentWorld::createIce() {
	for (int x = 0; x < COL; x++) {
		for (int y = 0; y < ROW; y++) {
			iceContainer[x][y] = std::unique_ptr<Ice>(new Ice(x, y));
		}
	}

}