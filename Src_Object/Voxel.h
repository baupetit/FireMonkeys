#ifndef VOXEL_H
#define VOXEL_H

#include "Vecteur.h"

struct Voxel {
	
	bool plein ;
	bool frontiere ;
	float combustibleRestant ;
	float temperature ;
	Vecteur3D repulsion ;
	float tauxConversion ;
	float tempThreshold ;
	float conduction ;
	Vecteur3I pos ;

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
