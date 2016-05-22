/******************************************************************************
*CPP file fo Vector3 class
*This file creates all of the methods for the vector3 class
******************************************************************************/

//include statements
#include "vector3.h"

/******************************************************************************
*vector3::normalize function
*This function accepts nothing but returns a vector3 type
*This function normalizes a vector3
******************************************************************************/
vector3 vector3::normalize() {

	//find the magnitude of the vector
	float length = sqrt((x * x) + (y * y) + (z * z));

	//if the magnitude is small
	if ((length > -0.01) && (length < 0.01))
		return vector3(0.0, 0.0, 0.0);

	else
		return vector3((x / length), (y / length), (z / length));
}
/******************************************************************************
*vector3::add function
*This function accepts a vector3 and returns a vector3 type
*This function adds a vector3 to another vector3
******************************************************************************/
vector3 vector3::add(vector3 v) {

	//add together all like elements and return a vector3
	return vector3(x + v.x, y + v.y, z + v.z);
}
/******************************************************************************
*vector3::subtrack function
*This function accepts a vector3 and returns a vector3 type
*This function subtracks a vector3 to another vector3
******************************************************************************/
vector3 vector3::subtract(vector3 v) {

	//subtrack like elements from each vector and return a vector3
	return vector3(x - v.x, y - v.y, z - v.z);
}
/******************************************************************************
*vector3::scalar function
*This function accepts a float and returns a vector3 type
*This function performs scalar multiplication on a vector
******************************************************************************/
vector3 vector3::scalar(float f) {

	//multiply a vectors elements by float f and return the vector3
	return vector3(x * f, y * f, z * f);
}
/******************************************************************************
*vector3::dot fuction
*This function accepts a type vector3 and returns a vector3 type
*This function performs the dot product or multiplys two vectors together
******************************************************************************/
float vector3::dot(vector3 v) {

	//perfrom the dot product and return a type vector3
	return ((x * v.x) + (y * v.y) + (z * v.z));
}
/******************************************************************************
*vector3::cross function
*This function accepts a type vector3 and returns a vector3 type
*This function performs the crossproduct of two vectors
******************************************************************************/
vector3 vector3::cross(vector3 v) {

	//multiply the vectors through
	float x1 = (y * v.z) - (z * v.y);
	float y1 = (z * v.x) - (x * v.z);
	float z1 = (x * v.y) - (y * v.x);

	//return the new vector3 type
	return vector3(x1, y1, z1);
}
/******************************************************************************
*vector3::reflect function
*This function accepts a type vector3 and returns a vector3 type
*This function will Reflects a vector off the plane defined by a normal
******************************************************************************/
vector3 vector3::reflect(vector3 norm) {

	// u = v - 2 * |Inc * Norm| * Norm
	vector3 n_inc = normalize();

	//call dot product the n_inc with the norm then multiply by 2
	float   term1 = 2 * n_inc.dot(norm);

	//multiply the norm by the term1
	vector3 term2 = norm.scalar(term1);

	//return the new vector3 after subracting n_inc by term2
	return vector3(n_inc.subtract(term2));
}
/******************************************************************************
*vector3::distance function
*This function accepts a type vector3 and returns a float
*This function finds the distance from one vector to another
******************************************************************************/
float vector3::distance(vector3 v) {

	//subtract like terms from each vector
	//subtract x
	float dx = v.x - x;

	//subtract y
	float dy = v.y - y;

	//subtract z
	float dz = v.z - z;

	//return after we double,sum and then sqrt the terms. 
	return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}
//end vector3.cpp file