#ifndef ENTITY_HPP
#define ENTITY_HPP

class Control;
class MoveBehavior;
class CombatBehavior;
class Container;
class ItemBehavior;
class Effect;
class Equipment;

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
   
    //Constructer and destructor
    Entity(int x, int y, std::string name, int ascii_char, TCODColor ascii_color);
    ~Entity();
    
    //Accessor
    int getX();
    int getY();
    int getType();
    int getAsciiChar();
    std::string getName();
    TCODColor getAsciiColor();
    void setX(int input);
    void setY(int input);
    void setName(std::string input);
    void setType(int input);
    void setAsciiChar(int input);
    void setAsciiColor(TCODColor input);
    
    //Method
    void doUpdate();
    void doRender();
};

#endif /* ENTITY_HPP */

