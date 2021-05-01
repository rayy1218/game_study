#ifndef ITEMS_HPP
#define ITEMS_HPP

class Entity;

enum item_dict{
    molotov = 0,
    throwing_axe = 1,
    incense = 2,
    potion_healing = 3,
    armor = 4
};

Entity* getItem(const int x, const int y, const int index);

#endif /* ITEMS_HPP */

