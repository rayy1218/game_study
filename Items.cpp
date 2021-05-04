#include "main.hpp"

int getIndexWeightedRandom(int *weight_arr) {
    int total_weight = 0;
    
    for (int i = 0; i < ITEM_TYPE; i++) {
        int weight = weight_arr[i];
        total_weight += weight;
    }std::cout << "total " << total_weight << std::endl;
    
    int random_num = game.global_rng->getInt(1, total_weight),
        cumulative_weight = 0,
        index = 0;
    std::cout << "ran " << random_num << std::endl;
    for (int i = 0; i < ITEM_TYPE; i++) {
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
            item->item_behavior = new ItemBehavior(item, 0.5, 1, true, equipment_index::unequipable);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 1, true);
            ItemPurpose *purpose = new ItemEffectBurn(6, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Can be throw at any place in sight and"
                                         " deal 6 damage for 4 round to creature"
                                         " in radius of 3");
            break;
        }
        
        case item_dict::throwing_axe: {
            item = new Entity(x, y, "Throwing Axe", 'P', TCODColor::darkSepia);
            item->item_behavior = new ItemBehavior(item, 1, 1, true, equipment_index::unequipable);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 0, false);
            ItemPurpose *purpose = new ItemDamage(15);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Deal 15 damage to selected creature in sight");
            break;
        }
        
        case item_dict::incense: {
            item = new Entity(x, y, "Incense", '8', TCODColor::gold);
            item->item_behavior = new ItemBehavior(item, 1, 1, true, equipment_index::unequipable);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 2, true);
            ItemPurpose *purpose = new ItemEffectConfusion(8);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Can be throw at any place in sight and"
                                         " confuse a creature for 8 round");
            break;
        }
        
        case item_dict::potion_healing: {
            item = new Entity(x, y, "Potion of Healing", '|', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, true, equipment_index::unequipable);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemHeal(20);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Heal 20 hp");
            break;
        }
        
        case item_dict::headwear_heavy_metal: {
            item = new Entity (x, y, "Heavy Plate Helmet", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::headwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 3);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 3");
            break;
        }
        
        case item_dict::headwear_light_metal: {
            item = new Entity (x, y, "Light Plate Helmet", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::headwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::headwear_leather: {
            item = new Entity (x, y, "Leather Hood", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::headwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::bodywear_heavy_metal: {
            item = new Entity (x, y, "Heavy Chestplate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 6);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 6");
            break;
        }
        
        case item_dict::bodywear_light_metal: {
            item = new Entity (x, y, "Light Chestplate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 4");
            break;
        }
        
        case item_dict::bodywear_reinforced_leather: {
            item = new Entity (x, y, "Reinforced Leather Gambeson", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::bodywear_cloth: {
            item = new Entity (x, y, "Tunic", '(', TCODColor::lighterGrey);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::bodywear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::legging_armored: {
            item = new Entity (x, y, "Chausses", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::legwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 3);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 5");
            break;
        }
        
        case item_dict::legging_reinforced_metal: {
            item = new Entity (x, y, "Leg Reinforcing Metal Plate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::legwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 3);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 3");
            break;
        }
        
        case item_dict::legging_reinforced_leather: {
            item = new Entity (x, y, "Leg Reinforcing Leather Plate", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::legwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::footwear_metal: {
            item = new Entity (x, y, "Plate Boots", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::footwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::footwear_leather: {
            item = new Entity (x, y, "Leather Boots", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::footwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
        
        case item_dict::armwear_metal_full: {
            item = new Entity (x, y, "Metal Vambrace", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::armwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 4");
            break;
        }
        
        case item_dict::armwear_reinforced_metal: {
            item = new Entity (x, y, "Arm Reinforcing Metal Plate", '[', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::armwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 2);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 2");
            break;
        }
        
        case item_dict::armwear_reinforced_leather: {
            item = new Entity (x, y, "Arm Reinforcing Metal Plate", '(', TCODColor::sepia);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::armwear);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
    }
    return item;
}

