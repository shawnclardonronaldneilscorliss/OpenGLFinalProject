/******************************************************************************
*Project Name: COMP 390 Final Project
*Author:Shawn Corliss, 2920939
*Date: May 20th 2016
*Purpose:The purpose of this application is to showcase my slills using openGL.
*Features:This applcation features
* - Multiple lights
* - a Complete lego scene complete with roads and houses
* - projection and distance using atmosphereic effects(fog blending)
* - both wood and picture textures
* - dynamic surface normal mapping
* - alpha transparency and blending
* - back face culling
* - Material libary that stores ambient, diffuse, specular and shinneyness.
*The best fit would be for option 1 for comp 390's final project as there are 
*no reflections but as the surfaces normals are being calcuated the 
*objects themselves are colored and shaded via thier orientation to the light
*and viewers perspective.
*
*Architechture: 32bit
*
******************************************************************************/
//include statements
#include "Materials.h" 
#include "Global_Header.h"
#include <time.h> 

/******************************************************************************
*crossProduct function
*The crossproduct function is used to find the cross product of two vectors
*This function takes two vector3s and and returns a vector3
******************************************************************************/
vector3 crossProduct(vector3 v1, vector3 v2){

	//define and set the vector3 cross
	vector3 cross = { v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x };

	//retrun
	return cross;
}//end of crossproduct
/******************************************************************************
*getSurfaceNormal function
*This function is used to calculate the surface normal of 3 vector3s
*This function takes 3 vector3s and and returns a vector3
*This function is created so that we dont have to call it from a vector member
******************************************************************************/
vector3 getSurfaceNormal(vector3 v1, vector3 v2, vector3 v3)
{
	//subsract vector3 1 from vector3 2 
	vector3 polyVector1 = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
	
	//subsract vector3 1 from vector3 3
	vector3 polyVector2 = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };

	//find the cross product between the two new vectors
	vector3 cross = crossProduct(polyVector1, polyVector2);

	//normalize the vector3
	 cross =cross.normalize();

	 //return the normalized poly vector3
	 return cross;
}//end of getSurfaceNormal
/******************************************************************************
*loadImage function
*This function accepts and returns nothing
*The load image function isused to to load an image to be used as a texture
******************************************************************************/
void loadImage(char *file)
{
	//variable declerations
	//create varibles used to move through the file
	int i, j = 0;

	//point to the file type itself
	FILE *l_file;

	//create the filename object and the set it to the file name
	const char * filename = file;

	//open the image file as a binary file using fileopen_secure variant
	fopen_s(&l_file, filename, "rb");

	//if the file dosent exist lets stop working
	if (l_file == NULL)return;

	// read file header and header info
	fread(&fileheader, sizeof(fileheader), 1, l_file);
	fseek(l_file, sizeof(fileheader), SEEK_SET);
	fread(&infoheader, sizeof(infoheader), 1, l_file);

	// allocate space for the image file
	l_texture = (GLubyte *)malloc(infoheader.biWidth * infoheader.biHeight * 4);

	memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

	// read da data
	j = 0;
	for (i = 0; i < infoheader.biWidth*infoheader.biHeight; i++)
	{
		fread(&rgb, sizeof(rgb), 1, l_file);

		l_texture[j + 0] = (GLubyte)rgb.rgbtRed; // Red component
		l_texture[j + 1] = (GLubyte)rgb.rgbtGreen; // Green component
		l_texture[j + 2] = (GLubyte)rgb.rgbtBlue; // Blue component
		l_texture[j + 3] = (GLubyte)255; // Alpha value

		j += 4; // Go to the next position
	}
	fclose(l_file); // Closes the file stream

}
/******************************************************************************
*drawslope function
*This function returns nothing and accepts a GLfloat
*This function is used to draw a slope lego peice.
*The input parameter for this function is used to specify width of the shape
******************************************************************************/
void drawslope(GLfloat width)
{
	//access the model view matrix
	glPushMatrix();

	//start drawing triangles
	glBegin(GL_TRIANGLES);
		
	//top face-----------------------------------------------------------------
		//calculate the slope normal
		slopenormal = getSurfaceNormal(
			vector3(1.0f, 1.0f, -1.0f), 
			vector3(-width, 1.0f, -1.0f), 
			vector3(-width, 1.0f, 1.0f));

		//set the top faces normal
		glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);
		
		//define the top face via triangles
		glVertex3f(1.0f, 1.0f, -1.0f);//top right
		glVertex3f(-width, 1.0f, -1.0f);//top left
		glVertex3f(-width, 1.0f, 0.0f);//top triangle bottom left
		glVertex3f(1.0f, 1.0f, -1.0f);//top right
		glVertex3f(-width, 1.0f, 0.0f);//bottom triangle bottom left 
		glVertex3f(1.0f, 1.0f, 0.0f);//bottom triangle bottom right 

		//front slope face-----------------------------------------------------
		//calculate the slope normal
		slopenormal = getSurfaceNormal(
			vector3(-width, 1.0f, 1.0f), 
			vector3(1.0f, -1.0f, 2.0f), 
			vector3(1.0f, 1.0f, 1.0f)
			);

		//set the front faces normal
		glNormal3f(slopenormal.x,slopenormal.y,slopenormal.z);

		//define the front face via triangles
		glVertex3f(-width, 1.0f, 0.0f);//top triagnle top left
		glVertex3f(1.0f, -1.0f, 2.0f);//top trianlge bottom right
		glVertex3f(1.0f, 1.0f, 0.0f);//top triangle top right
		glVertex3f(1.0f, -1.0f, 2.0f);
		glVertex3f(-width, 1.0f, 0.0f);
		glVertex3f(-width, -1.0f, 2.0f);

		//front bottom face----------------------------------------------------
		//calculate the slope normal
		slopenormal = getSurfaceNormal(
			vector3(-width, -1.5f, 2.0f), 
			vector3(-width, -1.0f, 2.0f), 
			vector3(1.0f, -1.0f, 2.0f)
			);
		//set the front bottoms faces normal
		glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

		//define the front bottom face via triangles
		glVertex3f(-width, -1.5f, 2.0f);//bottom left
		glVertex3f(-width, -1.0f, 2.0f);//top left
		glVertex3f(1.0f, -1.0f, 2.0f);//right
		glVertex3f(-width, -1.5f, 2.0f);//bottom left
		glVertex3f(1.0f, -1.0f, 2.0f);//right
		glVertex3f(1.0f, -1.5f, 2.0f);//bottom right
		
		//right face-----------------------------------------------------------
		//calculate the slope normal
		slopenormal = getSurfaceNormal(
			vector3(1.0f, -1.0f, -1.0f), 
			vector3(1.0f, 1.0f, -1.0f), 
			vector3(1.0f, -1.5f, 2.0f)
			);
		//set the right faces normal
		glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);
	
		//define the right face via triangles
		glVertex3f(1.0f, -1.0f, -1.0f);//bottom left
		glVertex3f(1.0f, 1.0f, -1.0f);//topleft
		glVertex3f(1.0f, 1.0f, 0.0f);//top right
		glVertex3f(1.0f, -1.0f, -1.0f);//bottom left
		glVertex3f(1.0f, 1.0f, 0.0f);//top right
		glVertex3f(1.0f, -1.0f, 2.0f);//bottom right
		glVertex3f(1.0f, -1.0f, -1.0f);//bottom left
		glVertex3f(1.0f, -1.0f, 2.0f);
		glVertex3f(1.0f, -1.5f, -1.0f);//bottom right

		glVertex3f(1.0f, -1.5f, -1.0f);//bottom right
		glVertex3f(1.0f, -1.0f, 2.0f);
		glVertex3f(1.0f, -1.5f, 2.0f);//bottom left
		
		//left face------------------------------------------------------------
		//calculate the slope normal
		slopenormal = getSurfaceNormal(
			vector3(-width, -1.0f, -1.0f), 
			vector3(-width, 1.0f, -1.0f), 
			vector3(-width, 1.0f, 1.0f)
			);
		//set the left faces normal
		glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);
		
		//define the left face via triangles
		glVertex3f(-width, -1.0f, -1.0f);//bottom left
		glVertex3f(-width, 1.0f, -1.0f);//topleft
		glVertex3f(-width, 1.0f, 0.0f);//top right
		glVertex3f(-width, -1.0f, -1.0f);//bottom left
		glVertex3f(-width, 1.0f, 0.0f);//top right
		glVertex3f(-width, -1.0f, 2.0f);//bottom right
		glVertex3f(-width, -1.0f, -1.0f);//bottom left
		glVertex3f(-width, -1.0f, 2.0f);
		glVertex3f(-width, -1.5f, -1.0f);//bottom right
		
		glVertex3f(-width, -1.5f, -1.0f);//bottom right
		glVertex3f(-width, -1.0f, 2.0f);
		glVertex3f(-width, -1.5f, 2.0f);//bottom left

		//back face------------------------------------------------------------
		//calculate the slope normal
		slopenormal = getSurfaceNormal(
			vector3(1.0f, 1.0f, -1.0f), 
			vector3(1.0f, -1.0f, -1.0f), 
			vector3(-width, -1.0f, -1.0f)
			);
		//set the back faces normal
		glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

		//define the backface face via triangles
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.5f, -1.0f);//bottom triangle bottom left
		glVertex3f(-width, -1.5f, -1.0f);//bottom triagle bottom right 
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-width, -1.5f, -1.0f);//Top triangle bottom right
		glVertex3f(-width, 1.0f, -1.0f);//top right

	glEnd();//end drawing triangles
	glPopMatrix();//close modelview matrix

}//end of the slope function
/******************************************************************************
*drawRectanlge function
*This function returns nothing and accepts a type GLfloat for:
*the width, length and depth settings of a square or rectangle
*Although this applications may appaer to be comprised of squares each peice
*of lego is quite complex. Each face of the lego is comprised of at least
*two triangles. The plane equasion is then ran on 3 of the vertices that make
*up the squares face to create the normal of the face which is then used
*to illuminate and color the surface.
******************************************************************************/
void drawrectangle(GLfloat width, GLfloat length,GLfloat depth)
{
	//access the model view matrix
	glPushMatrix();
	
	//define the rectanlges with triangles
	glBegin(GL_TRIANGLES);

	//top face-----------------------------------------------------------------
	//calculate the slope normal
	slopenormal = getSurfaceNormal(
		vector3(1.0f, 1.0f, -length),
		vector3(-width, 1.0f, -length),
		vector3(-width, 1.0f, 1.0f)
		);

	//set the normal for the front face we just calculated
	glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

	//render the front face
	glVertex3f(1.0f, 1.0f, -length);
	glVertex3f(-width, 1.0f, -length);
	glVertex3f(-width, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -length);
	glVertex3f(-width, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//front face --stays the same regardless of length
	//calculate the slope normal
	slopenormal = getSurfaceNormal(
		vector3(1.0f, -depth, 1.0f),
		vector3(1.0f, 1.0f, 1.0f),
		vector3(-width, 1.0f, 1.0f)
		);
	//set the normal for the front face we just calculated
	glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);
	
	//render the front face based on the negated width, depth and true left
	glVertex3f(1.0f, -depth, 1.0f);//top triangle bottom right
	glVertex3f(1.0f, 1.0f, 1.0);//top triangle top left
	glVertex3f(-width, 1.0f, 1.0f);//top triangle top right
	glVertex3f(1.0f, -depth, 1.0);
	glVertex3f(-width, 1.0f, 1.0f);
	glVertex3f(-width, -depth, 1.0f);


	//left face changes based the height and length----------------------------
	//calculate the slope normal
	slopenormal = getSurfaceNormal(
		vector3(-width, -depth, -length),
		vector3(-width, 1.0f, -length),
		vector3(-width, 1.0f, 1.0f)
		);
	//set the normal for the left face we just calculated
	glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

	//render the left face based on negated width, depth and length
	glVertex3f(-width, -depth, -length);
	glVertex3f(-width, 1.0f, -length);
	glVertex3f(-width, 1.0f, 1.0f);
	glVertex3f(-width, -depth, -length);
	glVertex3f(-width, 1.0f, 1.0f);
	glVertex3f(-width, -depth, 1.0f);
	
	//back face --stays the same regardless of length--------------------------
	//calculate the slope normal
	slopenormal = getSurfaceNormal(
		vector3(1.0f, -depth, -length),
		vector3(1.0f, 1.0f, -length),
		vector3(-width, 1.0f, -length)
		);

	//set the normal for the back face we just calculated
	glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

	//render the back face based on negated width, depth and length
	glVertex3f(1.0f, -depth, -length);
	glVertex3f(1.0f, 1.0f, -length);
	glVertex3f(-width, 1.0f, -length);
	glVertex3f(1.0f, -depth, -length);
	glVertex3f(-width, 1.0f, -length);
	glVertex3f(-width, -depth, -length);
	
	//right face---------------------------------------------------------------
	//calculate the slope normal
	slopenormal = getSurfaceNormal(
		vector3(1.0f, -depth, -length),
		vector3(1.0f, 1.0f, -length),
		vector3(1.0f, 1.0f, 1.0f)
		);

	//set the normal for the right face we just calculated
	glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

	//render the right face of the rectangle
	glVertex3f(1.0f, -depth, -length);
	glVertex3f(1.0f, 1.0f, -length);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -depth, -length);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -depth, 1.0f);

	glEnd();//stop rendering the triangles

	glPopMatrix();//stop accessing the modelview matrix
}//end of rectangle function
/******************************************************************************
*drawdisk function
*This function accepts a GLdouble and returns nothing
*Thie function is used to draw a disk based on a provided radius
******************************************************************************/
void drawdisk(GLdouble radius)
{
	//use the quadratic global pointer and put init a new glquadric
	quadratic = gluNewQuadric();
	
	//call gluDisk with the quadratic and the radius of the disk
	gluDisk(quadratic, 0, radius, 36, 36);

}//end drawdisk
/******************************************************************************
*drawPartialDisk function
*This function accepts 2 GLDoubles, which are for the radius and hole size 
*This function returns nothing
******************************************************************************/
void drawpartialdisk(GLdouble radius,GLdouble hole)
{
	//use the quadratic global pointer and put init a new glquadric
	quadratic = gluNewQuadric();

	//call glupartialDisk with the quadratic, the hole and radius
	gluPartialDisk(quadratic, hole, radius, 36, 36, 0, 360);

}//end partialdisk
/******************************************************************************
*drawclosedcylinder function
*This function accepts a GLdouble and a GLFloat
*This function returns nothing
*drawclosed cylinder renders a cylinder with a disk to cover the opening
******************************************************************************/
void drawCylinder(GLdouble radius,GLfloat height,bool isclosed)
{
	//use the quadratic global pointer and put init a new glquadric
	quadratic = gluNewQuadric();

	//call glucylinder with the quadratic, and even space from top to bottom
	//and a height parameter
	gluCylinder(quadratic, radius, radius,height, 18, 18);
	
	//we are looking for a closed cylinder call the below
	if (isclosed)
	{
		glPushMatrix();
			//we need to translate the cylinder on its height to lower it below the 
			//disk that we are going to close it with
			glTranslatef(0,0,height);
	
			//place a call to drawdisk function
			//with the provided radius of the cylinder
			drawdisk(radius);
		glPopMatrix();
	}

}//end of drawclosedcylinder
/******************************************************************************
*addnipples function
*This function accepts 2 GLFloats and a float and returns nothing
*addnipples is used to add the cylinders to all of the lego objects
******************************************************************************/
void addnipples(GLfloat width, GLfloat length, float startingpoint)
{
	//locals used to track the position of the nipples
	//we need to have a starting point and .55 is the move distance
	//for each width on the top of the object
	float lengthnipples = startingpoint;
	float currentwidth = 0.55;

	//create a nesetd for loop to run trough the width and heigt
	//enter on the width
	for (int x = -1; x < width; x++)
	{
		//for each width fill in the length
		for (int i = -1; i < length; i++)
		{
			//access the modelview matrix
			glPushMatrix();
				
				//translate the model 
				glTranslated(currentwidth,.6, lengthnipples);
				
				//rotate 90 degrees to see the top of the cylinder
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
				
				//call the draw closed cylinder function
				drawCylinder(0.4, 0.2f,true);

			//relase the modelview matrix
			glPopMatrix();

			//decrement the nipple position by 1
			lengthnipples = lengthnipples - 1;

		}//end of length loop

		//reset the length loop back to the statring position
		lengthnipples = startingpoint;
		//decrement the width position to -1.05
		currentwidth -= 1.05;
		
		//if the current width placment overflows the peice leave
		if (currentwidth*-1 >= width)
			return;

	}//end of width loop

}//end of drawnipples function
/******************************************************************************
*addopennipples function
*This function differs as it is only used when drawing small faces
*This function accepts 2 GLflots and two floats
*calling this function we need to specify the starting position on x and y
******************************************************************************/
void addopennipples(GLfloat width, GLfloat length, float startingpointx,float startingpointy)
{
	//set the starting potision for x and y to draw the nipples on the face
	float lengthnipples = startingpointx;
	float currentwidth = startingpointy;

	//create a nesetd for loop to run trough the width and heigt
	//enter on the width
	for (int x = -1; x < width; x++)
	{
		//for each width fill in the length
		for (int i = -1; i < length; i++)
		{

			//access the model view matrix
			glPushMatrix();

			//translate the model
			glTranslated(currentwidth/2, .6, lengthnipples/2);
			
			//rotate the object
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

			//draw an open cylinder
			drawCylinder(0.3, 0.2f,false);

			//release the modelview matrix
			glPopMatrix();

			//decrement the nipple position by 1
			lengthnipples = lengthnipples - 1;

		}//end of length loop

		//reset the length loop back to the statring position
		lengthnipples = startingpointy;
		//decrement the width position to -1.05
		currentwidth -= 1.05;

	}//end of width loop

}//end of drawopennippels
/******************************************************************************
*drawslopeLego function
*This function accepts a GLFLoat for the lego's width and returns nothing
*This function is used to call the drawslope function, Helper function
******************************************************************************/
void drawslopeLego(GLfloat width)
{
	//access the modelview matrix
	glPushMatrix();

		//make a call the draw slope function
		drawslope(width);

	//relase the modelview matrix
	glPopMatrix();

	//access the modelview matrix
	glPushMatrix();
		
	//we must first translate the nipples
		glTranslatef(0.0f, 0.4f, 0.0f);
		
		//make the call to addnipples
		addnipples(width, 0.0f,-.5);

	//release the modelview matrix
	glPopMatrix();

}//end of drawslopeLego function
/******************************************************************************
*drawrectangleLego function
*This funcion accepts 3 GLfloats for width, length and depth of a polygon
*drawrectangleLego is a helper function to call drawrectanlge and addnipples
******************************************************************************/
void drawrectangleLego(GLfloat width, GLfloat length,GLfloat depth)
{
	//access the modelview matrix
	glPushMatrix();
		//call drawrectangle funcion
		drawrectangle(width, length,depth);
	glPopMatrix();//release the modelview matrix
	
	glPushMatrix();
		//translte the modelview matrix to put the nipples on the plane
		glTranslatef(0.0f, 0.4f, 0.0f);
		//call to add nipples function
		addnipples(width, length, .5);
	glPopMatrix();
}//end of draw rectangle Lego function
/******************************************************************************
*drawcanister function
*This function accepts and returns nothing
*drawcanister is used to draw a lego canister(3 nested cylinders)
******************************************************************************/
void drawcanister(void)
{
	glPushMatrix();
		//translate the object
		glTranslatef(0.0f, 0.0f, 1.5f);
		//call to draw an open cylinder
		drawCylinder(0.7, 0.3f,false);
	glPopMatrix();
	
	glPushMatrix();
		//translate the object
		glTranslatef(0, 0, 1.5);
		//draw a parital disk
		drawpartialdisk(1.0, .5);
	glPopMatrix();

	//draw the main body of the canister
	glPushMatrix();
		//call the draw the cylinder
		drawCylinder(1.0, 1.5,false);
	glPopMatrix();

	//draw the top to close the canister
	glPushMatrix();
		//call to draw a partial disk
		drawpartialdisk(1.0, .3);
	glPopMatrix();

	//draw the top canister
	glPushMatrix();
		//translate
		glTranslatef(0.0f, 0.0f, -0.2f);
		//draw an open cylinder
		drawCylinder(0.5, 0.3f,false);
	glPopMatrix();

}//end of drawcanister
/******************************************************************************
*drawjumper function
*This function takes 2 GLfloats for both the length and width of the plane
******************************************************************************/
void drawjumper(GLfloat length, GLfloat width)
{
	//variables used to call the addopennipples function
	GLfloat startingx = 0;
	GLfloat startingy = 0;

	glPushMatrix();
		//call to draw rectangle with the lenght and width
		//this is a super flat and short object set depth to -.5
		drawrectangle(width, length,-.5f);
	glPopMatrix();
	glPushMatrix();
	//translte the modelview matrix to put the nipples on the plane
	glTranslatef(0.0f, 0.4f, 0.0f);

	//call to add the nipples to the jumper
	addopennipples(0.0,0.0,1-length,1-width);
	glPopMatrix();
}//end of draw jumper function
/******************************************************************************
*drawRoundPlate function
*This function accepts nothing and returns nothing
*this funcion has no parameters as we are creating a very specific shape
* a round plate in the Lego world is a an opencylinder + a disk + a closed 
*cylinder
******************************************************************************/
void drawRoundPlate(void)
{
	//place a call to draw a cylinder
	drawCylinder(.5f, 0.4f, false);
		
	glPushMatrix();
		//draw the lower and upper rings by drawing a closed cylinder
		drawCylinder(.6f, 0.1f,true);
	glPopMatrix();
		
	glPushMatrix();
		//translate the next cylinder	
		glTranslatef(0.0f, 0.0f, -0.2f);
		//place a call to draw an open cylinder
		drawCylinder(.3f, 0.2f,true);
	glPopMatrix();

}//end of drawroundplate
/******************************************************************************
*drawdish function
*This function returns nothing but accepts 3 GLfloat objects
*This function is used to create the dishes that are on top of the lights
*The dish is another Legoshape is well known and comes in every box
******************************************************************************/
void drawdish(GLfloat dishbase, GLfloat dishtop,GLfloat height)
{
	//set the global quadratic to a new glquadratic
	quadratic = gluNewQuadric();

	glPushMatrix();
		//translate the modelview matrix
		glTranslated(0,0, 0);
		//rotate the model
		glRotatef(-90.0, 1, 0, 0);
		//place a call to the glucylinder function
		gluCylinder(quadratic, dishbase, dishtop, height, 32, 32);
	glPopMatrix();
	
	//place a call to add nipples function
	addnipples(1.0, 1.0, .5);

}//end of drawdish function
/******************************************************************************
*drawglass function
*The drawglass function takes 2 GLfloats and returns nothing
*drawglass creates the glass used on the houses and is seethrough
******************************************************************************/
void drawglass(GLfloat height, GLfloat width)
{
	//access the modelview matrix
	glPushMatrix();

	//begin defining triangles
	glBegin(GL_TRIANGLES);
	
		//set the material for the glass (ambient, diffuse, specular,shininess)
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, clearGlass_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, clearGlass_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, clearGlass_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, clearGlassshine);
	
		//create the normal for the glass
			//front face --stays the same regardless of length
			slopenormal = getSurfaceNormal(
			vector3(1.0f, -1.0f, 1.0f),
			vector3(1.0f, 1.0f, 1.0f),
			vector3(-width, 1.0f, 1.0f)
			);

		//set the normal of the object based on the above calculation
		glNormal3f(slopenormal.x, slopenormal.y, slopenormal.z);

		//define the glass as a flat plane using two triangles
		glVertex3f(1.0f, -height, 1.0f);//top triangle bottom right
		glVertex3f(1.0f, 1.0f, 1.0f);//top triangle top right
		glVertex3f(-width, 1.0f, 1.0f);//top triangle top left
		glVertex3f(1.0f, -height, 1.0f);
		glVertex3f(-width, 1.0f, 1.0f);
		glVertex3f(-width, -height, 1.0f);
	
	glEnd();//stop rendering

	//stop accessing the modelveiw matirx
	glPopMatrix();

}//end of drawglass function
/******************************************************************************
*drawDoor function
*This function accepts and returns nothing
*The drawdoor function creates a seethrough door for the houses
******************************************************************************/
void drawDoor(void)
{
	glPushMatrix();
		//rotate the model negative 90 on the x axis
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		//draw a closed cylinder
		drawCylinder(.5, 4,true);
	glPopMatrix();
	

	glPushMatrix();
		//rotate the model negative 90 on the y axis
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		//move the model
		glTranslatef(-.5, 3, -1);
		//draw a flat faced rectangle
		drawrectangle(-1, 2, 0);
	glPopMatrix();

	glPushMatrix();
		//rotate the model negative 90 on the y axis
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		//move the obejct
		glTranslatef(-.5, 0, -1);
		//draw a flat faced rectangle
		drawrectangle(-1, 2, 0);
	glPopMatrix();

	glPushMatrix();
		//move the model
		glTranslatef(2.2f, 2.2f, -1.0f);
		//draw a square pane of glass
		drawglass(2, 2);
	glPopMatrix();

	//set the color/material of the object we want to draw 
	//We must recall the color after calling the window as its call alters 
	//the material we currntly have set
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greyplastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greyplastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greyplastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greyplasticshine);

	//draw the door knob
	glPushMatrix();
		//place the door knob on the rightside of the door
		glTranslatef(2.5, 2, .0);
		//draw the knob with a sphere that is cut in half from the doors glass 
		glutSolidSphere(.2, 64, 10);
	glPopMatrix();

}//end of drawDoor function
/******************************************************************************
*createInitalSceneDlist function
*This function is used to compile the inital primitive rectangle for ground
*Note: the * = required coordiates of primitives from assignment
*This function accepts and returns nothing
******************************************************************************/
void createHouse1Dlist(void)
{
	//create a new dlist and set it to compile
	glNewList(house1, GL_COMPILE);

	//access the modelview matrix
	glPushMatrix();

	//set the color/material of the object we want to draw 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grass_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grass_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grass_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, grassshine);

		//draw a flat rectangle for the base of the scene
		glTranslated(8, -1, 6);
		drawrectangleLego(20.0, 20.0, 0);
	glPopMatrix();

	glPushMatrix();
	//set the color/material of the object we want to draw 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greyplastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greyplastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greyplastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greyplasticshine);

	//draw 3 cement pads that are coming off the house
		//translate
		glTranslated(4.5, -.5, 3);
		drawjumper(1, 1); //call to jumper
	glPopMatrix();

	glPushMatrix();
		//translate
		glTranslated(4.5, -.5, 5);
		drawjumper(1, 1);//call to jumper
	glPopMatrix();

	glPushMatrix();
		//translate
		glTranslated(2.4, -.5, 5);
		drawjumper(1, 1);//call to jumper
	glPopMatrix();

	//draw the foundation of the house
	//frontside
	//set the material for the foundation to be a light grey color
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whiteplastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteplastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteplastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, whiteplasticshine);
		
	//frontside
	glPushMatrix();
		//translate
		glTranslated(7, 1, 1);
		drawrectangleLego(11.5, 1.0, 1.0);
	glPopMatrix();

	//leftside
	glPushMatrix();
		glTranslated(7, 1, 1);
		drawrectangleLego(1.0, 11.0, 1.0);
	glPopMatrix();

	//rightisde
	glPushMatrix();
		glTranslated(-5.5, 1, 1);
		drawrectangleLego(1.0, 11.0, 1.0);
	glPopMatrix();

	//backside of house
	//draw the foundation of the house
	glPushMatrix();
		glTranslated(5, 1, -9);
		drawrectangleLego(11.5, 1.0, 1.0);
	glPopMatrix();

	//set the material to red plastic
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticshine);
	
	glPushMatrix();
		//set the material for the foundation to be a light grey color
		//front right wall
		glTranslated(6, 4, -9);
		drawrectangleLego(11.5, 0.0, 2.0);
	glPopMatrix();

	//front right door jam
	glPushMatrix();
		glTranslated(6, 4, 0);
		drawrectangleLego(2, 0.0, 2.0);
	glPopMatrix();

	//front left wall
	glPushMatrix();
		glTranslated(-.5, 4, 0);
		drawrectangleLego(4, 0.0, 2.0);
	glPopMatrix();
	
	//front left wall door jam
	glPushMatrix();
		glTranslated(-.3, 4, 1);
		drawrectangleLego(0, 1.0, 2.0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(4, 4, 1);
		drawrectangleLego(0, 1.0, 2.0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(6, 4, 0);
		drawrectangleLego(0.0, 8, 2.0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-5.5, 4, 0);
		drawrectangleLego(0.0, 8, 2.0);
	glPopMatrix();

	//set the material to brown
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, brown_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brown_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, brown_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brownshine);
	
	glPushMatrix();
	//front right wall
		glTranslated(6, 5, -9);
		drawrectangleLego(11.5, 0.0, 0.0);
	glPopMatrix();

	//front right door jam
	glPushMatrix();
		glTranslated(6, 5, 0);
		drawrectangleLego(2, 0.0, 0.0);
	glPopMatrix();

	//front left wall
	glPushMatrix();
		glTranslated(-.5, 5, 0);
		drawrectangleLego(4, 0.0, .0);
	glPopMatrix();
	
	//front left wall door jam
	glPushMatrix();
		glTranslated(-.3, 5, 1);
		drawrectangleLego(0, 1.0, 0.0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(4, 5, 1);
		drawrectangleLego(0, 1.0, 0.0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(6, 5, 0);
		drawrectangleLego(0.0, 8, 0.0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-5.5, 5, 0);
		drawrectangleLego(0.0, 8, 0.0);
	glPopMatrix();

	//set material to red plastic
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greyplasticshine);


	glPushMatrix();
		glTranslated(6, 5, 0);
		drawrectangleLego(0.0, 8, 0.0);
	glPopMatrix();


	
	//set the color/material of the object we want to draw to gray plastic 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greyplastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greyplastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greyplastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greyplasticshine);
	
	glPushMatrix();
		//draw the door
		glTranslated(1.0, 2, 1.5);
		drawDoor();
	glPopMatrix();

	glPushMatrix();
		//draw the glass for the top of the house left front
		glTranslated(-5.5, 9, -7.0);
		glRotated(90, 0, 1, 0);
		drawglass(3, 7);
	glPopMatrix();

	glPushMatrix();
		//draw the glass for the backside of the house
		glTranslated(5.0, 9, -9.0);
		drawglass(3, 9.5);
		glPopMatrix();

	//draw the front pane of glass
	glPushMatrix();
		glTranslated(5.0, 9, -1.0);
		drawglass(3, 9.5);
	glPopMatrix();

	//draw the right side pane of glass
	glPushMatrix();
		glTranslated(5, 9, -7.0);
		glRotated(90, 0, 1, 0);
		drawglass(3, 7);
	glPopMatrix();

	//set the material to red plastic
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greyplasticshine);

	//draw the top of the house
	glPushMatrix();
		glTranslatef(5, 10.5, 1);
		drawrectangleLego(10.5, 8, .5);
	glPopMatrix();

	//close the list
	glEndList();

}//end of house 1 dlist
/******************************************************************************
*createTallhosueDlist dlsit decleration
*This is a dlist decleration to create a tall house
******************************************************************************/
void createTallHouseDlist(void)
{
	//create a tallhouse Dlist and set it to compile
	glNewList(tallhouse,GL_COMPILE);
	
	//first we create the base
	//set the color/material of the object we want to draw 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grass_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grass_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grass_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, grassshine);

	//access the modelview matrix
	glPushMatrix();
	//for rendering purposes draw this in the middle of the screen
	glTranslated(10, 0, 0);
		drawrectangleLego(20, 20, 0);
	glPopMatrix();//close the modelview matirx
	
	//set the material to red
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticshine);

	//access the modelview matrix
	glPushMatrix();
		//create the big red tower
		glTranslated(8.5, 11, -2.8);
		drawrectangleLego(4,4.0, 10);
	glPopMatrix();//close the modelview matirx

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silver_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silver_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silver_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, silvershine);

	//access the modelview matrix
	glPushMatrix();
		//draw the bottom row of the house
		glTranslated(3.6, 2, -3.9);
		drawrectangleLego(9.5, 1, 1);
	glPopMatrix();//close the modelview matirx

	//access the modelview matrix
	glPushMatrix();
		//draw the column of the house
		glTranslated(-5, 8, -3.9);
		drawrectangleLego(1, 1, 5);
	glPopMatrix();//close the modelview matirx

	//access the modelview matrix
	glPushMatrix();
		//draw the top of the front
		glTranslated(3.6, 8, -3.9);
		drawrectangleLego(9.5, 1, 1);
	glPopMatrix();//close the modelview matirx

	//draw the bottom left side of the box
	
	glPushMatrix();
		//draw the bottom of the back
		glTranslated(-5.0, 2, -3.9);
		drawrectangleLego(1, 8.5, 1);
	glPopMatrix();//close the modelview matirx

	//draw the top left side of the box

	glPushMatrix();
		//draw the bottom of the back
		glTranslated(-5.0, 8, -3.9);
		drawrectangleLego(1, 8.5, 1);
	glPopMatrix();//close the modelview matirx

	//access the modelview matrix
	glPushMatrix();
		//draw the bottom of the back
		glTranslated(6.6, 2, -12.9);
		drawrectangleLego(12.5, 1, 1);
	glPopMatrix();//close the modelview matirx

	//access the modelview matrix
	glPushMatrix();
		//draw the column of the left back of the hosue
 		glTranslated(6.6, 8, -12.9);
		drawrectangleLego(1, 1, 5);
	glPopMatrix();//close the modelview matirx

	//access the modelview matrix
	glPushMatrix();
		//draw the top of the back
		glTranslated(6.6, 8, -12.9);
		drawrectangleLego(12.5, 1, 1);
	glPopMatrix();//close the modelview matirx
	
	//draw the bottom right side of the box

	glPushMatrix();
	//draw the bottom of the back
		glTranslated(6.6, 2, -7.9);
		drawrectangleLego(1, 4, 1);
	glPopMatrix();//close the modelview matirx

	//draw the top right side of the box

	glPushMatrix();
		//draw the bottom of the back
		glTranslated(6.6, 8,-7.9);
		drawrectangleLego(1.0f, 4.0f, 1.0f);
	glPopMatrix();//close the modelview matirx

	//fix the rightside of the building
	glPushMatrix();
		//draw the bottom of the back
		glTranslated(6.6, 8, -5.9);
		drawrectangle(1.0f, 1.0f, 1.0f);
	glPopMatrix();//close the modelview matirx

	glPushMatrix();
		//draw the bottom of the back
		glTranslated(6.6, 2, -5.9);
		drawrectangleLego(1.0f, 1.0f, 1.0f);
	glPopMatrix();//close the modelview matirx

	//change the material to red plastic  
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticshine);

	//draw the front slope
	glPushMatrix();
		glTranslated(3.6, 10.5, -4.9);
		drawslopeLego(7.4f);
	glPopMatrix();

	//draw the left slope
	glPushMatrix();
		glTranslated(-4.0, 10.5, -6.8);
		glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
		drawslopeLego(4);
	glPopMatrix();

	//draw the back slope
	glPushMatrix();
		glTranslated(-2.6, 10.5, -11.9);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		drawslopeLego(7.5f);
	glPopMatrix();

	//draw the right slope
	glPushMatrix();
		glTranslated(5.6, 10.5, -10.9);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		drawslopeLego(5.0f);
	glPopMatrix();
	
	//draw the glass for the houses
	//access the modelview matrix
	glPushMatrix();
		//draw the top of the front
		glTranslated(3.6f, 8.0f, -4.9f);
		drawglass(8.0f,9.0f);
	glPopMatrix();//close the modelview matirx

	//draw the glass for the houses back side
	//access the modelview matrix
	glPushMatrix();
		//draw the top of the front
		glTranslated(5.0, 8, -13.9);
		drawglass(8.0f, 10.0f);
	glPopMatrix();//close the modelview matirx

	//draw the glass for the houses back side
	//access the modelview matrix
	glPushMatrix();
		//draw the top of the front
		glTranslated(-4.0, 8, -5.0);
		glRotated(-90, 0, 1, 0);
		drawglass(8, 8);
	glPopMatrix();//close the modelview matirx

	//draw the glass for the houses right side
	//access the modelview matrix
	glPushMatrix();
		//draw the top of the front
		glTranslated(6.0, 8, -12.9);
		glRotated(90, 0, 1, 0);
		drawglass(8.0f, 7.0f);
	glPopMatrix();//close the modelview matirx

	//change the material to gold  
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldshine);

	glPushMatrix();
		//draw top gold canister on house corner
		glTranslated(-5.6, 10.4, -3.5);
		glRotated(90, 1.5, 0, 0);
		glScaled(.9, .75, .75);
		drawcanister();
	glPopMatrix();//close the modelview matirx

	glEndList();//close the dlist

}//end dlist for tallhouse
/******************************************************************************
*createRoadDlist function
*This is a function that creates a road segemnt built out of lego and 
*stores it in a compiled Dlist
*This function returns nothing and accepts nothing
******************************************************************************/
void createRoadDlist()
{
	//create a new dlist
	glNewList(road, GL_COMPILE);
		
	//clear the color and depth buffer bits 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, concrete_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, concrete_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, concrete_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, concreteshine);

	//draw the left curb
	glPushMatrix();
		glTranslated(10, 0, 0);
		drawrectangleLego(2, 20, 2);
	glPopMatrix();
	
	//draw the right curb
	glPushMatrix();
		glTranslated(-9, 0, 0);
		drawrectangleLego(2, 20, 2);
	glPopMatrix();

	//set the material to black rubber
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackrubber_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackrubber_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackrubber_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackrubbershine);

	//draw the left side of the road
	glPushMatrix();
		glTranslated(7, -2, 0);
		drawrectangleLego(6, 20, 0);
	glPopMatrix();

	//draw the rightside of the road
	glPushMatrix();
		glTranslated(-2, -2, 0);
		drawrectangleLego(6, 20, 0);
	glPopMatrix();
	
	//set the mateiral to yellow plastic
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowplastic_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowplastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowplastic_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, yellowplasticshine);
	
	//draw the yellow markers in the middle of the road
	//front
	glPushMatrix();		
		glTranslated(0, -2, 0);
		drawrectangleLego(1.0f, 2.0f, 0.0f);
	glPopMatrix();

	//draw the yellow markers in the middle of the road
	//middle
	glPushMatrix();
		glTranslated(0, -2, -8);
		drawrectangleLego(1.0f, 2.0f, 0.0f);
	glPopMatrix();

	//draw the yellow markers in the middle of the road
	//back
	glPushMatrix();
		glTranslated(0, -2, -16);
		drawrectangleLego(1.0f, 2.0f, 0.0f);
	glPopMatrix();

	//set the material to black rubbder
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackrubber_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackrubber_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackrubber_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackrubbershine);

	//fill in the middle of the street with black
	//front
	glPushMatrix();
		glTranslated(0, -2, -2);
		drawrectangleLego(1, 6, 0);
	glPopMatrix();

	//fill in the middle of the street with black
	//middle
	glPushMatrix();
		glTranslated(0, -2, -11);
		drawrectangleLego(1, 4, 0);
	glPopMatrix();

	//fill in the middle of the street with black
	//back
	glPushMatrix();
		glTranslated(0, -2, -19);
		drawrectangleLego(1, 1, 0);
	glPopMatrix();

	//local varibales for the for loop
	//inital run through is to be the negtive x
	GLdouble signchange= 1;
	GLdouble z = 0;

	//run through this loop twice to draw both lights on each side
	for (int x = 0; x < 2;x++)
	{
		//change the material type to gold
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldshine);
	
		//local variable to store the y offest
		GLdouble y = 0;

		//for loop that runs 6 times, used to draw the cylinders for the lights
		for (int i = 0; i < 6; i++)
		{
			glPushMatrix();
				//scale down the cylinders
				glScaled(.5, .5, .5);
				//translate the cylinders to be on the far side of the road
				glTranslated(-19*signchange, 5+y, (-15+z+z));
				//rotate the cylinders to face the proper way
				glRotated(90, 1, 0, 0);
				//call the drawcanister funcion
				drawcanister();
			glPopMatrix();
		
			//increment y + 2(height of the canister) and call again
			y = y + 2;

		}//end of nested for loop
	
		//change the material to red see through plastic
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlasticAlpha_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlasticAlpha_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlasticAlpha_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticAlphashine);
	
		//call to draw a red seethrough light that is place upon the disk
		glPushMatrix();
			glTranslated(-9.5*signchange, 9.2, -7.5+z);
			glScaled(1.5, 1.5, 1.5);
			glRotated(90, 1, 0, 0);
			//call to drawroundplate
			drawRoundPlate();
		glPopMatrix();

		//change the material to silver
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silver_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silver_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silver_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, silvershine);
		
		//draw the dish on top of the cuylinders
		glPushMatrix();
			glTranslated(-9.6*signchange, 8, -7.5+z);
			drawdish(4.0, .5, .5);
		glPopMatrix();

		//for the second run through the main for loop we need to flip the sign
		//on the x axis and minus 10 from the z to get on the otherside of 
		//the street
		signchange = -1;
		z = -10;

	}//end of main for loop

	glEndList();//end of list

}//end of drawRoadDlist
/******************************************************************************
*createBackgrndDlist function
*This function is used to compile the background panels and apply its textures
*This function accepts and returns nothing
*Within this function calls all settings and function to create the texture
* and the polyon that we are placing the texture on
******************************************************************************/
void createBackgrndDlist()
{
	//clear the buffer and depth bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//disable the color material
	glDisable(GL_COLOR_MATERIAL);
	
	//clear the color material
	glClear(GL_COLOR_MATERIAL);

	//create a new dlist for the background and set it to compile
	glNewList(backgrndPanels, GL_COMPILE);

	//place a call to create the texure
	loadImage("wood1.bmp");

	//set the pixel storage mode to 1 byte-alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//generate 1 texture name stored in texName
	glGenTextures(1, &texName);

	//bind a named texture texName to target GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, texName);

	//set texture parameter values
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//set texture enviornment parameters
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//define the 2d texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infoheader.biWidth,
		infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	// enable texture
	glEnable(GL_TEXTURE_2D);


	//begin defining the poygon
	glBegin(GL_POLYGON);
	//apply the texture and then define the polygon position
	glTexCoord2d(0.0, 1.0);
	glVertex3fv(backgrndplate[2]);//top left
	glTexCoord2d(0.0, 0.0);
	glVertex3fv(backgrndplate[1]);//bottom left
	glTexCoord2d(1.0, 0.0);
	glVertex3fv(backgrndplate[0]);//bottom right
	glTexCoord2d(1.0, 1.0);
	glVertex3fv(backgrndplate[3]);//top right
	glEnd();

	//ensure that the texutreing is diabled outside of rendering the panel
	//not doing so will cause the primitive colors to be only black
	glDisable(GL_TEXTURE_2D);

	//close the list
	glEndList();

}//end of createbackgroundDlist
/******************************************************************************
*getrand function
*This function returns a GLfloat and accepts nothing
*The purpose of this funciton is to get a random number that is within 
*the limits of the MAX_RAND global
******************************************************************************/
GLfloat getrand(void)
{
	//create a variable to store the return value and initalize to zero
	GLfloat returnval = 0;

	//call the random function and ensure it is with the MAX_RAND range
	returnval = rand() % MAX_RAND - MAX_RAND/2;
	
	//return the variable
	return returnval;

}//end of getrand function
/******************************************************************************
*setRandMat function
*This function accepts and returns nothing
*This function finds and applies a randomly selected material
******************************************************************************/
void setRandMat(void)
{
	//set the object that we want to swtich on to a random number that is 
	//equal to or less than the number of materials available to us.
	int switch_on =rand() % material_MAX;

	//start switch statement
	switch (switch_on)
	{
	//silver
	case(1):
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silver_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silver_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silver_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, silvershine);
	break;
	//red plastic
	case(2) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticshine);
	break;
	//emerald
	case(3) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, emerald_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, emerald_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, emerald_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, emeraldshine);
	break;
	//black rubber
	case(4) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackrubber_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackrubber_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackrubber_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackrubbershine);
	break;
	//grass
	case(5) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grass_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grass_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grass_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, grassshine);
	break;
	//gold
	case(6) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldshine);
	break;
	//brown
	case(7) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, brown_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brown_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, brown_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brownshine);
	break;
	//yellow plastic
	case(8) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowplastic_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowplastic_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowplastic_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, yellowplasticshine);
	break;
	//red seethrough plastic
	case(9) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlasticAlpha_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlasticAlpha_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlasticAlpha_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticAlphashine);
	break;
	//jade ambient
	case(10) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, jade_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, jade_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, jade_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, jadeshine);
	break;
	//white plastic
	case(11) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whiteplastic_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteplastic_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteplastic_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, whiteplasticshine);
	break;
	//gray plastic
	case(12) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greyplastic_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greyplastic_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greyplastic_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, greyplasticshine);
	break;
	//concrete
	case(13) :
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, concrete_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, concrete_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, concrete_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, concreteshine);
		break;
	case(14) :
		break;

	}//end of switch statement

}//end of setRandMat
/******************************************************************************
*createLegoPile1 function
*This funcion returns nothing and accepts nothing
*This function uses the random materials swtich to create a pile of lego
*please note this is a compiled Dlist and calling this function more than 
*once will yeild the same results
******************************************************************************/
void createLegoPile1(void)
{
	//local variables
	GLfloat horizontal_step = 3;
	GLint startingpos = -2;
	GLint zpos = 5;

	//create a new dlist from the ID of legoPile1 and set the list to compile
	glNewList(legoPile1, GL_COMPILE);
	
		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(2, 1, 3);
			//make a 2*1*2 lego
			drawrectangleLego(2, 1, 2);
		glPopMatrix();

		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(-2, 1, 3);
			//make a 2*1*2 lego rectangle
			drawrectangleLego(2, 1, 2);
		glPopMatrix();

		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(-2, 1, -3);
			//make a 1*1*1 lego rectangle
			drawrectangleLego(1, 1, 1);
		glPopMatrix();

		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(4, 1, -3);
			//make a 5*5*1 rectangle
			drawrectangleLego(5,5 , 1);
		glPopMatrix();
		
		//set the material randomly
		setRandMat();

		glPushMatrix();
			glTranslatef(6, 2, 3);
			//make a 6*1*0 lego
			drawrectangleLego(6, 1, 0);
		glPopMatrix();

		//set the material randomly
		setRandMat();
	
		glPushMatrix();
			glTranslatef(-2, 2, -3);
			//make a 2*2*0 lego
			drawrectangleLego(2, 2, 0);
		glPopMatrix();

		//set the material randomly
		setRandMat();
	
		glPushMatrix();
			glTranslatef(-2, 2, -3);
			//make a 1*1*1 lego rectangle
			drawrectangleLego(1, 1, 1);
		glPopMatrix();

		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(4, 2, -3);
			//make a 0*5*1 lego rectangle
			drawrectangleLego(0, 5, 1);
		glPopMatrix();

		horizontal_step = 1;
		startingpos = -2;
		zpos = 5;

		//a nested for loop that runs 3*3 times to create 
		//randomly generated lego canisters
		for (int x = 0; x < 3; x++)
		{
			for (int i = 0; i<3; i++)
			{
				//set the material randomly
				setRandMat();

				glPushMatrix();
					glTranslatef
						(startingpos * horizontal_step*(rand()%4), 1, zpos);

					glRotated(rand() % 90, rand() % 1, rand() % 1, rand() % 1);
					glScaled(.5, .5, .5);
					//draw a canister
					drawcanister();
				glPopMatrix();
				startingpos += 2;
			
			}//end of inner for loop

			startingpos = -2;//reset the staring position
			zpos -= 5;
		}//end of outer for loop 

		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(5, 2, -0);
			glScaled(.5, .5, .5);
			glRotated(90, 0, 1, 0);	
			//draw a slope 2 wide
			drawslopeLego(2);
		glPopMatrix();
		
		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(0, 1, -2);
			glRotated(90, 0, 1, 0);
			//draw a slope 2 wide
			drawslopeLego(2);
		glPopMatrix();
		
		//set the material randomly
		setRandMat();

		glPushMatrix();
			glTranslatef(0, 1, -2);
			glRotated(90, 0, 1, 0);
			//draw a round plate
			drawRoundPlate();
		glPopMatrix();

		//set the material randomly
		setRandMat();

		glPushMatrix();
			glTranslatef(2, 4, 3);
			glRotated(-90, 1, 0, 0);
			//draw a rectanlge 5*1*2
			drawrectangleLego(5, 1, 2);
		glPopMatrix();

		//set the material randomly
		setRandMat();
		
		glPushMatrix();
			glTranslatef(2, 4, -3);
			glRotated(-90, 1, 0, 1);
			//draw a retangle 5*1*2
			drawrectangleLego(5, 1, 2);
		glPopMatrix();

	glEndList();//end of dlist 

}//end of legopile1 
/******************************************************************************
*createBackgrndDlist function
*This function is used to compile the background panels and apply its textures
*This function accepts and returns nothing
*Within this function calls all settings and function to create the texture
* and the polyon that we are placing the texture on
******************************************************************************/
void createPicture()
{
	//for a sanity check clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_COLOR_MATERIAL);
	glClear(GL_COLOR_MATERIAL);
	//create a new dlist for the background and set it to compile
	glNewList(picturePanels, GL_COMPILE);

	//place a call to create the texure
	loadImage("family.bmp");

	//set the pixel storage mode to 1 byte-alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//generate 1 texture name stored in texName
	glGenTextures(1, &texName);

	//bind a named texture texName to target GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, texName);

	//set texture parameter values
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//set texture enviornment parameters
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//define the 2d texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infoheader.biWidth,
		infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	// enable texture
	glEnable(GL_TEXTURE_2D);

	//begin defining the poygon
	glBegin(GL_POLYGON);
	//apply the texture and then define the polygon position
		glTexCoord2d(0.0, 1.0);
		glVertex3fv(backgrndplate[2]);//top left
		glTexCoord2d(0.0, 0.0);
		glVertex3fv(backgrndplate[1]);//bottom left
		glTexCoord2d(1.0, 0.0);
		glVertex3fv(backgrndplate[0]);//bottom right
		glTexCoord2d(1.0, 1.0);
		glVertex3fv(backgrndplate[3]);//top right
	glEnd();
	
	//ensure that the texutreing is diabled outside of rendering the panel
	//not doing so will cause the primitive colors to be only black
	glDisable(GL_TEXTURE_2D);

	//close the list
	glEndList();
}
/******************************************************************************
*initalize function
*This function accepts and returns nothing
*This is the initalization function used in openGL's call back functions
******************************************************************************/
void initialize()
{
	//seed a random number generator
	srand(time(NULL));

	// set background color
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// enable lighting
	glEnable(GL_LIGHTING);

	//setup the first light
	//enable the light number 1
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	//set up the second light
	//enable light number 2
	glEnable(GL_LIGHT1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT1, GL_AMBIENT, blue_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	//set front face culling to use vertex definitions declared counterclock
	glFrontFace(GL_CCW);

	//enable the normalization objects in the object matrix 
	glEnable(GL_NORMALIZE);

	//create the color 
	GLfloat fogColor[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

	//make the niceset fog
	glHint(GL_FOG_HINT, GL_NICEST);

	//create the fog object and give it a color
	glFogfv(GL_FOG_COLOR, fogColor);

	//set the fog mode
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.07f); // default is 1.0

	//set the start of the fog used in GL_LINEAR FOG
	glFogf(GL_FOG_START, 15.0f); // default 0.0

	//set the ending for the fog used in GL_LINEAR FOG
	glFogf(GL_FOG_END, 100.0f); // default 1.0

	//enable the fog
	glEnable(GL_FOG);

	//use depth testing
	glEnable(GL_DEPTH_TEST);

	//enable blending
	glEnable(GL_BLEND);

	//use the srouce alpha and what happens when we see a tranparent(alpha channel object)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//complie the dlists
	createBackgrndDlist();	//dlist for the table
	createHouse1Dlist();	//dlist for house1
	createTallHouseDlist();	//dlist for tallhouse
	createRoadDlist();		//dlist for the road
	createPicture();		//dlist for the picture
	createLegoPile1();		//dlist for the random lego pile
}//end of initalize function
/******************************************************************************
*display function
*this function accepts nothing and returns nothing
*the display function is a callback function that is used to draw the screen
******************************************************************************/
void display(void)
{
	//clear the color and depth buffer bits 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//load the identity
	glLoadIdentity();

	//create the camera and place its view in the world with glulookat function
	// move back the z-axis, up on the y-axis and rotate the camera on the x-axis
	gluLookAt(0.0,14.0,-50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//create the front of the wood table
	glPushMatrix();
		glTranslatef(0.5f, 0.0f, -40.0f);
		glScaled(2, 1, 1);
		glRotated(90, 1, 0, 0);
		glCallList(backgrndPanels);
	glPopMatrix();

	//create back of the wood table
	glPushMatrix();
		glTranslatef(0.5f, 0.0f, -40.0f);
		glScaled(2, 1, 1);
		glRotated(90, 1, 0, 0);
		glRotated(180, 0, 0, 1);
		glCallList(backgrndPanels);
	glPopMatrix();

	//draw the first house
	glPushMatrix();
		glTranslatef(4.0f, 8.0f, -34.0f);
		glScaled(.25, .25, .25);
		glCallList(tallhouse);
	glPopMatrix();

	//call for house1 
	glPushMatrix();
		glTranslatef(-2.0f, 8.5f, -38.0f);
		glScaled(.25, .25, .25);
		glRotated(189, 0, 1, 0);
		glCallList(house1);
	glPopMatrix();

	//change the material color to white
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, background_Color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteplastic_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, background_Color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, whiteplasticshine);

	//draw the front left wall
	glPushMatrix();
		glTranslatef(10.0f, 39.0f, -25.0f);
		glRotated(-90, 1, 0, 0);
		drawrectangle(5,50,3);
	glPopMatrix();

	//draw the front top wall
	glPushMatrix();
		glTranslatef(4.0f, 39.0f, -25.0f);
		glRotated(-90, 1, 0, 0);
		drawrectangle(20, 18, 3);
	glPopMatrix();

	//draw the front bottom wall
	glPushMatrix();
		glTranslatef(4.0f, 11.0f, -25.0f);
		glRotated(-90, 1, 0, 0);
		drawrectangle(13, 18, 3);
	glPopMatrix();

	//draw the front left wall
	glPushMatrix();
		glTranslatef(-10.0f, 39.0f, -25.0f);
		glRotated(-90, 1, 0, 0);
		drawrectangle(10, 50, 3);
	glPopMatrix();

	//draw the left wall
	glPushMatrix();
		glTranslatef(10.0f, -10.0f, -25.0f);
		glRotated(90, 1, 0, 0);
		glRotated(120, 0, 0, 1);
		drawrectangle(30, 50, 0);
	glPopMatrix();

	//draw the family picture
	glPushMatrix();
		glTranslatef(07.0f, 15.0f, -35.0f);
		glScaled(.25, .25, .25);
		glRotated(90, 0, 1, 0);
		glRotated(160, 0, 1, 0);
		glCallList(picturePanels);
	glPopMatrix();

	//draw the road on the table
	glPushMatrix();
		glTranslatef(0.0f, 8.5f, -43.0f);
		glScaled(.15, .15, .15);
		glRotated(90, 0, 1, 0);
		glCallList(road);
	glPopMatrix();
	
	//draw a second road disconnected from the first
	glPushMatrix();
		glTranslatef(5.0f, 8.5f, -43.0f);
		glScaled(.15, .15, .15);
		glRotated(90, 0, 1, 0);
		glCallList(road);
	glPopMatrix();

	//draw the random lego objects on the table
	glPushMatrix();
		glTranslatef(1.0f, 8.0f, -44.0f);
		glScaled(.125, .125, .125);
		glCallList(legoPile1);
	glPopMatrix();
	
	//draw the outside
	//start by setting the color for the feild
	//using lego we can create a swathed hay field
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldshine);

	//draw the outside feild
	glPushMatrix();
		glTranslatef(17.0f, 15.0f, 50.0f);
		glRotated(-6, 1, 0, 0);
		drawrectangleLego(50,50,1);
	glPopMatrix();

	//draw the left hay bale
	glPushMatrix();
		glTranslatef(-18.0f, 12.0f, 8.0f);
		glRotated(90, 0, 1, 0);
		drawCylinder(5, 5,true);
	glPopMatrix();

	//draw the right hay bale
	glPushMatrix();
		glTranslatef(0.0f, 12.0f, 8.0f);
		glRotated(90, 0, 1, 0);
		drawCylinder(5, 5,true);
	glPopMatrix();

	//draw the left hay bale
	glPushMatrix();
		glTranslatef(10.0f, 12.0f, 20.0f);
		glRotated(90, 0, 1, 0);
		drawCylinder(5, 5,true);
	glPopMatrix();

	//draw the far hay bale
	glPushMatrix();
		glTranslatef(-10.0f, 13.0f, 40.0f);
		glRotated(90, 0, 1, 0);
		drawCylinder(5, 5,true);
	glPopMatrix();

	//draw the farm house in the distance
	glPushMatrix();
		glTranslatef(-14.0f, 16.0f, 50.0f);
		glScaled(.25, .25, .25);
		glCallList(tallhouse);
	glPopMatrix();

	//draw the road in the distance
	glPushMatrix();
		glTranslatef(0.0f, 14.5f, 60.0f);
		glScaled(.75, .75, .75);
		glRotated(90, 0, 1, 0);
		glCallList(road);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(20.0f, 13.5f, 60.0f);
		glScaled(.75, .75, .75);
		glRotated(90, 0, 1, 0);
		glCallList(road);
	glPopMatrix();

	//draw the two large storage containers from the farm
	glPushMatrix();
		glTranslatef(20.0f, 17.5f, 60.0f);
		glRotated(90, 1, 0, .1);
		drawrectangle(10,10,10);
	glPopMatrix();

	//draw the two large storage containers from the farm
	glPushMatrix();
		glTranslatef(20.0f, 17.5f, 60.0f);
		glRotated(90, 1, 0, .1);
		drawrectangle(10, 10, 10);
	glPopMatrix();

	//draw the two large storage containers from the farm
	glPushMatrix();
		glTranslatef(10.0f, 17.5f, 80.0f);
		glRotated(90, 1, 0, .1);
		drawrectangle(10, 10, 10);
	glPopMatrix();

	//swap the backbuffer with the display buffer
	glutSwapBuffers();
}
/******************************************************************************
*reshape function
*This function accepts a height and width of the window
*The reshape is used to track and scale the window was the user moves and
*scales the window. This is called everytime the window is moved or resized
******************************************************************************/
void reshape(int w, int h)
{
	//recreate the viewport used to look into the window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	//access the projection
	glMatrixMode(GL_PROJECTION);

	//load the identity of the projction
	glLoadIdentity();

	//recreate the frustum to left -1,right 1, bottom -1, top 1
	//a near value of 1.5 and a far clipping value of 40
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 500);

	//access the modelview matrix
	glMatrixMode(GL_MODELVIEW);

}
/******************************************************************************
*main function
*This is the main entrance for this application
*main accepts an integer and a char array and returns an integer
******************************************************************************/
int main(int argc, char **argv)
{
	//initalize glut
	glutInit(&argc, argv);

	//initalize the display mode with double buffering, RGB colors and depth
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//set the height and width of the window
	glutInitWindowSize(WIDTH, HEIGHT);

	//set the inital window position(from top left) 100 * 100
	glutInitWindowPosition(100, 100);

	//set the window handle and title bar of the window
	int windowHandle = glutCreateWindow("AU-Comp390 TME 3-1, 2920939");

	//set the window handle
	glutSetWindow(windowHandle);

	//attach the rightclick of the mouse to the menu action handler
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//set the display callback function
	glutDisplayFunc(display);

	//set the reshape callback function
	glutReshapeFunc(reshape);

	//call the initalize function
	initialize();

	//call openGL's main loop
	glutMainLoop();

	//exit with a code of zero if all is well
	return 0;

}//end of main