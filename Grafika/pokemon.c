#include "pokemon.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//COMMON 10
#define RATTATA 0
#define PIDGEY 1
#define CATERPIE 2
#define GEODUDE 3
#define BELLSPROUT 4
#define ZUBAT 5
#define WEEDLE 6
#define ODISH 7
#define DROWZEE 8
#define EKANS 9
//UNCOMMON 9
#define ARCANINE 10
#define ONYX 11
#define ABRA 12
#define GASTLY 13
#define SANDSHREW 14
#define JYNX 15
#define HORSEA 16
#define VULPIX 17
#define JIGGLYPUFF 18
//RARE 8
#define SNORLAX 19
#define LAPRAS 20
#define DRATINI 21
#define AERODACTYL 22
#define CHANSEY 23
#define CHARMANDER 24
#define SQUIRTLE 25
#define BULBASAUR 26
//LEGENDARY 5
#define MEWTWO 27
#define ENTEI 28
#define HOOH 29
#define RAIKOU 30
#define ARTICUNO 31

void PokemonAttack(Pokemon* playerPokemon, Pokemon* enemyPokemon){
    enemyPokemon->health -= playerPokemon->attack;
    if(enemyPokemon->health <= 0){
        EnemyPokemonFeinted(playerPokemon,enemyPokemon);
    }
    else{
        playerPokemon->health -= enemyPokemon->attack;
    }
    enemyPokemon->catchChancePercent += playerPokemon->attack;
}

void EnemyPokemonFeinted(Pokemon* a, Pokemon* b){
    a->exp += b->expWorth;
    if(a->exp >= a->expMax){
        LevelUp(a);
    }
}

void PokemonHeal(Pokemon* p){
    p->health = p->healthMax;
}

void PlayerPokemonFeinted(){
    
}

void LevelUp(Pokemon* a){
    a->level++;
    a->health = a->healthMax = 20 + a->level * 4;
    a->attack = 5 + a->level * 3;
    a->defence = 2 + a->level * 2.2;
    a->exp = 0;
    a->expMax =  a->level* (a->level-1) * 100;
}

Pokemon MakeStarterPokemon(){
    Pokemon p;
    p.name = "Pikachu";
    p.level = 1;
    p.healthMax = p.health = 20 + p.level * 4;
    p.attack = 5 + p.level * 3;
    p.defence = 2 + p.level * 2.2;
    p.exp = 0;
    p.expMax = 100;
    p.rarity = 'S';
    return p;
}

Pokemon GetRandomPokemon(int playerPokemonLevel){
    Pokemon p;
    int randomNumber = rand()%100+1;
    
    Image* image;
    image = image_init(0, 0);
    
    if(randomNumber <= 70){
        p = GetRandomCommonPokemon(playerPokemonLevel,image);
    }
    else if(randomNumber <= 90){
        p = GetRandomUncommonPokemon(playerPokemonLevel,image);
    }
    else if(randomNumber <= 99){
        p = GetRandomRarePokemon(playerPokemonLevel,image);
    }
    else{
        p = GetRandomLegendaryPokemon(playerPokemonLevel,image);
    }
    
    image_done(image);
    
    return p;
}

