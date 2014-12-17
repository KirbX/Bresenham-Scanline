
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "Bresenham.h"
#include "CompConnexe.h"
#include "ScanLine.h"

Image *img;
CompConnexes cc ;
unsigned int cc_num = 0 ;


Point tab[] = {
(Point)	{ 50 , 0 } ,
(Point)	{ 50 , 25 } , 
(Point)	{ 50 , 50 } , 
(Point)	{ 25 , 50 } , 
(Point)	{ 0 , 50 } , 
(Point)	{ -25 , 50 } , 
(Point)	{ -50 , 50 } ,
(Point)	{ -50 , 25 } , 
(Point)	{ -50 , 0 } , 
(Point)	{ -50 , -25 }, 
(Point)	{ -50 , -50 } , 
(Point)	{ -25 , -50 } , 
(Point)	{ 0 , -50 } , 
(Point)	{ 25 , -50 } ,
(Point) { 50 , -50 } ,
(Point) { 50 , -25 } 
} ;

int inc = 0;

void nextCompCon()
{
	if( cc.ncon != 0 )
	{
		cc_num =  cc_num % cc.ncon + 1 ; 
		ccDraw( cc , img , cc_num ) ;
	}
}

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

	I_draw(img);

    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN))
		I_focusPoint(img,x,img->_height-y);

	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch(key)
	{
	case 27 : exit(1); break;
	case 'z' : I_zoom(img,2.0); break;
	case 'Z' : I_zoom(img,0.5); break;
	case 'i' : I_zoomInit(img); break;
	case 'n' : nextCompCon()  ; break;
	case 'b' : 
		I_fill( img , white ) ;
		I_changeColor( img , black ) ;
		I_bresenham( img , 50 , 50 , 50 + tab[inc].x , 50 + tab[inc].y ) ;
		inc = (inc+1) % (sizeof tab / sizeof tab[0] );
		break ;
	default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 10;

	switch(key)
	{
	case GLUT_KEY_UP    : I_move(img,0,d); break;
	case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
	case GLUT_KEY_LEFT  : I_move(img,d,0); break;
	case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
	default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if((argc!=3)&&(argc!=2))
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\nou",argv[0]);
		fprintf(stderr,"\t: %s <ppmfilename> \n\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if(argc==2)
		{
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur,hauteur);
			I_fill( img , white );
		}
		int windowPosX = 100, windowPosY = 100;

		//cc = calcCompConnexes( img , black , CON8 ) ;
		//cc_num = cc.ncon != 0 ;
		//ccDraw( cc , img , cc_num );
		
		//I_changeColor( img , black ) ;
		//I_bresenham( img , 50 , 50 , 25 , 25  ) ;

		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,0,hauteur,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}