//-------------------------------------------------------------
//----------------COSC363 Assignment 01------------------------
//-------Author: Jake Wilson-----------------------------------
//-------Student Number: 49606681------------------------------
//-------Student Code: jwi187----------------------------------
//-------------------------------------------------------------

#define _USE_MATH_DEFINES 
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;

GLUquadricObj* q;
GLuint txId[7];   //Texture ids
int timer_count = 0;
int timer_refresh = 8;
//---Global Variables for Camera Movement
float cam_hgt = 12;			//Camera height
float angle = 0.0;		//Rotation angle for viewing
float look_x = 0.0;
float look_z = 0.0;
float eye_x = 0.0;
float eye_y = 12.0;
float eye_z = 10.0;
//---Global Variables for Ames Window
float theta = 0.0;
float rotate_angle = 1;
float amesXPos = -40;
float amesYPos = 15;
float amesZPos = 0;
//---Global Variables for Moire Pattern
float moire_moving_x = 1;
bool moire_move_right = true;
float moireWindowRadius = 12.5;
float moireWindowHeight = 25;
float moireXPos = 0;
float moireYPos = 3;
float moireZPos = -40;
//---Global Variables for 3D Animation
bool count_up = true;
float z_bowl = 24.0;	//Z Axis Point where cricket ball Resets
float y_bowl = 6;		//Y Axis Point where cricket ball Resets
bool bowl_down = true;
float stump_angle = -90;
float bail_y = 6.0;
float bail_x = -0.2;
float bail_rotate = 0.0;
float cricketXPos = 30;
float cricketYPos = 4;
float cricketZPos = 0;
//---Global Variables for Display Podiums
float podiumHeight = 3.0;
float podiumRadius = 10.0;
float podiumMoveX = 0;
float podiumMoveZ = 0;

float xpts[16] = { 30.0, 28.5, 26.0, 23.0, 19.0, 15.0, 8.0, 2.0, -2.0, -8.0, -15.0, -19.0, -23.0, -26.0, -28.5, -30.0 };
float zpts[16] = { 10.0, 12.5, 14.5, 17.0, 19.0, 23.0, 28.0, 30.0, 30.0, 28.0, 23.0, 19.0, 17.0, 14.5, 12.5, 10.0 };

void special(int key, int x, int y)
{

	if (key == GLUT_KEY_LEFT) angle -= 0.05;  //Change direction
	else if (key == GLUT_KEY_RIGHT) angle += 0.05;
	else if (key == GLUT_KEY_DOWN)
	{  //Move backward
		eye_x -= 0.3 * sin(angle);
		eye_z += 0.3 * cos(angle);
	}
	else if (key == GLUT_KEY_UP)
	{ //Move forward
		eye_x += 0.3 * sin(angle);
		eye_z -= 0.3 * cos(angle);
	}

	look_x = eye_x + 10 * sin(angle);
	look_z = eye_z - 10 * cos(angle);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) 
{
	switch (key)
	{
	case '0': 
		angle = 0.0;
		look_x = 0.0;
		look_z = 0.0;
		eye_x = 40.0;
		eye_z = -30.0; 
		eye_y = 12.0;
		break;
	case '1': 
		look_x = -20.0;
		look_z = 0.0;
		eye_x = 0.0;
		eye_z = 0.0;  
		break;
	case '2': 
		look_x = 0.0;
		look_z = -30.0;
		eye_x = 0.0;
		eye_z = 0.0; 
		break;
	case '3': 
		look_x = 20.0;
		look_z = 0.0;
		eye_x = 0.0;
		eye_z = 0.0;
		break;
	}
	glutPostRedisplay();
}

void myTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(timer_refresh, myTimer, 0);
	rotate_angle += 1;
	timer_count += 1;
	if (timer_count == 120) {
		timer_count = 0;
	}
	if (timer_count % 10 == 0) {
		if (moire_move_right) {
			moire_moving_x++;
			if (moire_moving_x >= 12.5) {
				moire_move_right = false;
			}
		}
		else {
			moire_moving_x--;
			if (moire_moving_x <= -12.5) {
				moire_move_right = true;
			}
		}
	}
}

