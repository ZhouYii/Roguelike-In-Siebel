#ifndef PERSISTENT_H
#define PERSISTENT_H

class Persistent {
    public:
        virtual void load(TCODZip &zip) = 0;
        virtual void save(TCODZip &zip) = 0;

};

#endif
