#ifndef SOLVERPARAM_H
#define SOLVERPARAM_H

#include <iostream>

class SolverParam {
protected :
	/**
	 * Parameters used by the solver
	 */
	static float diffusionParamFire        ;
	static float diffusionParamSmoke       ;
	static float diffusionParamTemperature ;

	static float coolingParam              ;
	static float consumingParam            ;
	static float tempMaxParam              ;
	static float fireToSmokeParam          ;
	
	static float viscosityParam            ;
	static float buoyancyParam             ;
	static float vorticityConfinementParam ;
	
	/**
	 * Protected constructor : we do not want anyone to
	 * instanciate any of these
	 */
	SolverParam(){}
public :
	/** getters */
	inline static float getDiffusionParamFire()         ;
	inline static float getDiffusionParamSmoke()        ;
	inline static float getDiffusionParamTemperature()  ;
	inline static float getCoolingParam()               ;
	inline static float getConsumingParam()             ;
	inline static float getTempMaxParam()               ;
	inline static float getFireToSmokeParam()           ;
	inline static float getViscosityParam()             ;
	inline static float getBuoyancyParam()              ;
	inline static float getVorticityConfinementParam()  ;
	

	/** setters */
	inline static float setDiffusionParamFire( float param )        ;
	inline static float setDiffusionParamSmoke( float param )       ;
	inline static float setDiffusionParamTemperature( float param ) ;
	inline static float setCoolingParam( float param )              ;
	inline static float setConsumingParam( float param )            ;
	inline static float setTempMaxParam( float param )              ;
	inline static float setFireToSmokeParam( float param )          ;
	inline static float setViscosityParam( float param )            ;
	inline static float setBuoyancyParam( float param )             ;
	inline static float setVorticityConfinementParam( float param ) ;
	
};

float SolverParam::getDiffusionParamFire()         { return diffusionParamFire; }
float SolverParam::getDiffusionParamSmoke()        { return diffusionParamSmoke; }
float SolverParam::getDiffusionParamTemperature()  { return diffusionParamTemperature; }
float SolverParam::getCoolingParam()               { return coolingParam; }
float SolverParam::getConsumingParam()             { return consumingParam; }
float SolverParam::getTempMaxParam()               { return tempMaxParam; }
float SolverParam::getFireToSmokeParam()           { return fireToSmokeParam; }
float SolverParam::getViscosityParam()             { return viscosityParam; }
float SolverParam::getBuoyancyParam()              { return buoyancyParam; }
float SolverParam::getVorticityConfinementParam()  { return vorticityConfinementParam; }

float SolverParam::setDiffusionParamFire( float param )         { diffusionParamFire = param ; }
float SolverParam::setDiffusionParamSmoke( float param )        { diffusionParamSmoke = param ; }
float SolverParam::setDiffusionParamTemperature( float param )  { diffusionParamTemperature = param ; }
float SolverParam::setCoolingParam( float param )               { coolingParam = param ; }
float SolverParam::setConsumingParam( float param )             { consumingParam = param ; }
float SolverParam::setTempMaxParam( float param )               { tempMaxParam = param ; }
float SolverParam::setFireToSmokeParam( float param )           { fireToSmokeParam = param ; }
float SolverParam::setViscosityParam( float param )             { viscosityParam = param ; }
float SolverParam::setBuoyancyParam( float param )              { buoyancyParam = param ; }
float SolverParam::setVorticityConfinementParam( float param )  { vorticityConfinementParam = param ; }

#endif
