//FILE NAME:HW6
//description:Draw some text!
//Author:Nabeel Alkhatib
//Date created:10/25/2018
//Date modified:10/25/2018
#include <iostream>
#include <GL\freeglut.h>
using namespace std;

//Prototypes

void resetScene();
void display(void);
void reshape(GLsizei width, GLsizei height);
void keyboardClick(unsigned char key, int x, int y);
void mouseClick(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void timer(int milisec);
void drawCube();
void drawPyramid();
void enableLights();

void drawStrokeText(char *text, void *font, GLfloat x, GLfloat y, GLfloat z);
GLfloat getStrokeTextWidth(char *text, void *font);
void drawBitmapText(char *text, void *font, GLfloat x, GLfloat y);
GLfloat getBitmapTextWidth(char *text,void *font);

GLfloat locationX, locationY;	//current location of the object
GLfloat rotationX, rotationY;
GLsizei mouseX, mouseY;			//current mouse location in window
int orbitRotation;				//Rotation in degrees
bool blEnableLights= true;
bool blMouseLeftDown;			// Current state of button
bool blMouseMiddleDown;			// Current state of button
bool blMouseRightDown;			// Current state of button
int screenWidth = 640;
int screenHeight = 480; 

int main(int argc, char** argv) {

    

    //initializion functions
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);      						//enable double buffer mode
    glutInitWindowSize(screenWidth, screenHeight); 				//set the window's initial width and height
    glutInitWindowPosition(50, 50); 							//sets the window's position to top-left corner
    glutCreateWindow("Draw some text!"); 		//has to go after init size & position

    //callback functions - set up scene
    glutDisplayFunc(&display); 			// Callback for display refresh
    glutReshapeFunc(&reshape); 			// Callback for window resizing
	glutKeyboardFunc(&keyboardClick); 	// Callback function for key presses
	glutMouseFunc(&mouseClick); 		// Callback function for mouse clicks
	glutMotionFunc(&mouseMotion);		//callback function for mouse movement 
	glutTimerFunc(20, &timer, 20);		//redraw every 20 ms
    
	resetScene();
	//main loop
	glutMainLoop();
	return EXIT_SUCCESS;

    

}





void resetScene(){

	locationX = 0.0f;
	locationY = 0.0f;
	rotationX = 0.0f;
	rotationY = 0.0f;
	orbitRotation = 0;
	mouseX=0;
	mouseY=0;
	 blMouseLeftDown = false;	
	 blMouseMiddleDown = false;	
	 blMouseRightDown	= false;
	 blEnableLights = true;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  				     //Set the background color to black & opaque
    glClearDepth(1.0f);                     			    //Set the background to furthest away
    glEnable(GL_DEPTH_TEST);                			   //enable depth test for z-culling
    glDepthFunc(GL_LEQUAL);                				  //set the depth to be in front of the background
    glShadeModel(GL_SMOOTH);                			 //set to smooth shading (or glat!)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //Use best perspective correction

    enableLights();
    reshape(screenWidth, screenHeight);

}

void enableLights()
{
	if(blEnableLights)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);					//make it not grey
		GLfloat lightKa[] = {0.2f, 0.2f, 0.2f, 1.0f};	//ambient light
		GLfloat lightKd[] = {0.7f, 0.7f, 0.7f, 1.0f};	//diffuse light
		GLfloat lightKs[] = {1.0f, 1.0f, 1.0f, 1.0f};	//specular light
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);
		//position the light
		GLfloat lightPos[] = {0.0f, 0.0f, -20.0f, 1.0f};			//positional light
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
		glEnable(GL_LIGHT0);
		
	}
	else
		glDisable(GL_LIGHTING);
	
}

void reshape(GLsizei width, GLsizei height){
    
    if (height <= 0) height = 1;		//sanity
    if (width <= 0) width = 1;			//sanity
	
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
    
  	  glutReshapeWindow(width, height);
  	  

	//Set the viewport to cover the new window size
	glViewport(0, 0, width, height);
    //Set the aspect ratio for rendering to match the viewport
    glMatrixMode(GL_PROJECTION);            // Go into projection mode
    glLoadIdentity() ;                      // Reset stuff in current mode


    //Enable perspective projection mode with (fov, aspect, zNearClip, zFarCLip)

    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);

    

}



