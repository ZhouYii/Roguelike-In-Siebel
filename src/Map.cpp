#include "libtcod.hpp"
#include "Map.h"

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

    //Arbitrary walls.
    setWall(30,22);
    setWall(50,22);
}

Map::~Map(){
    if(tiles)
        delete[] tiles;
}

bool Map::isWall(int x, int y) const {
    return !tiles[y*width + x].can_walk;
}

void Map::setWall(int x, int y) {
    tiles[y*width + x].can_walk = false;
}

void Map::render() const {

    //Static allocates these two objects to cut some overhead from function
    //calls since the maps get rendered every event loop.
    static const TCODColor kDarkWall(0,0,100);
    static const TCODColor kDarkGround(50,50,150);

    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++)
            TCODConsole::root->setCharBackground(i, j, 
                    isWall(i,j) ? kDarkWall : kDarkGround);
}
