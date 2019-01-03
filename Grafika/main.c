#include <GL/glut.h>
#include "pokemon.h"
#include <stdio.h>
#include <time.h>
#include "image.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define false 0
#define true 1

#define POKEMON_MENU 1
#define INFO_MENU 2

#define TIMER_INTERVAL 1000/60  // 1000ms = 1 sek, 1/60 = 60fps


typedef struct PokemonField{
    int xBegin,xEnd,yBegin,yEnd;
}PokemonField;

typedef struct Player{
    int xPos, yPos, dir, xNewPos, yNewPos;
    Pokemon playerPokemon;
    Pokemon reservePokemon[5];
} Player;

/* Deklaracije callback funkcija. */
void render(void);
void reshape(int,int);
void on_keyboard(unsigned char key, int x, int y);
void SpecialInput(int key, int x, int y);
void Walk(int dir);
void InitializeSprites();
void InitializePlayer();

void EngageEncounter();
void DrawMenu();
void DrawPokemonMenu();
void DrawEnemyPokemon(int,int);
void DrawPlayerPokemon(int,int);
void DrawGrass(int,int,int,int);
void DrawPlayer();
void Pause();
void Unpause();
void Tackle();
void ThrowPokeBall();
void Respawn();

static GLuint sprites[4];
int wOrtho = 800, hOrtho = 600, arrowPos = 0, pokemonMenuArrowPos = 0;
PokemonField pf;
Player player;
Pokemon enemyPokemon;
int walking = false, battling = false, paused = false;
int menu = 0;

int main(int argc, char **argv)
{
    /* Random seed generalizujemo */
    srand(time(NULL));
    
    InitializePlayer();
    
    /* Testiranje */
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*PrintPokemon(player.playerPokemon);
    
    Pokemon x = GetRandomPokemon(player.playerPokemon.level);
    PrintPokemon(x);
    
    LevelUp(&player.playerPokemon);
    PrintPokemon(player.playerPokemon);*/
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
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
    
    InitializeSprites();

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.3, 0.3, 0.3, 0);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

