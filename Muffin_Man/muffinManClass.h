/**************************************************
** Filename: muffinManClass.h
** Programmer: Luke Fitzgerald
** Purpose: Class for the muffin sprite
***************************************************/

class MuffinMan
{
	private:
			float x, y;
			float oldpx, oldpy;
			int width, height;
			int facing;
			int jump;
			int curframe, maxframe;
			int framecount, framedelay;
			ALLEGRO_BITMAP *playerImage[7];
			float collWidth, collHeight;
			float collXOffset, collYOffset;
			int life;
			int lives;

	public:
			MuffinMan();
			~MuffinMan();

			void updatePlayer();
			void setX(int);
			void setY(int);
			float getX() {return x;}
			float getY() {return y;}
			int getWidth() {return width;}
			int getHeight() {return height;}
			int getFacing() {return facing;}
			void drawPlayer (int, int);
			bool loadImage();
			float getCollWidth() {return collWidth;}
			float getCollHeight() {return collHeight;}
			float getCollXOff() {return collXOffset;}
			float getCollYOff() {return collYOffset;}
			int getLife()	{return life;}
			void setLife(int);
			int getLives()	{return lives;}
			void setLives(int);
};

MuffinMan::MuffinMan()
{
	x = 30.0;
	y = 100.0;
	oldpx = x;
	oldpy = y;
	jump = MUFFINJUMP;	
	framecount = 0;
	framedelay = 4;
	facing = 1;
	curframe = 0;
	loadImage();
	maxframe = 6;
	life = 100;
	lives = 3;
} //end constructor

MuffinMan::~MuffinMan()
{
	for (int ct = 0; ct < 7; ++ct)
	{
		al_destroy_bitmap(playerImage[ct]);
	}
}

bool MuffinMan::loadImage()
{
	playerImage[0] = al_load_bitmap("RUN1.bmp");
	al_convert_mask_to_alpha(playerImage[0], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[0]);
	height = al_get_bitmap_height(playerImage[0]);

	playerImage[1] = al_load_bitmap("RUN2.bmp");
	al_convert_mask_to_alpha(playerImage[1], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[1]);
	height = al_get_bitmap_height(playerImage[1]); 

	playerImage[2] = al_load_bitmap("RUN3.bmp");
	al_convert_mask_to_alpha(playerImage[2], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[2]);
	height = al_get_bitmap_height(playerImage[2]);

	playerImage[3] = al_load_bitmap("RUN4.bmp");
	al_convert_mask_to_alpha(playerImage[3], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[3]);
	height = al_get_bitmap_height(playerImage[3]);

	playerImage[4] = al_load_bitmap("RUN5.bmp");
	al_convert_mask_to_alpha(playerImage[4], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[4]);
	height = al_get_bitmap_height(playerImage[4]);

	playerImage[5] = al_load_bitmap("RUN6.bmp");
	al_convert_mask_to_alpha(playerImage[5], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[5]);
	height = al_get_bitmap_height(playerImage[5]);

	playerImage[6] = al_load_bitmap("RUN7.bmp");
	al_convert_mask_to_alpha(playerImage[6], al_map_rgb(255,0,255));
	width = al_get_bitmap_width(playerImage[6]);
	height = al_get_bitmap_height(playerImage[6]);

	collWidth = width * .7;
	collHeight = height * .7;
	collXOffset = (width - collWidth)/2;
	collYOffset = (height - collHeight)/2;

	return true;
}

void MuffinMan::updatePlayer()
{
	oldpy = y;
	oldpx = x;

	if (keys[RIGHT])
	{
		facing = 1;
		x += 3;
		if (++framecount > framedelay)
		{
			framecount = 0;
			if (++curframe > maxframe)
				curframe = 0;
		}
	}

	else if (keys[LEFT])
	{
		facing = 0;
		x -= 3;
		if (x < 0)
			x = 0;
		if (++framecount > framedelay)
		{
			framecount = 0;
			if (++curframe > maxframe)
				curframe = 0;
		}
	}
	else curframe = 0;
	
	//jump
	if (jump == MUFFINJUMP)
	{
		if (!collided(x + width/2, y + height + 5))
		jump = 0;

		if (keys[SPACE])
		{
			jump = 30;
		}	
	}

	else
	{
		y -= jump/3;
		jump --;
	}

	if (jump < 0)
	{
		if (collided(x + width/2, y + height))
		{
			jump = MUFFINJUMP;
			while (collided(x + width/2, y + height))
				y -= 3;
		}
	}

	//check for collisions
	if (!facing)
	{
		if (collided(x, y + height))
			x = oldpx;
	}
	else
	{
		if (collided(x, y + height))
			x = oldpx;
	}
}

void MuffinMan::setX(int xVal)
{
	x = xVal;
}

void MuffinMan::setLife(int lifeLeft)
{
	life = lifeLeft;
}

void MuffinMan::setY(int yVal)
{
	y = yVal;
}

void MuffinMan::setLives(int numLives)
{
	lives = numLives;
}

void MuffinMan::drawPlayer(int mapxoff, int mapyoff)
{
	if (facing == 1)
		al_draw_bitmap(playerImage[curframe], (x-mapxoff), (y-mapyoff + 1), 0);
	else if (facing == 0)
		al_draw_bitmap(playerImage[curframe], (x-mapxoff), (y-mapyoff), ALLEGRO_FLIP_HORIZONTAL);
}