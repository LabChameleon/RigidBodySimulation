#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H

#include "Mesh.h"

class CircleShape : public Mesh
{
public:
	CircleShape(glm::vec2 center, int radius)
	{
		glm::vec2 vertices[50];
		m_radius = radius;

		for(int i = 0; i < 50; i++) 
		{ 
			float theta = 2.0f * 3.1415926f * float(i) / float(50);

			float x = m_radius * cosf(theta);
			float y = m_radius * sinf(theta);

			vertices[i] = glm::vec2(x+center.x, y+center.y);
		} 

		this->setMesh(vertices, 50);
	}

	int getRadius()
	{
		return m_radius;
	}

private:

	int m_radius;
};

#endif // CIRCLESHAPE_H