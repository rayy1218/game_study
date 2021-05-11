#ifndef ITEMS_HPP
#define ITEMS_HPP

class Entity;

static const int ITEM_TYPE = 19;

enum item_dict{
    //UTILITY
    molotov = 1,
    throwing_knife = 2,
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
    footwear_metal = 15,
    footwear_leather = 16,
    
    //ARMWEAR
    armwear_metal_full = 17,
    armwear_reinforced_metal = 18,
    armwear_reinforced_leather = 19,
    
    //RING
    ring_focus = 20,
    ring_fire = 21,
    ring_rage = 22,
    ring_purification = 23,
    
    //ACCESSORY
    accessory_lantern = 24,
    accessory_belt_bag = 25,
    
    //SINGLE HANDED WEAPON
    weapon_gladius= 26,
    weapon_axe,
    weapon_mace,
    
    //TWO HANDED WEAPON
    weapon_longsword,
    weapon_battleaxe,
    weapon_battlehammer,
    
    //SECONDARY WEAPON
    weapon_knuckle,
    weapon_dagger,
};

int getIndexWeightedRandom(int *weight_arr, int arr_size);
Entity* getItem(const int x, const int y, const int index);

#endif /* ITEMS_HPP */

