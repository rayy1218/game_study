#include "main.hpp"

Entity* getItem(const int x, const int y, const int index) {
    Entity* item;
    
    switch (index) {
        case item_dict::potion_healing: {
            item = new Entity(x, y, "Potion of Healing", '|', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, true, 0);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemHeal(20);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Heal 20 hp");
            break;
        }
        
        case item_dict::throwing_axe: {
            item = new Entity(x, y, "Throwing Axe", 'P', TCODColor::darkSepia);
            item->item_behavior = new ItemBehavior(item, 1, 1, true, 0);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 0, false);
            ItemPurpose *purpose = new ItemDamage(15);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Deal 15 damage to selected creature in sight");
            break;
        }
        
        case item_dict::molotov: {
            item = new Entity(x, y, "Molotov", 'd', TCODColor::darkRed);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, true, 0);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 3, true);
            ItemPurpose *purpose = new ItemEffectBurn(6, 4);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Can be throw at any place in sight and deal 6 damage for 4 round to creature in radius of 3");
            break;
        }
        
        case item_dict::incense: {
            item = new Entity(x, y, "Incense", '8', TCODColor::gold);
            item->item_behavior = new ItemBehavior(item, 1, 1, true, 0);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 3, true);
            ItemPurpose *purpose = new ItemEffectConfusion(8);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Can be throw at any place in sight and confuse a creature for 8 round");
            break;
        }
        
        case item_dict::armor: {
            item = new Entity (x, y, "Armor", 'M', TCODColor::silver);
            item->item_behavior = new ItemEquipmentBehavior(item, 3, 1, false, equipment_index::torso);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemEquipment(0, 1);
            item->item_behavior->all_purpose.push(purpose);
            item->item_behavior->setDesc("Increase self defense by 1");
            break;
        }
    }
    return item;
}

