#include "main.h"

Actor::Actor(int x, int y, int ch, const char* name, const TCODColor &col) :
    x(x), y(y), ch(ch), name(name), col(col), blocking(true), attacker(NULL), 
    destructible(NULL), ai(NULL) {}

void Actor::render() const 
{
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() 
{
    if(ai)
        ai->update(this);
}


