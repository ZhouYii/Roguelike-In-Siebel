#include "main.h"
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
Engine engine(WINDOW_WIDTH,WINDOW_HEIGHT);

int main() {
    engine.load();
    while( !TCODConsole::isWindowClosed() )
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    engine.save();

    return 0;
}
