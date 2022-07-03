#include "Display.h"

#include <SDL2\SDL.h>
#include <GL\glew.h>

Display::Display(int width, int height, const std::string& titel)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	glEnable(GL_MULTISAMPLE);
	
	m_window = SDL_CreateWindow(titel.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();

	if(status != GLEW_OK) 
		std::cout << "Glew failed to initialize" << std::endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);
	glOrtho(0, width, height, 0, 1, -1);

	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	m_isClosed = false;
}


Display::~Display(void)
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

bool Display::isClosed()
{
	return m_isClosed;
}

void Display::Update()
{
	SDL_GL_SwapWindow(m_window);

	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
			m_isClosed = true;
	}
}

void Display::Clear()
{
	glClearColor(0.0, 0.15f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}