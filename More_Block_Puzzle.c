/**
 * !Inspirado en el puzzle de Undertale
 * * estos aparecen cuando vamos a Hotland :)
 * * Lara Xocuis Martha Denisse S22002213
 * * 08/12/24
 * https://www.youtube.com/watch?v=v1uG7rDNups
 */

#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
#define CUADROS_No 15
//* para redondear a dos decimales porque tuve problemas
//* con la comparación de posiciones TvT
#define ROUND(u) (round((u) * 100.0) / 100.0)

//! TEXTURA vv
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//* se usó stb_image.h en vez de SOIL por su facilidad de compilación ;)

//? es movible? TRUE, FALSE
enum _bool{FALSE, TRUE};
//? variables para la textura
unsigned int textureid;
// const char *path = "GAME1.png";
int move = 0, exito = 2;

//! estructura para arreglo de estructuras, es para cada cubito 
typedef struct 
{
    float x, y, size;
    enum _bool bbol;
}CUADRO;

CUADRO cuadros[CUADROS_No];

//! Cola para verificar la entrada del teclado del jugador
typedef struct nodocola
{
    char letra;
    struct nodocola *next;
}NODITO_COLA;

typedef struct cola
{
    NODITO_COLA *frente;
    NODITO_COLA *final;
}COLA;

//* crear un nodocola
NODITO_COLA *crear_nodo(char w)
{
    NODITO_COLA *nodito = malloc(sizeof(NODITO_COLA));

    if(!nodito) return NULL;

    nodito-> letra = w;
    nodito-> next = NULL;
    return nodito;
} 

//* insertar
void insertarCola(COLA *head, char letra)
{
    NODITO_COLA *nodo_cola = crear_nodo(letra);
    //* primer valor
    if(!head->frente)
    {
        head->frente = nodo_cola;
        head->final = nodo_cola;
    }
    else
    {
        head->final->next= nodo_cola;
        head->final = nodo_cola;
    }
}

//* vaciar cola
void vaciarCola(COLA *head)
{
    NODITO_COLA *temp;
    while(head->frente)
    {
        temp = head->frente;
        head->frente = head->frente->next;
        free(temp);
    }
    head->frente = head->final = NULL;
}

//! PILA para manejar el orden de la resolución del puzzle
//? resolución: w a s a xD
typedef struct nodopila
{
    char letra;
    struct nodopila *next;
}PILA;

//* pop
PILA *pop(PILA **tope)
{
    if(!*tope) return NULL;
    PILA *aux = (*tope);
    (*tope) = (*tope)->next;
    return aux;
}

//*push
void push(char letra, PILA **tope)
{
    PILA *nodito = malloc(sizeof(PILA));
    if(!nodito) return;
    nodito->letra = letra;
    nodito->next=(*tope);
    (*tope) = nodito;
}

void cargarResolucion(PILA **tope)
{
    push('a', &(*tope));
    push('s', &(*tope));
    push('a', &(*tope));
    push('w', &(*tope));
}

static void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluPerspective(60.0,1.0,4.0,100.0);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
}

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

void lineas()
{
    //! lineas 
        glBegin(GL_LINE_STRIP);
            glVertex3f(0.9, 0.9, 0.0);
            glVertex3f(-0.9, 0.9, 0.0);
            glVertex3f(-0.9, -0.9, 0.0);
            glVertex3f(0.9, -0.9, 0.0);
            glVertex3f(0.9, 0.9, 0.0);
        glEnd();

        glBegin(GL_LINE_STRIP);
            glVertex3f(0.6, 0.6, 0.0);
            glVertex3f(-0.6, 0.6, 0.0);
            glVertex3f(-0.6, -0.6, 0.0);
            glVertex3f(0.6, -0.6, 0.0);
            glVertex3f(0.6, 0.6, 0.0);
        glEnd();

        //* triangulo blanco arriba
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0, 0.65, 0.0);
            glVertex3f(-0.1, 0.8, 0.0);
            glVertex3f(0.1, 0.8, 0.0);
        glEnd();

        //* triangulo amarillo abajo
        glColor3f(1.0,1.0,0.0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0, -0.65, 0.0);
            glVertex3f(-0.1, -0.8, 0.0);
            glVertex3f(0.1, -0.8, 0.0);
        glEnd();
}

