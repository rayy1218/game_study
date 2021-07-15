#include "main.hpp"

bool Casting::doCast() {
    if (!target_picking->doTargeting()) {return false;}

    for (Entity *target : target_picking->all_target) {
        for (Purpose *purpose : purpose_list) {
            purpose->doUse(target);
        }
    }

    return true;
}
