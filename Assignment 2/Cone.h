/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#pragma once
#ifndef H_CONE
#define H_CONE
#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cone located at 'center'
 * with the specified radius and height
 */
class Cone : public SceneObject
{

private:
	glm::vec3 center = glm::vec3(0);
	float radius = 1;
	float height = 5;

public:
	Cone() {};	//Default constructor creates a unit Cone

	Cone(glm::vec3 centre, float radius, float height) : center(centre), radius(radius), height(height) {}

	float intersect(glm::vec3 p0, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CONE