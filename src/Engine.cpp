#include "main.h"

Engine::Engine(int screen_width, int screen_height) : game_status(STARTUP), screen_width(screen_width), 
    screen_height(screen_height), fov_radius(10)
{
    TCODConsole::initRoot(80, 50, "A Rogue in Siebel", false);
    player = new Actor(40, 25, '@', "HeroProtagonist", TCODColor::white);
    player->destructible = new PlayerDestructible(30, 2, "your cold, dead body");
    player->attacker = new Attacker(5);
    player->ai = new PlayerAi();
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
    if(game_status == STARTUP)
        map->computeFov();

    game_status = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &last_key, NULL);
    player->update();

    //Check to see if player move was okay. If so, move all other Actors
    if( game_status == NEW_TURN ) 
    {
        for( Actor **iterator = actors.begin(); iterator != actors.end(); iterator++)
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

    // show the player's stats
    TCODConsole::root->print(1,screen_height-2, "HP : %d/%d",
        (int)player->destructible->hp,(int)player->destructible->max_hp);

}

void Engine::sendToBack(Actor * actor)
{
    if(actor)
    {
        actors.remove(actor);
        actors.insertBefore(actor, 0);
    }
}
