#ifndef TRAP_HPP
#define TRAP_HPP

enum trap_dict {
    trap_blade = 1,
    trap_electric = 2,
    trap_burn = 3,
    trap_poison = 4,
    trap_frozen = 5,
};

Entity* getTrap(const int x, const int y, const int index);

#endif /* TRAP_HPP */

