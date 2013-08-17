#ifndef ENGINE_H
#define ENGINE_H

class Engine {
    public:
        enum GameStatus {
            STARTUP,
            IDLE,
            NEW_TURN,
            WIN,
            LOSE,
        } game_status;
        
        TCODList<Actor *> actors;
        Actor * player;
        Map *map;

        //Radius of zero == infinite radius
        int fov_radius;

        Engine();
        ~Engine();
        void update();
        void render();

    private:
        bool compute_fov;
};

extern Engine engine;

#endif
