/* 
 * Anýl Osman TUR 
 * 
 * Terrain drawing  
 * Requirements:
 * 1. Day and night effect with idle function
 * 2. if user presses to h letter get colorize the terrain with height values
 *    red,    if h >= 80 
 *    yellow, if 80 > h >= 50
 *    green,  if 50 > h >= 0
 * 3. rotation with arrow keys
 *
 * key featurs:
 *  - 'g' and 'G' : is changing the drawing styles by triangle ,line strip and point 
 *  - 'h' and 'H' for coloring
 *  - 'q' 'Q' 'ESC' for quiting
 *  - Right and Left arrow keys are for rotating
 *  - Up and Down arrow keys for getting closer to terrain or moving apart from terrain
 */

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

// camera values
float camCords[3] = {-600, 300, 0}; // coordinates of the camera
float camIsLookingTo[3] = {0.0, 100.0, 0.0}; // it is looking to a bit higher to the origin
float camUp[3] = {0.0, 0.1, 0.0};// y is the up vector 
float camAngle = 180.0;
float camDist = 600.0;

// drawing params  
bool isColored = false; // for deciding to colorize
int drawingMode = GL_TRIANGLE_STRIP; // drawing switch
int drawCount = 0; // drawing switch count with this we just hit one key to change in modes

// day light and night light values
float dayTime = 0.05;
float nightTime = 0.;
float sunAngle = 180.0; // the light source's angle as a sun
float moonAngle = 180.0;//the light source 's angle as a moon
bool sunLightOn = true; // sun rised
bool moonLightOn = false; // moon is down and will wait sunset
GLfloat sunLightPos[] = {-600.0, 0.0, 200.0, 1.0}; // position of sun
GLfloat mooLightPos[] = {-600.0, 0.0, 200.0, 1.0}; // position of moon

// height matrix
int rows = 17;
int cols = 21;
int samplingGap = 20;
int centerX = 210;
int centerZ = 170;

// data height matrix 
int height[17][21] = {14 ,25 ,45 ,55 ,68, 70, 84, 91, 97, 101, 105, 105, 105, 105, 110, 110, 110, 110, 110, 110, 110,
                5 ,18, 43, 62, 73, 82, 88, 94, 99, 102, 105, 105, 105, 105, 110, 110, 110, 110, 110, 110, 110,
                5, 18, 38, 56, 69, 77, 86, 94, 99, 103, 106, 105, 105, 105, 110, 110, 110, 110, 110, 110, 110,
                5,  9, 31, 48, 60, 71, 81, 87, 95, 101, 106 ,105, 105, 105, 110 ,110, 110, 110, 110, 110, 110,
                5,  5, 18, 37, 49 ,56, 62, 81, 91 ,94 ,101 ,105 ,105 ,105 ,110, 110, 110, 110, 110, 110, 110,
                5,  5, 12, 23 ,34 ,40, 53 ,66, 77 ,82, 97 ,103 ,105 ,105 ,109, 110, 110, 110, 110, 115, 115,
                4,  5,  8, 15 ,20 ,24, 35 ,39, 40 ,77 ,92 ,101 ,104 ,104 ,105, 110, 110, 110 ,115, 115, 115,
                5,  7, 22, 36 ,46 ,48, 48 ,44, 50 ,58 ,80 ,96 ,96 ,97 ,106, 110, 110 ,115 ,115, 115, 115,
                4, 15, 31, 46 ,61 ,68, 69 ,63, 53 ,50 ,67 ,82 ,84 ,103, 108, 110, 110 ,115, 115, 115, 115,
                4, 12, 31, 46 ,64 ,78, 82 ,80, 69 ,54 ,73 ,71 ,92 ,105 ,108 ,110 ,110 ,115 ,115 ,115, 115,
                6, 26, 35, 45 ,63 ,75, 84 ,87, 84 ,74 ,77 ,80 ,96 ,103 ,108 ,110 ,110 ,110, 115, 115, 115,
    21, 30, 46, 57 ,64 ,76, 85  ,92, 92, 87 ,79 ,80 ,86 ,102 ,106 ,110 ,105 ,110 ,115 ,115 ,115,
    27, 40, 48, 62 ,75 ,84, 92  ,96 ,97 ,94 ,88 ,80 ,80 ,91 ,104, 105 ,105 ,105, 110, 115, 115,
    33, 43, 55 ,65 ,75 ,87, 96  ,101 ,101, 101, 97, 92, 80, 80, 98 ,105 ,105 ,105 ,105 ,110 ,115,
    45, 50, 58 ,68 ,80 ,91 ,99  ,102 ,105 ,105 ,105, 99, 90, 80, 80, 97 ,105, 105, 105, 110, 100,
    50, 60, 65 ,71 ,84 ,95 ,101 ,105 ,105 ,107 ,107 ,106, 102, 101, 92 ,80 ,98 ,104 ,105 ,100, 100,
    60, 70, 76 ,83 ,88 ,96 ,103 ,106 ,107 ,108 ,110 ,109 ,108 ,108 ,106, 101 ,90 ,100, 100, 100, 100};
