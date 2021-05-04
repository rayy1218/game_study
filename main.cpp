#include <winbase.h>

#include "main.hpp"

static const int CONSOLE_WIDTH = 130, CONSOLE_HEIGHT = 60;

GameManager game(CONSOLE_WIDTH, CONSOLE_HEIGHT);

void* operator new(size_t size) {
    std::cout << "Acollate " << size << "bytes" << std::endl;
    
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    std::cout << "Decollate " << size << "bytes" << std::endl;
    free(memory);
}

int main(void) {
    while (!TCODConsole::isWindowClosed()) {
        game.doUpdate();
        game.doRender();
        TCODConsole::root->flush();
    }
}