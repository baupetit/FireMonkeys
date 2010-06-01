#ifndef VIEWER_H
#define VIEWER_H

/*!*****************************************************************************
*
*       \file       :      Viewer.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/


#include <string>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GL/glut.h>


class Viewer{

public :
    Viewer(const string & title, int width, int height,
	       int initPositionX = 0, int initPositionY = 0);
	       
    ~Viewer();
    
    void start();
    
    virtual void rendu() = 0;;
    
private :
    // Instance
    static Viewer *_instance;

    // Camera
	float _cameraAngle;
	float _distToOrigin;
	float _cameraHeight;
	
	void  _setCamera();
	
    // Initialisations
	void _initGlut(const string & title, int width, int height, 
	               int initPositionX, int initPositionY);
	void _initCamera();
	void _initDepth();
	void _initRendering();
	void _initLighting();
	
	// Callbacks
	static void _displayFunc();
	static void _keyboardFunc(unsigned char key, int x, int y);
	static void _motionFunc(int x, int y);
	static void _mouseFunc(int button, int state, int x, int y);
	static void _passiveMotionFunc(int x, int y);
	static void _reshapeFunc(int w, int h);
	static void _specialFunc(int key, int x, int y);
	static void _visibilityFunc(int visible);
	
	// Touches clavier
	void  _keyMap(int key, int x, int y);

};


#endif
