#pragma once

#ifndef TEXTUREMANAGER_H

class TextureManager
{
	public:
		static TextureManager* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new TextureManager(); };

	private:
		static TextureManager* s_Instance;
};

#endif
