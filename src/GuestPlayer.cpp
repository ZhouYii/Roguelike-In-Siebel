#include "main.h"
Actor* GuestPlayer::createNemesis() {
    Actor *nemesis = new Actor(40, 25, '@', "Nemesis", TCODColor::black);
    nemesis->destructible = new MonsterDestructible(PLAYER_INIT_HP, 2, "dead body", 0);
    nemesis->attacker = new Attacker(5);
    nemesis->ai = new MonsterAi();
    nemesis->container = new Container(PLAYER_INV_SIZE);
    return nemesis;
}

Actor* GuestPlayer::createZhouYi() {
    Actor *zhou = new Actor(40,25,'J', "a wild joey", TCODColor::black);
    zhou->destructible = new MonsterDestructible(PLAYER_INIT_HP+90, 2, "dead body", 0);
    zhou->attacker = new Attacker(2);
    zhou->ai = new MonsterAi();
    zhou->container = new Container(PLAYER_INV_SIZE);
    return zhou;
}

