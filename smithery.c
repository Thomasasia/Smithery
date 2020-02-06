#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Sets up the random number generator with the clock as a seed
//srand(time(NULL));
// tuple struct to hold weapon data
// Stores the index (Redundant but screw you basically), the value, and the weapon name
static struct { char name[21]; float value; int index;} weapon[16];
int weaponCount = 0;
static int currentWeaponIndex;
static float currentWeaponCost;

// For random integer generation
int genRandInt(int lower, int upper){
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
}

float genRandFloat(float min, float max){
    float range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
//adds a tuple to the tuples, and fills it with values
static void addTuple(char *str, float val) {
    printf("Adding '%s', mapped to %f\n", str, val);
    printf("Weapon count = %d\n", weaponCount+1);
    strcpy(weapon[weaponCount].name, str);
    weapon[weaponCount].index = weaponCount;
    weapon[weaponCount++].value = val;
}

// Lists weapons for debug. Thanks stack overflow!
static void listWeaponTypes(void) {
    printf("==========\nTuple count is %d\n", weaponCount);
    for (int i = 0; i < weaponCount; ++i)
        printf("   [%s] -> %f\n", weapon[i].name, weapon[i].value);
    puts("==========");
}

// Populates the tuple array for the weapons
void configWeapons(void){
    addTuple("axe", 8.0);
    addTuple("poleaxe",14.0);
    addTuple("short Sword",12.0);
    addTuple("long Sword",15.0);
    addTuple("zweihander",20.0);
    addTuple("dagger",6.0);
    addTuple("knife",4.0);
    addTuple("spear",8.0);
    addTuple("pike",10.0);
    addTuple("halberd",15.0);
    addTuple("morning star", 14.0);
    addTuple("ball-and-chain",14.0);
    addTuple("warhammer",13.0);
    addTuple("maul",15.0);
    addTuple("war mattock",18.0);
    addTuple("scourge",12.0);

}

// This is what happens when a customer arrives.
void customer(void){
    // Randomly selects a weapon
    currentWeaponIndex = genRandInt(0,15);
    // Varies the value by up to 20% of the value of the weapon
    float variation = 0.2 * weapon[currentWeaponIndex].value;
    printf("I want a %s and I will pay you $%f\n", weapon[currentWeaponIndex].name, weapon[currentWeaponIndex].value + variation);
}
int main(){
    printf("Here comes the first customer!\n");
    srand(time(0));
    configWeapons();
    listWeaponTypes();
    double money = 100.0;
    printf("Starting money: %lf\n",money);
    customer();
    return 0;
}
