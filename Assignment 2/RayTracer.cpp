/*==================================================================================
* COSC 363  Computer Graphics (2023)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* Assignment 2
* In this assignment you will implement a ray tracer that can handle different types of
* geometric objects and global illumination features, and demonstrate its capability in
* enhancing the visual realism of a rendered scene.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"
#include "Cylinder.h"
#include "Cone.h"
using namespace std;

const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

vector<SceneObject*> sceneObjects;
//TextureBMP texture;	#Butterfly Texture

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(-10, 15, -40);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found


	if (ray.index == 0)	//Floor Plane
	{
		//Stripe pattern
		int stripeWidth = 6;
		int iz = (ray.hit.z) / stripeWidth;
		int k = iz % 2; //2 colors

		int stripeHeight = 6;
		int ix = (ray.hit.x) / stripeHeight;
		int j = ix % 2; //2 colors

		if (k == 0) {
			if (j == 0) {
				if (ray.hit.x > 0) {
					color = glm::vec3(1.0, 0.86, 0.92);		//Light Pink
				}
				else {
					color = glm::vec3(1, 1, 0);		//Yellow
				}
			}
			else {
				if (ray.hit.x > 0) {
					color = glm::vec3(1, 1, 0);		//Yellow
				}
				else {
					color = glm::vec3(1.0, 0.86, 0.92);		//Light Pink
				}
			}
		}
		else { 
			if (j == 0) {
				if (ray.hit.x > 0) {
					color = glm::vec3(1, 1, 0);		//Yellow
				}
				else {
					color = glm::vec3(1.0, 0.86, 0.92);		//Light Pink
				}
			}
			else {
				if (ray.hit.x > 0) {
					color = glm::vec3(1.0, 0.86, 0.92);		//Light Pink
				}
				else {
					color = glm::vec3(1, 1, 0);		//Yellow
				}
			}
		}
		obj->setColor(color);
	}
	
	color = obj->lighting(lightPos, -ray.dir, ray.hit);

	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(sceneObjects);
	if ((shadowRay.index > -1) && (shadowRay.dist < glm::length(lightVec))) {
		color = 0.2f * obj->getColor();				//0.2 = ambient scale factor
		if (sceneObjects[shadowRay.index]->isRefractive()) {
			color = 0.4f * obj->getColor();
		}
		if (sceneObjects[shadowRay.index]->isTransparent()) {
			color = 0.5f * obj->getColor();
		}
	}

	if (obj->isReflective() && step < MAX_STEPS)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	if (obj->isRefractive() && step < MAX_STEPS)
	{
		float refrCoeff = obj->getRefractionCoeff();
		float refrIndex = obj->getRefractiveIndex();

		glm::vec3 normalVec = obj->normal(ray.hit);
		float indexRatio = 1 / refrIndex;
		glm::vec3 gVec = glm::refract(ray.dir, normalVec, indexRatio);
		Ray refrRay(ray.hit, gVec);
		refrRay.closestPt(sceneObjects);
		glm::vec3 mVec = obj->normal(refrRay.hit);
		glm::vec3 hVec = glm::refract(gVec, -(mVec), 1.0f / indexRatio);
		Ray outRay(refrRay.hit, hVec);
		color = trace(outRay, step + 1);
	}

	if (obj->isTransparent() && step < MAX_STEPS)
	{
		float rho = obj->getTransparencyCoeff();
		Ray transparentRay(ray.hit, ray.dir);
		glm::vec3 transparentColor = trace(transparentRay, step + 1);
		color = rho * transparentColor;
	}

	//-----------------------------------------
	//--Fog Calculation - Uncomment to Use it--
	//-----------------------------------------

	/*
	float fogColorValue = (((ray.hit.z) - 0) / (-300 - 0));
	glm::vec3 fogColor(1.0, 1.0, 1.0);	//white
	color = ((1 - fogColorValue) * color) + (fogColorValue * fogColor);
	*/

	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		}
	}

    glEnd();
    glFlush();
}

