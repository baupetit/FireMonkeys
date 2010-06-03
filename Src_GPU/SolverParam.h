#ifndef SOLVERPARAM_H
#define SOLVERPARAM_H

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


    static float echantillonage;
	
	/**
	 * Protected constructor : we do not want anyone to
	 * instanciate any of these
	 */
	SolverParam(){}
	
public :
	/** getters */
	static float getDiffusionParamFire()         ;
	static float getDiffusionParamSmoke()        ;
	static float getDiffusionParamTemperature()  ;
	static float getCoolingParam()               ;
	static float getConsumingParam()             ;
	static float getTempMaxParam()               ;
	static float getFireToSmokeParam()           ;
	static float getViscosityParam()             ;
	static float getBuoyancyParam()              ;
	static float getVorticityConfinementParam()  ;
	static float getEchantillonage()  ;
	

	/** setters */
	static void setDiffusionParamFire( float param )        ;
	static void setDiffusionParamSmoke( float param )       ;
	static void setDiffusionParamTemperature( float param ) ;
	static void setCoolingParam( float param )              ;
	static void setConsumingParam( float param )            ;
	static void setTempMaxParam( float param )              ;
	static void setFireToSmokeParam( float param )          ;
	static void setViscosityParam( float param )            ;
	static void setBuoyancyParam( float param )             ;
	static void setVorticityConfinementParam( float param ) ;
	static void setEchantillonage( float param)             ;

	static void printParams() ;
    static void initSolverParam() ;
};

#endif
