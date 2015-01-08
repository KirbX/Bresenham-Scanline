#include "ScanLine.h" 

static void __TAA_remove( Arete **TAA , const int n , const int i )
{
	int j ;
	for( j = i ; j < n-1 ; ++ j )
		TAA[j] = TAA[j+1] ;	
}

static int __cmp_aretes( Arete **a1 , Arete **a2 )
{
	return (*a1)->x_inters - (*a2)->x_inters ;
}

void I_scanline( Image *I , Arete *TA , const int nb_aretes , const int ymin , const int ymax )
{
	int x , y , i , n , dx , dy ,
	    ind_next = 0 , 
	    aretes_actives = 0 ;

	Arete *a2i , *a2i1 , *a ;
	Arete **TAA = calloc( nb_aretes , sizeof *TAA ) ;

	for( y = ymin ; y <= ymax ; ++ y )
	{
		/* on ajoute à la table des arête actives les arêtes intersectées par le scanline courant */
		while( ind_next < nb_aretes && y >= TA[ind_next].pmin.y )
		{
			TA[ind_next].x_inters = TA[ind_next].pmin.x ;
			TA[ind_next].inc = 0 ;
			
			TAA[aretes_actives] = &TA[ind_next] ;
			aretes_actives += 1 ;

			ind_next += 1 ;
		}

		/* On doit trier pour les polygones non simples au cas où les arêtes se sont croisées */
		/* Il existe des méthodes plus efficaces mais ca suffit pour l'instant */
		qsort( TAA , aretes_actives , sizeof *TAA ,  (__compar_fn_t)&__cmp_aretes ) ;

		/* on enlève les arêtes qui ne sont plus intersectées par le scanline */
		for( i = 0 ; i < aretes_actives ; ++ i )
		{
			if( y == TAA[i]->pmax.y )
			{
				__TAA_remove( TAA , aretes_actives , i ) ;
				aretes_actives -= 1 ;
				i --  ;
			}
		}

		/* Coloriage */
		n = aretes_actives / 2 ;

		for( i = 0 ; i < n ; ++ i )
		{
			a2i = TAA[2*i] ;
			a2i1 = TAA[2*i+1] ;

			for( x = a2i->x_inters ; x <= a2i1->x_inters ; ++ x )
				I_plot( I , x , y ) ;
		}

		/* on calcule pour chaque arête actibe l'intersection en x avec le prochain scanline */
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
