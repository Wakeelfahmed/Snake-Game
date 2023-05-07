#include <iostream>
#include <vector>
#include "C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include "C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"
#include<string>
using namespace std;
//#include "Text_Box.h"
const int SCREEN_WIDTH = 1140;
const int SCREEN_HEIGHT = 720;
const int BLOCK_SIZE = 20;
const int GRID_WIDTH = (SCREEN_WIDTH / BLOCK_SIZE);
const int GRID_HEIGHT = (SCREEN_HEIGHT / BLOCK_SIZE);
TTF_Font* font = TTF_OpenFont("arial.ttf", 100);
void Display_Text(std::string text_for_Box, SDL_Rect Box, SDL_Color Text_Color, SDL_Renderer* renderer) {
	if (font == NULL)
		cout << "ERROR!!!\n(Arial.ttf) Font Not Found - unable to render text" << endl;
	int w, h;
	double scale = 0.65;
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
}
enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};
struct SnakeBlock {
	int x, y;
};
class Snake {
public:
	Snake() {
		body.push_back({ 4, 4 });
		body.push_back({ 3, 4 });
		body.push_back({ 2, 4 });
	}
	Snake(int x, int y) {
		body.push_back({ x, y });
	}
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
	bool check_collision() {
		int head_x = body[0].x;
		int head_y = body[0].y;

		// check wall collision
		if (head_x < 0 || head_x >= GRID_WIDTH || head_y < 0 || head_y >= GRID_HEIGHT) {
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
	std::vector<SnakeBlock> body;
};
class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake snake;
	Direction dir;
	SnakeBlock food;
	int Score;
	void spawn_food() {
		bool found = false;
		while (!found) {
			int x = rand() % GRID_WIDTH;
			int y = rand() % GRID_HEIGHT;

			found = true;
			for (auto block : snake.body) {
				if (block.x == x && block.y == y) {
					found = false;
					break;
				}
			}
			if (found) {
				food.x = x;
				food.y = y;
			}
		}
	}
	time_t Start_time;
	short Timer_Set;
public:
	Game() {
		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();
		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		font = TTF_OpenFont("arial.ttf", 100);//16  //max : 7332 /1000
		// create snake
		snake = Snake(GRID_WIDTH / 2, GRID_HEIGHT / 2);

		// spawn food/
		spawn_food();
		Score = 0;
		Start_time = time(NULL);   Timer_Set = 45;
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

			// move snake
			snake.move(dir);

			// check for collision
			if (snake.check_collision()) {
				quit = true;
				continue;
			}

			// check for food
			if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
				Score++;
				Start_time += 15;
				snake.grow();
				spawn_food();
			}

			// clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			// draw snake
			bool head = 1;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			for (auto block : snake.body) {
				if (head) {
					SDL_SetRenderDrawColor(renderer, 255, 150, 155, 255);
					head = 0;
				}
				else
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_Rect rect = { block.x * BLOCK_SIZE, block.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_RenderFillRect(renderer, &rect);
			}
			SDL_Rect box;
			string timer_left = to_string(Timer_Set - (time(NULL) - Start_time));
			Display_Text("Timer" + to_string(Timer_Set - (time(NULL) - Start_time)), { 500/*X*/,30/*yY*/, 0,0 }, { 255 }, renderer);
			Display_Text(("Score " + to_string(Score)), { SCREEN_WIDTH - 250/*X*/,30/*yY*/, 0,0 }, { 255 }, renderer);
			// draw food
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_Rect rect = { food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
			SDL_RenderFillRect(renderer, &rect);

			// present renderer
			SDL_RenderPresent(renderer);
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