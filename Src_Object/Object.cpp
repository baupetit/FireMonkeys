#include "Object.h"

Object::Object(){
	voxel = 0;
}

/* destructor */
Object::~Object(){
	if( voxel )
		delete [] voxel;
}

/* getters and setters */
float Object::getDiffusionParamTemp() {
	return diffusionParamTemp;
}
void  Object::setDiffusionParamTemp( float diff ) {
	diffusionParamTemp = diff;
}

float Object::getDensity() {
	return density;
}
void  Object::setDensity( float dens ) {
	density = dens ;
}

float Object::getGazEmissionRate() {
	return gazEmissionRate;
}
void  Object::setGazEmissionRate( float emr ) {
	gazEmissionRate = emr ;
}

float Object::getPyrolysisThreshold() {
	return pyrolysisThreshold;
}
void  Object::setPyrolysisThreshold( float pt ) {
	pyrolysisThreshold = pt ;
}

float* Object::getPresenceInfo(){
	return voxel;
}
Vecteur3I Object::getPresenceInfoSize(){
	return voxelSize;
}


/* herited methods */
void Object::Afficher(float dt) {
}

void Object::Afficher_Face_Camera(Vecteur3D& positionCamera,
				  Vecteur3D& directionCamera,
				  float dt ) {
	Afficher( dt );
}
