#include "Actor.h"
#include "StudentWorld.h"

#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

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

//*********** AVATAR ***********//
Avatar::Avatar (double startX, double startY, StudentWorld* sWorld) : Actor(IID_PLAYER, startX, startY, sWorld) {
    setDirection(right);
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
        }
    }
}

//FIXME: only checks to see if there's something else
bool Avatar::canMove(int dir) {
    double newX = getX();
    double newY = getY();
    
    switch(dir) {
        case left:
            newX--;
            break;
        case right:
            newX++;
            break;
        case up:
            newY++;
            break;
        case down:
            newY--;
            break;
    }
    Actor* act = getWorld()->getActor(newX, newY);
    if (act != nullptr && !act->isCollectable()) return false;
    return true;
}


