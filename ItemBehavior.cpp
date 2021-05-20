#include "main.hpp"

ItemBehavior::ItemBehavior(Entity *self,  float weight, int qty, bool stackable , int equipment_index): 
                           self(self), weight(weight), qty(qty), stackable(stackable),
                           equipment_index(equipment_index), is_equiped(false) {}

ItemBehavior::~ItemBehavior() {
    delete targeting;
    delete purpose;
}

bool ItemBehavior::pick(Entity *pick_by) {
    if (!pick_by->inventory->addItem(self)) {return false;}
    game.all_item.remove(self);
    return true;
}

bool ItemBehavior::use(Entity *use_by) {
    bool purpose_used = false;
    purpose_used = targeting->doTargeting();
    if (!purpose_used) {return false;}
    use_by->inventory->removeItem(self);
    return true;
}

ItemEquipmentBehavior::ItemEquipmentBehavior(Entity *self, float weight, int qty, bool stackable, int equipment_index):
                                             ItemBehavior(self, weight, qty, stackable, equipment_index) {
    is_equiped = false;
}

bool ItemEquipmentBehavior::use(Entity *use_by) {
    if (is_equiped) {
        use_by->equipment->doUnequip(equipment_index);
        is_equiped = false;
        return true;
    }
    
    if (equipment_index == equipment_type::single_hand) {
        int option;
        option = game.gui->doSelectWeaponSlot();
        if (option == -1) {return false;}
        use_by->equipment->doEquip(self, equipment_slot_type::primary_hand + option);
        is_equiped = true;
        return true;
    }
    
    use_by->equipment->doEquip(self, equipment_index);
    is_equiped = true;
    
    if (equipment_index == equipment_type::two_hand) {
        Entity* secondary_weapon = use_by->equipment->getEquipment(equipment_slot_type::secondary_hand);
        if (secondary_weapon != nullptr) {
            use_by->equipment->doUnequip(equipment_slot_type::secondary_hand);
            secondary_weapon->item_behavior->setIsEquip(false);
        }
    }
    
    
    return true;
}

int ItemBehavior::getItemId() {return item_id;}
void ItemBehavior::setItemId(int input) {item_id = input;}
void ItemBehavior::setQty( int input ) { qty = input; }
void ItemBehavior::setWeight( float input ) { weight = input; }
bool ItemBehavior::isStackable() { return stackable; }
int ItemBehavior::getQty() { return qty; }
float ItemBehavior::getWeight() { return weight; }
int ItemBehavior::getEquipmentIndex() {return equipment_index;}
void ItemBehavior::setIsEquip(bool input) {is_equiped = input;}
bool ItemBehavior::getIsEquip() {return is_equiped;}
std::string ItemBehavior::getDesc() {return description;}
void ItemBehavior::setDesc(std::string input) {description = input;}
