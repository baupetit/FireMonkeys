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
	
	Voxel(){
		plein = false ;
		frontiere = false ;
		combustibleRestant = 0.0 ;
		temperature = 0.0 ;
		repulsion = Vecteur3D(0.0,0.0,0.0);
		tauxConversion = 0.0 ;
		tempThreshold = 0.0 ;
		conduction = 0.0 ;
	}
	
	Voxel( bool plein,  bool frontiere, float combustibleRestant, float temperature, Vecteur3D repulsion, float tempThreshold, float conduction ) 
		:plein(plein),frontiere(frontiere),combustibleRestant(combustibleRestant),
		 temperature(temperature), repulsion(repulsion), tempThreshold(tempThreshold),
		 conduction(conduction)
		{}
		
	    
        
};
#endif
