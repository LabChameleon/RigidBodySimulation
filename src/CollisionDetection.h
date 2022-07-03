#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "RectangleShape.h"
#include "CircleShape.h"
#include "PhysicObject.h"
#include "Mesh.h"

struct Manifold
{
	glm::vec2 *position_A;
	glm::vec2 *position_B;
	glm::vec2 *velocity_A;
	glm::vec2 *velocity_B;
	float mass_A;
	float mass_B;
	float restitution_A;
	float restitution_B;
	float p_depth;
	glm::vec2 normal;
};

class CollisionDetection
{
public:

	CollisionDetection();

	static bool checkForCollision(RectangleShape A, Attributes attributes_A, RectangleShape B, Attributes attributes_B)
	{
		glm::vec2 normal = B.getPosition() - A.getPosition();

		float a_extend = (A.getMax().x - A.getMin().x) / 2;
		float b_extend = (B.getMax().x - B.getMin().x) / 2;

		float overlap_x = a_extend + b_extend - glm::abs(normal.x);

		if(overlap_x > 0)
		{
			a_extend = (A.getMax().y - A.getMin().y) / 2;
			b_extend = (B.getMax().y - B.getMin().y) / 2;

			float overlap_y = a_extend + b_extend - glm::abs(normal.y);

			if(overlap_y > 0)
			{
				Manifold manifold;

				//std::cout << overlap_x << ", " << overlap_y << ", " << normal.x << std::endl;

				if(overlap_x > overlap_y)
				{
					if(normal.y < 0)
					{
						manifold.normal = glm::vec2(0, -1);
					}
					else
					{
						manifold.normal = glm::vec2(0, 1);
					}

					manifold.p_depth = overlap_y;
				}
				else
				{
					if(normal.x < 0)
					{
						manifold.normal = glm::vec2(-1, 0);
					}
					else
					{
						manifold.normal = glm::vec2(1, 0);
					}

					manifold.p_depth = overlap_x;
				}

				manifold.position_A = attributes_A.position;
				manifold.position_B = attributes_B.position;

				manifold.velocity_A = attributes_A.velocity;
				manifold.velocity_B = attributes_B.velocity;

				manifold.mass_A = attributes_A.mass;
				manifold.mass_B = attributes_B.mass;

				manifold.restitution_A = attributes_A.restitution;
				manifold.restitution_B = attributes_B.restitution;

				resolveCollision(manifold);

				return true;
			}

			return false;
		}

		return false;
	}

	static bool checkForCollision(CircleShape A, Attributes attributes_A, CircleShape B, Attributes attributes_B)
	{
		float radius = A.getRadius() + B.getRadius();

		glm::vec2 normal = B.getPosition() - A.getPosition();
		float d = glm::length(normal);

		if(d >= radius)
		{
			return false;
		}

		Manifold manifold;

		manifold.position_A = attributes_A.position;
		manifold.position_B = attributes_B.position;

		manifold.velocity_A = attributes_A.velocity;
		manifold.velocity_B = attributes_B.velocity;

		manifold.mass_A = attributes_A.mass;
		manifold.mass_B = attributes_B.mass;

		manifold.restitution_A = attributes_A.restitution;
		manifold.restitution_B = attributes_B.restitution;

		manifold.p_depth = radius - d;

		manifold.normal = normal / d;

		resolveCollision(manifold);

		return true;
	}

	static bool checkForCollision(RectangleShape A, Attributes attributes_A, CircleShape B, Attributes attributes_B)
	{
		glm::vec2 n = B.getPosition() - A.getPosition();

		glm::vec2 closest = n;

		float x_extent = (A.getMax().x - A.getMin().x) / 2;
		float y_extent = (A.getMax().y - A.getMin().y) / 2;

		closest.x = clamp(-x_extent, x_extent, closest.x);
		closest.y = clamp(-y_extent, y_extent, closest.y);

		bool inside = false;

		if(n == closest)
		{
			inside = true;
			
			if(glm::abs(n.x) > glm::abs(n.y))
			{
				if(closest.x > 0)
				{
					closest.x = x_extent;
				}
				else
				{
					closest.x = -x_extent;
				}
			}
			else
			{
				if(closest.y > 0)
				{
					closest.y = y_extent;
				}
				else
				{
					closest.y = -y_extent;
				}
			}
		}

		glm::vec2 normal = n - closest;
		double distance = glm::length(normal);
		double radius = B.getRadius();

		if(distance > radius && inside == false)
		{
			return false;
		}

		Manifold manifold;

		if(inside)
		{
			manifold.normal = -glm::normalize(normal);
			manifold.p_depth = radius + distance;
		}
		else
		{
			manifold.normal = glm::normalize(normal);
			manifold.p_depth = radius - distance;
		}

		manifold.position_A = attributes_A.position;
		manifold.position_B = attributes_B.position;

		manifold.velocity_A = attributes_A.velocity;
		manifold.velocity_B = attributes_B.velocity;

		manifold.mass_A = attributes_A.mass;
		manifold.mass_B = attributes_B.mass;

		manifold.restitution_A = attributes_A.restitution;
		manifold.restitution_B = attributes_B.restitution;

		resolveCollision(manifold);

		return true;
	}