void display(void){
	reshape(screenWidth, screenHeight);
	enableLights();
		    
	char text[256];
	GLfloat textwidth = 0.0;
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);       // Go into model mode
    glEnable(GL_CULL_FACE);
    
    
	glLoadIdentity();	
	glTranslatef(0.0f, 0.0f, -7.0f);		//distance the axis of rotation
	
	//Draw the center object
	glPushMatrix(); 
	strcpy(text,"Nabeel Alkhatib");
	textwidth = getStrokeTextWidth(text,GLUT_STROKE_ROMAN);
	glRotatef(-orbitRotation, 0.0f, 1.0f, 0.0f);		//change the axis of rotation
	glScalef(0.005f, 0.005f, 0.005f);
	glTranslatef(-textwidth*0.5f, 0.0f, 0.0f);
	glLineWidth(3.0f);
	glColor3f(2.0f, 0.5f, 0.10f);
	drawStrokeText(text, GLUT_STROKE_ROMAN, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, screenWidth, screenHeight);
	//setup 2D coordinate mapping directly to windows pixles
	gluOrtho2D(0.0, (GLfloat)screenWidth, 0.0, (GLfloat)screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(1.0f);
	glColor3f(1.0, 3.0, 0.0);
	
	
	glPushMatrix();
	strcpy(text,"Bottom Left");
	drawBitmapText(text, GLUT_BITMAP_TIMES_ROMAN_24,0,0);
	glPopMatrix();
	
	glPushMatrix();
	strcpy(text,"Bottom right");
	textwidth = getBitmapTextWidth(text, GLUT_BITMAP_TIMES_ROMAN_24); 
	drawBitmapText(text, GLUT_BITMAP_TIMES_ROMAN_24, screenWidth-textwidth, 0);
	glPopMatrix();
	
	glPushMatrix();
	strcpy(text,"top Left");
	drawBitmapText(text, GLUT_BITMAP_TIMES_ROMAN_24,0,screenHeight-24);
	glPopMatrix();
	
	glPushMatrix();
	strcpy(text,"top right");
	textwidth = getBitmapTextWidth(text, GLUT_BITMAP_TIMES_ROMAN_24);
	drawBitmapText(text, GLUT_BITMAP_TIMES_ROMAN_24,screenWidth-textwidth,screenHeight-24);
	glPopMatrix();
	
	glPopMatrix();
	
	glutSwapBuffers();
}

void keyboardClick(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: case 'q': case'Q':
			exit(EXIT_SUCCESS);
			break;
		case 'l': case 'L':
				blEnableLights= !blEnableLights;
				glutPostRedisplay();
				break;
		case'r': case'R':
					resetScene();
					glutPostRedisplay();
					break;
		case 'd':
					locationX += 0.5;
					glutPostRedisplay();
					break;
		case 'a':
				locationX -= 0.5;
				glutPostRedisplay();
					break;
	}
}

void mouseClick(int button, int state, int x, int y)
{
	mouseX= x;
	mouseY= y;
	
	cout <<"button=" << button <<endl; 
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			blMouseLeftDown = true;
		else 
			blMouseLeftDown = false;
			
		if (button == GLUT_MIDDLE_BUTTON)
			if (state == GLUT_DOWN)
			blMouseMiddleDown = true;
		else 
			blMouseMiddleDown = false;
		
		if (button == GLUT_RIGHT_BUTTON)
			if (state == GLUT_DOWN)
			blMouseRightDown = true;
		else 
			blMouseRightDown = false;
	
}

