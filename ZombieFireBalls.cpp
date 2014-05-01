//-----------------------------------------------------------------------
//      File:           sample1cpp.cpp
//      Description:    A sample OpenGL program
//      Programmer:     Dave Mount
//      For:            CMSC 427 - Computer Graphics
//      Date:           Sep 2011
//
//      This is just a sample skeleton C++ program, which shows the
//      general structure of a minimal OpenGL program.
//-----------------------------------------------------------------------

#include <cstdlib>                      // standard definitions
#include <iostream>                     // C++ I/O
#include <cstdio>                       // C I/O (for sprintf) 
#include <cmath>                        // standard definitions
#include <math.h>
#include <list>

#include <GL/glut.h>                    // GLUT
#include <GL/glu.h>                     // GLU
#include <GL/gl.h>                      // OpenGL

#define PI 3.14159265358979

using namespace std;                    // make std accessible
double rm = double(RAND_MAX);
float screenWidth = 1.0f;
float height = 600.0f;
bool gameover = false;

//-----------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------

GLint TIMER_DELAY = 1;                      // timer delay (10 seconds)
GLfloat RED_RGB[] = {1.0, 0.0, 0.0};            // drawing colors
GLfloat BLUE_RGB[] = {0.0, 0.0, 1.0};
GLfloat GREEN_RGB[] = {0.0, 1.0, 0.0};
GLfloat D_GREEN_RGB[] = {0.0, 0.5, 0.0};

GLfloat L_GREEN[] = {0.0, 0.4, 0.0};
GLfloat LL_GREEN[] = {0.0, 0.25, 0.0};


//-----------------------------------------------------------------------
//	Classes
//-----------------------------------------------------------------------

class Fireball {
float originX, originY;
  float radius;
  float moveY;
public:
  Fireball(float, float, float, float);
  
  void move(){
    originY += moveY;
  }

  float getOX(){
	return originX;
  }

  float getOY(){
	return originY;
  }
  
  float getEdgeX(int a){
	return originX+radius*(cos(a*PI/10));
  }

  float getEdgeY(int a){
	return originY+radius*(sin(a*PI/10));
  }

  float getR(){
	return radius;
  }

  bool hit(){
	radius = radius/2.0;
	if (radius < 0.01){
		return true;
	}
	else{
		return false;
	}
  }

  int outOfBounds(){
	if (originY-radius > 1){
		return 1;
	}
	else {
		return 0;
	}
  }

  void draw(){
    int sides = 20;
    glBegin(GL_POLYGON);
      for (int c = 0; c < sides; c++){
// 	cout<<"sin: "<<sin(c*PI/180)<<" cos: "<<cos(c*PI/180)<<endl;
// 	cout<<"Xedge: "<<originX+radius*(cos(c*PI/180))
	// <<" Yedge: "<<originY+radius*(sin(c*PI/180))<<endl;
	glVertex2f(originX+radius*(cos(c*PI/10)),
		   originY+radius*(sin(c*PI/10))); 
      }
    glEnd();
  }
};

	
class Droplet {
  float originX, originY;
  float radius;
  float moveX;
  float moveY;
public:
  Droplet(float, float, float, float, float);
  
  void move(){
    originX += moveX;
    originY += moveY;
  }

  int outOfBounds(){
	if (originY-radius <= 0 || originY+radius >= 1){
		return 1;
	}
	else if (originX-radius <= 0){
		return 1;
	}
	else if (originX+radius >= screenWidth){
		return 1;
	}
	else {
		return 0;
	}
  }
	
  float getEdgeX(int a){
	return originX+radius*(cos(a*PI/10));
  }

  float getEdgeY(int a){
	return originY+radius*(sin(a*PI/10));
  }

	float getOX(){
		return originX;
	}

	float getOY(){
		return originY;
	}
  
  float getR(){
	return radius;
  }

  void draw(){
    int sides = 20;
    glBegin(GL_POLYGON);
      for (int c = 0; c < sides; c++){
	glVertex2f(originX+radius*(cos(c*PI/10)),
		   originY+radius*(sin(c*PI/10))); 
      }
    glEnd();
  }
};

Fireball::Fireball(float cX, float cY, float r, float dY){
  originX = cX;
  originY = cY;
  radius = r;
  moveY = dY;
}

Droplet::Droplet(float cX, float cY, float r, float dX, float dY){
  originX = cX;
  originY = cY;
  radius = r;
  moveX = dX;
  moveY = dY;
}

list<Droplet> dropList; //Global Lists
list<Fireball> fireList;
int fire = 0;



