#include "Engine.h"
#include<iostream>


//Initializing Engine Instance
Engine* Engine::s_Instance = nullptr;

bool Engine::Init(){

	if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
		SDL_Log("Failed to initialise SDL: %s",SDL_GetError());
		return false;
	}

	//Window Initiate Code
	m_Window = SDL_CreateWindow("Soft Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,0);
	if (m_Window == nullptr) {
		SDL_Log("Failed to create window: %s",SDL_GetError());
		return false;
	}

	//Initializing the renderer with window
	m_Renderer = SDL_CreateRenderer(m_Window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	return m_IsRunning=true;
}

bool Engine::Clean() {
	return false;
}

void Engine::Quit() {
	m_IsRunning = false;
}

void Engine::Update() {
	//std::cout << "Updating..." << std::endl;
	//SDL_Log("SDL Update Log");
}

void Engine::Render() {
	SDL_SetRenderDrawColor(m_Renderer,124,218,254,255);
	SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		Quit();
		//Quit() function would set m_IsRunning = false, which would exit the m_Window screen
		break;
	}
}