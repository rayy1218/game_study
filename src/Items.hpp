#ifndef ITEMS_HPP
#define ITEMS_HPP

class Entity;

static const int ITEM_TYPE = 47;

enum item_dict{
    null_item = 0,
    //UTILITY
    molotov = 1,
    throwing_knife = 2,
    incense = 3,
    food = 4,
    
    //POTION
    potion_healing_fine = 5,
    potion_healing_standard = 6,
    potion_healing_flawed = 7,
    potion_mana_fine = 8,
    potion_mana_standard = 9,
    potion_mana_flawed = 10,
    potion_rage_fine = 11,
    potion_rage_standard = 12,
    potion_rage_flawed = 13,
    potion_protection_fine = 14,
    potion_protection_standard = 15,
    potion_protection_flawed = 16,
    
    //HEADWEAR
    headwear_heavy_metal = 17,
    headwear_light_metal = 18,
    headwear_leather = 19,
    
    //BODYWEAR
    bodywear_heavy_metal = 20,
    bodywear_light_metal = 21,
    bodywear_reinforced_leather = 22,
    bodywear_cloth = 23,
    
    //LEGGING
    legging_armored = 24,
    legging_reinforced_metal = 25,
    legging_reinforced_leather = 26,
    
    //FOOTWEAR
    footwear_metal = 27,
    footwear_leather = 28,
    
    //ARMWEAR
    armwear_metal_full = 29,
    armwear_reinforced_metal = 30,
    armwear_reinforced_leather = 31,
    
    //SINGLE HANDED WEAPON
    weapon_gladius = 32,
    weapon_axe = 33,
    weapon_mace = 34,
    
    //TWO HANDED WEAPON
    weapon_longsword = 35,
    weapon_battleaxe = 36,
    weapon_battlehammer = 37,
    
    //SECONDARY WEAPON
    weapon_knuckle = 38,
    weapon_dagger = 39,
    
    //MATERIAL
    material_copper_chunk = 40,

    //tome
    tome_firebolt = 41,
    tome_lighting_chain = 42,
    tome_windblade = 43,

    //loot
    loot_goblin = 44,
    
    //harvest
    harvest_meat = 45,
    harvest_hide = 46,
    harvest_bone = 47
};

int getIndexWeightedRandom(int *weight_arr, int arr_size);
Entity* getItem(const int x, const int y, const int index);

#endif /* ITEMS_HPP */

