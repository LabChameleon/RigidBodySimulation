#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H

#include <glm\glm.hpp>
#include <iostream>

#include "RectangleShape.h"

struct Attributes
{
	glm::vec2 *position;
	glm::vec2 *velocity;
	float mass;
	float restitution;
	float p_depth;
};

template<typename T>

class PhysicObject
{
public:

	PhysicObject(T rectMesh, float mass, float restitution) : m_mesh(rectMesh)  
	{
		m_mesh.setFillColor(glm::vec3(1,0,0));
		m_gravity = 0.981;

		attributes.mass = mass;
		attributes.restitution = restitution;
	}

	void update(double elapsedTime)
	{
		if(attributes.mass != 0)
		{
			m_velocity = glm::vec2(m_velocity.x, m_velocity.y + m_gravity);
			m_mesh.move(glm::vec2(m_velocity.x * elapsedTime, m_velocity.y * elapsedTime));
		}
	}

	T getMesh()
	{
		return m_mesh;
	}

	void setPosition(glm::vec2 position)
	{
		m_mesh.setPosition(position);
	}

	void setVelocity(glm::vec2 velocity)
	{
		m_velocity = velocity;
	}

	glm::vec2 getPosition()
	{
		return m_mesh.getPosition();
	}

	glm::vec2 getVelocity()
	{
		return m_velocity;
	}

	Attributes getAttributes()
	{
		attributes.velocity = &m_velocity;
		attributes.position = m_mesh.getPositionPointer();
		return attributes;
	}

	void draw()
	{
		m_mesh.draw();
	}

private:
	T m_mesh;

	Attributes attributes;
	glm::vec2 m_velocity;
	float m_gravity;
};

#endif // PHYSIOBJECT

