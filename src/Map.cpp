#include "main.h"
#include <assert.h>
#include <stdio.h>

#include "BspListener.cpp"

//Used for the BSP tree to partition rooms
extern const int ROOM_MAX_SIZE;
extern const int ROOM_MIN_SIZE;
extern const int MAX_ROOM_MONSTERS;
extern const int MOX_ROOM_ITEMS;

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
    seed = TCODRandom::getInstance()->getInt(0,0x7FFFFFFF);
}

Map::~Map(){
    if(tiles)
        delete[] tiles;

    if(map)
        delete map;
}

void Map::init(bool load_actors)
{
    rng = new TCODRandom(seed, TCOD_RNG_CMWC);
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);

    //Tree structure
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(rng, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, (void*) load_actors);
}

void Map::addMonster(int x, int y) {
    TCODRandom *rng = TCODRandom::getInstance();

    //Randomly decide to generate an orc or troll
    if( rng->getInt(0, 100) < 80)
    {
        Actor * orc = new Actor(x, y, 'O', "Orc", TCODColor::desaturatedGreen);
        orc->destructible = new MonsterDestructible(10, 0, "dead orc");
        orc->attacker = new Attacker(3);
        orc->ai = new MonsterAi();
        engine.actors.push(orc);
    }
    else
    {
        Actor * troll = new Actor(x, y, 'T', "Troll", TCODColor::darkerGreen);
        troll->destructible = new MonsterDestructible(16, 1, "troll carcass");
        troll->attacker = new Attacker(4);
        troll->ai = new MonsterAi();
        engine.actors.push(troll);
    }

    if(engine.guest_roster.size() > 0)
    {
        Actor * guest = engine.guest_roster.pop();
        engine.actors.push(guest);
        engine.gui->log_message(TCODColor::red, "A new challenger as arrived.");
    }
}

void Map::addItem(int x, int y)
{
    TCODRandom *rng=TCODRandom::getInstance();
    int dice = rng->getInt(0,100);
    if ( dice < 70 ) {
        // create a health potion
        Actor *healthPotion = new Actor(x,y,'!',"health potion",
            TCODColor::violet);
        healthPotion->blocking = false;
        healthPotion->pickable = new HealingEntity(4);
        engine.actors.push(healthPotion);
    } else if ( dice < 70+10 ) {
        // create a scroll of lightning bolt 
        Actor *scrollOfLightningBolt = new Actor(x,y,'#',"scroll of lightning bolt",
            TCODColor::lightYellow);
        scrollOfLightningBolt->blocking = false;
        scrollOfLightningBolt->pickable = new LightningBolt(5,20);
        engine.actors.push(scrollOfLightningBolt);
    } else if ( dice < 70+10+10 ) {
        // create a scroll of fireball
        Actor *scrollOfFireball = new Actor(x,y,'#',"scroll of fireball",
            TCODColor::lightYellow);
        scrollOfFireball->blocking = false;
        scrollOfFireball->pickable = new Fireball(3,12);
        engine.actors.push(scrollOfFireball);
    } else {
        // create a scroll of confusion
        Actor *scrollOfConfusion = new Actor(x,y,'#',"scroll of confusion",
            TCODColor::lightYellow);
        scrollOfConfusion->blocking = false;
        scrollOfConfusion->pickable = new Confuse(10,8);
        engine.actors.push(scrollOfConfusion);
    }
}



bool Map::isWall(int x, int y) const {
    return !map->isWalkable(x,y);
}

bool Map::canWalk(int x, int y) const {
    if(isWall(x, y))
        return false;

    if(x < 0 || x >= width-1 || y < 0 || y > height-1)
        return false;
    
    for(Actor **iterator = engine.actors.begin(); 
            iterator != engine.actors.end(); iterator++) { 

        Actor *actor = *iterator;
        if(actor->blocking && (actor->x == x) && (actor->y == y))
            return false;
    }

    return true;
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

    x1 = x1 >= (width - 1) ? (width-2) : x1;
    y1 = y1 >= (height - 1) ? (height-2) : y1;
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

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool load_actors) {
    dig(x1, y1, x2, y2);
    //Not loading a prior game
    if(!load_actors)
        return;

    int avg_x = (x1 + x2) / 2;
    int avg_y = (y1 + y2) / 2;

    if(first) {

        engine.player->x = avg_x;
        engine.player->y = avg_y;

    } else {

        TCODRandom *rng = TCODRandom::getInstance();

        //Generate monsters randomly
        int num_monsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while(num_monsters > 0)
        {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if(canWalk(x, y))
                addMonster(x, y);
            num_monsters--;
        }

        //Generate items randomly
        int num_items = rng->getInt(0, MAX_ROOM_ITEMS);
        while(num_items > 0)
        {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if(canWalk(x, y))
                addItem(x, y);
            num_items--;
        }
    }

    engine.stairs->x = (x1 + x2)/2;
    engine.stairs->y = (y1 + y2)/2;
}

bool Map::inFov(int x, int y) const {
    if ( x < 0 || x >= width || y < 0 || y >= height ) 
        return false;

    if( map->isInFov(x, y) )
    {
        //When a cell is see-able, then mark it as explored.
        tiles[y*width + x].explored = true;
        return true;
    }
    return false;
}

void Map::computeFov() {
    map->computeFov(engine.player->x, engine.player->y, engine.fov_radius);
}


