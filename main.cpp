#include<stdio.h>
#include<SDL.h>
#include<SDL_image.h>
#undef main
#include "constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct player {
	float x;
	float y;
	float width;
	float height;
	float velocityX;
	float velocityY;
	//last-key variable for when a and d are pressed consecutively
	char lastKey;
	//These below keys are only for player
	bool aKeyPressed;
	bool dKeyPressed;
	bool wKeyPressed;
} player;

struct enemy {
	float x;
	float y;
	float width;
	float height;
	float velocityX;
	float velocityY;
	//last-key variable for when left and right are pressed consecutively
	char lastKey;
	//These below keys are only for enemy
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool upKeyPressed;
} enemy;

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
	//Player Initial Position Setup
	player.x = 0;
	player.y = 0;
	player.width = 50;
	player.height = 150;
	player.velocityY = 400;
	player.velocityX = 0;
	player.aKeyPressed = false;
	player.dKeyPressed = false;
	player.wKeyPressed = false;
	//Enemy Initial Position Setup
	enemy.x = 400;
	enemy.y = 100;
	enemy.width = 50;
	enemy.height = 150;
	enemy.velocityX = 0;
	enemy.velocityY = 0;
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
		else if (e.key.keysym.sym == SDLK_d) {
			//player.velocityX = 200;
			player.dKeyPressed = true;
			player.lastKey = 'd';
			break;
		}
		else if (e.key.keysym.sym == SDLK_a) {
			//player.velocityX = -200;
			player.aKeyPressed = true;
			player.lastKey = 'a';
			break;
		}
		else if (e.key.keysym.sym == SDLK_w) {
			//player.velocityX = -200;
			player.wKeyPressed = true;
			player.lastKey = 'w';
			player.velocityY = -2000;
			break;
		}
		else if(e.key.keysym.sym == SDLK_LEFT){
			enemy.leftKeyPressed = true;
			enemy.lastKey = 'l';
			break;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT) {
			enemy.rightKeyPressed = true;
			enemy.lastKey = 'r';
			break;
		}
		else if (e.key.keysym.sym == SDLK_UP) {
			enemy.upKeyPressed = true;
			enemy.lastKey = 'u';
			enemy.velocityY = -2000;
			break;
		}
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_d) {
			//player.velocityX = 0;
			player.dKeyPressed = false;
			break;
		}
		else if (e.key.keysym.sym == SDLK_a) {
			//player.velocityX = 0;
			player.aKeyPressed = false;
			break;
		}
		else if (e.key.keysym.sym == SDLK_w) {
			//player.velocityX = -200;
			player.wKeyPressed = false;
			break;
		}
		else if (e.key.keysym.sym == SDLK_LEFT) {
			enemy.leftKeyPressed = false;
			break;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT) {
			enemy.rightKeyPressed = false;
			break;
		}
		else if (e.key.keysym.sym == SDLK_UP) {
			enemy.upKeyPressed = false;
			break;
		}
	}
	
}

/*
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

	player.x += 70*delta_time;
	player.y += 70*delta_time;

	enemy.x += 70 * delta_time;
	enemy.y += 70 * delta_time;
}
*/
//Segragating PlayerUpdate and EnemyUpdate Functions
void playerUpdate() {
	//Instead of while loop burning out CPU resources, we use SDL_Delay
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	//delta time factor - factor to be used to update all my objects smoothly
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	//logic to keep a fixed timestamp/frame-rate
	last_frame_time = SDL_GetTicks();//SDL_GetTicks() returns the time passed since SDL_Init
	
	//Player moving along X axis on D KEYDOWN
	player.x += player.velocityX*delta_time;
	//Player stop at ground gravity logic
	player.y += player.velocityY*delta_time;
	if (player.y + player.height + player.velocityY*delta_time >= WINDOW_HEIGHT) {
		player.velocityY = 0;
	}
	else {
		player.velocityY += GRAVITY;
	}

	//Migrated A-KEY and D-KEY pressed logic from switch statement to here for smoother player movement
	player.velocityX = 0;
	if (player.aKeyPressed == true) {
		player.velocityX = -400;
	}
	else if (player.dKeyPressed == true) {
		player.velocityX = 400;
	}
}

void enemyUpdate() {
	//Instead of while loop burning out CPU resources, we use SDL_Delay
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	//delta time factor - factor to be used to update all my objects smoothly
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	//logic to keep a fixed timestamp/frame-rate
	last_frame_time = SDL_GetTicks();//SDL_GetTicks() returns the time passed since SDL_Init

	//enemy movement logic
	enemy.x += enemy.velocityX * delta_time;
	//Enemy stop at ground gravity logic
	enemy.y += enemy.velocityY * delta_time;
	if (enemy.y + enemy.height + enemy.velocityY * delta_time >= WINDOW_HEIGHT) {
		enemy.velocityY = 0;
	}
	else {
		enemy.velocityY += GRAVITY;
	}

	//Migrated A-KEY and D-KEY pressed logic from switch statement to here for smoother player movement
	enemy.velocityX = 0;
	if (enemy.leftKeyPressed == true) {
		enemy.velocityX = -400;
	}
	else if (enemy.rightKeyPressed == true) {
		enemy.velocityX = 400;
	}
}

void render() {
	//Setting the background color for buffer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);

	//HERE BELOW IS THE PART WHERE WE CAN START DRAWING OUR GAME OBJECTS
	//Draw a rectangle
	SDL_Rect player_rect = {(int)player.x,(int)player.y,(int)player.width,(int)player.height};
	//Before rendering the color, set the rendercolor so that it does not render as black again because we allocated background earlier black
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	//Render the rectangle
	SDL_RenderFillRect(renderer, &player_rect);

	//Draw enemy rectangle
	SDL_Rect enemy_rect = { (int)enemy.x,(int)enemy.y,(int)enemy.width,(int)enemy.height };
	//Render the rectangle
	SDL_RenderFillRect(renderer, &enemy_rect);

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
		playerUpdate();
		enemyUpdate();
		render();
	}

	destroy_window();

	return 0;
}