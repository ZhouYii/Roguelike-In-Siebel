#include "main.h"

bool Pickable::pick(Actor *item, Actor *owner)
{
    if(owner->container && owner->container->add(item))
    {
        engine.actors.remove(item);
        return true;
    }
    return false;
}

bool Pickable::use(Actor *item, Actor *owner)
{
    if(owner->container)
    {
        owner->container->remove(item);
        delete item;
        return true;
    }
    return false;
}


HealingEntity::HealingEntity(float amount) : amount(amount) {}

bool HealingEntity::use(Actor *item, Actor *owner)
{
    if(owner->destructible)
    {
        float heal_amt = owner->destructible->heal(amount);
        if(heal_amt > 0)
            return Pickable::use(item, owner);
    }
    return false;
}

