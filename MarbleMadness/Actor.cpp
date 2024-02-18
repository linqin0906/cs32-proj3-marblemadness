#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//*********** ACTOR ***********//
Actor::Actor(int imageID, double startX, double startY): GraphObject(imageID, startX, startY) {
    setDirection(none);
    liveStatus = false;
}

bool Actor::isAlive() {
    return liveStatus;
}

void Actor::setAlive(bool alive) {
    liveStatus = alive;
}

//*********** WALL ***********//
Wall::Wall (double startX, double startY): Actor(IID_WALL, startX, startY) {
    setVisible(true);
    setAlive(true);
}

void Wall::doSomething() {}

//*********** AVATAR ***********//
Avatar::Avatar (double startX, double startY) : Actor(IID_PLAYER, startX, startY) {
    setVisible(true);
    setAlive(true);
    setDirection(right);
}
void Avatar::doSomething() { //FIXME: incomplete
    if (!isAlive()) return;
}


