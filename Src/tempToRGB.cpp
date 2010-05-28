
#include "tempToRGB.h"

#include <cstdlib>
#include <cmath>

TempToRGB::TempToRGB( int size, float step )
	:size(size),step(step)
{
	tab = new struct _RGB[size];
	int i;
	float T;
	for(i=0, T=0; i<size; ++i, T+=step ){
		tempToRGB( T , &tab[i].R, &tab[i].G, &tab[i].B );
	}
}

TempToRGB::~TempToRGB(){
	delete [] tab ;
}

void TempToRGB::getRGB( float temperature, 
			float *R, 
			float *G, 
			float *B )
{
	int index = (int)temperature/step;
	index = (index >= size)?size-1:((index<0)?0:index);
	*R = tab[index].R ;
	*G = tab[index].G ;
	*B = tab[index].B ;
}


void TempToRGB::tempToRGB(float temperature, float *R, float *G, float*B ){
	/*
	 *      Chromaticity Coordinates for Red, Green, Blue and White
	 */
	float  XRed=0.64, YRed=0.33;
	float  XGreen=0.29, YGreen=0.60;
	float  XBlue=0.15, YBlue=0.06;
	float  XWhite=0.3127, YWhite=0.3291;
	float  ZRed, ZGreen, ZBlue, ZWhite;
	float  X, Y, Z;
	
	ZRed    =1.0-(XRed+YRed);
	ZGreen  =1.0-(XGreen+YGreen);
	ZBlue   =1.0-(XBlue+YBlue);
	ZWhite  =1.0-(XWhite+YWhite);
	/*
	 *      FIND COLOR VALUE
	 */
        BlackBody(temperature,&X,&Y,&Z); /* X,Y,Z */

        XYZ2RGB(XRed,YRed,ZRed,XGreen,YGreen,ZGreen,XBlue,YBlue,ZBlue,
                      X,Y,Z, R, G, B); /* convert to RGB */
}
/*
 ****************************************************************************
 *
 *      XYZ VALUES FROM TEMPERATURE OF OBJECT
 *
 *       A black body approximation is used where the temperature,
 *       T, is given in Kelvin.  The XYZ values are determined by
 *      "integrating" the product of the wavelength distribution of
 *       energy and the XYZ functions for a uniform source.
 */
