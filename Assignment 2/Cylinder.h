/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#pragma once
#ifndef H_CYLINDER
#define H_CYLINDER
#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cylinder located at 'center'
 * with the specified radius and height
 */
class Cylinder : public SceneObject
{

private:
	glm::vec3 center = glm::vec3(0);
	float radius = 1;
	float height = 5;

public:
	Cylinder() {};  //Default constructor creates a unit cylinder

	Cylinder(glm::vec3 centre, float radius, float height) : center(centre), radius(radius), height(height) {}

	float intersect(glm::vec3 p0, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_Cylinder