#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "fok_struct.h"

struct new_class_type new_class_table[NEW_MAX_CLASS] = {
 

    { //0
     "none", // the full name of the class
     "none", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     1, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, //Lawful, Neutral,Chaotic
     -1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    { //1
     "Warrior", // the full name of the class
     "war", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     4, // Dice hp
     2, // Dice Mana
     4, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    { //2
     "Mage", // the full name of the class
     "mag", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     1, // Dice hp
     5, // Dice Mana
     3, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //3
     "Cleric", // the full name of the class
     "cle", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     3, // Dice hp
     3, // Dice Mana
     3, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    
    { //4
     "Thief", // the full name of the class
     "thi", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     3, // Dice hp
     2, // Dice Mana
     4, // Dice Move
     0, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //5
     "Paladin", // the full name of the class
     "pal", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     5, // Dice hp
     3, // Dice Mana
     4, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,-1,-1}, //Good,Neutral,Evil
     {1,-1,-1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
      
    { //6
     "Knight", // the full name of the class
     "kni", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_SWORD, // First weapon
     1, // How good they hit
     6, // Dice hp
     1, // Dice Mana
     5, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,-1}, //Good,Neutral,Evil
     {1,-1,-1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },      

    { //7
     "Black knight", // the full name of the class
     "btk", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     5, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },        
    
    { //8
     "Barbarian", // the full name of the class
     "brb", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     7, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //9
     "Ronin", // the full name of the class
     "ron", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //10
     "Ranger", // the full name of the class
     "ran", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     5, // Dice Move
     1, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,-1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //11
     "Sorceror", // the full name of the class
     "sor", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     3, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //12
     "Conjurer", // the full name of the class
     "con", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     3, // Dice hp
     1, // Dice Mana
     5, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //13
     "Illusionist", // the full name of the class
     "ilu", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_DAGGER, // First weapon
     1, // How good they hit
     2, // Dice hp
     1, // Dice Mana
     3, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //14
     "Alchemist", // the full name of the class
     "alc", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     2, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //15
     "Witch", // the full name of the class
     "wit", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     1, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {  //16
     "Diabolic", // the full name of the class
     "dia", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     2, // Dice hp
     1, // Dice Mana
     3, // Dice Move
     4, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {  //17
     "Priest", // the full name of the class
     "pri", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //18
     "Inquisitor", // the full name of the class
     "ing", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     5, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,-1,-1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {  //19
     "Druid", // the full name of the class
     "dru", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_MACE, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,1,-1}, //Good,Neutral,Evil
     {-1,1,-1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {  //21
     "Necromancer", // the full name of the class
     "nec", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     3, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    {  //22
     "Shaman", // the full name of the class
     "sha", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     5, // Dice hp
     4, // Dice Mana
     5, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //23
     "Monk", // the full name of the class
     "mnk", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     3, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,-1}, //Good,Neutral,Evil
     {1,1,-1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //24
     "Rogue", // the full name of the class
     "rog", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     3, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    
    { //25
     "Assassin", // the full name of the class
     "asn", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     3, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },    

    {  //26
     "Bard", // the full name of the class
     "brd", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     4, // Dice hp
     5, // Dice Mana
     4, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },  
    
    {  //27
     "Spellfilcher", // the full name of the class
     "sfh", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     3, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     2, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {-1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },  

    
    { //28
     "Thri-kreen", // the full name of the class
     "thr", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     4, // Dice hp
     1, // Dice Mana
     4, // Dice Move
     5, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {1,1,1}, //Good,Neutral,Evil
     {1,1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },    


    { //29
     "Illithid", // the full name of the class
     "ill", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     1, // Dice hp
     7, // Dice Mana
     3, // Dice Move
     5, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     0 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //30
     "WarriorVampire", // the full name of the class
     "vwa", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     5, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },                

    { //31
     "ThiefVampire", // the full name of the class
     "vth", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     5, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },

    { //32
     "ClericVampire", // the full name of the class
     "vcl", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     5, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    },
    
    { //33
     "MageVampire", // the full name of the class
     "vma", // Three-letter name for 'who'
     OBJ_VNUM_SCHOOL_STAFF, // First weapon
     1, // How good they hit
     5, // Dice hp
     1, // Dice Mana
     6, // Dice Move
     6, // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 
     {-1,-1,1}, //Good,Neutral,Evil
     {-1,-1,1}, // Lawful, Neutral,Chaotic
     1 // Disabled -1 == Always 0 == Not Crestion 1 == Enabled
    }
 };

