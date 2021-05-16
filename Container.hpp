#ifndef CONTAINER_HPP
#define CONTAINER_HPP

class Equipment;

class Container {
private:
    //Attribute
    Entity *self;
    float current_weight, max_weight, burden_boost;
    TCODList<Entity*> containing;
public:
    //Constructor & Destructor
    Container(Entity *self, float max_weight);
    ~Container();
    
    //Method
    int getItemNum();
    bool addItem(Entity *to_add);
    void removeItem(Entity *to_remove);
    void dropItem(Entity *to_drop);
    Entity* getItem(int index);
    
    //Accessor
    float getMaxWeight();
    float getCurrentWeight();
    float getBurdenBoost();
    void setMaxWeight(float input);
    void setCurrentWeight(float input);
    void setBurdenBoost(float input);
};

#endif /* CONTAINER_HPP */

