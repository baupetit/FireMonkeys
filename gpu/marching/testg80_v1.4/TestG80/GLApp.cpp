#include "GLApp.h"
#include <iostream>

namespace NemoGraphics {

GLApp *GLApp::currentApp=NULL;

GLApp::GLApp(const char *title, int w, int h, int flags ){
	int argc=1;
	char *argv[]={"GLApp"};

	glutInit(&argc, argv);

	setVideoMode(w, h, flags);
	setTitle(title);

	dispFPS=false;
	curFPS=0.0f;
	lastFPSTime=0;
	fpsCounter=0;

	fpsCountInterval=200;

	getTimerReset=false;
	dispPrint=true;
}

GLApp::GLApp(int *argcp, char **argv, const char *title, int w, int h, int flags ){
	glutInit(argcp, argv);

	setVideoMode(w, h, flags);
	setTitle(title);

	dispFPS=false;
	curFPS=0.0f;
	lastFPSTime=0;
	fpsCounter=0;

	fpsCountInterval=5;

	getTimerReset=false;
	dispPrint=true;
}

GLApp::~GLApp(){}


void GLApp::idleFunc(){
	currentApp->idle();
}
void GLApp::displayFunc(){
	currentApp->display();
	
	currentApp->postDisplay();
}

void GLApp::postDisplay(){

	if(fpsCounter>fpsCountInterval){
		int time=glutGet(GLUT_ELAPSED_TIME);
		this->curFPS=(float)fpsCountInterval*1000.0f/(float)(time-lastFPSTime);
		lastFPSTime=time;
		fpsCounter=0;
	}
	fpsCounter++;

	if(this->dispFPS){
		sprintf_s(textBuffer, "FPS: %4.2f", curFPS);

		bool tmp=dispPrint;
		dispPrint=true;
		this->print(textBuffer, Vector2i(0, 20), Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
		dispPrint=tmp;
	}

	this->frameDuration=glutGet(GLUT_ELAPSED_TIME)-this->lastFrameTime;
	this->lastFrameTime=glutGet(GLUT_ELAPSED_TIME);

	if(this->stayTextTime!=0 ){
		//std::cout<<this->stayText<<"\n";
		bool tmp=dispPrint;
		dispPrint=true;
		this->print(this->stayText.c_str(), this->stayTextPos, this->stayTextColor);
		dispPrint=tmp;

		if(this->stayTextTime>0){
			this->stayTextTime=this->stayTextTime-this->frameDuration;
			if(this->stayTextTime<0)
				this->stayTextTime=0;
		}
	}

	currentApp->swapBuffers();
}
void GLApp::reshapeFunc(int w, int h){
	currentApp->windowSize.x=w;
	currentApp->windowSize.y=h;
	currentApp->reshape(w, h);
}

void GLApp::keyboardFunc(unsigned char key, int x, int y){
	currentApp->keyboard(key);
}

void GLApp::mouseFunc(int button, int state, int x, int y){
	currentApp->mouse(button, state, x, y);
}
void GLApp::mouseMotionFunc(int x, int y){
	currentApp->mouseMove(x, y);
}

void GLApp::mousePassiveMotionFunc(int x, int y){
	currentApp->mousePassiveMove(x, y);
}
///Protected
void GLApp::swapBuffers(){
	glutSwapBuffers();
}

void GLApp::redisplay(){
	glutPostRedisplay();
}


void GLApp::setOrthoProj() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, windowSize.x , 0,  windowSize.y);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, - windowSize.y, 0);
	glMatrixMode(GL_MODELVIEW);
}

void GLApp::resetPerspProj() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void GLApp::print(const char *s, Vector2i pos, Vector4f color) {
	if(!this->dispPrint)
		return;

	glViewport(0, 0, this->windowSize.x, this->windowSize.y);

	setOrthoProj() ;


	glPushAttrib(GL_TEXTURE_BIT);
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_RECTANGLE_EXT);
	glDisable(GL_TEXTURE_3D);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	//Attention, doit etre avant rasterpos !
	/*The explanation for this is found in the man page for glRasterPos-- 
	raster operations maintain their own state for current color and current 
	texture coordinate (e.g. GL_CURRENT_RASTER_COLOR) In this case, with lighting turned off, 
	the state is copied from GL_CURRENT_COLOR *when you call glRasterPos2f*. 
	So glColor4f between that and a raster operation is ignored.
	(http://www.idevgames.com/forum/printthread.php?t=6325)*/

	glColor4f(color.x, color.y, color.z, color.w);

	glRasterPos2f(pos.x, pos.y);


	if (s && strlen(s)) {
	 while (*s) {
	   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
	   s++;
	 }
	}


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
	glPopAttrib();
	resetPerspProj();
  
}

void GLApp::print(const char *txt, float val, Vector2i pos, Vector4f color){
	char buff[1024];
	char buff2[1024];

	strcpy(buff, txt);
	strcat(buff, " %f");
	sprintf(buff2, buff, val);
	this->print(buff2, pos, color);
}


void GLApp::dispText(long time, std::string txt, Vector2i pos, Vector4f color){
	this->stayTextTime=time;
	this->stayText=txt;
	this->stayTextPos=pos;
	this->stayTextColor=color;
}


////Public


int GLApp::setVideoMode(int w,int h,int flags ){
	windowSize.x=w;
	windowSize.y=h;
	windowFlags=flags;

	return 0;
}
void GLApp::setTitle(const char *title){
	windowTitle=title;
}

void GLApp::run(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB | GLUT_ALPHA | GLUT_STENCIL);
	
	glutInitWindowSize(windowSize.x, windowSize.y);
	windowID=glutCreateWindow(windowTitle.c_str());

	currentApp=this;

	///Callbacks
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);
	//glutSpecialFunc(special);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutPassiveMotionFunc(mousePassiveMotionFunc);
	glutIdleFunc(idleFunc);


	GLenum err = glewInit();

	this->init();


	this->lastFrameTime=this->lastFPSTime=glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();
}

}
