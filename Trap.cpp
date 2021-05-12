#include "main.hpp"

Entity* getTrap(const int x, const int y, const int index) {
    Entity *trap;
    
    switch (index) {
        case trap_dict::trap_blade: {
            trap = new Entity(x, y, "Blade Trap", '^', TCODColor::orange);
            trap->control = new TrapControl(trap, new PurposeDamage(20));
            break;
        }
        
        case trap_dict::trap_burn: {
            trap = new Entity(x, y, "Burn Trap", '^', TCODColor::red);
            trap->control = new TrapControl(trap, new PurposeEffectBurn(5, 3));
            break;
        }
        
        case trap_dict::trap_electric: {
            trap = new Entity(x, y, "Electric Trap", '^', TCODColor::yellow);
            trap->control = new TrapControl(trap, new PurposeDirectDamage(15));
            break;
        }
        
        case trap_dict::trap_frozen: {
            trap = new Entity(x, y, "Frozen Trap", '^', TCODColor::cyan);
            trap->control = new TrapControl(trap, new PurposeEffectFrozen(10));
            break;
        }
        
        case trap_dict::trap_poison: {
            trap = new Entity(x, y, "Poison Trap", '^', TCODColor::green);
            trap->control = new TrapControl(trap, new PurposeEffectPoison(2, 8));
            break;
        }
    }
    
    return trap;
}