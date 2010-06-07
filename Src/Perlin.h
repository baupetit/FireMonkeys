#ifndef PERLIN_H
#define PERLIN_H

/*!*****************************************************************************
*
*       \file       :      Perlin.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      31 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vecteur.h"

class Perlin {
public:
	
	
	Perlin();
	Perlin( int dim , int pas , int oct , int w , int h , int d , float  persist );
	~Perlin();

	void init();
	Vecteur3D* genererNoise();
	Vecteur3D* getNoise();

private:
	Vecteur3D* _noise;
	int _dimension;
	int _pas;
	int _nbOctaves;
	int _width;
	int _height;
	int _depth;
	float _persistance;




	Vecteur3D interpolation_lineaire(Vecteur3D &a, Vecteur3D &b , float xx );

	Vecteur3D interpolation_cos1D(Vecteur3D &a, Vecteur3D &b , float xx );
	Vecteur3D interpolation_cos2D(Vecteur3D &a , Vecteur3D &b , 
								   Vecteur3D &c , Vecteur3D &d ,
								   float x1 , float x2 );
	Vecteur3D interpolation_cos3D(Vecteur3D &a , Vecteur3D &b , 
								  Vecteur3D &c , Vecteur3D &d ,
								  Vecteur3D &e , Vecteur3D &f ,
								  Vecteur3D &g , Vecteur3D &h , 
								  float x1 , float x2 , float x3 );
	
	

	Vecteur3D fonctionBruit1D(float xx);
	Vecteur3D fonctionBruit2D(float xx, float yy);
	Vecteur3D fonctionBruit3D(float xx, float yy, float zz);

	Vecteur3D bruitDePerlin1D(float x,float persistance ,int nb_octave);
	Vecteur3D bruitDePerlin2D(float x,float y,float persistance ,int nb_octave);
	Vecteur3D bruitDePerlin3D(float x,float y,float z,float persistance ,int nb_octave);

};

#endif


