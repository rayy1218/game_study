#ifndef ITEMS_HPP
#define ITEMS_HPP

class Entity;

static const int ITEM_TYPE = 19;

enum item_dict{
    //UTILITY
    molotov = 1,
    throwing_axe = 2,
    incense = 3,
    
    //POTION
    potion_healing = 4,
    
    //HEADWEAR
    headwear_heavy_metal = 5,
    headwear_light_metal = 6,
    headwear_leather = 7,
    
    //BODYWEAR
    bodywear_heavy_metal = 8,
    bodywear_light_metal = 9,
    bodywear_reinforced_leather =10,
    bodywear_cloth = 11,
    
    //LEGGING
    legging_armored = 12,
    legging_reinforced_metal = 13,
    legging_reinforced_leather = 14,
    
    //FOOTWEAR
    footwear_metal,
    footwear_leather,
    
    //ARMWEAR
    armwear_metal_full,
    armwear_reinforced_metal,
    armwear_reinforced_leather,
    
    //RING
    ring_focus,
    ring_fire,
    ring_rage,
    ring_purification,
    
    //ACCESSORY
    accessory_lantern,
    accessory_belt_bag,
};

int getIndexWeightedRandom(int *weight_arr);
Entity* getItem(const int x, const int y, const int index);

#endif /* ITEMS_HPP */

