/**
 * ! Puzzle Matemático!
 * * Lara Xocuis Martha Denisse S22002213
 * * 12/12/24
 */

#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
#define TAMANIO_PILA 8
//! TEXTURA vv
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//* se usó stb_image.h en vez de SOIL por su facilidad de compilación ;)

//? variables para la textura
unsigned int textureid;
// const char *path = "button.png";
float mouseX, mouseY;

enum _bool{FALSE, TRUE, MIDDLE}BOOL;

enum _bool BOOL = MIDDLE;
typedef char TipoDato;

/**
 * *el jugador recibe numeros aleatorios (guardados en una cola)
 * * debe ordenarlos de menor a mayor (guardado en una pila)
 */

//! _________PILA___________
typedef struct nodopila
{
    TipoDato x;
    struct nodopila *suivant;
}PILA;

//* pop
void *pop(PILA **tope)
{
    if(!*tope) return NULL;
    PILA *aux = (*tope);
    (*tope) = (*tope)->suivant;
    free(aux);
}

//*push
void push(PILA **tope, TipoDato x)
{
    PILA *nodito = malloc(sizeof(PILA));
    if(!nodito) return;
    nodito->x = x;
    nodito->suivant=(*tope);
    (*tope) = nodito;
}

PILA *tope = NULL;

//* pila llena
int pilaLlena()
{
    int cont = 0; PILA *actuel = tope;

    while(actuel)
    {
        cont++;
        actuel = actuel->suivant;
    }

    return (cont >= TAMANIO_PILA);
}

//* vaciar pila
void vaciarPila()
{
    PILA *temp;
    while(tope)
    {
        temp = tope;
        tope = tope->suivant;
        free(temp);
    }
}

//! _________COLA___________
typedef struct nodocola
{
    TipoDato x;
    struct nodocola *suivant;
}NODITO_COLA;

typedef struct cola
{
    NODITO_COLA *avant;
    NODITO_COLA *final;
}COLA;

COLA cola_head = {NULL,NULL};

//* crear un nodocola
NODITO_COLA *crear_nodo(TipoDato x)
{
    NODITO_COLA *nodito = malloc(sizeof(NODITO_COLA));

    if(!nodito) return NULL;

    nodito-> x = x;
    nodito-> suivant = NULL;
    return nodito;
} 

//* insertar
void insertarCola(TipoDato x)
{
    NODITO_COLA *nodo_cola = crear_nodo(x);
    //* primer valor
    if(!cola_head.avant)
    {
        cola_head.avant = nodo_cola;
        cola_head.final = nodo_cola;
    }
    else
    {
        cola_head.final->suivant = nodo_cola;
        cola_head.final = nodo_cola;
    }
}

//* vaciar cola
void vaciarCola()
{
    NODITO_COLA *temp;
    while(cola_head.avant)
    {
        temp = cola_head.avant;
        cola_head.avant = cola_head.avant->suivant;
        free(temp);
    }
    cola_head.avant = cola_head.final = NULL;
}

//* eliminar por dato
void eliminarDeCola(TipoDato x)
{
    NODITO_COLA *temp = cola_head.avant,
                *prev = NULL;

    while(temp)
    {
        if(temp->x == x)
        {
            if(!prev) //* primer nodo
            {
                cola_head.avant = temp->suivant;
            }
            else
            {
                prev->suivant = temp->suivant;
            }

            free(temp);
            break;
        }
        
        prev = temp;
        temp = temp->suivant;
    }
}

//! _________ARBOL___________
typedef struct arbolito
{
    char signo;
    TipoDato x;
    struct arbolito *gauche, *droit;
}ARBOLITO;

ARBOLITO *crear_nodo_arbol(char signo, TipoDato x)
{
    ARBOLITO *nodito = malloc(sizeof(ARBOLITO));
    if(!nodito) return NULL;

    nodito->x = x;
    nodito->signo = signo;
    nodito->gauche = nodito ->droit = NULL;
    return nodito;
}

/////////////

ARBOLITO *racine = NULL;

/////////////
void cargarCola()
{
    TipoDato num[TAMANIO_PILA] = {'+','/', '-', '*', '^', '+', '/', '*'};
    for (int i = 0; i < TAMANIO_PILA; i++) insertarCola(num[i]);
}

void cargarArbol()
{
    racine = crear_nodo_arbol('+', 0);
    racine->gauche = crear_nodo_arbol('*', 0);
    racine->droit = crear_nodo_arbol('/', 0);
    racine->gauche->gauche = crear_nodo_arbol(' ', 5);
    racine->gauche->droit = crear_nodo_arbol(' ', 3);
    racine->droit->gauche = crear_nodo_arbol(' ', 9);
    racine->droit->droit = crear_nodo_arbol(' ', 3);
}

//! cargar textura
void cargarTextura(const char *path)
{
    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height,
    &nrChannels, 0);

    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);

    glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels == 4 ? GL_RGBA : GL_RGB), width, height, 0, (nrChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
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

void drawPila()
{
    PILA *actuel = tope;
    float x = -0.8,
          y = -0.8;
    
    while(actuel)
    {
        glColor3f(0.7,0.2,0.5);

        glBegin(GL_QUADS);
            glVertex2f(x, y + 0.05); 
            glVertex2f(x + 0.2, y + 0.05);
            glVertex2f(x + 0.2, y - 0.1);
            glVertex2f(x, y - 0.1);
        glEnd();

        //* dibujar valores de la pila
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(x + 0.085, y - 0.036);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, actuel->x);

        actuel = actuel->suivant;
        //* dseplaza un poco para el otro nodo, es para el espacio entre noditos
        y += 0.2;
    }
}

