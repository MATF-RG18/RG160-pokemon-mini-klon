typedef struct Pokemon{
    int attack, defence, health, healthMax, level, exp, expWorth, expMax, catchChancePercent;
    char rarity;
    char* name;
}Pokemon;

void PokemonAttack(Pokemon* playerPokemon, Pokemon* enemyPokemon);
void EnemyPokemonFeinted(Pokemon* a, Pokemon* b);
void PlayerPokemonFeinted();
void LevelUp(Pokemon* a);
Pokemon MakeStarterPokemon();
Pokemon GetRandomPokemon(int);
Pokemon GetRandomCommonPokemon(int);
Pokemon GetRandomUncommonPokemon(int);
Pokemon GetRandomRarePokemon(int);
Pokemon GetRandomLegendaryPokemon(int);
void PrintPokemon(Pokemon p);