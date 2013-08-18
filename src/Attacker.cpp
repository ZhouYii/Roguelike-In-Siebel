#include <stdio.h>
#include "main.h"

Attacker::Attacker(float power) : attack_points(power) {}

void Attacker::attack(Actor *attacker, Actor *target)
{
    if(target->destructible && !target->destructible->isDead() )
    {
        if(attack_points - target->destructible->defense > 0)
        {
            printf("%s attacks %s for %g hit points.\n", attacker->name, target->name,
                 attack_points - target->destructible->defense);
        } else {
            printf("%s attacks %s but is too weak!\n", attacker->name, target->name);
        }
    target->destructible->calcDamage(target, attack_points);

    } else {
        printf("%s attacks %s in vain.\n",attacker->name,target->name);
    }
}

