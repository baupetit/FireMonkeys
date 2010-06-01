#include "Viewer.h"

#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif


Viewer *Viewer::_instance = NULL;

Viewer::Viewer(const string & title, int width, int height,
	           int initPositionX, int initPositionY){
	_initGlut(title, width, height, initPositionX, initPositionY);
	_initDepth();
	_initRendering();
	_initLighting();
	_initCamera();
	_reshapeFunc(width, height);
	_setCamera();
	
	Viewer::_instance = this;
	
	init();
}


Viewer::~Viewer(){}
	
	
void Viewer::_setCamera(){
    float x = _distToOrigin*cos(_cameraAngle);
	float y = _cameraHeight;
	float z = _distToOrigin*sin(_cameraAngle);

	glLoadIdentity();
	gluLookAt(x, y, z, -x, y, -z, 0.0f, 1.0f, 0.0f);

	glutPostRedisplay();
}	

void Viewer::_initGlut(const string & title, int width, int height, 
	               int initPositionX, int initPositionY){
	               
	cout << "Initialisation de Glut " << endl;
	
	// Initialisation de glut
	const char * dummy_argv[] = { "run" } ;
	int dummy_argc = 1;
	glutInit(&dummy_argc, (char **)dummy_argv);
	
	// Initialisation de GLUT
	glutInitDisplayMode(GLUT_RGBA);// | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(initPositionX, initPositionY);
		
	// Creation de la fenetre
    int windowID = glutCreateWindow(title.c_str());
    
    // Initialisation des callbacks de glut
    glutDisplayFunc(_displayFunc);
	glutKeyboardFunc(_keyboardFunc);
	glutSpecialFunc(_specialFunc);
	glutMouseFunc(_mouseFunc);
	glutMotionFunc(_motionFunc);
	glutPassiveMotionFunc(_passiveMotionFunc);
	glutReshapeFunc(_reshapeFunc);
	glutVisibilityFunc(_visibilityFunc);
	glutIdleFunc(_displayFunc);
	
	// Affichage de la version et des extensions
	cout << "GL Version: " << glGetString(GL_VERSION) << endl;
	//cout << glGetString(GL_EXTENSIONS) << endl;
	
	// Initialisation de Glew
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cerr << "Error glew init: " << glewGetErrorString(err) << endl;
	}
	cout << "Glew initialized " << endl;
}

void Viewer::start(){
	cout << "Lancement de la boucle principale " << endl;
	glutMainLoop();
}	

void Viewer::_initCamera(){
	cout << "Initialisation de la camera " << endl;
	_cameraAngle = M_PI*0.75f;
	_distToOrigin = 5.0f;
	_cameraHeight = 0.5f;
}

