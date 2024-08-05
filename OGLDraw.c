#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "oglcarsim.h"
#include "cviogl.h"

extern double baseRotation;

// Function prototypes
// drawCube();
void drawWheelAndBolts(GLUquadric* quad);
void drawCar(GLUquadric* quad);
void drawGrid(float offset);

/// Draws a larger circular grid with consistent line thickness
void drawGrid(float offset)
{
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    glLineWidth(2.0);

    float gridSize = 20.0f;
    int lineCount = 81;
    float lineSpacing = (2 * gridSize) / (lineCount - 1);

    // Horizontal lines
    glBegin(GL_LINES);
    for (int i = 0; i < lineCount; i++)
    {
        float y = -gridSize + lineSpacing * i;
        for (float x = -gridSize; x <= gridSize; x += 0.2f)
        {
            float x1 = x + fmod(offset, lineSpacing);
            if (x1 > gridSize) x1 -= 2 * gridSize;
            glVertex2f(x1, y);
            glVertex2f(x1 + 0.2f, y);
        }
    }
    glEnd();

    // Vertical lines
    glBegin(GL_LINES);
    for (i = 0; i < lineCount; i++)
    {
        float x = -gridSize + lineSpacing * i + fmod(offset, lineSpacing);
        if (x > gridSize) x -= 2 * gridSize;
        for (float y = -gridSize; y <= gridSize; y += 0.2f)
        {
            glVertex2f(x, y);
            glVertex2f(x, y + 0.2f);
        }
    }
    glEnd();

    glPopMatrix();
}


//// Draw a simple cube using vertex data, allowing for scaling and translation
//void drawCube()
//{
//	glEnable(GL_NORMALIZE);
//	glBegin(GL_QUADS);
//	
//	// Front Face
//	glNormal3f(0.0f, 0.0f, 1.0f);
//	glVertex3f(-0.5f, -0.5f, 0.5f);
//	glVertex3f(0.5f, -0.5f, 0.5f);
//	glVertex3f(0.5f, 0.5f, 0.5f);
//	glVertex3f(-0.5f, 0.5f, 0.5f);
//	
//	// Back Face
//	glNormal3f(0.0f, 0.0f, -1.0f);
//	glVertex3f(-0.5f, -0.5f, -0.5f);
//	glVertex3f(-0.5f, 0.5f, -0.5f);
//	glVertex3f(0.5f, 0.5f, -0.5f);
//	glVertex3f(0.5f, -0.5f, -0.5f);
//	
//	// Top Face
//	glNormal3f(0.0f, 1.0f, 0.0f);
//	glVertex3f(-0.5f, 0.5f, -0.5f);
//	glVertex3f(-0.5f, 0.5f, 0.5f);
//	glVertex3f(0.5f, 0.5f, 0.5f);
//	glVertex3f(0.5f, 0.5f, -0.5f);
//	
//	// Bottom Face
//	glNormal3f(0.0f, -1.0f, 0.0f);
//	glVertex3f(-0.5f, -0.5f, -0.5f);
//	glVertex3f(0.5f, -0.5f, -0.5f);
//	glVertex3f(0.5f, -0.5f, 0.5f);
//	glVertex3f(-0.5f, -0.5f, 0.5f);
//	
//	// Right face
//	glNormal3f(1.0f, 0.0f, 0.0f);
//	glVertex3f(0.5f, -0.5f, -0.5f);
//	glVertex3f(0.5f, 0.5f, -0.5f);
//	glVertex3f(0.5f, 0.5f, 0.5f);
//	glVertex3f(0.5f, -0.5f, 0.5f);
//	
//	// Left Face
//	glNormal3f(-1.0f, 0.0f, 0.0f);
//	glVertex3f(-0.5f, -0.5f, -0.5f);
//	glVertex3f(-0.5f, -0.5f, 0.5f);
//	glVertex3f(-0.5f, 0.5f, 0.5f);
//	glVertex3f(-0.5f, 0.5f, -0.5f);
//		
//	glEnd();
//	glDisable(GL_NORMALIZE);
//}

// Drawing a wheel with bolts
void drawWheelAndBolts(GLUquadric* quad)
{
	glPushMatrix();	
		glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the axes
		// Draw the wheel
		glRotatef(90.0, 1.0, 0.0, 0.0);	
		glPushMatrix();		
			glTranslatef(0.0, 0.0, 0.05); // Translate disc to center of the wheel
			gluDisk(quad, 0.0, 0.4, 30, 1); // Inner radius, outer radius, slices, loops
		glPopMatrix();
		gluCylinder(quad, 0.4, 0.4, 0.1, 30, 1); // Add a cylinder to give thickness
		
		// Draw five bolts around the wheel
		for (int i = 0; i < 5; i++)
		{
			glPushMatrix();
				glRotatef(i * 72.0, 0.0, 0.0, 1.0); // Rotate bolt position
				glTranslatef(0.3, 0.0, 0.0); // Move bolt away from the center of the wheel
				gluCylinder(quad, 0.04, 0.04, 0.1, 10, 10); // Draw bolt
			glPopMatrix();
		}
	glPopMatrix();
}

// Drawing the complete car using a quadric
void drawCar(GLUquadric* quad)
{
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.4); // Move the whole car up (z-axis)
		
		glPushMatrix();
			glColor3f(1.0f, 1.0f, 0.0f); 
			glTranslatef(-0.75, 0.0, 0.0); 
			glRotatef(90.0, 0.0, 1.0, 0.0); // Rotate to align with car width
			gluCylinder(quad, 0.05, 0.05, 1.5, 20, 1); // Draw cylinder (radius, radius, length, slices, stacks)
			//glScalef(1.5, 0.5, 0.1); // Scale the cube to the desired dimensions (x-scale,  y-scale,  z-scale)
			//drawCube(); // Draw the car body (scaled cube)
		glPopMatrix();
		
		 // Draw wheel axes
        glPushMatrix();
            glColor3f(1.0f, 1.0f, 0.0f); 
            
            // Front axle
            glPushMatrix();
                glTranslatef(-0.75, 0.6, 0.0); // Position at the center between front wheels
				glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the axle with the wheels
                glRotatef(90.0, 1.0, 0.0, 0.0); // Rotate to align with car width
                gluCylinder(quad, 0.05, 0.05, 1.2, 20, 1); // Draw cylinder (radius, radius, length, slices, stacks)
            glPopMatrix();
            
            // Rear axle
            glPushMatrix();
                glTranslatef(0.75, 0.6, 0.0); // Position at the center between rear wheels
				glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the axle with the wheels
                glRotatef(90.0, 1.0, 0.0, 0.0); // Rotate to align with car width
                gluCylinder(quad, 0.05, 0.05, 1.2, 20, 1); // Draw cylinder (radius, radius, length, slices, stacks)
            glPopMatrix();
        glPopMatrix();
		
		// Draw wheels at each corner of the car
		glPushMatrix();
			glTranslatef(0.75, 0.6, 0.0); // Position rear-right wheel
			glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
			drawWheelAndBolts(quad);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(0.75, -0.5, 0.0); // Position rear-left wheel
			glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
			drawWheelAndBolts(quad);
		glPopMatrix();
	
		glPushMatrix();
			glTranslatef(-0.75, 0.6, 0.0); // Position front-right wheel
			glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
			drawWheelAndBolts(quad);
		glPopMatrix();
	
		glPushMatrix();
			glTranslatef(-0.75, -0.5, 0.0); // Position rear-left wheel
			glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
			drawWheelAndBolts(quad);
		glPopMatrix();
	
	glPopMatrix();
}