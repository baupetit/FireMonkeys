#include "ObjectViewer.h"

#include <iostream>
using namespace std;

ObjectViewer::ObjectViewer()
	:Viewer("Object Viewer", 800, 600),
	 s(10,Vecteur4D(0,1,55/(float)255,1.0), 100,100)
{
	
}

ObjectViewer::~ObjectViewer(){
}

void ObjectViewer::rendu(){
	s.Afficher(0);
}
