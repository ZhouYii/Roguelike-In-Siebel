class Menu {
    public :
        enum MenuItem {
            NONE,
            NEW_GAME,
            CONTINUE,
            EXIT,
            CONSTITUTION,
            STRENGTH,
            AGILITY
        };
        enum DisplayMode {
            MAIN,
            PAUSE
        };
        ~Menu();
        void clear();
        void addItem(MenuItem item, const char* label);
        MenuItem pick(DisplayMode mode=MAIN);

    protected:
        struct MenuLine {
            MenuItem item;
            const char *label;
        };
        TCODList<MenuLine *> items;
};



class Gui : public Persistent {
    public :
        Menu menu;
        Gui();
        ~Gui();
        void render();
        //fancy variadic function.
        void log_message(const TCODColor &col, const char* text, ...);
        void load(TCODZip &zip);
        void save(TCODZip &zip);
        void clear();


    protected:
        TCODConsole *con;
        struct Message {
            char *text;
            TCODColor col;
            Message(const char *text, const TCODColor &col);
            ~Message();
        };
        TCODList<Message *> log;

        void renderBar(int x, int y, int width, const char* name, float value, 
                float max_value, const TCODColor &bar_color, const TCODColor &back_color);
        void renderMouseLook();

};