void drawCuadros()
{
    glColor3f(1.0,1.0,1.0);
    for(int i=0; i <= CUADROS_No; i++)
    {
        glPushMatrix();
            glTranslatef(cuadros[i].x,cuadros[i].y,0);

            //* si es TRUE, es movible
            //* entonces, dibujar lineas y dibujar cuadrito con textura
            if(cuadros[i].bbol)
            {
                glLineWidth(7);
                glColor3f(0,1,1);
                glBegin(GL_LINE_STRIP);
                    glVertex3f(cuadros[i].size, cuadros[i].size, 0.0);
                    glVertex3f(-cuadros[i].size, cuadros[i].size, 0.0);
                    glVertex3f(-cuadros[i].size, -cuadros[i].size, 0.0);
                    glVertex3f(cuadros[i].size, -cuadros[i].size, 0.0);
                    glVertex3f(cuadros[i].size, cuadros[i].size, 0.0);
                glEnd();

                //!aqui cargando textura y poniendo color en blanco
                glColor3f(1, 1, 1);
                glBindTexture(GL_TEXTURE_2D, textureid);
                glEnable(GL_TEXTURE_2D);

                glBegin(GL_POLYGON);
                    glBegin(GL_POLYGON);
                    //? texcoord2f es para las coordenadas de la textura
                        glTexCoord2f(1,1); glVertex3f(cuadros[i].size, cuadros[i].size, 0.0);
                        glTexCoord2f(0,1); glVertex3f(-cuadros[i].size, cuadros[i].size, 0.0);
                        glTexCoord2f(0,0); glVertex3f(-cuadros[i].size, -cuadros[i].size, 0.0); 
                        glTexCoord2f(1,0); glVertex3f(cuadros[i].size, -cuadros[i].size, 0.0); 
                    glEnd();
                glEnd();

                //! desabilitando textura
                glDisable(GL_TEXTURE_2D);
            }
            else
            {
                glBegin(GL_POLYGON);
                    glVertex3f(cuadros[i].size, cuadros[i].size, 0.0);
                    glVertex3f(-cuadros[i].size, cuadros[i].size, 0.0);
                    glVertex3f(-cuadros[i].size, -cuadros[i].size, 0.0);
                    glVertex3f(cuadros[i].size, -cuadros[i].size, 0.0);
                glEnd();
            }

        glPopMatrix();
    }
}

//* aqui es para inicializar mis cuadros
void inicializarC()
{
    //! cuadros static
    cuadros[0] = (CUADRO){-0.48, 0.48, 0.12, FALSE};
    cuadros[1] = (CUADRO){-0.24, 0.48, 0.12, FALSE};
    cuadros[2] = (CUADRO){-0.48, -0.48, 0.12, FALSE};
    cuadros[3] = (CUADRO){-0.24,-0.48, 0.12, FALSE};
    cuadros[4] = (CUADRO){-0.24,0, 0.12, FALSE};
    cuadros[5] = (CUADRO){0.24,0.24, 0.12, FALSE};
    cuadros[6] = (CUADRO){0.48,0.48, 0.12, FALSE};
    cuadros[7] = (CUADRO){0.48,-0.48, 0.12, FALSE};
    cuadros[8] = (CUADRO){0.48,-0.24, 0.12, FALSE};
    cuadros[9] = (CUADRO){0.24,-0.48, 0.12, FALSE};
    
    //! cuadros no-static xD
    cuadros[10] = (CUADRO){-0.24,0.24, 0.12, TRUE};
    cuadros[11] = (CUADRO){-0.24,-0.24, 0.12, TRUE};
    cuadros[12] = (CUADRO){0.0,0.0, 0.12, TRUE};
    cuadros[13] = (CUADRO){0.24,-0.24, 0.12, TRUE};
    cuadros[14] = (CUADRO){0.48,0, 0.12, TRUE};
    cuadros[15] = (CUADRO){0.24,0.48, 0.12, TRUE};



}

