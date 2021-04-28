#ifndef CONTROL_HPP
#define CONTROL_HPP

class Pathfinding;

class Control {
protected:
    Entity *self;
public:
    Control(Entity *self);
    virtual ~Control();
    
    virtual void doUpdate();
    float getDistanceTo(float x, float y);
};

class PlayerControl : public Control {
public:
    PlayerControl(Entity *self);
    
    void doUpdate();
    bool handleMoveOrAttack(int dx, int dy);
    bool handleCharInput(int ascii);
};

class EnemyControl : public Control {
private:
    friend class Pathfinding;
    TCODPath *path_to_player;
    int move_count;
public:
    EnemyControl(Entity *self);
    ~EnemyControl();
    
    void doUpdate();
    void handleMoveOrAttack();
};

class CorpseControl : public Control {
public:
    CorpseControl();
    void doUpdate();
};

class ConfusedControl : public Control {
public:
    ConfusedControl(Entity *self);
    void doUpdate();
    void handleMoveOrAttack(int to_x, int to_y);
}; 

#endif /* CONTROL_HPP */

