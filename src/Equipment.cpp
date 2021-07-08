#include "main.hpp"

Equipment::Equipment(Entity *self): self(self), is_primary(true) {
    for (int i = 0; i < 10; i++) {
        equipment_slot[i] = nullptr;
    }
}

void Equipment::doEquip(Entity *to_equip, int index) {
    if (equipment_slot[index] != nullptr) {
        doUnequip(index);
    }
        game.gui->addMessage(TCODColor::green, "You equiped %s", to_equip->getName().c_str());

    to_equip->item_behavior->setIsEquip(true);
    equipment_slot[index] = to_equip;
}

void Equipment::doUnequip(int index) {
    Entity *to_unequip = getEquipment(index);
    game.gui->addMessage(TCODColor::red, "You unequiped %s", to_unequip->getName().c_str());
    equipment_slot[index]->item_behavior->setIsEquip(false);
    equipment_slot[index] = nullptr;
}

Entity* Equipment::getEquipment(int index) {
    return equipment_slot[index];
}

void Equipment::getAllEquipmentAttribute(bool is_primary) {
    Entity *hand_holding = nullptr;
    if (is_primary) {
        hand_holding = equipment_slot[equipment_slot_type::primary_hand];
    }
    else {
        hand_holding = equipment_slot[equipment_slot_type::secondary_hand];
    }
    
    for (int i = 0; i < 8; i++) {
        if (equipment_slot[i] == nullptr) {continue;}
        equipment_slot[i]->item_behavior->doUpdateEquipmentAttribute(self);
    }
    
    if (hand_holding != nullptr) {
        hand_holding->item_behavior->doUpdateEquipmentAttribute(self);
    }
}

bool Equipment::isPrimaryHand() {return is_primary;}
void Equipment::setHandUsing(bool primary) {is_primary = primary;}