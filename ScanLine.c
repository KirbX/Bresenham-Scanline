#include "ScanLine.h" 

static void __TAA_remove( Arete **TAA , int n , Arete *a )
{
	int i = 0 ;
	while( TAA[i] != a )
		++ i ;
	
	memmove( TAA+i , TAA+i+1 , n-i-1 ) ;	
}

static void __TAA_insert( Arete **TAA , int n , Arete *a )
{
	int j , i = 0 ;

	while( i < n-1 && TAA[i]->x_inters < a->x_inters )
		++ i ;	

	for( j = n-1 ; j != i ; ++ j )
		TAA[j] = TAA[j-1] ;

	TAA[i] = a ;
}

void I_scanline( Image *I , Arete *TA , const int nb_aretes , const int ymin , const int ymax , Color cnew )
{
	int x , y , i , n , dx , dy ,
	    ind_next = 0 , 
	    aretes_actives = 0 ,
	    next_ymin = TA[ind_next].pmin.y ;

	Arete *a2i , *a2i1 , *a ;
	Arete **TAA = malloc( nb_aretes * sizeof *TAA ) ;

	for( y = ymin ; y <= ymax ; ++ y )
	{
		/* on ajoute à la table des arete actives les aretes intersectees par le scanline courrant */
		while( y >= next_ymin )
		{
			TA[ind_next].x_inters = TA[ind_next].pmin.x ;
			TA[ind_next].inc = 0 ;
			__TAA_insert( TAA , aretes_actives , &TA[i] ) ;
			aretes_actives += 1 ;
			ind_next += 1 ;
			next_ymin = TA[ind_next].pmin.y ;
		}

		/* on enleve les aretes qui ne sont plus intersectees par le scanline */
		for( i = 0 ; i < aretes_actives ; ++ i )
		{
			if( y == TAA[i]->pmax.y )
			{
				__TAA_remove( TAA , aretes_actives , TAA[i] ) ;
				aretes_actives -= 1 ;
			}
		}

		/* Coloriage */
		n = aretes_actives / 2 ;

		for( i = 0 ; i < n ; ++ i )
		{
			a2i = TAA[2*i] ;
			a2i1 = TAA[2*i+1] ;
			for( x = a2i->x_inters ; x <= a2i1->x_inters ; ++ x )
				I_plotColor( I , x , y , cnew ) ;
		}

		/* on recalcule l'intersection en x avec le prochain scanline pour chaque arete */
		for( i = 0 ; i < aretes_actives ; ++ i )
		{
			a = TAA[i] ;
			dx = a->pmax.x - a->pmin.x ;
			dy = a->pmax.y - a->pmin.y ;
			a->inc += dx ;

			while( a->inc > dy )
			{
				a->inc -= dy ;
				a->x_inters += 1 ;
			}
			while( a->inc < -dy )
			{
				a->inc += dy ;
				a->x_inters -= 1 ;
			}
		}
	}
	free( TAA ) ;
}
