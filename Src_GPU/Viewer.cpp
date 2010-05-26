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
}


Viewer::~Viewer(){}
	
	
void Viewer::_setCamera(){
    glLoadIdentity();
	gluLookAt(_posx, _posy, _posz, 0.0f, 2.0f, -40.0f, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}	

void Viewer::_initGlut(const string & title, int width, int height, 
	               int initPositionX, int initPositionY){
	               
	cout << "Initialisation de Glut " << endl;
	
	// Initialisation de glut
	const char * dummy_argv[] = { "run" } ;
	int dummy_argc = 1;
	glutInit(&dummy_argc, (char **)dummy_argv);
	
	// Initialisation de GL
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
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
	
	
	// Affichage de la version et des extensions
	cout << "GL Version: " << glGetString(GL_VERSION) << endl;
	//cout << glGetString(GL_EXTENSIONS) << endl;
	
	// Initialisation de Glew
    #ifdef USE_GLEW	
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cerr << "Error glew init: " << glewGetErrorString(err) << endl;
	}
	cout << "Glew initialized " << endl;
    #endif
}

void Viewer::start(){
	cout << "Lancement de la boucle principale " << endl;
	glutMainLoop();
}	

void Viewer::_initCamera(){
	cout << "Initialisation de la camera " << endl;
	_cameraAngle = M_PI*0.75f;
	_posx = 0;
	_posy = 2;
	_posz = 10;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    _instance->rendu();
	glFlush();
	glutSwapBuffers();
}

void Viewer::_keyboardFunc(unsigned char key, int x, int y){}
void Viewer::_motionFunc(int x, int y){}
void Viewer::_mouseFunc(int button, int state, int x, int y){}
void Viewer::_passiveMotionFunc(int x, int y){}
void Viewer::_reshapeFunc(int w, int h){}
void Viewer::_specialFunc(int key, int x, int y){}
void Viewer::_visibilityFunc(int visible){}



void Viewer::rendu(){}