	static void resolveCollision(Manifold manifold)
	{
		// Solution 1

		float e = glm::min(manifold.restitution_A, manifold.restitution_B);

		glm::vec2 velocity_A = glm::vec2(manifold.velocity_A->x, manifold.velocity_A->y);
		glm::vec2 velocity_B = glm::vec2(manifold.velocity_B->x, manifold.velocity_B->y);

		float mass_A = manifold.mass_A;
		float mass_B = manifold.mass_B;

		float v_Adot = glm::dot(velocity_A, manifold.normal);
		glm::vec2 v_Apar = v_Adot * manifold.normal;
		glm::vec2 v_Asenk = (velocity_A - v_Apar);

		float v_Bdot = glm::dot(velocity_B, manifold.normal);
		glm::vec2 v_Bpar = v_Bdot * manifold.normal;
		glm::vec2 v_Bsenk = (velocity_B - v_Bpar);

		glm::vec2 u_Apar = (2 * mass_A * v_Bpar + ((mass_B - mass_A) * v_Apar)) / (mass_A + mass_B);
		glm::vec2 u_Bpar = (2 * mass_B * v_Apar + ((mass_A - mass_B) * v_Bpar)) / (mass_A + mass_B);

		if(manifold.mass_A != 0)
		{
			manifold.velocity_A->x = (u_Apar.x + v_Asenk.x);
			manifold.velocity_A->y = (u_Apar.y + v_Asenk.y);
		}

		if(manifold.mass_B != 0)
		{
			manifold.velocity_B->x = (u_Bpar.x + v_Bsenk.x);
			manifold.velocity_B->y = (u_Bpar.y + v_Bsenk.y);
		}




		// Solution 1.5


		//float e = glm::min(manifold.restitution_A, manifold.restitution_B);

		//glm::vec2 velocity_A = glm::vec2(manifold.velocity_A->x, manifold.velocity_A->y);
		//glm::vec2 velocity_B = glm::vec2(manifold.velocity_B->x, manifold.velocity_B->y);

		//float mass_A = manifold.mass_A;
		//float mass_B = manifold.mass_B;


		//float v_Adot = glm::dot(velocity_A, manifold.normal);
		//glm::vec2 v_Apar = v_Adot * manifold.normal;
		//glm::vec2 v_Asenk = (velocity_A - v_Apar);

		//float v_dot = glm::dot(velocity_B, manifold.normal);
		//glm::vec2 v_par = v_dot * manifold.normal;
		//glm::vec2 v_senk = (velocity_B - v_par);

		//glm::vec2 u_Apar = (mass_A * v_par + mass_B * v_Apar - mass_A * (v_Apar - v_par) * 0.9f) / (mass_A + mass_B); 
		//glm::vec2 u_par = (mass_A * v_par + mass_B * v_Apar - mass_B * (v_par - v_Apar) * 0.9f) / (mass_A + mass_B);

		//if(manifold.mass_A != 0)
		//{
		//	manifold.velocity_A->x = (u_Apar.x + v_Asenk.x);
		//	manifold.velocity_A->y = (u_Apar.y + v_Asenk.y);
		//}

		//if(manifold.mass_B != 0)
		//{
		//	manifold.velocity_B->x = (u_par.x + v_senk.x);
		//	manifold.velocity_B->y = (u_par.y + v_senk.y);
		//}




		// Solution 2

		//glm::vec2 velocity_A = glm::vec2(manifold.velocity_A->x, manifold.velocity_A->y);
		//glm::vec2 velocity_B = glm::vec2(manifold.velocity_B->x, manifold.velocity_B->y);

		//float mass_A = manifold.mass_A;
		//float mass_B = manifold.mass_B;

		//glm::vec2 rv = velocity_B - velocity_A;
		//glm::vec2 normal = manifold.normal;

		//float e = glm::min(manifold.restitution_A, manifold.restitution_B);

		//glm::vec2 impuls = -(1+e) * manifold.normal * glm::dot(rv, manifold.normal);

		//float massSum = mass_A + mass_B;

		//if(mass_A != 0)
		//{
		//	manifold.velocity_A->x -= ((manifold.mass_A / massSum) * impuls.x);
		//	manifold.velocity_A->y -= ((manifold.mass_A / massSum) * impuls.y);
		//}

		//if(mass_B != 0)
		//{
		//	manifold.velocity_B->x += ((manifold.mass_B / massSum) * impuls.x);
		//	manifold.velocity_B->y += ((manifold.mass_B / massSum) * impuls.y);
		//}



		// Solution3

		//glm::vec2 velocity_A = glm::vec2(manifold.velocity_A->x, manifold.velocity_A->y);
		//glm::vec2 velocity_B = glm::vec2(manifold.velocity_B->x, manifold.velocity_B->y);

		//// Calculate relative velocity
		//glm::vec2 rv = velocity_B - velocity_A;
		//glm::vec2 normal = manifold.normal;

		//// Calculate relative velocity in terms of the normal direction
		//float velAlongNormal = glm::dot(rv, normal);
 
		//// Do not resolve if velocities are separating
		//if(velAlongNormal > 0)
		//	return;
 
		//// Calculate restitution
		//float e = glm::min(manifold.restitution_A, manifold.restitution_B);
 
		//// Calculate impulse scalar
		//float j = -(1 + e) * velAlongNormal;

		//float invMass_A;
		//float invMass_B;

		//if(manifold.mass_A != 0)
		//	invMass_A = 1/manifold.mass_A;
		//else
		//	invMass_A = 0;

		//if(manifold.mass_B != 0)
		//	invMass_B = 1/manifold.mass_B;
		//else
		//	invMass_B = 0;

		//j = j / (invMass_A + invMass_B);

		//// Apply impulse
		//glm::vec2 impulse = j * normal;
		//float massSum = manifold.mass_A + manifold.mass_B;

		//if(invMass_A != 0)
		//{
		//	manifold.velocity_A->x = velocity_A.x - ((manifold.mass_A / massSum) * impulse.x);
		//	manifold.velocity_A->y = velocity_A.y - ((manifold.mass_A / massSum) * impulse.y);
		//}

		//if(invMass_B != 0)
		//{
		//	manifold.velocity_B->x = velocity_B.x + ((manifold.mass_B / massSum) * impulse.x);
		//	manifold.velocity_B->y = velocity_B.y + ((manifold.mass_B / massSum) * impulse.y);
		//}



		positionalCorrection(manifold);
	}

	static void positionalCorrection(Manifold manifold)
	{
		const float percent = 0.2;
		const float slop = 0.1;

		float invMass_A;
		float invMass_B;

		if(manifold.mass_A != 0)
			invMass_A = 1/manifold.mass_A;
		else
		    invMass_A = 0;

		if(manifold.mass_B != 0)
		    invMass_B = 1/manifold.mass_B;
	    else
		    invMass_B = 0;

		glm::vec2 correction = manifold.p_depth / (invMass_A + invMass_B) * percent * manifold.normal;

		if(invMass_A != 0)
		{
			manifold.position_A->x -= (1/manifold.mass_A) * correction.x;
			manifold.position_A->y -= (1/manifold.mass_A) * correction.y;
		}

		if(invMass_B != 0)
		{
			manifold.position_B->x += ((1/manifold.mass_B) * correction.x);
			manifold.position_B->y += ((1/manifold.mass_B) * correction.y);
		}
	}

	static float clamp (float min, float max, float point)
	{
		if(point < min)
		{
			return min;
		}
		else if(point > max)
		{
			return max;
		}
		else
		{
			return point;
		}
	}
};

#endif // COLLISIONDETECTION