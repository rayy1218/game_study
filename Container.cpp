#include "main.hpp"

Container::Container(Entity *self, float max_weight): self(self), 
                                                      max_weight(max_weight), 
                                                      current_weight(0) {}

Container::~Container() {
    containing.clearAndDelete();
}

int Container::getItemNum() {
    return containing.size();
}

bool Container::addItem( Entity *to_add ) {
    if ((current_weight + 
         to_add->item_behavior->getWeight() * 
         to_add->item_behavior->getQty()) > max_weight) {
        
        return false;
    } 
    
    current_weight += to_add->item_behavior->getWeight() * 
                      to_add->item_behavior->getQty();
    
    if (to_add->item_behavior->isStackable()) {
        bool item_existed = false;
        for (Entity *item : containing) {
            if (to_add->getName() == item->getName()) {
                item->item_behavior->setQty(item->item_behavior->getQty() +
                                            to_add->item_behavior->getQty());
                
                item_existed = true;
            }
        }
        if (!item_existed) {
            containing.push(to_add);
        }
        
    }
    else {
        containing.push(to_add);
    }

    return true;
}

void Container::removeItem(Entity* to_remove) {
    current_weight -= to_remove->item_behavior->getWeight();
    
    if (!to_remove->item_behavior->isStackable() || to_remove->item_behavior->getQty() == 1) {
        containing.remove(to_remove);
        delete to_remove;
    }
    else {
        to_remove->item_behavior->setQty(to_remove->item_behavior->getQty() - 1);
    }
}

void Container::dropItem(Entity* to_drop) {
    if (to_drop->item_behavior->getIsEquip()) {
        self->equipment->doUnequip(to_drop->item_behavior->getEquipmentIndex());
    }
    game.all_item.push(to_drop);
    
    current_weight -= to_drop->item_behavior->getWeight();
    
    to_drop->item_behavior->setQty(to_drop->item_behavior->getQty() - 1);
    if (to_drop->item_behavior->getQty() == 0) {
        containing.remove(to_drop);
    }
    
    to_drop->setX(self->getX());
    to_drop->setY(self->getY());
}

Entity* Container::getItem(int index) {
    if (containing.size() - index < 1) {return nullptr;}
    return containing.get(index);
}

float Container::getMaxWeight() {return max_weight;}
float Container::getCurrentWeight() {return current_weight;}