#ifndef __SCANLINE_H__
#define __SCANLINE_H__

#include <assert.h>

#include "Image.h"

typedef struct {
	int x , y ;
} Point ;

typedef struct {
	Point pmin , pmax ;
	int x_inters , inc ;
} Arete ;

void I_scanline( Image *I , Arete *TA , const int nb_aretes , const int ymin , const int ymax ) ;

#endif
