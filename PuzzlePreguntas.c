/**
 * ! Puzzle de preguntas!
 * * Lara Xocuis Martha Denisse S22002213
 * * 13/12/24
 */

#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int TipoDato;
int CORRECTO = 0;


//! _________ARBOL___________
typedef struct arbolito
{
    char *pregunta;
    TipoDato validez, respuesta;
    struct arbolito *gauche, *droit;
}ARBOLITO;

ARBOLITO *racine = NULL,
         *actuel = NULL;

ARBOLITO *crear_nodo_arbol(char *pregunta, TipoDato respuesta)
{
    ARBOLITO *nodito = malloc(sizeof(ARBOLITO));
    if (!nodito) return NULL;

    nodito->pregunta = pregunta;
    nodito->validez = 0;
    nodito->respuesta = respuesta;
    nodito->gauche = nodito->droit = NULL;
    return nodito;
}

void crearArbolPreguntas()
{
    /**
     * * 1: correcto
     * * 2: incorrecto
     */
    racine = crear_nodo_arbol("¿La penicilina fue descubierta antes de la Segunda Guerra Mundial?", 1);
    racine->droit = crear_nodo_arbol("¿La Segunda Guerra Mundial comenzó en 1935?", 2);
    racine->gauche = crear_nodo_arbol("¿El bombardeo atómico de Hiroshima ocurrio después de la rendición de Alemania?", 1);
    
    racine->droit->droit = crear_nodo_arbol("¿El sol sale siempre por el este?", 1);
    racine->droit->gauche = crear_nodo_arbol("¿La Revolucion Francesa comenzo en 1750?", 2);
    racine->gauche->gauche = crear_nodo_arbol("¿La batalla de Iwo Jima ocurrió en 1949?", 2);
    racine->gauche->droit = crear_nodo_arbol("¿La batalla de Midway fue una victoria clave para las fuerzas japonesas en el Pacífico?", 2);

    racine->droit->droit->droit = crear_nodo_arbol("¿El Tratado de Versalles fue uno de los factores que llevaron a la Segunda Guerra Mundial?", 1);
    racine->gauche->gauche->gauche = crear_nodo_arbol("¿El Holocausto fue llevado a cabo exclusivamente en Alemania?", 2);
    actuel = racine;
}

void dibujarTexto(float x, float y, const char *texto)
{
    glRasterPos2f(x, y);
    for (int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
}

//* esto es para glutTimerFunc ya que necesita una fncion
void exitP(int value)
{
    glutLeaveMainLoop();
}

void keyboard(unsigned char key, int x, int y)
{
    if(key == '1')
    {
        if (actuel->respuesta == 1)
        {
            actuel->validez = 1;
            actuel = actuel->droit;
            CORRECTO++;
        }
        else
        {
            actuel->validez = 0; 
            actuel = actuel->gauche;
            CORRECTO = 0;
        }
    }
    else if(key == '2')
    {
        if(actuel->respuesta == 2)
        {
            actuel->validez = 1;
            actuel = actuel->droit;
            CORRECTO++;
        }
        else
        {
            actuel->validez = 0;
            actuel = actuel->gauche;
            CORRECTO = 0;
        }
    }

    if (CORRECTO >= 3)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(-0.5, 0.0);

        char texto[30];
        snprintf(texto, sizeof(texto), "MUY BIEN!");
        for (char *c = texto; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        glutSwapBuffers();


        //* cerrar la ventana automáticamente
        glutTimerFunc(3000, exitP, 0);
        return;
    }

    glutPostRedisplay();
}

void text()
{
    //* posición del texto
    glRasterPos2f(-0.85, 0.85);
    const char *mensaje = "CONTESTA BIEN 3 PREGUNTAS SEGUIDAS";
    for (const char *c = mensaje; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    //* posición del texto
    glRasterPos2f(0.2, -0.85);
    const char *control = "1 o 2 para las respuestas";
    for (const char *c = control; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void opciones()
{
    char texto[256];
    glRasterPos2f(-0.7, 0.1);
    snprintf(texto, sizeof(texto), "1. Correcto");
    for (char *c = texto; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos2f(-0.7, 0.05);
    snprintf(texto, sizeof(texto), "2. Incorrecto");
    for (char *c = texto; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}


void dibujar_pantalla()
{
    char texto[256];
    glClear(GL_COLOR_BUFFER_BIT);
    text();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-0.8, 0.3);

    snprintf(texto, sizeof(texto), "%s", actuel->pregunta);
    for (char *c = texto; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    opciones();

    //* contador de respuestas
    glRasterPos2f(-0.5, -0.2);
    snprintf(texto, sizeof(texto), "Respuestas correctas consecutivas: %d", CORRECTO);
    for (char *c = texto; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glutSwapBuffers();
}

//! _____Funciones glut
static void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    // glEnable(GL_DEPTH_TEST);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    //! esto es para que los pixeles transparentes no se dibujen y no salga en color negro
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(900,800);
	glutInitWindowPosition(10,50);
	glutCreateWindow("PuzzlePreguntas");



	init();

    //* crear arbol bin 
    crearArbolPreguntas();

    glutDisplayFunc(dibujar_pantalla);
    glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}