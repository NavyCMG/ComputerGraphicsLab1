#include <iostream>
#include "RasterSurface.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include "Helper.H"
#include <random>
#include <vector>

const unsigned int screenHight = 500;
const unsigned int screenWidth = 500;
unsigned int screen[250000];

unsigned int currentPixel = 0;

std::vector <int> xRand;
std::vector <int> yRand;

void GreenPixel();
void DrawAPixel(int currentPOS, unsigned int newColor);
int TwoDtoOneD(int x, int y, int arrWidth);
unsigned int ColorShift(unsigned int orig);
void ShiftRed();
void BLIT(int sourceX, int sourceY, int imgWidth, int imgHight, int rastX, int rastY, const unsigned int* sourceArr, int sourceWidth);
unsigned int ColorBlend(unsigned int currentColor, unsigned int newColor);

int main()
{
	int x = 0;
	int y = 0;
	srand(time(NULL));
	XTime *timer = new XTime();
	timer->Restart();
	for (int i = 0; i < 10; ++i)
	{
		xRand.push_back(Helper::RNJesus(0, screenWidth - 17));
		yRand.push_back(Helper::RNJesus(0, screenHight - 17));
	}
	float elapTime = 0;

	RS_Initialize("Grimes Connnor", screenWidth, screenHight);
	do{
		timer->Signal();
		elapTime += timer->Delta();
		for (int y = 0; y < screenHight; y += 30)
		{
			for (int x = 0; x < screenWidth; x += 30)
			{
				BLIT(288, 128, 30, 30, x, y, tiles_12_pixels, 400);// nested for loop lays down grass on entire screen
			}
		}
		for (int i = 0; i < 10; ++i)
		{
			BLIT(48, 190, 17, 17, xRand[i], yRand[i], tiles_12_pixels, tiles_12_width);
		}
		BLIT(x, y, 128, 128, 186, 186, teleporter_hit_pixels, teleporter_hit_width);
		if (elapTime > .033)
		{
			if (x < 896) { x += 128; }
			else {
				if (y < 896) { y += 128; }
				else { y = 0; }
				x = 0;
			}
			elapTime -= .033;
			//timer->Restart();
		}

	} while (RS_Update(screen, sizeof(screen)>>2));

	RS_Shutdown();
	delete timer;
}

void BLIT(int sourceX, int sourceY, int imgWidth, int imgHight, int rastX, int rastY,const unsigned int* sourceArr, int sourceWidth)
{
	for (int y = 0; y < imgHight; ++y)
	{
		for (int x = 0; x < imgWidth; ++x)
		{
			if ((x + rastX) < screenWidth && (y + rastY) < screenHight) {
				DrawAPixel(TwoDtoOneD(x + rastX, y + rastY, screenWidth), ColorShift(sourceArr[TwoDtoOneD(sourceX + x, sourceY + y, sourceWidth)]));
			}
		}
	}
}

unsigned int ColorShift(unsigned int orig) //shifts collor from BGRA to ARGB
{
	unsigned int alpha = ((orig & 0x000000ff) << 24);
	unsigned int red = ((orig & 0x0000ff00) << 8);
	unsigned int blue = ((orig & 0xff000000) >> 24);
	unsigned int green = ((orig & 0x00ff0000) >> 8);
	//unsigned int tempNew = (red | blue | green);
	unsigned int result = (alpha | red | blue | green);
	return result;
}

unsigned int ColorBlend(unsigned int currentColor, unsigned int newColor)
{
	unsigned int newAlpha = (newColor & 0xff000000);
	if (newAlpha != 0xff000000)
	{
		float ratio = float(newAlpha) / float(0xff000000);
		/*currentColor = (currentColor & 0x00ffffff);
		newColor = (newColor & 0x00ffffff);
		newColor = (int(newColor) - int(currentColor)) * ratio + currentColor;*/
		unsigned int curRed = (currentColor & 0x00ff0000) >> 16;
		unsigned int curGrn = (currentColor & 0x0000ff00) >>8;
		unsigned int curBlu = (currentColor & 0x000000ff);
		unsigned int newRed = (newColor & 0x00ff0000) >> 16;
		unsigned int newGrn = (newColor & 0x0000ff00) >> 8;
		unsigned int newBlu = (newColor & 0x000000ff);

		newRed = (int(newRed) - int(curRed)) * ratio + curRed;
		newGrn = (int(newGrn) - int(curGrn)) * ratio + curGrn;
		newBlu = (int(newBlu) - int(curBlu)) * ratio + curBlu;
		
		newColor = (newAlpha | newRed << 16 | newGrn << 8 | newBlu);
	}
	return newColor;
}

void ShiftRed()
{
	currentPixel = ColorShift(0x0000ffff);
}

void GreenPixel()
{
	currentPixel = 0xff00ff00;
}

void DrawAPixel(int currentPos, unsigned int newColor)
{
	currentPixel = screen[currentPos];
	screen[currentPos] = ColorBlend(currentPixel, newColor);
}

int TwoDtoOneD(int x, int y, int arrWidth)
{
	int result = (y * arrWidth) + x;
	return result;
}