#ifndef __SCANLINE_H__
#define __SCANLINE_H__

#include "Image.h"

typedef struct {
	int x , y ;
} Point ;

typedef struct {
	Point top_left , bottom_right ;
} BoundingBox ;

void I_scanline( Image*, BoundingBox, Color );

#endif
