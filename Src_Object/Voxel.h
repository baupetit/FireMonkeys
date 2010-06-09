#ifndef VOXEL_H
#define VOXEL_H

#include "Vecteur.h"
#include "Triangle.h"

struct Voxel {

	// pour le modele d'objet
	bool plein ;
	bool frontiere ;
	float combustibleRestant ;
	float temperature ;
	Vecteur3D repulsion ;
	float tauxConversion ;
	float tempThreshold ;
	float conduction ;
	Vecteur3I pos ;
	
	// pour le marching cube
	Vecteur3D corner[8];
	float valuation[8];
	
	int nb_faces;
	Triangle faces[10];
	
	Voxel();
	
	Voxel( bool plein,  
	       bool frontiere, 
	       float combustibleRestant, 
	       float temperature, 
	       Vecteur3D repulsion, 
	       float tauxConversion,
	       float tempThreshold, 
	       float conduction,
	       Vecteur3I pos );		
};
#endif
