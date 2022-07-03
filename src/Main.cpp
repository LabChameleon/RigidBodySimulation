#include "Display.h"

#include "Mesh.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "CollisionDetection.h"
#include "Mouse.h"
#include "PhysicObject.h"
#include "Timer.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>
#include <list>
#include <SDL2\SDL.h>

#ifdef _WIN32 
#undef main 
#endif

int main()
{
	Display display(1200, 800, "Physic Engine");

	PhysicObject<RectangleShape> object2(RectangleShape(glm::vec2(100,600), glm::vec2(1100, 700)), 0, 0.8);
	PhysicObject<CircleShape> object3(CircleShape(glm::vec2(300,300), 100), 0, 0.8);

	std::list <PhysicObject<CircleShape>> objects;

	Timer timer;
	timer.start();

	bool isButtonPressed = false;

	while(!display.isClosed())
	{
		double elapsedTime = timer.restart().count();

		object2.update(elapsedTime);
		object3.update(elapsedTime);

		if(Mouse::isLeftButtonPressed() && isButtonPressed == false)
		{
			isButtonPressed = true;
			PhysicObject<CircleShape> temp(CircleShape(Mouse::getMousePosition(), 50), 1, 0.8);
			objects.push_back(temp);
		}

		if(!Mouse::isLeftButtonPressed())
		{
			isButtonPressed = false;
		}

		for(auto it = objects.begin(); it != objects.end();)
		{
			if(it->getPosition().y > 900)
			{
				it = objects.erase(it);
			}
			else
			{
				CollisionDetection::checkForCollision(object2.getMesh(), object2.getAttributes(), it->getMesh(), it->getAttributes());
				CollisionDetection::checkForCollision(object3.getMesh(), object3.getAttributes(), it->getMesh(), it->getAttributes());

				for(auto it2 = objects.begin(); it2 != objects.end();)
				{
					if(it2 == it && it2 == --objects.end())
					{
						break;
					}

					if(it2 == it && it2 != objects.end())
					{
						it2++;
					}

					CollisionDetection::checkForCollision(it2->getMesh(), it2->getAttributes(), it->getMesh(), it->getAttributes());

					it2++;
				}

				it->update(elapsedTime);
				it++;
			}
		}

		display.Clear();

		for(auto it = objects.begin(); it != objects.end(); it++)
		{
			it->draw();
		}

		object2.draw();
		object3.draw();

		display.Update();
	}

	return 0;
}