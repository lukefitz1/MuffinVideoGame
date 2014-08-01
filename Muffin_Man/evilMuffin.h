/********************************************************
** Filename: evilMuffin.h
** Programmer: Luke Fitzgerald
** Purpose: class for evil muffin sprite
*********************************************************/

#ifndef EVILMUFFIN_H
#define EVILMUFFIN_h

class EvilMuffin
{
		private:
			float x, y;
			float oldpx, oldpy;
			int width, height;
			ALLEGRO_BITMAP *evilMuffinImage;
			float collWidth, collHeight;
			float collXOffset, collYOffset;
			int facing;
			int jump;

			public:
			EvilMuffin(int, int);
			~EvilMuffin();

			void updatePlayer();
			float getX() {return x;}
			float getY() {return y;}
			int getWidth() {return width;}
			int getHeight() {return height;}
			void drawPlayer (int, int);	
			float getCollWidth() {return collWidth;}
			float getCollHeight() {return collHeight;}
			float getCollXOff() {return collXOffset;}
			float getCollYOff() {return collYOffset;}
};

//Constructor
EvilMuffin::EvilMuffin(int xVal, int yVal)
{
	evilMuffinImage = al_load_bitmap("evilMuffin.bmp");
	al_convert_mask_to_alpha(evilMuffinImage, al_map_rgb(255,0,255));
	width = al_get_bitmap_width(evilMuffinImage);
	height = al_get_bitmap_height(evilMuffinImage);
	x = xVal;
	y = yVal;
	oldpx = x;
	oldpy = y;
	int facing = 1;

	//collision detection
	collWidth  = width * 0.65;
	collHeight = height * 0.65;
	collXOffset = (width - collWidth)/2.0;
	collYOffset = (height - collHeight)/2.0;
}

//Destructor
EvilMuffin::~EvilMuffin()
{
	al_destroy_bitmap(evilMuffinImage);
}

void EvilMuffin::drawPlayer(int mapxoff, int mapyoff)
{	
	al_draw_bitmap(evilMuffinImage, x - mapxoff, y - mapyoff, 0);
}

void EvilMuffin::updatePlayer()
{
	oldpy = y;
	oldpx = x;	
	jump = 1;
	
	if (!collided (x + width/2, y + height))
	{
		y += 3;
	}

	if (collided(x + width/2, y + height))
	{
		y = oldpy;
		x = oldpx;
	}

	//animate
	/*if(collided(x + width/2, y + height))
	{
		if (jump == 1)
		{
			for (int ct = 0; ct < 5; ct++)
				y-= 1;
		}
		jump = 0;
	}

	if (jump == 0)
	{
		if (!collided (x + width/2, y + height))
		{
			for (int ct = 0; ct < 5; ct++)
				y += 1;
		}
	}*/


	//check for collisions
	if (collided(x, y + height))
		x = oldpx;

}
#endif

