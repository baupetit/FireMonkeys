#ifndef _BMP_H
#define _BMP_H

#include <string>
using namespace std;

struct BMP{
	int width, height;
	unsigned long *pixels;
	
	BMP();
	~BMP();

	/* allocate a new image */
	void newImage( int w, int h) ;
	
	/* clear image */
	void clear(unsigned long clearColor );

	/* write an image */
	int write( string path );
};

#endif

