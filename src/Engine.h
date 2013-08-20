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
        Gui *gui;
        TCOD_key_t last_key;
        TCOD_mouse_t mouse;

        TCODList<Actor *> actors;
        //Guests either in actors list or roster list.
        TCODList<Actor *> guest_roster;
        Actor * player;
        Actor * stairs;
        Map *map;
        

        //Radius of zero == infinite radius
        int fov_radius;
        int level;

        Engine(int screen_width, int screen_height);
        ~Engine();
        void update();
        void render();
        void init();
        void init_roster();
        void terminate();
        void load();
        void save();
        void sendToBack(Actor *actor);
        void nextLevel();

        //Returns NULL if no monsters found within range.
        //0 range is equivalent to infinite range
        Actor* getClosestMonster(int x, int y, float range) const;
        Actor* getActorAt(int x, int y) const;

        //Mouse Aiming
        bool pickATile(int *x, int *y, float max_range = 0.0f);

    private:
        bool compute_fov;
};

extern Engine engine;

#endif