/**************************************************/
/* end of global declarations */

// taking inputs from arrow keys 
void special ( int key, int x, int y )
{   
	switch ( key )
	{
		case GLUT_KEY_LEFT: // left arrow key pressed
			// rotate counter clockwise by y axis
			camAngle -= 0.7;
			break;
		case GLUT_KEY_RIGHT: // right arrow key pressed
			// rotete clockwise by y axis
			camAngle += 0.7;
			break;
		case GLUT_KEY_DOWN: // up arrow key pressed
			// get closer to origin
			camDist *= 1.1;
			break;
		case GLUT_KEY_UP: // down arrow key pressed
			// move out a bit from origin
			camDist /= 1.1;
			break;
			
		default:
			break;
	}
	if (camAngle > 360.0)
		camAngle = 0.0;
	camCords[0] = camDist * cos( camAngle / 180.0 * M_PI ); // changing the x value of the camera
	camCords[2] = camDist * sin( camAngle / 180.0 * M_PI ); // changing the z value of the camera
	glutPostRedisplay();
}

// input methot functions 
void keyboard (unsigned char key, int x, int y){
	switch ( key )
	{
		case 27: case 'q': case 'Q': // <esc> and q keys will close the app
			exit ( EXIT_SUCCESS );
			break;
			
		case 'h': case 'H': // open | close color
			if ( isColored )
				isColored = false;
			else
				isColored = true;
			break;
			
		case 'g': case 'G': // change drawing methot in different way
			if(drawCount > 2 || drawCount == 0) {
				drawingMode = GL_LINE_STRIP;
				drawCount = 1;
			}else if(drawCount > 1) {
				drawingMode = GL_TRIANGLE_STRIP;
				drawCount++;
			}else {
				drawingMode = GL_POINTS;
				drawCount++;
			}
			break;
		
		default:
			break;
	}
	glutPostRedisplay();
}

// idle function for day and night affect 
void idle ( void ) 
{
	sunAngle += dayTime;
	if ( sunAngle > 360.0 )
		{
		sunAngle = 0.0;
		if ( sunLightOn == true && moonLightOn == false) 
		{
			sunLightOn = false;
			moonLightOn = true;
			glDisable ( GL_LIGHT0 ); // sunset
			glEnable ( GL_LIGHT1 ); // moon rise
		}
	}
	else if ( sunAngle >  180.0 )
	{
		// sunrise operations 
		if ( sunLightOn == false )
		{
		    sunLightOn = true;
			moonLightOn = false;
			glDisable ( GL_LIGHT1 ); // moon down
			glEnable ( GL_LIGHT0 ); // sun rise	
		}
			
	}
	else  // sun angle is < 180
	{
		//after sunset while the sun turning around and  moon will shine till sun rise
		moonAngle += dayTime;
		if ( moonAngle > 360.0 )
		{
			moonAngle = 180.0;
		}
	}
glutPostRedisplay();
}


/* color choocing function by taking the height as param
 * and decide whivh color to use */ 
void heightColor ( int h )
{
	if ( isColored )
	{
		if ( h >= 80 )
			glColor3f ( 1.0, 0.0, 0.0 ); // red
		else if ( h >= 50 )
			glColor3f ( 1.0, 1.0, 0.0 ); // yellow
		else if ( h >= 0 )
			glColor3f ( 0.0, 1.0, 0.0); // green
		else
			glColor3f ( 0.0, 0.0, 1.0); // if there is negative number now we will see it with blue
	}
	else
		glColor3f ( 0.6, 0.6, 0.6 ); // draw a shade of gray
}


