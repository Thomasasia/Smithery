#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// Sets up the random number generator with the clock as a seed
//srand(time(NULL));
// tuple struct to hold weapon data
// Stores the index (Redundant but screw you basically), the value, and the weapon name
static struct { char name[21]; float value; int index;} weapon[16];
int weaponCount = 0;
static int currentWeaponIndex;
static float currentWeaponCost;
static char currentWeaponName[21];
static float variation;

static double playerMoney;

static int smithingSkill;
static double smithingXP;


// Sets up the materials to be used
//      Name is the material name. Ex Iron
//      Cost is the proportion of the weapon's base cost that must be paid to start the project
//      valueMod is the modifier to the final value of the weapon that will be applied after completion
//      dificulty impacts how hard it is to make weapons with this material. at smithingSkill = difficulty there is no penalty, otherwise it increases the chances of a negative result.
static struct { char name[21]; float cost; float valueMod; int difficulty;} material[6];
static int currentMaterialIndex;
int materialCount = 0;


static int smithingSkill;

// this is because im sick of the input messing me up
// without this, the program might not always wait for input.
int flushInput(void){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return 0;
}

// Generates a random int between the values of lower and upper
int genRandInt(int lower, int upper){
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
}

// Generates a random float between the values of lower and upper
float genRandFloat(float min, float max){

    //deal with negatives
    int minneg = min < 0;
    if(minneg){
        max -= min;
        min = 0;
    }



    float range = (max - min);
    double div = RAND_MAX / range;
    float result = min + (rand() / div);
    //50% chance it is negative if min is below 0
    if(minneg) 
            if(genRandInt(0,1)) result *= -1;
    return result;
}

//adds a tuple to the tuples, and fills it with values
static void addWeaponTuple(char *str, float val) {
//    printf("Adding '%s', mapped to %f\n", str, val);
//    printf("Weapon count = %d\n", weaponCount+1);
    strcpy(weapon[weaponCount].name, str);
    weapon[weaponCount].index = weaponCount;
    weapon[weaponCount++].value = val;
}

static void addMaterialTuple(char *str, float cost, float mod, int difficulty){
    strcpy(material[materialCount].name, str);
    material[materialCount].cost = cost;
    material[materialCount].valueMod = mod;
    material[materialCount++].difficulty = difficulty;

}

// Lists weapons for debug. Thanks stack overflow!
static void listWeaponTypes(void) {
    printf("==========\nTuple count is %d\n", weaponCount);
    for (int i = 0; i < weaponCount; ++i)
        printf("   [%s] -> %f\n", weapon[i].name, weapon[i].value);
    puts("==========");
}

static void listMaterials(void) {
    //TODO: Fix the last line of output.
    printf("==========\nThere are %d materials\n", materialCount);
    for (int i = 0; i < materialCount; ++i){
        printf("   (%i)%s:\n", i+1,material[i].name);
        printf("\tCreation Cost: %f\t", material[i].cost);
        printf("   Value Modifier: %f", material[i].valueMod);
        printf("    Material Difficulty: %i\n", material[i].difficulty);
    }
    puts("==========");
}

// Populates the tuple array for the weapons
void configWeapons(void){
    addWeaponTuple("axe", 8.0);
    addWeaponTuple("poleaxe",14.0);
    addWeaponTuple("short Sword",12.0);
    addWeaponTuple("long Sword",15.0);
    addWeaponTuple("zweihander",20.0);
    addWeaponTuple("dagger",6.0);
    addWeaponTuple("knife",4.0);
    addWeaponTuple("spear",8.0);
    addWeaponTuple("pike",10.0);
    addWeaponTuple("halberd",15.0);
    addWeaponTuple("morning star", 14.0);
    addWeaponTuple("ball-and-chain",14.0);
    addWeaponTuple("warhammer",13.0);
    addWeaponTuple("maul",15.0);
    addWeaponTuple("war mattock",18.0);
    addWeaponTuple("scourge",12.0);
}

void configMaterials(void){
    addMaterialTuple("Iron", 0.5, 0.0, 1);
    addMaterialTuple("Steel", 0.75, 0.2, 10);
    addMaterialTuple("Platinum", 1.5, 0.75, 25);
    addMaterialTuple("Mythril", 3.0, 3.0, 40);
    addMaterialTuple("Adamantium", 6.0, 8.0, 70);
    addMaterialTuple("Demonite", 15.0, 30.0, 100);
}

