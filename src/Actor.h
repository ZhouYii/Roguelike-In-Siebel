#ifndef ACTOR_H
#define ACTOR_H

class Actor {
    public:
        int x,y;
        int ch;
        const char* name;
        TCODColor col;

        Actor(int x, int y, int ch, const char* name, const TCODColor &col);
        void update();
        bool moveOrAttack(int x, int y);
        void render() const; 
};

#endif
