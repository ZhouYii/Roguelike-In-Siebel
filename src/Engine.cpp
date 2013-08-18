#include "main.h"

extern const int PLAYER_INV_SIZE;
extern const int PLAYER_INIT_HP;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

Engine::Engine(int screen_width, int screen_height) : game_status(STARTUP), screen_width(screen_width), 
    screen_height(screen_height), fov_radius(10)
{
    TCODConsole::initRoot(80, 50, "A Rogue in Siebel", false);
    player = new Actor(40, 25, '@', "HeroProtagonist", TCODColor::white);
    player->destructible = new PlayerDestructible(PLAYER_INIT_HP, 2, "your cold, dead body");
    player->attacker = new Attacker(5);
    player->ai = new PlayerAi();
    player->container = new Container(PLAYER_INV_SIZE);
    actors.push(player);
    map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT);
    gui = new Gui();

    gui->log_message(TCODColor::red, "Welcome to Siebel Center");

}

Engine::~Engine()  
{
    actors.clearAndDelete();
    delete map;
    delete gui;
}

void Engine::update() 
{
    if(game_status == STARTUP)
        map->computeFov();

    game_status = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&last_key,&mouse);
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
    gui->render();
    // show the player's stats
    //TCODConsole::root->print(1,screen_height-2, "HP : %d/%d",
        //(int)player->destructible->hp,(int)player->destructible->max_hp);
    player->render();

}

void Engine::sendToBack(Actor * actor)
{
    if(actor)
    {
        actors.remove(actor);
        actors.insertBefore(actor, 0);
    }
}
