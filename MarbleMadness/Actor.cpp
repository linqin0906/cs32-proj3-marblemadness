#include "Actor.h"
#include "StudentWorld.h"

#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
using namespace std;

//*********** ACTOR ***********//
//Actors are created visible, with no direction, and alive
Actor::Actor(int imageID, double startX, double startY, StudentWorld* sWorld): GraphObject(imageID, startX, startY) {
    setVisible(true);
    setDirection(none);
    liveStatus = true;
    world = sWorld;
}

StudentWorld* Actor::getWorld() {
    return world;
}

bool Actor::isAlive() {
    return liveStatus;
}

void Actor::setAlive(bool alive) {
    liveStatus = alive;
}

void Actor::getPosInDir(int dir, int& newX, int& newY) {
    int currX = getX();
    int currY = getY();
        
    switch(dir) {
        case left:
            currX--;
            break;
        case right:
            currX++;
            break;
        case up:
            currY++;
            break;
        case down:
            currY--;
            break;
    }
    newX = currX;
    newY = currY;
}

//*********** PIT ***********//
Pit::Pit(double startX, double startY, StudentWorld* sWorld) : Actor (IID_PIT, startX, startY, sWorld){}

void Pit::doSomething() {
    if (!isAlive()) return;
    Actor *act = getWorld()->getActor(getX(), getY(), this);
    if (act != nullptr && act->isDamageable() && !act->canAttack()) {//if marble on same spot
        setAlive(false);
        act->setAlive(false);
    }
}

//*********** PEA ***********//
Pea::Pea(int dir, double startX, double startY, StudentWorld* sWorld) : Actor (IID_PEA, startX, startY, sWorld){
    setDirection(dir);
}

void Pea::doSomething() {
    if (!isAlive()) return;

    if (makeWarAndPeas()) return; //pea made contact
    moveForward();
    if (makeWarAndPeas()) return;
}

bool Pea::makeWarAndPeas() { //max 3 nonplayer actors on a square: robot, factory, pea
    Actor *a = getWorld()->getActor(getX(), getY(), this); //robot or factory
    Actor *b = getWorld()->getActor(getX(), getY(), a, this); //if a is robot, b could be factory, and vice versa
    
    //1: a is robot, b is factory
    //2: b factory, a robot
    //3. a is null, b is factory/wall
    //4. b is null, a is factory/wall
    //5. a and b both null
    
    //if a is robot, b cannot be robot. check for robots first
    if (a != nullptr && a->isDamageable()) {
        a->takeDamage(SOUND_ROBOT_IMPACT, SOUND_ROBOT_DIE);
        setAlive(false);
        return true;
    } else if (b != nullptr && b->isDamageable()) {
        b->takeDamage(SOUND_ROBOT_IMPACT, SOUND_ROBOT_DIE);
        setAlive(false);
        return true;
    }
    //factory/wall check
    if (a != nullptr && a->canKillPeas()) {
        setAlive(false);
        return true;
    } else if (b != nullptr && b->canKillPeas()) {
        setAlive(false);
        return true;
    }
    
    if (getWorld()->isPlayerOn(getX(), getY())) { //same square as player
        getWorld()->getPlayer()->takeDamage(SOUND_PLAYER_IMPACT, SOUND_PLAYER_DIE);
        setAlive(false);
        return true;
    }
    
    return false;
}

//*********** WALL ***********//
Wall::Wall (double startX, double startY, StudentWorld* sWorld): Actor(IID_WALL, startX, startY, sWorld) {}

void Wall::doSomething() {}

//*********** COLLECTABLE ***********//
Collectable::Collectable(int imageID, double startX, double startY, StudentWorld* sWorld): Actor(imageID, startX, startY, sWorld) {}

void Collectable::beCollected(int points, int soundID) {
    getWorld()->increaseScore(points);
    setAlive(false);
    getWorld()->playSound(soundID);
}

//*********** CRYSTAL ***********//
Crystal::Crystal (double startX, double startY, StudentWorld* sWorld): Collectable(IID_CRYSTAL, startX, startY, sWorld) {}

