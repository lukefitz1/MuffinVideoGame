/*******************************************************
** Filename: main.cpp
** Programmer: Luke Fitzgerald
** Purpose: Side - scrolling Muffin Man game
********************************************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "mappy_A5.h"

int collided(int, int);
#include "muffindefs.h"
#include "muffinManClass.h"
#include "evilMuffin.h"

void changeGameState(int &, int);
bool spriteCollide (MuffinMan *, EvilMuffin *);

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool redrawRequired = true, quit = false; int mapxoff, mapyoff;

	//initialize allegro
	if (!al_init())
	{
		return -1;
	}

	//initialize keyboard
	if (!al_install_keyboard())
	{
		return -2;
	}

	//initialize timer
	timer = al_create_timer (1.0 / FPS);
	if (!timer)
	{
		return -3;
	}

	//initalize display
	display = al_create_display(SCRN_W, SCRN_H);
	if (!display)
	{
		al_destroy_timer(timer);
		return -4;
	}

	al_init_image_addon();

	//setup fonts
	al_init_font_addon();
	al_init_ttf_addon();
	text28 = al_load_font("arial.ttf", 28, 0);
	text20 = al_load_font("arial.ttf", 20, 0);

	if (!text28)
	{
		return -6;
	}

	//set back buffer 
	al_set_target_bitmap(al_get_backbuffer(display));

	//initialize sprite
	MuffinMan *muffin = new MuffinMan;

	//initialize enemy sprites
	EvilMuffin *evilMuffin = new EvilMuffin(800, 400);
	EvilMuffin *evilMuffin2 = new EvilMuffin(2688, 400);

	//initialize event queue
	eventQueue = al_create_event_queue();
	if (!eventQueue)
	{
		al_destroy_timer(timer);
		al_destroy_display(display);
		return -7;
	}

	//register events
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	al_init_primitives_addon();
	
	//audio
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(2);

	bgSong = al_load_sample("BGMusic.wav");

	if (MapLoad("MuffinManMap2.FMP", 1) != 0)
	{
		return -9;
	}

	//Change game state to start menu
	changeGameState(gameState, STARTUP);

	//al_draw_bitmap(backgroundImage, 0, 0, 0);
	al_flip_display();

	//start timer
	al_start_timer(timer);

	//central loop
	while (!quit)
	{
		ALLEGRO_EVENT evt;
		//Wait for one of the allegro-defined events
		al_wait_for_event(eventQueue, &evt);

		switch (evt.type)
		{
			case ALLEGRO_EVENT_KEY_DOWN:
				switch(evt.keyboard.keycode)
				{
					case ALLEGRO_KEY_ESCAPE:
						quit = true;
						break;
					case ALLEGRO_KEY_LEFT:
						keys[LEFT] = true;
						break;
					case ALLEGRO_KEY_RIGHT:
						keys[RIGHT] = true;
						break;
					case ALLEGRO_KEY_UP:
						keys[UP] = true;
						break;
					case ALLEGRO_KEY_DOWN:
						keys[DOWN] = true;
						break;
					case ALLEGRO_KEY_SPACE:
						keys[SPACE] = true;
						break;
					case ALLEGRO_KEY_P:
						keys[P] = true;
						break;
					case ALLEGRO_KEY_Q:
						keys[Q] = true;
						break;
					case ALLEGRO_KEY_ENTER:
						keys[ENTER] = true;
						break;
				}
				break;

			case ALLEGRO_EVENT_KEY_UP:
				switch(evt.keyboard.keycode)
				{
					case ALLEGRO_KEY_ESCAPE:
						quit = true;
						break;
					case ALLEGRO_KEY_LEFT:
						keys[LEFT] = false;
						break;
					case ALLEGRO_KEY_RIGHT:
						keys[RIGHT] = false;
						break;
					case ALLEGRO_KEY_UP:
						keys[UP] = false;
						break;
					case ALLEGRO_KEY_DOWN:
						keys[DOWN] = false;
						break;
					case ALLEGRO_KEY_SPACE:
						keys[SPACE] = false;
						break;
					case ALLEGRO_KEY_P:
						keys[P] = false;
						break;
					case ALLEGRO_KEY_Q:
						keys[Q] = true;
						break;
					case ALLEGRO_KEY_ENTER:
						keys[ENTER] = false;
						break;
				}
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				quit = true;
				break;
			
			case ALLEGRO_EVENT_TIMER:

				if (gameState == STARTUP)
				{
					if (keys[ENTER])
						changeGameState(gameState, PLAYING);
					if (keys[Q])
						quit = true;
				}
				
				if (gameState == PAUSED)
				{
					if (keys[ENTER])
						changeGameState(gameState, PLAYING);
					else if (keys[Q])
						quit = true;
				}

				if (gameState == PLAYING)
				{
					muffin->updatePlayer();
					evilMuffin->updatePlayer();
					evilMuffin2->updatePlayer();

					//bg music
					al_play_sample(bgSong, .7, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);


					//map scroll position
					mapxoff = muffin->getX() + muffin->getWidth()/2 - SCRN_W/2 + 10;
					mapyoff = muffin->getY() + muffin->getHeight()/2 - SCRN_H/2 + 10;


					//avoid moving beyond map edge
					if (mapxoff < 0)
						mapxoff = 0;
					if (mapxoff > (mapwidth * mapblockwidth - SCRN_W))
						mapxoff = mapwidth * mapblockwidth - SCRN_W;
					if (mapyoff < 0)
						mapyoff = 0;
					if (mapyoff > (mapheight * mapblockheight - SCRN_H))
						mapyoff = mapheight * mapblockheight - SCRN_H;

					//check collisions
					if (spriteCollide(muffin, evilMuffin))
					{
						muffin->setLife(muffin->getLife() - 25);

						if (muffin->getLife() == 0)
						{
							muffin->setLives(muffin->getLives() - 1);
							if (muffin->getLives() == 0)
							{
								changeGameState(gameState, GAMEOVER);
							}
							else if (muffin->getLives() > 0)
							{
								muffin->setLife(100);
							}
						}	
							
						if (muffin->getLife() > 0)
						{
							muffin->setX(30);
							muffin->setY(421);
						}
					}

					if (spriteCollide(muffin, evilMuffin2))
					{
						muffin->setLife(muffin->getLife() - 25);

						if (muffin->getLife() == 0)
						{
							muffin->setLives(muffin->getLives() - 1);
							if (muffin->getLives() == 0)
							{
								changeGameState(gameState, GAMEOVER);
							}
							else if (muffin->getLives() > 0)
							{
								muffin->setLife(100);
							}
						}	
							
						if (muffin->getLife() > 0)
						{
							muffin->setX(2355);
							muffin->setY(261);
						}
					}
				
					if (keys[P])
						changeGameState(gameState, PAUSED);	

					//falling off 
					if (muffin->getY() + muffin->getHeight() > 560)
					{
						muffin->setLives(muffin->getLives() - 1);
						if (muffin->getLives() == 0)
							changeGameState(gameState, GAMEOVER);
						else 
						{
							muffin->setLife(100);
							muffin->setX(30);
							muffin->setY(421);
						}
					}

					//reaching the doorway 
					if (muffin->getX() >= 5200 && muffin->getY() >= 419)
					{
						changeGameState(gameState, WINNING);
					}

					redrawRequired = true;
				}
				break;
		} //end switch evt.type 
		
		if (redrawRequired && al_is_event_queue_empty(eventQueue))
		{
			if (gameState == PLAYING)
			{
				redrawRequired = false;
				al_clear_to_color(al_map_rgb(0, 0, 0));

				//Draw the background
				MapDrawBG(mapxoff, mapyoff, 0, 0, SCRN_W-1, SCRN_H-1);
				MapDrawFG(mapxoff, mapyoff, 0, 0, SCRN_W-1, SCRN_H-1, 0);

				al_draw_textf(text20, WHITE, 30, 30, 0, "Press P to Pause");
				al_draw_textf(text20, WHITE, 30, 60, 0, "Press Spacebar to Jump");
				al_draw_textf(text20, WHITE, 30, 90, 0, "Life: %i", muffin->getLife());
				al_draw_textf(text20, WHITE, 30, 120, 0, "Lives: %i", muffin->getLives());

				/*int muffinxpos = muffin->getX();
				al_draw_textf(text20, WHITE, 30, 150, 0, "Y= %i", muffinxpos);
				int muffinypos = muffin->getY();
				al_draw_textf(text20, WHITE, 30, 180, 0, "Y= %i", muffinypos);
				*/

				//Draw the player's sprite
				muffin->drawPlayer(mapxoff, mapyoff);
				evilMuffin->drawPlayer(mapxoff, mapyoff);	
				evilMuffin2->drawPlayer(mapxoff, mapyoff);
			}
			else if (gameState == STARTUP)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2, ALLEGRO_ALIGN_CENTRE, "START MENU");
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2 + 30, ALLEGRO_ALIGN_CENTRE, "Press Enter to play");
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2 + 60, ALLEGRO_ALIGN_CENTRE, "Press Q to quit");
			}

			else if (gameState == PAUSED)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2, ALLEGRO_ALIGN_CENTRE, "Paused");
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2 + 30, ALLEGRO_ALIGN_CENTRE, "Press Enter to Continue");
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2 + 60, ALLEGRO_ALIGN_CENTRE, "Press Q to quit");
			}

			else if (gameState == GAMEOVER)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2, ALLEGRO_ALIGN_CENTRE, "GAME OVER......YOU SUCK");
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2 + 30, ALLEGRO_ALIGN_CENTRE, "Press Enter to Quit");
				al_stop_samples();
				if (keys[ENTER])
				{
					quit = true;
				}
			}

			else if (gameState == WINNING)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2, ALLEGRO_ALIGN_CENTRE, "YOU WON! CONGRATULATIONS!!");
				al_draw_textf(text28, WHITE, SCRN_W/2, SCRN_H/2 + 30, ALLEGRO_ALIGN_CENTRE, "Press Enter to Quit");
				al_stop_samples();
				if (keys[ENTER])
				{
					quit = true;
				}
			}

			al_flip_display();
		}
	}

	delete muffin;
	delete evilMuffin;
	
	MapFreeMem();
	al_destroy_font(text28);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(eventQueue);
	al_destroy_sample(bgSong);

	return 0;
}

int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata ->tl || blockdata->tr || blockdata->bl || blockdata->br;	
}

void changeGameState(int &state, int newState)
{
	state = newState;
	
	switch(state)
	{
		case STARTUP: break;
		case PLAYING: break;
		case PAUSED: break;
		case GAMEOVER: break;
		case WINNING: break;
	}
}

bool spriteCollide (MuffinMan *sprite1, EvilMuffin *sprite2)
{
	int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;

	left1 = sprite1->getX() + sprite1->getCollXOff();
	left2 = sprite2->getX() + sprite2->getCollXOff();
	right1 = left1 + sprite1->getCollWidth();
	right2 = left2 + sprite2->getCollWidth();
	top1 = sprite1->getY() + sprite1->getCollYOff();
	top2 = sprite2->getY() + sprite2->getCollYOff();
	bottom1 = top1 + sprite1->getCollHeight();
	bottom2 = top2 + sprite1->getCollHeight();
	
	if (bottom1 < top2) return FALSE;
	if (top1 > bottom2) return FALSE;
	if (right1 < left2) return FALSE;
	if (left1 > right2) return FALSE;
	
	return TRUE;
}	//end spriteCollide*/