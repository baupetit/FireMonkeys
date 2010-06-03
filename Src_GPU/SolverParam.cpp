#include "SolverParam.h"

#include <iostream>
using namespace std;

float SolverParam::diffusionParamFire        = 0.0;
float SolverParam::diffusionParamSmoke       = 0.0;
float SolverParam::diffusionParamTemperature = 0.0;
float SolverParam::coolingParam              = 0.0;
float SolverParam::consumingParam            = 0.0;
float SolverParam::tempMaxParam              = 0.0;
float SolverParam::fireToSmokeParam          = 0.0;
float SolverParam::viscosityParam            = 0.0;
float SolverParam::buoyancyParam             = 0.0;
float SolverParam::vorticityConfinementParam = 0.0;
float SolverParam::echantillonage = 0.0;

float SolverParam::getDiffusionParamFire()         { return SolverParam::diffusionParamFire; }
float SolverParam::getDiffusionParamSmoke()        { return SolverParam::diffusionParamSmoke; }
float SolverParam::getDiffusionParamTemperature()  { return SolverParam::diffusionParamTemperature; }
float SolverParam::getCoolingParam()               { return SolverParam::coolingParam; }
float SolverParam::getConsumingParam()             { return SolverParam::consumingParam; }
float SolverParam::getTempMaxParam()               { return SolverParam::tempMaxParam; }
float SolverParam::getFireToSmokeParam()           { return SolverParam::fireToSmokeParam; }
float SolverParam::getViscosityParam()             { return SolverParam::viscosityParam; }
float SolverParam::getBuoyancyParam()              { return SolverParam::buoyancyParam; }
float SolverParam::getVorticityConfinementParam()  { return SolverParam::vorticityConfinementParam; }
float SolverParam::getEchantillonage()             { return SolverParam::echantillonage; }

void SolverParam::setDiffusionParamFire( float param )         { SolverParam::diffusionParamFire = param ; }
void SolverParam::setDiffusionParamSmoke( float param )        { SolverParam::diffusionParamSmoke = param ; }
void SolverParam::setDiffusionParamTemperature( float param )  { SolverParam::diffusionParamTemperature = param ; }
void SolverParam::setCoolingParam( float param )               { SolverParam::coolingParam = param ; }
void SolverParam::setConsumingParam( float param )             { SolverParam::consumingParam = param ; }
void SolverParam::setTempMaxParam( float param )               { SolverParam::tempMaxParam = param ; }
void SolverParam::setFireToSmokeParam( float param )           { SolverParam::fireToSmokeParam = param ; }
void SolverParam::setViscosityParam( float param )             { SolverParam::viscosityParam = param ; }
void SolverParam::setBuoyancyParam( float param )              { SolverParam::buoyancyParam = param ; }
void SolverParam::setVorticityConfinementParam( float param )  { SolverParam::vorticityConfinementParam = param ; }
void SolverParam::setEchantillonage( float param)              { SolverParam::echantillonage = param ; }

void SolverParam::printParams() {
	cout << "Solver params are :" << endl ;
	cout << "    DiffusionParamFire        : " << diffusionParamFire        << endl ;
	cout << "    DiffusionParamFire        : " << diffusionParamSmoke       << endl ;
	cout << "    DiffusionParamTemperature : " << diffusionParamTemperature << endl ;
	cout << "    CoolingParam              : " << coolingParam              << endl ;
	cout << "    ConsumingParam            : " << consumingParam            << endl ;
	cout << "    TempMaxParam              : " << tempMaxParam              << endl ;
	cout << "    FireToSmokeParam          : " << fireToSmokeParam          << endl ;
	cout << "    ViscosityParam            : " << viscosityParam            << endl ;
	cout << "    BuoyancyParam             : " << buoyancyParam             << endl ;
	cout << "    VorticityConfinementParam : " << vorticityConfinementParam << endl ;
	cout << endl << endl ;
}



void SolverParam::initSolverParam() {
	SolverParam::diffusionParamFire        = 0.001;
	SolverParam::diffusionParamSmoke       = 0.00001;
	SolverParam::diffusionParamTemperature = 0.01;
	SolverParam::coolingParam              = 1.5;
	SolverParam::consumingParam            = 1.0;
	SolverParam::tempMaxParam              = 0.4;
	SolverParam::fireToSmokeParam          = 180.0;
	SolverParam::viscosityParam            = 0.0;
	SolverParam::buoyancyParam             = 0.5;
	SolverParam::vorticityConfinementParam = 2.0;
	SolverParam::echantillonage            = 1/128;
}

