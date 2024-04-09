#include "Engine.h"
#include<iostream>

//Initializing Engine Instance
Engine* Engine::s_Instance = nullptr;

bool Engine::Init(){
	m_IsRunning = true;
	return m_IsRunning;
}

bool Engine::Clean() {
	return false;
}

void Engine::Quit() {

}

void Engine::Update() {
	std::cout << "Updating..." << std::endl;
}

void Engine::Render() {

}

void Engine::Events() {

}