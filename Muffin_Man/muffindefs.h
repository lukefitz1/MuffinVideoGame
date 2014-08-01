/********************************************************
** Filename: muffindefs.h
** Programmer: Luke Fitzgerald
** Purpose: Defined constants for muffin man game
*********************************************************/

#ifndef MUFFINMANDEFS_H
#define MUFFINMANDEFS_H

//define frames per second
#define FPS 60

//define screen
#define SCRN_W 640
#define SCRN_H 480
#define MUFFINJUMP 1600

//define fonts
ALLEGRO_FONT *text28;
ALLEGRO_FONT *text20;

//colors
#define BLACK al_map_rgb(0, 0, 0)
#define WHITE al_map_rgb(255, 255, 255)

//define keys array
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, P, ENTER, Q};
bool keys[] = {false, false, false, false, false, false, false, false};

//define game states
enum GAMESTATE {STARTUP, PLAYING, PAUSED, GAMEOVER, WINNING};
int gameState = -1;

//jump audio
ALLEGRO_SAMPLE *jumpUp;
ALLEGRO_SAMPLE *bgSong;

#endif

/*error codes
-1  Failed to initialize allegro
-2  Failed to initialize keyboard input
-3  Failed to initialize timer
-4  Failed to initialize display
-5  Failed to load background image
-6  Failed to initialize font
-7  Failed to create event queue
-8  Failed to load sprite image
*/
