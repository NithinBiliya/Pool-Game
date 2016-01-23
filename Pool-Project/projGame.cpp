/*	9 ball pool
By : 	Nithin Kumar Biliya
	Dattatraya Mukri	*/

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include<iostream>
#include<fstream>
#include<cmath>

#define PI 3.142857143
#define font2 GLUT_BITMAP_TIMES_ROMAN_24
#define font3 GLUT_BITMAP_HELVETICA_18

using namespace std;

void mouse(int,int,int,int);
void passive(int,int);
void reshape(int,int);
void init(void);
void display(void);
void camera(void);
void initBalls(float,float);
void GetOGLPos(int,int);
void text(char [],GLfloat,GLfloat,void*);
void drawSector(GLfloat,GLfloat,GLfloat);
void keyboard(unsigned char,int,int);
void saveGame();
void loadGame();
bool isCollision(int*,int*,GLdouble*);
void detectAndResolveCollision();

int const meter=12;
float const timeSlice=0.03,acceleration=-0.2688;
int gameState=0,mousePosX,mousePosY,width,height,menuHover=0;
char *menuContent;
GLfloat angleSet=0,creditsY=4;
GLint playerFlag=1,foulFlag=0;
GLdouble modelviewMatrix[16],projectionMatrix[16];
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];
GLfloat winX, winY, winZ;
GLdouble posX, posY, posZ,pX,pY,pZ,cX,cY,cZ;
GLfloat greenDiffuseMaterial[] = {0.0, 1.0, 0.0}; //set the material to red
GLfloat blueDiffuseMaterial[] = {0.0, 0.0, 1.0}; //set the material to blue
GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0}; //set the material to red
GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0}; //set the material to white
//GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0}; //set the material to green
GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the diffuse light to white
//GLfloat blankMaterial[] = {0.0, 0.0, 0.0}; //set the diffuse light to white
GLfloat mShininess[] = {128}; //set the shininess of the material
GLfloat LightPosition[] = {0.25, 0.25, 0.25, 0.0}; //set the LightPosition to the specified values

class Board {
	GLfloat length,width,height,startX,startY,startZ;
	public :
		Board(GLfloat x,GLfloat y) {
			length=2.24;
			width=1.12;
			height=0;
			startX=x-1.12;
			startY=y-0.56;
			startZ=0.0;//4.2/100;
		}
		void drawBoard() {
			glPushMatrix(); //set where to start the current object transformations
			glColor3f(0.0,1.0,0.0);
			glBegin(GL_QUADS); // Start drawing a quad primitive    
			glVertex3f(startX*meter, startY*meter, startZ*meter); // The bottom left corner  
			glVertex3f(startX*meter, (startY+width)*meter, startZ*meter); // The top left corner  
			glVertex3f((startX+length)*meter, (startY+width)*meter, startZ*meter); // The top right corner  
			glVertex3f((startX+length)*meter, startY*meter, startZ*meter); // The bottom right corner
			glEnd();
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_LINES); // Start drawing a quad primitive    
			glVertex3f((startX+0.56)*meter, (startY)*meter, startZ*meter+0.0001);
			glVertex3f((startX+0.56)*meter, (startY+width)*meter, startZ*meter+0.0001);
			glEnd();
			glPushMatrix();
			glColor3f(0,0,0);
			glTranslatef(startX*meter-0.5,startY*meter-0.6,startZ*meter);
			drawSector(1,0,90);
			glTranslatef(0,width*meter,startZ*meter);
			drawSector(1,90,180);
			glTranslatef(length*meter,0,startZ*meter);
			drawSector(1,180,270);
			glTranslatef(0,-1*width*meter,startZ*meter);
			drawSector(1,270,360);
			glTranslatef(-1*length/2.0*meter,0,startZ*meter);
			drawSector(1,270,450);
			glTranslatef(0,width*meter,startZ*meter);
			drawSector(1,90,270);
			glPopMatrix();
			glColor3f(0.5,0.2,0.1);
			glPushMatrix();
			glTranslatef(startX*meter-0.5,0,0);
			glScalef(1,15,1);
			glutSolidCube(1);
			glTranslatef(length*meter+1,0,0);
			glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(0,startY*meter-0.5,0);
			glScalef(28.9,1,1);
			glutSolidCube(1);
			glTranslatef(0,width*meter+1,0);
			glutSolidCube(1);
			glPopMatrix();
			glPopMatrix(); //end the current object transformations
			glPushMatrix();
			glColor3f(1,1,1);
			menuContent="1 : ";
			text(menuContent,-20,5.8,font2);
			glColor3f(1,0,0);
			glTranslatef(-18,6,0);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="2 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(0,1,0);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="3 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(0,0,1);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="4 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(1,1,0);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="5 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(1,0,1);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="6 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(0,1,1);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="7 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(0.5,0.5,0);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="8 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(0,0.5,0.5);
			glutSolidSphere(0.042*meter,25,25);
			glColor3f(1,1,1);
			menuContent="9 : ";
			glTranslatef(0,-1,0);
			text(menuContent,-2,-0.2,font2);
			glColor3f(0,0,0);
			glutSolidSphere(0.042*meter,25,25);
			glPopMatrix();
			glPushMatrix();
			if(playerFlag>0) {
				glColor3f(1,0,0);
				menuContent="Player 1 to play ";
			}
			else {
				glColor3f(0,0,1);
				menuContent="Player 2 to play ";
			}
			text(menuContent,-3,9,font2);			
			glPopMatrix();
		}
}board(0,0);

