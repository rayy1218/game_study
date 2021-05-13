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
            item = new Entity(x, y, "Molotov", 'd', TCODColor::darkRed);
            item->item_behavior = new ItemBehavior(item, 0.2, 1, true, equipment_type::unequipable);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 1, true);
            Purpose *purpose = new PurposeEffectBurn(6, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Can be throw at any place in sight and"
                                         " deal 6 damage for 4 round to creature"
                                         " in radius of 3");
            break;
        }
        
        case item_dict::throwing_knife: {
            item = new Entity(x, y, "Throwing Knife", 'P', TCODColor::darkSepia);
            item->item_behavior = new ItemBehavior(item, 0.8, 1, true, equipment_type::unequipable);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 0, false);
            Purpose *purpose = new PurposeDamage(15);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Deal 15 damage to selected creature in sight");
            break;
        }
        
        case item_dict::incense: {
            item = new Entity(x, y, "Incense", '8', TCODColor::gold);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, true, equipment_type::unequipable);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 2, true);
            Purpose *purpose = new PurposeEffectConfusion(8);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Can be throw at any place in sight and"
                                         " confuse a creature for 8 round");
            break;
        }
        
        case item_dict::potion_healing: {
            item = new Entity(x, y, "Potion of Healing", '!', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.2, 1, true, equipment_type::unequipable);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeHeal(20);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Heal 20 hp");
            break;
        }
        
        case item_dict::food: {
            item = new Entity(x, y, "Food", 'Q', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.2, 1, true, equipment_type::unequipable);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemFood(100);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Decrease stravation by 100");
            break;
        }
        
        case item_dict::headwear_heavy_metal: {
            item = new Entity (x, y, "Heavy Plate Helmet", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 2.5, 1, false, equipment_type::headwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 3);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 3");
            break;
        }
        
        case item_dict::headwear_light_metal: {
            item = new Entity (x, y, "Light Plate Helmet", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.2, 1, false, equipment_type::headwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::headwear_leather: {
            item = new Entity (x, y, "Leather Hood", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.4, 1, false, equipment_type::headwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::bodywear_heavy_metal: {
            item = new Entity (x, y, "Heavy Chestplate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 4.8, 1, false, equipment_type::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 6);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 6");
            break;
        }
        
        case item_dict::bodywear_light_metal: {
            item = new Entity (x, y, "Light Chestplate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 2.5, 1, false, equipment_type::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 4");
            break;
        }
        
        case item_dict::bodywear_reinforced_leather: {
            item = new Entity (x, y, "Reinforced Leather Gambeson", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 1, 1, false, equipment_type::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::bodywear_cloth: {
            item = new Entity (x, y, "Tunic", '(', TCODColor::lighterGrey);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.2, 1, false, equipment_type::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::legging_armored: {
            item = new Entity (x, y, "Chausses", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_type::legwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 5);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 5");
            break;
        }
        
        case item_dict::legging_reinforced_metal: {
            item = new Entity (x, y, "Leg Reinforcing Metal Plate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.5, 1, false, equipment_type::legwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 3);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 3");
            break;
        }
        
        case item_dict::legging_reinforced_leather: {
            item = new Entity (x, y, "Leg Reinforcing Leather Plate", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.8, 1, false, equipment_type::legwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::footwear_metal: {
            item = new Entity (x, y, "Plate Boots", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1, 1, false, equipment_type::footwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::footwear_leather: {
            item = new Entity (x, y, "Leather Boots", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.4, 1, false, equipment_type::footwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::armwear_metal_full: {
            item = new Entity (x, y, "Metal Vambrace", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.8, 1, false, equipment_type::armwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 4");
            break;
        }
        
        case item_dict::armwear_reinforced_metal: {
            item = new Entity (x, y, "Arm Reinforcing Metal Plate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.2, 1, false, equipment_type::armwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::armwear_reinforced_leather: {
            item = new Entity (x, y, "Arm Reinforcing Metal Plate", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.6, 1, false, equipment_type::armwear);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::weapon_gladius: {
            item = new Entity (x, y, "Gladius", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1, 1, false, equipment_type::single_hand);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(8, 45);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 8 and have speed of 45");
            break;
        }
        
        case item_dict::weapon_axe: {
            item = new Entity (x, y, "Axe", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.2, 1, false, equipment_type::single_hand);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(10, 40);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 10 and have speed of 40");
            break;
        }
        
        case item_dict::weapon_mace: {
            item = new Entity (x, y, "Mace", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 1.5, 1, false, equipment_type::single_hand);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(12, 40);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 12 and have speed of 40");
            break;
        }
        
        case item_dict::weapon_longsword: {
            item = new Entity (x, y, "Longsword", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_type::two_hand);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(15, 30);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 15 and have speed of 30");
            break;
        }
        
        case item_dict::weapon_battlehammer: {
            item = new Entity (x, y, "Battlehammer", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3.5, 1, false, equipment_type::two_hand);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(18, 25);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 18 and have speed of 25");
            break;
        }
        
        case item_dict::weapon_battleaxe: {
            item = new Entity (x, y, "Battleaxe", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_type::two_hand);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(20, 25);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 20 and have speed of 25");
            break;
        }
        
        case item_dict::weapon_knuckle: {
            item = new Entity (x, y, "Knuckle", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.2, 1, false, equipment_type::secondary_weapon);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(6, 90);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 6 and have speed of 90");
            break;
        }
        
        case item_dict::weapon_dagger: {
            item = new Entity (x, y, "Dagger", '|', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 0.2, 1, false, equipment_type::secondary_weapon);
            item->item_behavior->targeting = new SelfTarget(item);
            Purpose *purpose = new PurposeItemWeapon(6, 80);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self attack by 6 and have speed of 80");
            break;
        }
        
    }
    return item;
}

