class Gui {
    public :
        Gui();
        ~Gui();
        void render();
        //fancy variadic function.
        void log_message(const TCODColor &col, const char* text, ...);


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
