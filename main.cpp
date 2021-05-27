#include "main.hpp"

static const int CONSOLE_WIDTH = 130, CONSOLE_HEIGHT = 60;

GameManager game(CONSOLE_WIDTH, CONSOLE_HEIGHT);

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

void printTest(std::string file_name, int line) {
    std::cout << "FILE: " << file_name << " LINE: " << line << std::endl;
}