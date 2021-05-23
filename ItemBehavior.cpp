#include "main.hpp"

EquipmentAttribute::EquipmentAttribute(int defense, int speed): defense(defense),
                                                                speed(speed) {}

WeaponAttribute::WeaponAttribute(int attack, int speed): attack(attack), 
                                                         speed(speed) {}

ItemBehavior::ItemBehavior(Entity *self,  float weight, int qty, bool stackable): 
                           self(self), weight(weight), qty(qty), stackable(stackable),
                           targeting(nullptr), purpose(nullptr) {}

ItemBehavior::~ItemBehavior() {
    if (targeting) {delete targeting;}
    if (purpose) {delete purpose;}
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
    use_by->inventory->deleteItem(self);
    return true;
}

ItemEquipmentBehavior::ItemEquipmentBehavior(Entity *self, float weight, int qty,
                                             bool stackable, int equipment_index, 
                                             EquipmentAttribute *equipment_attribute,
                                             WeaponAttribute *weapon_attribute):
                                             ItemBehavior(self, weight, qty, stackable),
                                             equipment_index(equipment_index),
                                             equipment_attribute(equipment_attribute),
                                             weapon_attribute(weapon_attribute),
                                             is_equiped(false) {}

ItemEquipmentBehavior::~ItemEquipmentBehavior() {
    if (targeting) {delete targeting;}
    if (purpose) {delete purpose;}
    if (equipment_attribute) {delete equipment_attribute;}
    if (weapon_attribute) {delete weapon_attribute;}
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

void ItemBehavior::doUpdateEquipmentAttribute(Entity *self) {
    return;
}

void ItemEquipmentBehavior::doUpdateEquipmentAttribute(Entity *self) {
    CombatBehavior *cbt = self->combat_behavior;
    if (weapon_attribute != nullptr) {
        cbt->setEquipmentAtkPoint(cbt->getEquipmentAtkPoint() + weapon_attribute->attack);
        cbt->setSpeed(weapon_attribute->speed);
    }
    if (equipment_attribute != nullptr) {
        cbt->setEquipmentDefPoint(cbt->getEquipmentDefPoint() + equipment_attribute->defense);
        cbt->setSpeed(cbt->getSpeed() + equipment_attribute->speed);
    }
}

int ItemBehavior::getItemId() {return item_id;}
void ItemBehavior::setItemId(int input) {item_id = input;}
void ItemBehavior::setQty( int input ) { qty = input; }
void ItemBehavior::setWeight( float input ) { weight = input; }
bool ItemBehavior::isStackable() { return stackable; }
int ItemBehavior::getQty() { return qty; }
float ItemBehavior::getWeight() { return weight; }
int ItemBehavior::getEquipmentIndex() {return equipment_type::unequipable;}
void ItemBehavior::setIsEquip(bool input) {return;}
bool ItemBehavior::getIsEquip() {return false;}
int ItemEquipmentBehavior::getEquipmentIndex() {return equipment_index;}
void ItemEquipmentBehavior::setIsEquip(bool input) {is_equiped = input;}
bool ItemEquipmentBehavior::getIsEquip() {return is_equiped;}
std::string ItemBehavior::getDesc() {return description;}
void ItemBehavior::setDesc(std::string input) {description = input;}