int getIntInput(void){
    int myInt;
    int result = scanf("%d", &myInt);
    
    if (result == EOF) {
        /* ... you're not going to get any input ... */
    }
    if (result == 0) {
        while (fgetc(stdin) != '\n');
    }
    return myInt;




}

//Gives you a Y/N prompt and returns 0 for N or 1 for Y
int ynQuestion(char text[50]){
    printf("%s \0", text);
    char answer;
    scanf(" %c",&answer);

    //makes sure that the response was eiter upper or lower case N or Y
    while(1){
        if(answer == 'y' || answer == 'Y') return 1;
        else if(answer == 'n' || answer == 'N') return 0;
        else printf("Please enter Y or N!   \0");
        scanf(" %c",&answer);
    }
    return 0;
}

// This is what happens when a customer arrives.
int customer(void){
    printf("Here comes a customer!\n");
    // Randomly selects a weapon
    currentWeaponIndex = genRandInt(0,15);
    // Varies the value by up to 20% of the value of the weapon
    variation = genRandFloat(-0.2,0.2) * weapon[currentWeaponIndex].value;

    //sets the globals to the new weapon
    currentWeaponCost = weapon[currentWeaponIndex].value;
    strcpy(currentWeaponName, weapon[currentWeaponIndex].name);

    printf("\"I want a %s and I will pay you based on how good the weapon is, plus %f\%.\"\n",currentWeaponName, variation);
    printf("\t(The base price of this weapon is %f)\n", currentWeaponCost);
    int accept = ynQuestion("Do you accept? (Y/N): ");
    // if you reject him he leaves
    if(!accept){
        printf("\"Fine then!\"\n");
        printf("The Customer leaves\n");
        return 0;
    }
    else{
        printf("\"Excellent!\"\n");
        return 1;
    }
    return 0;;
}


double customerPayment(double weaponValue){
    // TODO: add flavor text based on the quality of the weapon.
    double payment = ((variation/100)+1) * weaponValue;
    printf("\"Ah, thank you good Sir! I cannot wait to try out this new %s! Here is %lf for the weapon, plus the %f I promised, leading to a grand total of %lf!\"\n", currentWeaponName, weaponValue, variation, payment);
    return payment;

}

//This function is used to get input from the player as to which material they will use.
int chooseMaterial(void){
    printf("What material will you make the %s out of?\n", currentWeaponName);
    listMaterials();
    int index = 0;
    while(index == 0){
        printf("Enter that material's index(666 to cancel): ");
        index = getIntInput();
        printf("\n");

        // if the input is out of bounds or invalid, it's set to 0 and the loop continues, and a message is displayed.
        if(index == 666) return 666;
        if(index <= 0 || index > materialCount) index = 0;
        // if the material is to expensive, it's set to 0 and the loop continues
        double price = (material[index-1].cost + 1) * currentWeaponCost;
        if( price > playerMoney){
            index = 0;
            printf("Not enough money! That would cost %lf, but you only have %lf.\n", price, playerMoney);
        }
        if(index == 0)
            printf("Please enter a valid input.\n");
    }
    return index -1;
}

