#include <iostream>
#include <vector>
#include "C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include "C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"
#include "C:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"
#include<string>
using namespace std;
//#include "Text_Box.h"
const int SCREEN_WIDTH = 1140;
const int SCREEN_HEIGHT = 720;
const int BLOCK_SIZE = 20;
const int GRID_WIDTH = (SCREEN_WIDTH / BLOCK_SIZE);
const int GRID_HEIGHT = (SCREEN_HEIGHT / BLOCK_SIZE);
TTF_Font* font = TTF_OpenFont("arial.ttf", 100);
void Display_Text(string text_for_Box, SDL_Rect Box, short Font_Size, SDL_Color Text_Color, SDL_Color Box_Color, SDL_Renderer* renderer) {
	if (font == NULL)
		cout << "ERROR!!!\n(Arial.ttf) Font Not Found - unable to render text" << endl;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, 5, Box_Color.r, Box_Color.g, Box_Color.b, Box_Color.a);
	int w, h;
	double scale = 0.65;
	TTF_SetFontSize(font, Font_Size);
	SDL_Surface* buttonTextSurface;
	SDL_Texture* buttonTextTexture;
	buttonTextSurface = TTF_RenderText_Blended(font, text_for_Box.c_str(), Text_Color); //text Color
	buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
	double x, y;
	x = Box.x + (Box.w - double(w) * scale) / 2;
	y = Box.y + (Box.h - double(h) * scale) / 2;
	SDL_Rect dst;
	dst = { int(x), int(y), int(w * scale),int(h * scale) };
	SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
	SDL_FreeSurface(buttonTextSurface);
	SDL_DestroyTexture(buttonTextTexture);
}
enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
struct SnakeBlock { int x, y; };
class Snake {
public:
	std::vector<SnakeBlock> body;
	Snake() {	}
	Snake(int x, int y) { body.push_back({ x, y }); }
	void move(Direction dir) {
		// move body
		for (int i = body.size() - 1; i > 0; i--) {
			body[i].x = body[i - 1].x;
			body[i].y = body[i - 1].y;
		}

		// move head
		switch (dir) {
		case DIR_UP:
			body[0].y--;
			break;
		case DIR_DOWN:
			body[0].y++;
			break;
		case DIR_LEFT:
			body[0].x--;
			break;
		case DIR_RIGHT:
			body[0].x++;
			break;
		}
	}
	void grow() {
		body.push_back(body.back());
	}
	void shrink() {
		if (body.size() > 1) {
			body.pop_back();
		}
	}
	bool check_collision() {
		int head_x = body[0].x;
		int head_y = body[0].y;

		// check wall collision
		if (head_x < 1/*L*/ || head_x >= GRID_WIDTH - 1/*R*/ || head_y <= 3 || head_y >= GRID_HEIGHT - 1) {
			return true;
		}
		// check self collision
		for (int i = 1; i < body.size(); i++) {
			if (head_x == body[i].x && head_y == body[i].y) {
				return true;
			}
		}

		return false;
	}
};
class Power
{
	SnakeBlock Position;
	string Symbol;
public:
	void set_Power(SnakeBlock position, string Symbol) {
		this->Symbol = Symbol;
		this->Position = position;
	}
	void set_Power(SnakeBlock position) {
		this->Position = position;
	}
	SnakeBlock get_Position() const { return Position; }
	string get_Symbol() const { return Symbol; }
};
class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake snake;
	Direction dir;
	std::vector<SnakeBlock> food_positions;  short Food_Counter;
	int Score;
	Power Powers[2];
	time_t Start_time;
	short Timer_Set;
	short Lives;
	void spawn_foods() {
		bool found = false;
		while (!found) {
			int x = rand() % 55 + 1;
			//y is 4 - 33
			// x is 1 - 55
			int y = rand() % 30 + 4;// exclude top 3 rows
			found = true;
			for (auto block : snake.body) {
				if (block.x == x && block.y == y) {
					found = false;
					break;
				}
			}
			if (found) {
				for (auto block : food_positions) {
					if (block.x == x && block.y == y) {
						found = false;
						break;
					}
				}
			}
			if (found) {
				food_positions.push_back({ x,y });
				Food_Counter++;
			}
		}
	}
	void Spawn_Power(short Power_to_Spawn) {
		bool found = false;
		while (!found) {
			int x = rand() % 55 + 1;
			//y is 4 - 33
			// x is 1 - 55
			int y = rand() % 30 + 4;// exclude top 3 rows
			found = true;
			for (auto block : snake.body) {
				if (block.x == x && block.y == y) {
					found = false;
					break;
				}
			}
			if (found) {
				for (auto block : food_positions) {
					if (block.x == x && block.y == y) {
						found = false;
						break;
					}
				}
			}
			if (found) {
				Powers[Power_to_Spawn].set_Power({ x,y });
			}
		}
	}
