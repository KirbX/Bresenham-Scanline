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

    p.n=0;
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

    Poly->n++;

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
    
    Poly->n++;

	pv->next = post ;
	prec->next = pv ;
}

void P_remove( Polygon *Poly , PolygonVertex *p ) 
{
	PolygonVertex *it1 = NULL , *it2 = Poly->head ;
    
    Poly->n--;

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

static int A_cmp(Arete *a1, Arete *a2)
{
    return a1->pmin.y - a2->pmin.y;
}

static Arete* __fetch_edge_array(Polygon *Poly)
{
   Arete* TA = malloc(Poly->n * sizeof *TA);
   
   PolygonVertex *it = Poly->head;

   int i = 0;
   while (it->next != NULL)
   {
       if (it->p.y < it->next->p.y)
       {
          TA[i].pmin = it->p;
          TA[i].pmax = it->next->p;
       }
       else
       {
          TA[i].pmin = it->next->p;
          TA[i].pmax = it->p;
       }
       i++;
   }
   if (Poly->head->p.y < Poly->tail->p.y)
   {
       TA[i].pmin = Poly->head->p;
       TA[i].pmax = Poly->tail->p;
   }
   else
   {
       TA[i].pmax = Poly->head->p;
       TA[i].pmin = Poly->tail->p;
   }
   
   qsort(TA, Poly->n, sizeof(Arete), (__compar_fn_t)&A_cmp);
   
   return TA;
}

void Pedro( Image *I , Polygon *Poly ) 
{
    Arete *TA;
	PolygonVertex *it = Poly->head ;
    if (!P_is_empty(Poly))
    { 
        if( it == Poly->tail)
            I_plot( I , it->p.x , it->p.y ) ;
        else
        {
            while( it->next != NULL )
            {
                __dro_edge( I , it , it->next ) ;
                it = it->next;
            }
            if( Poly->is_closed ) 
                __dro_edge( I , Poly->tail , Poly->head ) ;
            if ( Poly->is_filled )
            {
                TA = __fetch_edge_array(Poly);
                I_scanline(I, TA, Poly->n, TA[0].pmin.y, TA[Poly->n-1].pmax.y, (Color) {0,0,0} );
            }
        }
    }

}

