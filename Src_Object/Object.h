#ifndef OBJECT_H
#define OBJECT_H
/*!*****************************************************************************
 *
 *       \file       :      Object.h
 *
 *       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
 *       \date       :      4 jun 2010
 *       \version    :      0.1
 *
 *******************************************************************************/
#include "BasicEntite.h"
#include "SolverParam.h"

class Object : public BasicEntite {
protected :
	/* attributes */
	/* diffusion of temperature within object */ 
	float diffusionParamTemp ; 
	/* density of the object */
	float density ; 
	/* gaz emission rate */
	float gazEmissionRate ;
	/* pyrolysis threshold */
	float pyrolysisThreshold;
	/* presence info */
	float *voxel;
	Vecteur3I voxelSize;
	
	/* protected methods */
	virtual void generateVoxel() = 0 ;

public :
	/* constructor */
	Object();
	
	/* destructor */
	virtual ~Object();
	
	/* getters and setters */
	float getDiffusionParamTemp() ;
	void  setDiffusionParamTemp( float diff ) ;

	float getDensity() ;
	void  setDensity( float dens ) ;
	
	float getGazEmissionRate() ;
	void  setGazEmissionRate( float emr ) ;

	float getPyrolysisThreshold() ;
	void  setPyrolysisThreshold( float pt ) ;
	
	/* get presence info */
	float* getPresenceInfo();
	Vecteur3I getPresenceInfoSize();
	virtual bool isInside( Vecteur3D p ) = 0 ;

	inline Vecteur3D cellToPoint( int i, int j , int k ){
		Vecteur3D res = AABB.lowerCorner ;
		res.x += i*SolverParam::getSpaceDiv() ;
		res.y += j*SolverParam::getSpaceDiv() ;
		res.z += k*SolverParam::getSpaceDiv() ;
		return res ;
	}

	/* herited methods */
	virtual void Afficher(float dt) = 0;
	virtual void Afficher_Face_Camera(Vecteur3D& positionCamera,
					  Vecteur3D& directionCamera,
					  float dt ) ; 
};

#endif
