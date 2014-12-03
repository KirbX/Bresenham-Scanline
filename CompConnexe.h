#ifndef __COMP_CONNEXE_H__
#define __COMP_CONNEXE_H__

#include "Image.h"

enum ConexType {
	CON4 ,
	CON8
} ;
static Color 
	black = { 0 , 0 , 0 },
	white = { 1 , 1 , 1 }; 

typedef struct {
	Image *img ;
	int *labels ;
	int ncon ;
} CompConnexes ;

CompConnexes calcCompConnexes( Image *I , Color c , enum ConexType ct ) ;

void ccDraw( CompConnexes cc , Image *I , int ccnum ) ;

#endif