class BallHolder {
	GLfloat size,scaleY1,scaleX2,tranX1,tranY2,tranY,colorDiffuseMaterial[3],xStart;
	GLint top;
	public :
		BallHolder(void) {
			top=-1;
			size=0.5;
			scaleY1=(0.042*2*meter+size*2)/size;
			scaleX2=0.042*2*9*meter/size;
			tranX1=-0.042*9*meter/size/2-size/2;
			tranY2=0.042*meter+size/2;
			tranY=-0.7*meter-1;
			colorDiffuseMaterial[0]=0.5;
			colorDiffuseMaterial[1]=0;
			colorDiffuseMaterial[2]=0;
			xStart=0.042*9*2;
		}
		void incTop() { top++; }
		GLfloat retXStart() { return xStart; }
		GLint retTop() {return top; }
		void draw(void) {
			//	Left edge
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(tranX1,tranY,0);
//			glRotatef(60,0,1,0);
			glScalef(1,scaleY1,1);
			glutSolidCube(size);
			glPopMatrix(); //end the current object transformations

			//	Right edge
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(-tranX1,tranY,0);
//			glRotatef(60,0,1,0);
			glScalef(1,scaleY1,1);
			glutSolidCube(size);
			glPopMatrix(); //end the current object transformations

			//	Top edge
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(0,tranY2+tranY,0);
//			glRotatef(60,0,1,0);
			glScalef(scaleX2,1,1);
			glutSolidCube(size);
			glPopMatrix(); //end the current object transformations

			//	Bottom edge
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(0,-tranY2+tranY,0);
//			glRotatef(60,0,1,0);
			glScalef(scaleX2,1,1);
			glutSolidCube(size);
			glPopMatrix(); //end the current object transformations
		}
}holder;

