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

LightningBolt::LightningBolt(float range, float damage) : range(range), damage(damage) {}

bool LightningBolt::use(Actor *item, Actor *user)
{
    Actor *closest_target = engine.getClosestMonster(user->x, user->y, range);
    if(!closest_target) {
        engine.gui->log_message(TCODColor::lightGrey, "No enemy is in range!");
        return false;
    }
    
    engine.gui->log_message(TCODColor::lightBlue, "A lighting bolt strikes the %s with a loud thunder!\n"
    "It deals %g points of damage.", closest_target->name,damage);

    closest_target->destructible->calcDamage(closest_target,damage);
    return Pickable::use(item, user);
}

Fireball::Fireball(float range, float damage) : LightningBolt(range, damage) {}

bool Fireball::use(Actor *item, Actor *user)
{
    engine.gui->log_message(TCODColor::cyan, "Left-click a square to blast it!\n Right-click to cancel. Party pooper.");
    int x, y;
    if(!engine.pickATile(&x, &y))
        return false;

    //attack everything in range
    engine.gui->log_message(TCODColor::orange,"The fireball explodes, burning everything within %g tiles!",range);
    for(Actor **it = engine.actors.begin(); it != engine.actors.end(); it++)
    {
        Actor *actor = *it;
        if(actor->destructible && !actor->destructible->isDead() && actor->getDistanceTo(x,y) < range)
        {
            engine.gui->log_message(TCODColor::orange,"The %s gets burned for %g hit points.", actor->name,damage);
            actor->destructible->calcDamage(actor,damage);
        }
    }

    return Pickable::use(item, user);
}
            
Confuse::Confuse(int duration, float range) : duration(duration), range(range) {}

bool Confuse::use(Actor* item, Actor *user)
{
    engine.gui->log_message(TCODColor::cyan, "Left-click an enemy to confuse it. \nRight-click to cancel.");
    int x,y;
    if(!engine.pickATile(&x, &y, range))
        return false;

    Actor *actor = engine.getActorAt(x, y);
    if(!actor)
        return false;

    Ai *confused_ai = new ConfusedMonsterAi(duration, actor->ai);
    actor->ai = confused_ai;

    engine.gui->log_message(TCODColor::lightGreen,
            "The eyes of the %s look vacant,\nas he starts to stumble around!", actor->name);
    return Pickable::use(item, user);
}

void Pickable::drop(Actor *item, Actor *owner)
{
    if(owner->container)
    {
        owner->container->remove(item);
        engine.actors.push(item);
        item->x = owner->x;
        item->y = owner->y;
        engine.gui->log_message(TCODColor::lightGrey, "%s drops a %s.", owner->name, item->name);
    }
}

