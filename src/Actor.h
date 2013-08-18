#ifndef ACTOR_H
#define ACTOR_H
class Actor 
{
    public:
        int x,y;
        int ch;
        const char* name;
        TCODColor col;
        bool blocking; //can we walk on this Actor
        Attacker *attacker; //can deal damage
        Destructible *destructible; //can be damaged
        Ai *ai; 

        Actor(int x, int y, int ch, const char* name, const TCODColor &col);
        void update();
        void render() const; 
};

#endif