class Ball {
	GLfloat radius,velX,velY,velZ,colorDiffuseMaterial[3],x,y,z,pixX,pixY,pixZ,totalVel,dir,accX,accY;
	GLint ballNum;
	bool pocket;
	public :
		Ball() {
			
		}
		Ball(GLfloat R,GLfloat G,GLfloat B,GLfloat xx,GLfloat yy) {
			radius=4.2/100;
			colorDiffuseMaterial[0]=R;
			colorDiffuseMaterial[1]=G;
			colorDiffuseMaterial[2]=B;
			x=xx;
			y=yy;
			z=radius;
			velX=0;
			velY=0;
			velZ=0;
			pocket=false;
			ballNum=0;
		}
		void initBall(GLfloat R,GLfloat G,GLfloat B,GLfloat xx,GLfloat yy,GLfloat tv,GLfloat d,GLint num) {
			radius=4.2/100;
			colorDiffuseMaterial[0]=R;
			colorDiffuseMaterial[1]=G;
			colorDiffuseMaterial[2]=B;
			x=xx;
			y=yy;
			z=radius;
			totalVel=tv;
			dir=d*3.142857143/180;
			velX=totalVel*cos(dir);
			velY=totalVel*sin(dir);
			velZ=0;
			accX=acceleration*fabs(cos(dir));
			accY=acceleration*fabs(sin(dir));
			pocket=false;
			ballNum=num;
		}
		void updateOtherValues() {
			dir=atan(velY/velX);
			accX=acceleration*fabs(cos(dir));
			accY=acceleration*fabs(sin(dir));			
		}
		bool insidePocket(GLfloat xc,GLfloat yc) {
			if(sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc))<=0.084)
				return true;
			return false;
		}
		void updateBall() {
			if(!pocket) {
			if(insidePocket(0,0) || insidePocket(0,1.12) || insidePocket(2.24,1.12) || insidePocket(2.24,0) || insidePocket(1.12,0) || insidePocket(1.12,1.12))
				pushBall();
			x+=velX*timeSlice;
			if(fabs(velX)>0.01)
			{
				if(velX>0)
					velX=velX+accX*timeSlice;
				else
					velX=velX-accX*timeSlice;
			}
			else
				velX=0;
			y+=velY*timeSlice;
			if(fabs(velY)>0.01)
			{
				if(velY>0)
					velY=velY+accY*timeSlice;
				else
					velY=velY-accY*timeSlice;
			}
			else
				velY=0;
			if(x>2.24-radius || x<0+radius)	{
				if(!pocket) {
					if(x>2.24-radius)	x=2.24-radius;
					else if(x<0+radius)	x=0+radius;
				}
				velX*=-1;
				if(velX>0)
					velX=velX+accX*timeSlice;
				else
					velX=velX-accX*timeSlice;
			}
			if(y>1.12-radius || y<0+radius)	{
				if(!pocket) {
					if(y>1.12-radius)	y=1.12-radius;
					else if(y<0+radius)	y=0+radius;
				}
				velY*=-1;
				if(velY>0)
					velY=velY+accY*timeSlice;
				else
					velY=velY-accY*timeSlice;
			}
			}
		}
		void drawBall() {
			x-=1.12;
			y-=0.56;
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(x*meter,y*meter,z*meter);
			glutSolidSphere(radius*meter,25,25);
			glPopMatrix(); //end the current object transformations
			x+=1.12;
			y+=0.56;
		}
		GLfloat getX(void) {
			return x;
		}
		GLfloat getY(void) {
			return y;
		}
		void setTotalVelocity(GLfloat tv, GLfloat d) {
			totalVel=tv;
			dir=d*3.142857143/180;
			velX=totalVel*cos(dir);
			velY=totalVel*sin(dir);
			velZ=0;
			accX=acceleration*fabs(cos(dir));
			accY=acceleration*fabs(sin(dir));
		}
		void pushBall() {
			if(ballNum==0) {
				x=0.56; y=0.56;
				velX=velY=accX=accY=totalVel=0;
				foulFlag=1;
				return;
			}
			else if(ballNum==9) {
				gameState=13;
				return;
			}
			pocket=true;
			holder.incTop();
			x=holder.retXStart()+holder.retTop()*0.042*2+0.03;
			y=(-2.5)/meter;
			velX=velY=velZ=totalVel=accX=accY=0;
		}
		friend void detectAndResolveCollision();
		friend bool isCollision(int*,int*,GLdouble*);
}balls[10];

class PowerMeter {
	GLfloat len,tranX,tranY,tranZ,scaleY,scaleInnerY,colorDiffuseMaterial[3],incdec;
	public : 
		PowerMeter() {
			len=0.16666667;
			tranX=1.416666667;
			tranY=0;
			tranZ=0;
			scaleY=0.416666667;
			scaleInnerY=0;
			colorDiffuseMaterial[0]=1;
			colorDiffuseMaterial[1]=0;
			colorDiffuseMaterial[2]=0;
			incdec=1;
		}
		void drawPowerMeter() {
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(tranX*meter,tranY*meter,tranZ*meter);
			glRotatef(60,0,1,0);
			glScalef(1,scaleY*meter,1);			
			glutWireCube(len*meter);
			glPopMatrix(); //end the current object transformations
			glPushMatrix(); //set where to start the current object transformations
			glColor3fv(colorDiffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseMaterial);
			glTranslatef(tranX*meter,tranY*meter,tranZ*meter);
			glRotatef(60,0,1,0);
			glScalef(1-0.1,scaleInnerY-0.1,1-0.1);
			glutSolidCube(len*meter);
			glPopMatrix(); //end the current object transformations
		}
		void updatePowerMeter() {
			if(incdec>0) scaleInnerY+=0.008333333*meter;
			else	scaleInnerY-=0.008333333*meter;
			if(scaleInnerY>0.416666667*meter) {
				incdec*=-1;
				scaleInnerY-=0.016666667*meter;
			}
			else if(scaleInnerY<0) {
				incdec*=-1;
				scaleInnerY+=0.016666667*meter;
			}
		}
		GLfloat retPower() {
			return 2.0*scaleInnerY/(0.416666667*meter);
		}
}powerMeter;

