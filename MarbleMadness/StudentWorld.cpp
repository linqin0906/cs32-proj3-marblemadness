#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

#include "Level.h"
#include "Actor.h"
#include <algorithm>

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
    int levelLoad = loadALevel("level0" + to_string(getLevel()) + ".txt");
    
    if (levelLoad == -1 || getLevel() == 100) return GWSTATUS_PLAYER_WON; //no file or finished lvl99
    if (levelLoad == -2) return GWSTATUS_LEVEL_ERROR;
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
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
                player = new Avatar(r, c);
            else if (item == Level::wall)
                actorList.push_back(new Wall(r, c));
        }
    }
    
    return 0;
}

StudentWorld::~StudentWorld() {
    delete player;
    
    list<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        Actor* temp = *it;
        it = actorList.erase(it);
        delete temp;
    }
}
