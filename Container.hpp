#ifndef CONTAINER_HPP
#define CONTAINER_HPP

class Equipment;

class Container {
private:
    //Attribute
    Entity *self;
    float current_weight, max_weight;
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
    
};

#endif /* CONTAINER_HPP */

