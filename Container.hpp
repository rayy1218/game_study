#ifndef CONTAINER_HPP
#define CONTAINER_HPP

class Equipment;

class Container {
private:
    Entity *self;
    float current_weight, max_weight;
    TCODList<Entity*> containing;
public:
    Container(Entity *self, float max_weight);
    ~Container();
    
    bool addItem(Entity *to_add);
    Entity* getItem(int index);
    int getItemNum();
    void removeItem(Entity *to_remove);
    void dropItem(Entity *to_drop);
    float getMaxWeight();
    float getCurrentWeight();
};

#endif /* CONTAINER_HPP */

