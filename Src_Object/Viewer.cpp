#include "Viewer.h"
#include "SolverParam.h"

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Viewer *Viewer::_instance = NULL;
float Viewer::sourisX0 = 0;
float Viewer::sourisY0 = 0;
float Viewer::sourisDX = 0;
float Viewer::sourisDY = 0;

Viewer::Viewer(const string & title, int width, int height,
	       int initPositionX, int initPositionY){
	_initGlut(title, width, height, initPositionX, initPositionY);
	_initDepth();
	_initRendering();
	_initLighting();
	_initCamera();
	_reshapeFunc(width, height);
	_setCamera();
	initScene();
	
	Viewer::_instance = this;
	dragging = false;
    normDist = 110.0f;
    
    selectBois  = false;
    selectCire  = false;
    selectMetal = false;    
    
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
	_cameraAngle = M_PI*0.55f;
	_distToOrigin = 20.0f;
	_cameraHeight = 0.0f;
}

void Viewer::_initDepth(){
	cout << "Initialisation de la profondeur " << endl;
	glClearDepth(1.0);  
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

void Viewer::_initRendering(){
	cout << "Initialisation du rendu " << endl;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

void Viewer::_keyboardFunc(unsigned char key, int x, int y){
	switch (key) {
	case 'a' :
		cout << "diminution de la diffusion pour la feu " << SolverParam::getDiffusionParamFire() << endl;
		if (SolverParam::getDiffusionParamFire() - 0.0001 >= 0)
		SolverParam::setDiffusionParamFire( SolverParam::getDiffusionParamFire() - 0.0001  ) ;
		break;
	case 'z' :
		cout << "diminution de la diffusion pour la fumée " << SolverParam::getDiffusionParamSmoke()  << endl;
		if (SolverParam::getDiffusionParamSmoke() - 0.000001 >= 0)
		SolverParam::setDiffusionParamSmoke( SolverParam::getDiffusionParamSmoke() - 0.000001  ) ;
		break;
	case 'e' :
		cout << "diminution de la diffusion pour la température " << SolverParam::getDiffusionParamTemperature()  << endl;
		if (SolverParam::getDiffusionParamTemperature() - 0.0001 >= 0)
		SolverParam::setDiffusionParamTemperature( SolverParam::getDiffusionParamTemperature() - 0.0001  ) ;
		break;
	case 'q' :
		cout << "augmentation de la diffusion pour le feu " << SolverParam::getDiffusionParamFire()  << endl;
		SolverParam::setDiffusionParamFire( SolverParam::getDiffusionParamFire() + 0.0001  ) ;
		break;
	case 's' :
		cout << "augmentation de la diffusion pour la fumée " << SolverParam::getDiffusionParamSmoke()  << endl;
		SolverParam::setDiffusionParamSmoke( SolverParam::getDiffusionParamSmoke() + 0.000001  ) ;
		break;
	case 'd' :
		cout << "augmentation de la diffusion de la température " << SolverParam::getDiffusionParamTemperature()  << endl;
		SolverParam::setDiffusionParamTemperature( SolverParam::getDiffusionParamTemperature() + 0.0001  ) ;
		break;
	case 'r' :
		cout << "diminution du cool " << SolverParam::getCoolingParam()  << endl;
		if (SolverParam::getCoolingParam() - 0.01 >= 0)
		SolverParam::setCoolingParam( SolverParam::getCoolingParam() - 0.01  ) ;
		break;
	case 'f' :
		cout << "augmentation du cool " << SolverParam::getCoolingParam()  << endl;
		SolverParam::setCoolingParam( SolverParam::getCoolingParam() + 0.01  ) ;
		break;
	case 't' :
		cout << "diminution de la consommation " << SolverParam::getConsumingParam()  << endl;
		if (SolverParam::getConsumingParam() - 0.01 >= 0)
		SolverParam::setConsumingParam( SolverParam::getConsumingParam() - 0.01  ) ;
		break;
	case 'g' :
		cout << "augmentation de la consommation " << SolverParam::getConsumingParam()  << endl;
		SolverParam::setConsumingParam( SolverParam::getConsumingParam() + 0.01  ) ;
		break;
	case 'y' :
		cout << "diminution de la conversion " << SolverParam::getFireToSmokeParam()  << endl;
		if (SolverParam::getFireToSmokeParam() - 0.01 >= 0)
			SolverParam::setFireToSmokeParam( SolverParam::getFireToSmokeParam() - 0.01  ) ;
		break;
	case 'h' :
		cout << "augmentation de la conversion " << SolverParam::getFireToSmokeParam()  << endl;
		SolverParam::setFireToSmokeParam( SolverParam::getFireToSmokeParam() + 0.01  ) ;
		break;
	case 'u' :
		cout << "diminution de la viscosité " << SolverParam::getViscosityParam()  << endl;
		if (SolverParam::getViscosityParam() - 0.0001 >= 0)
		SolverParam::setViscosityParam( SolverParam::getViscosityParam() - 0.0001  ) ;
		break;
	case 'j' :
		cout << "augmentation de la viscosité " << SolverParam::getViscosityParam()  << endl;
		SolverParam::setViscosityParam( SolverParam::getViscosityParam() + 0.0001  ) ;
		break;
	case 'i' :
		cout << "diminution de la buyoancy " << SolverParam::getBuoyancyParam() << endl;
		if (SolverParam::getBuoyancyParam() - 0.01 >= 0)
		SolverParam::setBuoyancyParam( SolverParam::getBuoyancyParam() - 0.01  ) ;
		break;
	case 'k' :
		cout << "augmentation de la buyoancy " << SolverParam::getBuoyancyParam() << endl;
		SolverParam::setBuoyancyParam( SolverParam::getBuoyancyParam() + 0.01  ) ;
		break;	
	case 'o' :
		cout << "diminution de la vortexité " << SolverParam::getVorticityConfinementParam() << endl;
		if (SolverParam::getVorticityConfinementParam() - 0.01 >= 0)
		SolverParam::setVorticityConfinementParam( SolverParam::getVorticityConfinementParam() - 0.01  ) ;
		break;
	case 'l' :
		cout << "augmentation de la vortexité " << SolverParam::getVorticityConfinementParam() << endl;
		SolverParam::setVorticityConfinementParam( SolverParam::getVorticityConfinementParam() + 0.01  ) ;
		break;


	default:
	        break;
	}
}
void Viewer::_motionFunc(int x, int y){
    _instance->mouse(x,y);
}

void Viewer::mouse(int x, int y){
	if (dragging) {
		sourisDX = (x-sourisX0)*_distToOrigin/normDist;
		sourisDY = (y-sourisY0)*_distToOrigin/normDist;
		sourisX0 = x;
		sourisY0 = y;
	}
}

void Viewer::mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		dragging = true;
		sourisX0 = x;
		sourisY0 = y;
	} else if (button == GLUT_LEFT_BUTTON && state==GLUT_UP) {
		dragging = false;	
		sourisDX = 0;
		sourisDY = 0;			
	}
}

void Viewer::_mouseFunc(int button, int state, int x, int y){
    _instance->mouse(button, state, x, y);
}

void Viewer::_passiveMotionFunc(int x, int y){
    _instance->mouse(x,y);

}

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
	float cam_speed = 1.0;
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
	case GLUT_KEY_F1 :
		selectMetal=true;
		selectBois=false;
		selectCire=false;
		break;
	case GLUT_KEY_F2 :
		selectMetal=false;
		selectBois=true;
		selectCire=false;
		break;
	case GLUT_KEY_F3 :
		selectMetal=false;
		selectBois=false;
		selectCire=true;
		break;

	default:
	        break;
	}
}

void Viewer::_visibilityFunc(int visible){}

void Viewer::initScene(){
	/*cout << "Initialisation de la Scene " << endl;
	  _fluid = new Fluid_GPU();
	  _fluid -> initialiserSpeedField();*/
}
void Viewer::rendu(){/*
		     //if ( _fluid!= 0 ) _fluid -> resolutionSpeedField();
		     //if ( _fluid!= 0 ) _fluid -> displaySpeedField();
		     glBegin(GL_POINTS);
		     glVertex3d(0.0f,0.0f,0.0f);
		     glEnd();
		     */
}
