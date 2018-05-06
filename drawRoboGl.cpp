/* 
 * Anýl Osman TUR

 * humanoid drawing and animating
 *
 * Requirements:
 * 1. Body, neck, arms, legs need to be done by cylinders
 * 2. Head should be a shpere
 * 3. At connections of body parts joints need to use for filling the gaps
 * 4. Arms need to be in two(2) part upper and lower parts but legs need to be in one(1) part
 * 5. lighting and shading need to be done
 * 6. for moving the camera on horizontal axis (left)<- ->(right) arrow keys will be used  
 * 7. for moving the camera on vertical axis (up)/\ \/(down) arrow keys will be used
 * 8. left click on mouse will start waving animation till clicked again.
 *    for waving 1 or 2 arms can be used 
 * 9. right click on mouse will start moving animation till clicked again.
 *    for moving can be on any axis and just the legs will animate 
 *    while humanoid moving not the arms.
 * 10. while walking waving animation can start or vice versa if user clicks.
 *
 * User inputs: 
 * - mouse:
 *    1. left click -> start and stop waving animation
 *    2. right click -> start and stop walking animation
 * - keys:
 *    1. left arrow -> move camera to left
 *    2. right arrow -> move camera to right
 *    3. up arrow -> move camera to up 
 *    4. down arrow -> move camera to down
 *    5. q, Q and esc -> close appliction 
 *    6. l, L -> open and close light
 *    7. w, W -> activete or deactivete right arm waving
 *    8. e, E -> activete or deactivete left arm waving
 *  
 * Animations:
 * - walking: after right mouse click model will walk in a square plane until another right mouse click.
 *     It will walk in that plane back and forth.
 * - waving: afert left mouse click waving will activate left arm waving its default one.
 *     Waving arm can be changed with 'w' or 'e' keys also it can wave with both arms.
 */

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


// camera values
float camCords[3] = {0.0, 10.0, 40.0}; // coordinates of the camera
float camUp[3] = {0.0, 1.0, 0.0};// y is the up vector 
float camMoveSpeed = 0.9;
float camAngle = 90.0;
float cam_yAngle = 30.0;
float camDist = 50.0;
float camDistY = 50.0;


// humanoid values
float humCords[3] = {0.0, 0.0, 0.0}; //coordinates I will also use it as camera's looking point.
bool walking = false;
bool waving = false;
bool humTurn = false;
float humAngle = 0.0;
float slices = 20, stack = 16;
float leg_h = 7.0, c_body_h = 7.0, neck_h = 0.7, arm_h = 3.0;
float c_body_w = 2.5, head_w = 2.1, arm_w = 0.7;
float canWalk = 50.0;

// arm movement values
bool rightArmWave = false, leftArmWave = true;
float up_arm_speed = 0.25, lo_arm_speed = 0.5;
// right arm
  bool r_arm_turn = false;
    // upper
    float up_r_arm_angle = 180.0; 
    float up_r_arm_turn_angle = 270.0, up_r_arm_start_angle = 180.0;
    // lower
    float lo_r_arm_angle = 0.0, lo_r_arm_start_angle = 0.0; // for non broken animation movement
    float lo_r_arm_stop_angle = 135.0, lo_r_arm_turn_angle = 45.0;
// left arm
  bool l_arm_turn = false;
    // upper
    float up_l_arm_angle = 180.0;
    float up_l_arm_turn_angle = 270.0, up_l_arm_start_angle = 180.0;
    // lower
    float lo_l_arm_angle = 0.0, lo_l_arm_start_angle = 0.0;// for non broken animation movement
    float lo_l_arm_stop_angle = 135.0, lo_l_arm_turn_angle = 45.0;


// leg movement values
bool leg_turn = false;
float walkingSpeed = 0.3;
float leg_dis = 0.0;
float d_height = 0.0, d_body_h = 5.0;
float leg_angle = 0.0, leg_start_angle = 0.0;
float leg_back_stop_angle = 45.0, leg_front_stop_angle = -45.0;

//light values
bool lightOn = true; // light is on if needed
GLfloat lightPos[] = {40.0, 40.0, 40.0, 1.0}; // position of light

