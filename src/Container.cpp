#include "main.h"

Container::Container(int size) : size(size) {}

Container::~Container() 
{
    inventory.clearAndDelete();
}

bool Container::add(Actor *item)
{
    if(size > 0 && inventory.size() >= size)
        return false; //Full

    inventory.push(item);
    return true;
}

void Container::remove(Actor *element)
{
    inventory.remove(element);
}

