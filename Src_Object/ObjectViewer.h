#ifndef OBJECTVIEWER_H
#define OBJECTVIEWER_H

#include "Viewer.h"
#include "Sphere.h"

class ObjectViewer : public Viewer {
protected :
	Sphere s;
public :
	ObjectViewer();
	~ObjectViewer();
	
	void rendu();
};

#endif
