#include <stdio.h>
#include "main.h"

Destructible::Destructible(float max_hp, float defense, const char *corpse_name, int xp) :
    max_hp(max_hp), hp(max_hp), defense(defense), xp(xp){
    if(corpse_name)
        this->corpse_name = strdup(corpse_name);
}

Destructible::~Destructible()
{
    free(corpse_name);
}

float Destructible::calcDamage(Actor *target, float damage) {
    damage -= defense;
    if(damage > 0) 
    {
        hp -= damage;
        if (hp <= 0)
            die(target);
    } else {
        damage = 0;
    }
    return damage;
}

void Destructible::die(Actor * target)
{
    target->ch = '%';
    target->col = TCODColor::darkRed;
    target->name = corpse_name;
    target->blocking = false;
    //Make sure the corses are rendered first, so nothing can hide under them.
    engine.sendToBack(target);
}


MonsterDestructible::MonsterDestructible(float max_hp, float defense, const char *corpse_name,
        int xp) : Destructible(max_hp,defense,corpse_name, xp) {
}
 
PlayerDestructible::PlayerDestructible(float max_hp, float defense, const char *corpse_name, int xp)
    : Destructible(max_hp,defense,corpse_name, xp) {
}

void MonsterDestructible::die(Actor *target) 
{
    engine.gui->log_message(TCODColor::lightGrey,"%s is dead. You gain %d xp",
        target->name, xp);
    engine.player->destructible->xp += xp;
    Destructible::die(target);
}

void PlayerDestructible::die(Actor *target) 
{
    engine.gui->log_message(TCODColor::red,"You failed to submit the assignment!");
    Destructible::die(target);
    engine.game_status = Engine::LOSE;
}

float Destructible::heal(float amount) 
{
    hp += amount;
    if(hp > max_hp)
    {
        amount -= hp - max_hp;
        hp = max_hp;
    }
    return amount;
}

