#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

enum equipment_type {
    unequipable      = -1,
    headwear         = 0,
    bodywear         = 1,
    legwear          = 2,
    footwear         = 3,
    armwear          = 4,
    ring             = 5,
    accessory        = 7,
    two_hand         = 8,
    secondary_weapon = 9,
    single_hand      = 10,
};

enum equipment_slot_type {
    head           = 0,
    body           = 1,
    leg            = 2,
    foot           = 3,
    arm            = 4,
    ring1          = 5,
    ring2          = 6,
    accessory1     = 7,
    primary_hand   = 8,
    secondary_hand = 9
};

class Equipment {
private:
    //Attribute
    Entity *self;
    Entity *equipment_slot[10];
    bool is_primary;
public:
    //Constructor & destructor
    Equipment(Entity *self);
    
    bool isPrimaryHand();
    void setHandUsing(bool is_primary);        
    
    //Method
    void doEquip(Entity *to_equip, int index);
    void doUnequip(int index);
    Entity* getEquipment(int index);
    void getAllEquipmentAttribute(bool is_primary);
};

#endif /* EQUIPMENT_HPP */

