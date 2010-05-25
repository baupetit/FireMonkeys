/****Classe GLApp, Framework Nemo, Copyright Cyril Crassin 2005****/

#ifndef GLApp_h
#define GLApp_h

#include <iostream>
#include <string>
#include "UtilsGL.h"


namespace NemoGraphics {

/** 
  * Base de toute application OpenGL. 
  * Permet de gérer toutes les interactions avec le systeme de fenetrage.
  */
class GLApp {
	static GLApp *currentApp;

	static void idleFunc();
	static void displayFunc();
	static void reshapeFunc(int w, int h);
	static void keyboardFunc(unsigned char key, int x, int y);
	static void mouseFunc(int button, int state, int x, int y);
	static void mouseMotionFunc(int x, int y);
	static void mousePassiveMotionFunc(int x, int y);



	std::string windowTitle;
	int windowFlags;
	int windowID;


	bool dispFPS;
	
	int lastFPSTime;
	int fpsCounter;
	int fpsCountInterval;

	long timerTime;


	std::string stayText;
	Vector2i stayTextPos;
	Vector4f stayTextColor;
	long stayTextTime;

	long lastFrameTime;
	long frameDuration;
protected:
	bool dispPrint;

	bool getTimerReset;
	float curFPS;
	Vector2i windowSize;

	char textBuffer[500];

	////Timers
	void startTimer(){
		timerTime=glutGet(GLUT_ELAPSED_TIME);
	}

	long getTimer(){
		glFinish();

		long res=glutGet(GLUT_ELAPSED_TIME)-timerTime;

		if(getTimerReset)
			timerTime=glutGet(GLUT_ELAPSED_TIME);

		return res;
	}

	/** 
	  * Methode d'initialisation de l'application.
	  * A reimplementer par les applications clientes
	  */
	virtual void init(){}
	
	/** Methodes de callback. */

	virtual void idle() { }
	virtual void display() { }
	virtual void postDisplay();

	virtual void reshape(int w, int h){}
	virtual void keyboard(unsigned char key){}
	virtual void mouse(int button, int state, int x, int y){}
	virtual void mouseMove(int x, int y){}
	virtual void mousePassiveMove(int x, int y){}
	//////


	/** Inversion des front et back buffers. */
	virtual void swapBuffers();
	/** Force le dessin de la fenetre. */
	virtual void redisplay();

	/** Affichage de texte dans la fenetre graphique. */
	virtual void print(const char *txt, Vector2i pos, Vector4f color=Vector4f(1.0, 1.0, 1.0, 1.0));
	virtual void print(const char *txt, float val, Vector2i pos, Vector4f color=Vector4f(1.0, 1.0, 1.0, 1.0));


	virtual void dispText(long time, std::string txt, Vector2i pos, Vector4f color=Vector4f(1.0, 1.0, 1.0, 1.0));

	/** Passage en mode ortho */
	virtual void setOrthoProj();
	virtual void resetPerspProj();
public:
	/** Constructeur d'une application.
	  * \param title Nom de l'application.
	  * \param w largeur de la fenetre.
	  * \param h hauteur de la fenetre.
	  * \param flags Parametres optionels.
	  */
	GLApp(const char *title, int w, int h, int flags = 0);


	GLApp(int *argcp, char **argv, const char *title, int w, int h, int flags = 0);

	virtual ~GLApp();

	/** Definition du mode d'affichage. */
	virtual int setVideoMode(int w,int h,int flags = 0);
	/** Definition du titre de la fenetre. */
	virtual void setTitle(const char *title);

	/** Demarage de l'application. */
	virtual void run();

	/** Activation de l'affichage du framerate.*/
	virtual void showFPS(bool b){
		dispFPS=b;
	}
};

}
#endif
