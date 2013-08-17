#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"

Engine::Engine() : fovRadius(10), computeFov(true) {
    TCODConsole::initRoot(80, 50, "A Rogue in Siebel", false);
    player = new Actor(40, 25, '@', TCODColor::white);
    actors.push(player);
    map = new Map(80, 45);
}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
}

void Engine::update() {
    TCOD_key_t key;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

    //If player moves, recompute field of view
    if(key.vk == TCODK_UP || 
        key.vk == TCODK_DOWN || 
        key.vk == TCODK_LEFT || 
        key.vk == TCODK_RIGHT) {
        computeFov = true;
    }

    switch(key.vk) {
        case TCODK_UP : 
            if ( ! map->isWall(player->x,player->y-1)) {
                player->y--;   
            }
            break;
        case TCODK_DOWN : 
            if ( ! map->isWall(player->x,player->y+1)) {
                player->y++;
            }
        break;
        case TCODK_LEFT : 
            if ( ! map->isWall(player->x-1,player->y)) {
                player->x--;
            }
        break;
        case TCODK_RIGHT : 
            if ( ! map->isWall(player->x+1,player->y)) {
                player->x++;
            }
        break;
        default:break;
    }

    //Not all keypresses lead to new fov
    if(computeFov) {
        map->computeFov();
        computeFov = false;
    }
}

void Engine::render() {
    TCODConsole::root->clear();
    map->render();

    //Pseudo interator : the begin function gives a pointer to the first
    //element, therefore double pointer.
    for(Actor** iterator = actors.begin(); iterator != actors.end(); iterator++)
        (*iterator)->render();
}


