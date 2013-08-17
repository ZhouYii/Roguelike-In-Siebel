#ifndef ENGINE_H
#define ENGINE_H

class Engine {
    public:
        TCODList<Actor *> actors;
        Actor * player;
        Map *map;

        //Radius of zero == infinite radius
        int fovRadius;

        Engine();
        ~Engine();
        void update();
        void render();

    private:
        bool computeFov;
};

extern Engine engine;

#endif
