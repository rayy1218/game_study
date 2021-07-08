#ifndef CONTROL_HPP
#define CONTROL_HPP

class Pathfinding;
class Purpose;

class Control {
protected:
    Entity *self;
public:
    Control(Entity *self);
    virtual ~Control();
    
    virtual void doUpdate();
    float getDistanceTo(float x, float y);
    virtual std::string getStatus();
};

class PlayerControl : public Control {
public:
    PlayerControl(Entity *self);
    
    void doUpdate();
    bool handleMoveOrAttack(int dx, int dy);
    bool handleCharInput(int ascii);
    std::string getStatus();
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
    std::string getStatus();
};

class CorpseControl : public Control {
public:
    CorpseControl();
    
    void doUpdate();
    std::string getStatus();
};

class ConfusedControl : public Control {
public:
    ConfusedControl(Entity *self);
    
    void doUpdate();
    void handleMoveOrAttack(int to_x, int to_y);
    std::string getStatus();
}; 

class StunnedControl : public Control {
public:
    StunnedControl(Entity *self);
    
    void doUpdate();
    std::string getStatus();
};

class StepTriggerControl : public Control {
public:
    StepTriggerControl(Entity *self);
    virtual void doUpdate();
    Entity* getCharacterStepOn();
};

class UpStairControl : public StepTriggerControl {
public:
    UpStairControl(Entity *self);
    void doUpdate();
};

class DownStairControl : public StepTriggerControl {
public:
    DownStairControl(Entity *self);
    void doUpdate();
};

class HoleControl : public StepTriggerControl {
public:
    HoleControl(Entity *self);
    void doUpdate();
};

class TrapControl : public StepTriggerControl {
private:
    Purpose *purpose;
    TCODColor orig_color;
    int orig_char;
public:
    TrapControl(Entity *self, Purpose *purpose);
    void doUpdate();
};

#endif /* CONTROL_HPP */

