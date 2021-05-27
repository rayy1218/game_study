#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "CorpseInteraction.hpp"


class Control;
class MoveBehavior;
class CombatBehavior;
class Container;
class ItemBehavior;
class Effect;
class Equipment;
class CorpseInteraction;

class Entity {
private:
    int position_x, position_y;
    std::string name;
    int ascii;
    TCODColor ascii_color;
    
public:
    //Attribute
    TCODList<Effect*> all_effect;
    
    //Behavior
    Control *control;
    MoveBehavior *move_behavior;
    CombatBehavior *combat_behavior;
    Container *inventory;
    ItemBehavior *item_behavior;
    Equipment *equipment;
    CorpseInteraction *corpse_interact;
   
    //Constructer & destructor
    Entity(int x, int y, std::string name, int ascii_char, TCODColor ascii_color);
    ~Entity();
    
    //Method
    void doUpdate();
    void doRender();
    
    //Accessor
    int getX();
    int getY();
    std::string getName();
    int getAsciiChar();
    TCODColor getAsciiColor();
    void setX(int input);
    void setY(int input);
    void setName(std::string input);
    void setAsciiChar(int input);
    void setAsciiColor(TCODColor input);
};

#endif /* ENTITY_HPP */

