#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(glm::vec2* vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	std::vector<glm::vec2> positions;
	positions.reserve(m_drawCount);

	for(unsigned int i = 0; i < m_drawCount; i++)
	{
		positions.push_back(vertices[i]);
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(1, &m_vertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	initializeCoords(positions);
}

void Mesh::setMesh(glm::vec2* vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	std::vector<glm::vec2> positions;
	positions.reserve(m_drawCount);

	for(unsigned int i = 0; i < m_drawCount; i++)
	{
		positions.push_back(vertices[i]);
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(1, &m_vertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	initializeCoords(positions);
}

void Mesh::initializeCoords(std::vector<glm::vec2>positions)
{
	m_center = glm::vec2(0,0);

	for(unsigned int i = 0; i < m_drawCount; i++)
	{
		m_center.x += positions[i].x;
		m_center.y += positions[i].y;
	}

	m_center.x = m_center.x / m_drawCount;
	m_center.y = m_center.y / m_drawCount;

	m_position = glm::vec2(0,0);
	m_rotationAroundCenter = 0;
}

void Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);
	
	glPushMatrix();
	
	glTranslatef(m_position.x, m_position.y, 0);

	glTranslatef(m_center.x, m_center.y, 0);
	glRotatef(m_rotationAroundCenter, 0, 0, 1);
	glTranslatef(-m_center.x, -m_center.y, 0);

	glColor3f(m_fillColor.x, m_fillColor.y, m_fillColor.z);

	glDrawArrays(GL_LINE_LOOP, 0, m_drawCount);

	glPopMatrix();

	glBindVertexArray(0);
}

void Mesh::setPosition(glm::vec2 position)
{
	m_position = position - m_center;
}

void Mesh::move(glm::vec2 movement)
{
	m_position += movement;
}

void Mesh::setRotationAroundCenter(int rotationAroundCenter)
{
	m_rotationAroundCenter = rotationAroundCenter;
}

void Mesh::setFillColor(glm::vec3 fillColor)
{
	m_fillColor = fillColor;
}

glm::vec2 Mesh::getPosition()
{
	return m_position + m_center;
}

glm::vec2 *Mesh::getPositionPointer()
{
	return &m_position;
}

int Mesh::getRotationAroundCenter()
{
	return m_rotationAroundCenter;
}

glm::vec3 Mesh::getFillColor()
{
	return m_fillColor;
}
