#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <SDL2\SDL.h>
#include <GL\glew.h>

class Display
{
public:
	Display(int width, int height, const std::string& titel);

	void Update();
	bool isClosed();
	void Clear();

	virtual ~Display();

private:
	Display(const Display& other) {}
	Display& operator=(const Display& other) {}

	SDL_Window* m_window;
	SDL_GLContext m_glContext;

	bool m_isClosed;
};

#endif //DISPLAY_H