void Crystal::doSomething() {
    if (!isAlive()) return;
    if (getWorld()->isPlayerOn(getX(), getY())) {
        beCollected(50, SOUND_GOT_GOODIE);
        getWorld()->decCrystals();
    }
}

//*********** EXIT ***********//
Exit::Exit(double startX, double startY, StudentWorld* sWorld): Collectable(IID_EXIT, startX, startY, sWorld) {
    setVisible(false);
}

void Exit::doSomething() {
    if (getWorld() -> getCrystals() == 0 && !isVisible()) {
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
        return;
    }
    if (getWorld()->isPlayerOn(getX(), getY()) && isVisible() && getWorld()->getCrystals()==0) {
        beCollected(2000, SOUND_FINISHED_LEVEL);
        getWorld()->setLevelComplete(true);
    }
}

//*********** MORTAL ***********//
Mortal::Mortal(int hp, int imageID, double startX, double startY, StudentWorld* sWorld): Actor(imageID, startX, startY, sWorld) {
    hitpoints = hp;
}

void Mortal::takeDamage(int soundImpact, int soundDeath) {
    hitpoints -= 2;
    if (hitpoints <= 0) {
        setAlive(false);
        if (soundDeath != -1) getWorld()->playSound(soundDeath);
        return;
    }
    if (soundImpact != -1) {
        getWorld()->playSound(soundImpact);
    }
}

int Mortal::getHP() {
    return hitpoints;
}

void Mortal::setHP(int amt) {
    hitpoints = amt;
}

//*********** MARBLE ***********//
Marble::Marble(double startX, double startY, StudentWorld* sWorld): Mortal(10, IID_MARBLE, startX, startY, sWorld) {}

void Marble::doSomething() {}

bool Marble::push(int r, int c) {
    Actor* act = getWorld()->getActor(r, c, this);
    if (act == nullptr || (act != nullptr && act->canReceive())) {
        moveTo(r, c);
        return true;
    }
    return false;
}

//*********** FIGHTER ***********//
Fighter::Fighter (int hp, int imageID, double startX, double startY, StudentWorld* sWorld): Mortal(hp, imageID, startX, startY, sWorld) {}

void Fighter::shoot(int soundID) {
    int newX;
    int newY;
    getPosInDir(getDirection(), newX, newY);
    getWorld()->spawnPea(getDirection(), newX, newY);
    getWorld()->playSound(soundID);
}

bool Fighter::canMove(int dir) {
    int newX;
    int newY;
    
    getPosInDir(dir, newX, newY);
    
    Actor* act = getWorld()->getActor(newX, newY, this);
    //if act is wall/factory, pit, or robot
    if (act != nullptr && (act->canKillPeas() || act->canReceive() || (act->canAttack() && !act->canScore()))) {
        return false;
    }
    
    return true;
}


//*********** AVATAR ***********//
Avatar::Avatar (double startX, double startY, StudentWorld* sWorld) : Fighter(20, IID_PLAYER, startX, startY, sWorld) { //FIXME: not sure about the fighter constructor
    setDirection(right);
    peaCount = 20;
}