double craftWeapon(){
    printf("Time to craft the %s!\n",currentWeaponName);

    //The price of the weapon is calculated and deducted from you money
    float cost = currentWeaponCost * material[currentMaterialIndex].cost;
    printf("The materials cost you a total of $%f\n",cost);

    playerMoney -= cost;

    printf("You now have $%lf\n",playerMoney);


    //TODO: Add pokemon style ...
    //determines the difficulty to craft the weapon, with a small ammount of variation.
    int difficulty = material[currentMaterialIndex].difficulty + genRandInt(-1,1);
    //can't have negative difficulty!
    if(difficulty <= 0) difficulty = 1;
    //max level is 100
    else if(difficulty > 100) difficulty = 100;
    
    printf("Crafting difficulty level: %i\n",difficulty);
    printf("Crafting skill level: %i\n", smithingSkill);

    //Probability curves are fun.
    int smithRoll = 0.0;
    int difficultyRoll = 0.0;
    
    //TODO: Fix getchars
    flushInput();
    getchar();

    //Each level is a d20 rolled. The percentage difference determines the quality modifier.
    for(int i = 0; i < smithingSkill; i++){
        smithRoll += genRandInt(1,20);
    }
    for(int i = 0; i < difficulty; i++){
        difficultyRoll += genRandInt(1,20);
    }
    printf("You roll a %i out of a needed %i.\n", smithRoll, difficultyRoll);
    double dif = (double) smithRoll / difficultyRoll;

    //max mod is 2
    if(dif > 2) dif = 2;

    //flavor text
    if(dif <= 1.1 && dif >= 0.9) printf("You make a decent %s.\n", currentWeaponName);
    else if(dif > 1){ //positive cases
       if(dif >= 1.1 && dif <= 1.4) printf("You make a good %s.\n", currentWeaponName);
       else if(dif > 1.4 && dif <= 1.7) printf("You make an excellent %s!\n", currentWeaponName);
       else if(dif > 1.7 && dif < 1.9) printf("You make a masterful %s!\n", currentWeaponName);
       else if(dif >= 1.9 && dif <2) printf("You make an artifact %s!!\n", currentWeaponName);
       else if(dif == 2) printf("You make a Supreme Legendary Artifact %s!!!\n", currentWeaponName);
       //TODO : Name generator for max quality weapons??
    }
    else{ // negative cases
        if(dif < 0.9 && dif >= 0.6) printf("You make a poor %s.\n", currentWeaponName);
        else if (dif < 0.6 && dif >= 0.4) printf("You make a shoddy %s.\n", currentWeaponName);
        else if (dif < 0.4 && dif > 0.2) printf("You make an awful %s!.\n", currentWeaponName);
        else if (dif <= 0.2 && dif >= 0.1) printf("You make a completely useless and utterly worthless %s!!.\n", currentWeaponName);
        else if (dif < 0.1) printf("You make such an increadably useless %s, that simply by looking at it you feel inept at just about everything.\nFlowers die around it, babies cry when they see it, and priests forbit it from their temples. You should be ashamed.\n", currentWeaponName);


    }

    //printf("weap cost: %f\ndif: %lf\nvalueMod: %f\n",currentWeaponCost,dif,material[currentMaterialIndex].valueMod);
    double value = currentWeaponCost * dif * (1 + material[currentMaterialIndex].valueMod);

    printf("The weapon is worth %lf!\n", value);
    printf("        (The base value of the weapon is %f.)\n", currentWeaponCost);

    getchar();

    return value;
}

int calculateExperience(double exp){
    // For now the equation I shall use will be 20x - 20 per level
    // The raw level may be calculated via calculus. The antiderivitive of the function above gives the level.
    // This equation is 20x^2-20x - exp = level
    smithingXP += exp;

    // Using the above equation and the quadratic formula, we get the level
    double rawLevel = (20 + sqrt(400 + 4*20*smithingXP))/40;

    int newLevel = (int) rawLevel;
    if (newLevel > 100) newLevel = 100;
    if(newLevel > smithingSkill){
        printf("Level Up!!\n");
        printf("New Level: %i\n", newLevel);
        smithingSkill = newLevel;
    }
    printf("Progress to next level: %f\%\n",(rawLevel - newLevel)*100);
    return 0;
}

int main(void){
    // sets up random generator
    srand(time(0));
    // sets up the weapons' configurations
    configWeapons();
    configMaterials();

    //initiates starting resources & skills
    playerMoney = 100.0;
    smithingSkill = 1;

    printf("Starting money: $%lf\n", playerMoney);
    printf("Smithing skill: %i\n", smithingSkill);
    while(1){
        int customerAccepted = customer();
        //if the customer was rejected, dont forge the weapon!
        if(customerAccepted){
            currentMaterialIndex = chooseMaterial();
            if(currentMaterialIndex != 666){
                double weaponValue;
                weaponValue = craftWeapon();
                double payment = customerPayment(weaponValue);
                playerMoney += payment;
                calculateExperience(weaponValue); 
            }
            else printf("\"Fine then!\"\nThe Customer Leaves\n");

            getchar();
        }
        // Random living expenses, but with a probability curve!
        int livingExpenses = genRandInt(1,5) + genRandInt(1,5);
        printf("You pay your living expenses. -$%i\n", livingExpenses);
        playerMoney -= livingExpenses;
        if(playerMoney <= 0){
            printf("You are all out of money!\n");
            break;
        }
        flushInput();
        getchar();
        printf("Player money: $%lf\n", playerMoney);
        printf("Smithing skill: %i\n", smithingSkill);
    }
    // This is the game over screen.
    printf(" _____   ___  ___  ___ _____   _____  _   _ ___________ \n|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | |  ___| ___ \\\n| |  \\// /_\\ \\| .  . || |__   | | | || | | | |__ | |_/ /\n| | __ |  _  || |\\/| ||  __|  | | | || | | |  __||    / \n| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ / |___| |\\ \\ \n \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/\\____/\\_| \\_|\n");                                                       
                                                        
    return 0;
}
