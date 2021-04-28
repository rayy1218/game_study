#include <winbase.h>

#include "main.hpp"

GameManager game(130, 60);

int main(void) {
    while (!TCODConsole::isWindowClosed()) {
        game.doUpdate();
        game.doRender();
        TCODConsole::root->flush();
    }
}