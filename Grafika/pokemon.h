#include "image.h"
#include <GL/glut.h>
#include <string.h>

typedef struct Pokemon{
    int attack, defence, health, healthMax, level, exp, expWorth, expMax, catchChancePercent;
    char rarity;
    char* name;
    GLuint sprite;
}Pokemon;

void PokemonAttack(Pokemon*, Pokemon*);
void EnemyPokemonFeinted(Pokemon*, Pokemon*);
void PlayerPokemonFeinted();
void LevelUp(Pokemon*);
Pokemon MakeStarterPokemon();
Pokemon GetRandomPokemon(int);
Pokemon GetRandomCommonPokemon(int,Image*);
Pokemon GetRandomUncommonPokemon(int,Image*);
Pokemon GetRandomRarePokemon(int,Image*);
Pokemon GetRandomLegendaryPokemon(int,Image*);
void AddPokemon(Pokemon*, Pokemon[]);
int PokemonInReserve(Pokemon[]);
void SwapPokemon(Pokemon*, Pokemon*);
void PrintPokemon(Pokemon);
void PokemonHeal(Pokemon*);
void HealAllPokemon(Pokemon*,Pokemon[]);
void ReleasePokemon(Pokemon[],int);