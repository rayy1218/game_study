#include "main.hpp"

Entity* getItem(const int x, const int y, const int index) {
    Entity* item;
    
    switch (index) {
        case item_dict::potion_healing: {
            item = new Entity(x, y, "Potion of Healing", '|', TCODColor::darkerCrimson);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, 8);
            item->item_behavior->targeting = new SelfTarget(item);
            ItemPurpose *purpose = new ItemHeal(20);
            item->item_behavior->all_purpose.push(purpose);
            break;
        }
        
        case item_dict::throwing_axe: {
            item = new Entity(x, y, "Throwing Axe", 'P', TCODColor::darkSepia);
            item->item_behavior = new ItemBehavior(item, 1, 1, 8);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 1, false);
            ItemPurpose *purpose = new ItemDamage(15);
            item->item_behavior->all_purpose.push(purpose);
            break;
        }
        
        case item_dict::molotov: {
            item = new Entity(x, y, "Molotov", 'd', TCODColor::darkRed);
            item->item_behavior = new ItemBehavior(item, 0.5, 1, 8);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 3, true);
            ItemPurpose *purpose = new ItemDamage(5);
            item->item_behavior->all_purpose.push(purpose);
            purpose = new ItemEffectBurn(6, 4);
            item->item_behavior->all_purpose.push(purpose);
            break;
        }
        
        case item_dict::incense: {
            item = new Entity(x, y, "Incense", '8', TCODColor::gold);
            item->item_behavior = new ItemBehavior(item, 1, 1, 8);
            item->item_behavior->targeting = new SelectAreaAllTarget(item, 3, true);
            ItemPurpose *purpose = new ItemEffectConfusion(8);
            item->item_behavior->all_purpose.push(purpose);
            break;
        }
    }
    return item;
}

