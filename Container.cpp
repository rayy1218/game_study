#include "main.hpp"

Container::Container(Entity *self, float max_weight): self(self), 
                                                      max_weight(max_weight), 
                                                      current_weight(0) {}

Container::~Container() {
    containing.clearAndDelete();
}

bool Container::addItem( Entity *to_add ) {
    if ( ( current_weight + to_add->item_behavior->getWeight() ) > max_weight ) {
        return false;
    } 
    containing.push(to_add);
    
    current_weight += to_add->item_behavior->getWeight();
    return true;
}

int Container::getItemNum() {
    return containing.size();
}

float Container::getMaxWeight() {return max_weight;}
float Container::getCurrentWeight() {return current_weight;}

Entity* Container::getItem(int index) {
    if (containing.size() - index < 1) {return nullptr;}
    return containing.get(index);
}

void Container::removeItem(Entity* to_remove) {
    containing.remove(to_remove);
    current_weight -= to_remove->item_behavior->getWeight();
}

void Container::dropItem(Entity* to_drop) {
    game.all_item.push(to_drop);
    removeItem(to_drop);
    to_drop->setX(self->getX());
    to_drop->setY(self->getY());
}