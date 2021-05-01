#include "main.hpp"

Entity* getMonster(int x, int y, int index) {
    Entity *monster;
    switch (index) {
        case monster_dict::goblin_archer: {
            monster = new Entity(x, y, "Goblin Archer", 'o', TCODColor::darkLime);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 20, 4, 0);
            break;
        }

        case monster_dict::goblin_skirmisher: {
            monster = new Entity(x, y, "Goblin Skirmisher", 'o', TCODColor::darkerGreen);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 25, 6, 1);
            break;
        }

        case monster_dict::goblin_armored: {
            monster = new Entity(x, y, "Armored Goblin", 'o', 
                                 TCODColor::darkerGreen + TCODColor::grey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 25, 4, 4);
            break;
        }

        case monster_dict::wolf: {
            monster = new Entity(x, y, "Wolf", 'G', TCODColor::lightestGrey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 20, 5, 1);
            break;
        }

        case monster_dict::gaint_wolf: {
            monster = new Entity(x, y, "Gaint Wolf", 'G', TCODColor::lighterGrey);
            monster->control = new EnemyControl(monster);
            monster->move_behavior = new MoveBehavior(monster);
            monster->combat_behavior = new EnemyCombatBehavior(monster, 30, 8, 3);
            break;
        }
    }
    
    return monster;
}

