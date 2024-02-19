#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>

#include <list>
class Actor;
class Avatar;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    Actor* getActor(int r, int c);

    ~StudentWorld();
    
private:
    int loadALevel(std::string currLevel);
    bool everyoneDoSomething();
    std::list<Actor*> actorList;
    Avatar* player;
};

#endif // STUDENTWORLD_H_
