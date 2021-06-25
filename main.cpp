#include "main.hpp"

static const int CONSOLE_WIDTH = 130, CONSOLE_HEIGHT = 60;

GameManager game(CONSOLE_WIDTH, CONSOLE_HEIGHT);

void* operator new(size_t size) {
//    std::cout << "allocate: " <<  size << std::endl;
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
//    std::cout << "dellocate: " << size << std::endl;
    free(memory);
}

int main(void) {
    if (checkFileExist("save.txt")) {game.doLoad();}
    
    while (!TCODConsole::isWindowClosed()) {
        game.doUpdate();
        game.doRender();
        TCODConsole::root->flush();
    }
    
    doCloseWindow();
}

void doCloseWindow() {
    game.doSave();
    exit(0);
}

bool checkFileExist(std::string file_name) {
    std::ifstream file;
    file.open(file_name.c_str());
    bool exist = file.good();
    file.close();
    
    return exist;
}