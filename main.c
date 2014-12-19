
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

#include "Polygon.h"
Polygon Poly;
int _V = 0; //bool pour mode VERTEX
int _E = 0; //bool pour mode EDGE

static const Color white = (Color) { 1.0 , 1.0 , 1.0 } ; 
static const Color black = (Color) { 0.0 , 0.0 , 0.0 } ;
int inc=0;

Image *img;
static int __Gut_Y(int y)
{
    return img->_height-y;
}

static void __Carre (Image *img, int x, int y){
    I_changeColor(img, (Color) {1.0,0.0,0.0});
    I_bresenham( img, x-4, __Gut_Y(y)+4, x+4, __Gut_Y(y)+4);
    I_bresenham( img, x+4, __Gut_Y(y)+4, x+4, __Gut_Y(y)-4);
    I_bresenham( img, x+4, __Gut_Y(y)-4, x-4, __Gut_Y(y)-4);
    I_bresenham( img, x-4, __Gut_Y(y)-4, x-4, __Gut_Y(y)+4);
}

static void __Carre_PV(Image *img, PolygonVertex *pv){
    __Carre(img, pv->p.x, __Gut_Y(pv->p.y));
}

static void I_refresh()
{
    I_fill( img, white ) ;
    I_changeColor(img, black) ;
    Pedro(img, &Poly);
    if (_V && Poly.current_vertex !=NULL)
        __Carre_PV(img, Poly.current_vertex);

    if (_E && Poly.current_vertex !=NULL && Poly.current_vertex->next != NULL)
    {
        __Carre_PV(img, Poly.current_vertex);
        __Carre_PV(img, Poly.current_vertex->next);
    }
    if (_E && Poly.current_vertex == Poly.tail){
        __Carre_PV(img, Poly.current_vertex);
        __Carre_PV(img, Poly.head);
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
    {
        if (!_V && !_E)
        {
            I_focusPoint(img,x,img->_height-y);
            Point pn = (Point) { x , __Gut_Y(y) };
            P_add_vertex (&Poly, pn, black);
            I_refresh();
        }
        else if (_V)
        {
            if (Poly.n != 0)
            Poly.current_vertex = P_closest_vertex(&Poly, x, __Gut_Y(y));
            I_refresh();
        }
        if (_E)
        {
            if (Poly.n >= 2)
            Poly.current_vertex = P_closest_edge(&Poly, x, __Gut_Y(y));
            I_refresh();
        }
    }
    if((button==GLUT_RIGHT_BUTTON)&&(state==GLUT_DOWN))
    {
        if (!_V && !_E)
            P_remove(&Poly, Poly.tail);
        I_refresh();
    }
    if((button==GLUT_MIDDLE_BUTTON)&&(state==GLUT_DOWN))
    {
        if (_E)
        {
            if (Poly.current_vertex != NULL)
            {
                P_insert(&Poly, Poly.current_vertex, Poly.current_vertex->next, (Point) {(Poly.current_vertex->p.x + Poly.current_vertex->next->p.x) / 2, (Poly.current_vertex->p.y + Poly.current_vertex->next->p.y) / 2}, (Color){0.0,0.0,0.0});
                _E = !_E;
                _V = !_V;
                fprintf(stderr,"On est en mode vertex et on sort du mode edge, la\n");
                Poly.current_vertex = Poly.current_vertex->next;
            }
            I_refresh();
        }
    }

	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27 : exit(1); break;
	case 'z' : I_zoom(img,2.0); break;
	case 'Z' : I_zoom(img,0.5); break;
	case 'i' : I_zoomInit(img); break;
	case 'c' : Poly.is_closed=!Poly.is_closed; break;
	case 'f' : Poly.is_filled=!Poly.is_filled; break;
    case 'v' :
               _V = !_V; (_V==1)?fprintf(stderr, "On passe en mode vertex\n"):fprintf(stderr,"On sort du mode Vertex\n"); 
               Poly.is_edge = 1; 
               if (_E)
               {
                   _E = 0;
                   fprintf(stderr, "Et on sort du mode Edge\n");
               }
               break;
    case 'e' :
               _E = !_E; (_E==1)?fprintf(stderr, "On passe en mode edge\n"):fprintf(stderr,"On sort du mode Edge\n");
               Poly.is_edge = 0;
               if (_V)
               {
                   _V = 0;
                   fprintf(stderr, "Et on sort du mode Vertex\n");
               }
               break;
    case 127 : if (_V && Poly.current_vertex != NULL ) {
                    P_remove(&Poly, Poly.current_vertex);
               if (Poly.n == 0)
               {
                   fprintf(stderr," Plus de point, on repasse en mode normal\n");
                   _V=!_V;
               }
               }
                   break;
	default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
	I_refresh();
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
	case GLUT_KEY_UP    : 
	    if (!_E && !_V)
	        I_move(img,0,d);
	    if (_V){
	        if (Poly.current_vertex != NULL)
                _Inc_Y(img, Poly.current_vertex);
	        I_refresh();
        }
	    break;
	case GLUT_KEY_DOWN  :
	    if (!_E && !_V)
	        I_move(img,0,-d);
	    if (_V){
	        if (Poly.current_vertex != NULL)
                _Dec_Y(img, Poly.current_vertex);
	        I_refresh();
        }
	    break;
	case GLUT_KEY_LEFT  :
	    if (!_E && !_V)
	        I_move(img,d,0); 
	    if (_V){
	        if (Poly.current_vertex != NULL)
                _Dec_X(img, Poly.current_vertex);
	        I_refresh();
        }
        break;
	case GLUT_KEY_RIGHT :
	    if (!_E && !_V)
	        I_move(img,-d,0);
	    if (_V){
	        if (Poly.current_vertex != NULL)
                _Inc_X(img, Poly.current_vertex);
	        I_refresh();
        }
	    break;
	case GLUT_KEY_PAGE_UP  : if (_V || _E)
                                P_inc_current(&Poly);
                        break;
	case GLUT_KEY_PAGE_DOWN: if (_V || _E)
	                            P_dec_current(&Poly);
	                    break;
	default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	I_refresh();
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
		Poly = P_new();
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