void InitializeSprites()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    image_read(image, "character1.bmp");

    /* Generisu se identifikatori tekstura. */
    glGenTextures(4, sprites);

    glBindTexture(GL_TEXTURE_2D, sprites[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se druga tekstura. */
    image_read(image, "grass.bmp");

    glBindTexture(GL_TEXTURE_2D, sprites[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Kreira se treca tekstura. */
    image_read(image, "Resources/menu.bmp");
    
    glBindTexture(GL_TEXTURE_2D, sprites[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Kreira se cetvrta tekstura. */
    image_read(image, "Resources/arrow.bmp");
    
    glBindTexture(GL_TEXTURE_2D, sprites[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Kreira se peta tekstura. */
    image_read(image, "PokemonSprites/Pikachu.bmp");
    
    glGenTextures(1, &player.playerPokemon.sprite);
    
    glBindTexture(GL_TEXTURE_2D, player.playerPokemon.sprite);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

void InitializePlayer(){
    
    player.xPos = wOrtho/2;
    player.yPos = hOrtho/2-20;
    player.dir = DOWN;
    player.playerPokemon = MakeStarterPokemon();
    for(int i = 0; i < 5; i++)
    {
        player.reservePokemon[i].name = "NULL";
    }
}

void render(void)
{
    /* Brise se prethodni sadrzaj 'prozora'. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(battling == false){
        DrawGrass(wOrtho - 160, 0, 4, hOrtho/40);
        DrawPlayer();
    }else{
        DrawEnemyPokemon(600,400);
        DrawPlayerPokemon(200,100);
    }
    if(paused == true){
        DrawMenu();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
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

void DrawMenu(){
    glBindTexture(GL_TEXTURE_2D, sprites[2]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex2i(wOrtho-160, 0);

        glTexCoord2f(1, 0);
        glVertex2i(wOrtho, 0);

        glTexCoord2f(1, 1);
        glVertex2i(wOrtho, hOrtho);

        glTexCoord2f(0, 1);
        glVertex2i(wOrtho-160, hOrtho);
    glEnd();
    
    
    int xArrow = 660, yArrow;
    switch(arrowPos){
        case 0:
            yArrow = 540;
            break;
        case 1:
            yArrow = 500;
            break;
        case 2:
            yArrow = 80;
    }
    glBindTexture(GL_TEXTURE_2D, sprites[3]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex2i(xArrow, yArrow);

        glTexCoord2f(1, 0);
        glVertex2i(xArrow + 20, yArrow);

        glTexCoord2f(1, 1);
        glVertex2i(xArrow + 20, yArrow + 20);

        glTexCoord2f(0, 1);
        glVertex2i(xArrow, yArrow + 20);
    glEnd();
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    switch(menu){
        case 0:
            break;
        case POKEMON_MENU:
            DrawPokemonMenu();
            break;
    }
}

void DrawPokemonMenu(){
    glColor3f(1,1,1);
    
    glBegin(GL_QUADS);
        glVertex2i(0,0);
        glVertex2i(wOrtho,0);
        glVertex2i(wOrtho,hOrtho);
        glVertex2i(0,hOrtho);
    glEnd();
    
    /* Nacrtaj pokemona koj je trenutno u upotrebi */
    glBindTexture(GL_TEXTURE_2D, player.playerPokemon.sprite);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex2i(80, 240);

        glTexCoord2f(1, 0);
        glVertex2i(80 + 120, 240);

        glTexCoord2f(1, 1);
        glVertex2i(80 + 120, 240 + 120);

        glTexCoord2f(0, 1);
        glVertex2i(80, 240 + 120);
    glEnd();
    
    /* Iscrtaj ostale koje su u rezervi */
    for(int i = 0; i < 5; i++){
        if(strcmp(player.reservePokemon[i].name,"NULL") == 0)
            break;
        glBindTexture(GL_TEXTURE_2D, player.reservePokemon[i].sprite);
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex2i(300, 500 - i*100);

            glTexCoord2f(1, 0);
            glVertex2i(300 + 80, 500 - i*100);

            glTexCoord2f(1, 1);
            glVertex2i(300 + 80, 500 - i*100 + 80);

            glTexCoord2f(0, 1);
            glVertex2i(300, 500 - i*100 + 80);
        glEnd();
    }
    
    if(strcmp(player.reservePokemon[0].name,"NULL") == 0){
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, sprites[3]);
    glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex2i(240, 520 - pokemonMenuArrowPos * 100);

            glTexCoord2f(1, 0);
            glVertex2i(240 + 40, 520 - pokemonMenuArrowPos * 100);

            glTexCoord2f(1, 1);
            glVertex2i(240 + 40, 520 - pokemonMenuArrowPos * 100 + 40);

            glTexCoord2f(0, 1);
            glVertex2i(240, 520 - pokemonMenuArrowPos * 100 + 40);
        glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawEnemyPokemon(int xPos, int yPos){
    
    float step = 120/(float)enemyPokemon.healthMax;
    
    /* Health bar */
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(xPos+120, yPos+160);
        glVertex2f(xPos+120, yPos+180);
        glVertex2f(xPos+120 - enemyPokemon.health * step, yPos+180);
        glVertex2f(xPos+120 - enemyPokemon.health * step, yPos+160);
    glEnd();
    
    printf("%f %f", enemyPokemon.health * step, step);
    
    /* Pokemon */
    glBindTexture(GL_TEXTURE_2D, enemyPokemon.sprite);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex2i(xPos, yPos);

        glTexCoord2f(1, 0);
        glVertex2i(xPos+120, yPos);

        glTexCoord2f(1, 1);
        glVertex2i(xPos + 120, yPos + 120);

        glTexCoord2f(0, 1);
        glVertex2i(xPos, yPos + 120);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void DrawPlayerPokemon(int xPos, int yPos){
    
    float step = 120/(float)player.playerPokemon.healthMax;
    
    /* Health bar */
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(xPos, yPos - 40);
        glVertex2f(xPos, yPos - 60);
        glVertex2f(xPos + player.playerPokemon.health * step, yPos - 60);
        glVertex2f(xPos + player.playerPokemon.health * step, yPos - 40);
    glEnd();
    
    /* Pokemon */
    glBindTexture(GL_TEXTURE_2D, player.playerPokemon.sprite);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex2i(xPos, yPos);

        glTexCoord2f(1, 0);
        glVertex2i(xPos+120, yPos);

        glTexCoord2f(1, 1);
        glVertex2i(xPos + 120, yPos + 120);

        glTexCoord2f(0, 1);
        glVertex2i(xPos, yPos + 120);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void DrawGrass(int xStart, int yStart, int columns, int rows){
    int i, j;
    
    //glColor3f(0,1,0);
    
    
    // Moze i sa GL_REPEAT u texturi pa samo ivice da se odrede i kolko puta da se ponovi (visina/40, sirina/40)
    glBindTexture(GL_TEXTURE_2D, sprites[1]);
    for(i = 0; i < columns; i++)
    {
        for(j = 0; j < rows; j++)
        {
            glBegin(GL_QUADS);
                glNormal3f(0, 0, 1);
                
                glTexCoord2f(0, 0);
                glVertex2i(xStart + i*40, yStart + j*40);
                
                glTexCoord2f(1, 0);
                glVertex2i(xStart + 40 + i*40, yStart + j*40);
                
                glTexCoord2f(1, 1);
                glVertex2i(xStart + 40 + i*40, yStart + 40 + j*40);
                
                glTexCoord2f(0, 1);
                glVertex2i(xStart + i*40, yStart + 40 + j*40);
            glEnd();
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    pf.xBegin = xStart;
    pf.xEnd = xStart + 40*columns;
    pf.yBegin = yStart;
    pf.yEnd = yStart + 40*rows;
}

void DrawPlayer(){
    
    //glPolygonMode(GL_FRONT, GL_FILL);
    //glColor3f(1,1,1);

    /*
    glBegin(GL_POLYGON);
        glVertex2i(player.xPos, player.yPos);
        glVertex2i(player.xPos+40, player.yPos);
        glVertex2i(player.xPos+40, player.yPos+40);
        glVertex2i(player.xPos, player.yPos+40);
    glEnd();
    */
    
    glBindTexture(GL_TEXTURE_2D, sprites[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex2i(player.xPos, player.yPos);

        glTexCoord2f(1, 0);
        glVertex2i(player.xPos+40, player.yPos);

        glTexCoord2f(1, 1);
        glVertex2i(player.xPos+40, player.yPos+40);

        glTexCoord2f(0, 1);
        glVertex2i(player.xPos, player.yPos+40);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

int InPokemonField(Player p){
    if( (p.xPos >= pf.xBegin && p.xPos < pf.xEnd) && (p.yPos >= pf.yBegin && p.yPos < pf.yEnd ) )
        return true;
    else
        return false;
}

/* Dupli kod, al mora ako zelim da kretanje moze i na strelice, posto nemaju ASCII vrednost (moze da se napravi 4 fja al to jos vise gomila stvari) */
void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            /* Pomeranje strelice u glavnom meniju. */
            if (paused == true && menu == 0){
                if(arrowPos > 0){
                    arrowPos--;
                    glutPostRedisplay();
                }
            }
            /* Pomeranje strelice u pokemon meniju. */
            else if(paused == true && menu == POKEMON_MENU){
                if(pokemonMenuArrowPos > 0){
                    pokemonMenuArrowPos--;
                    glutPostRedisplay();
                }
            }
            /* Pokrece se Walk i direkcija je gore. */
            else if (!walking && !battling) {
                player.yNewPos = player.yPos + 40;
                if(player.yNewPos > 600-40)
                    player.yNewPos = 600-40;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, UP);
            }
            break;
        case GLUT_KEY_DOWN:
            /* Pomeranje strelice u glavnom meniju. */
            if (paused == true && menu == 0){
                if(arrowPos < 2){
                    arrowPos++;
                    glutPostRedisplay();
                }
            }
            /* Pomeranje strelice u pokemon meniju. */
            else if(paused == true && menu == POKEMON_MENU){
                if(pokemonMenuArrowPos < PokemonInReserve(player.reservePokemon)-1){
                    pokemonMenuArrowPos++;
                    glutPostRedisplay();
                }
            }
            /* Pokrece se Walk i direkcija je dole. */
            else if (!walking && !battling) {
                player.yNewPos = player.yPos - 40;
                if(player.yNewPos < 0)
                    player.yNewPos = 0;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, DOWN);
            }
            break;
        case GLUT_KEY_LEFT:
                /* Pokrece se Walk i direkcija je levo. */
            if (!walking && !battling) {
                player.xNewPos = player.xPos - 40;
                if(player.xNewPos < 0)
                    player.xNewPos = 0;
                walking = 1;
                glutTimerFunc(TIMER_INTERVAL, Walk, LEFT);
            }
            break;
        case GLUT_KEY_RIGHT:
            /* Pokrece se Walk i direkcija je desno. */
            if (!walking && !battling) {
                player.xNewPos = player.xPos + 40;
                if(player.xNewPos > 800-40)
                    player.xNewPos = 800-40;
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
        /* Pomeranje strelice u glavnom meniju. */
        if (paused == true && menu == 0){
            if(arrowPos > 0){
                arrowPos--;
                glutPostRedisplay();
            }
            
        }
        /* Pomeranje strelice u pokemon meniju. */
        else if(paused == true && menu == POKEMON_MENU){
            if(pokemonMenuArrowPos > 0){
                pokemonMenuArrowPos--;
                glutPostRedisplay();
            }
        }
        /* Pokrece se Walk i direkcija je gore. */
        else if (!walking && !battling) {
            if(player.yPos == 600-40)
                break;
            player.yNewPos = player.yPos + 40;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, UP);
        }
        break;

    case 's':
        /* Pomeranje strelice u glavnom meniju. */
        if (paused == true && menu == 0){
            if(arrowPos < 2){
                arrowPos++;
                glutPostRedisplay();
            }
        }
        /* Pomeranje strelice u pokemon meniju. */
        else if(paused == true && menu == POKEMON_MENU){
            if(pokemonMenuArrowPos < PokemonInReserve(player.reservePokemon)-1){
                pokemonMenuArrowPos++;
                glutPostRedisplay();
            }
        }
        /* Pokrece se Walk i direkcija je dole. */
        else if (!walking && !battling) {
            if(player.yPos == 0)
                break;
            player.yNewPos = player.yPos - 40;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, DOWN);
        }
        break;
        
    case 'a':
        /* Pokrece se Walk i direkcija je levo. */
        if (!walking && !battling) {
            if(player.xPos == 0)
                break;
            player.xNewPos = player.xPos - 40;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, LEFT);
        }
        break;
        
    case 'd':
        /* Pokrece se Walk i direkcija je desno. */
        if (!walking && !battling) {
            if(player.xPos == 800-40)
                break;
            player.xNewPos = player.xPos + 40;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, RIGHT);
        }
        break;
        
    case 32:
        if(battling)
            Tackle();
        else if(paused && menu == 0){
            switch(arrowPos){
                case 0:
                    pokemonMenuArrowPos = 0;
                    menu = POKEMON_MENU;
                    break;
                case 1:
                    menu = INFO_MENU;
                    break;
                case 2:
                /* Zavrsava se program. */
                    exit(0);
                    break;
            }
            glutPostRedisplay();
        }
        /* Zameni pokemone */
        else if(paused && menu == POKEMON_MENU){
            SwapPokemon(&player.reservePokemon[pokemonMenuArrowPos], &player.playerPokemon);
            
            glutPostRedisplay();
        }
        break;
        
    case 'c':
        if(battling)
            ThrowPokeBall();
        break;
    
    case 13:
        if(menu != 0){
            menu = 0;
            glutPostRedisplay();
        }
        else if(paused == true)
            Unpause();
        else
            Pause();
    }
}

void Pause(){
    if(walking == true || battling == true)
        return;
    paused = true;
    glutPostRedisplay();
}

void Unpause(){
    paused = false;
    glutPostRedisplay();
}

void Walk(int dir)
{
    if(paused == true){
        walking = false;
        return;
    }
    
    switch(dir){
        case UP:
            if(player.yPos == player.yNewPos)
                walking = 0;
            else
                player.yPos += 4;
            break;
        case DOWN:
            if(player.yPos == player.yNewPos)
                walking = 0;
            else
                player.yPos -= 4;
            break;
        case LEFT:
            if(player.xPos == player.xNewPos)
                walking = 0;
            else
                player.xPos -= 4;
            break;
        case RIGHT:
            if(player.xPos == player.xNewPos)
                walking = 0;
            else
                player.xPos += 4;
            break;
        
    }
    
    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
    
    /* Provera da li je igrac stigao do kranje lokacije */
    if (walking) {
        glutTimerFunc(TIMER_INTERVAL, Walk, dir);
    }else{
        /* Provera dal je u PokemonField-u */
        if(InPokemonField(player) == true){
            /* Random sansa za random encounter */
            int randomNumber = rand()%100+1;
            
            if(randomNumber <= 20){
                EngageEncounter();
            }
        }
    }
}

void EngageEncounter(){
    enemyPokemon = GetRandomPokemon(player.playerPokemon.level);
    
    PrintPokemon(enemyPokemon);
    PrintPokemon(player.playerPokemon);
    
    battling = true;
}

void Tackle(){
    PokemonAttack(&player.playerPokemon, &enemyPokemon);
    
    PrintPokemon(enemyPokemon);
    PrintPokemon(player.playerPokemon);
    
    if(enemyPokemon.health <= 0){
        battling = false;
    }
    
    if(player.playerPokemon.health <= 0){
        battling = false;
        Respawn();
    }
    
    glutPostRedisplay();
}

void ThrowPokeBall(){
    int randomNumber = rand()%100+1;
    
    if(randomNumber <= enemyPokemon.catchChancePercent){
        AddPokemon(&enemyPokemon,player.reservePokemon);
        battling = false;
    }
    else{
        player.playerPokemon.health -= enemyPokemon.attack;
        if(player.playerPokemon.health <= 0){
            battling = false;
            Respawn();
        }
    }
    
    //printf("Broj pokemona: %d\n",PokemonInReserve(player.reservePokemon));
    
    glutPostRedisplay();
}

void Respawn(){
    player.xPos = wOrtho/2;
    player.yPos = hOrtho/2-20;
    player.dir = DOWN;
    PokemonHeal(&player.playerPokemon);
}





