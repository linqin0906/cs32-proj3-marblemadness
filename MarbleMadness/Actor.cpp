#include "Actor.h"
#include "StudentWorld.h"

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
                moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                moveTo(getX(), getY()+1);
                break;
            case KEY_PRESS_DOWN:
                moveTo(getX(), getY()-1);
                break;

        }
    }
}


