#include "main.h"
#include <stdarg.h> //va-list
#include <stdio.h> //vsprintf
#include <string.h> //strlen

extern const int GUI_CONSOLE_HEIGHT;
extern const int GUI_HEALTHBAR_WIDTH;
extern const int GUI_LOG_XPOS;
extern const int GUI_LOG_HEIGHT;
extern const bool DEBUG;

Gui::Gui()
{
    con = new TCODConsole(engine.screen_width, GUI_CONSOLE_HEIGHT);
}

Gui::~Gui() {
    delete con;
    log.clearAndDelete();
}

//About const char* : Being what it is, you cannot initialize it any way other
//than a memcopy or strdup. strdup allocates onto heap!
Gui::Message::Message(const char *msg, const TCODColor &col) :
    text(strdup(msg)), col(col) {
}

//strDup uses malloc. Therefore free.
Gui::Message::~Message(){
    free(text);
}

void Gui::render()
{
    con->setDefaultBackground(TCODColor::black);
    con->clear();
    renderBar(1, 1, GUI_HEALTHBAR_WIDTH, "HP", engine.player->destructible->hp, 
            engine.player->destructible->max_hp, TCODColor::lightRed, TCODColor::darkerRed);

    int y = 1;
    //Darken messages as they get older
    float visibility = 0.4f;
    for(Message **iterator = log.begin(); iterator != log.end(); iterator++)
    {
        Message *msg = *iterator;
        con->setDefaultForeground(msg->col * visibility);
 
        con->print(GUI_LOG_XPOS, y, msg->text);
        y++;

        //newer messages become more visible.
        if(visibility < 1.0f)
            visibility += 0.3f;
    }

    renderMouseLook();
    TCODConsole::blit(con, 0, 0, engine.screen_width, GUI_CONSOLE_HEIGHT, TCODConsole::root,
            0, engine.screen_height-GUI_CONSOLE_HEIGHT);

}

void Gui::renderBar(int x, int y, int width, const char* name, float value, float max_value,
        const TCODColor &bar_color, const TCODColor &back_color)
{
    con->setDefaultBackground(back_color);
    con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

    int bar_width = (int)(value / max_value * width);
    if(bar_width > 0)
    {
        con->setDefaultBackground(bar_color);
        con->rect(x, y, bar_width, 1, false, TCOD_BKGND_SET);
    }

    con->setDefaultForeground(TCODColor::white);
    con->printEx(x+width/2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, max_value);
}


void Gui::log_message(const TCODColor &col, const char *text, ...)
{
    //boilerplate for variadic fxn. Needs stdarg.h
    va_list ap;
    char buf[128];
    //Take pointer to last defined argument
    va_start(ap, text);
    //Using the previous pointer as starting point, iterate down the stack. For
    //each addition input, read the input into the buffer, then does necessary
    //replacements in the format string (char *text)
    vsprintf(buf, text, ap);
    va_end(ap);

    //TODO Line wrapping?
    char *line_begin = buf;
    char *line_end;

    do {
        if(log.size() == GUI_LOG_HEIGHT) 
        {
            Message *to_remove = log.get(0);
            log.remove(to_remove);
            delete to_remove;
        }

        //break multi-line messages with a search for newlines
        line_end = strchr(line_begin, '\n');
        if(line_end)
            *line_end = '\0';

        Message *msg = new Message(line_begin, col);
        log.push(msg);
        line_begin = line_end + 1;
    } while(line_end);
}

void Gui::renderMouseLook()
{
    if(!engine.map->inFov(engine.mouse.cx, engine.mouse.cy))
        return;

    char buf[128] = "";
    bool first = true;
    for(Actor **it = engine.actors.begin(); it != engine.actors.end(); it++)
    {
        Actor *actor = *it;
        if(actor->x == engine.mouse.cx && actor->y == engine.mouse.cy)
        {
            if(first)
                first = false;
            else
                strcat(buf, ", ");
            strcat(buf, actor->name);
        }
    }

    con->setDefaultForeground(TCODColor::lightGrey);
    con->print(1,0,buf);
}


