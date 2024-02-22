#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// *********** ACTORS *********** //
class Actor: public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething() = 0;
        void getPosInDir(int dir, int& newX, int& newY);
        virtual bool canScore() {return false;}
        virtual bool isCollectable() {return false;}
        virtual bool isDamageable() {return false;}
        virtual bool canKillPeas() {return false;}
        virtual bool canReceive() {return false;}
        virtual bool canAttack() {return false;}
        virtual bool push(int r, int c) {return false;} //returns false if not pushable
        virtual void takeDamage() {} //does nothing if not Mortal
        bool isAlive();
        void setAlive(bool alive);
        StudentWorld* getWorld();
    private:
        bool liveStatus;
        StudentWorld* world;
};

class Pea: public Actor {
    public:
        Pea(int dir, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
    private:
        bool makeWarAndPeas();
};

class Pit: public Actor {
    public:
        Pit(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canReceive() {return true;}
};

class Wall: public Actor {
    public:
        Wall(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canKillPeas() {return true;}
};

class ThiefbotFactory: public Actor {
    public:
        ThiefbotFactory(std::string botType, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canKillPeas() {return true;}
};

// *********** MORTALS *********** //
class Mortal: public Actor {
    public:
        Mortal(int hp, int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething() = 0;
        virtual bool isDamageable() {return true;}
        virtual void takeDamage();
        int getHP();
        void setHP(int amt);
    private:
        int hitpoints;
};

// *********** MARBLE *********** //
class Marble: public Mortal {
    public:
        Marble(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool push(int r, int c);
};

// *********** FIGHTERS *********** //
class Fighter: public Mortal {
    public:
        Fighter(int hp, int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual bool canAttack() {return true;}
        void shoot(int soundID);
};

class Avatar: public Fighter {
    public:
        Avatar(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canScore() {return true;}
        bool makePush(Actor* a);
        int getPeaCount();
        double getHealthPercentage();
    private:
        bool canMove(int dir);
        int peaCount;
};

// *********** COLLECTABLES *********** //
class Collectable: public Actor {
    public:
        Collectable(int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething() = 0;
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


#endif // ACTOR_H_
