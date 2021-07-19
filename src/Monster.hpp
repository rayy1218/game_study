#ifndef MONSTER_HPP
#define MONSTER_HPP

class Entity;

static const int MONSTER_TYPE = 15;
const int BOSS_MONSTER_TYPE = 9;

enum monster_dict{
    null,
    goblin_skirmisher   = 1,
    goblin_armored      = 2,
    wolf                = 3,
    skeleton            = 4,
    zombie              = 5,
    zombie_high         = 6,
    slime               = 7,
    slime_acid          = 8,
    slime_gaint         = 9,
    minotaur            = 10,
    medusa              = 11,
    griffin             = 12,
    imp                 = 13,
    demon               = 14,
    tentacle            = 15,
    dragon_child,
    dragon_adult,
    dragon_boss,
    one_eyed_infantry,
    one_eyed_cavalry,
    one_eyed_boss,
    tyrant_blade,
    tyrant_lance,
    tyrant_boss,
};

Entity* getMonster(int x, int y, int index);

#endif /* MONSTER_HPP */

