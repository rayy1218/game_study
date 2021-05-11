#ifndef MONSTER_HPP
#define MONSTER_HPP

class Entity;

static const int MONSTER_TYPE = 5;

enum monster_dict{
    goblin_archer       = 1,
    goblin_skirmisher   = 2,
    goblin_armored      = 3,
    wolf                = 4,
    gaint_wolf          = 5
};

Entity* getMonster(int x, int y, int index);

#endif /* MONSTER_HPP */