void drawCola()
{
    NODITO_COLA *actuel = cola_head.avant;
    //* posicion en la pantalla
    float x = -0.98,
          y = 0.7;

    while(actuel)
    {
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textureid);
        glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glTexCoord2f(0,1); glVertex2f(x, y + 0.1); 
            glTexCoord2f(1,1); glVertex2f(x + 0.2, y + 0.1);
            glTexCoord2f(1,0); glVertex2f(x + 0.2, y - 0.1);
            glTexCoord2f(0,0); glVertex2f(x, y - 0.1);
        glEnd();

        //! desabilitando textura
        glDisable(GL_TEXTURE_2D);

        //* dibujar valores de la cola
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(x + 0.085, y - 0.02);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, actuel->x);

        actuel = actuel->suivant;
        //* dseplaza un poco para el otro nodo, es para el espacio entre noditos
        x += 0.25;
    }
}

//* esto es para glutTimerFunc ya que necesita una fncion
void exitP(int value)
{
    glutLeaveMainLoop();
}

void drawWINorFAIL(int bool)
{
    if(bool)
    {
        //* posición del texto
        glRasterPos2f(0,0);
        const char *mensaje = "MUY BIEN!";
        for (const char *c = mensaje; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        //* cerrar la ventana automáticamente
            glutTimerFunc(3000, exitP, 0);

        glutPostRedisplay();
    }
    else 
    {
        //* posición del texto
        glRasterPos2f(0,0);
        const char *mensaje = "HAZLO DE NUEVO!";
        for (const char *c = mensaje; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
         glutPostRedisplay();
    }
}

//* asignarle valor a los signos para saber si está ordenado
int VALOR(char signo)
{
    switch (signo)
    {
        case '^': return 3;
        case '/': case '*': return 2;
        case '+': case '-': return 1;
    }
}

int verificarRespuesta()
{
    if(!pilaLlena()) return 0;
    else
    {
        PILA *temp = tope;
        if(!temp) return 1;

        while (temp && temp->suivant)
        {
            int actuel = VALOR(temp->x),
                suivant = VALOR(temp->suivant->x);


            //* checar si no está en orden ascendente
            if(actuel < suivant) return 0;

            temp = temp->suivant;
        }

        return 1; //* si está en orden ascendente
    }
}

void seleccionMouse(float x, float y)
{
    float ancho = 0.2, alto = 0.1, margen = 0.04; int i=0;

    //* para ver si el click está adentro del boton dibujado
    NODITO_COLA *actuel = cola_head.avant;
    float x_inicio = -0.99,
          y_final = 0.75f;

    while(actuel)
    {
        float COLA_X = x_inicio + i * (ancho + margen),
              COLA_Y = y_final;
        
        if (x >= COLA_X && x <= COLA_X + ancho && y >= COLA_Y - alto && y <= COLA_Y)
        {
            //* extraer el num y ponerlo en la pila
            int num = actuel->x;
            push(&tope, num);
            eliminarDeCola(num);
            //* dibujar pila
            //* dibujar pila
            drawPila();
            // * volver a dibujar cola porque se eliminó
            drawCola();
            glutPostRedisplay();

            break;
        }
        actuel = actuel->suivant; i++;
    }


}

//* manejar la entrada por mouse
void mouse(int button, int estado, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && estado == GLUT_DOWN)
    {
        //? convertir coordenadas de mouse a coordenadas de opengl
        mouseX = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0 - 1.0;
        mouseY = 1.0 - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0;
        
        
        if (mouseX >= -1.0 && mouseX <= 1.0 && mouseY >= -1.0 && mouseY <= 1.0)
            seleccionMouse(mouseX, mouseY);

    }
}

void drawArbol(float x, float y, float dx)
{
    if(!racine) return;
    char buff[10];

    sprintf(buff, "%c", racine->signo);
    glRasterPos2f(x, y);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buff[0]);
    if (racine->gauche) {
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x - dx, y - 0.3);
        glEnd();
        drawArbol(x - dx, y - 0.3, dx / 2);
    }
    if (racine->droit) {
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + dx, y - 0.3);
        glEnd();
        drawArbol(x + dx, y - 0.3, dx / 2);
    }
}

void text()
{
    //* posición del texto
    glRasterPos2f(-0.85, 0.85);
    const char *mensaje = "ORDENA LOS SIGUIENTES SIGNOS DE MENOR A MAYOR!";
    for (const char *c = mensaje; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    //* posición del texto
    glRasterPos2f(0.2, -0.85);
    const char *control = "[ESPACIO] cuando tengas el orden!";
    for (const char *c = control; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void reiniciarJuego()
{
    vaciarPila();
    vaciarCola();
    cargarCola(&cola_head);
    drawCola();
    drawPila();
    glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
    if(key == 32)
    {
        //* si está en orden ascendente, GANASTEEE
            if(verificarRespuesta())
            {
                BOOL = TRUE;
                glutPostRedisplay();
            }
            else //* está mal >:C 
            {
                BOOL = FALSE;
                reiniciarJuego();
            }
    }
}

static void draw()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
    glLoadIdentity();
    text();
    
    glColor3f(1.0,1.0,1.0);
    //* dibujar pila
    glPushMatrix(); 
         drawCola();
         drawPila();
    glPopMatrix();

    // si existe entrada de que ganó, poner texto
    if(BOOL == TRUE) drawWINorFAIL(1);
    else if(!BOOL) drawWINorFAIL(0);
    


    glutSwapBuffers();
}


int main(int argc, char **argv)
{
    const char *texture_path = argv[1];

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800,800);
	glutInitWindowPosition(10,50);
	glutCreateWindow("PuzzleMatematico");

    //* numeros desordenados
    cargarCola(&cola_head);
    //* crear arbol bin 
    cargarArbol(racine);

	init();
    cargarTextura(texture_path);


    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}