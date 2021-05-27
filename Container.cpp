#include "main.hpp"

Container::Container(Entity *self, float max_weight): self(self), 
                                                      max_weight(max_weight), 
                                                      current_weight(0),
                                                      burden_boost(1) {}

Container::~Container() {
    containing.clearAndDelete();
}

int Container::getItemNum() {
    return containing.size();
}

bool Container::addItem( Entity *to_add ) {
    if ((getCurrentWeight() + 
         to_add->item_behavior->getWeight() * 
         to_add->item_behavior->getQty()) > getMaxWeight()) {
        
        return false;
    } 
    
    setCurrentWeight(getCurrentWeight() + to_add->item_behavior->getWeight() * 
                     to_add->item_behavior->getQty());
    
    if (to_add->item_behavior->isStackable()) {
        bool item_existed = false;
        for (Entity *item : containing) {
            if (to_add->item_behavior->getItemId() == item->item_behavior->getItemId()) {
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

void Container::deleteItem(Entity* to_delete, bool delete_all) {
    int qty = (delete_all)? to_delete->item_behavior->getQty() : 1;
    setCurrentWeight(getCurrentWeight() - to_delete->item_behavior->getWeight() * qty);
    if (to_delete->item_behavior->getQty() == 1 || delete_all) {
        containing.remove(to_delete);
        delete to_delete;
        return;
    }
    
    to_delete->item_behavior->setQty(to_delete->item_behavior->getQty() - 1);
}

void Container::removeItem(Entity* to_remove, bool remove_all) {
    int qty = (remove_all)? to_remove->item_behavior->getQty() : 1;
    setCurrentWeight(getCurrentWeight() - to_remove->item_behavior->getWeight() * qty);
    if (to_remove->item_behavior->getQty() == 1 || remove_all) {
        containing.remove(to_remove);
        return;
    }
    
    to_remove->item_behavior->setQty(to_remove->item_behavior->getQty() - 1);
}

void Container::dropItem(Entity* to_drop, bool drop_all) {
    if (to_drop->item_behavior->getIsEquip()) {
        self->equipment->doUnequip(to_drop->item_behavior->getEquipmentIndex());
    }
    
    if (drop_all || to_drop->item_behavior->getQty() == 1) {
        removeItem(to_drop, drop_all);
    
        game.all_item.push(to_drop);
        to_drop->setX(self->getX());
        to_drop->setY(self->getY());
        
        return;
    }
    else if (to_drop->item_behavior->getQty() > 1) {
        removeItem(to_drop);
        to_drop = getItem(0, 0, to_drop->item_behavior->getItemId());
        
        game.all_item.push(to_drop);
        to_drop->setX(self->getX());
        to_drop->setY(self->getY());
        
        return;
    }
}

Entity* Container::getIndexItem(int index) {
    if (containing.size() - index < 1) {return nullptr;}
    return containing.get(index);
}

float Container::getMaxWeight() {return max_weight * burden_boost;}
float Container::getCurrentWeight() {return current_weight;}
float Container::getBurdenBoost() {return burden_boost;}
void Container::setMaxWeight(float input) {max_weight = input;}
void Container::setCurrentWeight(float input) {current_weight = input;}
void Container::setBurdenBoost(float input) {burden_boost = input;}