/* light placement to create day and night affect */
void bringDayNight ( void )
{
    GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shiny[] = {50.0};
    GLfloat sunLight[] = {1.0, 1.0, 0.9, 1.0};
    GLfloat moonLight[] = {0.2, 0.2, 0.2, 1.0};
  
    glShadeModel (GL_SMOOTH);
    glMaterialfv (GL_FRONT, GL_SPECULAR,spec);
    glMaterialfv (GL_FRONT, GL_SHININESS,shiny);
    // sun light spesifics
	glLightfv (GL_LIGHT0, GL_DIFFUSE, sunLight);
    glLightfv (GL_LIGHT0, GL_SPECULAR, sunLight);
	// moon light spesifics
	glLightfv (GL_LIGHT1, GL_DIFFUSE, moonLight);
    glLightfv (GL_LIGHT1, GL_SPECULAR, moonLight);

	// for positioning the sun and the moon
	if ( sunLightOn ) // sun is shining
	{
		glPushMatrix ();
			glRotated (sunAngle, 0.0, -0.5, 1.0); // rotate by z-y axis 
			glLightfv (GL_LIGHT0, GL_POSITION, sunLightPos); // positioning the sun light
			glTranslated (sunLightPos[0], sunLightPos[1], sunLightPos[2]);
			glDisable (GL_LIGHTING);
			glColor3f (1.0, 1.0, 0.0);
			glutSolidSphere (20.0,40, 16);// to see where is the representative sun light i put a yellow sphere onto it
			glEnable (GL_LIGHTING);
		glPopMatrix ();
		glColorMaterial(GL_FRONT,GL_DIFFUSE);
		glEnable (GL_COLOR_MATERIAL);
		glEnable (GL_LIGHT0);
	}
	else // moon is shining now
	{
		glPushMatrix ();
			glRotated ( moonAngle, 0.0, -0.5, 1.0); // rotate by z-y axis
			glLightfv (GL_LIGHT1, GL_POSITION, mooLightPos); // positioning the moon light
			glTranslated (mooLightPos[0], mooLightPos[1], mooLightPos[2]);
			glDisable (GL_LIGHTING);
			glColor3f (0.4, 0.4, 0.4);
			glutSolidSphere (5.0,40, 16); // to see where is the representative moon light i put a gray sphere onto it
			glEnable (GL_LIGHTING);
		glPopMatrix ();
		glColorMaterial(GL_FRONT,GL_DIFFUSE);
		glEnable (GL_COLOR_MATERIAL);
		glEnable (GL_LIGHT1);
	}
}

void drawTerrain ( void )
{
	int x, z;
	
	/*
     *	with two different triangle we will go though the matix
	 * as x(s) we use second indexes of height with added sampling gap
	 * as y(s) we do the same but this time with first indexes
	 * to postpond the center of the terrain to the origin by subtrackting the center values
	 */
	for ( z = 0; z < rows - 1; z++ ) 
	{
		for ( x = 0; x < cols - 1; x++)
		{
			glBegin( drawingMode );
				heightColor( height[z+1][x] );
				glVertex3f (  x * samplingGap - centerX, height[z+1][x], ( z + 1) * samplingGap - centerZ); // begin at 1,0
				heightColor( height[z+1][x+1] );
				glVertex3f (  (x + 1)  * samplingGap - centerX, height[z+1][x+1], (z + 1) * samplingGap - centerZ); // one left 1,1
				heightColor( height[z][x+1] );
				glVertex3f (  (x + 1) * samplingGap - centerX, height[z][x+1], z * samplingGap - centerZ); // to up 0,1
				heightColor( height[z][x] );
				glVertex3f (  x * samplingGap - centerX, height[z][x], z * samplingGap - centerZ); // start  at 0,0
				heightColor( height[z+1][x] );
				glVertex3f (  x * samplingGap - centerX, height[z+1][x], (z + 1) * samplingGap - centerZ); // to one step down 1,0
				heightColor( height[z][x+1] );
				glVertex3f (  (x + 1) * samplingGap - centerX, height[z][x+1], z * samplingGap - centerZ); // to norteast (up+left) 0,1 
			glEnd();
		}
		
	}
	
}

// this will draw the enviroment to the screen buffers
void display(void)
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ready to new frame by cleaning the buffers
    
	glPushMatrix ();
	  glLoadIdentity();
	  //glTranslatef (0.0, 0.0, 50.0);
	  gluLookAt(camCords[0],camCords[1],camCords[2],0,0,0,0,1,0); // initializing a camera
	  bringDayNight(); // it will handle the day night process in other way the lights
	  drawTerrain (); // it will draw the terrain
	  glEnable(GL_DEPTH_TEST);
	glPopMatrix ();
    glutSwapBuffers(); 
}  

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //gluPerspective(40,1,100,-300);
  gluPerspective (30.0, (GLfloat) w / (GLfloat) h, 100.0, 2000.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

int main(int argc,char *argv[])
{
	// standard glut initialization
	glutInit ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	
	// arrange the window size and position
    glutInitWindowSize(1000,800);
	glutInitWindowPosition ( 300, 0);
	glutCreateWindow ("window");
    // reshape function call
	glutReshapeFunc (reshape);
    // good a bit dark blue backround
    glClearColor( 0.0, 0.0,  0.4 ,1 );
	// display function init
	glutDisplayFunc ( display );
	// input handler functions
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( special );
	// idle function init
	glutIdleFunc ( idle );

	// starts the loop and actually ends the glut initialization
	glutMainLoop();
	return EXIT_SUCCESS;
}