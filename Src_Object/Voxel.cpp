#include "Voxel.h"

Voxel::Voxel(){
	plein = false ;
	frontiere = false ;
	combustibleRestant = 0.0 ;
	temperature = 0.0 ;
	repulsion = Vecteur3D(0.0,0.0,0.0);
	tauxConversion = 0.0 ;
	tauxPerte = 0.0 ;
	tempThreshold = 0.0 ;
	diffusion = 0.0 ;
	conductance = 0.0;
	pos = Vecteur3I( 0,0,0 );
	nb_faces = 0;
	tauxFumee = 0.0;

}
	
Voxel::Voxel( bool plein,  
	      bool frontiere, 
	      float combustibleRestant, 
	      float temperature, 
	      Vecteur3D repulsion, 
	      float tempThreshold, 
	      float tauxConversion, 
	      float tauxPerte, 
	      float diffusion,  
	      float conductance, 
	      float tauxFumee,
	      Vecteur3I pos ) 
	:plein(plein),
	 frontiere(frontiere),
	 combustibleRestant(combustibleRestant),
	 temperature(temperature), 
	 temperature0(temperature), 
	 repulsion(repulsion),
	 tauxPerte(tauxPerte),
	 tauxConversion(tauxConversion),
	 tempThreshold(tempThreshold),
	 diffusion(diffusion), 
	 conductance(conductance), 
	 tauxFumee(tauxFumee),
	 pos(pos)
{
	nb_faces = 0;
}
