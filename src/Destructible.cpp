#include <stdio.h>
#include "main.h"

Destructible::Destructible(float max_hp, float defense, const char *corpse_name) :
    max_hp(max_hp), hp(max_hp), defense(defense), corpse_name(corpse_name) {}

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


MonsterDestructible::MonsterDestructible(float max_hp, float defense, const char *corpse_name)
    : Destructible(max_hp,defense,corpse_name) {
}
 
PlayerDestructible::PlayerDestructible(float max_hp, float defense, const char *corpse_name)
    : Destructible(max_hp,defense,corpse_name) {
}

void MonsterDestructible::die(Actor *target) 
{
    engine.gui->log_message(TCODColor::lightGrey,"%s is dead",target->name);
    Destructible::die(target);
}

void PlayerDestructible::die(Actor *target) 
{
    engine.gui->log_message(TCODColor::red,"You failed to submit the assignment!");
    Destructible::die(target);
    engine.game_status = Engine::LOSE;
}