//* texto en pantalla
void texto()
{
    //* posición del texto
    glRasterPos2f(-0.85, 0.85);
    const char *mensaje = "DISPARA AL OTRO LADO SIN FALLAR CON 4 MOVIMIENTOS";
    for (const char *c = mensaje; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glRasterPos2f(-0.85, 0.7);
    const char *controles = "Controles: AWSD";
    for (const char *c = controles; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glRasterPos2f(-0.85, -0.65);
    const char *control2 = "[ESPACIO] CUANDO TENGAS 4 MOVIMIENTOS";
    for (const char *c = control2; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    //* contador de movimientos
    glRasterPos2f(-0.85, -0.7);
    char movimientosTexto[50];
    snprintf(movimientosTexto, sizeof(movimientosTexto), "Movimientos: %d", move);
    for (const char *c = movimientosTexto; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

//* esto es para glutTimerFunc ya que necesita una fncion
void exitP(int value)
{
    glutLeaveMainLoop();
}

//* vaciar toda la pila y volver a llenar
void rellenarPila(PILA **tope)
{
    while ((*tope))
    {
        pop(&*tope);
    }
    cargarResolucion(&*tope); 
}

static void draw()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
    glLoadIdentity();
    texto();

    glPushMatrix();  
        lineas();
        glPushMatrix();
            drawCuadros();
        glPopMatrix();
    glPopMatrix();

    if(exito == 1)
    {   //* si hay exito, mostrar texto de victoria
        glRasterPos2f(0.2, -0.7);
        const char *FINAL = "¡PUZZLE COMPLETADO!";
        for (const char *c = FINAL; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        glRasterPos2f(0.2, -0.75);
        const char *final2 = "Saliendo...";
        for (const char *c = final2; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    else if(!exito)
    {   //* falló :(
        glRasterPos2f(-0.85, -0.75);
        const char *FAIL = "FALLASTE! :(";
        for (const char *c = FAIL; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    //* las habia puesto como variables globales pero tuve problemas y guardaba basura :(
    static COLA head = {NULL, NULL}; 
    static PILA *tope = NULL;

    //* cargar la resolución [PILA] si es null desde el principio
    if(!tope) cargarResolucion(&tope);
    move++; //* aumentar cantidad de movimientos para imprimir
    //? si se pasan los movimientos, checar si todos los movimientos coinciden con la resolución de la PILA
    if(move>4)
    {
        while(head.frente)
        {
            //* hacer pop en la pila si es el orden correcto
            if(tope->letra == head.frente->letra)
            {
                pop(&tope);
                //* avanzar
                NODITO_COLA *temp = head.frente;
                head.frente = head.frente->next;
                free(temp);
                continue;
            }
            head.frente = head.frente->next;
            
        }
        //* si se eliminó todo, es porque completó el puzzle
        if(!tope)
        {
            exito = 1;
            move = 4;
            glutPostRedisplay(); 
            //* cerrar la ventana automáticamente
            glutTimerFunc(3000, exitP, 0);
            return;
        } //* sino, el orden está mal y es necesario reiniciar
        else
        {
            move = 0;   
            exito = 0;             
            vaciarCola(&head);       
            rellenarPila(&tope);
            inicializarC();          
            drawCuadros();           
            glutPostRedisplay(); 
        }

        
    }
    //? sino,ir encolando la entrada del usuario
    else insertarCola(&head, key);

    //? lógica de movimiento dependiendo la letra
    switch (key)
    {
        case 'd':
        //! lógica por colision en x
            for(int i=0; i <= CUADROS_No; i++)
            {
                //* se mueve?
                if(cuadros[i].bbol)
                {
                    int move=1;
                    //? verificar si hay una colisión con cuadros estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        //* es cuadro estático?
                        if(!cuadros[j].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].x + 0.24) == ROUND(cuadros[j].x) && 
                               ROUND(cuadros[i].y) == ROUND(cuadros[j].y))
                            {
                                move=0;
                                break;
                            }
                        }
                    }

                    //? verificar si hay una colisión con cuadros NO estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        if(cuadros[i].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].x + 0.24) == ROUND(cuadros[j].x) && 
                               ROUND(cuadros[i].y) == ROUND(cuadros[j].y))
                            {
                                move=0;
                                break;
                            }
                        }
                    }

                    //? verificar si pasa del margen de lineas
                    if(ROUND(cuadros[i].x + 0.24) == 0.72) move = 0;
                    if(move) cuadros[i].x += 0.24;
                }
            }
            break;
        case 'a':
        //! lógica por colision en x
            for(int i=0; i <= CUADROS_No; i++)
            {
                //* se mueve?
                if(cuadros[i].bbol)
                {
                    int move=1;
                    //? verificar si hay una colisión con cuadros estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        //* es cuadro estático?
                        if(!cuadros[j].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].x - 0.24) == ROUND(cuadros[j].x) && 
                               ROUND(cuadros[i].y) == ROUND(cuadros[j].y))
                            {
                                move=0;
                                break;
                            }
                        }
                    }
                    //? verificar si hay una colisión con cuadros NO estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        if(cuadros[i].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].x - 0.24) == ROUND(cuadros[j].x) && 
                               ROUND(cuadros[i].y) == ROUND(cuadros[j].y))
                            {
                                move=0;
                                break;
                            }
                        }
                    }

                    //? verificar si pasa del margen de lineas
                    if(ROUND(cuadros[i].x - 0.24) == -0.72) move = 0;
                    if(move) cuadros[i].x -= 0.24;
                }
            }
            break;
        case 'w':
        //! lógica por colision en y
            for(int i=0; i <= CUADROS_No; i++)
            {
                //* se mueve?
                if(cuadros[i].bbol)
                {
                    int move=1;
                    //? verificar si hay una colisión con cuadros estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        //* es cuadro estático?
                        if(!cuadros[j].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].y + 0.24) == ROUND(cuadros[j].y) && 
                               ROUND(cuadros[i].x) == ROUND(cuadros[j].x))
                            {
                                move=0;
                                break;
                            }
                        }
                    }

                    //? verificar si hay una colisión con cuadros NO estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        if(cuadros[i].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].y + 0.24) == ROUND(cuadros[j].y) && 
                               ROUND(cuadros[i].x) == ROUND(cuadros[j].x))
                            {
                                move=0;
                                break;
                            }
                        }
                    }


                    //? verificar si pasa del margen de lineas
                    if(ROUND(cuadros[i].y + 0.24) == 0.72) move = 0;
                    if(move) cuadros[i].y += 0.24;
                }
            }
            break;
        case 's':
        //! lógica por colision en y
            for(int i=0; i <= CUADROS_No; i++)
            {
                //* se mueve?
                if(cuadros[i].bbol)
                {
                    int move=1;
                    //? verificar si hay una colisión con cuadros estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        //* es cuadro estático?
                        if(!cuadros[j].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].y - 0.24) == ROUND(cuadros[j].y) && 
                               ROUND(cuadros[i].x) == ROUND(cuadros[j].x))
                            {
                                move=0;
                                break;
                            }
                        }
                    }
                    
                    //? verificar si hay una colisión con cuadros NO estáticos
                    for(int j=0; j <= CUADROS_No; j++)
                    {
                        if(cuadros[i].bbol)
                        {
                            //* hay colisión?
                            if(ROUND(cuadros[i].y - 0.24) == ROUND(cuadros[j].y) && 
                               ROUND(cuadros[i].x) == ROUND(cuadros[j].x))
                            {
                                move=0;
                                break;
                            }
                        }
                    }

                    //? verificar si pasa del margen de lineas
                    if(ROUND(cuadros[i].y - 0.24) == -0.72) move = 0;
                    if(move) cuadros[i].y -= 0.24;
                }
            }
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    const char *texture_path = argv[1];


    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800,800);
	glutInitWindowPosition(10,50);
	glutCreateWindow("PuzzleHotland");

	init();
    inicializarC();
    cargarTextura(texture_path);

    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}