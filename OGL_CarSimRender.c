#include "OGL_CarSimHeader.h"

double centerX;
Obstacle obstacles[MAX_OBSTACLES];
int obstacleCount = 0;

void RenderGrid()
{
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glLineWidth(3.0);
	float lineSpacing = (2 * GRID_SIZE) / (LINE_COUNT - 1);

	// Render the primary grid
	glTranslatef(centerX, 0.0f, 0.0f);
	RenderGridLines(LINE_COUNT, lineSpacing);
	RenderObstacleCars();
	// Check if the car is near the threshold to render additional grids
	if (!isNearCenter(car.positionX, centerX))
	{
		// Reset the grid center when the car passes the threshold
		ResetGridCenter(car.positionX);
	}
	glPopMatrix();
}

bool isNearCenter(float carX, float centerX)
{
    float threshold = 2 * GRID_SIZE / 3.0f;
    return (fabs(carX - centerX) < threshold);
}

void ResetGridCenter(float carX)
{
    centerX = carX;
    InitializeObstacles();
}

void InitializeObstacles(void)
{
    srand(time(NULL));
    obstacleCount = 4 + rand() % 7;

    for (int i = 0; i < obstacleCount; i++)
    {
        obstacles[i].posX = (-(float)rand() / RAND_MAX) * (2*GRID_SIZE /3) - GRID_SIZE/10;
        obstacles[i].posY = ((float)rand() / RAND_MAX) * (GRID_SIZE / 16) - ((float)rand() / RAND_MAX) * (GRID_SIZE / 16);
        obstacles[i].width = 1.0f;
        obstacles[i].height = 1.0f;
        obstacles[i].isWall = rand() % 2;
    }
}

void RenderGridLines(int lineCount, float lineSpacing)
{
    // Horizontal lines
    glBegin(GL_LINES);
    for (int i = 0; i < lineCount; i++)
    {
        float y = -GRID_SIZE + lineSpacing * i;
        glVertex2f(-GRID_SIZE, y / 16);
        glVertex2f(GRID_SIZE, y / 16);
    }
    glEnd();

    // Vertical lines
    glBegin(GL_LINES);
    for (int i = 0; i < 16 * lineCount; i++)
    {
        float x = -GRID_SIZE + lineSpacing / 16 * i;
        glVertex2f(x, -GRID_SIZE / 16);
        glVertex2f(x, GRID_SIZE / 16);
    }
    glEnd();
}

void RenderCarImage(void)
{
    GLfloat specularLight0[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight0);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);
    
    UpdateCameraPosition();
    
    RenderGrid();

    GLUquadricObj *object = gluNewQuadric();
    RenderCar(object);
    gluDeleteQuadric(object);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glPopAttrib();
    glFlush();
}

int CVICALLBACK OGLCallback(int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case OGLEVENT_REFRESH:
            RenderCarImage();
            break;
    }
    return 0;
}

void UpdateCameraPosition(void)
{
    static double currentDistance = DISTANCE;
    static double currentHeight = HEIGHT;

    double currentCenterX = car.positionX;
    double currentCenterY = 0.0;
    double currentCenterZ = CENTERZ;

    double eyeX = car.positionX + currentDistance;
    double eyeY = 0.0;
    double eyeZ = currentCenterZ + currentHeight;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eyeX, eyeY, eyeZ,
        currentCenterX, currentCenterY, currentCenterZ,
        0.0, 0.0, 1.0
    );
}


void DrawWheelAndBolts(GLUquadric* quad)
{
    glPushMatrix();
    
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.05);
    gluDisk(quad, 0.0, 0.25, 30, 1);
    glPopMatrix();
    gluCylinder(quad, 0.25, 0.25, 0.1, 30, 1);

    for (int i = 0; i < 5; i++)
    {
        glPushMatrix();
        glRotatef(i * 72.0, 0.0, 0.0, 1.0);
        glTranslatef(0.16, 0.0, 0.0);
        gluCylinder(quad, 0.025, 0.025, 0.1, 10, 10);
        glPopMatrix();
    }
    glPopMatrix();
}

