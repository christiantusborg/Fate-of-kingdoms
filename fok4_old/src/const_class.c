#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "fok_struct.h"

struct class_type class_table[MAX_CLASS] = {
 

    { 
     "none", // the full name of the class
     "none", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     -1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    {
     "Warrior", // the full name of the class
     "war", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    {
     "Mage", // the full name of the class
     "mag", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Cleric", // the full name of the class
     "cle", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    
    {
     "Thief", // the full name of the class
     "thi", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Paladin", // the full name of the class
     "pal", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
      
    {
     "Knight", // the full name of the class
     "kni", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },      

    {
     "Black knight", // the full name of the class
     "btk", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },        
    
    {
     "Barbarian", // the full name of the class
     "brb", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Ronin", // the full name of the class
     "ron", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Ranger", // the full name of the class
     "ran", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Sorceror", // the full name of the class
     "sor", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Conjurer", // the full name of the class
     "con", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Illusionist", // the full name of the class
     "ilu", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Alchemist", // the full name of the class
     "alc", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Witch", // the full name of the class
     "wit", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Diabolic", // the full name of the class
     "dia", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Priest", // the full name of the class
     "pri", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Inquisitor", // the full name of the class
     "ing", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Druid", // the full name of the class
     "dru", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Necromancer", // the full name of the class
     "nec", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Shaman", // the full name of the class
     "sha", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Monk", // the full name of the class
     "mnk", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {
     "Rogue", // the full name of the class
     "rog", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    
    {
     "Assassin", // the full name of the class
     "asn", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },    

    {"Bard", // the full name of the class
     "brd", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },  
    
    {"Spellfilcher", // the full name of the class
     "sfh", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },  

    
    {"Thri-kreen", // the full name of the class
     "thr", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },    


    {"Illithid", // the full name of the class
     "ill", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {"Warrior Vampire", // the full name of the class
     "vwa", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },                

    {"Thief Vampire", // the full name of the class
     "vth", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {"Cleric Vampire", // the full name of the class
     "vcl", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    
    {"Mage Vampire", // the full name of the class
     "vma", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, // Lawful, Neutral,Chaotic
     {1,1,1}, //Good,Neutral,Evil
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    }
 };

