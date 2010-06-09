#ifndef VOXEL_H
#define VOXEL_H

#include "Vecteur.h"

struct Voxel {
	bool plein ;
	bool frontiere ;
	float combustibleRestant ;
	float temperature ;
	float temperature0;
	Vecteur3D repulsion ;
	float tauxConversion ;
	float tempThreshold ;
	float diffusion ;
	float conductance;
	
	Voxel(){
		plein = false ;
		frontiere = false ;
		combustibleRestant = 0.0 ;
		temperature = 0.0 ;
		repulsion = Vecteur3D(0.0,0.0,0.0);
		tauxConversion = 0.0 ;
		tempThreshold = 0.0 ;
		diffusion = 0.0 ;
		conductance = 0.0;
	}
	
	Voxel( bool plein,  bool frontiere, float combustibleRestant, float temperature, float temperature0, Vecteur3D repulsion, float tempThreshold, float diffusion, float conductance ) 
		:plein(plein),frontiere(frontiere),combustibleRestant(combustibleRestant),
		 temperature(temperature), temperature0(temperature0), repulsion(repulsion), tempThreshold(tempThreshold),
		 diffusion(diffusion), conductance(conductance) 
		{}
		
	    
        
};
#endif