int main (int argc,char **argv) {
	glutInit (&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(1364,689);
	glutInitWindowPosition(0,0);
	glutCreateWindow("POOL");
	init();
	initBalls(1.68,0.56);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
//	glutSpecialFunc(spclKeys);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passive);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void display(void) {
	glClearColor (0.0,0.0,0.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	camera();

	if(gameState==0) {	//	Intro video
//		cout<<gameState;
	}
	else if(gameState==1) {	//	game menu
		glColor3f(1,1,1);
		menuContent="9 BALL POOL";
		text(menuContent,-3.0,8.0,font2);
		glColor3f(1,0,0);
		if(menuHover==1)
			glColor3f(0,0,1);
		menuContent="New Game";
		text(menuContent,0.0,4.0,font2);
		if(menuHover==1)
			glColor3f(1,0,0);
		if(menuHover==2)
			glColor3f(0,0,1);
		menuContent="Load Game";
		text(menuContent,0.0,2.0,font2);
		if(menuHover==2)
			glColor3f(1,0,0);
		if(menuHover==3)
			glColor3f(0,0,1);
		menuContent="Save Game";
		text(menuContent,0.0,0.0,font2);
		if(menuHover==3)
			glColor3f(1,0,0);
		if(menuHover==4)
			glColor3f(0,0,1);
		menuContent="Credits";
		text(menuContent,0.0,-2.0,font2);
		if(menuHover==4)
			glColor3f(1,0,0);
		if(menuHover==5)
			glColor3f(0,0,1);
		menuContent="Quit Game";
		text(menuContent,0.0,-4.0,font2);
		if(menuHover==5)
			glColor3f(1,0,0);
		glutSwapBuffers();
	}
	else if(gameState==2) {	//	set angle
		board.drawBoard();
		balls[0].drawBall();
		balls[1].drawBall();
		balls[2].drawBall();
		balls[3].drawBall();
		balls[4].drawBall();
		balls[5].drawBall();
		balls[6].drawBall();
		balls[7].drawBall();
		balls[8].drawBall();
		balls[9].drawBall();
		powerMeter.drawPowerMeter();
		holder.draw();
		GetOGLPos(mousePosX,mousePosY);
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		glVertex3f((balls[0].getX()-1.12)*meter,(balls[0].getY()-0.56)*meter , 0.01);
		glVertex3f(posX,posY,posZ);
		glEnd();
		glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluProject((balls[0].getX()-1.12)*meter,(balls[0].getY()-0.56)*meter , 0.01, modelviewMatrix, projectionMatrix, viewport, &cX, &cY, &cZ);
		gluProject(posX, posY, posZ, modelviewMatrix, projectionMatrix, viewport, &pX, &pY, &pZ);
		angleSet=atan((float)(pY-cY)/(pX-cX))*180.0/3.14;
		glutSwapBuffers();
	}
	else if(gameState==3) {	//	set power
		board.drawBoard();
		balls[0].drawBall();
		balls[1].drawBall();
		balls[2].drawBall();
		balls[3].drawBall();
		balls[4].drawBall();
		balls[5].drawBall();
		balls[6].drawBall();
		balls[7].drawBall();
		balls[8].drawBall();
		balls[9].drawBall();
		powerMeter.drawPowerMeter();
		holder.draw();
		glutSwapBuffers();
		powerMeter.updatePowerMeter();
	}
	else if(gameState==4) {	//	move balls!
		
		board.drawBoard();
		balls[0].drawBall();
		balls[1].drawBall();
		balls[2].drawBall();
		balls[3].drawBall();
		balls[4].drawBall();
		balls[5].drawBall();
		balls[6].drawBall();
		balls[7].drawBall();
		balls[8].drawBall();
		balls[9].drawBall();
		powerMeter.drawPowerMeter();
		holder.draw();

		glutSwapBuffers();

		balls[0].updateBall();
		balls[1].updateBall();
		balls[2].updateBall();
		balls[3].updateBall();
		balls[4].updateBall();
		balls[5].updateBall();
		balls[6].updateBall();
		balls[7].updateBall();
		balls[8].updateBall();
		balls[9].updateBall();
		detectAndResolveCollision();
	}
	else if(gameState==10) {
		glColor3f(1,1,1);
		menuContent="Game saved!!";
		text(menuContent,-3.0,0.0,font2);
		glutSwapBuffers();
	}
	else if(gameState==12) {	//	Credists
		glColor3f(1,1,1);
		menuContent="Game Developed by : ";
		text(menuContent,-3.0,creditsY,font2);
		glColor3f(1,0,0);
		menuContent="NITHIN KUMAR B";
		text(menuContent,-3.0,creditsY-3,font2);
		glColor3f(0,1,0);
		menuContent="and";
		text(menuContent,-0.5,creditsY-5,font2);
		glColor3f(0,0,1);
		menuContent="DATTATRAYA";
		text(menuContent,-2.5,creditsY-7,font2);
		glutSwapBuffers();
		creditsY+=0.1;
		if(creditsY>20)
			creditsY=-12;
	}
	else if(gameState==13) {	//	Win
		glColor3f(1,1,1);
		if(playerFlag>0)
			menuContent="Player 1 WINS!!!";
		else
			menuContent="Player 2 WINS!!!";
		text(menuContent,-3,0,font2);
		glutSwapBuffers();		
	}
}

void camera(void) {
	glRotatef(0.0,1.0,0.0,0.0);
	glRotatef(0.0,0.0,1.0,0.0);
	glTranslated(0,0,-20);
}

void init(void) {
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);
}

void initBalls(float x,float y) {
	GLdouble theta=30*PI/180.0,R=0.0421;
	balls[0].initBall(1,1,1,0.56,0.56,1.3,30,0);
	balls[1].initBall(1,0,0,x,y,0.0,0.0,1);
	balls[2].initBall(0,1,0,x+4*R*cos(theta),y+2*R,0.0,0.0,2);
	balls[3].initBall(0,0,1,x+4*R*cos(theta),y-2*R,0.0,0.0,3);
	balls[4].initBall(1,1,0,x+8*R*cos(theta),y,0.0,0.0,4);
	balls[5].initBall(1,0,1,x+2*R*cos(theta),y+R,0.0,60.0,5);
	balls[6].initBall(0,1,1,x+2*R*cos(theta),y-R,0.0,0.0,6);
	balls[7].initBall(0.5,0.5,0,x+6*R*cos(theta),y+R,0.0,0.0,7);
	balls[8].initBall(0,0.5,0.5,x+6*R*cos(theta),y-R,0.0,0.0,8);
	balls[9].initBall(0,0,0,x+4*R*cos(theta),y,0.0,0.0,9);
}

void reshape(int w, int h) {
	width=w; height=h;
	glViewport(0,0,(GLsizei)w,(GLsizei)h); 			//set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); 				//set the matrix to projection
	glLoadIdentity();
	gluPerspective(60,(GLfloat)w/(GLfloat)h,1.0,100.0); 	//set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); 				//set the matrix back to model
}

void mouse(int button,int state,int x,int y) {
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && gameState==1) {
		if(x>=650 && x<=840) {
			if(y>=185 && y<246)
				gameState++;
			else if(y>=246 && y<307) {
				loadGame();
				gameState=2;
			}
			else if(y>=307 && y<368) {
				saveGame();
				gameState=10;
			}
			else if(y>=368 && y<429)
				gameState=12;
			else if(y>=429 && y<490)
				exit(0);
		}
	}
	else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && gameState==2) {
		if((balls[0].getX()-1.12)*meter>posX)
			angleSet+=180;
		gameState++;
	}
	else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && gameState==3) {
		balls[0].setTotalVelocity(powerMeter.retPower(),angleSet);
		gameState++;
	}
	else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && gameState==0)
		gameState++;
}

