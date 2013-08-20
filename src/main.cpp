#include "main.h"
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
Engine engine(WINDOW_WIDTH,WINDOW_HEIGHT);

int main() {

/*    TCODConsole::setCustomFont("dullard14x14_zpsb7e02c49.png",
    TCOD_FONT_LAYOUT_TCOD|TCOD_FONT_TYPE_GREYSCALE,30,36);

    TCODConsole::initRoot(80,50,"test",false);
    TCODConsole::mapAsciiCodeToFont(157,0,0); // Deciduous
    TCODConsole::mapAsciiCodeToFont(156,1,1); // Shrubland
    TCODConsole::mapAsciiCodeToFont(154,2,5); // Cacti
    TCODConsole::mapAsciiCodeToFont(153,3,5); // Heathland
    TCODConsole::mapAsciiCodeToFont(151,0,5); // Broadleaf
    TCODConsole::mapAsciiCodeToFont('@',27,6); // Mixed Forest
    TCODConsole::mapAsciiCodeToFont(149,0,2); // Coniferous
    TCODConsole::mapAsciiCodeToFont(148,6,5); // Evergreen
    TCODConsole::mapAsciiCodeToFont(147,7,5); // Caves
    TCODConsole::mapAsciiCodeToFont(146,8,5); // Tropical Forest    
    while ( !TCODConsole::isWindowClosed() ) {
        TCODConsole::root->clear();
        int x=10;
        TCODConsole::root->putChar(x++,10,'@');
        TCODConsole::flush();
        TCOD_key_t key;
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL,true);
    }
*/
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
