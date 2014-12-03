#include "Bresenham.h"

#define BITS_PER_BYTE 8
#define NB_BITS(I) sizeof I * BITS_PER_BYTE

typedef void (*plot_func_t)(Image*,int,int) ;

static void I_reversePlot( Image *img , int x , int y )
{
	I_plot( img , y , x ) ;
}

/*
 * Précondition :
 * la droite passant par (x0,y0) et de vecteur directeur (dx,dy)
 * doit se trouver dans le premier octant
 */
static void __bresenham( Image *img , int x0 , int y0 , int dx , int dy , int incr , plot_func_t plot )
{
	int y = y0 ,
	    c = dx ,
	    x ;

	for( x = x0; x <= x0 + dx ; ++ x )
	{
		plot( img , x , y ) ;

		if( c <= 0 )
		{
			c +=  2 * (dx - dy);
			y += incr ;
		}
		else
			c -= 2 * dy;		
	}
}

static inline void __int_swap( int *i1 , int *i2 )
{
	int tmp = *i1 ;
	*i1 = *i2 ;
	*i2 = tmp ;
}

static inline void __point_swap( int *xA , int *yA , int *xB , int *yB ) 
{
	__int_swap( xA , xB ) ;
	__int_swap( yA , yB ) ;
}

static inline int __abs( int i )
{
	int mask = i >> ( NB_BITS(i) - 1 ) ;

	return ( mask + i ) ^ mask ;
}

void I_bresenham(Image *img, int xA, int yA, int xB, int yB) 
{
	int dx = __abs( xB - xA ) , 
	    dy = __abs( yB - yA ) ,
	    incr = 1 ;

	plot_func_t plot = &I_plot ;

	/*
	 * Pour les octants 2 , 3 , 6 et 7 :
	 * On se ramène aux octants 1 , 4 , 5 et 8 respectivement
	 */
	if( dy > dx )
	{
		__int_swap( &xA , &yA ) ;
		__int_swap( &xB , &yB ) ;
		
		plot = &I_reversePlot ;

		__int_swap( &dx , &dy ) ;	
	}

	/*
	 * Pour les octants 4 et 5 
	 * On se ramène aux octants 1 et 8 respectivement
	 */
	if( xA > xB )
		__point_swap( &xA , &yA , &xB , &yB ) ;	

	/*
	 * Pour l'octant 8 on se ramène à l'octant 1
	 */
	incr -= ( yB < yA ) * 2 ;

	__bresenham( img , xA , yA , dx , dy , incr , plot ) ;
}
