#include "StudentWorld.h"
#include "GameController.h"
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
using namespace std;

const int TUNNEL_COL_START = 30;
const int TUNNEL_COL_END = 33;
const int TUNNEL_ROW = 4;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::createNewItem() {  //21
	int g = static_cast<int>(getLevel() * 25 + 300);
	if (rand() % g == 0) {  //ranges 1 to g; this should mean a 1/g change of running
		if ((rand() % g + 1) <= (g / 5)) {  //this should be 1/5, create sonar
			createSonar();
		}
		else { //rest is 4/5, create water
			createWater();
		}
	}
	return;
}

int StudentWorld::getSonarWaterTick() {
	return max(100, (300 - 10 * static_cast<int>(getLevel())));
}

//pg 19
int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	setDisplayText();
	player->doSomething();

	if (player->getIsAlive()) {
		createNewItem();

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
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////GLOBAL FUNCTIONS(DISTANCE, RAND NUM GEN, REMOVE DEAD, EXC)//////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::removeDead() {
	if (actors.size() == 0)
		return;

	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	actors.erase(std::remove_if(it, actors.end(), [&](unique_ptr<Actor>& upAct)-> bool
	{return (upAct->getIsAlive() == false); }),
		actors.end());

	return;
}
//generates a random x value
int StudentWorld::generateRandX() {
	return (rand() % 60);
}
//generates a random y value
int StudentWorld::generateRandY() {
	return (rand() % 56);
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
//returns true is distance between actors is far enough
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

double StudentWorld::radius(int x1, int y1, int x2, int y2) {
	double d = (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
	return d;
}


bool StudentWorld::annoyNearbyPeople(const Actor& a, unsigned int hp)
{
	bool ans = false;
	if (radius(player->getX(), player->getY(), a.getX(), a.getY()) <= 3) {
		if (player->annoy(hp))
			ans = true;
	}
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.end(); it++) {
		if (radius(a.getX(), a.getY(), (*it)->getX(), (*it)->getY()) <= 3) {
			if ((*it)->annoy(hp))
				ans = true;
			//TODO: pg 43,48 make sure score gets increased accordingly
		}
	}
	return ans;
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
// x and y are the coord of whatever object is calling the fucntion
// r is the specific radius specifed by the object
bool StudentWorld::icemanNearby(const Actor& a, int x, int y, double r) {
	int playerX = player->getX();
	int playerY = player->getY();

	if (radius(playerX, playerY, x, y) <= r) {
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICE//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::createIce() {
	for (int row = 0; row < MAX_WINDOW; row++) {
		for (int col = 0; col < MAX_WINDOW - 4; col++) {
			if (row < TUNNEL_COL_START || row > TUNNEL_COL_END || col < TUNNEL_ROW) {
				iceContainer[row][col] = std::unique_ptr<Ice>(new Ice(row, col));
			}
		}
	}

}
//checks whether the pixel around it is a ice block
bool StudentWorld::iceInFront(const Actor& a) {
	int x = a.getX();
	int y = a.getY();
	switch (a.getDirection()) {
	case GraphObject::Direction::left:
		for (int i = y; i < y + 4; i++)
		{
			if (iceContainer[x - 1][i])
				return true;
		}
		break;
	case GraphObject::Direction::right:
		for (int i = y; i < y + 4; i++)
		{
			if (iceContainer[x + 4][i])
				return true;
		}
		break;
	case GraphObject::Direction::down:
		for (int i = x; i < x + 4; i++)
		{
			if (iceContainer[i][y - 1])
				return true;
		}
		break;
	case GraphObject::Direction::up:
		for (int i = x; i < x + 4; i++)
		{
			if (iceContainer[i][y + 4])
				return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

//deletes ice block of a specified cooridnate
void StudentWorld::deleteIce(int x, int y) {
	iceContainer[x][y].reset();
	iceContainer[x][y] = nullptr;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////ICEMAN///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::createPlayer() {
	player = std::unique_ptr<Iceman>(new Iceman(this));
}
void StudentWorld::incIcemanItem(char x) {
	switch (x) {
	case 'g':
		player->gainGold();
		break;
	case 'o':
		player->gainOilIceman();
		break;
	case 's':
		player->gainSonarIceman();
		break;
	case 'w':
		player->gainWaterIceman();
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PROTESTOR///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::createProtestor() {
	unique_ptr<RegularProtestor> protestor;
	protestor = unique_ptr<RegularProtestor>(new RegularProtestor(this, 60, 60));
	actors.emplace_back(std::move(protestor));
}

bool StudentWorld::icemanInSight(int x, int y) {
	if (player->getX() == x || player->getY() == y) {
		return true;
	}
	else {
		return false;
	}
}
double StudentWorld::protestorRadius(int x, int y) {
	double d = (sqrt(pow(player->getX() - x, 2) + pow(player->getY() - y, 2)));
	return d;
}
GraphObject::Direction StudentWorld::getIcemanDirection() {
	GraphObject::Direction x = player->getDirection();
	return x;
}
GraphObject::Direction StudentWorld::faceIceman(int x, int y) {
	if (player->getX() < x) {
		return GraphObject::left;
	}
	if (player->getX() > x) {
		return GraphObject::right;
	}
	if (player->getY() < y) {
		return GraphObject::down;
	}
	if (player->getY() > y) {
		return GraphObject::up;
	}
}
bool StudentWorld:: canReachIceman(int x, int y) { //TODO:: cannot figure out the boulder issue
	int px = player->getX();
	int py = player->getY();
	if (px == x) {  //check row or col first then separate but length
		if (py > y) { //if the player is higher then the protestor
			int startY = y;
			while (startY <= py) {
				if (iceContainer[x][startY])  // TODO:: account for boulders
				{
					return false;
				}
				startY++;
			}
			return true;
		}
		else {
			int startY = py;
			while (startY <= y) {
				if (iceContainer[x][startY]) { // TODO:: account for boulders
					return false;
				}
				startY++;
			}
			return true;
		}

	}
	//other half
	if (py == y){
		if (px > x) {
			int startX = x;
			while (startX <= px) {
				if (iceContainer[startX][y]) {
					return false;
				}
				startX++;
			}
			return true;
		}
		else {
			int startX = px;
			while (startX <= px) {
				if (iceContainer[startX][y]) {
					return false;
				}
				startX++;
			}
			return true;
		}
	}
}
bool StudentWorld::canTurn(int x, int y, GraphObject::Direction r) {
	int e = player->getX();
	int q = player->getY();
	switch (r) {
	case GraphObject::up:
	case GraphObject::down:
		//THE ICE IS 3 PIXELS WIDE BUT ICEMAN DETECTS THE FIRST PIXEL OF THE TUNNEL
		for (int r = x; r < x + 3; r++) {
			if (iceContainer[r + 1][y] || iceContainer[r - 1][y]) {
				return false;
			}
		}
		return true;
		break;
	case GraphObject::left:
	case GraphObject::right:
		for (int k = x; k < x+4; k++) {
			for (int p = y; p < y+4; p++) {
				if (iceContainer[k][p]) {
					return false;
				}
			}
		}
		return false;
		break;
	}
}
GraphObject::Direction StudentWorld::makeTurn(int x, int y, GraphObject::Direction r) {
	switch (r) {
	case GraphObject::up:
	case GraphObject::down:
		//if there is no ice on the right and bottom  TODO:: account for boulders
		if (!iceContainer[x + 1][y] && !iceContainer[x - 1][y]) {
			int choice = rand() % 2;
			if (choice == 0) {
				return GraphObject::right;
			}
			if (choice == 1) {
				return GraphObject::left;
			}
		}
		if (!iceContainer[x + 1][y]) {
			return GraphObject::right;
		}
		if (!iceContainer[x - 1][y]) {
			return GraphObject::left;
		}
		break;
	case GraphObject::left:
	case GraphObject::right:
		//TODO::account for boulders
		if (!iceContainer[x][y + 1] && !iceContainer[x][y - 1]) {
			int choice = rand() % 2;
			if (choice == 0) {
				return GraphObject::up;
			}
			if (choice == 1) {
				return GraphObject::down;
			}
		}
		if (!iceContainer[x][y+1]) {
			return GraphObject::up;
		}
		if (!iceContainer[x][y-1]) {
			return GraphObject::down;
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////SQUIRT///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::createSquirt(Iceman& man) {
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

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////BOULDER//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int StudentWorld::lvlBoulder() {
	return min(static_cast<int>(getLevel()) / 2 + 2, 9);
}

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
// parses through actor vector and finds boulders
bool StudentWorld::boulderInFront(const Actor& a)
{
	bool ans = false;
	int xActor = a.getX();
	int yActor = a.getY();
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.begin() + getBouldersLeft(); ++it)
	{

		int xBoulder = (*it)->getX();
		int yBoulder = (*it)->getY();
		switch (a.getDirection()) {
		case GraphObject::Direction::left:
			if ((xActor == xBoulder + 4) && ((yActor >= yBoulder - 3) && yActor <= yBoulder + 3))
				ans = true;
			break;
		case GraphObject::Direction::right:
			if ((xActor == xBoulder - 4) && ((yActor >= yBoulder - 3) && yActor <= yBoulder + 3))
				ans = true;
			break;
		case GraphObject::Direction::down:
			if ((yActor == yBoulder + 4) && ((xActor >= xBoulder - 3) && xActor <= xBoulder + 3))
				ans = true;
			break;
		case GraphObject::Direction::up:
			if ((yActor == yBoulder - 4) && ((xActor >= xBoulder - 3) && xActor <= xBoulder + 3))
				ans = true;
			break;
		default:
			return false;
		}
	}
	return ans;
}
int StudentWorld::getBouldersLeft() const
{
	return m_bouldersLeft;
}

void StudentWorld::decBouldersLeft()
{
	m_bouldersLeft--;
}

void StudentWorld::incBouldersLeft()
{
	m_bouldersLeft++;
}
//we used this function
bool StudentWorld::boulderInTheWay(const Actor& a)
{
	bool ans = false;
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.begin() + getBouldersLeft(); ++it)
	{
		int xActor = a.getX();
		int yActor = a.getY();
		int xBoulder = (*it)->getX();
		int yBoulder = (*it)->getY();
		switch (a.getDirection()) {
		case GraphObject::Direction::left:
			xActor = xActor - 1;
			break;
		case GraphObject::Direction::right:
			xActor = xActor + 1;
			break;
		case GraphObject::Direction::down:
			yActor = yActor - 1;
			break;
		case GraphObject::Direction::up:
			yActor = yActor + 1;
			break;
		default:
			return false;
		}
		if (radius(xActor, yActor, xBoulder, yBoulder) <= 3)
			ans = true;
	}

	return ans;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////GOLD/////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int StudentWorld::lvlGold() {
	return max(5 - static_cast<int>(getLevel()) / 2, 2);
}
void StudentWorld::createGold(int num)
{
	for (int k = 0; k < num; k++) {
		int x = 0;
		int y = 0;
		do {
			do {
				x = generateRandX();
			} while (x > 26 && x < 34);

			y = generateRandY();

		} while (!distance(x, y));
		unique_ptr<Gold> gold;
		gold = unique_ptr<Gold>(new Gold(this, x, y, false, true, false)); //false to make gold invisible when created
		actors.emplace_back(std::move(gold));
	}
}

int StudentWorld::getGoldLeft() const
{
	return m_goldleft;
}

void StudentWorld::decGoldLeft()
{
	m_goldleft--;
}

void StudentWorld::incGoldLeft() {
	m_goldleft++;
}
void StudentWorld::placeGold(int x, int y) {
	unique_ptr<Gold> gold;
	gold = unique_ptr<Gold>(new Gold(this, x, y, true, false, true)); //true to make gold visible
	actors.emplace_back(std::move(gold));
}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////OIL//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int StudentWorld::lvlOil() {
	return min(2 + static_cast<int>(getLevel()), 21);
}
void StudentWorld::createOil(int num)
{
	for (int k = 0; k < num; k++) {
		int x = 0;
		int y = 0;
		do {
			do {
				x = generateRandX();
			} while (x > 26 && x < 34);

			y = generateRandY();

		} while (!distance(x, y));
		unique_ptr<Oil> oil;
		oil = unique_ptr<Oil>(new Oil(this, x, y));
		actors.emplace_back(std::move(oil));
	}
}

int StudentWorld::getOilLeft() const
{
	return m_oilleft;
}

void StudentWorld::decOilLeft()
{
	m_oilleft--;
}

void StudentWorld::incOilLeft() {
	m_oilleft++;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////SONAR////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::createSonar()
{
	unique_ptr<Sonar> sonar;
	sonar = unique_ptr<Sonar>(new Sonar(this, 0, 60, true, getSonarWaterTick()));
	actors.emplace_back(std::move(sonar));

}
void StudentWorld::incSonarLeft() {
	m_sonarleft++;
}
void StudentWorld::decSonarLeft() {
	m_sonarleft--;
}
int StudentWorld::getSonarLeft() const {
	return m_sonarleft;
}
void StudentWorld::useSonar() {
	GameController::getInstance().playSound(SOUND_SONAR);
	int playerX = player->getX();
	int playerY = player->getY();
	std::vector<unique_ptr<Actor>> ::iterator it = actors.begin();
	for (; it != actors.end(); it++) {
		if ((radius(playerX, playerY, (*it)->getX(), (*it)->getY()) < 12)) {
			(*it)->setVisible(true);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////WATER POOL///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void StudentWorld::createWater()
{

	//Each new Water Goodie must be added to a random ice - less spot in the oil field.
	//Water may only be added to a location if the entire 4x4 grid at that location is free
	//of Ice.
	int x = 0;
	int y = 0;
	do {

		x = generateRandX();
		do {
			y = generateRandY();
		} while (y < 20);

	} while (overlapAt(x, y));

	unique_ptr<Water> water;
	water = unique_ptr<Water>(new Water(this, x, y, true, getSonarWaterTick()));
	actors.emplace_back(std::move(water));

}
void StudentWorld::decWaterLeft() {
	m_waterleft--;
}
void StudentWorld::incWaterLeft() {
	m_waterleft++;
}

int StudentWorld::getWaterLeft() const {
	return m_waterleft;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////DISPLAY//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//pg 22
string StudentWorld::formatStats(unsigned int level, unsigned int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
{
	string s_level = "Lvl: " + to_string(level) + "  ";
	string s_lives = "Lives: " + to_string(lives) + "  ";
	string s_health = "Hlth:  " + to_string(health) + "%  ";
	string s_squirt = "Wtr:  " + to_string(squirts) + "  ";
	string s_gold = "Gld:  " + to_string(gold) + "  ";
	string s_barrel = "Oil Left:  " + to_string(barrelsLeft) + "  ";
	string s_sonar = "Sonar:  " + to_string(sonar) + "  ";
	string s_score = "Scr:  " + to_string(score);         //TODO: MAKE THIS DISPLAY 6 DIGITS

	string display = s_level + s_lives + s_health + s_squirt + s_gold + s_barrel + s_sonar + s_score;
	return display;
}

void StudentWorld::setDisplayText() {
	unsigned int level = getLevel();
	unsigned int lives = getLives();
	int health = (*player).getHP() * 10;
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
