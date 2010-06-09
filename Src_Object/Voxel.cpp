#include "Voxel.h"

Voxel::Voxel(){
	plein = false ;
	frontiere = false ;
	combustibleRestant = 0.0 ;
	temperature = 0.0 ;
	repulsion = Vecteur3D(0.0,0.0,0.0);
	tauxConversion = 0.0 ;
	tempThreshold = 0.0 ;
	diffusion = 0.0 ;
	conductance = 0.0;
	pos = Vecteur3I( 0,0,0 );
}
	
Voxel::Voxel( bool plein,  
	      bool frontiere, 
	      float combustibleRestant, 
	      float temperature, 
	      Vecteur3D repulsion, 
	      float tauxConversion, 
	      float tempThreshold, 
	      float diffusion,  
	      float conductance, 
	      Vecteur3I pos ) 
	:plein(plein),
	 frontiere(frontiere),
	 combustibleRestant(combustibleRestant),
	 temperature(temperature), 
	 temperature0(temperature), 
	 repulsion(repulsion), 
	 tauxConversion(tauxConversion),
	 tempThreshold(tempThreshold),
	 diffusion(diffusion), 
	 conductance(conductance), 
	 pos(pos)
{}
