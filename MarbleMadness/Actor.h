#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
    public:
        Actor(int imageID, double startX, double startY);
        virtual void doSomething() = 0;
        bool isAlive();
        void setAlive(bool alive);
    private:
        bool liveStatus;
};

class Wall: public Actor {
    public:
        Wall(double startX, double startY);
        virtual void doSomething();
};

class Avatar: public Actor {
    public:
        Avatar(double startX, double startY);
        virtual void doSomething();
};

#endif // ACTOR_H_