class Hose {
	float x, y;
	float length;
	float width;
	float angle;
	int ammo;
	
	public:
		Hose(float, float, float, float);
		
		void changeX(float dx){
			if (x + dx > 0.02f && x + dx < screenWidth-.02 && !gameover){
				x = x + dx;
			}
		}

		float getX(){
			return x;
		}
		
		void changeY(float dy){
			y = y + dy;
		}
		float getY(){
			return y;
		}

		float getLength(){
			return length;
		}

		void setAngle(int mX, int mY){
			if (!gameover)
			angle = -1*180/PI*atan2(mY-((1-y)*height), mX-(height*x));
		}
    

		float getAngle(){
			return angle;
		}

		void shootDroplet(){
		   	if (ammo > 0){
				float velocity = .01;
				float dX = velocity*cos(angle*PI/180);
				float dY = velocity*sin(angle*PI/180);
				float oX = x+(length*cos(angle*PI/180))+dX;
				float oY = y+(length*sin(angle*PI/180))+dY;
				Droplet d (oX, oY, .01f, dX, dY);
				dropList.push_back(d);
				--ammo;
			}
		}

		void reload(){
			ammo = 100;
		}

		void draw(){
			//Draw the hose
			glPushMatrix();
			glTranslatef(x, y, 0.0f);
			glRotatef(angle, 0.0f, 0.0f, 1.0f);
			glRectf(0.0f, -width/2.0f, length, width/2.0f);
			glPopMatrix();
		}
};

Hose::Hose(float cx, float cy, float l, float w){
	x = cx;
	y = cy;
	length = l;
	width = w;
	angle = -90.0f;
	ammo = 100;
}

static Hose myHose(0, 0, 1, 1);

class Tank{
	float x, y;
	float angle;
	float width;
	int treds;
	GLfloat *F_Tread;
	GLfloat *B_Tread;
	public:
		Tank(float, float, float);
		
		void moveX(float dx){
			GLfloat *temp = F_Tread;
			F_Tread = B_Tread;
			B_Tread = temp;
			if (x+dx+.09 <= screenWidth && x+dx-.09 >= 0){
				x = x + dx;
				myHose.changeX(dx);
			}
		}

		void moveY(float dY){
			GLfloat *temp = F_Tread;
			F_Tread = B_Tread;
			B_Tread = temp;
			if (y+dY+.09 <= 1 && y+dY-.09 >= 0){
				y = y + dY;
				myHose.changeY(dY);
			}
		}

		void setAngle(float a){
			angle = a;
		}

		void draw(){
			glColor3fv(D_GREEN_RGB);
			glPushMatrix();
			glTranslatef(x, y, 0.0f);
			glRotatef(angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_POLYGON);
				glVertex2f(-0.1f, -.07f);
				glVertex2f(0.06f, -0.07f);
				glVertex2f(0.1f, -0.045f);
				glVertex2f(0.1f, 0.045f);
				glVertex2f(0.06f, 0.07f);
				glVertex2f(-0.1f, 0.07f);	
			glEnd();
			// left treads
			glColor3fv(F_Tread);
			glRectf(-.1f, -.09f, -.075f, -.07f);
			glColor3fv(B_Tread);
			glRectf(-.075f, -.09f, -.05f, -.07f);
			glColor3fv(F_Tread);
			glRectf(-.05f, -.09f, -.025f, -.07f);
			glColor3fv(B_Tread);
			glRectf(-.025f, -.09f, 0.0f, -.07f);
			glColor3fv(F_Tread);
			glRectf(.0f, -.09f, .025f, -.07f);
			glColor3fv(B_Tread);
			glRectf(.025f, -.09f, 0.05f, -.07f);
			//right treads
			glColor3fv(F_Tread);
			glRectf(-.1f, .07f, -.075f, .09f);
			glColor3fv(B_Tread);
			glRectf(-.075f, .07f, -.05f, .09f);
			glColor3fv(F_Tread);
			glRectf(-.05f, .07f, -.025f, .09f);
			glColor3fv(B_Tread);
			glRectf(-.025f, .07f, 0.0f, .09f);
			glColor3fv(F_Tread);
			glRectf(.0f, .07f, .025f, .09f);
			glColor3fv(B_Tread);
			glRectf(.025f, .07f, 0.05f, .09f);
			//glRectf(0.0f, -width/2.0f, 0.14f, width/2.0f);
			glPopMatrix();
		}
};

Tank::Tank(float cx, float cy, float w){
	x = cx;
	y = cy;
	width = w;
	angle = -90.0f;
	F_Tread = L_GREEN;
	B_Tread = LL_GREEN;
}



  
  
  


