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
void DrawPokecenter();
void DrawPokemonMenu();
void DrawHealthBarBelow(int xPos, int yPos, Pokemon p, int h, int w);
void DrawHealthBarAbove(int xPos, int yPos, Pokemon p, int h, int w);
void DrawExpBar(int xPos,int yPos,Pokemon p);
void DrawEnemyPokemon(int,int);
void DrawPlayerPokemon(int,int);
void DrawGrass(int,int,int,int);
void DrawPlayer();
void displayText(float,float,float,float,float,const char*);
void displayTextNoRaster(float,float,float,const char*);
void Pause();
void Unpause();
void UsePokecenter();
void Tackle();
void ThrowPokeBall();
void Respawn();

static GLuint sprites[5];
int wOrtho = 800, hOrtho = 600, arrowPos = 0, pokemonMenuArrowPos = 0, pCenterX = 40, pCenterY = 440;
PokemonField pf;
Player player;
Pokemon enemyPokemon;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
char* infoText;
int walking = false, battling = false, paused = false, infoTextActive = false;
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
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
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
    
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    image_read(image, "character1.bmp");

    /* Generisu se identifikatori tekstura. */
    glGenTextures(5, sprites);

    glBindTexture(GL_TEXTURE_2D, sprites[0]);
    
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
    image_read(image, "Resources/Pokecenter.bmp");
    
    glBindTexture(GL_TEXTURE_2D, sprites[4]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Kreira se sesta tekstura. */
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
        DrawPokecenter();
        DrawPlayer();
        if(infoTextActive == true){
            int n = strlen(infoText);
            glColor3f(1,1,1);
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
            displayText(wOrtho/2 - n*8, 70, 1, 1, 1, infoText);
        }
    }else{
        DrawEnemyPokemon(600,360);
        DrawPlayerPokemon(200,140);
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
    //glMatrixMode(GL_PROJECTION);
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

void DrawPokecenter(){
    
    glBindTexture(GL_TEXTURE_2D, sprites[4]);
    
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        
        glTexCoord2f(0, 0);
        glVertex2i(pCenterX, pCenterY);

        glTexCoord2f(1, 0);
        glVertex2i(pCenterX + 120, pCenterY);

        glTexCoord2f(1, 1);
        glVertex2i(pCenterX + 120, pCenterY + 120);

        glTexCoord2f(0, 1);
        glVertex2i(pCenterX, pCenterY + 120);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glColor3f(0.6,0.3,0.3);
    glBegin(GL_QUADS);
        glVertex2i(pCenterX + 40, pCenterY - 40);
        glVertex2i(pCenterX + 80, pCenterY - 40);
        glVertex2i(pCenterX + 80, pCenterY);
        glVertex2i(pCenterX + 40, pCenterY);
    glEnd();
    
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
    
    glBindTexture(GL_TEXTURE_2D, 0);
    /* Health bar */
    DrawHealthBarBelow(80,240,player.playerPokemon, 20, 120);
    
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
        
        glBindTexture(GL_TEXTURE_2D,0);
        /* Health bar */
        DrawHealthBarBelow(300,535 - i*100,player.reservePokemon[i], 10, 80);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /* Help text */
    glColor3f(0,0,0);
    font_style = GLUT_BITMAP_HELVETICA_18;
    displayText(460, 100, 0, 0, 0, "PRESS R TO RELEASE A POKEMON");
    displayText(540, 60, 0, 0, 0, "PRESS ENTER TO RETURN");
    
    if(strcmp(player.reservePokemon[0].name,"NULL") == 0){
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

void DrawHealthBarBelow(int xPos, int yPos, Pokemon p, int h, int w){
    float step = w/(float)p.healthMax;
    
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(xPos, yPos - 40);
        glVertex2f(xPos, yPos - 40 - h);
        glVertex2f(xPos + p.health * step, yPos - 40 - h);
        glVertex2f(xPos + p.health * step, yPos - 40);
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xPos, yPos - 40);
        glVertex2f(xPos, yPos - 40 - h);
        glVertex2f(xPos + w, yPos - 40 - h);
        glVertex2f(xPos + w, yPos - 40);
    glEnd();
    
    /* Health numbers */
    if(h > 15 && w > 100)
        font_style = GLUT_BITMAP_HELVETICA_18;
    else{
        font_style = GLUT_BITMAP_HELVETICA_12;
        xPos -= 13;
        yPos += 7;
    }
    glColor3f(0,0,0);
    int length = snprintf( NULL, 0, "%d", p.health );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", p.health );
    glRasterPos2f(xPos+35, yPos-56);
    displayTextNoRaster(0, 0, 0, str);
    displayTextNoRaster(0, 0, 0, "/");
    free(str);
    length = snprintf( NULL, 0, "%d", p.healthMax );
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", p.healthMax );
    displayTextNoRaster(0, 0, 0, str);
    free(str);
}

void DrawHealthBarAbove(int xPos, int yPos, Pokemon p, int h, int w){
    float step = 120/(float)p.healthMax;
    
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(xPos + 120, yPos + 160);
        glVertex2f(xPos + 120, yPos + 160 + h);
        glVertex2f(xPos + 120 - p.health * step, yPos + 160 + h);
        glVertex2f(xPos + 120 - p.health * step, yPos + 160);
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xPos + 120, yPos + 160);
        glVertex2f(xPos + 120, yPos + 160 + h);
        glVertex2f(xPos, yPos + 160 + h);
        glVertex2f(xPos, yPos + 160);
    glEnd();
    
    if(h > 15 && w > 100)
        font_style = GLUT_BITMAP_HELVETICA_18;
    else{
        font_style = GLUT_BITMAP_HELVETICA_12;
        xPos -= 30;
    }
    glColor3f(0,0,0);
    int length = snprintf( NULL, 0, "%d", p.health );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", p.health );
    glRasterPos2f(xPos+35, yPos+164);
    displayTextNoRaster(0, 0, 0, str);
    displayTextNoRaster(0, 0, 0, "/");
    free(str);
    length = snprintf( NULL, 0, "%d", p.healthMax );
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", p.healthMax );
    displayTextNoRaster(0, 0, 0, str);
    free(str);
}

void DrawExpBar(int xPos,int yPos, Pokemon p){
    float step2 = 180/(float)player.playerPokemon.expMax;
    
    glColor3f(1,0,1);
    glBegin(GL_QUADS);
        glVertex2f(xPos, yPos - 70);
        glVertex2f(xPos, yPos - 80);
        glVertex2f(xPos + player.playerPokemon.exp * step2, yPos - 80);
        glVertex2f(xPos + player.playerPokemon.exp * step2, yPos - 70);
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xPos, yPos - 70);
        glVertex2f(xPos, yPos - 80);
        glVertex2f(xPos + 180, yPos - 80);
        glVertex2f(xPos + 180, yPos - 70);
    glEnd();
    
    glColor3f(0,0,0);
    font_style = GLUT_BITMAP_HELVETICA_12;
    int length = snprintf( NULL, 0, "%d", p.exp );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", p.exp );
    glRasterPos2f(xPos+68, yPos-79);
    displayTextNoRaster(0, 0, 0, str);
    displayTextNoRaster(0, 0, 0, "/");
    free(str);
    length = snprintf( NULL, 0, "%d", p.expMax );
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", p.expMax );
    displayTextNoRaster(0, 0, 0, str);
    free(str);
}

