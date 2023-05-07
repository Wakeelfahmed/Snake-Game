//extern SDL_Window* window;
//extern SDL_Renderer* renderer;
//extern TTF_Font* font;
//#include"Globals.h"
//#include "C:\SDL2-devel-2.26.1-VC\include\SDL.h"		//for SDL GUI
//#include "C:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"	//for Advanced SDL GUI
//#include "C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"	//for SDL GUI Font

#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#pragma once
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h> // for COORD
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // for time()
#include <algorithm> // for sort(),unique() etc
#include <vector>    // for vector

using namespace std;
#define Screen_Width 405
#define Screen_Height 571
//SDL_Window* window;
//SDL_Renderer* renderer;
struct Size {
	int height;
	int width;
	Size() : height(0), width(0) {}
	Size(int h, int w) : height(h), width(w) {}
};
class Text_Box {
protected:
	string text_for_Box;
	COORD Position{};
	SDL_Surface* buttonTextSurface{};
	SDL_Texture* buttonTextTexture{};
	SDL_Color Box_Color;
	Size Box_Size;
public:
	static double scale;
	static int Rounding_Radius;
	SDL_Rect Box;
	void set_Box_Text_Char(char Text, int Font_Size, SDL_Color Text_Color, int style);
	void set_Box_Text(const char* Text, int Font_Size, SDL_Color Text_Color, int style);
	void set_Text_Box(const char* Text, int Font_Size, SDL_Color Text_Color, COORD Position, Size Box_Size, SDL_Color Box_Color, bool BOLD);
	void set_Box_Color(SDL_Color Box_Color1);
	void set_Box_Size(Size New_Size);

	void Display_Text_Box(SDL_Color newcolor, bool usenewcolor_instead_boxcolor) const;
	string get_text_for_Box();
	SDL_Rect get_Main_Box() const;
	SDL_Color get_Box_Color() const;
	Size get_Box_Size() const;
	COORD get_Box_Position() const;
	SDL_Surface* get_buttonTextSurface() const;
	SDL_Texture* get_buttonTextTexture() const;
	~Text_Box();
};

#endif TEXT_BOX_H