//-----------------------------------------------------------------------
//  Global variables
//-----------------------------------------------------------------------
static Tank myTank(0, 0, 0);
int mouseX = 0;
int mouseY = 0;

//-----------------------------------------------------------------------
//  Class Initializations
//-----------------------------------------------------------------------
 void myInit(float cx, float cy, float l, float w){
 	myHose = Hose (cx, cy, l, w);
	myTank = Tank (cx, cy, .15f);
 }

void reset(float cx, float cy, float l, float w){
    list<Droplet>::iterator d;
    list<Fireball>::iterator f;
    for (d = dropList.begin(); d!=dropList.end();){
      d = dropList.erase(d);
    }
    for (f = fireList.begin(); f!=fireList.end();){
      f = fireList.erase(f);
    }
    myHose = Hose (cx, cy, l, w);
    myTank = Tank (cx, cy, .15f);
    gameover = false;
}

//Helper functions
int doesIntersect(Droplet d, Fireball f){
	float doX = d.getOX();
	float doY = d.getOY();
	float dr = d.getR();
	float foX = f.getOX();
	float foY = f.getOY();
	float fr = f.getR();
	float oDistance = sqrt((doY-foY)*(doY-foY) + (doX-foX)*(doX-foX));
	float rDistance = dr + fr;
	if (oDistance <= rDistance){
		return 1;
	}
	else{		
		return 0;
	}
}

//-----------------------------------------------------------------------
//  Callbacks
//      The global variable "isReversed" describes the drawing state.
//      When false, a blue rectangle is drawn on top of red diamond.
//      When true the colors are reversed.  The "isReversed" variable is
//      complemented whenever the left mouse button is clicked or the
//      timer goes off (every 10 seconds).
//-----------------------------------------------------------------------

void myReshape(int w, int h) {
    glViewport (0, 0, w, h);                    // update the viewport
    glMatrixMode(GL_PROJECTION);                // update projection
    glLoadIdentity();
    // Change 2nd for change x
    gluOrtho2D(0.0, (double)w/(double)h, 0.0, 1.0);             // map unit square to viewport
    screenWidth = (double)w/(double)h;
    height = h;
    glMatrixMode(GL_MODELVIEW);
    reset(screenWidth/2.0f, .9f, 0.1f, 0.03f);
    glutPostRedisplay();                        // request redisplay
}
                                                // draw diamond and rectangle
void drawObjects(GLfloat* hoseColor, GLfloat* dropColor, GLfloat* fireColor) {
    myTank.draw();
    glColor3fv(hoseColor);                      // set hose color
    myHose.draw();				// draw hose
    glColor3fv(L_GREEN);
    glBegin(GL_POLYGON);
    for (int c = 0; c < 20; c++){
	glVertex2f(myHose.getX()+.04*(cos(c*PI/10)),
		   myHose.getY()+.04*(sin(c*PI/10))); 
      }
    glEnd();
    glColor3fv(dropColor);
    list<Droplet>::iterator d;
    list<Fireball>::iterator f;
    for (d = dropList.begin(); d!=dropList.end(); d++){
      d->draw();
    }
    glColor3fv(fireColor);
    for (f = fireList.begin(); f!=fireList.end(); f++){
      f->draw();
    }
}
list<Fireball>::iterator f;
void myDisplay() {                          // display callback
    //cout << "MyDisplay called" << endl;
    if (gameover){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);               // clear the window
	glColor3fv(GREEN_RGB);                      // set hose color
	myTank.draw();
    	myHose.draw();
	glColor3fv(L_GREEN);
	glBegin(GL_POLYGON);
	for (int c = 0; c < 20; c++){
		glVertex2f(myHose.getX()+.04*(cos(c*PI/10)),
			myHose.getY()+.04*(sin(c*PI/10))); 
	}
	glEnd();
	glutPostRedisplay();
	glutSwapBuffers();                          // swap buffers
    }
    else {
	glClearColor(0.5, 0.5, 0.5, 1.0);           // background is gray
	glClear(GL_COLOR_BUFFER_BIT);               // clear the window
	drawObjects(GREEN_RGB, BLUE_RGB, RED_RGB);
	glutSwapBuffers();                          // swap buffers
   }
}

