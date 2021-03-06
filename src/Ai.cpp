#include <stdio.h>
#include <math.h>
#include "main.h"

extern const int TRACKING_TURNS;
extern const int INVENTORY_SCRN_WIDTH;
extern const int INVENTORY_SCRN_HEIGHT;
extern const int XP_COST_BASE;
extern const int XP_COST_MULTIPLIER;

PlayerAi::PlayerAi() : xp_level(1) {}

int PlayerAi::getNextLevelXp() {
    return XP_COST_BASE + xp_level*XP_COST_MULTIPLIER;
}

void PlayerAi::update(Actor *target) 
{
    int level_up_cost = getNextLevelXp();
    if(target->destructible->xp >= level_up_cost)
    {
        xp_level++;
        target->destructible->xp -= level_up_cost;
        engine.gui->log_message(TCODColor::lightGreen, 
                "Level up! You're level %d now!", xp_level);
        engine.gui->menu.clear();
        engine.gui->menu.addItem(Menu::CONSTITUTION,"Constitution (+20HP)");
        engine.gui->menu.addItem(Menu::STRENGTH,"Strength (+1 attack)");
        engine.gui->menu.addItem(Menu::AGILITY,"Agility (+1 defense)");
        Menu::MenuItem menu_item = engine.gui->menu.pick(Menu::PAUSE);

        switch (menu_item) {
            case Menu::CONSTITUTION :
                target->destructible->max_hp+=20;
                target->destructible->hp+=20;
                break;
            case Menu::STRENGTH :
                target->attacker->attack_points += 1;
                break;
            case Menu::AGILITY :
                target->destructible->defense += 1;
                break;
            default:break;
        }

    }
    //Did the player fuck up and get himself killed?
    if(target->destructible && target->destructible->isDead())
        return;

    int dx=0,dy=0;
    switch(engine.last_key.vk) {
    case TCODK_UP : dy=-1; break;
    case TCODK_DOWN : dy=1; break;
    case TCODK_LEFT : dx=-1; break;
    case TCODK_RIGHT : dx=1; break;
    case TCODK_CHAR : handleActionKey(target, engine.last_key.c); break;
        default:break;
    }
    if (dx != 0 || dy != 0) {
        engine.game_status=Engine::NEW_TURN;
        if (moveOrAttack(target, target->x+dx,target->y+dy)) 
            engine.map->computeFov();
    }
}

void PlayerAi::handleActionKey(Actor *player, int ascii)
{
    switch(ascii)
    {
        case '>' :
            if ( engine.stairs->x == player->x && engine.stairs->y == player->y ) {
                engine.nextLevel();
            } else {
                engine.gui->log_message(TCODColor::lightGrey,"There are no stairs here.");
            }
        break;
        case 'd':
            {
                Actor *actor = choseFromInventory(player);
                if(actor)
                {
                    actor->pickable->drop(actor, player);
                    //Dropping item costs a turn
                    engine.game_status = Engine::NEW_TURN;
                }
            }
            break;
        case 'g': //Pick up the item
            {
                bool found = false;
                for(Actor **it = engine.actors.begin(); it != engine.actors.end(); it++)
                {
                    Actor *actor = *it;
                    if(actor->pickable && actor->x == player->x && actor->y == player->y) 
                    {
                        if(actor->pickable->pick(actor, player))
                        {
                            found = true;
                            engine.gui->log_message(TCODColor::lightGrey, "You picked up a %s", actor->name);
                            break;
                        } else if(!found) {
                            //Pick up failed despite item being pick-able and
                            //player having an inventory means inventory is
                            //full.
                            found = true;
                            engine.gui->log_message(TCODColor::red, "Your inventory is full");
                        }
                    }
                }

                if(!found)
                    engine.gui->log_message(TCODColor::lightGrey, "There's nothing here to pick up");
                engine.game_status = Engine::NEW_TURN;
            }
            break;

        case 'i':
            {
                Actor *actor = choseFromInventory(player);
                if(actor)
                {
                    actor->pickable->use(actor, player);
                    engine.game_status = Engine::NEW_TURN;
                }
            }
            break;

        default: break;
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
        
        bool corpse_or_item = (actor->destructible && actor->destructible->isDead()) || actor->pickable;
        if(corpse_or_item && actor->x == x && actor->y == y) {
            engine.gui->log_message(TCODColor::lightGrey,"There's a %s here",actor->name);
        }
    }

    target->x = x;
    target->y = y;
    return true;
}

Actor *PlayerAi::choseFromInventory(Actor *player)
{
    static TCODConsole con(INVENTORY_SCRN_WIDTH,INVENTORY_SCRN_HEIGHT);

    //Put a frame
    con.setDefaultForeground(TCODColor(200,180,50));
    con.printFrame(0,0,INVENTORY_SCRN_WIDTH,INVENTORY_SCRN_HEIGHT,true, 
                TCOD_BKGND_DEFAULT,"inventory");
    con.setDefaultForeground(TCODColor::white);
    int shortcut = 'a';
    int y = 1;
    for(Actor **it = player->container->inventory.begin();
            it != player->container->inventory.end(); it++)
    {
        Actor *actor = *it;
        con.print(2, y, "(%c) %s", shortcut, actor->name);
        y++;
        shortcut++;
    }

    TCODConsole::blit(&con, 0,0,INVENTORY_SCRN_WIDTH, INVENTORY_SCRN_HEIGHT,
            TCODConsole::root, engine.screen_width/2 - INVENTORY_SCRN_WIDTH/2,
            engine.screen_height/2 - INVENTORY_SCRN_HEIGHT/2);;
    TCODConsole::flush();

    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

    if(key.vk == TCODK_CHAR)
    {
        int index = key.c - 'a';
        if(index >= 0 && index < player->container->inventory.size())
            return player->container->inventory.get(index);
    }
    return NULL;
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

ConfusedMonsterAi::ConfusedMonsterAi(int duration, Ai *old_ai)
    : duration(duration), old_ai(old_ai) {}

void ConfusedMonsterAi::update(Actor* target)
{
    TCODRandom *rng = TCODRandom::getInstance();
    int dx = rng->getInt(-1, 1);
    int dy = rng->getInt(-1, 1);
    if(dy != 0 || dx != 0)
    {
        int dest_x = target->x + dx;
        int dest_y = target->y + dy;
        if (engine.map->canWalk(dest_x, dest_y))
        {
            target->x = dest_x;
            target->y = dest_y;
        } else {
            //If something is on that cell, attack it 
            Actor *actor = engine.getActorAt(dest_x, dest_y);
            if(actor)
                target->attacker->attack(target, actor);
        }
    }
    
    duration--;
    if(duration <= 0)
    {
        target->ai = old_ai;
        delete this;
    }
}
