#include "main.h"

extern const int PLAYER_INV_SIZE;
extern const int PLAYER_INIT_HP;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int GUI_CONSOLE_HEIGHT;

Engine::Engine(int screen_width, int screen_height) : game_status(STARTUP), screen_width(screen_width),
                screen_height(screen_height), player(NULL),map(NULL), fov_radius(10), level(1) {
    TCODConsole::initRoot(WINDOW_WIDTH, WINDOW_HEIGHT, "A Rogue in Siebel", false);
    gui = new Gui();
}

void Engine::init() {
    player = new Actor(40, 25, '@', "HeroProtagonist", TCODColor::white);
    player->destructible = new PlayerDestructible(PLAYER_INIT_HP, 2, "your cold, dead body", 0);
    player->attacker = new Attacker(5);
    player->ai = new PlayerAi();
    player->container = new Container(PLAYER_INV_SIZE);
    actors.push(player);
    stairs = new Actor(0, 0, '>', "stairs", TCODColor::white);
    stairs->blocking = false;
    stairs->fov_only = false;
    actors.push(stairs);
    map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT-GUI_CONSOLE_HEIGHT);
    map->init(true);
    gui->log_message(TCODColor::red, "Welcome to Siebel Center");
    game_status = STARTUP;
}

Engine::~Engine()  
{
    terminate();
    delete gui;
}

void Engine::update() 
{
    if(game_status == STARTUP)
        map->computeFov();

    game_status = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&last_key,&mouse);
    if ( last_key.vk == TCODK_ESCAPE ) {
        save();
        load();
    }
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
        if ( (!actor->fov_only && map->isExplored(actor->x, actor->y)) || 
                map->inFov(actor->x,actor->y) ) 
        {
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

Actor *Engine::getClosestMonster(int x, int y, float range) const {
    Actor * closest = NULL;
    float best_distance = 1E6f;

    for(Actor **it = actors.begin(); it != actors.end(); it++)
    {
        Actor *actor = *it;
        if(actor != player && actor->destructible && !actor->destructible->isDead())
        {
            float distance = actor->getDistanceTo(x, y);
            if(distance < best_distance && (distance <= range || range == 0.0f))
            {
                best_distance = distance;
                closest = actor;
            }
        }
    }
    return closest;
}

bool Engine::pickATile(int *x, int *y, float max_range)
{
    while(!TCODConsole::isWindowClosed())
    {
        render();

        //Draw the targettable tiles
        for(int cx = 0 ; cx < map->width; cx++){
            for(int cy = 0; cy < map->height; cy++){
                if(map->inFov(cx, cy) && (max_range == 0 || player->getDistanceTo(cx, cy) <= max_range))
                {
                    TCODColor col = TCODConsole::root->getCharBackground(cx, cy);
                    col = col * 1.2f;
                    TCODConsole::root->setCharBackground(cx,cy,col);
                }
            }
        }
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&last_key,&mouse);

        if ( map->inFov(mouse.cx,mouse.cy) && 
                ( max_range == 0 || player->getDistanceTo(mouse.cx,mouse.cy) <= max_range )) {
            TCODConsole::root->setCharBackground(mouse.cx,mouse.cy,TCODColor::white);   
            if ( mouse.lbutton_pressed ) {
                *x=mouse.cx;
                *y=mouse.cy;
                return true;
            }
        }

        //Cancel with right click or keypress
        if (mouse.rbutton_pressed || last_key.vk != TCODK_NONE) {
            return false;
        }
        TCODConsole::flush();
    }

    return false;
}
        
    
Actor *Engine::getActorAt(int x, int y) const {
    for (Actor **iterator=actors.begin();
        iterator != actors.end(); iterator++) {
        Actor *actor=*iterator;
        if ( actor->x == x && actor->y ==y && actor->destructible
            && ! actor->destructible->isDead()) {
            return actor;
        }
    }
    return NULL;
}

void Engine::terminate() 
{
    actors.clearAndDelete();
    if(map) delete map;
    gui->clear();
}

void Engine::nextLevel()
{
    level++;
    gui->log_message(TCODColor::red, "You descend deeper into the heart of Siebel");
    delete map;
    for (Actor **it=actors.begin(); it!=actors.end(); it++) {
        if ( *it != player && *it != stairs ) {
            delete *it;
            it = actors.remove(it);
        }
    }
    map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT);
    map->init(true);
    game_status=STARTUP;
}
