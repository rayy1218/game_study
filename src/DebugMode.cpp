#include "main.hpp"

void Debug::toggleFog() {
    game.debug_mode.show_fog = !game.debug_mode.show_fog;
}

void Debug::doRenderDebugMenu() {
    TCODConsole debug_console(30, 6);
    int option_pointing = 0;
    const int OPTION_AVAILABLE = 3;

    while (true) {
        debug_console.setDefaultBackground(TCODColor::darkGrey);
        debug_console.setDefaultForeground(TCODColor::darkestGrey);
        debug_console.clear();

        debug_console.printFrame(0, 0, 30, 6, false, TCOD_BKGND_SET, "debug");

        debug_console.setDefaultBackground(TCODColor::darkerGrey);
        debug_console.rect(2, 2 + option_pointing, 26, 1, true, TCOD_BKGND_SET);

        debug_console.printf(2, 2, "fog - %s", (game.debug_mode.show_fog)? "true" : "false");
        debug_console.printf(2, 3, "spawn a monster");
        debug_console.printf(2, 4, "spawn a item");

        TCODConsole::blit(&debug_console, 0 , 0, 30, 6, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, nullptr, false);

        if (game.keyboard.vk == TCODK_UP && option_pointing != 0) {
            option_pointing -= 1;
        }
        if (game.keyboard.vk == TCODK_DOWN && option_pointing + 1 < OPTION_AVAILABLE) {
            option_pointing += 1;
        }
        if (game.keyboard.vk == TCODK_ESCAPE) {return;}
        if (game.keyboard.vk == TCODK_ENTER) {break;}
    }

    switch (option_pointing) {
        case debug_option::fog: Debug::toggleFog(); break;
        case debug_option::spawn_monster: Debug::doSpawnMonster(); break;
        case debug_option::spawn_item: Debug::doSpawnItem(); break;
    }
}

void Debug::doSpawnMonster() {
    TCODConsole monster_list_console(30, 50);
    int option_pointing = 0;
    const int AVAILABLE_OPTION = MONSTER_TYPE + BOSS_MONSTER_TYPE;

    while (true) {
        int y = 0;

        monster_list_console.setDefaultBackground(TCODColor::darkGrey);
        monster_list_console.setDefaultForeground(TCODColor::darkestGrey);
        monster_list_console.clear();

        monster_list_console.printFrame(0, 0, 30, 50, false, TCOD_BKGND_SET, "spawn monster");

        monster_list_console.setDefaultBackground(TCODColor::darkerGrey);
        monster_list_console.rect(2, 2 + option_pointing, 26, 1, false, TCOD_BKGND_SET);

        for (int i = 1; i <= AVAILABLE_OPTION; i++) {
            Entity *monster = getMonster(0, 0, i);
            monster_list_console.printf(2, 2 + y, "%s", monster->getName().c_str());
            y++;

            delete monster;
        }

        TCODConsole::blit(&monster_list_console, 0 , 0, 30, 50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, nullptr, false);

        if (game.keyboard.vk == TCODK_UP && option_pointing != 0) {
            option_pointing -= 1;
        }
        if (game.keyboard.vk == TCODK_DOWN && option_pointing + 1 < AVAILABLE_OPTION) {
            option_pointing += 1;
        }

        if (game.keyboard.vk == TCODK_ENTER) {break;}
    }

    game.gui->addMessage(TCODColor::yellow, "[NUMPAD] to select place to spawn monster");
    game.doRender();
    TCODConsole::flush();

    Entity *monster = getMonster(game.player->getX(), game.player->getY(), option_pointing + 1);
    int x = monster->getX(), y = monster->getY();
    while (true) {
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, nullptr, false);

        switch (game.keyboard.vk) {
            case TCODK_KP8:
                y = (monster->getY() - 1);
                break;

            case TCODK_KP6:
                x = (monster->getX() + 1);
                break;

            case TCODK_KP2:
                y = (monster->getY() + 1);
                break;

            case TCODK_KP4:
                x = (monster->getX() - 1);
                break;

            case TCODK_KP9:
                x = (monster->getX() + 1);
                y = (monster->getY() - 1);
                break;

            case TCODK_KP3:
                x = (monster->getX() + 1);
                y = (monster->getY() + 1);
                break;

            case TCODK_KP1:
                x = (monster->getX() - 1);
                y = (monster->getY() + 1);
                break;

            case TCODK_KP7:
                x = (monster->getX() - 1);
                y = (monster->getY() - 1);
                break;

        }

        if (game.map->canWalk(x, y)) {
            monster->setX(x);
            monster->setY(y);
        }
        else {
            x = game.player->getX();
            y = game.player->getY();
            continue;
        }

        if (game.keyboard.vk == TCODK_KP1 || game.keyboard.vk == TCODK_KP2 || game.keyboard.vk == TCODK_KP3 ||
            game.keyboard.vk == TCODK_KP4 || game.keyboard.vk == TCODK_KP6 || game.keyboard.vk == TCODK_KP7 ||
            game.keyboard.vk == TCODK_KP8 || game.keyboard.vk == TCODK_KP9) {

            monster->setX(x);
            monster->setY(y);
            game.all_character.push(monster);
            return;
        }

        if (game.keyboard.vk == TCODK_ESCAPE) {
            delete monster;
            return;
        }
    }
}

