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
	int is_edge; //is_edge = 1 => edge || is_edge = 0 => vertex
	PolygonVertex *current_vertex ;
	PolygonVertex *head , *tail ;
} Polygon ;

Polygon P_new() ;

void P_add_vertex( Polygon *Poly , Point p , Color c ) ;

void P_insert( Polygon *Poly , PolygonVertex *prec , PolygonVertex *post , Point p , Color c ) ;

void P_remove( Polygon *Poly , PolygonVertex *p ) ;

void P_inc_current( Polygon *Poly );
void P_dec_current( Polygon *Poly );

PolygonVertex * P_closest_vertex( Polygon *Poly , int x , int y ) ;
PolygonVertex * P_closest_edge( Polygon* Poly, int x, int y);

void Pedro( Image *I , Polygon *Poly ) ;

#endif
