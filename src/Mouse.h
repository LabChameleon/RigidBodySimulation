#ifndef MOUSE_H
#define MOUSE_H

#include <SDL2\SDL.h>
#include <glm\glm.hpp>

class Mouse
{
public:
	Mouse() {}
	
	static glm::vec2 getMousePosition()
	{
		int x;
		int y;

		SDL_GetMouseState(&x, &y);

		return glm::vec2(x,y);
	}

	static bool isLeftButtonPressed()
	{
		if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
		{
			return true;
		}
	}

	static bool isRightButtonPressed()
	{
		if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3))
		{
			return true;
		}
	}
};

#endif // MOUSE_H