#ifndef CONFIG
#define CONFIG
const bool DEBUG = true;
//Map generation variables
const int ROOM_MAX_SIZE = 12;
const int ROOM_MIN_SIZE = 6;
const int MAX_ROOM_MONSTERS = 3;

//Size of the console (message bar + health bar)
const int GUI_CONSOLE_HEIGHT = 7; //Panel Height

const int GUI_HEALTHBAR_WIDTH = 20; // Bar Width

//Message log constants
const int GUI_LOG_XPOS = GUI_HEALTHBAR_WIDTH + 2; //Msg X
const int GUI_LOG_HEIGHT = GUI_CONSOLE_HEIGHT - 1; //Msg Height

//How many turns a monster will track, even after you've left it's FOV
const int TRACKING_TURNS = 3;
#endif