Pokemon GetRandomLegendaryPokemon(int playerPokemonLevel, Image* image){
    Pokemon p;
    /* Od 19 do (sa)26 */
    int randomNumber = rand() % 5 + 27;
    
    switch(randomNumber){
        case MEWTWO:
            p.name = "MEWTWO";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 22 + p.level * 5;
            p.attack = 6 + p.level * 2.5;
            p.defence = 6 + p.level * 2.5;
            p.expWorth = 100 + p.level * 20;
            image_read(image, "PokemonSprites/Mewtwo.bmp");
            break;
        case ENTEI:
            p.name = "Entei";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 22 + p.level * 5;
            p.attack = 6 + p.level * 2.5;
            p.defence = 6 + p.level * 2.5;
            p.expWorth = 100 + p.level * 20;
            image_read(image, "PokemonSprites/Entei.bmp");
            break;
        case RAIKOU:
            p.name = "Raikou";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 22 + p.level * 5;
            p.attack = 6 + p.level * 2.5;
            p.defence = 6 + p.level * 2.5;
            p.expWorth = 100 + p.level * 20;
            image_read(image, "PokemonSprites/Raikou.bmp");
            break;
        case ARTICUNO:
            p.name = "Articuno";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 22 + p.level * 5;
            p.attack = 6 + p.level * 2.5;
            p.defence = 6 + p.level * 2.5;
            p.expWorth = 100 + p.level * 20;
            image_read(image, "PokemonSprites/Articuno.bmp");
            break;
        case HOOH:
            p.name = "Ho-Oh";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 22 + p.level * 5;
            p.attack = 6 + p.level * 2.5;
            p.defence = 6 + p.level * 2.5;
            p.expWorth = 100 + p.level * 20;
            image_read(image, "PokemonSprites/Hooh.bmp");
            break;
    }
    p.catchChancePercent = 5;
    p.rarity = 'L';
    
    glGenTextures(1, &p.sprite);
    glBindTexture(GL_TEXTURE_2D, p.sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return p;
}

Pokemon GetRandomRarePokemon(int playerPokemonLevel, Image* image){
    Pokemon p;
    /* Od 19 do (sa)26 */
    int randomNumber = rand() % 8 + 19;
    
    switch(randomNumber){
        case SNORLAX:
            p.name = "Snorlax";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Snorlax.bmp");
            break;
        case CHANSEY:
            p.name = "Chansey";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Chansey.bmp");
            break;
        case LAPRAS:
            p.name = "Lapras";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Lapras.bmp");
            break;
        case AERODACTYL:
            p.name = "Aerodactyl";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Aerodactyl.bmp");
            break;
        case DRATINI:
            p.name = "Dratini";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Dratini.bmp");
            break;
        case BULBASAUR:
            p.name = "Bulbasaur";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Bulbasaur.bmp");
            break;
        case CHARMANDER:
            p.name = "Charmander";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Charmander.bmp");
            break;
        case SQUIRTLE:
            p.name = "Squirtle";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 4;
            p.attack = 6 + p.level * 2;
            p.defence = 6 + p.level * 2;
            p.expWorth = 40 + p.level * 10;
            image_read(image, "PokemonSprites/Squirtle.bmp");
            break;
    }
    p.catchChancePercent = 20;
    p.rarity = 'R';
    
    glGenTextures(1, &p.sprite);
    glBindTexture(GL_TEXTURE_2D, p.sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return p;
}

Pokemon GetRandomUncommonPokemon(int playerPokemonLevel, Image* image){
    Pokemon p;
    /* Od 10 do (sa)18 */
    int randomNumber = rand() % 9 + 10;
    
    switch(randomNumber){
        case ARCANINE:
            p.name = "Arcanine";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Arcanine.bmp");
            break;
        case VULPIX:
            p.name = "Vulpix";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Vulpix.bmp");
            break;
        case ABRA:
            p.name = "Abra";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Abra.bmp");
            break;
        case ONYX:
            p.name = "Onyx";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Onyx.bmp");
            break;
        case GASTLY:
            p.name = "Gastly";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Gastly.bmp");
            break;
        case JYNX:
            p.name = "Jynx";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Jynx.bmp");
            break;
        case HORSEA:
            p.name = "Horsea";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Horsea.bmp");
            break;
        case JIGGLYPUFF:
            p.name = "Jigglypuff";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Jigglypuff.bmp");
            break;
        case SANDSHREW:
            p.name = "Sandshrew";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 16 + p.level * 3;
            p.attack = 5 + p.level * 1.5;
            p.defence = 4 + p.level * 1.5;
            p.expWorth = 30 + p.level * 7;
            image_read(image, "PokemonSprites/Sandshrew.bmp");
            break;
    }
    p.catchChancePercent = 40;
    p.rarity = 'U';
    
    glGenTextures(1, &p.sprite);
    glBindTexture(GL_TEXTURE_2D, p.sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return p;
}

Pokemon GetRandomCommonPokemon(int playerPokemonLevel, Image* image){
    Pokemon p;
    /* Od 0 do (sa)9 */
    int randomNumber = rand()%10;
    
    switch(randomNumber){
        case RATTATA:
            p.name = "Rattata";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Rattata.bmp");
            break;
        case GEODUDE:
            p.name = "Geodude";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Geodude.bmp");
            break;
        case PIDGEY:
            p.name = "Pidgey";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Pidgey.bmp");
            break;
        case BELLSPROUT:
            p.name = "Bellsprout";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Bellsprout.bmp");
            break;
        case CATERPIE:
            p.name = "Caterpie";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Caterpie.bmp");
            break;
        case WEEDLE:
            p.name = "Weedle";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Weedle.bmp");
            break;
        case ZUBAT:
            p.name = "Zubat";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Zubat.bmp");
            break;
        case ODISH:
            p.name = "Odish";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Odish.bmp");
            break;
        case DROWZEE:
            p.name = "Drowzee";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Drowzee.bmp");
            break;
        case EKANS:
            p.name = "Ekans";
            p.level = playerPokemonLevel + rand() % 5 - 2;
            if(p.level <= 0)
                p.level = 1;
            p.health = p.healthMax = 14 + p.level * 2;
            p.attack = 3 + p.level;
            p.defence = 2 + p.level;
            p.expWorth = 20 + p.level * 5;
            image_read(image, "PokemonSprites/Ekans.bmp");
            break;
    }
    p.rarity = 'C';
    p.catchChancePercent = 75;
    
    glGenTextures(1, &p.sprite);
    glBindTexture(GL_TEXTURE_2D, p.sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return p;
}

void AddPokemon(Pokemon* p, Pokemon roster[5]){
    for(int i = 0; i < 5; i++){
        //attack, defence, health, healthMax, level, exp, expWorth, expMax, catchChancePercent;char rarity;char* name;GLuint sprite;
        if(strcmp(roster[i].name,"NULL") == 0){
            roster[i].name = p->name;
            roster[i].attack = p->attack;
            roster[i].defence = p->defence;
            roster[i].health = roster[i].healthMax = p->healthMax;
            roster[i].level = p->level;
            roster[i].exp = 0;
            roster[i].expMax = p->expMax;
            roster[i].rarity = p->rarity;
            roster[i].sprite = p->sprite;
            return;
        }
    }
    
    for(int i = 0; i < 5; i++){
        printf("SLOT %d:\n", i);
        PrintPokemon(roster[i]);
    }
    printf("NO MORE SPACE FOR POKEMON!\n");
    // Send to PC
}

int PokemonInReserve(Pokemon roster[5]){
    int i;
    for(i = 0; i < 5; i++)
    {
        if(strcmp(roster[i].name,"NULL") == 0)
            break;
    }
    return i;
}

void HealAllPokemon(Pokemon* p, Pokemon roster[5])
{
    int n = PokemonInReserve(roster);
    
    PokemonHeal(p);
    
    for(int i = 0; i < n; i++){
        PokemonHeal(&roster[i]);
    }
}

void ReleasePokemon(Pokemon roster[5], int index){
    int n = PokemonInReserve(roster);
    
    if(n-1 != index)
        SwapPokemon(&roster[index], &roster[n-1]);
    roster[n-1].name = "NULL";
}

void SwapPokemon(Pokemon* a, Pokemon* b){
    Pokemon tmp;
    tmp.name = b->name;
    tmp.attack = b->attack;
    tmp.defence = b->defence;
    tmp.health = b->health;
    tmp.healthMax = b->healthMax;
    tmp.level = b->level;
    tmp.exp = b->exp;
    tmp.expMax = b->expMax;
    tmp.rarity = b->rarity;
    tmp.sprite = b->sprite;
    
    b->name = a->name;
    b->attack = a->attack;
    b->defence = a->defence;
    b->health = a->health;
    b->healthMax = a->healthMax;
    b->level = a->level;
    b->exp = a->exp;
    b->expMax = a->expMax;
    b->rarity = a->rarity;
    b->sprite = a->sprite;
    
    a->name = tmp.name;
    a->attack = tmp.attack;
    a->defence = tmp.defence;
    a->health = tmp.health;
    a->healthMax = tmp.healthMax;
    a->level = tmp.level;
    a->exp = tmp.exp;
    a->expMax = tmp.expMax;
    a->rarity = tmp.rarity;
    a->sprite = tmp.sprite;
}

void PrintPokemon(Pokemon p){
    printf("--------POKEMON--------\nNAME: %s\nLVL: %d\nHP: %d\nATT: %d\nDEF: %d\nEXP: %d\nRARITY: %c\n-----------------------\n", p.name, p.level, p.health, p.attack, p.defence, p.exp, p.rarity);
}



