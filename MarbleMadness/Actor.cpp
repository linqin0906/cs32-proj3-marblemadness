#include "Actor.h"
#include "StudentWorld.h"

#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//*********** ACTOR ***********//
Actor::Actor(int imageID, double startX, double startY, StudentWorld* sWorld): GraphObject(imageID, startX, startY) {
    setDirection(none);
    liveStatus = false;
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
Wall::Wall (double startX, double startY, StudentWorld* sWorld): Actor(IID_WALL, startX, startY, sWorld) {
    setVisible(true);
    setAlive(true);
}

void Wall::doSomething() {}

//*********** AVATAR ***********//
Avatar::Avatar (double startX, double startY, StudentWorld* sWorld) : Actor(IID_PLAYER, startX, startY, sWorld) {
    setVisible(true);
    setAlive(true);
    setDirection(right);
}
void Avatar::doSomething() { //FIXME: incomplete
    if (!isAlive()) return;
    int ch;
    if(getWorld()->getKey(ch)) {
        switch(ch) {
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
    if (getWorld()->getActor(newX, newY) != nullptr) return false;
    return true;
}


