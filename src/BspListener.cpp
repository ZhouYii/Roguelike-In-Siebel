#include "libtcod.hpp"
#include "Map.h"

extern const int ROOM_MAX_SIZE;
extern const int ROOM_MIN_SIZE;

class BspListener : public ITCODBspCallback {
    private:
        Map &map;
        int roomNum;
        int last_x, last_y; //center of last digged room

    public:
        BspListener(Map &map) : map(map), roomNum(0) {}
        bool visitNode(TCODBsp *node, void *userData) {
            if(node->isLeaf()) {
                //Create a room at leaf
                int x, y, w, h;
                TCODRandom *rng = TCODRandom::getInstance();
                
                //-2 guarantees no joined rooms and walls such that player cant
                //walk out of bounds and segfault the program.
                w=rng->getInt(ROOM_MIN_SIZE, node->w-3);
                h=rng->getInt(ROOM_MIN_SIZE, node->h-3);
                x=rng->getInt(node->x+1, node->x+node->w-w-1);
                y=rng->getInt(node->y+1, node->y+node->h-h-1);
                map.createRoom( roomNum == 0, x, y, x+w+1, y+h-1 );
            
                //TODO: RoomNum can be replaced with a bool
                
                //Connect rooms
                if(roomNum != 0)
                {
                    map.dig(last_x, last_y, x+w/2, last_y);
                    map.dig(x+w/2, last_y, x+w/2, y+h/2);
                }

                last_x = x+w/2;
                last_y = y+h/2;
                roomNum++;
            }

            return true;
        }
};

