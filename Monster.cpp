#include "main.hpp"

Entity* getMonster(int x, int y, int index) {
    Entity *monster;
    switch (index) {
        case monster_dict::goblin_archer: {
            monster = new Entity(x, y, "goblin", 'o', TCODColor::darkLime);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 20, 8, 0, 50);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             new CorpseLooting(monster), 
                                                             new CorpseBurn, 
                                                             nullptr); 
            
            monster->inventory = new Container(monster, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, 
                                                         item_dict::material_gold_chunk),
                                                         10, 20);
                    
            break;
        }

        case monster_dict::goblin_skirmisher: {
            monster = new Entity(x, y, "goblin skirmisher", 'o', TCODColor::darkerGreen);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 25, 10, 1, 50);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             new CorpseLooting(monster), 
                                                             new CorpseBurn, 
                                                             nullptr); 
            
            monster->inventory = new Container(monster, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, 
                                                         item_dict::material_gold_chunk),
                                                         10, 20);
            
            break;
        }

        case monster_dict::goblin_armored: {
            monster = new Entity(x, y, "armored goblin", 'o', 
                                 TCODColor::darkerGreen + TCODColor::grey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 25, 10, 4,30);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             new CorpseLooting(monster), 
                                                             new CorpseBurn, 
                                                             nullptr); 
            
            monster->inventory = new Container(monster, 20);
            monster->corpse_interact->addLootableItem(getItem(0, 0, 
                                                         item_dict::material_gold_chunk),
                                                         10, 20);
            
            break;
        }

        case monster_dict::wolf: {
            monster = new Entity(x, y, "wolf", 'G', TCODColor::lightestGrey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 20, 8, 0, 80);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1)); 
            
            
            break;
        }

        case monster_dict::gaint_wolf: {
            monster = new Entity(x, y, "gaint wolf", 'G', TCODColor::lighterGrey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 30, 15, 1, 70);
            monster->corpse_interact = new CorpseInteraction(monster,
                                                             new CorpseHarvest(monster), 
                                                             nullptr, 
                                                             new CorpseBurn, 
                                                             new CorpseRecall(1));
            
            break;
        }
    }
    
    return monster;
}

