#include "Polygon.h"
#include "math.h"

static const float EPSILON = 1e-4;

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

	if (!(Poly->n==0))
	{
		Poly->n--;
		if (Poly->n==0)
		{
			Poly->head=NULL;
			Poly->tail=NULL;
			Poly->current_vertex = NULL;
		}
		if( it2 == p )
		{
			Poly->head = it2->next ;
			Poly->current_vertex = it2->next;
		}
		else
		{
			while( it2 != p )
			{
				it1 = it2 ;
				it2 = it2->next ;
			}
			if (it2 == Poly->tail)
			{
				Poly->tail = it1;
			}
			it1->next = it2->next ;
			Poly->current_vertex = it1;
		}
		free( it2 ) ;
	}
}

void P_inc_current( Polygon *Poly )
{
	if (Poly->current_vertex != NULL)
	{
		if (Poly->current_vertex == Poly->tail)
			Poly->current_vertex = Poly->head;
		else if (Poly->current_vertex->next != NULL)
			Poly->current_vertex = Poly->current_vertex->next;
	}
}

void P_dec_current( Polygon *Poly )
{
	PolygonVertex *it = Poly->head ;
	if (Poly->current_vertex != NULL) 
	{
		if (Poly->current_vertex == Poly->head)
			Poly->current_vertex = Poly->tail;
		else{
			while (it->next != Poly->current_vertex)
				it = it->next;
			Poly->current_vertex = it;
		}
	}
}

void _Inc_Y(Image *img, PolygonVertex *current)
{
	if (current->p.y + 1 < img->_height)
		current->p.y ++;
}

void _Dec_Y(Image *img, PolygonVertex *current)
{
	if (current->p.y - 1 >= 0)
		current->p.y --;
}

void _Inc_X(Image *img, PolygonVertex *current)
{
	if (current->p.x + 1 < img->_width)
		current->p.x ++;
}

void _Dec_X(Image *img, PolygonVertex *current)
{
	if (current->p.x - 1 >= 0)
		current->p.x --;
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
		it=it->next;
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

static int __fetch_edge_array_and_pmax(Polygon *Poly , Arete *TA )
{
	PolygonVertex *it = Poly->head;

	int i = 0 , ymax = it->p.y ;

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

		if( it->p.y > ymax )
			ymax = it->p.y ;

		it = it->next ;
		i++;
	}
	if (Poly->head->p.y < Poly->tail->p.y)
	{
		TA[i].pmin = Poly->head->p;
		TA[i].pmax = Poly->tail->p;
	}
	else
	{
		TA[i].pmin = Poly->tail->p;
		TA[i].pmax = Poly->head->p;
	}

	if( Poly->tail->p.y > ymax )
		ymax = Poly->tail->p.y ;

	qsort(TA, Poly->n, sizeof(Arete), (__compar_fn_t)&A_cmp );

	return ymax ;
}

void Pedro( Image *I , Polygon *Poly ) 
{
	int ymax ;
	Arete *TA;
	PolygonVertex *it = Poly->head ;

	if (!P_is_empty(Poly))
	{ 
		if( it == Poly->tail)
			I_plot( I , it->p.x , it->p.y ) ;
		else
		{
			if ( Poly->is_filled )
			{
				TA = malloc( Poly->n * sizeof( *TA ) ) ;
				ymax = __fetch_edge_array_and_pmax( Poly , TA ) ;
				I_scanline(I, TA, Poly->n, TA[0].pmin.y, ymax );
				free( TA ) ;
			}
			else
			{
				while( it->next != NULL )
				{
					__dro_edge( I , it , it->next ) ;
					it = it->next;
				}
				if( Poly->is_closed ) 
					__dro_edge( I , Poly->tail , Poly->head ) ;
			}
		}
	}

}



/*************************************************/
static inline float __det(Point A, Point B, Point M)
{
	return (A.x * B.y) + (M.x * A.y) + (B.x * M.y) - (M.x * B.y) - (B.x * A.y) - (A.x * M.y);
}

static inline int __dist_carre(Point a, Point b)
{
	return __carre( a.x - b.x ) + __carre( a.y - b.y ) ;
}

static float __dist_pt_xy(Point a, int x, int y){
	return sqrt(__carre(a.x-x) + __carre(a.y-y));

}
static float __dot(Point a, Point b){
	return (float)a.x*b.x + a.y*b.y;
}

static float __dist_edge_point(Point a, Point b, int x, int y) //FAUX
{
	// |b-a| ^ 2
	int l2 = __dist_carre(a, b);
	// si l2 == 0, a et b sont sur le meme point, donc on renvoie la distance entre un point et a
	if ( l2 == 0) return __dist_pt_xy(a, x, y);
	//projection de (x,y) sur ab
	//(avec P (x,y)
	//ca tombe sur ab lorsque t vaut :
	//t = __dot[(P-a), (B-a)] / l2
	float t = __dot((Point) {x - a.x, y - a.y},(Point) {b.x - a.x, b.y - a.y}) / l2;
	//si t est "avant" a: 
	if (t < 0.0) return __dist_pt_xy(a, x, y);
	//si t est "apres" b:
	else if (t > 1.0) return __dist_pt_xy(b, x, y);
	//sinon, la projection est:
	Point projection = (Point){a.x + t * (b.x - a.x), a.y +t * (b.y - a.y)};
	return __dist_pt_xy(projection, x,y);

}

PolygonVertex * P_closest_edge( Polygon* Poly, int x, int y)
{
	PolygonVertex *pv = Poly->head ,
				  *it = Poly->head->next ;
	float ced , min = __dist_edge_point( pv->p, pv->next->p , x , y ) ;

	while( it != NULL && it->next !=NULL)
	{
		ced = __dist_edge_point( it->p, it->next->p , x , y ) ;
		if( min > ced )
		{
			pv = it	;
			min = ced ;
		}
		it=it->next;
	}
	if (Poly->tail != NULL)
	{
		ced = __dist_edge_point ( Poly->tail->p, Poly->head->p, x, y);
	}
	if (min > ced){
		return Poly->tail;
	}
	return pv ;
}