void DrawEnemyPokemon(int xPos, int yPos){
    
    /* Health bar */
    DrawHealthBarAbove(xPos,yPos, enemyPokemon, 20, 120);
    
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
    
    /* Victory text */
    if(battling == true && enemyPokemon.health <= 0){
        font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        displayText(380,310,1,0,0,"VICTORY!");
        font_style = GLUT_BITMAP_HELVETICA_18;
        displayText(300,290,1,0,0,"(PRESS SPACE TO CONTINUE)");
    }
    
}

void DrawPlayerPokemon(int xPos, int yPos){
    
    /* Health bar */
    DrawHealthBarBelow(xPos, yPos, player.playerPokemon, 20, 120);
    
    /* Exp bar */
    DrawExpBar(xPos, yPos, player.playerPokemon);
    
    
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
    
    /* Defeat text */
    if(battling == true && player.playerPokemon.health <= 0){
        font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        displayText(380,310,1,0,0,"DEFEAT!");
        font_style = GLUT_BITMAP_HELVETICA_18;
        displayText(300,290,1,0,0,"(PRESS SPACE TO CONTINUE)");
    }
    
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
                if(player.yNewPos >= pCenterY && player.yNewPos <= pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.yNewPos = player.yPos;
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
                if(player.yNewPos >= pCenterY && player.yNewPos <= pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.yNewPos = player.yPos;
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
                if(player.yNewPos >= pCenterY && player.yNewPos < pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.xNewPos = player.xPos;
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
                if(player.yNewPos >= pCenterY && player.yNewPos < pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.xNewPos = player.xPos;
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
            player.yNewPos = player.yPos + 40;
            if(player.yPos == 600-40)
                break;
            if(player.yNewPos >= pCenterY && player.yNewPos < pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.yNewPos = player.yPos;
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
            player.yNewPos = player.yPos - 40;
            if(player.yPos == 0)
                break;
            if(player.yNewPos >= pCenterY && player.yNewPos < pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.yNewPos = player.yPos;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, DOWN);
        }
        break;
        
    case 'a':
        /* Pokrece se Walk i direkcija je levo. */
        if (!walking && !battling) {
            player.xNewPos = player.xPos - 40;
            if(player.xPos == 0)
                break;
            if(player.yNewPos >= pCenterY && player.yNewPos < pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.xNewPos = player.xPos;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, LEFT);
        }
        break;
        
    case 'd':
        /* Pokrece se Walk i direkcija je desno. */
        if (!walking && !battling) {
            player.xNewPos = player.xPos + 40;
            if(player.xPos == 800-40)
                break;
            if(player.yNewPos >= pCenterY && player.yNewPos < pCenterY+120 && player.xNewPos >= pCenterX && player.xNewPos < pCenterX+120)
                    player.xNewPos = player.xPos;
            walking = 1;
            glutTimerFunc(TIMER_INTERVAL, Walk, RIGHT);
        }
        break;
        
    case 32:
        if(battling){
            if(enemyPokemon.health <= 0){
                battling = false;
                glutPostRedisplay();
            }
            else if(player.playerPokemon.health <= 0){
                battling = false;
                Respawn();
                glutPostRedisplay();
            }
            else
                Tackle();
        }
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
        else if(paused && menu == POKEMON_MENU && PokemonInReserve(player.reservePokemon) > 0){
            SwapPokemon(&player.reservePokemon[pokemonMenuArrowPos], &player.playerPokemon);
            
            glutPostRedisplay();
        }
        else if(player.xPos == pCenterX + 40 && player.yPos == pCenterY - 40){
            UsePokecenter();
        }else
        {
            displayText(400,300, 0, 1, 0, "HELLO");
            glutPostRedisplay();
        }
        break;
        
    case 'r':
        if(paused && menu == POKEMON_MENU && PokemonInReserve(player.reservePokemon) > 0){
            ReleasePokemon(player.reservePokemon, pokemonMenuArrowPos);
            if(PokemonInReserve(player.reservePokemon) == pokemonMenuArrowPos)
                pokemonMenuArrowPos--;
            
            glutPostRedisplay();
        }
        break;
        
    case 'c':
        if(battling && enemyPokemon.health > 0)
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

void displayText(float x, float y, float r, float g, float b, const char *string){
    int j = strlen(string);
 
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for(int i = 0; i < j; i++){
        glutBitmapCharacter(font_style, string[i]);
    }
}

void displayTextNoRaster(float r, float g, float b, const char *string){
    int j = strlen(string);
 
    glColor3f(r, g, b);
    //glRasterPos2f(x, y);
    for(int i = 0; i < j; i++){
        glutBitmapCharacter(font_style, string[i]);
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
    
    infoTextActive = false;
    
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

void UsePokecenter(){
    // SFX
    infoText = "ALL YOUR POKEMON HAVE BEEN HEALED!";
    infoTextActive = true;
    
    HealAllPokemon(&player.playerPokemon, player.reservePokemon);
    glutPostRedisplay();
}

void Tackle(){
    PokemonAttack(&player.playerPokemon, &enemyPokemon);
    
    PrintPokemon(enemyPokemon);
    PrintPokemon(player.playerPokemon);
    
    if(enemyPokemon.health <= 0){
        //battling = false;
        enemyPokemon.health = 0;
    }
    
    if(player.playerPokemon.health <= 0){
        player.playerPokemon.health = 0;
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
    infoTextActive = true;
    infoText = "RESPAWNED";
}





