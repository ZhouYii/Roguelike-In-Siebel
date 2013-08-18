#include <stdio.h>
#include "main.h"

Attacker::Attacker(float power) : attack_points(power) {}

void Attacker::attack(Actor *attacker, Actor *target)
{
    if(target->destructible && !target->destructible->isDead() )
    {
        if(attack_points - target->destructible->defense > 0)
        {
            engine.gui->log_message(attacker==engine.player ? TCODColor::red : TCODColor::lightGrey,
                "%s attacks %s for %g hit points.", attacker->name, target->name,
                attack_points - target->destructible->defense);
        } else {
            engine.gui->log_message(TCODColor::lightGrey,
                "%s attacks %s but it has no effect!", attacker->name, target->name);
        }
    target->destructible->calcDamage(target, attack_points);

    } else {
        engine.gui->log_message(TCODColor::lightGrey,
            "%s attacks %s in vain.",attacker->name,target->name);
    }
}

