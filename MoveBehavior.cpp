#include "main.hpp"

MoveBehavior::MoveBehavior(Entity *self): self(self) {}

MoveBehavior::~MoveBehavior() {}

bool MoveBehavior::doMoveEntity(int to_x, int to_y) {
    if (!game.map->canWalk(to_x, to_y)) {return false;}
    
    self->setX(to_x);
    self->setY(to_y);
    return true;
}


