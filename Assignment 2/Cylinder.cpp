/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray.
*/
float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    
    //Quadratic Equation x = (-b +- sqrt(b^2 - 4ac)) / 2a
    float a = ((dir.x * dir.x) + (dir.z * dir.z));
    float b = 2 * (dir.x * (p0.x - center.x) + dir.z * (p0.z - center.z));
    float c = (((p0.x - center.x) * (p0.x - center.x)) + ((p0.z - center.z) * (p0.z - center.z)) - (radius * radius));
    
    //Discriminant = b^2-4ac
    float discriminant = (b * b) - (4 * (a * c));
    if (discriminant < 0.001) {
        return -1;
    }

    float t1 = ((-b) + sqrt(discriminant)) / (2 * a);
    float t2 = ((-b) - sqrt(discriminant)) / (2 * a);

    float r1 = p0.y + t1 * dir.y;
    float r2 = p0.y + t2 * dir.y;

    if (r1 > center.y + height) {       //intersection above cylinder
        if (r2 <= center.y + height) {      //second intersection is inside the cylinder
            return t2;
        } 
        else {      //both intersects miss the cylinder
            return -1;
        }
    }
    else if (r1 < center.y) {       //intersection below cylinder
        if (r2 >= center.y) {       //second intersection is inside the cylinder
            return t2;
        } 
        else {      //both intersects miss the cylinder
            return -1;
        }
    } 
    else {
        if (t1 < t2) {  
            return t1;
        }
        else {
            return t2;
        }
    }
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the Cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    if (p.y > center.y + height) {    
        return glm::vec3(0, 1, 0);      //y = 1 is the top cap
    }
    else if (p.y <= center.y) {
        return glm::vec3(0, -1, 0);     //y = -1 is the bottom cap
    }
    else {
        glm::vec3 n = glm::vec3(p.x - center.x, 0, p.z - center.z);
        n = glm::normalize(n);
        return n;
    }
}