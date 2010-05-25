/**** Geometry Shader Marching Cubes
	* Copyright Cyril Crassin, January 2007.
	* This code is partially based on the example of 
	* Paul Bourke "Polygonising a scalar field" located at :
	* http://local.wasp.uwa.edu.au/~pbourke/geometry/polygonise/
****/

#include "GLAppTestG80.h"

int main(int argc, char** argv){
	
	GLAppTestG80 *app=new GLAppTestG80(&argc, argv, "Geometry Shader Marching Cubes, Cyril Crassin 2007", 512, 512);


	app->run();
}