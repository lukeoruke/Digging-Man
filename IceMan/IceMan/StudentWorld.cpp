#include "StudentWorld.h"
#include "GameController.h"
#include <string>
using namespace std;
#include <memory>
#include <algorithm>

const int TUNNEL_COL_START = 30;
const int TUNNEL_COL_END = 33;
const int TUNNEL_ROW = 4;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}



//pg 19
int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	//updateDisplayText();

	//player->overlap(std::move(iceContainer), player);

	player->doSomething();
	if (player->getIsAlive()) {

		if (player->getOilAmnt() < lvlOil()) {   //checks if player got the oil amnt
			//then do something for all actors in a for loop

			for (size_t i = 0; i < actors.size(); i++)  //go thorough the actor vector and trigger its do something
			{
				if (actors.at(i) != nullptr)
					actors.at(i)->doSomething();

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
		removeDead();
	}
	
	else {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	
	//decLives();
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
	std::vector<unique_ptr<Actor>>::iterator it = actors.begin();
	for (; it != actors.end(); it++)
	{
		delete (*it).release();
	}
	actors.clear();
}

StudentWorld::~StudentWorld() {
	cleanUp();
}
//not used yet
StudentWorld* StudentWorld::getStudentWorld() {
	return this;
}


///////////////////ICE/////////////////
void StudentWorld::createIce() {
	for (int row = 0; row < MAX_WINDOW; row++) {
		for (int col = 0; col < MAX_WINDOW - 4; col++) {
			if (row < TUNNEL_COL_START || row > TUNNEL_COL_END || col < TUNNEL_ROW) {
				iceContainer[row][col] = std::unique_ptr<Ice>(new Ice(row, col));
			}
		}
	}

}

int StudentWorld::lvlGold() {
	return max(5 - static_cast<int>(getLevel()) / 2, 2);
}

int StudentWorld::lvlOil() {
	return min(2 + static_cast<int>(getLevel()), 21);
}

void StudentWorld::overlap(const Actor& a) {

	int playerX = a.getX();
	int playerY = a.getY();

	for (int x = playerX; x < playerX + 4; x++) {
		for (int y = playerY; y < playerY + 4; y++) {
			if (iceContainer[x][y] != nullptr) {
				deleteIce(x, y);
				if (a.getID() == IID_PLAYER)
					GameController::getInstance().playSound(SOUND_DIG);
			}
		}
	}
}
bool StudentWorld::iceInFront(const Actor& a) {
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
bool StudentWorld::boulderInFront(const Actor& a)
{
	bool ans = false;
	int xActor = a.getX();
	int yActor = a.getY();
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.begin() + lvlBoulder(); ++it)
	{	
		int xBoulder = (*it)->getX();
		int yBoulder = (*it)->getY();
		switch (a.getDirection()) {
		case GraphObject::Direction::left:
			//if (iceContainer[x - 1][y] || iceContainer[x - 1][y + 3])
			if ( ( xActor == xBoulder + 4 ) && ((yActor >= yBoulder - 3) && yActor <= yBoulder + 3) )
				ans = true;
			break;
		case GraphObject::Direction::right:
			//if (iceContainer[x + 4][y] || iceContainer[x + 4][y + 3])
			if ((xActor == xBoulder - 4) && ((yActor >= yBoulder - 3) && yActor <= yBoulder + 3))
				ans = true;
			break;
		case GraphObject::Direction::down:
			//if (iceContainer[x][y - 1] || iceContainer[x + 3][y - 1])
			if ((yActor == yBoulder + 4) && ((xActor >= xBoulder - 3) && xActor <= xBoulder + 3))
				ans = true;
			break;
		case GraphObject::Direction::up:
			//if (iceContainer[x][y + 4] || iceContainer[x + 3][y + 4])
			if ((yActor == yBoulder - 4) && ((xActor >= xBoulder - 3) && xActor <= xBoulder + 3))
				ans = true;
			break;
		default:
			return false;
		}
	}
	return ans;
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
	bool ans = false;
	int x = a.getX();
	int y = a.getY();
	switch (a.getDirection()) {
	case GraphObject::Direction::left:
		if (!overlapAt(x - 4, y))
			ans = true;
		break;
	case GraphObject::Direction::right:
		if (!overlapAt(x + 4, y))
			ans = true;
		break;
	case GraphObject::Direction::down:
		if (!overlapAt(x, y - 4))
			ans = true;
		break;
	case GraphObject::Direction::up:
		if (!overlapAt(x, y + 4))
			ans = true;
		break;
	default:
		return false;
	}
	return ans;
}



void StudentWorld::deleteIce(int x, int y) {
	iceContainer[x][y].reset();
	iceContainer[x][y] = nullptr;
}
///////////////////ICEMAN/////////////////
void StudentWorld::createPlayer() {
	player = std::unique_ptr<Iceman>(new Iceman(this));
}

///////////////////SQUIRT/////////////////
void StudentWorld::createSquirt(Iceman &man) {
	int x = man.getX();
	int y = man.getY();
	switch (man.getDirection()) {
	case GraphObject::Direction::left:
		x = x - 4;
		break;
	case GraphObject::Direction::right:
		x = x + 4;
		break;
	case GraphObject::Direction::down:
		y = y - 4;
		break;
	case GraphObject::Direction::up:
		y = y + 4;
		break;
	}
	actors.emplace_back(unique_ptr<Squirt>(new Squirt(this, x, y, man.getDirection())));
}

//Iceman* StudentWorld::getPlayer() const {
//	return player;
//}

///////////////////BOUDLER/////////////////
void StudentWorld::createBoulder(int create) {
	for (int k = 0; k < create; k++) {
		int x = 0;
		int y = 0;
		do {
			do {
				x = generateRandX();
			} while (x > 26 && x < 34);

			do {
				y = generateRandY();
			} while (y < 20);
		

		} while (!distance(x, y));

		unique_ptr<Boulder> boulder;
		boulder = unique_ptr<Boulder>(new Boulder(this, x, y));
		overlap(*boulder);
		actors.emplace_back(std::move(boulder));
	}

}

int StudentWorld::lvlBoulder() {
	return min(static_cast<int>(getLevel()) / 2 + 2, 9);
}

int StudentWorld::generateRandX() {
	return (rand() % 60);
	
}

int StudentWorld::generateRandY() {
	return (rand() % 56);
}

bool StudentWorld::distance(int x, int y) {
	if (actors.size() == 0) {   //temp
		return true;
	}
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.end(); it++) {
		double d = (sqrt(pow(x - (*it)->getX(), 2) + pow(y - (*it)->getY(), 2)));
		if (d <= 6) {
			return false;
		}
	}
	return true;
}

void StudentWorld::removeDead() {
	if (actors.size() == 0)
		return;
	
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.end(); it++)
	{
		if ((*it) != nullptr) {

			if (!(*it)->getIsAlive())
			{
				(*it).reset();

			}
		}
	}
	return;
}

void StudentWorld::setDisplayText() {
	unsigned int level = getLevel();
	unsigned int lives = getLives();
	int health = (*player).getHP();
	int squirts = (*player).getWaterAmnt();
	int gold = (*player).getGoldAmnt();
	int barrelsLeft = lvlOil() - (*player).getOilAmnt();
	int sonar = (*player).getSonarAmnt();
	int score = getScore();
	// Next, create a string from your statistics, of the form:  // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000 

	string s = formatStats(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
	// Finally, update the display text at the top of the screen with your newly created stats 
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatStats(unsigned int level, unsigned int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
{
	//TODO:: format
	return string();
}
