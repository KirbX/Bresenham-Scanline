#include "Polygon.h"

PolygonVertex * PV_new( Point p , Color c )
{
	PolygonVertex *pv = malloc( sizeof *pv ) ;

	pv->p = p ;
	pv->c = c ;
	pv->next = NULL ;

	return pv ;
}

Polygon P_new() 
{
	Polygon p ;

	p.is_filled = p.is_closed = 0 ;
	p.current_vertex = NULL ;
	p.head = p.tail = NULL ;

	return p ;
}

static inline int P_is_empty( Polygon *p )
{
	return p->head == NULL ;
}

void P_add_vertex( Polygon *Poly , Point p , Color c ) 
{
	PolygonVertex *pv = PV_new( p , c ) ;

	if( P_is_empty( Poly ) )
		Poly->head = Poly->tail = pv ;

	else
	{
		Poly->tail->next = pv ;
		Poly->tail = pv ;
	}
}

void P_insert( Polygon *Poly , PolygonVertex *prec , PolygonVertex *post , Point p , Color c ) 
{
	PolygonVertex *pv = PV_new( p , c ) ;	

	pv->next = post ;
	prec->next = pv ;
}

void P_remove( Polygon *Poly , PolygonVertex *p ) 
{
	PolygonVertex *it1 = NULL , *it2 = Poly->head ;

	if( it2 == p )
	{
		Poly->head = it2->next ;
	}
	else
	{
		while( it2 != p )
		{
			it1 = it2 ;
			it2 = it2->next ;
		}
		it1->next = it2->next ;
	}
	free( it2 ) ;
}

static inline int __carre( int x )
{
	return x*x ;
}

static inline int __carred_euclid_distance( Point p , int x , int y )
{
	return __carre( p.x - x ) + __carre( p.y - y ) ;
}

PolygonVertex * P_closest_vertex( Polygon *Poly , int x , int y ) 
{
	PolygonVertex *pv = Poly->head ,
		      *it = Poly->head->next ;

	int ced , min = __carred_euclid_distance( pv->p , x , y ) ;

	while( it != NULL )
	{
		ced = __carred_euclid_distance( it->p , x , y ) ;
		if( min > ced )
		{
			pv = it	;
			min = ced ;
		}
	}

	return pv ;
}

static void __dro_edge( Image *I , PolygonVertex *v1 , PolygonVertex *v2 )
{
	I_bresenham( I , v1->p.x , v1->p.y , v2->p.x , v2->p.y ) ;
}

void Pedro( Image *I , Polygon *Poly ) 
{
	PolygonVertex *it = Poly->head ;

	if( it == Poly->tail )
		I_plot( I , it->p.x , it->p.y ) ;
	else
	{
		while( it->next != NULL )
		{
			__dro_edge( I , it , it->next ) ;
		}
		if( Poly->is_closed ) 
			__dro_edge( I , Poly->tail , Poly->head ) ;
	}
}

