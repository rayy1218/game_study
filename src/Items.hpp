#ifndef ITEMS_HPP
#define ITEMS_HPP

class Entity;

static const int ITEM_TYPE = 19;

enum item_dict{
    //UTILITY
    molotov = 1,
    throwing_knife,
    incense,
    food,
    
    //POTION
    potion_healing_fine,
    potion_healing_standard,
    potion_healing_flawed,
    potion_mana_fine,
    potion_mana_standard,
    potion_mana_flawed,
    potion_rage_fine,
    potion_rage_standard,
    potion_rage_flawed,
    potion_protection_fine,
    potion_protection_standard,
    potion_protection_flawed,
    potion_joy_fine,
    potion_joy_standard,
    potion_joy_flawed,
    potion_feather_fall,
    
    //HEADWEAR
    headwear_heavy_metal,
    headwear_light_metal,
    headwear_leather,
    
    //BODYWEAR
    bodywear_heavy_metal,
    bodywear_light_metal,
    bodywear_reinforced_leather,
    bodywear_cloth,
    
    //LEGGING
    legging_armored,
    legging_reinforced_metal,
    legging_reinforced_leather,
    
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
    
    //SINGLE HANDED WEAPON
    weapon_gladius,
    weapon_axe,
    weapon_mace,
    
    //TWO HANDED WEAPON
    weapon_longsword,
    weapon_battleaxe,
    weapon_battlehammer,
    
    //SECONDARY WEAPON
    weapon_knuckle,
    weapon_dagger,
    
    //MATERIAL
    material_copper_chunk,
            
    //loot
    loot_goblin,
    
    //harvest
    harvest_meat,
    harvest_hide,
    harvest_bone
};

int getIndexWeightedRandom(int *weight_arr, int arr_size);
Entity* getItem(const int x, const int y, const int index);

#endif /* ITEMS_HPP */

