#include <stdio.h>
#include <math.h>
#include "main.h"

extern const int TRACKING_TURNS;
void PlayerAi::update(Actor *target) 
{
    //Did the player fuck up and get himself killed?
    if(target->destructible && target->destructible->isDead())
        return;

    int dx=0,dy=0;
    switch(engine.last_key.vk) {
    case TCODK_UP : dy=-1; break;
    case TCODK_DOWN : dy=1; break;
    case TCODK_LEFT : dx=-1; break;
    case TCODK_RIGHT : dx=1; break;
        default:break;
    }
    if (dx != 0 || dy != 0) {
        engine.game_status=Engine::NEW_TURN;
        if (moveOrAttack(target, target->x+dx,target->y+dy)) {
            engine.map->computeFov();
        }
    }
}

bool PlayerAi::moveOrAttack(Actor *target, int x, int y)
{
    if(engine.map->isWall(x, y))
        return false;

    for (Actor **iterator=engine.actors.begin();
        iterator != engine.actors.end(); iterator++) {
        Actor *actor = *iterator;
        if ( actor->destructible && !actor->destructible->isDead()
             && actor->x == x && actor->y == y ) {
            target->attacker->attack(target, actor);
            return false;
        }
    }

    for(Actor ** iterator=engine.actors.begin();
            iterator != engine.actors.end(); iterator++) {
        Actor *actor = *iterator;
        if(actor->destructible && actor->destructible->isDead() &&
                actor->x == x && actor->y == y) {
            engine.gui->log_message(TCODColor::lightGrey,"There's a %s here",actor->name);
        }
    }

    target->x = x;
    target->y = y;
    return true;
}

void MonsterAi::update(Actor *target) {
    //Is it dead?
    if(target->destructible && target->destructible->isDead())
        return;

    //If player can see the monster, then the monster can also see the player.
    //For now, no monsters are implemented with unique vision ranges.
    if(engine.map->inFov(target->x, target->y))
        move_count = TRACKING_TURNS;
    else
        move_count--;
    
    if(move_count > 0)
        moveOrAttack(target, engine.player->x, engine.player->y);
}

void MonsterAi::moveOrAttack(Actor *target, int x, int y)
{
    int dx = x - target->x;
    int dy = y - target->y;
    
    //wall sliding vectors for when monster is stuck on a wall.
    int step_x = (dx > 0 ? 1 : -1);
    int step_y = (dy > 0 ? 1 : -1);

    float distance=sqrtf(dx*dx+dy*dy);
    if(distance > 2) 
    {
        //Go from distance vector to normalized distance vector.
        //Takes care of threshold diagonal movement by making x,y movements bounded
        //by -1 and 1
        dx = (int)(round(dx/distance));
        dy = (int)(round(dy/distance));
        if(engine.map->canWalk(target->x + dx, target->y + dy))
        {
            target->x += dx;
            target->y += dy;
        } else if ( engine.map->canWalk(target->x+step_x,target->y) ) {
            target->x += step_x;
        } else if ( engine.map->canWalk(target->x,target->y+step_y) ) {
            target->y += step_y;
        }
    } else if (target->attacker && distance <= 1) {
        //wall hack are prevented by the attack class.
         target->attacker->attack(target, engine.player);
    }
}


