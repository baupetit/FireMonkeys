#include "Perlin.h"

#include <cmath>

#include <cstdlib>
#include <ctime>

#include <unistd.h>


Perlin::Perlin(): _dimension(1) ,
				  _pas(1) , 
				  _nbOctaves(1) ,
				  _width(1) , 
				  _height(1) ,
				  _depth(1) ,
				  _persistance(0.0)	
{
}

Perlin::Perlin( int dim, int pas , int oct , int w , int h , int d , float  persist): 
		_dimension(dim) ,
		_pas(pas) , 
		_nbOctaves(oct) ,
		_width(w) , 
		_height(h) ,
		_depth(d) ,
		_persistance(persist)
{
}

Perlin::~Perlin() { delete[] _noise; }


void Perlin::init()
{


	int puissance_max = 1;
	for (int o=1; o<=_nbOctaves  - 1; o++)
	{
		puissance_max *= 2;
	}


	int l = (int) ceil(_width * puissance_max / _pas);
    int h2 = (int) ceil(_height * puissance_max / _pas);
	int p = (int) ceil(_depth * puissance_max / _pas);

	srand(time(NULL));

	_noise = new Vecteur3D[l*h2*p];
	for (int i = 0 ; i < p*h2*l ; i++){
		float i1 = ((float) rand()) / RAND_MAX;
		float i2 = ((float) rand()) / RAND_MAX;
		float i3 = ((float) rand()) / RAND_MAX;
				
		_noise[i] = Vecteur3D(i1,i2,i3); 
	}

}

//interpolation linéaire
Vecteur3D Perlin::interpolation_lineaire(Vecteur3D &a, Vecteur3D &b , float xx )
{
	return (a * (1 - xx) + b * xx);
}

//interpolation cosinusoidale sur 1D
Vecteur3D Perlin::interpolation_cos1D(Vecteur3D &a, Vecteur3D &b , float xx )
{
	float k = (1 - cos(xx * M_PI)) / 2;	
   	return interpolation_lineaire(a, b, k);

}



//interpolation cosinusoidale sur 2D
Vecteur3D Perlin::interpolation_cos2D(Vecteur3D &a , Vecteur3D &b , 
								  	  Vecteur3D &c , Vecteur3D &d ,
								 	  float x1     , float x2 )
{
	Vecteur3D i1 = interpolation_cos1D(a,b,x1);
	Vecteur3D i2 = interpolation_cos1D(c,d,x1);

	return interpolation_cos1D(i1,i2,x2);
}



//interpolation cosinusoidale sur 3D
Vecteur3D Perlin::interpolation_cos3D(Vecteur3D &a , Vecteur3D &b , 
								   	   Vecteur3D &c , Vecteur3D &d ,
								       Vecteur3D &e , Vecteur3D &f ,
								       Vecteur3D &g , Vecteur3D &h , 
								       float     x1 , float     x2 , float x3 )
{
	Vecteur3D i1 = interpolation_cos2D(a,b,c,d,x1,x2);
	Vecteur3D i2 = interpolation_cos2D(e,f,g,h,x1,x2);

	return interpolation_cos1D(i1,i2,x3);
}





//generation de la fonction de bruit.	
Vecteur3D Perlin::fonctionBruit1D(float xx)
{
	int i1 = (int) (xx / _pas);
	return interpolation_cos1D(_noise[i1] , _noise[i1+1] , fmod((xx / _pas) , 1));
}



Vecteur3D Perlin::fonctionBruit2D(float xx, float yy)
{
	int i = (int) (xx / _pas);
	int j = (int) (yy / _pas);
	return interpolation_cos2D( _noise[i + j*_width]       , _noise[i + 1 + j*_width]   ,
								_noise[i + (j+1)*_width]   , _noise[i + 1 + (j+1)*_width] ,
								fmod((xx / _pas) , 1 )     	   , fmod((yy / _pas) , 1 )
							  );
}



Vecteur3D Perlin::fonctionBruit3D(float xx, float yy, float zz)
{
	int i = (int) (xx / _pas);
	int j = (int) (yy / _pas);
	int k = (int) (zz / _pas);
	return interpolation_cos3D( _noise[i + j*_width + k*_depth*_depth]   , _noise[i + 1 + j*_width + k*_depth*_depth]   ,
								_noise[i + (j+1)*_width + k*_depth*_depth]   , _noise[i + j*_width + (k+1)*_depth*_depth] ,
 								_noise[i + 1 + (j+1)*_width + k*_depth*_depth]   , _noise[i + 1 + j*_width + (k+1)*_depth*_depth]   ,
								_noise[i + (j+1)*_width + (k+1)*_depth*_depth]   , _noise[i + 1 + (j+1)*_width + (k+1)*_depth*_depth] ,
								fmod((xx / _pas) , 1)        , fmod((yy / _pas) , 1) , fmod((zz / _pas) , 1)
							  );

}

Vecteur3D Perlin::bruitDePerlin1D(float x,float persistance ,int nb_octave)
{
	Vecteur3D perlin = Vecteur3D(0.0,0.0,0.0);
	float p = 1;
	int f = 1;

	for(int i = 0 ; i < nb_octave ; i++) {
        perlin += p * fonctionBruit1D(x * f);
    	p *= persistance;
        f *= 2;
    }
 
    return perlin * ((1 - persistance) / (1 - p));		
}


Vecteur3D Perlin::bruitDePerlin2D(float x,float y,float persistance ,int nb_octave)
{
	Vecteur3D perlin = Vecteur3D(0.0,0.0,0.0);
	float p = 1;
	int f = 1;

	for(int i = 0 ; i < nb_octave ; i++) {
        perlin += p * fonctionBruit2D(x * f , y * f);
    	p *= persistance;
        f *= 2;
    }
 
    return perlin * ((1 - persistance) / (1 - p));	
}


Vecteur3D Perlin::bruitDePerlin3D(float x,float y,float z,float persistance ,int nb_octave)
{
	Vecteur3D perlin = Vecteur3D(0.0,0.0,0.0);
	float p = 1;
	int f = 1;

	for(int i = 0 ; i < nb_octave ; i++) {
        perlin += p * fonctionBruit3D(x * f , y * f , z * f);
    	p *= persistance;
        f *= 2;
    }
 
    return perlin * ((1 - persistance) / (1 - p));	
}





Vecteur3D* Perlin::getNoise()
{

	return _noise;

}


Vecteur3D* Perlin::genererNoise()
{
	Vecteur3D *res = new Vecteur3D[_width*_height*_depth];

	for (int k = 0 ; k < _depth ; k++){ 
		for (int j = 0 ; j < _height ; j++){ 
			for (int i = 0 ; i < _width ; i++){ 

				res[i+j*_height+k*_depth*_depth] = (bruitDePerlin3D((float) i,(float) j,(float) k, _persistance ,_nbOctaves) - 0.5f)*2.0f;
				
			}	
		}
	}

	return res;

}