void saveGame() {
	fstream  ftowrite;
	ftowrite.open("pool1.dat", ios::out );
	for(int i=0;i<=9;i++)
		ftowrite.write((char*)&balls[i],sizeof(Ball));
	ftowrite.write((char*)&playerFlag,sizeof(GLint));
	ftowrite.write((char*)&foulFlag,sizeof(GLint));
	ftowrite.close();
}

void loadGame() {
	fstream  ftoread;
	ftoread.open("pool1.dat", ios::in );
	for(int i=0;i<=9;i++)
		ftoread.read((char*)&balls[i],sizeof(Ball));
	ftoread.read((char*)&playerFlag,sizeof(GLint));
	ftoread.read((char*)&foulFlag,sizeof(GLint));
	ftoread.close();
}

void passive(int x,int y) {
	if(gameState==2) {
		mousePosX=x; mousePosY=y;
	}
	else if(gameState==1) {
		if(x>=650 && x<=840) {
			if(y>=185 && y<246)
				menuHover=1;
			else if(y>=246 && y<307)
				menuHover=2;
			else if(y>=307 && y<368)
				menuHover=3;
			else if(y>=368 && y<429)
				menuHover=4;
			else if(y>=429 && y<490)
				menuHover=5;
		}
	}
}

void GetOGLPos(int x, int y)
{ 
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
}

