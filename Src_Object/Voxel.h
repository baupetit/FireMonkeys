#ifndef VOXEL_H
#define VOXEL_H

#include "Vecteur.h"

struct Voxel {
	
	bool plein ;
	bool frontiere ;
	float combustibleRestant ;
	float temperature;
	float temperature0;
	Vecteur3D repulsion ;
	float tauxConversion ;
	float tempThreshold ;
	float diffusion ;
	float conductance;
	Vecteur3I pos;
	
	Voxel();
	
	Voxel( bool plein,  bool frontiere, float combustibleRestant, float temperature, Vecteur3D repulsion, float tempThreshold, float tauxConversion, float diffusion, float conductance, Vecteur3I position );
		
	    
        

};
#endif
