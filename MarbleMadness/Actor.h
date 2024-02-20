#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething() = 0;
        virtual bool canScore() = 0;
        virtual bool isCollectable() = 0;
        bool isAlive();
        void setAlive(bool alive);
        StudentWorld* getWorld();
    private:
        bool liveStatus;
        StudentWorld* world;
};

class Wall: public Actor {
    public:
        Wall(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canScore() {return false;}        
        virtual bool isCollectable() {return false;}
};

class Collectable: public Actor {
    public:
    Collectable(int imageID, double startX, double startY, StudentWorld* sWorld);
    virtual void doSomething() = 0;
    virtual bool canScore() {return false;}
    virtual bool isCollectable() {return true;}
    void beCollected(int points, int soundID);
};

class Crystal: public Collectable {
    public:
    Crystal(double startX, double startY, StudentWorld* sWorld);
    virtual void doSomething();
};

class Exit: public Collectable {
    public:
    Exit(double startX, double startY, StudentWorld* sWorld);
    virtual void doSomething();
};

class Avatar: public Actor {
    public:
        Avatar(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canScore() {return true;}
        virtual bool isCollectable() {return false;}
    private:
        bool canMove(int dir);
};

#endif // ACTOR_H_
