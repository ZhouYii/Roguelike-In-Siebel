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

        int screen_width;
        int screen_height;
        TCOD_key_t last_key;
        
        TCODList<Actor *> actors;
        Actor * player;
        Map *map;

        //Radius of zero == infinite radius
        int fov_radius;

        Engine(int screen_width, int screen_height);
        ~Engine();
        void update();
        void render();
        void sendToBack(Actor *actor);

    private:
        bool compute_fov;
};

extern Engine engine;

#endif
