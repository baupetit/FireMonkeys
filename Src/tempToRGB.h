#ifndef TEMPTORGB_H
#define TEMPTORGB_H

/*
 *      RGB VALUES FOR HOT OBJECTS   by Dan Bruton (astro@sfasu.edu)
 *
 *      This program can be found at
 *           http://www.physics.sfasu.edu/astro/color.html
 *      and was last updated on March 19, 1996.
 *
 *      Reference Book
 *      Color Science : Concepts and Methods, Quantitative Data and Formula
 *                      by Gunter Wyszecki, W. S. Stiles
 *                      John Wiley & Sons; ISBN: 0471021067
 *
 *      This program will calculate the RGB values for a given
 *      energy distribution as a function of wavelength of light.
 *      A black body is used and an example.
 *
 *      NetPBM's ppmtogif can be used to convert the ppm image generated
 *      to a gif.  The red, green and blue values (RGB) are
 *      assumed to vary linearly with wavelength (for GAM=1).
 *      NetPBM Software: ftp://ftp.cs.ubc.ca/ftp/archive/netpbm/
 ***
 *      Converted to C by William T. Bridgman (bridgman@wyeth.gsfc.nasa.gov)
 *      in February, 2000.
 *        - Original integration method replaced by trapezoid rule.
 *
 */

class TempToRGB {
public :
	TempToRGB( int size , float step );
	
	~TempToRGB();
	
	void getRGB( float temperature, 
		     float *R, 
		     float *G, 
		     float *B );

protected :
	struct _RGB{
		float R;
		float G;
		float B;
	};

	struct _RGB *tab;

	int size ;
	float step;

	void BlackBody(float temperature, 
	       float *X, float *Y,float *Z);

	void XYZ2RGB(float xr, float yr, float zr, 
		     float xg, float yg, float zg,
		     float xb, float yb, float zb, 
		     float xc, float yc, float zc,
		     float *red, float *green,float *blue);
	
	float DMAX1(float x, float y, float z);
	
	void tempToRGB(float temperature, 
		       float *R, 
		       float *G, 
		       float *B );
};

#endif
