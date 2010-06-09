#include "ObjectViewer.h"

#include "Voxel.h"
#include <iostream>
using namespace std;

static Voxel def( true, 
		  false, 
		  0,
		  0,
		  Vecteur3D(0,0,0), 
		  0,
		  0,
		  0, 
		  0,
		  Vecteur3I(0,0,0));

ObjectViewer::ObjectViewer()
	:Viewer("Object Viewer", 800, 600),
	 s(def,1,Vecteur4D(0,1,55/(float)255,1.0), 100,100)
{
	
}

ObjectViewer::~ObjectViewer(){
}

void ObjectViewer::rendu(){
	s.Afficher(0);
}