//---- A function to load textures in TGA format ----
void loadTextures()
{
	glGenTextures(6, txId); 	

	glBindTexture(GL_TEXTURE_2D, txId[0]);  
	loadTGA("moirepattern.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[1]);  
	loadTGA("floorPattern.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);  
	loadTGA("woodplank.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[3]); 
	loadTGA("podiumFrame.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadTGA("skyBackground.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[5]);
	loadTGA("illusionOne.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[6]);
	loadTGA("illusionTwo.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void drawAmesWindow()
{
	//Render Each little window
	glPushMatrix();

	glTranslatef(amesXPos, amesYPos, amesZPos);

	glPushMatrix();
	glRotatef(rotate_angle, 0, 5, 0);
	glTranslatef(-10.0, 0, 0);

	//Top Left Window

	glBegin(GL_QUADS);	//Side Vertical Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.75, 3.25);
	glVertex2f(0.75, 0.45);
	glVertex2f(1.6, 0.45);
	glVertex2f(1.6, 2.8);
	glEnd();

	glBegin(GL_QUADS);	//Top Horizontal Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.75, 3.25);
	glVertex2f(1.6, 2.8);
	glVertex2f(5.15, 3.2);
	glVertex2f(5.15, 3.8);
	glEnd();

	glEnable(GL_BLEND);
	glBegin(GL_QUADS);	//Inside Window Piece
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(1.6, 2.8);
	glVertex2f(1.6, 0.45);
	glVertex2f(5.15, 0.45);
	glVertex2f(5.15, 3.3);
	glEnd();
	glDisable(GL_BLEND);

	//Bottom Left Window
	glBegin(GL_QUADS);	//Side Vertical Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.75, -3.25);
	glVertex2f(0.75, -0.45);
	glVertex2f(1.6, -0.45);
	glVertex2f(1.6, -2.8);
	glEnd();

	glBegin(GL_QUADS);	//Bottom Horizontal Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.75, -3.25);
	glVertex2f(1.6, -2.8);
	glVertex2f(5.15, -3.2);
	glVertex2f(5.15, -3.8);
	glEnd();

	glEnable(GL_BLEND);
	glBegin(GL_QUADS);	//Inside Window Piece
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(1.6, -2.8);
	glVertex2f(1.6, -0.45);
	glVertex2f(5.15, -0.45);
	glVertex2f(5.15, -3.3);
	glEnd();
	glDisable(GL_BLEND);

	//Top Mid Window
	glBegin(GL_QUADS);	//Side Vertical Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(6.25, 3.9);
	glVertex2f(6.25, 0.45);
	glVertex2f(7.3, 0.45);
	glVertex2f(7.3, 3.5);
	glEnd();

	glBegin(GL_QUADS);	//Top Horizontal Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(6.25, 3.9);
	glVertex2f(7.3, 3.5);
	glVertex2f(11.75, 4.1);
	glVertex2f(11.75, 4.75);
	glEnd();

	glEnable(GL_BLEND);
	glBegin(GL_QUADS);	//Inside Window Piece
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(7.3, 3.5);
	glVertex2f(7.3, 0.45);
	glVertex2f(11.75, 0.45);
	glVertex2f(11.75, 4.1);
	glEnd();
	glDisable(GL_BLEND);

	//Bottom Mid Window
	glBegin(GL_QUADS);	//Side Vertical Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(6.25, -3.9);
	glVertex2f(6.25, -0.45);
	glVertex2f(7.3, -0.45);
	glVertex2f(7.3, -3.5);
	glEnd();

	glBegin(GL_QUADS);	//Bottom Horizontal Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(6.25, -3.9);
	glVertex2f(7.3, -3.5);
	glVertex2f(11.75, -4.1);
	glVertex2f(11.75, -4.75);
	glEnd();

	glEnable(GL_BLEND);
	glBegin(GL_QUADS);	//Inside Window Piece
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(7.3, -3.5);
	glVertex2f(7.3, -0.45);
	glVertex2f(11.75, -0.45);
	glVertex2f(11.75, -4.1);
	glEnd();
	glDisable(GL_BLEND);

	//Top Right Window
	glBegin(GL_QUADS);	//Side Vertical Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(13, 4.9);
	glVertex2f(13, 0.45);
	glVertex2f(14.25, 0.45);
	glVertex2f(14.25, 4.4);
	glEnd();

	glBegin(GL_QUADS);	//Top Horizontal Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(13, 4.9);
	glVertex2f(14.25, 4.4);
	glVertex2f(19.8, 5.1);
	glVertex2f(19.8, 5.8);
	glEnd();

	glEnable(GL_BLEND);
	glBegin(GL_QUADS);	//Inside Window Piece
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(14.25, 4.4);
	glVertex2f(14.25, 0.45);
	glVertex2f(19.8, 0.45);
	glVertex2f(19.8, 5.1);
	glEnd();
	glDisable(GL_BLEND);

	//Bottom Right Window
	glBegin(GL_QUADS);	//Side Vertical Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(13, -4.9);
	glVertex2f(13, -0.45);
	glVertex2f(14.25, -0.45);
	glVertex2f(14.25, -4.4);
	glEnd();

	glBegin(GL_QUADS);	//Top Horizontal Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(13, -4.9);
	glVertex2f(14.25, -4.4);
	glVertex2f(19.8, -5.1);
	glVertex2f(19.8, -5.8);
	glEnd();

	glEnable(GL_BLEND);
	glBegin(GL_QUADS);	//Inside Window Piece
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(14.25, -4.4);
	glVertex2f(14.25, -0.45);
	glVertex2f(19.8, -0.45);
	glVertex2f(19.8, -5.1);
	glEnd();
	glDisable(GL_BLEND);

	glBegin(GL_QUADS); //Top Top Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0, 4.0);
	glVertex2f(0.0, 3.05);
	glVertex2f(21.0, 5.95);
	glVertex2f(21.0, 8.0);
	glEnd();

	glBegin(GL_QUADS); //Bottom Bottom Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0, -4.0);
	glVertex2f(0.0, -3.05);
	glVertex2f(21.0, -5.95);
	glVertex2f(21.0, -8.0);
	glEnd();

	glBegin(GL_QUADS); //Left Mid Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0, 3.05);
	glVertex2f(0.0, -3.05);
	glVertex2f(0.75, -3.25);
	glVertex2f(0.75, 3.25);
	glEnd();

	glBegin(GL_QUADS); //Second from Left Mid Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(5.15, 3.8);
	glVertex2f(5.15, -3.8);
	glVertex2f(6.25, -3.9);
	glVertex2f(6.25, 3.9);
	glEnd();

	glBegin(GL_QUADS); //Second from Right Mid Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(11.75, 4.75);
	glVertex2f(11.75, -4.75);
	glVertex2f(13, -4.9);
	glVertex2f(13, 4.9);
	glEnd();

	glBegin(GL_QUADS); //Right Mid Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(19.8, 5.8);
	glVertex2f(19.8, -5.8);
	glVertex2f(21, -5.95);
	glVertex2f(21, 5.95);
	glEnd();

	glBegin(GL_QUADS); //Mid Mid Quad
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.75, 0.45);
	glVertex2f(0.75, -0.45);
	glVertex2f(19.8, -0.45);
	glVertex2f(19.8, 0.45);
	glEnd();

	glBegin(GL_QUADS);	//End Piece
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(21.0, 8.0);
	glVertex2f(21.0, -8.0);
	glVertex2f(22.0, -7.0);
	glVertex2f(22.0, 7.0);
	glEnd();

	glPopMatrix();
	glPopMatrix();

	glFlush();
}

void drawFloor()
{
	glPushMatrix();
	glColor3f(0., 1, 1.);		//Floor colour
	glNormal3f(0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);			//A single quad
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-100, 0, 100);
	glTexCoord2f(20.0, 0.0);
	glVertex3f(100, 0, 100);
	glTexCoord2f(20.0, 20.0);
	glVertex3f(100, 0, -100);
	glTexCoord2f(0.0, 20.0);
	glVertex3f(-100, 0, -100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void drawWalls()
{
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glNormal3f(0, 1, 0);

	// -X Wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-100, 0, 100);
	glTexCoord2f(0.0, 3.0);
	glVertex3f(-100, 0, -100);
	glTexCoord2f(6.0, 3.0);
	glVertex3f(-100, 200, -100);
	glTexCoord2f(6.0, 0.0);
	glVertex3f(-100, 200, 100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	// -Z Wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-100, 0, -100);
	glTexCoord2f(10.0, 0.0);
	glVertex3f(100, 0, -100);
	glTexCoord2f(10.0, 10.0);
	glVertex3f(100, 200, -100);
	glTexCoord2f(0.0, 10.0);
	glVertex3f(-100, 200, -100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	// +X Wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(100, 0, -100);
	glTexCoord2f(10.0, 0.0);
	glVertex3f(100, 0, 100);
	glTexCoord2f(10.0, 10.0);
	glVertex3f(100, 200, 100);
	glTexCoord2f(0.0, 10.0);
	glVertex3f(100, 200, -100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	// +Z Wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(100, 0, 100);
	glTexCoord2f(10.0, 0.0);
	glVertex3f(-100, 0, 100);
	glTexCoord2f(10.0, 10.0);
	glVertex3f(-100, 200, 100);
	glTexCoord2f(0.0, 10.0);
	glVertex3f(100, 200, 100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

void drawPodiums()
{

	for(int i = 0; i < 3; i++) {
		if (i == 0) {	//First Podium (Ames Windows)
			podiumMoveX = amesXPos;
			podiumMoveZ = amesZPos;
		}
		else if (i == 1) {	//Second Podium	(Moire Pattern)
			podiumMoveX = moireXPos;
			podiumMoveZ = moireZPos;
		}
		else if (i == 2) {	//Third Podium (3D Cricket Animation)
			podiumMoveX = cricketXPos;
			podiumMoveZ = cricketZPos;
		}

		glPushMatrix();
		glTranslatef(podiumMoveX, 0, podiumMoveZ);

		glPushMatrix();	//Top Piece
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[2]);
		glColor3f(1.0, 0.0, 0.0);
		//glRotatef(-60, 5, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-(podiumRadius), podiumHeight, podiumRadius);
		glTexCoord2f(2.0, 0.0);
		glVertex3f(podiumRadius, podiumHeight, podiumRadius);
		glTexCoord2f(2.0, 2.0);
		glVertex3f(podiumRadius, podiumHeight, -(podiumRadius));
		glTexCoord2f(0.0, 2.0);
		glVertex3f(-(podiumRadius), podiumHeight, -(podiumRadius));
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();	//+Z Axis Side piece
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glColor3f(1.0, 0.0, 0.0);
		//glRotatef(-60, 5, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-(podiumRadius), 0, podiumRadius);
		glTexCoord2f(4.0, 0.0);
		glVertex3f(podiumRadius, 0, podiumRadius);
		glTexCoord2f(4.0, 1.0);
		glVertex3f(podiumRadius, podiumHeight, podiumRadius);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-(podiumRadius), podiumHeight, podiumRadius);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();	//+X Axis Front piece
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glColor3f(1.0, 0.0, 0.0);
		//glRotatef(-60, 5, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(podiumRadius, 0, podiumRadius);
		glTexCoord2f(4.0, 0.0);
		glVertex3f(podiumRadius, 0, -(podiumRadius));
		glTexCoord2f(4.0, 1.0);
		glVertex3f(podiumRadius, podiumHeight, -(podiumRadius));
		glTexCoord2f(0.0, 1.0);
		glVertex3f(podiumRadius, podiumHeight, podiumRadius);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();	//-X Axis Front piece
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glColor3f(1.0, 0.0, 0.0);
		//glRotatef(-60, 5, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-(podiumRadius), 0, -(podiumRadius));
		glTexCoord2f(4.0, 0.0);
		glVertex3f(-(podiumRadius), 0, podiumRadius);
		glTexCoord2f(4.0, 1.0);
		glVertex3f(-(podiumRadius), podiumHeight, podiumRadius);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-(podiumRadius), podiumHeight, -(podiumRadius));
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();	//-Z Axis Side piece
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glColor3f(1.0, 0.0, 0.0);
		//glRotatef(-60, 5, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(podiumRadius, 0, -(podiumRadius));
		glTexCoord2f(4.0, 0.0);
		glVertex3f(-(podiumRadius), 0, -(podiumRadius));
		glTexCoord2f(4.0, 1.0);
		glVertex3f(-(podiumRadius), podiumHeight, -(podiumRadius));
		glTexCoord2f(0.0, 1.0);
		glVertex3f(podiumRadius, podiumHeight, -(podiumRadius));
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPopMatrix();
	}
	
}

void drawMoirePattern()
{
	glPushMatrix();
	glTranslatef(moireXPos, moireYPos, moireZPos);

	glPushMatrix();
	glBegin(GL_QUADS);			//Big WhiteBackground
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.0, 3.0, 0.0);
	glVertex3f(-(moireWindowRadius + 5), 2, -1);
	glVertex3f(moireWindowRadius + 5, 2, -1);
	glVertex3f(moireWindowRadius + 5, moireWindowHeight + 2, -1);
	glVertex3f(-(moireWindowRadius + 5), moireWindowHeight + 2, -1);
	glEnd();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);			//Background non moving pattern
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-(moireWindowRadius), podiumHeight, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(moireWindowRadius, podiumHeight, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(moireWindowRadius, moireWindowHeight, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-(moireWindowRadius), moireWindowHeight, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(moire_moving_x, 0, 0);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glColor4f(0.0, 0.0, 1.0, 0.9);
	glBegin(GL_QUADS);			//Foreground moving pattern
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-(moireWindowRadius), podiumHeight, 1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(moireWindowRadius, podiumHeight, 1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(moireWindowRadius, moireWindowHeight, 1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-(moireWindowRadius), moireWindowHeight, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-(moire_moving_x), 0, 0);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glColor4f(0.0, 0.0, 1.0, 0.9);
	glBegin(GL_QUADS);			//Second Foreground moving pattern
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-(moireWindowRadius), podiumHeight, 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(moireWindowRadius, podiumHeight, 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(moireWindowRadius, moireWindowHeight + 1, 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-(moireWindowRadius), moireWindowHeight + 1, 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

	glPopMatrix();
	
}

void DrawStumps()
{
	glPushMatrix();
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);			//A single quad
	//glTexCoord2f(0.0, 0.0);
	glVertex3f(-3.0, 0.1, -2);
	//glTexCoord2f(10.0, 0.0);
	glVertex3f(-3, 0.1, 14);
	//glTexCoord2f(10.0, 10.0);
	glVertex3f(5, 0.1, 14);
	//glTexCoord2f(0.0, 10.0);
	glVertex3f(5, 0.1, -2);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();	//stump getting hit
	glColor3f(0.8, 0.8, 0.8);
	glRotatef(stump_angle, 1, 0, 0);
	glTranslatef(0.0, 0.0, 0.0);
	glutSolidCylinder(0.3, 6, 50, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.8, 0.8);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(1.0, 0.0, 0.0);
	glutSolidCylinder(0.3, 6, 50, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.8, 0.8);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(2.0, 0.0, 0.0);
	glutSolidCylinder(0.3, 6, 50, 2);
	glPopMatrix();

	glPushMatrix();	//Bail Coming off
	glColor3f(0.7, 0.4, 0.1);
	glTranslatef(bail_x, bail_y, 0.0);
	glRotatef(bail_rotate, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glutSolidCylinder(0.2, 1.1, 50, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.7, 0.4, 0.1);
	glTranslatef(1.1, 6.0, 0.0);
	glRotatef(90, 0, 1, 0);
	glutSolidCylinder(0.2, 1.1, 50, 2);
	glPopMatrix();
	glPopMatrix();
}

void BowlingSequence()
{
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, y_bowl, z_bowl);
	glutSolidSphere(0.4, 50, 50);
	glPopMatrix();
	if (y_bowl <= 0) {
		bowl_down = false;
	}
	if (bowl_down) {
		z_bowl -= 0.2;
		y_bowl -= 0.1;
	}
	else {
		z_bowl -= 0.2;
		y_bowl += 0.1;
	}
	if (z_bowl <= 0) { //Bails pop off and stump falls back
		stump_angle -= 1;
		bail_y += 0.1;
		bail_x += 0.05;
		bail_rotate += 5;
	}
	if (z_bowl <= -6) {	//reset
		z_bowl = 24.0;
		y_bowl = 6;
		bowl_down = true;
		stump_angle = -90;
		bail_y = 6.0;
		bail_x = -0.2;
		bail_rotate = 0.0;
	}
}

void draw3DCricketAnimation()
{
	glPushMatrix();
	glTranslatef(cricketXPos + 3, cricketYPos, cricketZPos + -4);
	glRotatef(-50.0, 0, 1, 0);

	DrawStumps();
	BowlingSequence();
	glPopMatrix();
}

void drawCurveWall()
{
	glPushMatrix();
	glTranslatef(0, 0, 10);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	glColor3f(1, 1, 1);
	glLineWidth(2.0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 16; i++)
	{
		glTexCoord2f((i * 15) / (16 - 1), 0);
		glVertex3f(xpts[i], 0, zpts[i]);
		glTexCoord2f((i * 15) / (16 - 1), 1);
		glVertex3f(xpts[i], 15, zpts[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void normal(int i)
{
	float xdiff1, zdiff1, xdiff2, zdiff2;
	if (i == 0 || i == 12) glNormal3f(-1, 0, 0);   //default normal along -x direction
	else
	{
		xdiff1 = xpts[i] - xpts[i - 1];
		zdiff1 = zpts[i] - zpts[i - 1];
		xdiff2 = xpts[i + 1] - xpts[i];
		zdiff2 = zpts[i + 1] - zpts[i];
		glNormal3f(-(zdiff1 + zdiff2), 0, (xdiff1 + xdiff2));
	}
}

void drawStaticIllusions()
{
	glPushMatrix();
	glTranslatef(-15.0, 3.0, 30);
	glRotatef(-30.0, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5, 0, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(5, 0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5, 10, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-5, 10, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 3.0, 30);
	glRotatef(30.0, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5, 0, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(5, 0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5, 10, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-5, 10, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void display(void)
{
	float lpos[4] = { 0., 30., 0., 1.0 };  //light's position
	float white[3] = { 1, 1, 1 };
	float grey[3] = { 0.2, 0.2, 0.2};
	float cyan[3] = { 0, 1, 1 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, eye_y, eye_z, look_x, 12, look_z, 0, 1, 0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glDisable(GL_LIGHTING);			

	drawFloor();
	drawWalls();
	drawPodiums();
	drawStaticIllusions();
	drawCurveWall();

	drawAmesWindow();	//Assignment Item 1 - Ames Window
	drawMoirePattern();	//Assignment Item 2 - Moire Pattern
	draw3DCricketAnimation();	//Assignment Item 3 - Cricket Bowling Animation

	glEnable(GL_LIGHTING);			

	glutSwapBuffers();

	glFlush();
}

void initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	loadTextures();

	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glBlendFunc(GL_DST_COLOR, GL_ZERO);	//For Assignment Item 2 - Moire Pattern

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1, 10.0, 1000.0);   //Camera Frustum
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("COSC363 Assignment 1");
	initialize();
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(timer_refresh, myTimer, 0);

	glutMainLoop();
	return 0;
}