#include "CompConnexe.h"

static CompConnexes cc_new( Image *I ) 
{
	CompConnexes cc ;
	cc.img = I ;
	cc.labels = calloc( I->_width * I->_height , sizeof *cc.labels ) ;
	cc.ncon = 0 ;
	return cc ;
}

static void setLabel( CompConnexes cc , int x , int y , int l )
{
	cc.labels[ y * cc.img->_width + x ] = l ;
} 

static int getLabel( CompConnexes cc , int x , int y )
{
	return cc.labels[ y * cc.img->_width + x ] ;
} 

static int isInsideBoundaries( CompConnexes cc , int x , int y )
{
	return x >= 0 && x < cc.img->_width && y >= 0 && y < cc.img->_height ;
}

static void fill8( CompConnexes cc , int x , int y , Color c )
{
	int i , j ;

	if(    isInsideBoundaries( cc , x , y ) 
	    && getLabel( cc , x , y ) == 0 
	    && C_cmp ( cc.img->_buffer[x][y] , c ) )
	{
		setLabel( cc , x , y , cc.ncon ) ;
		for( i = -1 ; i <= 1 ; ++ i )
			for( j = -1 ; j <= 1 ; ++ j )
				if( !( i == 0 && j == 0 ) ) 
					fill8( cc , x+i , y+j , c ) ;
	}
}

static void fill4( CompConnexes cc , int x , int y , Color c )
{
	if(    isInsideBoundaries( cc , x , y ) 
	    && getLabel( cc , x , y ) == 0 
	    && C_cmp ( cc.img->_buffer[x][y] , c ) )
	{
		setLabel( cc , x , y , cc.ncon ) ;
		fill4( cc , x + 1 , y , c ) ;
		fill4( cc , x - 1 , y , c ) ;
		fill4( cc , x  , y + 1, c ) ;
		fill4( cc , x  , y - 1, c ) ;
	}
}

CompConnexes calcCompConnexes( Image *I , Color c , enum ConexType ct )
{
	CompConnexes cc = cc_new( I ) ;
	
	int i , j ;

	void(*fillfunc)(CompConnexes,int,int,Color) = 
		ct == CON4 ? &fill4 : &fill8 ;

	for( i = 0 ; i < I->_width ; ++ i )
		for( j = 0 ; j < I->_height ; ++ j )

			if(    C_cmp( cc.img->_buffer[i][j] , c ) 
			    && getLabel( cc , i , j ) == 0 )
			{
				cc.ncon ++ ;
				fillfunc( cc , i , j , c ) ;
			}

	return cc ; 
}
 

void ccDraw( CompConnexes cc , Image *I , int ccnum ) 
{
	int i , j ;
	Color c ;
	for( i = 0 ; i < I->_width ; ++ i )
		for( j = 0 ; j < I->_height ; ++ j )
		{
			c = getLabel( cc , i , j ) == ccnum ? black : white ;
			I_plotColor( I , i , j , c ) ;
		}	
}
	