public:
	Game() {
		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();
		font = TTF_OpenFont("arial.ttf", 80);
		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		snake = Snake(GRID_WIDTH / 2, GRID_HEIGHT / 2);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		Score = 0;
		Start_time = time(NULL);   Timer_Set = 45;
		Lives = 3;
		Powers[0].set_Power({ 0, 0 }, "<>");		Powers[1].set_Power({ 0, 0 }, "><");
		Spawn_Power(0);		Spawn_Power(1);
	}
	~Game() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	void run() {
		bool quit = false;

		while (!quit) {
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					quit = true;
				}
				else if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						if (dir != DIR_DOWN) {
							dir = DIR_UP;
						}
						break;
					case SDLK_DOWN:
						if (dir != DIR_UP) {
							dir = DIR_DOWN;
						}
						break;
					case SDLK_LEFT:
						if (dir != DIR_RIGHT) {
							dir = DIR_LEFT;
						}
						break;
					case SDLK_RIGHT:
						if (dir != DIR_LEFT) {
							dir = DIR_RIGHT;
						}
						break;
					}
				}
			}

			while (Food_Counter < 3)
			{
				spawn_foods();
			}

			// move snake
			snake.move(dir);

			// check for collision
			if (snake.check_collision() || Timer_Set - (time(NULL) - Start_time) == 0) {
				Lives--;
				Start_time -= 10;
				if (Lives == 0 || Timer_Set - (time(NULL) - Start_time) == 0)
				{
					//quit = true;
					Start_time = time(NULL);
					SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
					Display_Text("Game Over", { (SCREEN_WIDTH - 400) / 2/*X*/,(SCREEN_HEIGHT - 250) / 2/*Y*/, 400,250 }, 100, { 255 }, { 155,155,155, 80 }, renderer);
					SDL_RenderPresent(renderer);
					SDL_Delay(25);
					//continue;
				}
			}

			for (auto block = food_positions.begin(); block != food_positions.end(); ++block) {
				if (snake.body[0].x == block->x && snake.body[0].y == block->y) {
					Score++;
					Start_time += 5;
					snake.grow();
					food_positions.erase(block);
					spawn_foods();
					break;
				}
			}

			// clear screen
			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 224, 230, 27, 225);
			int cellWidth = SCREEN_WIDTH / GRID_WIDTH;
			int cellHeight = SCREEN_HEIGHT / GRID_HEIGHT;
			SDL_Rect Wall = { 0, cellWidth * 3, SCREEN_WIDTH, cellHeight };     //top
			SDL_RenderFillRect(renderer, &Wall);
			Wall = { 0, (GRID_HEIGHT - 1) * cellHeight, SCREEN_WIDTH, cellHeight };     //bottom
			SDL_RenderFillRect(renderer, &Wall);
			Wall = { 0, cellWidth * 3, cellWidth, SCREEN_HEIGHT };     //left
			SDL_RenderFillRect(renderer, &Wall);
			Wall = { (GRID_WIDTH - 1) * cellWidth, cellWidth * 3, cellWidth, SCREEN_HEIGHT };     //right
			SDL_RenderFillRect(renderer, &Wall);

			// draw snake
			bool head = 1;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			for (auto block : snake.body) {
				if (head) {
					SDL_SetRenderDrawColor(renderer, 95, 229, 28, 255);
					head = 0;
				}
				else
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_Rect rect = { block.x * BLOCK_SIZE, block.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_RenderFillRect(renderer, &rect);
			}
			for (short i = 0; i < 2; i++)
			{
				SDL_Surface* textSurface = TTF_RenderText_Solid(font, Powers[i].get_Symbol().c_str(), { 255, 255, 255 });
				SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
				SDL_Rect rect = { Powers[i].get_Position().x * BLOCK_SIZE, Powers[i].get_Position().y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_RenderCopy(renderer, textTexture, NULL, &rect);
				SDL_FreeSurface(textSurface);
				SDL_DestroyTexture(textTexture);
				if (snake.body[0].x * BLOCK_SIZE == Powers[i].get_Position().x * BLOCK_SIZE && snake.body[0].y * BLOCK_SIZE == Powers[i].get_Position().y * BLOCK_SIZE) {
					if (i == 0) {
						int temp = snake.body.size();
						for (short i = 0; i < temp; i++)
						{
							snake.grow();
							Spawn_Power(0);
						}
					}
					if (i == 1) {
						for (short i = 0; i < snake.body.size() / 2; i++)
						{
							snake.shrink();
							Spawn_Power(1);
						}
					}
				}
			}


			SDL_Rect box;
			string timer_left = to_string(Timer_Set - (time(NULL) - Start_time));
			Display_Text("Timer " + to_string(Timer_Set - (time(NULL) - Start_time)), { SCREEN_WIDTH / 2/*X*/,30/*yY*/, 0,0 }, 80, { 255 }, { 155,155,155, 60 }, renderer);
			Display_Text(("Score " + to_string(Score)), { SCREEN_WIDTH - 250/*X*/,30/*yY*/, 0,0 }, 80, { 255 }, { 155,155,155, 60 }, renderer);
			// draw food
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			//SDL_Rect rect = { food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
			SDL_Rect rect;
			//SDL_RenderFillRect(renderer, &rect);
			for (auto block : food_positions) {
				rect = { block.x * BLOCK_SIZE, block.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_RenderFillRect(renderer, &rect);
			}

			int heart_x = 20;
			SDL_Color color = { 56, 255, 255, 255 };
			TTF_SetFontSize(font, 60);
			const char* text = u8"\u2665"; // Unicode code point for heart symbol
			for (int i = 0; i < Lives; i++) {
				SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_Rect dstrect = { heart_x, -5, surface->w, surface->h };
				heart_x += 40;//padding between hearts
				SDL_RenderCopy(renderer, texture, NULL, &dstrect);
				SDL_DestroyTexture(texture);
			}

			SDL_RenderPresent(renderer);
			if (Score > 10)
				SDL_Delay(50); // Add a delay of 100 milliseconds (adjust as needed)
			else
				SDL_Delay(55); // Add a delay of 100 milliseconds (adjust as needed)
		}
	}
};

int main(int argc, char* argv[]) {
	srand(time(nullptr)); // function to generate random number every time rand function call
	Game game;
	game.run();
	return 0;
}