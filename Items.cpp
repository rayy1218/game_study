#include "main.hpp"

int getIndexWeightedRandom(int *weight_arr, int arr_size) {
    int total_weight = 0;
    
    for (int i = 0; i < arr_size; i++) {
        int weight = weight_arr[i];
        total_weight += weight;
    }
    
    int random_num = game.global_rng->getInt(1, total_weight),
        cumulative_weight = 0,
        index = 0;
    for (int i = 0; i < arr_size; i++) {
        int weight = weight_arr[i];
        cumulative_weight += weight;
        if (random_num - cumulative_weight <= 0) {
            return index + 1;
        }
        index++;
    }
}

Entity* getItem(const int x, const int y, const int index) {
    Entity* item;
    
    switch (index) {
        case item_dict::molotov: {
            item = new Entity(x, y, "molotov", 'd', TCODColor::darkRed);
            item->item_behavior = new ItemBehavior(item, 0.2, 1, true);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 1, true);
            item->item_behavior->purpose = new PurposeEffectBurn(6, 4);
            item->item_behavior->setDesc("can be throw at any place in sight and"
                                         " deal 6 damage for 4 round to creature"
                                         " in radius of 3");
            break;
        }
        
        case item_dict::throwing_knife: {
            item = new Entity(x, y, "throwing knife", 'P', TCODColor::darkSepia);
            item->item_behavior = new ItemBehavior(item, 0.8, 1, true);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 0, false);
            item->item_behavior->purpose = new PurposeDamage(15);
            item->item_behavior->setDesc("deal 15 damage to selected creature in sight");
            break;
        }
        
        case item_dict::incense: {
            item = new Entity(x, y, "incense", '8', TCODColor::gold);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, true);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 2, true);
            item->item_behavior->purpose = new PurposeEffectConfusion(8);
            item->item_behavior->setDesc("can be throw at any place in sight and"
                                         " confuse a creature for 8 round");
            break;
        }
        
        case item_dict::potion_healing: {
            item = new Entity(x, y, "potion of healing", '!', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.2, 1, true);
            item->item_behavior->targeting = new SelfTarget(item);
            item->item_behavior->purpose = new PurposeHeal(20);
            item->item_behavior->setDesc("heal 20 hp");
            break;
        }
        
        case item_dict::food: {
            item = new Entity(x, y, "food", 'Q', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.2, 1, true);
            item->item_behavior->targeting = new SelfTarget(item);
            item->item_behavior->purpose = new PurposeItemFood(100);
            item->item_behavior->setDesc("decrease stravation by 100");
            break;
        }
        
        case item_dict::headwear_heavy_metal: {
            item = new Entity (x, y, "heavy plate helmet", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 2.5, 1, false, equipment_type::headwear,
                                                            new EquipmentAttribute(3, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 3");
            break;
        }
        
        case item_dict::headwear_light_metal: {
            item = new Entity (x, y, "light plate helmet", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.2, 1, false, equipment_type::headwear,
                                                            new EquipmentAttribute(2, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 2");
            break;
        }
        
        case item_dict::headwear_leather: {
            item = new Entity (x, y, "leather hood", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.4, 1, false, equipment_type::headwear,
                                                            new EquipmentAttribute(1, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 1");
            break;
        }
        
        case item_dict::bodywear_heavy_metal: {
            item = new Entity (x, y, "heavy chestplate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 4.8, 1, false, equipment_type::bodywear,
                                                            new EquipmentAttribute(6, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 6");
            break;
        }
        
        case item_dict::bodywear_light_metal: {
            item = new Entity (x, y, "light chestplate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 2.5, 1, false, equipment_type::bodywear,
                                                            new EquipmentAttribute(4, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 4");
            break;
        }
        
        case item_dict::bodywear_reinforced_leather: {
            item = new Entity (x, y, "reinforced leather gambeson", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 1, 1, false, equipment_type::bodywear,
                                                            new EquipmentAttribute(2, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 2");
            break;
        }
        
        case item_dict::bodywear_cloth: {
            item = new Entity (x, y, "tunic", '(', TCODColor::lighterGrey);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.2, 1, false, equipment_type::bodywear,
                                                            new EquipmentAttribute(1, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 1");
            break;
        }
        
        case item_dict::legging_armored: {
            item = new Entity (x, y, "chausses", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_type::legwear,
                                                            new EquipmentAttribute(5, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 5");
            break;
        }
        
        case item_dict::legging_reinforced_metal: {
            item = new Entity (x, y, "leg reinforcing metal plate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.5, 1, false, equipment_type::legwear,
                                                            new EquipmentAttribute(3, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 3");
            break;
        }
        
        case item_dict::legging_reinforced_leather: {
            item = new Entity (x, y, "leg reinforcing leather plate", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.8, 1, false, equipment_type::legwear,
                                                            new EquipmentAttribute(1, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 1");
            break;
        }
        
        case item_dict::footwear_metal: {
            item = new Entity (x, y, "plate boots", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1, 1, false, equipment_type::footwear,
                                                            new EquipmentAttribute(2, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 2");
            break;
        }
        
        case item_dict::footwear_leather: {
            item = new Entity (x, y, "leather boots", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.4, 1, false, equipment_type::footwear,
                                                            new EquipmentAttribute(1, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 1");
            break;
        }
        
        case item_dict::armwear_metal_full: {
            item = new Entity (x, y, "metal vambrace", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.8, 1, false, equipment_type::armwear,
                                                            new EquipmentAttribute(4, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 4");
            break;
        }
        
        case item_dict::armwear_reinforced_metal: {
            item = new Entity (x, y, "arm reinforcing metal plate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.2, 1, false, equipment_type::armwear,
                                                            new EquipmentAttribute(2, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 2");
            break;
        }
        
        case item_dict::armwear_reinforced_leather: {
            item = new Entity (x, y, "arm reinforcing metal plate", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.6, 1, false, equipment_type::armwear,
                                                            new EquipmentAttribute(1, 0), nullptr);
            item->item_behavior->setDesc("increase self defense by 1");
            break;
        }
        
        case item_dict::weapon_gladius: {
            item = new Entity (x, y, "gladius", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1, 1, false, equipment_type::single_hand,
                                                            nullptr, new WeaponAttribute(8, 45));
            item->item_behavior->setDesc("increase self attack by 8 and have speed of 45");
            break;
        }
        
        case item_dict::weapon_axe: {
            item = new Entity (x, y, "axe", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.2, 1, false, equipment_type::single_hand,
                                                            nullptr, new WeaponAttribute(10, 40));
            item->item_behavior->setDesc("increase self attack by 10 and have speed of 40");
            break;
        }
        
        case item_dict::weapon_mace: {
            item = new Entity (x, y, "mace", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.5, 1, false, equipment_type::single_hand,
                                                            nullptr, new WeaponAttribute(12, 40));
            item->item_behavior->setDesc("increase self attack by 12 and have speed of 40");
            break;
        }
        
        case item_dict::weapon_longsword: {
            item = new Entity (x, y, "longsword", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_type::two_hand,
                                                            nullptr, new WeaponAttribute(15, 30));
            item->item_behavior->setDesc("increase self attack by 15 and have speed of 30");
            break;
        }
        
        case item_dict::weapon_battlehammer: {
            item = new Entity (x, y, "battlehammer", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3.5, 1, false, equipment_type::two_hand,
                                                            nullptr, new WeaponAttribute(18, 25));
            item->item_behavior->setDesc("increase self attack by 18 and have speed of 25");
            break;
        }
        
        case item_dict::weapon_battleaxe: {
            item = new Entity (x, y, "battleaxe", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_type::two_hand,
                                                            nullptr, new WeaponAttribute(20, 25));
            item->item_behavior->setDesc("increase self attack by 20 and have speed of 25");
            break;
        }
        
        case item_dict::weapon_knuckle: {
            item = new Entity (x, y, "knuckle", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.2, 1, false, equipment_type::secondary_weapon,
                                                            nullptr, new WeaponAttribute(6, 90));
            item->item_behavior->setDesc("increase self attack by 6 and have speed of 90");
            break;
        }
        
        case item_dict::weapon_dagger: {
            item = new Entity (x, y, "dagger", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.2, 1, false, equipment_type::secondary_weapon,
                                                            nullptr, new WeaponAttribute(6, 80));
            item->item_behavior->setDesc("increase self attack by 6 and have speed of 80");
            break;
        }
        
    }
    
    item->item_behavior->setItemId(index);
    
    return item;
}