void Viewer::_initDepth(){
	cout << "Initialisation de la profondeur " << endl;
	glClearDepth(1.0);  
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

void Viewer::_initRendering(){
	cout << "Initialisation du rendu " << endl;
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
	glLineWidth(1.0f);
	glPointSize(2.0f);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void Viewer::_initLighting(){
	cout << "Initialisation de la lumière " << endl;
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat lightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightDiffuse[]= { 0.9f, 0.9f, 1.0f, 1.0f };
	GLfloat lightPosition[]= {1.0,10.0,1.0,1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION,lightPosition);
	glEnable(GL_LIGHT0);
}
	
void Viewer::_displayFunc(){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	_instance->rendu();
	//glFlush();
	//glutSwapBuffers();
}

void Viewer::_keyboardFunc(unsigned char key, int x, int y){}
void Viewer::_motionFunc(int x, int y){}
void Viewer::_mouseFunc(int button, int state, int x, int y){}
void Viewer::_passiveMotionFunc(int x, int y){}

void Viewer::_reshapeFunc(int w, int h){
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume (perspective camera)
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void Viewer::_specialFunc(int key, int x, int y){
    _instance->_keyMap(key, x, y);
}

void Viewer::_keyMap(int key, int x, int y){
    float cam_speed = 5.0;
	switch (key) {
		case GLUT_KEY_LEFT :
			_cameraAngle += 0.1f * cam_speed;
			_setCamera();
			break;
		case GLUT_KEY_RIGHT :
			_cameraAngle -=0.1f * cam_speed;
			_setCamera();
			break;
		case GLUT_KEY_UP :
			_distToOrigin -= 0.2f * cam_speed;
			if (_distToOrigin<1.5f)
				_distToOrigin = 1.5f;
			_setCamera();
			break;
		case GLUT_KEY_DOWN :
			_distToOrigin += 0.2f * cam_speed;
			_setCamera();
			break;
		case GLUT_KEY_PAGE_UP :
			_cameraHeight += 0.1f * cam_speed;
			_setCamera();
			break;
		case GLUT_KEY_PAGE_DOWN :
			_cameraHeight -= 0.1f * cam_speed;
			_setCamera();
			break;
	    default:
	        break;
	}
}

void Viewer::_visibilityFunc(int visible){}

void Viewer::rendu(){
    
    glDisable(GL_DEPTH_TEST);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glViewport(0,0,128,128);
    glClearColor(1.0,0.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);//|GL_DEPTH_BUFFER_BIT);
    
    
    glPushMatrix();
    glMatrixMode (GL_MODELVIEW);
    glColor4f(1.0,0.0,0.0,1.0);
    glBegin(GL_QUADS);        
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1.0,0.0);
        glVertex3f(1.0,1.0,0.0);
        glVertex3f(1.0,0.0,0.0);
    glEnd();
    glPopMatrix();
    
    glGenerateMipmap(GL_TEXTURE_3D);
    
    
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    
    glEnable(GL_DEPTH_TEST);
    
    
    glViewport(0,0,800,600);
    glClearColor(0.0,0.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glColor4f(1.0,1.0,0.0,0.5);
    
    
    
                glEnableClientState(GL_TEXTURE_3D);
                
                
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);

                glEnable(GL_TEXTURE_3D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                
    glBindTexture(GL_TEXTURE_3D,texture);
    
    glPushMatrix();
    glMatrixMode (GL_MODELVIEW);
    
    //glTranslatef(1.0, 1.0,1.0);
    //glScalef(0.2, 0.2,0.2);
    glBegin(GL_QUADS);
    
        glTexCoord3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        
        glTexCoord3f(0.0,1.0,0.0);
        glVertex3f(0.0,1.0,0.0);
        
        glTexCoord3f(1.0,1.0,0.0);
        glVertex3f(1.0,1.0,0.0);
        
        glTexCoord3f(1.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);
        
        
        
        
        
        
        
        
        
        
        
    
        glTexCoord3f(0.0,0.0, 0.1);
        glVertex3f(0.0,0.0, - 0.9);
        
        glTexCoord3f(0.0,1.0, 0.1);
        glVertex3f(0.0,1.0, - 0.9);
        
        glTexCoord3f(1.0,1.0, 0.1);
        glVertex3f(1.0,1.0, - 0.9);
        
        glTexCoord3f(1.0,0.0, 0.1);
        glVertex3f(1.0,0.0, - 0.9);
      
        
        
        
        
        
        
        
        /*
        glTexCoord3f(0.0,0.0,0.0);
        glVertex3f(32.0,32.0,0.0);
        
        glTexCoord3f(0.0,1.0,0.0);
        glVertex3f(32.0,0.0,-2.0);
        
        glTexCoord3f(1.0,1.0,0.0);
        glVertex3f(64.0,64.0,-2.0);
        
        glTexCoord3f(1.0,0.0,0.0);
        glVertex3f(64.0,32.0,0.0);
        */
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
    
    
            
}

void Viewer::init(){



    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_3D,texture);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,128,128,2,
                 0, GL_RGBA, GL_FLOAT, NULL);
                 
/*                 
    glGenRenderbuffers(1,&renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24, 128,128);
                 
                 
*/                 
    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,  GL_TEXTURE_3D, texture, 0, 0);
    
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,  GL_TEXTURE_3D, texture, 0, 1);
    
    /*
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,  GL_TEXTURE_3D, texture, 0, 2);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,  GL_TEXTURE_3D, texture, 0, 3);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,  GL_TEXTURE_3D, texture, 0, 4);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,  GL_TEXTURE_3D, texture, 0, 5);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6,  GL_TEXTURE_3D, texture, 0, 6);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT7,  GL_TEXTURE_3D, texture, 0, 7);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT8,  GL_TEXTURE_3D, texture, 0, 8);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT9,  GL_TEXTURE_3D, texture, 0, 9);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT10, GL_TEXTURE_3D, texture, 0, 10);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT11, GL_TEXTURE_3D, texture, 0, 11);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT12, GL_TEXTURE_3D, texture, 0, 12);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT13, GL_TEXTURE_3D, texture, 0, 13);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT14, GL_TEXTURE_3D, texture, 0, 14);
    glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT15, GL_TEXTURE_3D, texture, 0, 15);
    */
    
//    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    
//    glEnable(GL_DEPTH_TEST);
    
}













