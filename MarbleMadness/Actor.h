#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// *********** ACTORS *********** //
class Actor: public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual ~Actor() {}
        virtual void doSomething() = 0;
        void getPosInDir(int dir, int& newX, int& newY);
        virtual bool canScore() {return false;}
        virtual bool isCollectable() {return false;}
        virtual bool isDamageable() {return false;}
        virtual bool canKillPeas() {return false;}
        virtual bool canReceive() {return false;}
        virtual bool canAttack() {return false;}
        virtual bool isThievable() {return false;}
        virtual bool push(int r, int c) {return false;} //returns false if not pushable
        virtual void takeDamage(int soundImpact, int soundDeath) {} //does nothing if not Mortal
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
        virtual bool isDamageable() {return true;}
        virtual void takeDamage(int soundImpact, int soundDeath);
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
        virtual bool canMove(int dir);
};

// *********** AVATAR *********** //
class Avatar: public Fighter {
    public:
        Avatar(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual bool canScore() {return true;}
        bool makePush(Actor* a);
        int getPeaCount();
        double getHealthPercentage();
        virtual bool canMove(int dir);
    private:
        int peaCount;
};

// *********** ROBOT *********** //
class Robot: public Fighter {
    public:
        Robot(int hp, int imageID, double startX, double startY, StudentWorld* sWorld);
        bool canTakeAction();
        void incTick();
        int getTickCount();
        bool canFire();
        virtual bool canMove(int dir);
    private:
        int tickCount;
};

// *********** RAGEBOT *********** //
class RageBot: public Robot {
    public:
        RageBot(int dir, double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual void takeDamage(int soundImpact, int soundDeath);
};

// *********** THIEFBOT *********** //
class ThiefBot: public Robot {
    public:
        ThiefBot(double startX, double startY, StudentWorld* sWorld, int hp=5, int imageID=IID_THIEFBOT);
        void doThiefBot();
        virtual void doSomething();
        virtual void takeDamage(int soundImpact, int soundDeath);
    private:
        void setDistanceBeforeTurning();
        bool setRandDirectionAndMove();
        int distBeforeTurn;
        int squaresMoved;
        bool hasThieved;
        Actor* goodie;
};

// *********** MEANTHIEFBOT *********** //
class MeanThiefBot: public ThiefBot {
    public:
        MeanThiefBot(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
        virtual void takeDamage(int soundImpact, int soundDeath);
};

// *********** COLLECTABLES *********** //
class Collectable: public Actor {
    public:
        Collectable(int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual bool isCollectable() {return true;}
        void beCollected(int points, int soundID=SOUND_GOT_GOODIE);
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

class Goodie: public Collectable {
    public:
        Goodie(int imageID, double startX, double startY, StudentWorld* sWorld);
        virtual bool isThievable() {return true;}
};

class ExtraLife: public Goodie {
    public:
        ExtraLife(double startX, double startY, StudentWorld* sWorld);
        virtual void doSomething();
};



#endif // ACTOR_H_
