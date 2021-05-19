#include "main.hpp"

Town::Town() {
}

Town::~Town() {
}

void Town::doRenderTownConsole() {
    static TCODConsole town_console(100, 50);
    town_console.setDefaultBackground(TCODColor::darkGrey);
    town_console.setDefaultForeground(TCODColor::darkestGrey);
    town_console.clear();
    town_console.printFrame(0, 0, 100, 50, false, TCOD_BKGND_SET, "town");
    
    while (game.keyboard.vk != TCODK_ESCAPE) {
        game.gui->doRender();
        TCODConsole::blit(&town_console, 0, 0, 100, 50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, true);

        if (game.keyboard.vk == TCODK_ESCAPE) {return;}
    }
}