// camera place initialization
void initCameraPlace(void)
{
	// for horizontal rotation
	camDist = camDistY * cos( cam_yAngle / 180.0 * M_PI ); // changing the distance vector of vertical rotation
	camCords[1] = camDistY * sin( cam_yAngle / 180.0 * M_PI ); // changing the y value of the camera
	// for vertical rotatation
	camCords[0] = camDist * cos( camAngle / 180.0 * M_PI ); // changing the x value of the camera
	camCords[2] = camDist * sin( camAngle / 180.0 * M_PI ); // changing the z value of the camera
}
// this will keep the camera angles in limits
void camAngleCheck(void)
{
	// vertical rotation angle limits
	if (camAngle > 360.0)
		camAngle = 0.0;
	else if (camAngle < 0.0)
		camAngle = 360.0;
	// horizontal rotation angle limits
	if(cam_yAngle > 360.0){
		cam_yAngle = 0.0;
    }else if (cam_yAngle < 0.0){
		cam_yAngle = 360.0;
	}
	// horizontal camera roation 
	if (cam_yAngle > 90.0){
		camUp[1] = -1.0;
		if (cam_yAngle > 270.0)
			camUp[1] = 1.0;
	}else{
		camUp[1] = 1.0;
	}
}
// mouse handler function
void mouse (int button, int state, int x, int y)
{
	switch (button)
	{
		// in case of mouse left button press start waving if it's waving stop it
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				if (waving){
					puts("waving false");
					waving = false;
				}else{
					puts("waving true");
					waving = true;
				}
			}
			break;
		// in case of mouse right button press start walking if it's walking stop it
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){
				if (walking){
					puts("walking false");
					walking = false;
				}else{
					puts("walking true");
					walking = true;
				}
			}
			break;
		default:
			break;
	}

  glutPostRedisplay();
}
// normal keyboard input methot function 
void keyboard (unsigned char key, int x, int y)
{
	switch ( key )
	{
		case 27: case 'q': case 'Q': // <esc> and q keys will close the app
			exit ( EXIT_SUCCESS );
			break;

		case 'l': case 'L': // just opens and close light
			if( lightOn ){
				lightOn = false;
				puts("light off");
			}else{
				lightOn = true;
				puts("light on");
			}
			break;

		case 'w': case 'W': // waving with right arm
			if(rightArmWave) { // stop right arm
				rightArmWave = false;
				puts("right arm stop");
			}else{ // start right arm
				rightArmWave = true;
				puts("right arm start");
			}
			break;

		case 'e': case 'E': // waving with left arm
			if(leftArmWave) { // stop letf arm
				leftArmWave = false;
				puts("left arm stop");
			}else{ // start left arm
				leftArmWave = true;
				puts("left arm start");
			}
			break;
		
		default:
			break;
	}
  glutPostRedisplay();
}
// taking inputs from arrow keys 
void special ( int key, int x, int y )
{   
	switch ( key )
	{
		case GLUT_KEY_LEFT: // left arrow key pressed
			// move  camera on x axis
			camAngle += camMoveSpeed;
			break;
		case GLUT_KEY_RIGHT: // right arrow key pressed
			// move camera on x axis
			camAngle -= camMoveSpeed;
			break;
		case GLUT_KEY_DOWN: // up arrow key pressed
			// move down the camera on y axis
			cam_yAngle -= camMoveSpeed;
			break;
		case GLUT_KEY_UP: // down arrow key pressed
			// move up the camera on y axis
			cam_yAngle += camMoveSpeed;
			break;
			
		default:
			break;
	}
	camAngleCheck();
    initCameraPlace();
	glutPostRedisplay();
}
// light placement
void putLight( void )
{
	GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shiny[] = {50.0};
	GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};

	// materail's reaction to light
	glShadeModel (GL_SMOOTH);
	glMaterialfv (GL_FRONT, GL_SPECULAR,spec);
    glMaterialfv (GL_FRONT, GL_SHININESS,shiny);
    // light spesifics
	glLightfv (GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv (GL_LIGHT0, GL_SPECULAR, lightColor);

	//for positioning light
	if (lightOn)
	{
		glPushMatrix ();
			glLightfv (GL_LIGHT0, GL_POSITION, lightPos); // positioning the light
			glTranslated (lightPos[0], lightPos[1], lightPos[2]);
			glDisable (GL_LIGHTING);
			glColor3f (1.0, 1.0, 0.0);
			glutSolidSphere (5.0, 40, 16);// to see where is the representative light I put a yellow sphere onto it
			glEnable (GL_LIGHTING);
		glPopMatrix ();
		glColorMaterial(GL_FRONT,GL_DIFFUSE);
		glEnable (GL_COLOR_MATERIAL);
		glEnable (GL_LIGHT0);
	}
	else
	{
		glDisable (GL_LIGHT0);
	}
}
// just draws a plane surface
void drawSquarePlane(float size)
{
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
		glVertex3f(-size, 0.0, size);
		glVertex3f(size, 0.0, size);
		glVertex3f(size, 0.0, -size);
		glVertex3f(-size, 0.0, -size);
	glEnd();
}
// to draw cyliders easier
void drawCylinder( GLUquadricObj *p , float radius, float height) // float x, float y, float z
{
	//glTranslated(x,y,z);
	glPushMatrix();
		glRotated (-90, 1.0, 0.0, 0.0); // this will make the cylinder straight position
		gluDisk(p ,0.0 , radius, slices, stack);// to close the cylinder I put a disk to the buttom
		gluCylinder(p, radius, radius, height, slices, stack); //p, f base_r, f top_r, f height,int slices, int stacks
		glTranslated(0.0,0.0,height); // to close the cylinder I put disk on it
		gluDisk(p ,0.0 , radius, slices, stack);//p, f innerRadius, f outerRadius, int slices, int loops
	glPopMatrix ();
}
// draw a joint on a size
void drawJoint(float size)// float x, float y, float z
{
	//glTranslated(x, y, z);
	glutSolidSphere(size, slices, stack);
}
// draws the homanoid on needed place
void drawHumanoid(void)
{
	GLUquadricObj *p = gluNewQuadric(); 
	gluQuadricDrawStyle(p, GLU_FILL);

	glTranslated(humCords[0],humCords[1],humCords[2]); // movement
	glRotated (humAngle, 0.0, 1.0, 0.0); // maybe we can spin it lol
	
	glPushMatrix ();
		// drawing legs
		// left leg
		glPushMatrix ();
			glTranslated(-1.4, 0.0, -leg_dis);
			// to move leg rotate
			glRotated (leg_angle, 1.0, 0.0, 0.0); // for leg movenent
			glColor3f(1.0, 0.0, 0.0);
			drawCylinder( p , 1.0, leg_h);
			glTranslated(0.0, leg_h, 0.0);
			drawJoint(1.2);
		glPopMatrix ();
		// right leg
		glPushMatrix ();
			glTranslated(1.4, 0.0, leg_dis);
			// to move leg rotate
			glRotated (-leg_angle, 1.0, 0.0, 0.0); // this will be the opposite of other one 
			glColor3f(1.0, 0.0, 0.0);
			drawCylinder( p , 1.0, leg_h);
			glTranslated(0.0, leg_h, 0.0);
			drawJoint(1.2);
		glPopMatrix ();
		glTranslated(0.0, d_height, 0.0); // translete to height of legs
		// body
		// centeral body
		glColor3f(1.0, 0.0, 0.0);
		drawCylinder( p , c_body_w, c_body_h); // draw central body
		glTranslated(0.0, c_body_h, 0.0); // translete to height of central body
		// upper body
		glPushMatrix ();
			// neck
			glColor3f(1.0, 0.0, 0.0);
			drawCylinder( p , 1.0, 0.7);
			// head
			glTranslated(0.0, neck_h + 1.7, 0.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(head_w, slices, stack); //scale, slices, stacks
			// nose to see where is front 
			glTranslated(0.0,0.0,head_w);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.3, slices, stack);
		glPopMatrix ();
		// Arms
		glTranslated(0.0, -0.5, 0.0); // translete to height of central body
		//right arm
		glPushMatrix ();
			//upper
			glTranslated(-(arm_w + c_body_w + 0.1), 0.0, 0.0);
			// to move arm rotate
			glRotated (up_r_arm_angle, 0.0, 0.0, -1.0);
			glColor3f(1.0, 0.0, 0.0);
			drawJoint(1.1);
			drawCylinder( p , arm_w, arm_h);
			glTranslated(0.0, arm_h , 0.0);
			// lower
			// to move arm rotate
			glRotated (lo_r_arm_angle, 0.0, 0.0, -1.0);
			glColor3f(1.0, 0.0, 0.0);
			drawJoint(0.8);
			drawCylinder( p , arm_w, arm_h);
			glTranslated(0.0, arm_h , 0.0);
		glPopMatrix ();
		//left arm
		glPushMatrix ();
			//upper
			glTranslated((arm_w + c_body_w + 0.1), 0.0, 0.0);
			// to move arm rotate
			glRotated (up_l_arm_angle, 0.0, 0.0, 1.0);
			glColor3f(1.0, 0.0, 0.0);
			drawJoint(1.1);
			drawCylinder( p , arm_w, arm_h);
			glTranslated(0.0, arm_h , 0.0);
			// lower
			// to move arm rotate
			glRotated (lo_l_arm_angle, 0.0, 0.0, 1.0);
			glColor3f(1.0, 0.0, 0.0);
			drawJoint(0.8);
			drawCylinder( p , arm_w, arm_h);
			glTranslated(0.0, arm_h , 0.0);
		glPopMatrix ();
	glPopMatrix ();
}

