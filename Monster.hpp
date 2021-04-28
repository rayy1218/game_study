#ifndef MONSTER_HPP
#define MONSTER_HPP

class Entity;

enum monster_dict{
    goblin_archer = 0,
    goblin_skirmisher = 1,
    goblin_armored = 2,
    wolf = 3,
    gaint_wolf = 4,
};

Entity* getMonster(int x, int y, int index);

#endif /* MONSTER_HPP */