//FIXME: incomplete
void Avatar::doSomething() {
    if (!isAlive()) return;
    int ch;
    if(getWorld()->getKey(ch)) {
        switch(ch) {
            case KEY_PRESS_ESCAPE:
                setAlive(false);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (canMove(left)) moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (canMove(right)) moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (canMove(up)) moveTo(getX(), getY()+1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (canMove(down)) moveTo(getX(), getY()-1);
                break;
            case KEY_PRESS_SPACE:
                if (peaCount > 0) {
                    shoot(SOUND_PLAYER_FIRE);
                    peaCount--;
                }
        }
    }
}

//FIXME: only checks to see if there's something else
bool Avatar::canMove(int dir) {
    if (!Fighter::canMove(dir)) return false;
    
    int newX;
    int newY;
    getPosInDir(dir, newX, newY);
    
    Actor* act = getWorld()->getActor(newX, newY, this);

    if (act != nullptr && act->isDamageable() && !act->canAttack()) { //if it's a marble
        if (makePush(act)) return true; //if marble can be pushed, allow avatar to move
        else {return false;}
    }
    
    return true;
}

bool Avatar::makePush(Actor *a) {
    int newX;
    int newY;
    a->getPosInDir(getDirection(), newX, newY);
    
    if (a->push(newX, newY)) {
        return true;
    } return false;
}

int Avatar::getPeaCount() {
    return peaCount;
}

double Avatar::getHealthPercentage() {
    double percent = getHP() * 1.0/20;
    setprecision(2);
    percent *= 100;
    return percent;
}

//*********** ROBOT ***********//
Robot::Robot(int hp, int imageID, double startX, double startY, StudentWorld* sWorld) : Fighter (hp, imageID, startX, startY, sWorld) {
    tickCount = 0;
}

bool Robot::canTakeAction() {

    if (tickCount % (getWorld()->computeTicks()) == 0) {
        tickCount = 0;
        return true;
    }
    return false;
}

void Robot::incTick() {
    tickCount++;
}

int Robot::getTickCount() {
    return tickCount;
}

bool Robot::canFire() {
    int dir = getDirection();
    switch(dir) {
        case up:
            for (int y = getY() + 1; y < 15; y++) {
                if (getWorld()->isPlayerOn(getX(), y)) return true; //player found w/o obstacles
                Actor* a = getWorld()->getActor(getX(), y, this);
                if (a != nullptr) {
                    if (a->canKillPeas() || (a->isDamageable() && !a->canAttack()) || (a->isDamageable() && !a->canScore())) return false;
                }
            } break;
        case right:
            for (int x = getX() + 1; x < 15; x++) {
                if (getWorld()->isPlayerOn(x, getY())) return true; //player found w/o obstacles
                Actor* a = getWorld()->getActor(x, getY(), this);
                if (a != nullptr) {
                    if (a->canKillPeas() || (a->isDamageable() && !a->canAttack()) || (a->isDamageable() && !a->canScore())) return false;
                }
            } break;
        case down:
            for (int y = getY() - 1; y >= 0; y--) {
                if (getWorld()->isPlayerOn(getX(), y)) return true; //player found w/o obstacles
                Actor* a = getWorld()->getActor(getX(), y, this);
                if (a != nullptr) {
                    if (a->canKillPeas() || (a->isDamageable() && !a->canAttack()) || (a->isDamageable() && !a->canScore())) return false;
                }
            } break;
        case left:
            for (int x = getX() - 1; x >= 0; x--) {
                if (getWorld()->isPlayerOn(x, getY())) return true; //player found w/o obstacles
                Actor* a = getWorld()->getActor(x, getY(), this);
                if (a != nullptr) {
                    if (a->canKillPeas() || (a->isDamageable() && !a->canAttack()) || (a->isDamageable() && !a->canScore())) return false;
                }
            } break;
    }
    return false; //should never get here
}

bool Robot::canMove(int dir) {
    if (!Fighter::canMove(dir)) return false;
    int newX, newY;
    getPosInDir(dir, newX, newY);
    if (getWorld()->isPlayerOn(newX, newY)) return false;
    Actor* a = getWorld()->getActor(newX, newY, this);
    if (a != nullptr && a->isDamageable() && !a->canAttack()) {
        return false; //is marble
    }
    return true;
}

//*********** RAGEBOT ***********//
RageBot::RageBot(int dir, double startX, double startY, StudentWorld* sWorld) : Robot(10, IID_RAGEBOT, startX, startY, sWorld){
    setDirection(dir);
}

void RageBot::doSomething() {
    if (!isAlive()) return;
    if (!canTakeAction()) {
        incTick();
        return;
    }
    incTick();
    if (canFire()) {
        shoot(SOUND_ENEMY_FIRE);
        return;
    }
    if (canMove(getDirection())) {
        int newX, newY;
        getPosInDir(getDirection(), newX, newY);
        moveTo(newX, newY);
    } else {
        int dir = getDirection();
        switch (dir) {
            case up:
                setDirection(down); break;
            case down:
                setDirection(up); break;
            case left:
                setDirection(right); break;
            case right:
                setDirection(left); break;
        }
    }
}