// this will draw the enviroment to the screen buffers
void display (void)
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ready to new frame by cleaning the buffers
	glPushMatrix ();
		glLoadIdentity();
		gluLookAt(camCords[0],camCords[1],camCords[2], // placement of camera
				  humCords[0],humCords[1]+10.0,humCords[2], // looking point it will locked to humanoid
				  camUp[0],camUp[1],camUp[2]);
		putLight(); // this will handle the light
		drawSquarePlane(canWalk);
		drawHumanoid();
		glEnable(GL_DEPTH_TEST);
	glPopMatrix ();

	glutSwapBuffers();
}

// waving animation handler function
void wavingAnimation( bool* arm_turn, 
					  float* up_arm_angle,
					  float* up_arm_turn_angle,
					  float* lo_arm_angle,
					  float* lo_arm_turn_angle,
					  float* lo_arm_stop_angle)
{
	if (*up_arm_angle < *up_arm_turn_angle){ //upper arm did't placed yet
		*up_arm_angle += up_arm_speed;
	}else{ //upper arm placed
		if (*lo_arm_angle < *lo_arm_stop_angle){
			if (*arm_turn){
				if (*lo_arm_angle > *lo_arm_turn_angle){
					*lo_arm_angle -= lo_arm_speed;
				}else{
					*arm_turn = false;
				}
			}else{
				*lo_arm_angle += lo_arm_speed;
			}
		}else{
			*arm_turn = true;
			*lo_arm_angle -= lo_arm_speed;
		}
	}

}
// wave stopping animation handler function
void stopWavingAniamation( float* up_arm_angle,
						   float* up_arm_start_angle,
						   float* lo_arm_angle,
						   float* lo_arm_start_angle,
						   float* lo_arm_stop_angle)
{
	if (*up_arm_angle > *up_arm_start_angle){ // upper arm not in place
		*up_arm_angle -= up_arm_speed;
		if(*lo_arm_angle > *lo_arm_start_angle){ // lower arm not in place
			*lo_arm_angle -= lo_arm_speed;
		}else{
			*lo_arm_angle = *lo_arm_start_angle;
		}
	}else{ // upper arm in place
		if(*lo_arm_angle > lo_l_arm_start_angle){ // lower arm not in place
			*lo_arm_angle -= lo_arm_speed;
		}else{
			*lo_arm_angle = *lo_arm_start_angle;
		}
		*up_arm_angle = *up_arm_start_angle;
	}

}
// model will move inside of the square plane
void walkInSquare(void){
	if(humTurn){
		humAngle = 180.0;
		leg_dis = leg_h * sin(leg_angle / 180.0 * M_PI ); // moved distance
		if (leg_dis < 0.0){
			humCords[2] -= -leg_dis * 0.01;
		}else{
			humCords[2] -= leg_dis * 0.01;
		}
		if (-canWalk > humCords[2]){
			humTurn = false;
		}
	}else{
		humAngle = 0.0;
		leg_dis = leg_h * sin(leg_angle / 180.0 * M_PI ); // moved distance
		if (leg_dis < 0.0){
			humCords[2] += -leg_dis * 0.01;
		}else{
			humCords[2] += leg_dis * 0.01;
		}
		if (canWalk < humCords[2]){
			humTurn = true;
		}
	}
}
// this will handles the movement changes
void idle( void )
{
	if (walking){ // walking animation
		if (leg_angle < leg_back_stop_angle){ // one leg will move front the other will go back
			if (leg_turn){ // leg will move front now
				if (leg_angle > leg_front_stop_angle){
					leg_angle -= walkingSpeed;
				}else{
					leg_turn = false;
				}
			}else{ // it haven't placed yet
				leg_angle += walkingSpeed;
			}
		}else{
			leg_turn = true;
			leg_angle -= walkingSpeed;

		}
		d_height = leg_h * cos(leg_angle / 180.0 * M_PI ); // up and down movement
		walkInSquare();
		
	}else{ // stopping animation
		if (leg_angle < leg_start_angle){ // legs haven't placed yet
			leg_angle += walkingSpeed;
			if (leg_angle > leg_start_angle){
				leg_angle = leg_start_angle;
			}
		}else if (leg_angle > leg_start_angle){
			leg_angle -= walkingSpeed;
			if (leg_angle < leg_start_angle){
				leg_angle = leg_start_angle;
			}
		}else{
			leg_angle = leg_start_angle;
		}
		d_height = leg_h * cos(leg_angle / 180.0 * M_PI ); // up and down movement
		walkInSquare();
	}

	if (waving){
		if (rightArmWave){
			wavingAnimation(&r_arm_turn, &up_r_arm_angle, &up_r_arm_turn_angle,
							&lo_r_arm_angle, &lo_r_arm_turn_angle, &lo_r_arm_stop_angle);
		}else{
			stopWavingAniamation(&up_r_arm_angle, &up_r_arm_start_angle, &lo_r_arm_angle,
								 &lo_r_arm_start_angle, &lo_r_arm_stop_angle);
		}
		if(leftArmWave){
			wavingAnimation(&l_arm_turn, &up_l_arm_angle, &up_l_arm_turn_angle,
							&lo_l_arm_angle, &lo_l_arm_turn_angle, &lo_l_arm_stop_angle);
		}else{
			stopWavingAniamation(&up_l_arm_angle, &up_l_arm_start_angle, &lo_l_arm_angle,
								 &lo_l_arm_start_angle, &lo_l_arm_stop_angle);
		}
	}else{	// stoping the waving
		stopWavingAniamation(&up_r_arm_angle, &up_r_arm_start_angle, &lo_r_arm_angle,
						     &lo_r_arm_start_angle, &lo_r_arm_stop_angle);
		stopWavingAniamation(&up_l_arm_angle, &up_l_arm_start_angle, &lo_l_arm_angle,
						     &lo_l_arm_start_angle, &lo_l_arm_stop_angle);
	}// waving
	glutPostRedisplay();
}
// reshaping function will handle window reshape
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (65.0, (GLfloat) w / (GLfloat) h, 1.0, 100);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (0.0, 0.0, -5.0);
}

int main(int argc,char *argv[])
{
	// standard glut initialization
	glutInit ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	// arrange the window size and position
    glutInitWindowSize(1024,680);
	glutInitWindowPosition ( 300, 0);
	glutCreateWindow ("window");
    // reshape function call
	glutReshapeFunc (reshape);
    // good a bit dark blue backround
    glClearColor( 0.0, 0.0,  0.4 ,1 );
	// display function init
	initCameraPlace(); // first camera place initialization
	glutDisplayFunc ( display );
	// input handler functions
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( special );
	glutMouseFunc( mouse );
	// idle function init
	glutIdleFunc ( idle );

	// starts the loop and actually ends the glut initialization
	glutMainLoop();
	return EXIT_SUCCESS;
}