void DrawAxleSystem(GLUquadric* quad)
{
    glPushMatrix();
    glTranslatef(-0.75, 0.5, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.04, 0.04, 1.0, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75, 0.5, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.04, 0.04, 1.0, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.75, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    gluCylinder(quad, 0.04, 0.04, 1.5, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.75, 0.5, 0.0);
    gluSphere(quad, 0.06, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.75, -0.5, 0.0);
    gluSphere(quad, 0.06, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75, 0.5, 0.0);
    gluSphere(quad, 0.06, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75, -0.5, 0.0);
    gluSphere(quad, 0.06, 20, 20);
    glPopMatrix();
}

void RenderCar(GLUquadric* quad)
{
    double innerWheelAngle = atan(WHEEL_BASE / (WHEEL_BASE / tan(car.steeringAngle * PI / 180.0) + TRACK_WIDTH / 2)) * 180.0 / PI;
    double outerWheelAngle = atan(WHEEL_BASE / (WHEEL_BASE / tan(car.steeringAngle * PI / 180.0) - TRACK_WIDTH / 2)) * 180.0 / PI;

    if (car.forwardMove != car.prevForwardMove)
    {
        UpdateCarPosition(car.forwardMove - car.prevForwardMove);
        car.prevForwardMove = car.forwardMove;
    }

    glPushMatrix();
    glTranslatef(car.positionX, car.positionY, 0.25);
    glRotatef(car.direction, 0.0, 0.0, 1.0);
    glColor3f(0.5f, 0.5f, 0.5f);
    
    DrawAxleSystem(quad);

    glPushMatrix();
    glTranslatef(-0.75, 0.55, 0.0);
    glRotatef(-innerWheelAngle, 0.0, 0.0, 1.0);
    glRotatef(car.forwardMove, 0.0, 1.0, 0.0);
    DrawWheelAndBolts(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.75, -0.45, 0.0);
    glRotatef(-outerWheelAngle, 0.0, 0.0, 1.0);
    glRotatef(car.forwardMove, 0.0, 1.0, 0.0);
    DrawWheelAndBolts(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75, 0.55, 0.0);
    glRotatef(car.forwardMove, 0.0, 1.0, 0.0);
    DrawWheelAndBolts(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75, -0.45, 0.0);
    glRotatef(car.forwardMove, 0.0, 1.0, 0.0);
    DrawWheelAndBolts(quad);
    glPopMatrix();

    glPopMatrix();
}

void UpdateCarPosition(double pushChange)
{
    double movementAmount = pushChange * 0.01;

    car.positionX += movementAmount * cos(car.direction * PI / 180.0);
    car.positionY += movementAmount * sin(car.direction * PI / 180.0);

    if (CheckCollision(car.positionX, car.positionY, 1.0, 1.0))
    {
        isRunning = 0;
        ResetCarPosition();
    }

    if (movementAmount != 0.0 && car.steeringAngle != 0.0)
    {
        double turningRadius = WHEEL_BASE / tan(car.steeringAngle * PI / 180.0);
        car.direction += (movementAmount / turningRadius) * (180.0 / PI);
    }
}


void RenderObstacleCars(void)
{
    GLUquadric* obstacleQuad = gluNewQuadric();

    for (int i = 0; i < obstacleCount; i++)
    {
        if (obstacles[i].isWall)
        {
            glColor3f(0.0f, 0.8f, 0.0f);
        }
        else
        {
            glColor3f(0.0f, 0.0f, 0.8f);
        }
        glPushMatrix();
        glTranslatef(obstacles[i].posX, obstacles[i].posY, 0.25);

        DrawAxleSystem(obstacleQuad);

        glPushMatrix();
        glTranslatef(-0.75, 0.55, 0.0);
        DrawWheelAndBolts(obstacleQuad);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.75, -0.45, 0.0);
        DrawWheelAndBolts(obstacleQuad);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.75, 0.55, 0.0);
        DrawWheelAndBolts(obstacleQuad);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.75, -0.45, 0.0);
        DrawWheelAndBolts(obstacleQuad);
        glPopMatrix();

        glPopMatrix();
    }

    gluDeleteQuadric(obstacleQuad);
}

bool CheckCollision(double carX, double carY, double carWidth, double carHeight)
{
    double yMinLimit = -GRID_SIZE / 16;
    double yMaxLimit = GRID_SIZE / 16;

    if (carY < yMinLimit || carY + carHeight > yMaxLimit)
    {
        return true;
    }

    for (int i = 0; i < obstacleCount; i++)
    {
        double adjustedPosX = obstacles[i].posX + centerX;

        if (carX < adjustedPosX + obstacles[i].width &&
            carX + carWidth > adjustedPosX &&
            carY < obstacles[i].posY + obstacles[i].height &&
            carY + carHeight > obstacles[i].posY)
        {
            return true;
        }
    }

    return false;
}
