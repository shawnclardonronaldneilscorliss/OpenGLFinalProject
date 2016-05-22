#ifndef _GLOBAL_HEADER
#define _GLOBAL_HEADER

#include "gl/glut.h"
#include "vector3.h"
#include <GL\freeglut.h>

//global for height and width of the intial window
int WIDTH = 600;
int HEIGHT = 600;

//global type for a 3 term int and float array
typedef GLint vertex3i[3];		//for an 3 term int array
typedef GLfloat vertex3f[3];	//for a 3 term float array

//define a quadratic pointer to be used by all circle objects
GLUquadricObj *quadratic;

//declare the lights
GLint light0;
GLint light1;

//set the position of the lights
GLfloat light_position0[4] = { -1.5f, 1.0f, 10.0f, 1.0f };	//for GL_LIGHT0
GLfloat light_position1[4] = { 0, 100.0, 2000.0, 1.0 };		//for GL_LIGHT1

//define the color of the lights
GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };		//white light color
GLfloat blue_light[] =  { 0.002f, 0.002f, .1f, 1.0f };	//mild blue light

// Create gloabl light components
GLfloat ambientLight[] =  { 0.2f, 0.2f, 0.2f, 1.0f };	//ambient
GLfloat diffuseLight[] =  { 0.8f, 0.8f, 0.8, 1.0f };	//diffuse
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };	//specular

//create a 4*3 vertex array for the background plates
//moving counterclockwise from top left
vertex3f backgrndplate[4] =
{
	{ 8.0f, -0.1f, -8.0f },  { -8.0f, -0.1f, -8.0f },
	{ -8.0f, 17.0f, -8.0f }, { 8.0f, 17.0f, -8.0f }
};

//Create a dlist for the house
const int house1 = 1;
const int tallhouse = 2;
const int road = 3;
const int backgrndPanels = 5;
const int picturePanels = 6;
const int legoPile1 = 7;

//set the max random number
int MAX_RAND = 7;
int material_MAX = 14;

//create the variable for the image
GLubyte *image;

RGBTRIPLE rgb;

GLuint texName;

//unsigned char pointer
GLubyte *l_texture;

//file header and info header 
BITMAPINFOHEADER infoheader;
BITMAPFILEHEADER fileheader; // Each BMP file begins by a 54-bytes header

//place holder for the slope normal and normal of an objects face
vector3 slopenormal(0.0, 0.0, 0.0);
vector3 normal(0.0, 0.0, 0.0);

#endif // !_GLOBAL_HEADER.H