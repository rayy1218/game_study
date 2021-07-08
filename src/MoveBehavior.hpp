#ifndef MOVEBEHAVIOR_HPP
#define MOVEBEHAVIOR_HPP

class MoveBehavior {
    Entity *self;
public:
    MoveBehavior(Entity *self);
    virtual ~MoveBehavior();
    virtual bool doMoveEntity(int to_x, int to_y);
};

#endif /* MOVEBEHAVIOR_HPP */