bool isCollision(int *a,int *b,GLdouble *dist) {
	for(*a=0;*a<10;(*a)++)
		for(*b=0;*b<10;(*b)++) {
			*dist=sqrt((balls[*a].x-balls[*b].x)*(balls[*a].x-balls[*b].x)+(balls[*a].y-balls[*b].y)*(balls[*a].y-balls[*b].y));
			if(*a!=*b && *dist < balls[*a].radius+balls[*b].radius)
				return true;
		}
}

void detectAndResolveCollision() {
	int i,j;
	GLdouble dist,tx1,ty1,tvx1,tvy1,tx2,ty2,tvx2,tvy2;
	while(isCollision(&i,&j,&dist)) {
		if(balls[i].pocket || balls[j].pocket) break;
		tx1=balls[i].x; ty1=balls[i].y;
		tx2=balls[j].x; ty2=balls[j].y;
		double mass1=1.0,mass2=1.0;
		double theta = atan2(balls[i].y-balls[j].y, balls[i].x-balls[j].x);
		double c = cos(theta);
		double s = sin(theta);
		double xVel1prime = balls[i].velX*c+balls[i].velY*s;
		double xVel2prime = balls[j].velX*c+balls[j].velY*s;
		double yVel1prime = balls[i].velY*c-balls[i].velX*s;
		double yVel2prime = balls[j].velY*c-balls[j].velX*s;

		double ax=(balls[j].x-balls[i].x)/dist;
		double ay=(balls[j].y-balls[i].y)/dist;
		double vai=balls[i].velX*ax + balls[i].velY*ay,vaj=balls[j].velX*ax + balls[j].velY*ay;
		double dt=(balls[i].radius+balls[j].radius-dist)/(vai-vaj);
		balls[i].x-=balls[i].velX*dt;
		balls[i].y-=balls[i].velY*dt;
		balls[j].x-=balls[j].velX*dt;
		balls[j].y-=balls[j].velY*dt;

		double P = (mass1*xVel1prime+mass2*xVel2prime);
		double V = (xVel1prime-xVel2prime);
		double v2f = (P+mass1*V)/(mass1+mass2);
		double v1f = v2f-xVel1prime+xVel2prime;
		xVel1prime = v1f;
		xVel2prime = v2f;

		balls[i].velX = xVel1prime*c-yVel1prime*s;
		balls[j].velX = xVel2prime*c-yVel2prime*s;
		balls[i].velY = yVel1prime*c+xVel1prime*s;
		balls[j].velY = yVel2prime*c+xVel2prime*s;

		balls[i].x+=balls[i].velX*dt;
		balls[i].y+=balls[i].velY*dt;
		balls[j].x+=balls[j].velX*dt;
		balls[j].y+=balls[j].velY*dt;

		balls[i].updateOtherValues();
		balls[j].updateOtherValues();
		balls[i].updateBall();
		balls[j].updateBall();
	}
	for(i=0;i<=9;i++)
		if(balls[i].velX!=0 || balls[i].velY!=0)
			return;
	playerFlag*=-1;
	gameState=2;
}

void text(char tex[],GLfloat xtext,GLfloat ytext,void *font) {
	char *p;
	p = tex;
	glRasterPos2f(xtext, ytext);
	while(*p) glutBitmapCharacter(font, *p++);
}

void drawSector(GLfloat radius,GLfloat startAngle,GLfloat endAngle) {
	float x1,y1,x2,y2;
	float angle,a;
	x1 = 0.5,y1=0.6;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x1,y1,0.0001);
	for (angle=startAngle;angle<endAngle;angle+=0.2) {
		a=angle*3.14/180;
		x2 = x1+sin(a)*radius;
		y2 = y1+cos(a)*radius;
		glVertex3f(x2,y2,0.0001);
	}
	glEnd();
}

void keyboard(unsigned char key,int a,int b) {
	switch(key) {
		case 27 : 	if(gameState==0 || gameState==1) exit(0);
				else if(gameState==10 || gameState==12) gameState=1;
				else gameState=1;
	}
}