void Debug::doSpawnItem() {
    TCODConsole item_list_console(40, 50);
    int option_pointing = 0;
    const int AVAILABLE_OPTION = ITEM_TYPE;

    while (true) {
        int y = 0;

        item_list_console.setDefaultBackground(TCODColor::darkGrey);
        item_list_console.setDefaultForeground(TCODColor::darkestGrey);
        item_list_console.clear();

        item_list_console.printFrame(0, 0, 40, 50, false, TCOD_BKGND_SET, "spawn item");

        item_list_console.setDefaultBackground(TCODColor::darkerGrey);
        item_list_console.rect(2, 2 + option_pointing, 36, 1, false, TCOD_BKGND_SET);

        for (int i = 1; i <= AVAILABLE_OPTION; i++) {
            Entity *item = getItem(0, 0, i);
            item_list_console.printf(2, 2 + y, "%s", item->getName().c_str());
            y++;

            delete item;
        }

        TCODConsole::blit(&item_list_console, 0 , 0, 40, 50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, nullptr, false);

        if (game.keyboard.vk == TCODK_UP && option_pointing != 0) {
            option_pointing -= 1;
        }
        if (game.keyboard.vk == TCODK_DOWN && option_pointing + 1 < AVAILABLE_OPTION) {
            option_pointing += 1;
        }

        if (game.keyboard.vk == TCODK_ENTER) {break;}

        if (game.keyboard.vk == TCODK_ESCAPE) {return;}
    }

    game.gui->addMessage(TCODColor::yellow, "[NUMPAD] to select place to spawn item [ENTER] to spawn in inventory]");
    game.doRender();
    TCODConsole::flush();

    Entity *item = getItem(game.player->getX(), game.player->getY(), option_pointing + 1);
    int x = item->getX(), y = item->getY();
    while (true) {
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, nullptr, false);
        switch (game.keyboard.vk) {
            case TCODK_KP8:
                y = (item->getY() - 1);
                break;

            case TCODK_KP6:
                x = (item->getX() + 1);
                break;

            case TCODK_KP2:
                y = (item->getY() + 1);
                break;

            case TCODK_KP4:
                x = (item->getX() - 1);
                break;

            case TCODK_KP9:
                x = (item->getX() + 1);
                y = (item->getY() - 1);
                break;

            case TCODK_KP3:
                x = (item->getX() + 1);
                y = (item->getY() + 1);
                break;

            case TCODK_KP1:
                x = (item->getX() - 1);
                y = (item->getY() + 1);
                break;

            case TCODK_KP7:
                x = (item->getX() - 1);
                y = (item->getY() - 1);
                break;

        }

        if (game.keyboard.vk == TCODK_ENTER) {
            game.player->inventory->addItem(item);
            return;
        }

        if (game.keyboard.vk == TCODK_ESCAPE) {
            delete item;
            return;
        }

        if (game.map->canWalk(x, y)) {
            item->setX(x);
            item->setY(y);
        }
        else {
            x = game.player->getX();
            y = game.player->getY();
            continue;
        }

        if (game.keyboard.vk == TCODK_KP1 || game.keyboard.vk == TCODK_KP2 || game.keyboard.vk == TCODK_KP3 ||
            game.keyboard.vk == TCODK_KP4 || game.keyboard.vk == TCODK_KP6 || game.keyboard.vk == TCODK_KP7 ||
            game.keyboard.vk == TCODK_KP8 || game.keyboard.vk == TCODK_KP9) {

            game.all_item.push(item);
            return;
        }
    }
}

