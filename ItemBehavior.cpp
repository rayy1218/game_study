#include "main.hpp"

ItemBehavior::ItemBehavior( Entity *self, float weight, int qty, int max_stack ): self( self ), weight(weight) {}

ItemBehavior::~ItemBehavior() {
    
}

void ItemBehavior::setMaxStack( int input ) { max_stack = input; }
void ItemBehavior::setQty( int input ) { qty = input; }
void ItemBehavior::setWeight( float input ) { weight = input; }
int ItemBehavior::getMaxStack() { return max_stack; }
int ItemBehavior::getQty() { return qty; }
float ItemBehavior::getWeight() { return weight; }

bool ItemBehavior::pick(Entity *pick_by) {
    if ( !pick_by->inventory->addItem(self) ) { return false; }
    game.all_item.remove(self);
    return true;
}

bool ItemBehavior::use(Entity *use_by) {
    bool purpose_used = false;
    purpose_used = targeting->doTargeting();
    if (!purpose_used) {return false;}
    use_by->inventory->removeItem(self);
    delete self;
    return true;
}