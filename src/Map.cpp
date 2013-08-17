#include "libtcod.hpp"
#include "Map.h"
#include "Actor.h"
#include "Engine.h"

#include "BspListener.cpp"

//Used for the BSP tree to partition rooms
const int ROOM_MAX_SIZE = 12;
const int ROOM_MIN_SIZE = 6;

/*
 * <-----------Width--------->
 * ^
 * |
 * |
 * Height    (x,y) -> x + y*width as index
 * |
 * |
 * v
 */

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);

    //Tree structure
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map(){
    if(tiles)
        delete[] tiles;

    if(map)
        delete map;
}

bool Map::isWall(int x, int y) const {
    return !map->isWalkable(x,y);
}

bool Map::isExplored(int x, int y) const {
    return tiles[width*y + x].explored;
}

void Map::render() const {
    //Static allocates these two objects to cut some overhead from function
    //calls since the maps get rendered every event loop.
    static const TCODColor DARK_WALL(0,0,100);
    static const TCODColor DARK_GROUND(50,50,150);
    static const TCODColor LIGHT_WALL(130,110,50);
    static const TCODColor LIGHT_GROUND(200,180,50);

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
            if(inFov(x,y))
                TCODConsole::root->setCharBackground(x, y, 
                        isWall(x,y) ? LIGHT_WALL : LIGHT_GROUND);
            else if(isExplored(x,y))
                TCODConsole::root->setCharBackground(x, y,
                        isWall(x,y) ? DARK_WALL : DARK_GROUND);
}

void Map::checkCoordinateBounds(int &x1, int &y1)
{
    x1 = x1 < 0 ? 0 : x1;
    y1 = y1 < 0 ? 0 : y1;

    x1 = x1 >= width ? (width-1) : x1;
    y1 = y1 >= height ? (height-1) : y1;
}

void Map::verifyRoomCoords(int &x1, int &y1, int &x2, int &y2)
{
    if(x2 < x1) {
        int tmp = x2;
        x2 = x1;
        x1 = tmp;
    }

    if(y2 < y1) {
        int tmp = y2;
        y2 = y1;
        y1 = tmp;
    }

    checkCoordinateBounds(x1, y1);
    checkCoordinateBounds(x2, y2);
}


void Map::dig(int x1, int y1, int x2, int y2) {
    verifyRoomCoords(x1, y1, x2, y2);
    for(int tile_x = x1; tile_x <= x2; tile_x++)
        for(int tile_y = y1; tile_y <= y2; tile_y++)
            map->setProperties(tile_x, tile_y, true, true);

}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
    dig(x1, y1, x2, y2);
    int avg_x = (x1 + x2) / 2;
    int avg_y = (y1 + y2) / 2;
    if(first) {
        engine.player->x = avg_x;
        engine.player->y = avg_y;
    } else {
        TCODRandom *rng = TCODRandom::getInstance();

        //Right now I'm neglecting any entity generation. Just put some NPCs
        //randomly.
        if(rng->getInt(0,3) == 0)
            engine.actors.push(new Actor(avg_x, avg_y, 'N', TCODColor::yellow));
    }
}

bool Map::inFov(int x, int y) const {
    if( map->isInFov(x, y) )
    {
        //When a cell is see-able, then mark it as explored.
        tiles[y*width + x].explored = true;
        return true;
    }
    return false;
}

void Map::computeFov() {
    map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}


