#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "Bresenham.h"
#include "ScanLine.h"

typedef struct poly_vert {
	Point p ;
	Color c ;
	struct poly_vert *next ;
} PolygonVertex ;

typedef struct {
	int n;
	int is_filled , is_closed ;
	PolygonVertex *current_vertex ;
	PolygonVertex *head , *tail ;
} Polygon ;

Polygon P_new() ;

void P_add_vertex( Polygon *Poly , Point p , Color c ) ;

void P_insert( Polygon *Poly , PolygonVertex *prec , PolygonVertex *post , Point p , Color c ) ;

void P_remove( Polygon *Poly , PolygonVertex *p ) ;

PolygonVertex * P_closest_vertex( Polygon *Poly , int x , int y ) ;

void Pedro( Image *I , Polygon *Poly ) ;

#endif
