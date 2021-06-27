#include "main.hpp"

Entity* getMonster(int x, int y, int index) {
    Entity *monster;
    switch (index) {
        case monster_dict::goblin_skirmisher: {
            monster = new Entity(x, y, "goblin skirmisher", 'o', TCODColor::darkerGreen);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 25, 10, 1, 12, 12);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             new CorpseLooting(monster), 
                                                             new CorpseBurn, 
                                                             nullptr); 
            
            monster->inventory = new Container(monster, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, item_dict::material_copper_chunk), 10, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, item_dict::loot_goblin), 0, 1);
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 4, 6);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_hide), 1, 3);
            
            break;
        }

        case monster_dict::goblin_armored: {
            monster = new Entity(x, y, "armored goblin", 'o', 
                                 TCODColor::darkerGreen + TCODColor::grey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 25, 10, 4, 10, 7);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             new CorpseLooting(monster), 
                                                             new CorpseBurn, 
                                                             nullptr); 
            
            monster->inventory = new Container(monster, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, item_dict::material_copper_chunk), 10, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, item_dict::loot_goblin), 0, 1);
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 4, 6);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_hide), 1, 3);
            
            break;
        }

        case monster_dict::wolf: {
            monster = new Entity(x, y, "wolf", 'G', TCODColor::lightestGrey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 20, 8, 0, 9, 14);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_hide), 1, 3);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 3, 5);
            
            break;
        }
        
        case monster_dict::skeleton: {
            monster = new Entity(x, y, "fragile skeleton", 's', TCODColor::lightestGrey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 16, 12, 0, 7, 20);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 7, 10);
            
            break;
        }
        
        case monster_dict::zombie: {
            monster = new Entity(x, y, "rotting zombie", 'z', TCODColor::darkGreen);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 18, 6, 1, 10, 8);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 2, 3);
            
            break;
        }
        
        case monster_dict::zombie_high: {
            monster = new Entity(x, y, "rigid zombie", 'z', TCODColor::darkGreen);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 18, 6, 3, 10, 6);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 3, 5);
            
            break;
        }
        
        case monster_dict::slime: {
            monster = new Entity(x, y, "translucent slime", 's', TCODColor::lightLime);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 8, 3, 6, 12, 15);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             nullptr, 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            break;
        }
        
        case monster_dict::slime_acid: {
            monster = new Entity(x, y, "acidic slime", 's', TCODColor::lightPurple);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 8, 3, 6, 12, 15);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             nullptr, 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            break;
        }
        
        case monster_dict::slime_gaint: {
            monster = new Entity(x, y, "acidic slime", 'S', TCODColor::lightLime);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 12, 5, 6, 15, 13);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             nullptr, 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            break;
        }
        
        case monster_dict::minotaur: {
            monster = new Entity(x, y, "mighty minotaur", 'M', TCODColor::darkerSepia);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 22, 12, 3, 7, 6);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 5, 7);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_hide), 5, 8);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_meat), 8, 10);
            
            break;
        }
        
        case monster_dict::griffin: {
            monster = new Entity(x, y, "sharp eyed griffin", 'W', TCODColor::darkerSepia);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 18, 8, 2, 9, 12);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 3, 6);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_hide), 4, 6);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_meat), 3, 5);
            
            break;
        }
        
        case monster_dict::medusa: {
            monster = new Entity(x, y, "terror medusa", 'M', TCODColor::darkerSepia);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 15, 7, 1, 10, 9);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_bone), 1, 3);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_hide), 1, 3);
            monster->corpse_interact->addHasvestableItem(getItem(0, 0, item_dict::harvest_meat), 2, 4);
            
            break;
        }
        
        case monster_dict::imp: {
            monster = new Entity(x, y, "fiery imp", 'i', TCODColor::darkerRed);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 15, 7, 1, 10, 9);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             nullptr, 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            break;
        }
        
        case monster_dict::demon: {
            monster = new Entity(x, y, "demon servant", 'D', TCODColor::darkRed);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 15, 7, 1, 10, 9);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             nullptr, 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1));
            
            break;
        }
        
        case monster_dict::tentacle: {
            monster = new Entity(x, y, "flesh tentable", 't', TCODColor::darkRed);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 15, 7, 1, 10, 9);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             nullptr, 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            break;
        }
        
        case monster_dict::dragon_child: {
            
        }
        
        case monster_dict::dragon_adult: {
            
        }
        
        case monster_dict::dragon_boss: {
            
        }
        
        case monster_dict::one_eyed_infantry: {
            
        }
        
        case monster_dict::one_eyed_cavalry: {
            
        }
        
        case monster_dict::one_eyed_boss: {
            
        }
        
        case monster_dict::tyrant_blade: {
            
        }
        
        case monster_dict::tyrant_lance: {
            
        }
        
        case monster_dict::tyrant_boss: {
            
        }
        
        case monster_dict::elder_marble: {
            
        }
        
        case monster_dict::elder_tome: {
            
        }
        
        case monster_dict::elder_boss: {
            
        }
    }
    
    return monster;
}

