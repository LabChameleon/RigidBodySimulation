#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include "Mesh.h"

#include <iostream>
#include <glm\glm.hpp>
#include <GL\glew.h>

class RectangleShape : public Mesh
{
public:

	RectangleShape(glm::vec2 min, glm::vec2 max) 
	{
		m_min = min;
		m_max = max;

		m_length = m_max.x - m_min.x;
		m_height = m_max.y - m_min.y;

		glm::vec2 vertices[4] = {   m_min,
									glm::vec2(m_min.x, m_max.y),
									m_max,
									glm::vec2(m_max.x, m_min.y) };

		Mesh::setMesh(vertices, 4);
	}

	void setPosition(glm::vec2 position)
	{
		Mesh::setPosition(position);

		m_min = glm::vec2(position.x - m_length/2, position.y - m_height/2);
		m_max = glm::vec2(position.x + m_length/2, position.y + m_height/2);
	}

	void move(glm::vec2 movement)
	{
		Mesh::move(movement);

		m_min = glm::vec2(m_min.x + movement.x, m_min.y + movement.y);
		m_max = glm::vec2(m_max.x + movement.x, m_max.y + movement.y);
	}


	glm::vec2 getMin()
	{
		return m_min;
	}

	glm::vec2 getMax()
	{
		return m_max;
	}

private:
	int m_length;
	int m_height;
	glm::vec2 m_min;
	glm::vec2 m_max;

};

#endif // RECTANGLESHAPE_H