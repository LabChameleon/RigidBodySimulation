#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <iostream>
#include <vector>

class Mesh 
{
public:

	Mesh();
	Mesh(glm::vec2* vertices, unsigned int numVertices);

	void draw();

	virtual void setPosition(glm::vec2 position);
	virtual void move(glm::vec2 movement);
	void setRotationAroundCenter(int rotationAroundCenter);
	void setFillColor(glm::vec3);

	glm::vec2 getPosition();
	glm::vec2 *getPositionPointer();
	int getRotationAroundCenter();
	glm::vec3 getFillColor();

protected:

	void setMesh(glm::vec2* vertices, unsigned int numVertices);

private:

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffer;
	unsigned int m_drawCount;

	void initializeCoords(std::vector<glm::vec2>positions);
	glm::vec2 m_position;
	int m_rotationAroundCenter;
	glm::vec2 m_center;
	glm::vec3 m_fillColor;
};

#endif // MESH_H
