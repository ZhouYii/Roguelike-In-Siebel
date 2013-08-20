#include "libtcod.hpp"
#include "Persistent.h"
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
        bool visitNode(TCODBsp *node, void *user_data) {
            if(node->isLeaf()) {
                //Create a room at leaf
                int x, y, w, h;
                
                bool load_actors = (bool)user_data;
                w=map.rng->getInt(ROOM_MIN_SIZE, node->w-3);
                h=map.rng->getInt(ROOM_MIN_SIZE, node->h-3);
                x=map.rng->getInt(node->x+1, node->x+node->w-w-1);
                y=map.rng->getInt(node->y+1, node->y+node->h-h-1);
                while(x >= map.width)
                    x--;
                while(y >= map.height)
                    y--;
                map.createRoom( roomNum == 0, x, y, x+w-1, y+h-1, load_actors);
            
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

