#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"

Engine::Engine() : game_status(STARTUP), fov_radius(10)
{
    TCODConsole::initRoot(80, 50, "A Rogue in Siebel", false);
    player = new Actor(40, 25, '@', "HeroProtagonist", TCODColor::white);
    actors.push(player);
    map = new Map(80, 45);
}

Engine::~Engine()  
{
    actors.clearAndDelete();
    delete map;
}

void Engine::update() 
{
    TCOD_key_t key;

    if(game_status == STARTUP)
        map->computeFov();
    game_status = IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    int dx=0, dy=0;

    switch(key.vk) 
    {
        case TCODK_UP : dy=-1; break;
        case TCODK_DOWN : dy=1; break;
        case TCODK_LEFT : dx=-1; break;
        case TCODK_RIGHT : dx=1; break;
        default:break;
    }

    if( dx != 0 || dy != 0)
    {
        game_status = NEW_TURN;
        if( player->moveOrAttack( player->x + dx, player->y + dy) )
            map->computeFov();
    }

    //Check to see if player move was okay. If so, move all other Actors
    if( game_status == NEW_TURN ) {
        for( Actor **iterator = engine.actors.begin();
                iterator != engine.actors.end(); iterator++)
        {
            Actor *actor = *iterator;
            if( actor != player )
                actor->update();
        }
    }
}

void Engine::render() 
{
    TCODConsole::root->clear();
    map->render();

    //Pseudo interator : the begin function gives a pointer to the first
    //element, therefore double pointer.
    // draw the actors
    for (Actor **iterator=actors.begin(); iterator != actors.end();
        iterator++) {
        Actor *actor=*iterator;
        if ( map->inFov(actor->x,actor->y) ) {
            actor->render();
        }
    }
}


