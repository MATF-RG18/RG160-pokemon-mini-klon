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
        if(playerPokemon->health <= 0){
            PlayerPokemonFeinted();
        }
    }
}

void EnemyPokemonFeinted(Pokemon* a, Pokemon* b){
    a->exp += b->expWorth;
    if(a->exp >= a->expMax){
        LevelUp(a);
    }
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
    
    if(randomNumber <= 70){
        p = GetRandomCommonPokemon(playerPokemonLevel);
    }
    else if(randomNumber <= 90){
        p = GetRandomUncommonPokemon(playerPokemonLevel);
    }
    else if(randomNumber <= 99){
        p = GetRandomRarePokemon(playerPokemonLevel);
    }
    else{
        p = GetRandomLegendaryPokemon(playerPokemonLevel);
    }
    
    return p;
}

Pokemon GetRandomLegendaryPokemon(int playerPokemonLevel){
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
            break;
    }
    p.catchChancePercent = 5;
    p.rarity = 'L';
    
    return p;
}

Pokemon GetRandomRarePokemon(int playerPokemonLevel){
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
            break;
    }
    p.catchChancePercent = 20;
    p.rarity = 'R';
    
    return p;
}

Pokemon GetRandomUncommonPokemon(int playerPokemonLevel){
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
            break;
    }
    p.catchChancePercent = 40;
    p.rarity = 'U';
    
    return p;
}

Pokemon GetRandomCommonPokemon(int playerPokemonLevel){
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
            break;
    }
    p.rarity = 'C';
    
    p.catchChancePercent = 75;
    
    return p;
}

void PrintPokemon(Pokemon p){
    printf("--------POKEMON--------\nNAME: %s\nLVL: %d\nHP: %d\nATT: %d\nDEF: %d\nRARITY: %c\n-----------------------\n", p.name, p.level, p.healthMax, p.attack, p.defence, p.rarity);
}



