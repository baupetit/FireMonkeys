
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "BMP.h"

#define HDRLEN (54)

BMP::BMP(){
	pixels = 0;
}

BMP::~BMP(){
	if( pixels ) delete [] pixels ;
}

void BMP::newImage( int w, int h ){
	unsigned long size;
	if (w<1 || h<1)
		return;

	if (w & 3) 
		w += 4 - (w & 3);
	if (h & 3) 
		h += 4 - (h & 3);

	width = w;
	height = h;
	size = w * h;
	pixels = new unsigned long[3*size];
}

void BMP::clear( unsigned char r , unsigned char g , unsigned char b ){
	if( pixels ){
		for( int i = 0; i < width*height*3 ; i+=3 ){
			pixels[i] = r ;
			pixels[i+1] = g ;
			pixels[i+2] = b ;
			
		}
	}
}

int BMP::write( string path ){
	FILE *f;
	unsigned char h[HDRLEN];
	unsigned long len;
	int i, j;

	if (!pixels)
		return -1;
	//----------

	//----------------------------------------
	// Create the file.
	//
	f = fopen (path.c_str(), "wb");
	/*
	if (!f) {
		perror ("fopen");
		return 0;
	}
	*/

	//----------------------------------------
	// Prepare header
	//
	for( int i = 0 ; i < HDRLEN; ++i ) h[i] = 0;
	len = HDRLEN + 3 * width * height;

	h[0] = 'B'; 
	h[1] = 'M';
	h[2] = len & 0xff;
	h[3] = (len >> 8) & 0xff;
	h[4] = (len >> 16) & 0xff;
	h[5] = (len >> 24) & 0xff;
	h[10] = HDRLEN;
	h[14] = 40;
	h[18] = width & 0xff;
	h[19] = (width >> 8) & 0xff;
	h[20] = (width >> 16) & 0xff;
	h[22] = height & 0xff;
	h[23] = (height >> 8) & 0xff;
	h[24] = (height >> 16) & 0xff;
	h[26] = 1;
	h[28] = 24;
	h[34] = 16;
	h[36] = 0x13; // 2835 pixels/meter
	h[37] = 0x0b;
	h[42] = 0x13; // 2835 pixels/meter
	h[43] = 0x0b;

	//----------------------------------------
	// Write header.
	//
	if (HDRLEN != fwrite (h, 1, HDRLEN, f)) {
		//perror ("fwrite");
		fclose (f);
		return 0;
	}

	//----------------------------------------
	// Write pixels.
	// Note that BMP has lower rows first.
	//
	long size = width * height * 3 ;
	if( size != fwrite( pixels, 1, size, f) )  {
		fclose (f);
		return 0;
	}
	/*
	for (j=height-1; j >= 0; j--) {
		for (i=0; i < width; i++) {
			unsigned char rgb[3];
			int ix = i + j * width;
			unsigned long pixel = pixels[ix];
			rgb[0] = (pixel >> 8) & 0xff;
			rgb[1] = (pixel >> 16) & 0xff;
			rgb[2] = (pixel >> 24) & 0xff;
			if (3 != fwrite (rgb, 1, 3, f)) {
				//perror ("fwrite");
				fclose (f);
				return 0;
			}
		}
	}
	*/
	fclose (f);
	return 1;

}
