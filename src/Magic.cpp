#include "main.hpp"

Magic::Magic(int mana_point): max_mana_point(mana_point), mana_point(mana_point) {
    for (int i = 0; i < CASTING_NUM; i++) {
        is_casting_unlock[i] = false;
    }

    Casting *casting = new Casting;
    casting->name = "firebolt";
    casting->mana_cost = 20;
    casting->target_picking = new SelectAreaAllTarget(1, true);
    casting->purpose_list.push_back(new PurposeDamage(20));
    available_casting.push_back(casting);

    casting = new Casting;
    casting->name = "lighting chain";
    casting->mana_cost = 20;
    casting->target_picking = new SelectAreaAllTarget(3, true);
    casting->purpose_list.push_back(new PurposeDirectDamage(10));
    available_casting.push_back(casting);

    casting = new Casting;
    casting->name = "windblade";
    casting->mana_cost = 15;
    casting->target_picking = new SelectAreaAllTarget(0, true);
    casting->purpose_list.push_back(new PurposeDamage(15));
    available_casting.push_back(casting);
}

bool Magic::doRenderCastingList() {
    TCODConsole casting_console(40, 10);

    int to_cast = 0;

    while (true) {
        casting_console.setDefaultBackground(TCODColor::darkGrey);
        casting_console.setDefaultForeground(TCODColor::darkestGrey);
        casting_console.clear();

        casting_console.printFrame(0, 0, 40, 10, false, TCOD_BKGND_SET, "magic casting");
        int y = 0;
        casting_console.setDefaultBackground(TCODColor::darkerGrey);
        casting_console.rect(2, 2 + to_cast, 36, 1, false, TCOD_BKGND_SET);
        for (Casting *casting : available_casting) {
            if (!is_casting_unlock[y]) {casting_console.setDefaultForeground(TCODColor::grey);}
            casting_console.printf(2, 2 + y, "%s (%i)", casting->name.c_str(), casting->mana_cost);
            casting_console.setDefaultForeground(TCODColor::darkestGrey);
            y++;
        }

        TCODConsole::blit(&casting_console, 0, 0, 40, 10, TCODConsole::root, 0, 0);
        TCODConsole::flush();

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);

        if (game.keyboard.vk == TCODK_DOWN && to_cast + 1 < CASTING_NUM) {
            to_cast += 1;
        }

        if (game.keyboard.vk == TCODK_UP && to_cast != 0) {
            to_cast -= 1;
        }

        if (game.keyboard.vk == TCODK_ENTER) {break;}
        if (game.keyboard.vk == TCODK_ESCAPE) {return false;}
    }

    return doCastMagic(to_cast);
}

void Magic::setCastingUnlock(int casting, bool is_unlock) {
    is_casting_unlock[casting] = is_unlock;
}

bool Magic::doCastMagic(int casting_magic) {
    if (!is_casting_unlock[casting_magic]) {
        return false;
    }

    Casting *to_cast = available_casting[casting_magic];

    if (mana_point - to_cast->mana_cost < 0) {
        return false;
    }

    if (!to_cast->doCast()) {return false;}
    mana_point -= to_cast->mana_cost;

    return true;
}

int Magic::getMaxMp() {return max_mana_point;}
int Magic::getMp() {return mana_point;}
void Magic::setMaxMp(int input) {max_mana_point = input;}
void Magic::setMp(int input) {mana_point = input;}
