#include "ScanLine.h" 

void I_scanline( Image *I , BoundingBox bb , Color edge_color )
{
	int i , j ;
	int draw , col_eq ; 

	for( i = bb.top_left.x ; i < bb.bottom_right.x ; ++ i )
	{
		draw = 0 ;
		for( j = bb.top_left.y ; j < bb.bottom_right.y ; ++ j )
		{
			col_eq = C_cmp( I->_buffer[i][j] , edge_color ) ;

			/* 
			 * Si l'on ne dessine pas et que
			 * l'on recontre une arete alors on se met à dessiner
			 * Sinon si on dessine et 
			 * qu'on rencontre une arete alors on ne dessine plus
			 */
			draw = !draw * col_eq + draw * !col_eq ;

			if( draw )
				I_plotColor( I , i , j , edge_color ) ;
		}
	}
}