void TempToRGB::BlackBody(float temperature, float *X, float *Y,float *Z) {

/*
 *      CIE Color Matching Functions (x_bar,y_bar,z_bar)
 *      for wavelenghts in 5 nm increments from 380 nm to 780 nm.
 */
       float fColorMatch[][3]={
         {0.0014, 0.0000, 0.0065},
         {0.0022, 0.0001, 0.0105},
         {0.0042, 0.0001, 0.0201},
         {0.0076, 0.0002, 0.0362},
         {0.0143, 0.0004, 0.0679},
         {0.0232, 0.0006, 0.1102},
         {0.0435, 0.0012, 0.2074},
         {0.0776, 0.0022, 0.3713},
         {0.1344, 0.0040, 0.6456},
         {0.2148, 0.0073, 1.0391},
         {0.2839, 0.0116, 1.3856},
         {0.3285, 0.0168, 1.6230},
         {0.3483, 0.0230, 1.7471},
         {0.3481, 0.0298, 1.7826},
         {0.3362, 0.0380, 1.7721},
         {0.3187, 0.0480, 1.7441},
         {0.2908, 0.0600, 1.6692},
         {0.2511, 0.0739, 1.5281},
         {0.1954, 0.0910, 1.2876},
         {0.1421, 0.1126, 1.0419},
         {0.0956, 0.1390, 0.8130},
         {0.0580, 0.1693, 0.6162},
         {0.0320, 0.2080, 0.4652},
         {0.0147, 0.2586, 0.3533},
         {0.0049, 0.3230, 0.2720},
         {0.0024, 0.4073, 0.2123},
         {0.0093, 0.5030, 0.1582},
         {0.0291, 0.6082, 0.1117},
         {0.0633, 0.7100, 0.0782},
         {0.1096, 0.7932, 0.0573},
         {0.1655, 0.8620, 0.0422},
         {0.2257, 0.9149, 0.0298},
         {0.2904, 0.9540, 0.0203},
         {0.3597, 0.9803, 0.0134},
         {0.4334, 0.9950, 0.0087},
         {0.5121, 1.0000, 0.0057},
         {0.5945, 0.9950, 0.0039},
         {0.6784, 0.9786, 0.0027},
         {0.7621, 0.9520, 0.0021},
         {0.8425, 0.9154, 0.0018},
         {0.9163, 0.8700, 0.0017},
         {0.9786, 0.8163, 0.0014},
         {1.0263, 0.7570, 0.0011},
         {1.0567, 0.6949, 0.0010},
         {1.0622, 0.6310, 0.0008},
         {1.0456, 0.5668, 0.0006},
         {1.0026, 0.5030, 0.0003},
         {0.9384, 0.4412, 0.0002},
         {0.8544, 0.3810, 0.0002},
         {0.7514, 0.3210, 0.0001},
         {0.6424, 0.2650, 0.0000},
         {0.5419, 0.2170, 0.0000},
         {0.4479, 0.1750, 0.0000},
         {0.3608, 0.1382, 0.0000},
         {0.2835, 0.1070, 0.0000},
         {0.2187, 0.0816, 0.0000},
         {0.1649, 0.0610, 0.0000},
         {0.1212, 0.0446, 0.0000},
         {0.0874, 0.0320, 0.0000},
         {0.0636, 0.0232, 0.0000},
         {0.0468, 0.0170, 0.0000},
         {0.0329, 0.0119, 0.0000},
         {0.0227, 0.0082, 0.0000},
         {0.0158, 0.0057, 0.0000},
         {0.0114, 0.0041, 0.0000},
         {0.0081, 0.0029, 0.0000},
         {0.0058, 0.0021, 0.0000},
         {0.0041, 0.0015, 0.0000},
         {0.0029, 0.0010, 0.0000},
         {0.0020, 0.0007, 0.0000},
         {0.0014, 0.0005, 0.0000},
         {0.0010, 0.0004, 0.0000},
         {0.0007, 0.0002, 0.0000},
         {0.0005, 0.0002, 0.0000},
         {0.0003, 0.0001, 0.0000},
         {0.0002, 0.0001, 0.0000},
         {0.0002, 0.0001, 0.0000},
         {0.0001, 0.0000, 0.0000},
         {0.0001, 0.0000, 0.0000},
         {0.0001, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}};

    float XX=0.0, YY=0.0, ZZ=0.0; /* initialize accumulators */
    float con, dis, wavelength, weight;
    short band, nbands=81;

    /* loop over wavelength bands
     * integration by trapezoid method
     */
    for(band=0; band<nbands; band++) {
        weight=1.0;
        if((band==0)||(band==nbands-1)) weight=0.5; /* properly weight end
points */
        wavelength=380.0+(float)band*5.0;/* wavelength in nm */
        /* generate a black body spectrum */
        con=1240.0/8.617e-5;

dis=3.74183e-16*(1.0/pow(wavelength,5))/(exp(con/(wavelength*temperature))-1.);
/* simple integration over bands */
        XX=XX+weight*dis*fColorMatch[band][0];
        YY=YY+weight*dis*fColorMatch[band][1];
        ZZ=ZZ+weight*dis*fColorMatch[band][2];
    } /* end of 'band' loop */

    /* re-normalize the color scale */
    *X=XX/DMAX1(XX,YY,ZZ);
    *Y=YY/DMAX1(XX,YY,ZZ);
    *Z=ZZ/DMAX1(XX,YY,ZZ);

}

/*
 *********************************************************************
 */
void TempToRGB::XYZ2RGB(float xr, float yr, float zr,
              float xg, float yg, float zg,
              float xb, float yb, float zb,
              float xColor, float yColor, float zColor,
              float *red, float *green,float *blue) {

    float denominator;

    denominator=(xr*yg-xg*yr)*zb+(xb*yr-xr*yb)*zg+(xg*yb-xb*yg)*zr;

    *red=((xColor*yg-xg*yColor)*zb+(xg*yb-xb*yg)*zColor
        +(xb*yColor-xColor*yb)*zg)/denominator;
    *green=((xr*yColor-xColor*yr)*zb+(xb*yr-xr*yb)*zColor
        +(xColor*yb-xb*yColor)*zr)/denominator;
    *blue=((xr*yg-xg*yr)*zColor+(xColor*yr-xr*yColor)*zg
        +(xg*yColor-xColor*yg)*zr)/denominator;

    if(*red<0.0) *red=0.0;
    if(*red>1.0) *red=1.0;
    if(*green<0.0) *green=0.0;
    if(*green>1.0) *green=1.0;
    if(*blue<0.0) *blue=0.0;
    if(*blue>1.0) *blue=1.0;
}

/* ************************************************************************/
float TempToRGB::DMAX1(float x, float y, float z) {
    float max;
    max=x;
    if(y>max) max=y;
    if(z>max) max=z;
    return max;
}

