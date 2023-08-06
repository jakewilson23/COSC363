/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray.
*/
float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
    float m = ((radius * radius) / (height * height));
    glm::vec3 vertex = glm::vec3(center.x, center.y + height, center.z);
    glm::vec3 w = p0 - vertex;

    float a = glm::dot(dir, dir) - m * (dir.y * dir.y) - (dir.y * dir.y);
    float b = 2 * (glm::dot(dir, w) - m * (dir.y * w.y) - (dir.y * w.y));
    float c = glm::dot(w, w) - m * (w.y * w.y) - (w.y * w.y);

    //Discriminant = b^2-4ac
    float discriminant = (b * b) - (4 * (a * c));
    if (discriminant < 0.001)
        return -1;

    float t1 = ((-b) + sqrt(discriminant)) / (2 * a);
    float t2 = ((-b) - sqrt(discriminant)) / (2 * a);

    float r1 = p0.y + t1 * dir.y;
    float r2 = p0.y + t2 * dir.y;

    float base_y = center[1];
    float top_y = center[1] + height;

    if (r1 > center.y + height) {       //intersection above cone
        return -1;      
    }
    else if (r1 < center.y) {       //intersection below cone
        if (r2 >= center.y) {       //second intersection is inside the cone
            return t2;
        }
        else {      //both intersects miss the cone
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
* Assumption: The input point p lies on the cone.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    glm::vec3 n = glm::vec3(p.x - center.x, 0, p.z - center.z);
    n = glm::normalize(n);
    return n;
}