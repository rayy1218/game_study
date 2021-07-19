#ifndef DEBUGMODE_HPP
#define DEBUGMODE_HPP

enum debug_option {
    fog,
    spawn_monster,
    spawn_item
};

namespace Debug {
    void doRenderDebugMenu();
    void doSpawnMonster();
    void doSpawnItem();
    void toggleFog();
}

#endif //DEBUGMODE_HPP
