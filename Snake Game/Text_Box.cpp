#include "Text_Box.h"
#include<string>
#include<string.h>
using namespace std;
static double scale;
static int Rounding_Radius;

void Text_Box::set_Box_Text_Char(char Text, int Font_Size, SDL_Color Text_Color, int style) {
	if (buttonTextSurface)
		SDL_FreeSurface(buttonTextSurface);
	if (buttonTextTexture)
		SDL_DestroyTexture(buttonTextTexture);
	text_for_Box = "";
	text_for_Box = Text;
	TTF_SetFontSize(font, Font_Size);
	TTF_SetFontStyle(font, style);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	buttonTextSurface = TTF_RenderText_Blended(font, text_for_Box.c_str(), Text_Color); //text Color
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}
void Text_Box::set_Box_Text(const char* Text, int Font_Size, SDL_Color Text_Color, int style) {
	if (font == NULL) {
		//TTF_Init();
		font = TTF_OpenFont("arial.ttf", 100);
		//TTF_OpenFont("arial.ttf", 100);
	}
	if (buttonTextSurface)
		SDL_FreeSurface(buttonTextSurface);
	if (buttonTextTexture)
		SDL_DestroyTexture(buttonTextTexture);
	text_for_Box = Text;
	TTF_SetFontSize(font, Font_Size);
	if (font == NULL) {
		TTF_Init();
		font = TTF_OpenFont("arial.ttf", 100);
	}
	TTF_SetFontStyle(font, style);
	buttonTextSurface = TTF_RenderText_Blended(font, text_for_Box.c_str(), Text_Color); //text Color
	buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
}
void Text_Box::set_Text_Box(const char* Text, int Font_Size, SDL_Color Text_Color, COORD Position, Size Box_Size, SDL_Color Box_Color, bool BOLD) {
	this->Position = Position;
	this->Box_Size = Box_Size;		Box = { Position.X, Position.Y, Box_Size.height, Box_Size.width };
	this->Box_Color = Box_Color;
	set_Box_Text(Text, Font_Size, Text_Color, BOLD);
}
void Text_Box::set_Box_Color(SDL_Color Box_Color1) { Box_Color = Box_Color1; }
void Text_Box::set_Box_Size(Size New_Size) { Box_Size = New_Size; }
void Text_Box::Display_Text_Box(SDL_Color newcolor, bool usenewcolor_instead_boxcolor) const {
	if (usenewcolor_instead_boxcolor)
		roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, Rounding_Radius, newcolor.r, newcolor.g, newcolor.b, 255);
	else
		roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, Rounding_Radius, Box_Color.r, Box_Color.g, Box_Color.b, 255);
	//scale = .65;//1.5
	int w, h;
	SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
	double x, y;
	x = Box.x + (Box.w - double(w) * scale) / 2;
	y = Box.y + (Box.h - double(h) * scale) / 2;
	SDL_Rect dst;
	dst = { int(x), int(y), int(w * scale),int(h * scale) };
	SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
}
string Text_Box::get_text_for_Box() { return text_for_Box; }
SDL_Rect Text_Box::get_Main_Box() const { return Box; }
SDL_Color Text_Box::get_Box_Color() const { return Box_Color; }
Size Text_Box::get_Box_Size() const { return Box_Size; }
COORD Text_Box::get_Box_Position() const { return Position; }
SDL_Surface* Text_Box::get_buttonTextSurface() const { return buttonTextSurface; }
SDL_Texture* Text_Box::get_buttonTextTexture() const { return buttonTextTexture; }
Text_Box::~Text_Box() {
	//SDL_DestroyTexture(buttonTextTexture);
	//SDL_FreeSurface(buttonTextSurface);
	//cout << "Called Text des\n";
}

double Text_Box::scale = 0.65;
int Text_Box::Rounding_Radius = 20;