//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

	Plane* floorPlane = new Plane(
		glm::vec3(-40., -20, 0), //Point A
		glm::vec3(40., -20, 0), //Point B
		glm::vec3(40., -20, -300), //Point C
		glm::vec3(-40., -20, -300)); //Point D
	floorPlane->setColor(glm::vec3(1.0, 1.0, 0));	//Yellow
	floorPlane->setSpecularity(false);
	sceneObjects.push_back(floorPlane);

	Plane* leftWallPlane = new Plane(
		glm::vec3(-40., -20, 0), //Point A
		glm::vec3(-40., -20, -300), //Point B
		glm::vec3(-40., 30, -300), //Point C
		glm::vec3(-40., 30, 0)); //Point D
	leftWallPlane->setColor(glm::vec3(1.0, 0.0, 0));	//Red
	leftWallPlane->setSpecularity(false);
	sceneObjects.push_back(leftWallPlane);

	Plane* rightWallPlane = new Plane(
		glm::vec3(40., -20, 0), //Point A
		glm::vec3(40., 30, 0), //Point B
		glm::vec3(40., 30, -300), //Point C
		glm::vec3(40., -20, -300)); //Point D
	rightWallPlane->setColor(glm::vec3(0.0, 1.0, 1.0));	//Teal
	rightWallPlane->setSpecularity(false);
	sceneObjects.push_back(rightWallPlane);

	Plane* backPlane = new Plane(
		glm::vec3(-40., -20, -300), //Point A
		glm::vec3(40., -20, -300), //Point B
		glm::vec3(40., 30, -300), //Point C
		glm::vec3(-40., 30, -300)); //Point D
	backPlane->setColor(glm::vec3(0.0, 1.0, 0.0));	//Green
	backPlane->setSpecularity(false);
	sceneObjects.push_back(backPlane);

	Plane* roofPlane = new Plane(
		glm::vec3(-40., 30, 0), //Point A
		glm::vec3(-40., 30, -300), //Point B
		glm::vec3(40., 30, -300), //Point C
		glm::vec3(40., 30, 0)); //Point D
	roofPlane->setColor(glm::vec3(1.0, 1.0, 0.0));
	roofPlane->setSpecularity(false);
	sceneObjects.push_back(roofPlane);

	Plane* frontPlane = new Plane(
		glm::vec3(-40., -20, 0), //Point A
		glm::vec3(-40., 30, 0), //Point B
		glm::vec3(40., 30, 0), //Point C
		glm::vec3(-40., -20, 0)); //Point D
	frontPlane->setColor(glm::vec3(1.0, 0.0, 1.0));	
	sceneObjects.push_back(frontPlane);

	Sphere* mirrorSphere = new Sphere(glm::vec3(-14.0, -8.0, -110.0), 5);
	mirrorSphere->setColor(glm::vec3(0, 0, 1));   //Set colour to blue
	mirrorSphere->setSpecularity(false);
	mirrorSphere->setReflectivity(true, 1.0);
	sceneObjects.push_back(mirrorSphere);		 //Add sphere to scene objects
	
	Sphere	*transparentSphere = new Sphere(glm::vec3(12.0, -3.0, -80.0), 5.0);
	transparentSphere->setTransparency(true, 0.7);
	sceneObjects.push_back(transparentSphere);		 //Add sphere to scene objects
	
	Sphere* refractSphere = new Sphere(glm::vec3(0.0, -8.0, -70.0), 5.0);
	refractSphere->setRefractivity(true, 1.0, 1.4);
	sceneObjects.push_back(refractSphere);		 //Add sphere to scene objects
	
	Cylinder* cylinder1 = new Cylinder(glm::vec3(-13.0, -20.0, -90.0), 5.0, 6.0);
	cylinder1->setColor(glm::vec3(1, 0, 1));   
	sceneObjects.push_back(cylinder1);		

	Cone* cone1 = new Cone(glm::vec3(13.0, -20.0, -120.0), 5.0, 15.0);
	cone1->setColor(glm::vec3(0, 1, 1));
	sceneObjects.push_back(cone1);
	
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("COSC363 Assignment 2");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
