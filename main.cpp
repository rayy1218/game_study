#include <winbase.h>

#include "main.hpp"

GameManager game(130, 60);

void* operator new(size_t size) {
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    free(memory);
}

int main(void) {
    while (!TCODConsole::isWindowClosed()) {
        game.doUpdate();
        game.doRender();
        TCODConsole::root->flush();
    }
}