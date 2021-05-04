#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

enum equipment_index {
    unequipable     = -1,
    headwear        = 0,
    bodywear        = 1,
    legwear         = 2,
    footwear        = 3,
    armwear         = 4,
    ring1           = 5,
    ring2           = 6,
    accessory       = 7,
    primary_hand    = 8,
    secondary_hand  = 9
};

class Equipment {
private:
    //Attribute
    Entity *self;
    Entity *equipment_slot[10];
public:
    //Constructor & destructor
    Equipment(Entity *self);
    
    //Method
    void doEquip(Entity *to_equip, int index);
    void doUnequip(int index);
    Entity* getEquipment(int index);
    void getAllEquipmentAttribute();
};

#endif /* EQUIPMENT_HPP */

