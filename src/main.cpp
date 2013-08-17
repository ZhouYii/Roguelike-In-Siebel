#include "libtcod.hpp"
#include "Actor.h"
int main() {
    int player_x = 40, player_y = 25;
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    while ( !TCODConsole::isWindowClosed() ) {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
        switch(key.vk) {
            case TCODK_UP : player_y--; break;
            case TCODK_DOWN : player_y++; break;
            case TCODK_LEFT : player_x--; break;
            case TCODK_RIGHT : player_x++; break;
            default:break;
        }
        TCODConsole::root->clear();
        TCODConsole::root->putChar(player_x,player_y,'@');
        TCODConsole::flush();
    }
    return 0;
}
