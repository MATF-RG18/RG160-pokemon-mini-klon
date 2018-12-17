#include <GL/glut.h>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define TIMER_INTERVAL 1000/60  // 1000ms = 1 sek, 1/60 = 60fps


typedef struct PokemonField{
    int xBegin,xEnd,yBegin,yEnd;
}PokemonField;

typedef struct Player{
    int xPos, yPos, dir, xNewPos, yNewPos;
} Player;

/* Deklaracije callback funkcija. */
void render(void);
void reshape(int,int);
void on_keyboard(unsigned char key, int x, int y);
void SpecialInput(int key, int x, int y);
void Walk(int dir);

void DrawGrass(int,int,int,int);
void DrawPlayer();

int wOrtho = 800, hOrtho = 600;
PokemonField pf;
Player p;
int walking = 0;

int main(int argc, char **argv)
{
    p.xPos = wOrtho/2;
    p.yPos = hOrtho/2-20;
    p.dir = DOWN;
    
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    /* Kreira se prozor. */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pokemon Banana Blue");
    
    /* Registruju se callback funkcije. */
    glutDisplayFunc(render);
    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(SpecialInput);
    
    glutReshapeFunc(reshape);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.3, 0.3, 0.3, 0);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

void render(void)
{
    /* Brise se prethodni sadrzaj 'prozora'. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawGrass(wOrtho - 160, 0, 4, hOrtho/40);
    DrawPlayer();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,wOrtho,0,hOrtho+1);
    glMatrixMode(GL_MODELVIEW);
}

void DrawGrass(int xStart, int yStart, int columns, int rows){
    int i, j;
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0,1,0);
    for(i = 0; i < columns; i++)
    {
        for(j = 0; j < rows; j++)
        {
            glBegin(GL_QUADS);
                glVertex2i(xStart + i*40, yStart + j*40);
                glVertex2i(xStart + 40 + i*40, yStart + j*40);
                glVertex2i(xStart + 40 + i*40, yStart + 40 + j*40);
                glVertex2i(xStart + i*40, yStart + 40 + j*40);
            glEnd();
        }
    }
    
    pf.xBegin = xStart;
    pf.xEnd = xStart + 40*columns;
    pf.yBegin = yStart;
    pf.yEnd = yStart + 40*rows;
}

void DrawPlayer(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1,0,0);
    
    glBegin(GL_POLYGON);
        glVertex2i(p.xPos, p.yPos);
        glVertex2i(p.xPos+40, p.yPos);
        glVertex2i(p.xPos+40, p.yPos+40);
        glVertex2i(p.xPos, p.yPos+40);
    glEnd();
    
}


/* Dupli kod, al mora ako zelim da kretanje moze i na strelice, posto nemaju ASCII vrednost (moze da se napravi 4 fja al to jos vise gomila stvari) */
void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            /* Pokrece se Walk i direkcija je gore. */
            if (!walking) {
                p.yNewPos = p.yPos + 40;
                if(p.yNewPos > 600-40)
                    p.yNewPos = 600-40;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, UP);
            }
            break;
        case GLUT_KEY_DOWN:
            /* Pokrece se Walk i direkcija je dole. */
            if (!walking) {
                p.yNewPos = p.yPos - 40;
                if(p.yNewPos < 0)
                    p.yNewPos = 0;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, DOWN);
            }
            break;
        case GLUT_KEY_LEFT:
                /* Pokrece se Walk i direkcija je levo. */
            if (!walking) {
                p.xNewPos = p.xPos - 40;
                if(p.xNewPos < 0)
                    p.xNewPos = 0;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, LEFT);
            }
            break;
        case GLUT_KEY_RIGHT:
            /* Pokrece se Walk i direkcija je desno. */
            if (!walking) {
                p.xNewPos = p.xPos + 40;
                if(p.xNewPos > 800-40)
                    p.xNewPos = 800-40;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, RIGHT);
            }
            break;
    }
}

void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'w':
        /* Pokrece se Walk i direkcija je gore. */
        if (!walking) {
            p.yNewPos = p.yPos + 40;
            if(p.yNewPos > 600-40)
                p.yNewPos = 600-40;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, UP);
        }
        break;

    case 's':
        /* Pokrece se Walk i direkcija je dole. */
        if (!walking) {
            p.yNewPos = p.yPos - 40;
            if(p.yNewPos < 0)
                p.yNewPos = 0;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, DOWN);
        }
        break;
        
    case 'a':
        /* Pokrece se Walk i direkcija je levo. */
        if (!walking) {
            p.xNewPos = p.xPos - 40;
            if(p.xNewPos < 0)
                p.xNewPos = 0;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, LEFT);
        }
        break;
        
    case 'd':
        /* Pokrece se Walk i direkcija je desno. */
        if (!walking) {
            p.xNewPos = p.xPos + 40;
            if(p.xNewPos > 800-40)
                p.xNewPos = 800-40;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, RIGHT);
        }
        break;
    }
}

void Walk(int dir)
{
    
    switch(dir){
        case UP:
            if(p.yPos == p.yNewPos)
                walking = 0;
            else
                p.yPos += 4;
            break;
        case DOWN:
            if(p.yPos == p.yNewPos)
                walking = 0;
            else
                p.yPos -= 4;
            break;
        case LEFT:
            if(p.xPos == p.xNewPos)
                walking = 0;
            else
                p.xPos -= 4;
            break;
        case RIGHT:
            if(p.xPos == p.xNewPos)
                walking = 0;
            else
                p.xPos += 4;
            break;
        
    }
    
    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
    
    /* Provera da li je igrac stigao do kranje lokacije */
    if (walking) {
        glutTimerFunc(TIMER_INTERVAL, Walk, dir);
    }else{
        
        //TODO: Provera dal je u PokemonField-u, i ako jeste random sansa za encounter sa random pokemonom
        
    }
}