void mouseMotion(int x, int y)
{
	cout << "x=" << x << "y=" << y <<endl;
	
	if (blMouseLeftDown)
	{	
		
		rotationX += (x-mouseX);
		rotationY -= (y-mouseY);		
	}
	
	if (blMouseRightDown)
	{	
		
		locationX += 10.0 * (x-mouseX) / screenWidth;
		locationY -= 10.0 * (y-mouseY) / screenHeight;		
	}
	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

void timer(int milisec)
{
	orbitRotation++;
	if(orbitRotation >=360) orbitRotation = 0;
	glutTimerFunc(milisec, &timer, milisec);	//Redraw every milisec ms
	glutPostRedisplay();
}

void drawCube()
{

    glBegin(GL_QUADS);
		
		//Top face (y = 1.0)

        glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);        //A
		glVertex3f( 1.0f,  1.0f, -1.0f);        //F
		glVertex3f(-1.0f,  1.0f, -1.0f);        //E
		glVertex3f(-1.0f,  1.0f,  1.0f);        //B

    

        //Bottom Face (y = -1.0f)

        glColor3f(0.5f, 0.5f, 1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);        //C
		glVertex3f(-1.0f, -1.0f, -1.0f);        //H
		glVertex3f( 1.0f, -1.0f, -1.0f);        //G
        glVertex3f( 1.0f, -1.0f,  1.0f);        //D

    

        //Front face (z = 1.0f)

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);        //A
        glVertex3f(-1.0f,  1.0f,  1.0f);        //B
        glVertex3f(-1.0f, -1.0f,  1.0f);        //C
        glVertex3f( 1.0f, -1.0f,  1.0f);        //D

    

        //Back Face (z = -1.0f)

        glColor3f(0.0f, 1.2f, 1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);        //E
        glVertex3f( 1.0f,  1.0f, -1.0f);        //F
        glVertex3f( 1.0f, -1.0f, -1.0f);        //G
        glVertex3f(-1.0f, -1.0f, -1.0f);        //H

    

        //Right Face (x = 1.0f)

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);        //A
        glVertex3f( 1.0f, -1.0f,  1.0f);        //D
        glVertex3f( 1.0f, -1.0f, -1.0f);        //G
        glVertex3f( 1.0f,  1.0f, -1.0f);        //F

    	// left face
		
		glColor3f(0.8f, 0.8f, 0.9f);
		glVertex3f(-1.0f,  1.0f,  1.0f);        //B
		glVertex3f(-1.0f,  1.0f, -1.0f);        //E
		glVertex3f(-1.0f, -1.0f, -1.0f);        //H
		glVertex3f(-1.0f, -1.0f,  1.0f);        //C
    
        
    glEnd();
}

 void drawPyramid()
{
	glBegin(GL_TRIANGLES);
//Triangle 1
    glColor3f(1.0f,0.0f,0.0f); glVertex3f( 0.0f, 1.0f, 0.0f);   //V0(red)
    glColor3f(0.0f,1.0f,0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);   //V1(green)
    glColor3f(0.0f,0.0f,1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);   //V2(blue)
  //Triangle 2
    glColor3f(1.0f,0.0f,0.0f); glVertex3f( 0.0f, 1.0f, 0.0f);   //V0(red)
    glColor3f(0.0f,0.0f,1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);   //V2(blue)
    glColor3f(0.0f,1.0f,0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);   //V3(green)
  //Triangle 3
    glColor3f(1.0f,0.0f,0.0f); glVertex3f( 0.0f, 1.0f, 0.0f);   //V0(red)
    glColor3f(0.0f,1.0f,0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);   //V3(green)
    glColor3f(0.0f,0.0f,1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);   //V4(blue)
  //Triangle 4
    glColor3f(1.0f,0.0f,0.0f); glVertex3f( 0.0f, 1.0f, 0.0f);   //V0(red)
    glColor3f(0.0f,0.0f,1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);   //V4(blue)
    glColor3f(0.0f,1.0f,0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);   //V1(green)
        
	glEnd();
	
	glBegin(GL_QUADS);
	glColor3f(0.0f, 4.0f, 0.5f);
	
	glVertex3f( -1.0f, -1.0f, 1.0f);
	glVertex3f( -1.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glEnd();
}
void drawStrokeText(char *text, void *font, GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	char *c;
	for (c=text; *c; c++)
	{
		
	glutStrokeCharacter(font, *c);
	
	}
	glPopMatrix();
}
GLfloat getStrokeTextWidth(char *text, void *font)
{
	
	GLfloat w = 0.0f;
	char *c;
	for (c=text; *c; c++)
	{
		
		w += glutStrokeWidth(font, *c);
	
	}
	return w;
}

void drawBitmapText(char *text, void *font, GLfloat x, GLfloat y)
{
	glPushMatrix();
	glRasterPos2f(x, y);
	char *c;
	for (c=text; *c; c++)
	{
		
		glutBitmapCharacter(font, *c);
	
	}
	glPopMatrix();
}
GLfloat getBitmapTextWidth(char *text, void *font)
{
	
	GLfloat w = 0.0f;
	char *c;
	for (c=text; *c; c++)
	{
		
		w += glutBitmapWidth(font, *c);
	
	}
	return w;
}
