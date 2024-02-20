#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

#include "Level.h"
#include "Actor.h"
#include <algorithm>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    numCrystals = 0;

    int levelLoad = loadALevel("level0" + to_string(getLevel()) + ".txt");
    
    if (levelLoad == -1 || getLevel() == 100) return GWSTATUS_PLAYER_WON; //no file or finished lvl99
    if (levelLoad == -2) return GWSTATUS_LEVEL_ERROR;
    
    levelBonus = 1000; //FIXME: think about moving this to constructor
    isLevelComplete = false;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    setDisplayText();
    
    bool result = everyoneDoSomething();
    if (!result) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    } else {player->doSomething();}
    
    cleanDeadActors();
    if (levelBonus != 0) levelBonus--;
    
    if (isLevelComplete) {
        increaseScore(levelBonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (player != nullptr) delete player; //player isn't already deleted
    player = nullptr;
    
    list<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        Actor* temp = *it;
        it = actorList.erase(it);
        delete temp;
    }
    
}

//checks for NONPLAYER actors
Actor* StudentWorld::getActor(int r, int c) {
    list<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->getX() == r && (*it)->getY() == c)
            return *it;
        it++;
    }
    return nullptr;
}

bool StudentWorld::isPlayerOn(int r, int c) {
    if (player == nullptr) return false;
    if (player->getX() == r && player->getY() == c) return true;
    return false;
}

int StudentWorld::getCrystals() {
    return numCrystals;
}

void StudentWorld::decCrystals() {
    numCrystals--;
}

void StudentWorld::setLevelComplete(bool complete) {
    isLevelComplete = complete;
}
//TODO: not done
void StudentWorld::setDisplayText() {
    
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(7) << getScore();
    oss << "  Level: " << setw(2) << getLevel();
    oss.fill(' ');
    oss << "  Lives: " << setw(2) << getLives();
    oss << "  Bonus: " << setw(4) << levelBonus;
    
    string s = oss.str();
    setGameStatText(s);
}

int StudentWorld::loadALevel(string currLevel) {
   
    Level lev(assetPath()); 
    Level::LoadResult result = lev.loadLevel(currLevel);
    
    if (result == Level::load_fail_file_not_found) return -1;
    if (result == Level::load_fail_bad_format) return -2;
    
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            Level::MazeEntry item = lev.getContentsOf(r, c);
            
            if (item == Level::player)
                player = new Avatar(r, c, this);
            else if (item == Level::wall)
                actorList.push_back(new Wall(r, c, this));
            else if (item == Level::crystal) {
                actorList.push_back(new Crystal(r, c, this));
                numCrystals++;
            } else if (item == Level::exit)
                actorList.push_back(new Exit(r, c, this));
        }
    }
    
    return 0;
}

//everyone does something EXCEPT player
bool StudentWorld::everyoneDoSomething() {
    list<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();
            if (!player->isAlive()) return false;
        }
        it++;
    }
    return true;
}

void StudentWorld::cleanDeadActors() {
    list<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        if (!(*it)->isAlive()) {
            Actor* temp = *it;
            it = actorList.erase(it);
            delete temp;
        }
        else {it++;}
    }
}

StudentWorld::~StudentWorld() {
    if (player != nullptr) delete player; //player isn't already deleted
    player = nullptr;
    
    list<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        Actor* temp = *it;
        it = actorList.erase(it);
        delete temp;
    }
}