void myTimer(int id) {                          // timer callback
    double r = double(rand())/double(RAND_MAX);
    //cout << "Timer just went off." << endl;
    list<Droplet>::iterator d;
    if (r < .006){  //Create fireball;
	//float start = (double)rand()%width;
	double range = (screenWidth-.1) - 0.1;
	float rLoc = float(fmod(rand(),range)) + .1;
	Fireball f (rLoc, 0.0f, .1f, .0028f);
	fireList.push_back(f);
    }
    if (fire == 1){
      myHose.shootDroplet();

    }
    for (d = dropList.begin(); d!=dropList.end();){
      //cout<<"Draw droplet"<<endl;
      d->move();
      if (d->outOfBounds() == 1){
	d= dropList.erase(d);
      }
      else {
	++d;
      }	
      //d->draw();  
    }
    list<Fireball>::iterator f;
    for (f = fireList.begin(); f!=fireList.end();){
	f->move();
	if(f->outOfBounds() == 1){
		f = fireList.erase(f);
		gameover = true;
		
	}
	else{
		++f;
	}
    }
    //Check for intersections after each move
    for (d = dropList.begin(); d!=dropList.end();){
	bool dropStillHere = true;
	for (f = fireList.begin(); f!=fireList.end();){
		if(doesIntersect(*d, *f) == 1){
			bool dead = f->hit();
			dropStillHere = false;
			if (dead){
				f = fireList.erase(f);
			}
			else {
				++f;
			}
		}
		else{
			++f;
		}
	}
	if (dropStillHere){
		++d;
	}
	else{
		d = dropList.erase(d);
	}

    }
    glutPostRedisplay();                        // request redraw
    glutTimerFunc(TIMER_DELAY, myTimer, 0);     // reset timer for 10 seconds
}

void myMouse(int b, int s, int x, int y) {      // mouse click callback
        if (b == GLUT_LEFT_BUTTON) {
	  if (s == GLUT_DOWN){
            fire = 1;
	  }
	  if (s == GLUT_UP){
	    fire = 0;
	    myHose.reload();
	    //exit(0);
	  }
        }
}

void myMotion(int x, int y){
  //if (x != mouseX || y != mouseY){
    mouseX = x;
    mouseY = y;
    myHose.setAngle(mouseX, mouseY);
    glutPostRedisplay();
    //}
}
                                                // keyboard callback
void myKeyboard(unsigned char c, int x, int y) {
    switch (c) {                                // c is the key that is hit
        case 'q':                               // 'q' means quit
            exit(0);
            break;
	case 'd':
	    myHose.shootDroplet();
	    break;
	case 'r':
	    reset(screenWidth/2.0f, .9f, 0.1f, 0.03f);	
	default:
            break;
    }
}

void myKeyboardSpecial(int k, int x, int y) {
	if (!gameover){
		switch (k) {                                // k is special key hit
			case GLUT_KEY_LEFT:
			myTank.setAngle(180.0f);
			myTank.moveX(-0.01f);
			glutPostRedisplay();
			break;
			case GLUT_KEY_RIGHT:
			myTank.setAngle(0.0f);
			myTank.moveX(0.01f);
			glutPostRedisplay();
			break;
			case GLUT_KEY_DOWN:
			myTank.setAngle(-90.f);
			myTank.moveY(-0.01f);
			glutPostRedisplay();
			break;
			case GLUT_KEY_UP:
			myTank.setAngle(90.f);
			myTank.moveY(0.01f);
			glutPostRedisplay();
			break;
				
			default:
			break;
		}
	}
}


//-----------------------------------------------------------------------
//  Main program.
//      This does all the set up for the program.  It creates the game
//      and then passes control to glut.
//-----------------------------------------------------------------------

int main(int argc, char** argv)
{
    cout << "\n\
-----------------------------------------------------------------------\n\
  CMSC 427 Sample Program.\n\
  - Click left mouse button to swap colors.\n\
  - Try resizing and covering/uncovering the window.\n\
  - Hit q to quit.\n\
-----------------------------------------------------------------------\n";

    glutInit(&argc, argv);                      // OpenGL initializations
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);// double buffering and RGB
    glutInitWindowSize(600, 600);               // create a 400x400 window
    glutInitWindowPosition(0, 0);               // ...in the upper left
    glutCreateWindow(argv[0]);                  // create the window

    //Hose myHose(200.0f, 400.0f, 10.0f, 2.0f);					//sttup classes
    myInit(screenWidth/2.0f, .9f, 0.1f, 0.03f);		//x,y, length, width
    glutDisplayFunc(myDisplay);                 // setup callbacks
    glutReshapeFunc(myReshape);
    glutMouseFunc(myMouse);
    glutPassiveMotionFunc(myMotion);
    glutMotionFunc(myMotion);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(myKeyboardSpecial);
    glutTimerFunc(TIMER_DELAY, myTimer, 0);
    glutMainLoop();                             // start it running
    return 0;                                   // ANSI C expects this
}
