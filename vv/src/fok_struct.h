/***************************************************************************
 *  Fate of Kingdom IV                                                     *
 *   By Chrisitan Tusborg (Kazmir).                                        *
 *									   *
 * The use of this code without the permission of Christian Tusborg        *
 * author is prohibited.  You will be punished to the fullest extend       *
 * of the law.                                                             *
 *                                                                         *
 *     Original Fate of Kingdom Mud (Mud.mysteria.dk:4000)		   *
 ***************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <time.h>

#define MAX_SKILLLEVELS 5
#define TYPE_NONE	0
#define STAT_SHORT      0
#define MAX_BIT         40
#define MAX_RESI_VUNL   64
#define USE_RESI_VUNL   35


// Base data Races
extern  struct    stat_type stat_table [MAX_STATS];


struct race_type
{
    char *	name;			/* call name of the race */
    bool	pc_race;		/* can be chosen by pcs */
    long	act;			/* act bits for the race */
    long	aff;			/* aff bits for the race */
    long	aff2;			/* aff2 bits for the race */
    long	detect;			/* detect bits for the race */
    long	off;			/* off bits for the race */
    long	imm;			/* imm bits for the race */
    long        res;			/* res bits for the race */
    long	vuln;			/* vuln bits for the race */
    long	form;			/* default form flag for the race */
    long	parts;			/* default parts for the race */
}; 

// additional data for player races
struct pc_race_type
 {
    char *	name;			// Name MUST be in race_type
    char *	who_name;           //Who name
    char *	short_name;         //Short name
    int		hpdice;                 // Dice hp
    int		manadice;               // Dice Mana
    int		movedice;               // Dice Move
    sh_int	class_mult[MAX_CLASS];	// multiplier for class, Normal = 100
    sh_int	max_stats[MAX_STATS];	// maximum stats
    sh_int	min_stats[MAX_STATS];	// minimum stats
    sh_int	size;			// Size of race
    sh_int      alignment[3];               // Lawful, Neutral,Chaotic
    sh_int      ethos[3];               //Good,Neutral,Evil
    sh_int	disabled;               // Disabled -1 == Always 0 == Crestion 1 == Enabled
    int         resi_vunl[MAX_RESI_VUNL]; // new resistant and vunerable system
 };

// Player class
struct	class_type 
{
    char *	name;			// the full name of the class
    char 	who_name[5];	// Three-letter name for 'who'
    sh_int	weapon;			// First weapon
    sh_int	thac0;                  // How good they hit
    int		hpdice;                 // Dice hp
    int		manadice;               // Dice Mana
    int		movedice;               // Dice Move
    sh_int      classgroup;             // 0 == Base, 1 == Warrior, 2 == Thief, 3 == Cleric, 4 == Mage, 5 == Master
    sh_int  	alignment[3];           // Lawful, Neutral,Chaotic
    sh_int  	ethos[3];               //Good,Neutral,Evil
    sh_int	disabled;               // Disabled -1 == Always 0 == Crestion 1 == Enabled
};

struct    abilities_type
{
    char *       abiliti_name;                   // Name of abiliti        
    sh_int       abiliti_level[MAX_CLASS];       // Level needed by class onless it is a spell 
    sh_int       abiliti_skill[MAX_CLASS];       // On How high abiliti level can the class get it 
    sh_int       abiliti_rating[MAX_CLASS];      // How hard it is to learn
    sh_int       abiliti_race[MAX_PC_RACE];      // On How high abiliti level can a race get it (Race abiliti are level 0)
    SPELL_FUN *  spell_fun;                      // Spell pointer (for spells)    
    sh_int       abiliti_target;                 // Legal targets        
    sh_int       abiliti_minimum_position;       // Position for caster / user    
    sh_int *     abiliti_pgsn;                   // Pointer to associated gsn    
    sh_int       abiliti_slot;                   // Slot for #OBJECT loading    
    sh_int       abiliti_manacost;               // Minimum mana used        
    sh_int       abiliti_beats[MAX_SKILLLEVELS]; // Waiting time after use    
    char *       abiliti_noun_damage;            // Damage message        
    char *       abiliti_msg_off;                // Wear off message        
    char *       abiliti_msg_obj;                // Wear off message for obects    
    sh_int abiliti_type;                   // 1 == skill, 2 == spell. 3 == song, 4 == dance, 5 == focus, 6 == vampire
    sh_int Objects[6];                   // 1 == skill, 2 == spell. 3 == song, 4 == dance, 5 == focus, 6 == vampire
};

struct    bloodline_type
{
 char * name;
 int    number;
};


struct    stat_type
{
 char * stat_name;
 char   stat_short[3];
 int    stat_number;
};

struct   battleskill_type
{
 char *name;
};

struct battlepart_type
{
 char *name;
};
