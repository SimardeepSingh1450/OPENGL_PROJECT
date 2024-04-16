#include<stdio.h>
#include<SDL.h>
#include<SDL_image.h>
#undef main
#include "constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct ball {
	float x;
	float y;
	float width;
	float height;
} ball;

int initialize_window() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL \n");
		return FALSE;
	}

	//Creating SDL Window
	window = SDL_CreateWindow("Simar Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
	if (!window) {
		fprintf(stderr, "Error creating SDL window\n");
		return FALSE;
	}

	//Creating renderer
	renderer = SDL_CreateRenderer(window,-1,0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer\n");
		return FALSE;
	}

	return TRUE;
}

void setup() {
	ball.x = 20;
	ball.y = 20;
	ball.width = 15;
	ball.height = 15;
}

void process_input() {
	SDL_Event e;
	SDL_PollEvent(&e);

	switch (e.type) {
	case SDL_QUIT:
		game_is_running = FALSE;
		break;
	case SDL_KEYDOWN:
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			game_is_running = FALSE;
			break;
		}
	}
}

void update() {
	//waste some time or sleep until we reach target frame
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));//execution locked for some time

	//Instead of while loop burning out CPU resources, we use SDL_Delay
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	//delta time factor - factor to be used to update all my objects smoothly
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	//logic to keep a fixed timestamp/frame-rate
	last_frame_time = SDL_GetTicks();//SDL_GetTicks() returns the time passed since SDL_Init

	ball.x += 70*delta_time;
	ball.y += 70*delta_time;
}

void render() {
	//Setting the background color for buffer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);

	//HERE BELOW IS THE PART WHERE WE CAN START DRAWING OUR GAME OBJECTS
	//Draw a rectangle
	SDL_Rect ball_rect = {(int)ball.x,(int)ball.y,(int)ball.width,(int)ball.height};

	//Before rendering the color, set the rendercolor so that it does not render as black again because we allocated background earlier black
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	//Render the rectangle
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_RenderPresent(renderer);//Swapping out the front buffer with back buffer
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc,char* argv[]) {
	game_is_running = initialize_window();

	setup();

	while (game_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}