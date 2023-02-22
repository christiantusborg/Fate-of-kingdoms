/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*     Fate 1.0 is copyright 1997-1998 Hui Kwong Tin (Takeda)               *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.							           *
*               							   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)		   *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


char *  const   month_name      [] =
{
"the Falcon",
"the Wolf",
"the Specter",
"the Strider",
"the Lily",
"Knowledge",
"Nature",
"Futility",
"the Dragon",
"the Daystar",
"Vigor",
"Conflict",
"Reaping",
"the Demon",
"the Angel",
"Great Lust",
"the Coming"
};

/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,	"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,	"wand"		},
    {   ITEM_STAFF,	"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,	"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,	"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,	"key"		},
    {	ITEM_FOOD,	"food"		},
    {	ITEM_MONEY,	"money"		},
    {	ITEM_BOAT,	"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,	"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,	"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,	"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {   ITEM_TATTOO,	"tattoo",	},
    {   ITEM_ARROW,	"arrow",	},
    {   ITEM_JUKEBOX,	"jukebox"	},
    {   ITEM_INGREDIENT,"ingredient"	},
    {   ITEM_TRAP,	"trap"	},
    {   0,		NULL		}
};
/* confuse command table  & 0=npc 1=pc 2=npc&pc ,*/
const	struct	confuse_type	confuse_table	[]	=
{
   { "do_mok",	2, TO_ALL  },
   { "do_give", 2, TO_ROOM },
   { NULL,	0, 0	   }
};

/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword,	25},
   { "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace ,	15},
   { "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger,	20},
   { "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe,	10},
   { "staff",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear,	15},
   { "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail,	5},
   { "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip,	5},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm,	20},
   { "bow",	OBJ_VNUM_SCHOOL_BOW,	WEAPON_BOW,	&gsn_bow,	0},
   { "arrow",   OBJ_VNUM_SCHOOL_POLEARM,WEAPON_ARROW,   &gsn_arrow,     0 },
   { "lance",   OBJ_VNUM_SCHOOL_LANCE,  WEAPON_LANCE,   &gsn_lance,     5 },
   { NULL,	0,				0,	NULL,		0}
};





/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[]		=
{
    { 	"none",		"hit",		-1		,0},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	,1},
    {   "stab",		"stab",		DAM_PIERCE	,1},
    {	"slash",	"slash",	DAM_SLASH	,1},
    {	"whip",		"whip",		DAM_SLASH	,1},
    {   "claw",		"claw",		DAM_SLASH	,0},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	,0},
    {   "pound",	"pound",	DAM_BASH	,1},
    {	"crush",	"crush",	DAM_BASH	,0},
    {   "grep",		"grep",		DAM_SLASH	,0},
    {	"bite",		"bite",		DAM_PIERCE	,0},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	,1},
    {   "suction",	"suction",	DAM_BASH	,0},
    {	"beating",	"beating",	DAM_BASH	,0},
    {   "digestion",	"digestion",	DAM_ACID	,0},
    {	"charge",	"charge",	DAM_BASH	,1},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	,1},
    {	"punch",	"punch",	DAM_BASH	,1},
    {	"wrath",	"wrath",	DAM_ENERGY	,0},
    {	"magic",	"magic",	DAM_ENERGY	,0},
    {   "divine",	"divine power",	DAM_HOLY	,0},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	,0},
    {	"scratch",	"scratch",	DAM_PIERCE	,0},
    {   "peck",		"peck",		DAM_PIERCE	,0},
    {   "peckb",	"peck",		DAM_BASH	,0},
    {   "chop",		"chop",		DAM_SLASH	,0},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	,1},
    {   "smash",	 "smash",	DAM_BASH	,1},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	,0},
    {	"flbite",	"flaming bite", DAM_FIRE	,0},
    {	"frbite",	"freezing bite", DAM_COLD	,0},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	,0},
    {	"chomp",	"chomp",	DAM_PIERCE	,0},
    {  	"drain",	"life drain",	DAM_NEGATIVE	,0},
    {   "thrust",	"thrust",	DAM_PIERCE	,1},
    {   "slime",	"slime",	DAM_ACID	,0},
    {	"shock",	"shock",	DAM_LIGHTNING	,0},
    {   "thwack",	"thwack",	DAM_BASH	,0},
    {   "flame",	"flame",	DAM_FIRE	,0},
    {   "chill",	"chill",	DAM_COLD	,0},
    {   "drown",        "drowning",        DAM_DROWNING ,0},          
    {   "illum",        "illumination",    DAM_LIGHT    ,0},          
    {   "sonicb",       "sonic boom",      DAM_SOUND    ,0},          
    {   "demonst",      "demonic strike",  DAM_HARM     ,0},          
    {   "pestil",       "pestilence",      DAM_DISEASE  ,0},    
    {   "abysmal",	"Abysmal Blight",  DAM_NEGATIVE ,0},
    {   NULL,		NULL,		0		,0}
};

/*  adds traps */
const 	struct attack_type	trap_attack_table[] =
{
    { 	"sleep","sleep",TRAP_DAM_SLEEP		,1},
    { 	"teleport","teleport",TRAP_DAM_TELEPORT	,1},
    { 	"fire","fire",TRAP_DAM_FIRE		,1},
    { 	"cold","cold",TRAP_DAM_COLD		,1},
    { 	"acid","acid",TRAP_DAM_ACID		,1},
    { 	"energy","energy",TRAP_DAM_ENERGY	,1},
    { 	"blunt","blunt",TRAP_DAM_BLUNT		,1},
    { 	"pierce","slash",TRAP_DAM_PIERCE	,1},
    { 	"slash","slash",TRAP_DAM_SLASH		,1},
    {   NULL,	NULL,	0			,1}
};
/*  end */

/* race table */
const 	struct	race_type	race_table	[]		=
{
    { "unique",	/* race name */
	FALSE, /* pc_race ? */
	0,	/* act flag */ 
	0, 	/* aff flag */
	0, 	/* aff2 flag */
	0, 	/* detect flag */
	0, 	/* Off flag */
	0, 	/* Immune flag */
	0, 	/* resist flag */
	0, 	/* vuln flag */
	0, 	/* form flag */
	0 },	/* Parts flag */

/*  race_type changes   */
/* ADD NEW RACE TYPES HERE */
/* *********************** */
/* *********************** */
/* generic for now */
{ /* 1 - */
"human", TRUE,
0,
0,
0,
0,
0,
0,
0,
0,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 2 - */
"elf",
TRUE,
0,
AFF_INFRARED,
0,
0,
0,
0,
RES_CHARM|RES_MENTAL,
0,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 3 - */
"drow",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE|OFF_FAST|OFF_KICK_DIRT|OFF_TRIP,
0,
RES_MENTAL|RES_CHARM,
VULN_LIGHT,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 4 - */
"dwarf",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_BERSERK,
0,
RES_POISON|RES_DISEASE	,	VULN_DROWNING	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 5 - */
"duergar",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_BERSERK,
0,
RES_NEGATIVE|RES_DISEASE,	VULN_LIGHT|VULN_DROWNING	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 6 - */
"pixie",
TRUE,
0,
AFF_INFRARED|AFF_FLYING,
0,
DETECT_MAGIC|DETECT_EVIL|DETECT_INVIS|ACUTE_VISION,
OFF_FAST,
0,
RES_CHARM|RES_SUMMON|RES_MAGIC|RES_LIGHT,
0,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 7 - */
"gargoyle",
TRUE,
0,
AFF_INFRARED|AFF_FLYING,
0,
DETECT_MAGIC,
OFF_PARRY,
0,
RES_SUMMON|RES_LIGHTNING|RES_ACID	,	VULN_LIGHT	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 8 - */
"draconian",
TRUE,
0,
AFF_FLYING,
0,
DETECT_GOOD,
OFF_BASH|OFF_DISARM|OFF_TRIP,
0,
RES_NEGATIVE|RES_MENTAL|RES_ACID,	VULN_HOLY	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 9 - */
"arial",
TRUE,
0,
AFF_INFRARED|AFF_FLYING,
0,
0,
OFF_FAST,
0,
RES_SILVER|RES_CHARM	,	VULN_CHARM|VULN_SUMMON	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 10 - */
"felar",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_TAIL,
0,
0,VULN_SUMMON,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 11 - */
"merma",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE,
0,
IMM_DROWNING,VULN_SUMMON	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 12 - */
"giant",
TRUE,
0,
0,
0,
0,
OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,
0,
RES_POISON|RES_DISEASE,	VULN_MENTAL	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 13 - */
"troll",
TRUE,
0,
AFF_INFRARED,
0,
DETECT_GOOD,
OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,
0,
RES_POISON|RES_DISEASE	,	VULN_FIRE	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 14 - */
"ogre",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,
0,
RES_POISON|RES_DISEASE	,	VULN_MAGIC|VULN_SUMMON,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 15 - */
"orc",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,
0,
RES_POISON|RES_DISEASE	,	VULN_MAGIC|VULN_SUMMON,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 16 - */
"darnok",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE|OFF_TRIP,
0,
RES_MAGIC,
VULN_HOLY|VULN_POISON|VULN_DISEASE,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{ /* 17 - */
"illithid",
TRUE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE|OFF_TRIP,
0,
RES_MENTAL|RES_ENERGY,
VULN_HOLY|VULN_POISON|VULN_DISEASE,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"githyanki",
FALSE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE|OFF_TRIP,
0,
RES_MAGIC,
VULN_HOLY|VULN_POISON|VULN_DISEASE,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},


/* *********************** */
/* *********************** */
/*  race_types added above */
 /*  end */
{
"human-parson",	FALSE,
0,
0,
0,
DETECT_MAGIC,
OFF_PARRY,
0,
RES_NEGATIVE,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"human-aristocrat",
FALSE,
0,
0,
0,
0,
OFF_RESCUE,
0,
RES_CHARM	,	VULN_POISON	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"human-militant",
FALSE,
0,
0,
0,
0,
OFF_DISARM|OFF_RESCUE,
0,
RES_SLASH	,	VULN_SUMMON	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"human-vagrant",
FALSE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE,
0,
RES_POISON|RES_DISEASE	,	VULN_CHARM	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"half-elf",
FALSE,
0,
0,
0,
0,
OFF_DODGE|OFF_KICK_DIRT|OFF_TRIP,
0,
RES_CHARM,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"wild-elf",
FALSE,
0,
AFF_INFRARED,
0,
ACUTE_VISION,
OFF_DODGE|OFF_KICK_DIRT|OFF_TRIP,
0,
RES_SUMMON,
0,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"high-elf",
FALSE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE|OFF_KICK_DIRT|OFF_TRIP,
0,
RES_CHARM|RES_SUMMON,
0,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"vexie",
FALSE,
0,
AFF_INFRARED|AFF_FLYING,
0,
DETECT_MAGIC|DETECT_EVIL|DETECT_INVIS|ACUTE_VISION,
OFF_FAST,
0,
RES_CHARM|RES_SUMMON|RES_MAGIC|RES_NEGATIVE,
VULN_HOLY,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"griffin",
FALSE,
0,
AFF_FLYING,
0,
DETECT_EVIL,
OFF_KICK|OFF_DODGE|OFF_RESCUE,
0,
RES_HOLY|RES_CHARM	,	VULN_NEGATIVE	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"cyclops",
FALSE,
0,
0,
0,
DETECT_EVIL,
OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,
0,
RES_BASH	,	VULN_LIGHTNING	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"ethereal",
FALSE,
0,
AFF_INFRARED|AFF_PASS_DOOR|AFF_FLYING,
0,
0,
OFF_DODGE|OFF_TRIP,
0,
RES_WEAPON|RES_DROWNING|RES_HOLY	,
VULN_NEGATIVE|VULN_LIGHTNING|VULN_FIRE	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
{
"nym",
FALSE,
0,
AFF_INFRARED,
0,
DETECT_MAGIC,
0,
0,
RES_MENTAL	,   VULN_DISEASE,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},

{
"svirfneblin",
FALSE,
0,
AFF_INFRARED,
0,
0,
OFF_DODGE,
0,
RES_POISON|RES_DISEASE,
0,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},
    { /* 12 */
	"bat",			FALSE,
	0,		 AFF_FLYING,	0, 0, OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P
    },

    { /* 13 */
	"bear",			FALSE,
	0,		0,
	0, 0, OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {  /* 14 */
	"cat",			FALSE,
	0,		0,	0, 0, OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {  /* 15 */
	"centipede",		FALSE,
	0,			0, 0,	0,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K
    },

    {  /* 16 */
	"dog",			FALSE,
	0,		0,	0, 0,	OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
    },

    { /* 17 */
	"doll",			FALSE,
	0,		0,	0, 0,	0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K
    },

    { /* 18 */
        "dragon", 		FALSE,
	0, 			AFF_INFRARED|AFF_FLYING,	0, 0, 0,
	0,			RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },

    { /* 19 */
	"fido",			FALSE,
	0,		0,	0, 0,	OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },

    { /* 20 */
	"fox",			FALSE,
	0,		0, 0, 0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    { /* 21 */
	"goblin",		FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 22 */
	"hobgoblin",		FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y
    },

    {  /* 23 */
	"kobold",		FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {  /* 24 */
	"lizard",		FALSE,
	0,		0,		0, 0, 0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {  /* 25 */
	"modron",		FALSE,
	0,		AFF_INFRARED,
	0, 0, ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K
    },

    {  /* 26 */
	"orc_npc",			FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_DISEASE,	VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 27 */
	"pig",			FALSE,
	0,		0,		0, 0, 0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },

    { /* 28 */
	"rabbit",		FALSE,
	0,		0,	0, 0,	OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K
    },

    { /* 29 */
	"school monster",	FALSE,
	ACT_NOALIGN,		0, 0, 0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },

    {  /* 30 */
	"snake",		FALSE,
	0,		0,		0, 0, 0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },

    {  /* 31 */
	"song bird",		FALSE,
	0,		AFF_FLYING,	0, 0, OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {  /* 32 */
	"troll_npc",		FALSE,
	0,		AFF_INFRARED, 0,
	DETECT_HIDDEN, OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V
    },

    {  /* 33 */
	"water fowl",		FALSE,
	0,		AFF_FLYING, 0, 0,		0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {  /* 34 */
	"wolf",			FALSE,
	0,		0,	0, 0, OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {  /* 35 */
	"wyvern",		FALSE,
	0,		AFF_FLYING, 0,
	DETECT_INVIS|DETECT_HIDDEN, OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X
    },

    {  /* 36 */
	"unique",		FALSE,
	0,		0,		0, 0, 0,
	0,		0,		0,
	0,		0
    },


    {
	NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};


/* pc_race_type structure
*    {
*	"race name", 	short name, 	points,	{ class multipliers },
* for class multipler -1 means that class is not avaliable for that race.
*	{ bonus skills },
*	{ stat mods },	{ max stats },	size , hp_bonus, mana_bonus,
*	  practice_bonus, align
*    },
*/
/*remember mer now = sha*/
const	struct	pc_race_type	pc_race_table	[MAX_PC_RACE]	=
{
  { "null race", "", 0,{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
      { "" }, { 13, 13, 13, 13, 13 }, { 18, 18, 18, 18, 18 }, 0,0,0,0,0 },
/*  new pc_race_table */
/* --------------------- */
{ /* 1 - */
"human",
"{BHuman{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
{ },
{0,0,0,0,0},
{21,20,21,20,20},
SIZE_MEDIUM,
//5,
8,
5,
0,
""
},
{ /* 2 - */
"elf",
"{g Elf {w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{ 100,-1,-1, -1,100,100,100,100,-1,100,100, -1,100,-1,100,100,-1,-1},
{ "sneak" },
{-1,0,0,1,-1},
{19,22,24,23,17},
SIZE_MEDIUM,
//-5,
7,
15,
0,
""
},
{ /* 3 - */
"drow",
"{D Drow{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, 100, 100,-1, 100,100,-1,100,-1,100,-1,100,100,-1,-1,100},
{ "sneak","hide","detect hidden" },
{-2,1,-1,2,-1},
{20,20,22,23,18},
SIZE_MEDIUM,
//-5,
7,
15,
0,
"evil"
},
{ /* 4 - */
"dwarf",
"{yDwarf{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,100, -1, 100,100, -1,100,-1,100,100,-1,-1,-1,-1,-1,100,-1},
{ "berserk" },
{0,-1,1,-3,2},
{22,18,20,18,23},
SIZE_SMALL,
//20,
9,
-10,
0,
""
},
{ /* 5 */
"duergar",
"{yDuerg{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, 100, 100,-1, -1,100,-1,100,-1,100,-1,-1,-1,-1,-1,-1},
{ "berserk" },
{1,-1,0,-2,1},
{23,17,19,18,23},
SIZE_SMALL,
//15,
8,

-5,
0,
"evil"
},
{ /* 6 */
"pixie",
"{MPixie{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, -1, 100,-1, 100,100,100,100,-1,-1,100,-1,100,100,-1,-1},
{ "sneak","invis","detect magic","detect invis","detect hidden" },
{-4,2,2,0,-3},
{13,25,25,22,16},
SIZE_TINY,
//-10,
4,
20,
0,
"good"
},
{ /* 7 */
"gargoyle",
"{DGargo{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, -1, 100,100, 100,100,-1,100,-1,-1,-1,-1,-1,-1,-1,-1},
{ "stone skin","armor","rock flesh","sanctuary" },
{2,-1,-1,-1,1},
{22,20,19,18,22},
SIZE_LARGE,
//15,
9,
-5,
0,
"Good"
},
{ /* 8 */
"draconian",
"{RDraco{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, 100, 100,-1, -1,100,-1,100,-1,100,-1,100,-1,-1,-1,100},
{ "curse","blindness","fireball","acid blast" },
{1,-1,-1,-2,1},
{23,18,19,18,22},
SIZE_LARGE,
//0,
7,
10,
0,
"evil"
},
{ /* 9 */
"arial",
"{WArial{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, -1, 100,-1, 100,100,-1,100,100,-1,-1,-1,-1,-1,100,-1},
{ "animal essence", "acute vision","detect invis","detect hidden","fly" },
{-1,-1,0,2,-1},
{17,21,21,23,17},
SIZE_SMALL,
//0,
7,
10,
0,
"good"
},
{ /* 10 */
"felar",
"{rFelar{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, 100,-1, -1, 100,-1, 100,100,-1,100,-1,100,100,-1,-1,100,100,-1},
{ "rage","berserk","detect hidden","infrared" },
{1,-1,-1,1,0},
{20,19,19,25,19},
SIZE_MEDIUM,
//10,
6,
0,
0,
""
},
{ /* 11 */
"merma",
"{CMerma{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, 100,-1, -1, 100,100, -1,100,100,100,-1,-1,-1,-1,100,-1,-1,-1},
{ "know alignment","detect magic","create water","create food","swim" },
{0,0,0,-1,1},
{22,20,21,20,21},
SIZE_MEDIUM,
//3,
8,
7,
0,
"good"
},
{ /* 12 */
"giant",
"{YGiant{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, 100,-1, -1, 100,-1, -1,100,-1,100,-1,-1,-1,-1,-1,-1,-1,-1},
{ "bash","enhanced damage","kick" },
{2,-1,-1,0,1},
{25,16,17,14,25},
SIZE_GIANT,
//20,
13,
-10,
0,
"neutral"
},
{ /* 13 */
"troll",
"{DTroll{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, 100, 100,-1, -1,100,-1,100,-1,-1,-1,-1,-1,-1,-1,-1},
{ "bash","enhanced damage","kick" },
{1,-1,-2,0,1},
{23,16,16,18,23},
SIZE_HUGE,
//20,
12,
-10,
0,
"evil"
},
{ /* 14 */
"ogre",
"{D Ogre{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, 100,-1, -1, 100,-1, -1,100,-1,100,-1,-1,-1,-1,-1,-1,-1,-1},
{ "bash","enhanced damage","kick" },
{3,-3,-3,-5,2},
{24,17,17,16,24},
SIZE_HUGE,
//20,
12,
-10,
0,
""
},
{ /* 15 */
"orc",
"{g Orc {w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, 100,-1, 100, 100,-1, -1,100,-1,100,-1,100,-1,-1,-1,-1,-1,-1},
{ "bash","enhanced damage","kick" },
{2,-1,-2,-3,2},
{23,19,19,18,21},
SIZE_LARGE,
//20,
11,
-10,
0,
"evil"
},
{ /* 16 */
"darnok",
"{DDarnk{w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, 100, 100,-1, 100,100,100,100,-1,-1,-1,-1,100,-1,-1,100},
{ "create water","dodge","pass door" },
{-2,1,1,1,-2},
{20,21,19,21,20},
SIZE_MEDIUM,
//-5,
6,
15,
0,
"evil"
},
{ /* 17 */
"Illithid",
"{D Ill {w",
0,
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
{100, -1,-1, -1, 100,-1, -1,100,100,100,-1,-1,-1,100,100,-1,-1,100}, 
{ "curse","blindness","energy drain","paralysis" },
{-1,1,1,0,-1},
{18,25,25,18,17},
SIZE_MEDIUM,
//-10,
6,
20,
0,
"evil"
},
/* --------------------- */
/*  end */
};

/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
/*  new classes added */
/* here */
/* goes */
{
"warrior",
"War",
STAT_STR,
OBJ_VNUM_SCHOOL_SWORD,
{3022,0,0,0,0,0},
75,
30,
-10,
14,
//14,
16,
FALSE,
"",
"all",
500,
{1,0,0,0,1},
""
},
{
"shaman",
"Sha",
STAT_CON,
OBJ_VNUM_SCHOOL_STAFF,
{3022,0,0,0,0,0},
75,
10,
-6,
8,
//12,
5,
TRUE,
"",
"all",
500,
{1,0,0,1,0},
"neutral"
},
{
"paladin",
"Pdn",
STAT_STR,
OBJ_VNUM_SCHOOL_SWORD,
{3022,0,0,0,0,0},
75,
25,
-7,
8,
//12,
10,
TRUE,
"",
"all",
500,
{1,0,1,0,0},
"good"
},
{
"black-knight",
"Bkt",
STAT_STR,
OBJ_VNUM_SCHOOL_SWORD,
{3022,0,0,0,0,0},
75,
25,
-6,
8,
//12,
10,
TRUE,
"",
"all",
500,
{1,0,1,0,0},
"evil"
},
{
"thief",
"Thi",
STAT_DEX,
OBJ_VNUM_SCHOOL_DAGGER,
{3028,0,0,0,0,0},
75,
15,
-6,
8,
//11,
8,
FALSE,
"",
"all",
500,
{0,0,0,1,1},
""
},
{
"inquisitor",
"Inq",
STAT_DEX,
OBJ_VNUM_SCHOOL_DAGGER,
{3028,0,0,0,0,0},
75,
15,
-5,
8,
8,
TRUE,
"",
"all",
500,
{0,0,1,1,0},
""
},
{
"spellfilcher",
"Spe",
STAT_DEX,
OBJ_VNUM_SCHOOL_DAGGER,
{3028,0,0,0,0,0},
75,
10,
-4,
7,
//10,
7,
TRUE,
"",
"all",
500,
{0,1,1,0,0},
""
},
{
"cleric",
"Cle",
STAT_WIS,
OBJ_VNUM_SCHOOL_MACE,
{3003,0,0,0,0,0},
75,
15,
-4,
8,
//11,
5,
TRUE,
"",
"all",
500,
{0,1,1,0,0},
""
},
{
"prodigy",
"Pro",
STAT_INT,
OBJ_VNUM_SCHOOL_MACE,
{3003,0,0,0,0,0},
75,
15,
-2,
7,
//9,
5,
TRUE,
"",
"all",
500,
{0,1,1,0,0},
""
},
{
"mage",
"Mag",
STAT_INT,
OBJ_VNUM_SCHOOL_DAGGER,
{3018,0,0,0,0,0},
75,
5,
0,
7,
//9,
4,
TRUE,
"",
"all",
500,
{0,1,0,1,0},
""
},
{
"ranger",
"Ran",
STAT_STR,
OBJ_VNUM_SCHOOL_SWORD,
{3022,0,0,0,0,0},
75,
20,
-10,
11,
//13,
9,
FALSE,
"",
"all",
500,
{1,0,0,1,0},
"good"
},
{
"assassin",
"Asn",
STAT_DEX,
OBJ_VNUM_SCHOOL_DAGGER,
{3028,0,0,0,0,0},
75,
20,
-6,
8,
//12,
8,
TRUE,
"",
"all",
500,
{1,0,0,1,0},
"evil"
},
{
"bard",
"Brd",
STAT_INT,
OBJ_VNUM_SCHOOL_DAGGER,
{3018,0,0,0,0,0},
75,
20,
-4,
8,
//12,
8,
TRUE,
"",
"all",
500,
{0,0,1,0,1},
""
},
{
"maligner",
"Mal",
STAT_INT,
OBJ_VNUM_SCHOOL_DAGGER,
{3018,0,0,0,0,0},
75,
5,
0,
6,
//9,
5,
TRUE,
"",
"all",
500,
{0,1,0,1,0},
"evil"
},
{
"alchemist",
"Alc",
STAT_INT,
OBJ_VNUM_SCHOOL_DAGGER,
{3018,0,0,0,0,0},
75,
5,
0,
7,
//10,
4,
TRUE,
"",
"all",
500,
{0,0,1,1,0},
""
},
{
"druid",
"Drd",
STAT_WIS,
OBJ_VNUM_SCHOOL_STAFF,
{3018,0,0,0,0,0},
75,
10,
0,
8,
//10,
6,
TRUE,
"",
"all",
500,
{0,0,1,0,1},
"neutral"
},
{
"monk",
"Mnk",
STAT_DEX,
0,
{3018,0,0,0,0,0},
75,
25,
-8,
12,
//15,
8,
FALSE,
"",
"all",
500,
{0,0,0,1,1},
"good"
},
{
"necromancer",
"Nec",
STAT_INT,
OBJ_VNUM_SCHOOL_DAGGER,
{3018,0,0,0,0,0},
75,
10,
0,
6,
//15,
8,
TRUE,
"",
"all",
500,
{0,0,1,1,0},
"evil"
},
/*  end */
};

/*
 * Titles.
 */
char *	const			title_table	[MAX_CLASS][MAX_LEVEL+1][2] =
{
  {
 /* warrior */
     { "Man",			      "Woman"				},

	{ "Green Fighter",	     "Green Fighter"			},
	{ "Swordpupil","Swordpupil"			},
	{ "Warrior Trainee",	"Warrior Trainee"			},
	{ "Recruit",	"Recruit"			},
	{ "Sentry",	"Sentress"			},

	{ "Fighter",	"Fighter"			},
	{ "Soldier",	"Soldier"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Warrior",			"Warrior"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Contender",		     "Contender"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",		"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Battler",			"Battler"			},
	{ "Polemic",			"Polemic"			},

	{ "Grand Fighter",		"Grand Fighter"			},
	{ "Master of Swords",		"Master of Swords"			},
	{ "Gladiator",			"Gladiator"			},
	{ "Master Gladiator",	"Master Gladiator"		},
	{ "Skull-Cracker",		"Skull-Cracker"			},

	{ "Skull-Crusher",	"Skull-Crusher"		},
	{ "Dragon Slayer",		     "Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord of Weapons",	"Underlord of Swords"			},
	{ "Overlord of War",		"Overlord of Combat"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	     "Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "Baron of Fire",  "Baroness of Fire"},
	{ "Baron of Ice",  "Baroness of Ice"},
	{ "Baron of Lightning",  "Baroness of Lightning"},
	{ "Master of Stone",  "Mistress of the Stone"},
	{ "Master of Copper",  "Mistress of Copper"},

	{ "Master of Bronze",  "Mistress of Bronze"},
	{ "Master of Brass",  "Mistress of Brass"},
	{ "Master of Iron",  "Mistress of Iron"},
	{ "Master of Steel",  "Mistress of Steel"},
	{ "Master of Mithril",  "Mistress of Mithril"},

	{ "Master of Adamantite",  "Mistress of Adamantite"},
	{ "Veteran Captain",  "Veteran Captain"},
	{ "General",  "General"},
	{ "Field Marshall",  "Field Marshall"},
	{ "Master of War",  "Mistress of War"},

	{ "Hero of the Blade",		"Heroine of the Blade"		},
	{ "Avatar of War",		"Avatar of War"			},
	{ "Angel of War",		"Angel of War"			},
	{ "Demigod of War",		"Demigoddess of War"		},
	{ "Immortal Warlord",		"Immortal Warlord"		},
	{ "God of War",			"God of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
     { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

  /* Mercenary */
   {
     { "Man",			      "Woman"				},

	{ "Self-Seeking",     "Self-Seeking"			},
	{ "Tree Hugger",	"Tree Hugger"			},
	{ "Selfish",	"Selfish"			},
	{ "Leaf Collector",	"Leaf Collector"			},
	{ "Shameless",	"Shameless"			},

	{ "Lewd",	     "Lewd"			},
	{ "Crude",	     "Crude"			},
	{ "Cheapshot",	"Cheapshot"			},
	{ "Anti-Social",			"Anti-Social"			},
	{ "Estranged",			"Estranged"			},

	{ "Avaricious",			"Avariscious"			},
	{ "Sneaky Myrmidon",			"Sneaky Myrmidon"			},
	{ "Bonepicker",		"Bonepicker"		},
	{ "Unknown Contender",		     "Uknown Contender"			},
	{ "Stranger",		"Stranger"			},

	{ "Forest Whelp",		"Forest Whelp"			},
	{ "Interim",			"Interim"			},
	{ "Ephemeral",			"Ephemeral"			},
	{ "Drifter",			"Drifter"			},
	{ "Interloper",			"Interloper"			},

	{ "Trespasser",		"Trespasser"			},
	{ "Unseen Trespasser",		"Unseen Trespasser"			},
	{ "Flame Eater",			"Flame Eater"			},
	{ "Infamous Flame Eater",	"Infamous Flame Eater"		},
	{ "Contusionist",		"Contusionist"			},

	{ "Infamous Contusionist",	"Infamous Contusionist"		},
	{ "Breaker of Bones",    "Breaker of Bones"			},
	{ "Infamous Bone Breaker","Infamous Bone Breaker"		},
	{ "Seeker",			     "Seeker"			},
	{ "Punisher",			     "Punisher"			},

	{ "Expert on Bruises",		"Expert on Bruises"		},
	{ "Expert of Contusions",		"Expert of Contusions"		},
	{ "Expert of Concussions",		"Expert of Concussions"		},
	{ "Expert on Persistence",	"Expert on Persistence"	},
	{ "Expert on Tenacity",		"Expert on Tenacity"		},

	{ "Expert of Endurance",  "Expert of Endurance"},
	{ "Expert on Submission",  "Expert on Submission"},
	{ "Connoisseur of Close Calls",  "Connoisseur of Close Calls"},
	{ "Master of the Wounds",  "Mistress of Wounds"},
	{ "Master of Abuse",  "Mistress of Abuse"},

	{ "Master of Hurt",  "Mistress of Hurt"},
	{ "Master of Pain",  "Mistress of Pain"},
	{ "Master of Submission",  "Mistress of Submission"},
	{ "Master of Tenacity",  "Mistress of Tenacity"},
	{ "Master of Mangling",  "Mistress of Mangling"},

	{ "Contracted Thrasher",  "Contracted Thrasher"},
	{ "Giver of a Good Beating",  "Giver of a Good Beating"},
	{ "Governor of Anguish",  "Governor of Anguish"},
	{ "Busy Bountyman",  "Busy Bountywoman"},
	{ "Ignoble Bounty Hunter",  "Ignoble Bounty Hunter"},

	{ "Bounty Hunter Extraordinaire",		"Bounty Hunter Extraordinaire"		},
	{ "Avatar",		"Avatar"			},
	{ "Angel",		"Angel"			},
	{ "Demigod",		"Demigoddess"		},
	{ "Immortal",		"Immortal"		},
	{ "God",			"God"			},
	{ "Deity",		"Deity"			},
	{ "Supreme",	"Supreme"	},
     { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
/* Defender */
    {
      { "Man",                        "Woman"                         },
      { "Defender Pupil",  "Defender Pupil"},
      { "Scullery Man",  "Scullery Maid"},
      { "Squire Candidate",  "Squire Candidate"},
      { "Shield Bearer",  "Shield Bearer"},
      { "Sword Bearer",  "Sword Bearer"},
      { "Bow Bearer",  "Bow Bearer"},
      { "Standard Bearer",  "Standard Bearer"},
      { "Horseman",  "Horsewoman"},
      { "Squire Initiate",  "Squire Initiate"},
      { "Squire",  "Squire"},
      { "Footman",  "Footwoman"},
      { "Pikeman",  "Pikewoman"},
      { "Bowman",  "Bowwoman"},
      { "Swordsman",  "Swordswoman"},
      { "Honorable",  "Honorable"},
      { "Noble",  "Noble"},
      { "Trustworthy",  "Trustworthy"},
      { "Truthful",  "Truthful"},
      { "Chivalrous",  "Chivalrous"},
      { "Defender",  "Defender"},
      { "Questor",  "Questor"},
      { "Cavalier",  "Cavalier"},
      { "Champion",  "Champion"},
      { "Knight of Renown",  "Knight of Renown"},
      { "Defender Knight",  "Defender Lady"},
      { "Templar Initiate",  "Templar Initiate"},
      { "Priest-Knight",  "Priestess-Knight"},
      { "Knight of the Cross",  "Lady of the Cross"},
      { "Champion of the Cross",  "Champion of the Cross"},
      { "Knight Templar",  "Lady Templar"},
      { "Templar",  "Templar"},
      { "Templar Footman",  "Templar Footwoman"},
      { "Templar Lieutenant",  "Templar Lieutenant"},
      { "Templar Captain",  "Templar Captain"},
      { "Templar Colonel",  "Templar Colonel"},
      { "Templar General",  "Templar General"},
      { "Templar Field Marshall",  "Templar Field Marshall"},
      { "Healer-Knight",  "Healer-Lady"},
      { "Avenger",  "Avenger"},
      { "Crusader",  "Crusader"},
      { "Leader of the Crusades",  "Leader of the Crusades"},
      { "Champion of the Crusades",  "Champion of the Crusades"},
      { "Champion of the Templars",  "Champion of the Templars"},
      { "Priest of the Templars",  "Priestess of the Templars"},
      { "High Priest of the Templars",  "High Priestess of Templars"},
      { "Lord of the Templars",  "Lady of the Templars"},
      { "Hammer of Heretics",  "Hammer of Heretics"},
      { "Slayer of Infidels",  "Slayer of Infidels"},
      { "Pious",  "Pious"},
      { "Holy Knight",  "Holy Lady"},
      { "Defender Hero",  "Defender Heroine"},

      { "Avatar of Honor",		"Avatar of Honor"		},
      { "Angel of Honor",		"Angel of Honor"		},
      { "Demigod of Honor",		"Demigoddess of Honor"		},
      { "Immortal Knight",		"Immortal Knight"		},
      { "God of Honor",		"God of Honor"			},
      { "Deity of Honor",		"Deity of Honor"		},
      { "Supreme Master of Honor",	"Supreme Mistress of Honor"	},
      { "Creator",                    "Creator"                       },
      { "Implementor",		"Implementress"			}
    },
/* Black Knight */
    {
      { "Man", "Woman" },
      { "Scum",  "Scum"},
      { "Bully",  "Bully"},
      { "Anarchist",  "Anarchist"},
      { "Brute",  "Bitch"},
      { "Ruffian",  "Butch"},
      { "Pillager",  "Pillager"},
      { "Destroyer",  "Destroyer"},
      { "Burner",  "Burner"},
      { "Heretic",  "Heretic"},
      { "Attacker",  "Attacker"},
      { "Mercenary",  "Mercenary"},
      { "Black Sword",  "Black Sword"},
      { "Crimson Sword",  "Crimson Sword"},
      { "Sneaky",  "Sneaky"},
      { "Cruel",  "Cruel"},
      { "Despised",  "Despised"},
      { "Infamous",  "Infamous"},
      { "Hated",  "Hated"},
      { "Complete Bastard",  "Complete Bitch"},
      { "Defiler",  "Defiler"},
      { "Evil Fighter",  "Evil Fighter"},
      { "Dark Fighter",  "Dark Fighter"},
      { "Evil Champion",  "Evil Champion"},
      { "Dishonorable",  "Dishonorable"},
      { "Black Knight",  "Black Lady"},
      { "Crimson Knight",  "Crimson Lady"},
      { "Knight of Brimstone",  "Lady of Brimstone"},
      { "Knight of the Inverted Cross",  "Lady of the Inverted Cross"},
      { "Knight of Pain",  "Lady of Pain"},
      { "Knight of Legion",  "Lady of Legion"},
      { "Footman of Legion",  "Footwoman of Legion"},
      { "Cavalier of Legion",  "Cavalier of Legion"},
      { "Captain of Legion",  "Captain of Legion"},
      { "General of Legion",  "General of Legion"},
      { "Field Marshall of Legion",  "Field Marshall of Legion"},
      { "Knight of the Apocaplypse",  "Lady of the Apocalypse"},
      { "LightSlayer",  "LightSlayer"},
      { "Invoker of Suffering",  "Invokeress of Suffering"},
      { "Paramount Corruption",  "Paramount Corruption"},
      { "Evil Knight",  "Evil Lady"},
      { "Evil Lord",  "Evil Baroness"},
      { "Evil King",  "Evil Queen"},
      { "Herald of Depravity",  "Herald of Depravity"},
      { "Spreader of Pestilence",  "Spreader of Pestilence"},
      { "Bringer of Famine",  "Bringer of Famine"},
      { "Harbringer of Death",  "Harbringer of Death"},
      { "Unholy Bishop",  "Unholy Priestess"},
      { "Slayer",  "Slayer"},
      { "Evil Incarnate",  "Evil Incarnate"},
      { "Unholy Knight",  "Unholy Lady"},
      { "Anti-Hero",  "Anti-Heroine"},

      { "Avatar of Evil",		"Avatar of Evil"		},
      { "Angel of Evil",		"Angel of Evil"		},
      { "Demigod of Evil",		"Demigoddess of Evil"		},
      { "Immortal Anti-Knight",	"Immortal Anti-Knight"		},
      { "God of Evil",		"God of Evil"			},
      { "Deity of Evil",		"Deity of Evil"		},
      { "Supreme Master of Evil",	"Supreme Mistress of Evil"	},
      { "Creator",                    "Creator"                       },
      { "Implementor",		"Implementress"			}
    },
/* thief */
    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Mugger",  "Mugger"},
	{ "Swindler",		"Swindler"		},
	{ "Hijacker",			"Hijacker"			},
	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},

	{ "Master Spy",			"Master Spy"			},
	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},

	{ "Master of Touch",		"Mistress of Touch"		},
	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},

	{ "Godfather",			"Godmother"			},
	{ "Crime Boss",  "Crime Boss"},
	{ "Kingpin",  "Kingpin"},
	{ "Greater Kingpin",  "Greater Kingpin"},
	{ "Master of Piercing",  "Master of Piercing"},

	{ "Master of Silence",  "Mistress of Silence"},
	{ "Master of Guile",  "Mistress of Guile"},
	{ "Master of the Dagger",  "Mistress of the Dagger"},
	{ "Master of Poison",  "Mistress of Poison"},
	{ "Master of Stealth",  "Mistress of Stealth"},

	{ "Master Thief",  "Master Thief"},
	{ "Master of Thieves",  "Master of Thieves"},
	{ "Unseen",  "Unseen"},
	{ "Grand Master of Guile",  "Grand Mistress of Guile"},
	{ "Grand Master of Theft",  "Grand Mistress of Theft"},

	{ "Thief Hero",		"Thief Heroine"		},
	{ "Avatar of Death",		"Avatar of Death",		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Demigod",	"Demigoddess"	},
	{ "Immortal",		"Immortal"		},
	{ "God",		"God",		},
	{ "Deity",		"Deity"		},
	{ "Supreme",		"Supreme"		},
     { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
/* inquisitor */
    {
	{ "Man",			"Woman"				},

	{ "Firm Believer",			"Firm Believer"			},
	{ "Daft Attendant",			"Daft Attendant"			},
	{ "Misled Acolyte",			"Misled Acolyte"			},
	{ "Daft Novice",			"Daft Novice"			},
	{ "Forceful Missionary",			"Forceful Missionary"			},

	{ "Sneaky Adept",			"Sneaky Adept"				},
	{ "Daft Deacon",			"Daft Deaconess"			},
	{ "Zealous Vicar",			"Zealous Vicaress"			},
	{ "Selfish Priest",			"Selfish Priestess"			},
	{ "Confused Minister",			"Confused Lady of Ministry"			},

	{ "Cogent",			"Cogent"				},
	{ "Wild Levite",			"Wild Levitess"			},
	{ "Convincing",			"Convincing"			},
	{ "Unstable",			"Unstable"				},
	{ "Rigid",			"Rigid"			},

	{ "Candid",			"Candid"			},
	{ "Callow Expositor",			"Callow Expositress"			},
	{ "Callow Bishop",			"Callow Bishop"			},
	{ "Callow Arch Bishop",		"Callow Arch Bishop"	},
	{ "Archprelate",			"Archprelate"			},

	{ "Puerile",		"Puerile"		},
	{ "Unsophisticated",		"Unsophisticated"		},
	{ "Apprehensive",		"Apprehensive"		},
	{ "Great Apprehensive",		"Great Apprehensive"			},
	{ "Zealot",	"Zealot"		},

	{ "Analyst of the Sea",	"Analyst of the Sea"		},
	{ "Analyst of the Earth",	"Analyst of the Earth"		},
	{ "Analyst of the the Winds",	"Analyst of the the Winds"		},
	{ "Analyst of the Ether",	"Analyst of the Ether"		},
	{ "Analyst of the Heavens",	"Analyst of the Heavens"	},

	{ "Questioner",  "Questioner"},
	{ "Seeker",  "Seeker"},
	{ "Investigator",  "Investigator"},
	{ "Suspicious",  "Suspicious"},
	{ "Devout Analyst",  "Devout Analyst"},

	{ "Order of the White Robe"  "Order of the White Robe"},
	{ "Order of the Grey Robe",  "Order of the Grey Robe"},
	{ "Order of the Black Robe",  "Order of the Black Robe"},
	{ "Order of the Red Robe",  "Order of the Red Robe"},
	{ "Examiner of the Order",  "Examiner of the Order"},

	{ "Examiner of Earth",  "Examiner of Earth"},
	{ "Examiner of the Sea",  "Examiner of the Sea"},
	{ "Examiner of Time",  "Examiner of Time"},
	{ "Interrogator",  "Interrogator"},
	{ "Adamant Interrogator",  "Adamant Interrogator"},

	{ "Censor",	"Censor"		},
	{ "Expurgator",		"Expurgator"		},
	{ "Self Appointed Judge",	"Self Appointed Judge"		},
	{ "Arbiter of Personal Opinion",	"Arbiter of Personal Opinion"	},
	{ "Magistrate of an Imaginary Court","Magistrate of an Imaginary Court"},

	{ "Inquisitor Hero",			"Inquisitor Hero"			},
	{ "Holy Avatar",		"Holy Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

/* spellfilcher */
    {
	{ "Man",			"Woman"				},

	{ "Measly Pilferer",	"Measly Pilferess"			},
	{ "Street Urchin",			"Street Urchin"			},
	{ "Key Filcher",			"Key Filcheress"			},
	{ "Petty Pick-Pocket",		"Petty Pick-Pocket"			},
	{ "Casting Sneak",			"Casting Sneak"				},

	{ "Guttersnipe",			"Guttersnipe"			},
	{ "Curious",			"Curious"			},
	{ "Nosy",			"Nosy"			},
	{ "Meddlesome",			"Meddlesome"			},
	{ "Waif",			"Waif"				},

	{ "Potion Robber",			"Potion Robber"			},
	{ "Scroll Filcher",			"Scroll Filcher"			},
	{ "Arcane Lifter",			"Arcane Lifter"			},
	{ "Tower Burglar",			"Tower Burglaress"			},
	{ "Hasty",			"Hasty"				},

	{ "Trickster",			"Trickster"			},
	{ "Fraud",		"Fraud"			},
	{ "Imposter",			"Imposter"			},
	{ "Dissember",			"Dissembler"			},
	{ "Defrauder",			"Defraudar"			},

	{ "Spell Swindler",		"Spell Swindler"		},
	{ "Mage Hijacker",			"Mage Hijacker"			},
	{ "Tower Spy",			"Tower Spy"				},
	{ "Grand Tower Spy",			"Grand Tower Spy"			},
	{ "Master Tower Spy",			"Master Tower Spy"			},

	{ "Soliciter of Scrolls",		"Soliciter of Scrolls"		},
	{ "Soliciter of Potions",		"Soliciter of Potions"		},
	{ "Soliciter of Spellbooks",		"Soliciter of Spellbooks"	},
	{ "Soliciter of Enchanted Daggers","Soliciter of Enchanted Daggers"},
	{ "Soliciter of Knowledge",		"Soliciter of Knowledge"		},

	{ "Borrower",			"Borrower"		},
	{ "Infamous Borrower",	"Infamous Borrower"	},
	{ "Parasite",      "Parasite"	},
	{ "Mana Leech",			"Mana Leech"			},
	{ "Mana Thief",		"Mana Thief"		},

	{ "Castle Invader",  "Castle Invader"},
	{ "Violator",  "Violator"},
	{ "Veritable Living Probe",  "Veritable Living Probe"},
	{ "Borrowing Annoyance",  "Borrowing Annoyance"},
	{ "Master of Mana Scent",  "Master of Mana Scent"},

	{ "Follower of Mana Trails",  "Follower of Mana Trails"},
	{ "Usurper of Research",  "Usurper of Research"},
	{ "Counterfeit Caster",  "Counterfeit Caster"},
	{ "Master of Removal",  "Mistress of Removal"},
	{ "Master of Seizure",  "Mistress of Seizure"},

	{ "Master Confiscator",  "Master Confiscator"},
	{ "Expropriating",  "Expropriating"},
	{ "Furtive Spellfilcher",  "Furtive Spellfilcher"},
	{ "Grand Master Spell Forger",  "Grand Mistress Spell Forger"},
	{ "Grand Master Plagiarist",  "Grand Mistress Plagiarist"},

	{ "Spellfilcher Hero",		"Spellfilcher Heroine"		},
	{ "Avatar",		"Avatar",		},
	{ "Angel",		"Angel"		},
	{ "Demigod",	"Demigoddess"	},
	{ "Immortal",		"Immortal"		},
	{ "God",		"God",		},
	{ "Deity",		"Deity"		},
	{ "Supreme",		"Supreme"		},
     { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
/* cleric */
    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},

	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},

	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},
	{ "Rector",			"Rector"				},
	{ "Healer",			"Healess"			},

	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},

	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},

	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},

	{ "Enlightened",  "Enlightened"},
	{ "Sacred",  "Sacred"},
	{ "Saint",  "Saint"},
	{ "Divine",  "Divine"},
	{ "Cardinal of Flowers",  "Cardinal of Flowers"},

	{ "Cardinal of Water",  "Cardinal of Water"},
	{ "Cardinal of Fire",  "Cardinal of Fire"},
	{ "Cardinal of Ice",  "Cardinal of Ice"},
	{ "Cardinal of the Winds",  "Cardinal of the Winds"},
	{ "Cardinal of Storms",  "Cardinal of Storms"},

	{ "Father of Earth",  "Mother of Earth"},
	{ "Father of the Sea",  "Mother of the Sea"},
	{ "Father of Time",  "Mother of Time"},
	{ "Sacred Heart",  "Sacred Heart"},
	{ "Holy Father",  "Holy Mother"},

	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},

	{ "Holy Hero",			"Holy Heroine"			},
	{ "Holy Avatar",		"Holy Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

/* prodigy */
    {
        { "Man",                        "Woman"                         },

        { "Psychic",                    "Psychic",                      },
        { "Medium",                     "Medium",                       },
        { "Gypsy",                      "Gypsy",                        },
        { "Meditator",                  "Meditator",                    },
        { "Mind Prober",                "Mind Prober",                  },

        { "Soul Searcher",              "Soul Searcher",                },
        { "Astral Voyager",             "Astral Voyager",               },
        { "Seeker",                     "Seeker",                       },
        { "Empath",                     "Empath",                       },
        { "Mind Reader",                "Mind Reader"                   },

        { "Telepath",                   "Telepath",                     },
        { "Mental Adept",               "Mental Adept",                 },
        { "Spoonbender",                "Spoonbender",                  },
        { "Perceptive",                 "Perceptive",                   },
        { "Clever",                     "Clever",                       },

        { "Sensible",   		"Sensible",   },
        { "Responsive",   		"Responsive",   },
        { "Impressible",   		"Impressible",   },
        { "Receptive",   		"Receptive",   },
        { "Perceptive",   		"Perceptive",   },

        { "Reactive",   		"Reactive",   },
        { "Incisive",   		"Incisive",   },
        { "Observant",   		"Observant",   }, 
        { "Wise",                       "Wise",                         },
        { "Genius",                     "Genius",                       },

        { "Oracle",                     "Oracle",                       },
        { "Soothsayer",                 "Soothsayer",                   },
        { "Truthteller",                "Truthteller",                  },
        { "Wise",                       "Wise",                         },
        { "Keen Observant",   		"Keen Observant",   },

        { "Explorer of Cognizance",     "Explorer of Cognizance",   },
        { "Master Psychic",             "Mistress Psychic",             },
        { "Master Meditator",           "Mistress Meditator",           },
        { "Master Empath",              "Mistress Empath",              },
        { "Master Clairvoyant",         "Mistress Clairvoyant",         },

        { "Master Mind Reader",         "Mistress Mind Reader",         },
        { "Master Telepath",            "Mistress Telepath",            },
        { "Master Spoonbender",         "Mistress Spoonbender",         },
        { "Grand Master Psychic",       "Grand Mistress Psychic",       },
        { "Grand Master Meditator",     "Grand Mistress Meditator",     },

        { "Grand Master Empath",        "Grand Mistress Empath",        },
        { "Grand Master Clairvoyant",   "Grand Mistress Clairvoyant",   },
        { "Grand Master Mind Reader",   "Grand Mistress Mind Reader",   },
        { "Grand Master Telepath",      "Grand Mistress Telepath",      },
        { "Grand Master Spoonbender",   "Grand Mistress Spoonbender",   },


        { "Reader of Eyes",   "Reader of Eyes",   },
        { "Warden of Intellect",   "Warden of Intellect",   },
        { "Exalted Soul Searcher",   "Exalted Soul Searcher",   },
        { "Experienced Prodigy",   "Experienced Prodigy",   },
        { "Grand Prodigy",  "Grand Prodigy",   },

        { "Prodigy Hero",            "Prodigy Herione",           },
	{ "Holy Avatar",		"Holy Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}

    },

/* mage */
                {
      { "Man",			"Woman"				},

      { "Apprentice of Magic",	"Apprentice of Magic"		},
      { "Spell Student",		"Spell Student"			},
      { "Scholar of Magic",		"Scholar of Magic"		},
      { "Delver in Spells",		"Delveress in Spells"		},
      { "Medium of Magic",		"Medium of Magic"		},

      { "Seeker of Magic",		"Seeker of Magic"		},
      { "Seer",			"Seeress"			},
      { "Prudent",			"Prudent"				},
      { "Illusionist",		"Illusionist"			},
      { "Abjurer",			"Abjuress"			},

      { "Invoker",			"Invoker"			},
      { "Enchanter",			"Enchantress"			},
      { "Conjurer",			"Conjuress"			},
      { "Magician",			"Magician"				},
      { "Creator",			"Creator"			},

      { "Intelligent",			"Intelligent"			},
      { "Magus",			"Craftess"			},
      { "Wizard",			"Wizard"			},
      { "Warlock",			"War Witch"			},
      { "Sorcerer",			"Sorceress"			},

      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Great Sorcerer",		"Great Sorceress"		},
      { "Spellbinder",		"Spellbinder"			},
      { "Reknown Spellbinder",	"Reknown Spellbinder"		},

     { "Apprentice Invoker",		"Apprentice Invoker",		},
	{ "Student of Evocation",		"Student of Evocation",		},
	{ "Student of Ice",		"Student of Ice",		},
	{ "Student of Pyrotechniques","Student of Pyrotechniques",		},
	{ "Greater Invoker",		"Greater Invoker",		},

	{ "Wyvern Breeder",	"Wyvern Breeder"	},
	{ "Wyvern Trainer",		"Wyvern Trainer"		},
	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
     { "Channeler",  "Channeler"},

      { "Master Channeler",  "Master Channeler"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Sortilege",  "Mistress of Sortilege"},
      { "Master of Legerdemain",  "Master of Legerdemain"},

      { "Master of Delving",  "Mistress of Delving"},
      { "Master of Sorcery",  "Mistress of Sorcery"},
      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master of Magic",  "Mistress of Magic"},
      { "Master of the Past",  "Mistress of the Past"},

      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Future",  "Mistress of the Future"},
      { "Spellbinder",  "Spellbinder"},
      { "Arch Mage",  "Arch Mage"},
      { "Mystical",  "Mystical"},

	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },
/* ranger */
    {
      { "Man", "Woman" },
      { "Forest Pupil",  "Forest Pupil"},
      { "Forest Recruit",  "Forest Recruit"},
      { "Forester",  "Forester"},
      { "Fletcher",  "Fletcher"},
      { "Bowyer",  "Bowyer"},
      { "Trapper",  "Trapper"},
      { "Hunter",  "Huntress"},
      { "Tracker",  "Tracker"},
      { "Seeker",  "Seeker"},
      { "Scout",  "Scout"},
      { "Master Scout",  "Mistress Scout"},
      { "Green Man",  "Green Woman"},
      { "Archer",  "Archer"},
      { "Apprentice Ranger",  "Apprentice Ranger"},
      { "Woodsman",  "Woodswoman"},
      { "Master Woodsman",  "Mistress Woodswoman"},
      { "Ranger Initiate",  "Ranger Initiate"},
      { "Ranger Candidate",  "Ranger Candidate"},
      { "Ranger Squire",  "Ranger Squire"},
      { "Yeoman",  "Yeoman"},
      { "Inaugurated Ranger",  "Inaugurated Ranger"},
      { "Forest Yeoman",  "Forest Yeoman"},
      { "Ranger",  "Ranger"},
      { "Ranger Captain",  "Ranger Captain"},
      { "Warder",  "Warder"},
      { "Warder Captain",  "Warder Captain"},
      { "Warder General",  "Warder General"},
      { "Master of Warders",  "Mistress of Warders"},
      { "Knight of the Forest",  "Lady of the Forest"},
      { "Sword of the Forest",  "Sword of the Forest"},
      { "Guardian of the Forest",  "Guardian of the Forest"},
      { "Lord of the Forest",  "Queen of the Forest"},
      { "Overlord of the Forest",  "Overlord of the Forest"},
      { "Baron of Forests",  "Baroness of Forests"},
      { "Keeper",  "Keeper"},
      { "Huntsman",  "Huntswoman"},
      { "Caller of the Pack",  "Caller of the Pack"},
      { "Master of the Pack",  "Mistress of the Pack"},
      { "Ranger Knight Initiate",  "Ranger Lady Initiate"},
      { "Ranger Knight",  "Ranger Lady"},
      { "Ranger Lord",  "Ranger Lady"},
      { "Ranger Baron", "Ranger Baroness"},
      { "Ranger Prince",  "Ranger Princess"},
      { "Ranger King",  "Ranger Queen"},
      { "Warder of the Glade",  "Warder of the Glade"},
      { "Keeper of the Glade",  "Keeper of the Glade"},
      { "Knight of the Glade",  "Lady of the Glade"},
      {"Master of the Glade",  "Mistress of the Glade"},
      { "King of the Glade",  "Queen of the Glade"},
      { "Watcher",  "Watcher"},
      { "Ranger Hero",  "Ranger Heroine"},

	{ "Ranger Avatar",	"Ranger Avatar"		},
	{ "Ranger Angel",		"Ranger Angel"		},
	{ "Ranger Demigod",		"Ranger Demigoddess"		},
	{ "Immortal Ranger",	"Immortal Ranger"		},
	{ "Ranger God",		"Ranger God"			},
	{ "Ranger Deity",		"Ranger Deity" 		},
	{ "Supreme Ranger",	"Supreme Ranger"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

/* assassin */
    {
      { "Man", "Woman" },
      { "Uke",  "Uke"},
      { "Tori",  "Tori"},
      { "White Belt",  "White Belt"},
      { "Yellow belt",  "Yellow Belt"},
      { "Green Belt",  "Green Belt"},
      { "3rd Brown",  "3rd Brown"},
      { "2nd Brown",  "2nd Brown"},
      { "1st Brown",  "1st Brown"},
      { "3rd Black",  "3rd Black"},
      { "2nd Black",  "2nd Black"},
      { "Okuiri",  "Okuiri"},
      { "Shuto",  "Shuto"},
      { "Uraken",  "Uraken"},
      { "Shotei",  "Shotei"},
      { "Tateken",  "Tateken"},
      { "Seiken",  "Seiken"},
      { "Ude",  "Ude"},
      { "Empi",  "Empi"},
      { "Josokutei",  "Josokotei"},
      { "Kasokutei",  "Kasokutei"},
      { "Master of Atemi Waza",  "Mistress of Atemi Waza"},
      { "Master of Kotegaeshi",  "Mistress of Kotegaeshi"},
      { "Master of Kansetsuwaza",  "Mistress of Kansetsuwaza"},
      { "Master of Taisabaki",  "Mistress of Taisabaki"},
      { "Master of Kyusho",  "Mistress of Kyusho"},
      { "Student of Kamae",  "Student of Kamae"},
      { "Master of Kamae",  "Mistress of Kamae"},
      { "Master of Ukemi",  "Master of Ukemi"},
      { "Mokuroku",  "Mokuroku"},
      { "Ogoshi",  "Ogoshi"},
      { "Ippon Seinage",  "Ippon Seinage"},
      { "Koshi Garuma",  "Koshi Garuma"},
      { "Sukuinage",  "Sukuinage"},
      { "Osotogari",  "Osotogari"},
      { "Uki Goshi",  "Uki Goshi"},
      { "Taiotoshi",  "Taiotoshi"},
      { "Harai Goshi",  "Harai Goshi"},
      { "Yama Arashi",  "Yama Arashi"},
      { "Master of Nage Waza",  "Mistress of Nage Waza"},
      { "Entrance to Owaza",  "Entrance to Owaza"},
      { "Novice of Owaza",  "Novice of Owaza"},
      { "Student of Owaza",  "Student of Owaza"},
      { "Learned of Owaza",  "Learned of Owaza"},
      { "Master of Owaza",  "Mistress of Owaza"},
      { "Menkyo",  "Menkyo"},
      { "Sensei",  "Sensei"},
      { "Shinan",  "Shinan"},
      { "Shihan",  "Shihan"},
      { "Kaiden",  "Kaiden"},
      { "Master of the Miyama Ryu",  "Mistress of the Miyama Ryu"},
      { "Assassin Hero",  "Assassin Heroine"},

	{ "Assassin Avatar",	"Assassin Avatar"		},
	{ "Assassin Angel",		"Assassin Angel"		},
	{ "Assassin Demigod",		"Assassin Demigoddess"		},
	{ "Immortal Assassin",	"Immortal Assassin"		},
	{ "Assassin God",		"Assassin God"			},
	{ "Assassin Deity",		"Assassin Deity" 		},
	{ "Supreme Assassin",	"Supreme Assassin"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },


/* bard */
     {
{      "Man", "Woman"         },

{      "Apprentice Minstrel" ,"Apprentice Minstrel" },
{      "Juggler" ,"Juggler"},
{      "Jester" ,"Jester"}, 
{      "Dancer" ,"Dancer"},
{      "Caroler" ,"Caroler" },

{      "Drummer" ,"Drummer"},
{      "Piper" ,"Piper"},
{      "Fiddler" ,"Fiddler"},
{      "Chanter" ,"Chanter"},
{      "Troubadour" ,"Troubadour"},
       
{      "Flutist" , "Flutist"},
{      "Harpist" ,"Harpist"},
{      "Singer" ,"Singer"},
{      "Soloist" ,"Soloist"},
{      "Instrumentalist" ,"Instrumentalist"},
       
{      "Rhymer" , "Rhymer"},
{      "Poet" ,"Poet"},
{      "Melody Maker" ,"Melody Maker"},
{      "Balladeer" ,"Balladeer"},
{      "Musician" ,  "Musician"},
       
{      "Master Singer" ,"Master Singer"},
{      "Master Instrumentalist" ,"Master Instrumentalist"},
{      "Master Rhymer" , "Master Rhymer"},
{      "Master Poet" ,"Master Poet"},
{      "Composer" ,  "Composer"},
       
{      "Lyracist" ,"Lyracist"},
{      "Elegist" ,"Elegist"},
{      "Hymnist" , "Hymnist"},
{      "Songsmith" ,"Songsmith"},
{      "Minstrel" ,"Minstrel"},

{      "Master Minstrel" , "Master Minstrel"},
{      "Master of Instruments" , "Master of Instruments"},
{      "Master of Words" ,"Master of Words"},
{      "Master of Notes" ,"Master of Notes"},
{      "Master of Melody" , "Master of Melody"},

{      "Master of Song" ,"Master of Song"},
{      "Master of Verse" , "Master of Verse"},
{      "Master of Harmony" ,"Master of Harmony"},
{      "Master of Euphony" ,"Master of Euphony"},
{      "Bard" ,"Bard"},

{      "Master Poet" ,"Master Poet"},
{      "Master Melody Maker" ,"Master Melody Maker"},
{      "Master Composer" , "Master Composer"},
{      "Master Songsmith" , "Master Songsmith"},
{      "Student of History" , "Student of History"},
       
{      "Historian" ,"Historian"},
{      "Master Historian" , "Master Historian"},
{      "Chronicler" ,"Chronicler" },
{      "Master Chronicler","Master Chronicler" },
{      "Master Bard" , "Master Bard" },

     {      "Bard Hero","Bard Hero"},
	{ "Avatar",		"Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}

   },
/* maligner */
     {
{      "Man", "Woman"         },

{"Apprentice of Black Magic","Apprentice of Black Magic"},
{"Dark Spell Student","Dark Spell Student"},
{"Scholar of Black Magic","Scholar of Black Magic"},
{"Delver in Dark Spells","Delver in Dark Spells"},
{"Medium of Black Magic","Medium of Black Magic"},

{"Evil Caster","Evil Caster"},
{"Tainted","Tainted"},
{"Dark Apprentice","Dark Apprentice"},
{"Wretched Illusionist","Wretched Illusionist"},
{"Nefarious Abjurer","Nefarious Abjurer"},

{"Depraved Conjurer","Depraved Conjurer"},
{"Iniquitous Mage","Iniquitous Magess"},
{"Obscene Creator","Obscene Creator"},
{"Nauseating","Nauseating"},
{"Corpse Mangler","Corpse Mangler"},

{"Apprentice Warlock","Apprentice Warlock"},
{"Warlock","Witch"},
{"Corrupt Sorceror","Corrupt Sorceress"},
{"Black Sorceror","Black Sorceress"},
{"Necrotic","Necrotic"},

{"Apprentice Golem Maker","Apprentice Golem Maker"},
{"Golem Maker","Golem Maker"},
{"Creator of Disease","Creator of Disease"},
{"Bringer of Plague","Bringer of Plague"},
{"Apprentice Animator","Apprentice Animator"},

{"Animator","Animator"},
{"Master Animator","Master Animator"},
{"Apprentice Animator","Apprentice Animator"},
{"Assembler","Assembler"},
{"Master Assembler","Master Assembler"},

{"Demon Summoner","Demon Summoner"},
{"Greater Demon Summoner","Greater Demon Summoner"},
{"Greater Golem Maker","Greater Golem Maker"},
{"Master Golem Maker","Master Golem Maker"},
{"Abomination","Abomination"},

{"Malefic","Malefic"},
{"Infernal","Infernal"},
{"Malevolent","Malevolent"},
{"Scourge","Scourge"},
{"Occultist Disciple","Occultist Disciple"},

{"Learned Occultist","Learned Occultist"},
{"Master of the Occult","Master of the Occult"},
{"Learned of Black Magic","Learned of Black Magic"},
{"Master of Black Magic","Master of Black Magic"},
{"Master of Necromancy","Master of Necromancy"},

{"Master of the Undead","Master of the Undead"},
{"Warlock of the Spectre","Witch of the Spectre"},
{"Warlock of the Wraith","Witch of the Wraith"},
{"Warlock of the Lich","Witch of the Lich"},
{"Lord of the Lich","Lady of the Lich"},

{"Hero of Death","Hero of Death"},
	{ "Avatar",		"Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
     { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}

   },

/* alchemist */
      {
{      "Man", "Woman"         },

{"Apprentice of Magic","Apprentice of Magic"},
{"Spell Student","Spell Student"},
{"Scholar of Magic","Scholar of Magic"},
{"Delver in Spells","Delver in Spells"},
{"Medium of Magic","Medium of Magic"},
{"Recorder of Magic","Recorder of Magic"},
{"Advanced Recorder of Magic","Advanced Recorder of Magic"},
{"Sage's Assistant","Sage's Assistant"},
{"Creator of Illusions","Creator of Illusions"},
{"Intent Abjurer","Intent Abjurer"},
{"Diligent Conjurer","Diligent Conjurer"},
{"Assiduous Magician","Assiduous Magician"},
{"Creator of Lights","Creator of Lights"},
{"Alchemist's Assistant","Alchemist's Assistant"},
{"Tireless Magus","Tireless Magus"},
{"Busy Wizard","Busy Wizard"},
{"Erudite","Erudite"},
{"Innovator","Innovator"},
{"Inventor","Inventor"},
{"Transmuter","Transmuter"},
{"Maker of Wands","Maker of Wands"},
{"Maker of Staves","Maker of Staves"},
{"Maker of Scrolls","Maker of Scrolls"},
{"Maker of Potions","Maker of Potions"},
{"Craftsman","Craftsman"},
{"Student of Iota","Student of Iota"},
{"Student of Light","Student of Light"},
{"Student of Form","Student of Form"},
{"Student of Charm","Student of Charm"},
{"Student of Beguilement","Student of Beguilement"},
{"Student of Time","Student of Time"},
{"Student of Space","Student of Space"},
{"Student of Alteration","Student of Alteration"},
{"Alchemist","Alchemist"},
{"Master of Iota","Master of Iota"},
{"Master of Light","Master of Light"},
{"Master of Form","Master of Form"},
{"Master Charmer","Master Charmer"},
{"Master of Beguilement","Master of Beguilement"},
{"Master of the Past","Master of the Past"},
{"Master of the Present","Master of the Present"},
{"Master of the Future","Master of the Future"},
{"Master of Time","Master of Time"},
{"Master of Space","Master of Space"},
{"Master of Alteration","Master of Alteration"},
{"Master of Transference","Master of Transference"},
{"Master of Mutation","Master of Mutation"},
{"Alchemist of Changelings","Alchemist of Changelings"},
{"Alchemist Shapeshifters","Alchemist Shapeshifters"},
{"Elder Alchemist","Elder Alchemist"},

     { "Hero of Alteration" , "Hero of Alteration" }, 
	{ "Avatar",		"Avatar"		},
	{ "Angel",			"Angel"			},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"		},
	{ "God",			"Goddess"		},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",          "Creator"   },
	{ "Implementor",		"Implementress"			}

   },
/* druid */
      {
{      "Man", "Woman"         },

{"Apprentice of the Forest","Apprentice of the Forest"},
{"Student of the Green","Student of the Green"},
{"Scholar of Nature","Scholar of Nature"},
{"Delver in Nature","Delver in Nature"},
{"Medium of Nature","Medium of Nature"},
{"Recorder of Nature","Recorder of Nature"},
{"Advanced Recorder of Nature","Advanced Recorder of Nature"},
{"Sage's Assistant","Sage's Assistant"},
{"Creator of Illusions","Creator of Illusions"},
{"Intent Abjurer","Intent Abjurer"},
{"Diligent Conjurer","Diligent Conjurer"},
{"Assiduous Magician","Assiduous Magician"},
{"Creator of Lights","Creator of Lights"},
{"Alchemist's Assistant","Alchemist's Assistant"},
{"Tireless Magus","Tireless Magus"},
{"Busy Wizard","Busy Wizard"},
{"Erudite","Erudite"},
{"Innovator","Innovator"},
{"Inventor","Inventor"},
{"Transmuter","Transmuter"},
{"Maker of Wands","Maker of Wands"},
{"Maker of Staves","Maker of Staves"},
{"Maker of Scrolls","Maker of Scrolls"},
{"Maker of Potions","Maker of Potions"},
{"Craftsman","Craftsman"},
{"Student of Iota","Student of Iota"},
{"Student of Light","Student of Light"},
{"Student of Form","Student of Form"},
{"Student of Charm","Student of Charm"},
{"Student of Beguilement","Student of Beguilement"},
{"Student of Time","Student of Time"},
{"Student of Space","Student of Space"},
{"Student of Alteration","Student of Alteration"},
{"Alchemist","Alchemist"},
{"Master of Iota","Master of Iota"},
{"Master of Light","Master of Light"},
{"Master of Form","Master of Form"},
{"Master Charmer","Master Charmer"},
{"Master of Beguilement","Master of Beguilement"},
{"Master of the Past","Master of the Past"},
{"Master of the Present","Master of the Present"},
{"Master of the Future","Master of the Future"},
{"Master of Time","Master of Time"},
{"Master of Space","Master of Space"},
{"Master of Alteration","Master of Alteration"},
{"Master of Transference","Master of Transference"},
{"Master of Mutation","Master of Mutation"},
{"Alchemist of Changelings","Alchemist of Changelings"},
{"Alchemist Shapeshifters","Alchemist Shapeshifters"},
{"Elder Alchemist","Elder Alchemist"},

     { "Hero of Alteration" , "Hero of Alteration" }, 
	{ "Avatar",		"Avatar"		},
	{ "Angel",			"Angel"			},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"		},
	{ "God",			"Goddess"		},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",          "Creator"   },
	{ "Implementor",		"Implementress"			}

   },
/* monk */
      {
{      "Man", "Woman"         },

{"Novice"," Novice"},
{"Understudy","  Understudy"},
{"student of Chen Chi"," student of Chen Chi"},
{"Student of Ti Chi"," Student of Ti Chi"},
{"Chic Tzu"," Chic Tzu"},
{"Tong Jen"," Tong Jen"},
{"student of Wei Chi"," student of Wei Chi"},
{"Chun Tzu"," Chun Tzu"},
{"Shih"," Shih"},
{"Bald Head"," Bald Head"},
{"understudy to  Badger"," understudy to  Badger"},
{"student of  World"," student of  World"},
{"Jang Tzu"," Jang Tzu"},
{"Gua Shi"," Gua Shi"},
{"student of Monkey"," student of Monkey"},
{"Wu Kung"," Wu Kung"},
{"Student of the Snake","The student of the Snake"},
{"humble student"," humble student"},
{"Kung Tzu"," Kung Tzu"},
{"Of  Red Robe","Of  Red Robe"},
{"understudy of Monkey"," understudy of Monkey"},
{"Sheng Shen"," Sheng Shen"},
{"Hou Shi"," Hou Shi"},
{"master of Pao Chi"," master of Pao Chi"},
{"student of  Tiger"," student of  Tiger"},
{"humble understudy"," humble understudy"},
{"Shan Tzu"," Shan Tzu"},
{"Of  Yellow Robe","Of  Yellow Robe"},
{"understudy of  Tiger"," understudy of  Tiger"},
{"Chien Lao Shih"," Chien Lao Shih"},
{"Chien Shih"," Chien Shih"},
{"Hu Shih"," Hu Shih"},
{"master of Wai Chia"," master of Wai Chia"},
{"Student of Alteration","Student of Alteration"},
{"humble teacher","The humble teacher"},
{"Niu Tzu","The Niu Tzu"},
{"Of the Orange Robe","Of the Orange Robe"},
{"Understudy of the Snake","Understudy of the Snake"},
{"Tao Lao Shih","Tao Lao Shih"},
{"Shen Shi","The Shen Shi"},
{"Daster of Tien Hsueh","The master of Tien Hsueh"},
{"humble master","The humble master"},
{"Gui Tzu","The Gui Tzu"},
{"master of Xian Chi","The master of Xian Chi"},
{"Long Tzu","The Long Tzu"},
{"understudy to Immortals","The understudy to Immortals"},
{"High Abbot","The High Abbot"},
{"Master of Mutation","Master of Mutation"},
{"Hsien Shi","The Hsien Shi"},
{"Shining Abbot","The Shining Abbot"},

     { "Hero of Alteration" , "Hero of Alteration" }, 
	{ "Avatar",		"Avatar"		},
	{ "Angel",			"Angel"			},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"		},
	{ "God",			"Goddess"		},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",          "Creator"   },
	{ "Implementor",		"Implementress"			}

   },
/* necromancer */
      {
{      "Man", "Woman"         },

{"Student of Negativity","Student of Negativity"},
{"Student of Disease","Student of Disease"},
{"Student of Pain","Student of Pain"},
{"Student of the Shadow","Student of the Shadow"},
{"Student of the Dark","Student of the Dark"},
{"Student of Death","Student of Death"},
{"Student of the Paranormal","Student of the Paranormal"},
{"Student of the Seance"," Student of the Seance"},
{"Student of Possession","Student of Possession"},
{"Distraught Student","the Distraught Student"},
{"Acolyte of Disease","Acolyte of Disease"},
{"Acolyte of Pain","Acolyte of Pain"},
{"Acolyte of the Shadow","Acolyte of the Shadow"},
{"Acolyte of the Dark","Acolyte of the Dark"},
{"Acolyte of Death","Acolyte of Death"},
{"Acolyte of Possession","Acolyte of Possession"},
{"Acolyte of the Forsaken","Acolyte of the Forsaken"},
{"Acolyte of the Damned","Acolyte of the Damned"},
{"Disciple of Pain","Disciple of Pain"},
{"Disciple of the Shadow","Disciple of the Shadow"},
{"Disciple of the Dark","Disciple of the Dark"},
{"Disciple of Death","Disciple of Death"},
{"Disciple of Possession","Disciple of Possession"},
{"Disciple of the Forsaken","Disciple of the Forsaken"},
{"Disciple of the Damned","Disciple of the Damned"},
{"Master of the Shadow","Mistress of the Shadow"},
{"Master of the Dark","Mistress of the Dark"},
{"Master of Death","Mistress of Death"},
{"Master of Possession","Mistress of Possession"},
{"Master of the Forsaken","Mistress of the Forsaken"},
{"Master of the Damned","Mistress of the Damned"},
{"Shadow Adept","Shadow Adept"},
{"Dark Adept","Dark Adept"},
{"Adept of Death","Adept of Death"},
{"Forsaken Adept","Forsaken Adept"},
{"Damned Adept","Damned Adept"},
{"Student of Necromancy","Student of Necromancy"},
{"Acolyte of Necromancy","Acolyte of Necromancy"},
{"Disciple of Necromancy","Disciple of Necromancy"},
{"Master of Necromancy","Mistress of Necromancy"},
{"Adept of Necromancy","Adept of Necromancy"},
{"Bane of the Decent","Bane of the Decent"},
{"Bane of the Good","Bane of the Good"},
{"Bane of the Light","Bane of the Light"},
{"Wielder of the Shadow","Wielder of the Shadow"},
{"Wielder of Pain","Wielder of Pain"},
{"Wielder of the Dark","Wielder of the Dark"},
{"Wielder of Death","Wielder of Death"},
{"Wielder of Necrolisis","Wielder of Necrolisis"},
{"Adept of the Craft","The Adept of the Craft"},

     { "Hero of Alteration" , "Hero of Alteration" }, 
	{ "Avatar",		"Avatar"		},
	{ "Angel",			"Angel"			},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"		},
	{ "God",			"Goddess"		},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",          "Creator"   },
	{ "Implementor",		"Implementress"			}

   }
};

/* Leader's Name, name of religion, tattoo vnum */
const struct religion_type religion_table [] =
{
  { "", "None", 0 },
/*
  { "Ta'aal", "life",		OBJ_VNUM_MARK_LIFE },
  { "Ta'aal", "creation",	OBJ_VNUM_MARK_CREATION },

  { "Tarah'el", "order",		OBJ_VNUM_MARK_ORDER },
  { "Tarah'el", "peace",		OBJ_VNUM_MARK_PEACE },

  { "Miyara", "miracles",		OBJ_VNUM_MARK_MIRACLES  },
  { "Miyara", "wizardry",		OBJ_VNUM_MARK_WIZARDRY },

  { "Kazmir", "leadership",	OBJ_VNUM_MARK_LEADERSHIP },
  { "Kazmir", "Valor",		OBJ_VNUM_MARK_VALOR },

  { "Tambrael", "nature",		OBJ_VNUM_MARK_NATURE },
  { "Tambrael", "balance",	OBJ_VNUM_MARK_BALANCE },

  { "Anizius", "wisdom",		OBJ_VNUM_MARK_WISDOM },
  { "Anizius", "knowledge",	OBJ_VNUM_MARK_KNOWLEDGE },

  { "Oyajin", "silence",	OBJ_VNUM_MARK_SILENCE },
  { "Oyajin", "seclusion",		OBJ_VNUM_MARK_SECLUSION },
  
  { "Alusazc", "death",		OBJ_VNUM_MARK_DEATH },
  { "Alusazc", "destruction",	OBJ_VNUM_MARK_DESTRUCTION },
  
  { "Ehraat", "deception",	OBJ_VNUM_MARK_DECEPTION },
  { "Ehraat", "hate",		OBJ_VNUM_MARK_HATE },
  
  { "Kazmir", "chaos",		OBJ_VNUM_MARK_CHAOS },
  { "Kazmir", "war",		OBJ_VNUM_MARK_WAR },

  { "Hsahe", "honor",		OBJ_VNUM_MARK_HONOR },
  { "Hsahe", "quests",		OBJ_VNUM_MARK_QUESTS },

*/
};



/*  this was commented out with the tattoo code */
const struct ethos_type ethos_table [] =
{
  { "Null" },
  { "Lawful" },
  { "Neutral" },
  { "Chaotic" }
};


/*  altar good neut evil, recall good neut evil, pit good neut evil */
const struct hometown_type hometown_table [] =
{
  { "Sanaria", {3370, 3054, 3372}, {3368, 3001, 3371}, {3369,3054,3372} },
  { "New Thalos", {9605, 9605, 9605}, {9609,9609,9609}, {9609,9609,9609} },
  { "Rivendell", {1800, 1800, 1800}, {1800,1800,1800}, {1818,1818,1818} },
  { "Ofcol", {669, 669, 669},       {698, 698, 698},   {669, 669, 669} },
  { "Arkham", {700, 700, 700},  {700, 700, 700}, {700, 700, 700}}, 
  { "Gondor city", {4236, 4236, 4236},{4251, 4251, 4251},{4251, 4251, 4251} }
  
};

/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[31]		=
{
    { -5, -5,   0,  0 },  /* 0  */
    { -5, -5,   3,  1 },  /* 1  */
    { -3, -5,   3,  2 },
    { -4, -5,  10,  3 },  /* 3  */
    { -2, -5,  25,  4 },
    { -2, -4,  55,  5 },  /* 5  */
    { -1, -4,  80,  6 },
    { -1, -3,  90,  7 },
    {  0, -2, 100,  8 },
    {  0, -1, 100,  9 },
    {  0, -1, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 130, 12 },
    {  0,  0, 130, 13 }, /* 13  */
    {  0,  1, 140, 14 },
    {  1,  1, 150, 15 }, /* 15  */
    {  1,  2, 165, 16 },
    {  2,  3, 180, 22 },
    {  2,  3, 200, 25 }, /* 18  */
    {  3,  4, 225, 30 },
    {  3,  5, 250, 40 }, /* 20  */
    {  4,  6, 300, 45 },
    {  4,  6, 350, 55 },
    {  8,  10,450, 65 },
    {  10, 12,550, 75 },
    {  18, 24,650, 85 },  /* 25   */
    {  18, 24,650, 85 },  /* 26   */
    {  18, 24,650, 85 },  /* 27   */
    {  18, 24,650, 85 },  /* 28   */
    {  18, 24,650, 85 },  /* 29   */            
    {  18, 24,650, 85 }  /* 30   */    
        
};



const	struct	int_app_type	int_app		[31]		=
{
    {  3,	-70 },	/*  0 */
    {  5,	-60 },	/*  1 */
    {  7,	-50 },
    {  8,	-40 },	/*  3 */
    {  9,	-35 },
    { 10,	-30 },	/*  5 */
    { 11,	-24 },
    { 12,	-22 },
    { 13,	-18 },
    { 15,	-14 },
    { 17,	-10 },	/* 10 */
    { 19,	-8 },
    { 22,	-6 },
    { 25,	-4 },
    { 28,	1 },
    { 31,	0 },	/* 15 */
    { 34,	1 },
    { 37,	2 },
    { 40,	4 },	/* 18 */
    { 44,	6 },
    { 49,	8 },	/* 20 */
    { 55,	10 },
    { 60,	12 },
    { 70,	14 },
    { 80,	16 },
    { 85,	20 },	/* 25 */
    { 85,	25 },	/* 26 */
    { 85,	30 },	/* 27 */
    { 85,	40 },	/* 28 */
    { 85,	50 },	/* 29 */
    { 85,	60 }	/* 30 */                
};



const	struct	wis_app_type	wis_app		[31]		=
{
    { 0,	80 },	/*  0 */
    { 0 ,	70},	/*  1 */
    { 0,	60 },
    { 0,	50 },	/*  3 */
    { 0,	40 },
    { 1,	35 },	/*  5 */
    { 1,	30 },
    { 1,	24 },
    { 1,	22 },
    { 1,	18 },
    { 1,	14 },	/* 10 */
    { 1,	10 },
    { 1,	8 },
    { 1,	4 },
    { 1 ,	1},
    { 2,	-0 },	/* 15 */
    { 2,	-1 },
    { 2,	-2 },
    { 3,	-4 },	/* 18 */
    { 3,	-6 },
    { 3,	-8 },	/* 20 */
    { 3,	-10 },
    { 4,	-12 },
    { 4,	-14 },
    { 4,	-16 },
    { 5,	-20 },	/* 25 */
    { 5,	-25 },	/* 26 */
    { 5,	-30 },	/* 27 */
    { 5,	-40 },	/* 28 */
    { 5,	-50 },	/* 29 */
    { 5,	-60 }	/* 30 */

};



const	struct	dex_app_type	dex_app		[31]		=
{
    {   60 ,0},   /* 0 */
    {   50 ,0},   /* 1 */
    {   50 ,0},
    {   40 ,0},
    {   30 ,0},
    {   20 ,0},   /* 5 */
    {   10 ,0},
    {    0 ,0},
    {    0 ,0},
    {    0 ,1},
    {    0 ,2},   /* 10 */
    {    0 ,3},
    {    0 ,4},
    {    0 ,5},
    {    0 ,6},
    { - 10 ,7},   /* 15 */
    { - 15 ,8},
    { - 20 ,9},
    { - 30 ,11},
    { - 40 ,13},
    { - 50 ,15},   /* 20 */
    { - 60 ,17},
    { - 75 ,19},
    { - 90 ,21},
    { -105 ,23},
    { -120 ,25},    /* 25 */
    { -130 ,27},    /* 26 */
    { -140 ,29},    /* 27 */
    { -150 ,31},    /* 28 */
    { -160 ,35},    /* 29 */
    { -200 ,45}    /* 30 */

};


const	struct	con_app_type	con_app		[31]		=
{
    { -4, 20, 10 },   /*  0 */
    { -3, 25, 10 },   /*  1 */
    { -2, 30, 10 },
    { -2, 35, 10 },	  /*  3 */
    { -1, 40, 10 },
    { -1, 45, 10 },   /*  5 */
    { -1, 50, 10 },
    {  0, 55, 10 },
    {  0, 60, 10 },
    {  0, 65, 10 },
    {  0, 70, 11 },   /* 10 */
    {  0, 75, 11 },
    {  0, 80, 12 },
    {  0, 85, 12 },
    {  0, 88, 13 },
    {  1, 90, 13 },   /* 15 */
    {  2, 95, 14 },
    {  2, 97, 14 },
    {  3, 99, 15 },   /* 18 */
    {  3, 99, 15 },
    {  4, 99, 16 },   /* 20 */
    {  4, 99, 17 },
    {  5, 99, 18 },
    {  6, 99, 19 },
    {  10, 99, 20 },
    {  18, 99, 22 },    /* 25 */
    {  18, 99, 22 },    /* 26 */
    {  19, 99, 22 },    /* 27 */
    {  20, 99, 22 },    /* 28 */
    {  21, 99, 22 },    /* 29 */
    {  22, 99, 22 }    /* 30 */

};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	},
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   },
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

/*  took out the const that was at start of this line, for class code */
struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */
/* 1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  1,  2,  3,  4,  5,  6,  7,  8*/
/*war/mer/dfn/dfl/thi/inq/spe/cle/pro/mag/ran/asn/brd/mal/alc/dru/mok/nec*/
    {
	"reserved",		
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1,1,1,1,1},spell_null,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			"",		"", KINGDOM_NONE,GROUP_NONE
    },
    {
        "firewind",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 45,50,50,50 ,1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 9, 9 ,9, 9, 9, 9, 6, 9, 9, 9, 7, 7, 7 ,7, 8},
        {0,1,0,0,0},spell_firewind,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(901),33,12,
        "flaming winds","!Firewind!","",KINGDOM_NONE,GROUP_FIRE
    },
    {
        "Spectral fist",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 45,50,50,50 ,1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 9, 9 ,9, 9, 9, 9, 6, 9, 9, 9, 7, 7, 7 ,7, 7},
        {0,0,0,0,0},spell_spectral_fist,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(901),33,15,
        "Spectral fist","Spectral fist","",KINGDOM_NONE,GROUP_FIRE
    },
    {
        "meteor swarm",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 9, 9, 9, 9, 9, 9, 6, 9, 9, 9, 6, 6, 7 ,7, 9},
        {0,1,0,0,0},spell_meteor_swarm,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(1144),33,64,
        "meteor swarm","Your start to recover from the blast.",
	"", KINGDOM_NONE,GROUP_LIGHTNING
    },
    {
        "multi missile",
       /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 9,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 9, 9, 8, 8, 8, 8, 8, 8},
        {0,1,0,0,0},spell_multi_missile,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(903),25,12,
        "magic missile","!Missile!","",KINGDOM_NONE,GROUP_PRO_MAGIC
    },
    {
	"mend", 
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 9, 9, 9, 9, 9, 9, 9, 4, 9, 9, 9, 9, 9, 9, 9, 7, 7, 7},
	{0,0,0,0,0},spell_mend,TAR_CHAR_DEFENSIVE,POS_STANDING,
	NULL,SLOT(599),50,24,
	"","!Mend!","",KINGDOM_NONE,GROUP_CURATIVE 
    },
    {
        "ice ray",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 33,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 9, 9, 9, 9, 9, 9, 6, 8, 9, 9, 7, 6, 6, 8, 6},
        {0,1,0,0,0},spell_ice_ray,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(904),20,12,
        "ice ray","The chill leaves your body.","",KINGDOM_NONE,GROUP_COLD
    },
    {
        "ice storm",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/        
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 9, 9, 9, 7, 9, 7, 6, 8, 9, 9, 8, 8, 6, 8, 6},
        {0,0,0,0,0},spell_ice_storm,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(905),33,24,
        "ice storm","The chill leaves your body.",
	"", KINGDOM_NONE,GROUP_COLD
    },
    {
        "fire storm",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9, 9, 8, 9, 7, 7, 8, 7, 6, 8, 8, 8, 7, 7, 6, 8, 8},
        {0,0,0,0,0},spell_fire_storm,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(1140),33,24,
        "fire storm","The feel alot cooler.",
	"", KINGDOM_NONE,GROUP_FIRE
    },
    {
        "lightning storm",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9, 9, 8, 9, 7, 7, 8, 7, 6, 8, 8, 8, 7, 7, 7, 9, 8},
        {0,0,0,0,0},spell_lightning_storm,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(1142),33,24,
        "lightning storm","Your muscle starts to response.",
	"", KINGDOM_NONE,GROUP_LIGHTNING
    },
    {
        "divine word",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 8, 9, 7, 9, 5, 9, 8, 8, 9, 9, 8, 8, 6, 6, 7},
        {0,0,0,0,0},spell_divine_word,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(1144),33,32,
        "divine word","You start to get back to your feet.",
	"", KINGDOM_NONE,GROUP_BENEDICTIONS
    },
    {
        "vision",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 27,17,32,28 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 9, 9, 7, 7, 8, 8, 5, 9, 9, 8, 6, 7, 7, 7, 7},
        {0,1,0,0,0},spell_vision,TAR_IGNORE,POS_FIGHTING,
        NULL,SLOT(906),40,20,
        "","!Vision!","",KINGDOM_NONE,GROUP_DETECTION
    },
    {
        "restoration",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 50,45,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 9, 9, 7, 9, 6, 9, 7, 9, 9, 9, 7, 9, 8, 8, 8},
        {0,0,0,0,0},spell_restoration,TAR_CHAR_DEFENSIVE,POS_FIGHTING,
        NULL,SLOT(907),75,12,
        "","!Restore!","",KINGDOM_NONE,GROUP_HEALING
    },


    {
        "destroy cursed",
       /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9, 9, 8, 9, 9, 7, 8, 5, 7, 7, 8, 9, 9, 7, 7, 7, 9, 8},
        {0,0,0,0,0},spell_destroy_cursed,TAR_CHAR_DEFENSIVE,POS_STANDING,
        NULL,SLOT(908),20,12,
        "","!destroy cursed!","",KINGDOM_NONE,GROUP_CURATIVE
    },
   {
       "remove alignment",
       /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
       {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9, 9, 9, 9, 9, 9, 7, 9, 8, 7, 9, 9, 8, 8, 4, 9, 9, 9},
       {0,0,0,0,0},spell_remove_alignment,TAR_OBJ_INV,POS_STANDING,
       NULL,SLOT(909),10,12,
       "","!Remove Alignment!","",KINGDOM_NONE,GROUP_ENCHANTMENT
   },

    /*
     * Psionicist spell/skill definitions
     * Psi class conceived,created,and coded by Thelonius (EnvyMud)
     */

    {
        "adrenaline control",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 8, 9, 7, 7, 9, 6, 6, 9, 7, 5, 7, 7, 8, 6, 7},
        {1,0,1,0,0},spell_adrenaline_control,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(910),6,12,
        "","The adrenaline rush wears off.","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "agitation",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 9, 9, 6, 6, 6, 6, 5, 9, 9, 7, 5, 6, 6, 6, 6},
        {0,0,0,0,0},spell_agitation,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(911),10,12,
        "molecular agitation","!Agitation!","",KINGDOM_NONE,GROUP_MIND
    },


    {
        "awe",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {0,0,0,0,0},spell_awe,TAR_CHAR_DEFENSIVE,POS_FIGHTING,
        NULL,SLOT(913),35,12,
        "","!Awe!","",KINGDOM_NONE,GROUP_BENEDICTIONS
    },
    
    {
	"shadow door",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 7},
	{0,0,0,0,0},spell_shadow_door,		TAR_OBJ_CHAR_DEF,	POS_FIGHTING,
	&gsn_shadow_door,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",
	"", KINGDOM_NONE, GROUP_ILLUSION
    },

    {
        "ballistic attack",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 8, 9, 8, 7, 8, 7, 5, 8, 9, 7, 5, 5, 7, 7, 7},
        {0,0,0,0,0},spell_ballistic_attack,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(914),5,12,
        "ballistic attack","!Ballistic Attack!","",
	KINGDOM_NONE,GROUP_PRO_MAGIC
    },


    {
        "cell adjustment",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
        {1,0,1,0,0},spell_cell_adjustment,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(916),8,12,
        "","!Cell Adjustment!","",KINGDOM_NONE,GROUP_MIND
    },


    {
        "combat mind",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8, 6, 6},
        {0,0,0,0,0},spell_combat_mind,TAR_CHAR_DEFENSIVE,POS_STANDING,
        NULL,SLOT(918),15,12,
        "","Your battle sense has faded.","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "complete healing",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,28, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 8, 9, 8, 9, 7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_complete_healing,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(919),300,12,
        "","!Complete Healing!","",KINGDOM_NONE,GROUP_HEALING
    },

    {
        "control flames",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7},
        {0,0,0,0,0},spell_control_flames,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(920),15,12,
        "tongue of flame","!Control Flames!","",KINGDOM_NONE,GROUP_FIRE
    },


    {
        "death field",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 6, 7, 8, 9, 6},
        {0,0,0,0,0},spell_death_field,TAR_IGNORE,POS_FIGHTING,
        NULL,SLOT(922),100,18,
        "field of death","!Death Field!","",KINGDOM_NONE,GROUP_NECRO
    },

    {
        "detonate",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 5, 6, 7, 9, 8},
        {1,0,1,0,0},spell_detonate,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(923),35,24,
        "detonation","!Detonate!","",KINGDOM_NONE,GROUP_ATTACK
    },


    {
        "displacement",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 7, 8, 7},
        {0,0,1,0,0},spell_displacement,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(925),10,12,
        "","You are no longer displaced.","",KINGDOM_NONE,GROUP_PROTECTIVE
    },


    {
        "ectoplasmic form",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,19, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 9, 9},
        {0,0,0,0,0},spell_ectoplasmic_form,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(927),20,12,
        "","You feel solid again.","",KINGDOM_NONE,GROUP_ILLUSION
    },

    {
        "ego whip",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,13, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 9, 7, 7},
        {0,0,0,0,0},spell_ego_whip,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(928),20,12,
        "","You feel more confident.","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "energy containment",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 8},
        {1,0,1,0,0},spell_energy_containment,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(929),10,12,
        "","You no longer absorb energy.","",KINGDOM_NONE,GROUP_ATTACK
    },


    {
        "enhanced strength",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7},
        {1,1,1,0,0},spell_enhanced_strength,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(931),20,12,
        "","You no longer feel so HUGE.","",KINGDOM_NONE,GROUP_ENHANCEMENT
    },

    {
        "flesh armor",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 9, 7},
        {1,0,1,0,0},spell_flesh_armor,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(932),15,12,
        "","Your skin returns to normal.","",KINGDOM_NONE,GROUP_PROTECTIVE
    },


    {
        "inflict pain",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8, 8, 8},
        {0,0,0,0,0},spell_inflict_pain,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(935),10,12,
        "mindpower","!Inflict Pain!","",KINGDOM_NONE,GROUP_ATTACK
    },

    {
        "intellect fortress",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,12, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 9, 7, 7},
        {0,0,0,0,0},spell_intellect_fortress,TAR_IGNORE,POS_STANDING,
        NULL,SLOT(936),25,24,
        "","Your intellectual fortress crumbles.","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "lend health",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 7, 9, 8, 6, 6, 6, 5, 6, 6, 6, 5, 6, 8, 8, 7},
        {0,0,0,0,0},spell_lend_health,TAR_CHAR_DEFENSIVE,POS_FIGHTING,
        NULL,SLOT(937),10,12,
        "","!Lend Health!","",KINGDOM_NONE,GROUP_HEALING
    },

    {
        "levitation",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 7, 9, 8, 6, 6, 6, 4, 6, 6, 6, 5, 4, 6, 7, 6},
        {1,1,0,0,0},spell_levitation,TAR_CHAR_DEFENSIVE,POS_STANDING,
        NULL,SLOT(938),10,18,
        "","You slowly float to the ground.","",KINGDOM_NONE,GROUP_PROTECTIVE
    },

    {
        "mental barrier",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 7, 9, 8, 6, 4, 6, 5, 6, 6, 6, 5, 6, 8, 6, 6},
        {0,0,0,0,0},spell_mental_barrier,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(939),8,12,
        "","Your mental barrier breaks down.","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "mind thrust",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 7, 9, 8, 6, 6, 6, 5, 6, 6, 6, 5, 6, 8, 8, 8},
        {0,0,0,0,0},spell_mind_thrust,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(940),8,12,
        "mind thrust","!Mind Thrust!","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "project force",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7, 7, 9, 8, 6, 6, 6, 5, 7, 8, 8, 5, 6, 8, 8, 8},
        {0,0,0,0,0},spell_project_force,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(941),18,12,
        "projected force","!Project Force!","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "psionic blast",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 7, 8, 7},
        {0,0,0,0,0},spell_psionic_blast,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(942),25,12,
        "psionic blast","!Psionic Blast!","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "psychic crush",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 7, 7, 6},
        {0,0,0,0,0},spell_psychic_crush,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(943),15,18,
        "psychic crush","!Psychic Crush!","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "psychic drain",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 8, 7, 7},
        {0,0,0,0,0},spell_psychic_drain,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(944),20,12,
        "","You no longer feel drained.","",KINGDOM_NONE,GROUP_MIND
    },

    {
        "psychic healing",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 7, 7, 7},
        {0,0,0,0,0},spell_psychic_healing,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(945),20,12,
        "","!Psychic Healing!","",KINGDOM_NONE,GROUP_HEALING
    },


    {
        "share strength",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1,11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 7, 7, 7},
        {0,1,0,0,0},spell_share_strength,TAR_CHAR_DEFENSIVE,POS_STANDING,
        NULL,SLOT(947),8,12,
        "","You no longer share strength with another.",
	"",KINGDOM_NONE,GROUP_HEALING
    },

    {
        "thought shield",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 8, 7, 7},
        {0,0,0,0,0},spell_thought_shield,TAR_CHAR_SELF,POS_STANDING,
        NULL,SLOT(948),5,12,
        "","You no longer feel so protected.",
	"",KINGDOM_NONE,GROUP_MIND
    },

    {
        "ultrablast",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 1, 1,25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 8, 8, 8},
        {0,0,0,0,0},spell_ultrablast,TAR_IGNORE,POS_FIGHTING,
        NULL,SLOT(949),75,18,
        "ultrablast","!Ultrablast!","",KINGDOM_NONE,GROUP_MIND
    },
	/* Necromantic spells by Maniac for Mythran Mud */

    {
	"animate dead2",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 7, 9, 7, 9, 5, 6, 6, 9, 9, 9, 6, 9, 7, 9, 4},
	{0,1,0,0,0},spell_animate_dead_2,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(950),	20,	12,
	"",			"You feel like you have the power to summon more zombies.",
	"",KINGDOM_NONE,GROUP_NECRO
    },
    
    {
	"raise shadelich",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 7, 9, 7, 9, 6, 6, 6, 9, 9, 9, 6, 9, 7, 9, 6},
	{0,0,0,0,0},spell_raise_shadelich,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(950),	100,	24,
	"",			"",
	"",KINGDOM_NONE,GROUP_NECRO
    },
    
    {
	"raise vampire",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9, 7, 9, 7, 9, 4, 6, 6, 9, 9, 9, 6, 9, 7, 9, 7},
	{0,0,0,0,0},spell_raise_vampire,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(950),	125,	24,
	"",			"",
	"",KINGDOM_NONE,GROUP_NECRO
    },

    {
	"rock flesh",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 1, 1, 1, 1,10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8, 7, 9, 7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 1, 1, 8},
	{0,0,0,0,0},spell_rock_flesh,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(951),	10,	10,
        "rock flesh",		"Your flesh turns back to normal.",
	"",KINGDOM_NONE,
	GROUP_PROTECTIVE
    },

	 {
	"animal essence",
	/* 1,  2,  3,  4, 5, 6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,103,103,103,1, 5, 2, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9, 9, 9, 9,  9,  9, 7, 5, 9, 9, 8, 9, 5, 8, 8},
	{0,0,0,0,0},spell_animal_essence,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(954),	50,	12,
	"",	"You feel your essence slip away.", 
	"", KINGDOM_NONE,GROUP_NATURE
	 },

	 {
	"drain blade",
	/* 1, 2, 3,  4,  5, 6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  5, 6,103,103,103,5,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 9,  8,  9, 7, 8,  9, 8, 7, 9, 9, 9, 8, 4, 8, 8, 7},
	{0,0,0,0,0},spell_drain_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(958),	150,	24,
	"","!Drain Blade!",	"$p looks less malevolant.",KINGDOM_NONE,
	GROUP_NECRO

	 },

	 {
	"empower blade",
	/* 1, 2,  3,  4,  5, 6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7,8*/
	{  1,103,103,103,103,5,103,103,4 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9,  9,  8,  9, 7, 8,  9, 8, 7, 9, 9, 9, 8, 4, 8, 9},
	{0,0,0,0,0},spell_empower_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(959),	100,	24,
	"","!Empower Blade!",	"$p looses its electric charge.",KINGDOM_NONE,
	GROUP_FIGHTMASTER
	 },
	 {
	"flame blade",
	/* 1, 2,  3,  4,  5, 6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  3,103,103,103,103,3,103,103,3 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9,  9,  8,  9, 7, 8,  9, 8, 7, 9, 9, 9, 8, 4, 8, 8, 7},
	{0,0,0,0,0},spell_flame_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(960),	100,	24,
	"","!Flame Blade!",	"$p looses its fiery glow.",KINGDOM_NONE,
	GROUP_ENHANCEMENT
	 },


	 {
	"frost blade",
	/* 1, 2,  3,  4,  5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  3,103,103,103,103,3, 3,103,3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9,  9,  8,  9, 7, 8, 9, 8, 7, 9, 9, 9, 8, 4, 7, 8, 8},
	{0,0,0,0,0},spell_frost_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(961),	100,	24,
	"",			"!Frost Blade!",	"$p warms back up.",
	KINGDOM_NONE,GROUP_ENHANCEMENT
	 },

	 {
	 "holy aura",
	 /* 1,  2, 3,  4, 5, 6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 { 103,50,103,103,6, 7,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9,  9, 6,  7, 9, 7, 7,  5,  6, 6, 8, 8, 6, 6, 6, 7, 8, 9},
	 {0,1,0,0,0},spell_holy_aura,TAR_CHAR_DEFENSIVE,POS_STANDING,
	 NULL,	SLOT(962),60,11,"",
	 "Your golden glow fades.",	 "$p's golden glow fades.",
	KINGDOM_NONE,GROUP_BENEDICTIONS
	 },

	 {
	 "illusions grandeur",
	 /* 1, 2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 {  5,103,103,103,103,103,103,103,5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9, 9,  8,  7,  9,  7,  7,  7, 6, 6, 8, 8, 6, 6, 6, 7, 8, 8},
	 {0,0,0,0,0},spell_illusions_granduer,TAR_CHAR_SELF,POS_STANDING,
	 NULL,	SLOT(963),75,20,"",
      "You do not feel grand anymore.","$p looks a little less grand.",
	KINGDOM_NONE,GROUP_ILLUSION
	 },

	 {
	 "improved phantasm",
	 /* 1, 2,  3,  4,  5,  6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 {  3,103,103,103,103,103,8,103,3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9, 9,  8,  7,  9,  7, 7, 7, 6, 6, 8, 8, 6, 6, 6, 7, 8, 8},
	 {0,0,0,0,0},spell_improved_phantasm,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
	 NULL,	SLOT(964),40,11,
	 "phantasmal forces","!Phantasmal Force!","",KINGDOM_NONE,
	GROUP_ILLUSION
	 },

	 {
	 "magic resistance",
	 /* 1, 2, 3,  4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 { 5, 6, 103,103,5, 9, 9,103,5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 9, 9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 8, 8},
	 {1,1,0,0,0},spell_magic_resistance,TAR_CHAR_SELF,POS_STANDING,
	 &gsn_magic_resistance,SLOT(965),30,24,
	 "","You are no longer protected from magic.","",KINGDOM_NONE,
	GROUP_DEFENSIVE
	 },

	 {
	 "phantasmal force",
	 /* 1, 2,  3,  4,  5,  6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 {  1,103,103,103,103,103,1,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9, 9,  8,  7,  9,  7, 7, 7, 7, 7, 8, 8, 7, 7, 7, 1, 1, 7},
	 {0,0,0,0,0},spell_phantasmal_force,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
	 NULL,	SLOT(966),15,11,
	 "phantasmal forces","!Phantasmal Force!","",KINGDOM_NONE,
	GROUP_ILLUSION
	 },

	 {
	 "protection cold",
	 /* 1, 2, 3,  4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 {  1, 1,103,103,1, 1, 1,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9, 9, 5,  7, 9, 9, 6, 5, 7, 5, 7, 8, 8, 6, 6, 4, 8, 5},
	 {0,1,0,0,0},spell_protection_cold,TAR_CHAR_SELF,POS_STANDING,
	 NULL,	SLOT(968),10,11,
      "",	"You are no longer protected from cold.","",KINGDOM_NONE,
	GROUP_PROTECTIVE
	 },

	 {
	 "protection fire",
	 /* 1, 2, 3,  4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 {  1, 1,103,103,1, 1, 1,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9, 9, 5,  7, 9, 9, 6, 5, 7, 5, 7, 8, 8, 6, 6, 5, 8, 6},
	 {0,1,0,0,0},spell_protection_fire,TAR_CHAR_SELF,POS_STANDING,
	 NULL,	SLOT(969),10,11,
	 "",	"You are no longer protected from fire.","",KINGDOM_NONE,
	GROUP_PROTECTIVE
	 },

	 {
	 "protection lightning",
	 /* 1, 2, 3,  4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 {  1, 1,103,103,1, 3, 3,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9, 9, 5,  7, 9, 9, 6, 5, 7, 5, 7, 8, 8, 6, 6, 3, 8, 6},
	 {0,1,0,0,0},spell_protection_lightning,TAR_CHAR_SELF,POS_STANDING,
	 NULL,	SLOT(970),10,11,
	 "",	"You are no longer protected from lightning.","",KINGDOM_NONE,
	GROUP_PROTECTIVE
      },

    {
	"poison",
	/* 1, 2, 3,  4,  5, 6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 17,12,103,103,103,6,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 7,  6,  9, 7, 6,  7, 6, 5, 7, 6, 7, 4, 6, 5, 8, 6},
	{1,1,1,0,0},spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick.",
	"The poison on $p dries up.", KINGDOM_NONE,
	GROUP_POISON
    },
         {
         "poison immunity",
         /* 1,  2,  3,  4, 5, 6, 7, 8 , 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         { 103,103,103,103,1,103,1,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9,  9,  7,  8, 9, 7, 7, 5,  6, 4, 6, 8, 9, 5, 7, 4, 7, 8},
         {1,0,1,0,0},spell_poison_immunity,TAR_CHAR_SELF,POS_STANDING,
         NULL,SLOT(971),20,11,
         "","You feel less vigorous.","",KINGDOM_NONE,
	GROUP_DEFENSIVE
         },
      {

	"rage",
	/* 1, 2, 3,  4,  5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 3,103,103,103,6, 5,103,4 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7, 7, 7,  7,  8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 7, 7},
	{1,0,0,0,0},spell_rage,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(973),	70,	36,
	"",	 "You feel less like killing.","",KINGDOM_NONE,
	GROUP_FIGHTMASTER
	},

	 {
	"rejuvinate",
	/* 1, 2, 3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,7,103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 8,  9,  9,  9,  9,  7,  9, 9, 9, 9, 9, 9, 9, 8, 9, 8},
	{0,0,0,0,0},spell_rejuvinate,TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(974),	300,	24,
	"",		"!Rejuvinate!","",KINGDOM_NONE,
	GROUP_HEALING
	 },
	 {
	 "resist poison",
	 /* 1,  2,  3,  4, 5, 6, 7, 8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	 { 103,103,103,103,1,103,1,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  8,  8,  6,  7, 8, 6, 6, 4,  5, 3, 5, 7, 8, 4, 6, 2, 8, 5},
	 {1,0,1,0,0},spell_resist_poison,TAR_CHAR_SELF,POS_STANDING,
	 NULL,SLOT(975),10,11,
	 "","You feel less vigorous.","",KINGDOM_NONE,
	GROUP_DEFENSIVE
	 },

	 {
	"rune edge",
	 /* 1,  2,  3,  4,  5, 6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/	
	 {  1, 103,103,103,103,1,103,103, 4 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9,  9,  9,  8,  9, 7, 8,  9,  9, 8, 9, 9, 9, 8, 5, 8, 9, 8},
	{0,0,0,0,0},spell_rune_edge,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(976),	200,	24,
	"","!Rune Edge!",	"$p becomes quite dull.",KINGDOM_NONE,
	GROUP_ENCHANTMENT
	 },

	 {
	"runic blade",
	/* 1,  2,  3,  4,  5, 6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7, 103,103,103,103,7,103,103, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9, 7, 8,  9,  9, 8, 9, 9, 9, 8, 5, 8, 9, 8},
	{0,0,0,0,0},spell_runic_blade,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(977),	225,	24,
	"","!Runic Blade!",	"$p becomes quite dull.",KINGDOM_NONE,
	GROUP_ENCHANTMENT
	 },

       {
        "cobble",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  7,  6,  9,  9, 7, 8, 8, 4, 7, 5, 8, 9, 7},
        {0,0,0,0,0},spell_cobble,TAR_OBJ_INV,POS_STANDING,
        NULL,SLOT(981),200,24,
        "","!Cobble!","",KINGDOM_NONE, GROUP_CREATION
        },
 {
        "polymorph",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  9,  8,  8,  6, 7, 7, 7, 8, 8, 7, 9, 9, 8},
        {1,1,1,0,0},spell_polymorph,        TAR_IGNORE,     POS_STANDING,
        NULL,                   SLOT(639),      250,    24,
        "",                     "You return to your own race.","",
        KINGDOM_NONE , GROUP_BENEDICTIONS
    },
   {
        "dragon breath",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/        
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  8,  8,  8,  8, 7, 8, 9, 8, 7, 7, 9, 9, 8},
        {0,0,0,0,0},spell_dragon_breath,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_dragon_breath,  SLOT(563),   75,   12,
        "blast of fire", "!dragon breath!", "", KINGDOM_NONE ,
        GROUP_NONE
    },
  {
        "enhanced armor",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/           
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  6,  7,  5,  7, 4, 7, 8, 8, 6, 4, 6, 8, 7},
        {0,0,0,0,0},spell_enhanced_armor,   TAR_CHAR_DEFENSIVE,     POS_FIGHTING,
        &gsn_enhanced_armor,    SLOT(583),      20,     12,
        "",     "You feel yourself unprotected.", "", KINGDOM_NONE ,
        GROUP_PROTECTIVE
    },
   {
        "meld into stone",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/  
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  6,  7,  5,  7, 4, 7, 8, 8, 6, 4, 4, 9, 8},
        {0,0,0,0,0},spell_meld_into_stone,  TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(584),      12,     18,
        "",     "The stones on your skin crumble into dust.",   "",
        KINGDOM_NONE ,  GROUP_PROTECTIVE
    },
   {
        "scream",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/             
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  7,  8,  9,  8, 7, 9, 9, 6, 8, 7, 8, 7, 7},
        {0,0,0,0,0},spell_scream,   TAR_IGNORE,     POS_FIGHTING,
        &gsn_scream,            SLOT(578),      200,    24,
        "scream",       "You can hear again.",  "", KINGDOM_NONE ,
        GROUP_NONE
    },
  {
        "hurricane",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/   
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  7,  8,  6,  8, 5, 9, 9, 7, 5, 7, 5, 9, 8},
        {0,0,0,0,0},spell_hurricane,        TAR_IGNORE,     POS_FIGHTING,
        NULL,           SLOT(672),      200,    15,
        "helical flow", "!Hurricane!",  "", KINGDOM_NONE ,
         GROUP_NATURE
    },
 {
        "web",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/          
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  7,  8,  7,  9,  9, 6, 7, 7, 7, 6, 6, 5, 9, 8},
        {0,1,0,0,0},spell_web,              TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_web,               SLOT(592),      30,     12,
        "",             "The webs around you dissolve",
        "", KINGDOM_NONE , GROUP_NATURE
    },
	{
        "esp",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/     
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  7,  8,  9,  5, 7, 9, 9, 7, 8, 7, 8, 8, 8},
        {0,0,0,0,0},spell_esp,              TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_esp,               SLOT(754),      50,     24,
        "",             "The field of ESP disintegrates",
        "", KINGDOM_NONE , GROUP_NATURE
    },
	{
        "aura of invulnerabilty",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/     
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_invul,             TAR_CHAR_SELF,     POS_STANDING,
        &gsn_invul,               SLOT(757),      100,     24,
        "",             "The aura of invulnerability fades away.",
        "", KINGDOM_NONE , GROUP_DEFENSIVE
    },

	{
        "ironskin",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/     
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_ironskin,             TAR_CHAR_SELF,     POS_STANDING,
        &gsn_ironskin,               SLOT(757),      100,     24,
        "",             "The magical aura fades away",
        "", KINGDOM_NONE , GROUP_DEFENSIVE
    },
    
    {
        "group defense",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/        
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  9,  9,  6,  9,  5,  8, 7, 9, 9, 9, 7, 7, 6, 8, 8},
        {0,1,0,0,0},spell_group_defense,            TAR_IGNORE, POS_STANDING,
        NULL,           SLOT(586),      100,    36,
        "",                     "You feel less protected.",     "",
	KINGDOM_NONE , GROUP_PROTECTIVE
    },
  {
        "inspire",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/ 
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  7,  8,  7,  8, 7, 9, 9, 5, 6, 7, 7, 9, 8},
        {0,1,0,0,0},spell_inspire,          TAR_IGNORE,             POS_STANDING,
        NULL,           SLOT(587),      75,     24,
        "",                     "You feel less inspired",       "",
	KINGDOM_NONE , GROUP_BENEDICTIONS
    },
   {
        "group heal",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/          
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  7,  9,  6,  8, 9, 9, 9, 8, 9, 9, 7, 8, 8},
        {0,0,0,0,0},spell_group_healing,    TAR_CHAR_DEFENSIVE, POS_FIGHTING,
        NULL,                   SLOT(642),      100,    24,
        "",                     "!Group Heal!",         "", KINGDOM_NONE ,
         GROUP_HEALING
    },
 {
        "restoring light",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/ 
	{  1, 103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  7,  8,  6,  8, 6, 9, 9, 6, 8, 7, 7, 9, 8},
        {0,1,0,0,0},spell_restoring_light,  TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(643),      50,     24,
        "",                     "!restoring light!","",
        KINGDOM_NONE , GROUP_BENEDICTIONS
    },
 {
        "sulfurus spray",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103, 5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7,  6,  6,  5,  6,  7,  6, 5, 7, 7, 6, 5, 5, 6, 8, 8},
        {0,0,0,0,0},spell_sulfurus_spray,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(652),      20,     12,
        "sulfurus spray",       "!sulfurus spray!", "", KINGDOM_NONE ,
        GROUP_ACID
    },
 {
        "caustic font",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7,  6,  6,  5,  6,  7, 6, 5, 7, 7, 6, 5, 5, 6, 8, 7},
        {0,0,0,0,0},spell_caustic_font,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(653),      20,     12,
        "caustic font", "!caustic font!", "", KINGDOM_NONE ,
         GROUP_ACID
    },
 {
        "acetum primus",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7,  6,  6,  5,  6,  7, 6, 5, 7, 7, 6, 5, 5, 5, 7, 8},
        {0,0,0,0,0},spell_acetum_primus,    TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(654),      20,     12,
        "acetum primus",        "!acetum primus!", "", KINGDOM_NONE ,
        GROUP_ACID
    },
 {
        "galvanic whip",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/      
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7,  6,  6,  5,  6,  7, 6, 5, 7, 7, 6, 5, 5, 6, 7, 8},
        {0,0,0,0,0},spell_galvanic_whip,    TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(655),      20,     12,
        "galvanic whip",        "!galvanic whip!", "", KINGDOM_NONE ,
        GROUP_LIGHTNING
    },
 {
        "magnetic trust",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/ 
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7,  6,  6,  5,  6,  7, 6, 5, 7, 7, 6, 5, 5, 6, 7, 8},
        {0,0,0,0,0},spell_magnetic_trust,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(656),      20,     12,
        "magnetic trust",       "!magnetic trust!", "", KINGDOM_NONE ,
        GROUP_LIGHTNING
    },

    {
        "quantum spike",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/      
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7,  6,  6,  5,  6,  7, 6, 5, 7, 7, 6, 5, 5, 6, 7, 8},
        {0,0,0,0,0},spell_quantum_spike,    TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(657),      20,     12,
        "quantum spike",        "!quantum spike!", "", KINGDOM_NONE ,
        GROUP_LIGHTNING
    },
 {
        "hand of undead",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  8,  8,  8,  7, 8, 6, 7, 7, 8, 7, 8, 7, 8, 5},
        {0,0,0,0,0},spell_hand_of_undead,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(647),      20,     24,
        "hand of undead",       "!hand of undead!", "", KINGDOM_NONE ,
        GROUP_NECRO
    },
    {
        "corruption",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 7, 7, 7, 8, 6, 8, 8, 9, 7},
        {1,1,1,0,0},spell_corruption,    TAR_CHAR_OFFENSIVE,        POS_FIGHTING,
        &gsn_corruption,             SLOT(671),        5,       6,
        "corruption", "You feel yourself healthy again.", "",KINGDOM_NONE,
         GROUP_NECRO
    },
    
    {
        "regeneration",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 7, 7, 7, 8, 6, 8, 8, 9, 7},
        {0,0,0,0,0},spell_regeneration,    TAR_CHAR_DEFENSIVE,        POS_STANDING,
        &gsn_regeneration,             SLOT(671),        30,       12,
        "regeneration", "You stop regenerating", "",KINGDOM_NONE,
         GROUP_HEALING
    },
    
   {
        "mysterious dream",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 7, 7, 7, 6, 7, 8, 8, 9, 8},
        {0,0,0,0,0},spell_mysterious_dream, TAR_IGNORE,     POS_STANDING,
        &gsn_mysterious_dream,  SLOT(678),      200,    24,
        "",             "!mysterous dream!",    "",
        KINGDOM_NONE ,  GROUP_BEGUILING
    },
    
    {
        "totem",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 7, 7, 7, 6, 7, 8, 8, 9, 9},
        {0,0,0,0,0},spell_null, TAR_IGNORE,     POS_STANDING,
        &gsn_totem,  SLOT(678),      50,    24,
        "",             "!You are ready to build another totem!",    "",
        KINGDOM_NONE ,  GROUP_BEGUILING
    },
    
  {
        "insanity",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 7, 7, 7, 6, 7, 8, 8, 9, 8},
        {1,0,0,0,0},spell_insanity,         TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_insanity,           SLOT(616),      100,    24,
        "",     "Now you feel yourself calm down.",     "",
        KINGDOM_NONE ,GROUP_BEGUILING
    },

  {
        "acid arrow",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/   
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 5, 7, 7, 8, 6, 7, 7, 9, 8},
        {0,1,0,0,0},spell_acid_arrow,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(644),      20,     12,
        "acid blast",           "!Acid Arrow!", "", KINGDOM_NONE ,
	GROUP_ACID
    },
  {
        "etheral fist",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 5, 7, 7, 8, 6, 7, 7, 8, 8},
        {0,0,0,0,0},spell_etheral_fist,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(645),      20,     12,
        "etheral fist",         "!Etheral Fist!", "", KINGDOM_NONE ,
        GROUP_NECRO
    },
   {
        "spectral furor",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/  
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 5, 7, 7, 8, 6, 7, 8, 9, 7},
        {0,0,0,0,0},spell_spectral_furor,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(646),      20,     12,
        "spectral furor",       "!spectral furor!", "", KINGDOM_NONE ,
        GROUP_NECRO
    },
 {
        "disruption",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/ 
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 5, 7, 7, 8, 6, 7, 8, 9, 8},
        {0,0,0,0,0},spell_disruption,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(648),      20,     12,
        "disruption",   "!disruption!", "", KINGDOM_NONE ,
        GROUP_ATTACK
    },

 {
        "sonic resonance",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/ 
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 4, 7, 7, 6, 5, 5, 8, 8, 9},
        {0,0,0,0,0},spell_sonic_resonance,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(649),      20,     12,
        "sonic resonance",      "!sonic resonance!", "", KINGDOM_NONE ,
        GROUP_ATTACK
    },
  {
        "mind wrack",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  8, 8, 4, 7, 7, 6, 5, 5, 8, 8, 8},
        {0,0,0,0,0},spell_mind_wrack,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(650),      20,     12,
        "mind wrack",   "!mind wrack!", "", KINGDOM_NONE ,
        GROUP_MIND
    },

  {
        "sanctify lands",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  7, 8, 7, 9, 9, 8, 7, 7, 8, 8, 8},
        {0,0,0,0,0},spell_sanctify_lands,   TAR_IGNORE,     POS_STANDING,
        NULL,                   SLOT(673),      200,    24,
        "",             "!sanctify lands!",     "",
        KINGDOM_NONE ,  GROUP_BENEDICTIONS
    },
  {
        "deadly venom",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/    
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  7, 8, 7, 9, 9, 8, 7, 7, 7, 9, 8},
        {0,1,0,0,0},spell_deadly_venom,     TAR_IGNORE,     POS_STANDING,
        &gsn_deadly_venom,              SLOT(674),      200,    24,
        "",             "!deadly venom!",       "",
        KINGDOM_NONE ,  GROUP_POISON
    },

    {
        "lethargic mist",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  7, 8, 7, 9, 9, 8, 7, 7, 8, 9, 8},
        {0,0,0,0,0},spell_lethargic_mist,   TAR_IGNORE,     POS_STANDING,
        &gsn_lethargic_mist,    SLOT(676),      200,    24,
        "",             "!lethargic mist!",     "",
        KINGDOM_NONE ,  GROUP_MALADICTIONS
    },

    {
        "black death",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  7, 8, 7, 9, 9, 8, 7, 7, 8, 9, 8},
        {1,0,0,0,0},spell_black_death,      TAR_IGNORE,     POS_STANDING,
        &gsn_black_death,       SLOT(677),      200,    24,
        "",             "!black death!",        "",
        KINGDOM_NONE ,  GROUP_NECRO
    },
  {
        "extension",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  8,  7, 8, 7, 9, 9, 8, 7, 7, 8, 9, 8},
        {0,0,0,0,0},spell_extension,        TAR_OBJ_INV,    POS_STANDING,
        NULL,           SLOT(599),  200, 48,
        "",     "!Extension!",          "",
        KINGDOM_NONE ,  GROUP_MALADICTIONS
        },
   {
        "fire shield",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  7,  7, 8, 5, 9, 9, 8, 6, 5, 7, 9, 9},
        {0,0,0,0,0},spell_fire_shield,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_fire_shield,       SLOT(219),      75,     12,
        "flaming aura", "The fire around your body dies.",      "$p fiery aura.",
        KINGDOM_NONE ,  GROUP_FIRE
        },
    {
        "vampiric aura",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  7,  7, 8, 5, 9, 9, 8, 6, 5, 7, 9, 8},
        {0,0,0,0,0},spell_vampiric_aura,     TAR_CHAR_SELF,     POS_STANDING,
        &gsn_vampiric_aura,       SLOT(219),      50,     12,
        "", "You lose your animal magnitude.",      "",
        KINGDOM_NONE ,  GROUP_FIRE
        },
     {
        "ghoul touch",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  7,  7, 8, 5, 9, 9, 8, 6, 5, 7, 9, 8},
        {0,0,0,0,0},spell_ghoul_touch,     TAR_CHAR_SELF,     POS_STANDING,
        &gsn_ghoul_touch,       SLOT(219),      40,     12,
        "", "Your hands turn back to normal ",      "",
        KINGDOM_NONE ,  GROUP_FIRE
        },
      {
        "vampiric touch",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  7,  7, 8, 5, 9, 9, 8, 6, 5, 7, 9, 8},
        {0,0,0,0,0},spell_vampiric_touch,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_vampiric_touch,       SLOT(219),      40,     12,
        "", "Your hands turn back to normal ",      "",
        KINGDOM_NONE ,  GROUP_FIRE
        },      
     {
        "command",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  7,  7, 8, 5, 9, 9, 8, 6, 5, 7, 9, 8},
        {0,0,0,0,0},spell_command,     TAR_IGNORE,     POS_STANDING,
        NULL,       SLOT(219),      15,     12,
        "", "",      "",
        KINGDOM_NONE ,  GROUP_FIRE
        },    
 {
        "chill shield",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  8,  8,  7,  7, 8, 5, 9, 9, 8, 6, 5, 7, 9, 8},
        {0,0,0,0,0},spell_chill_shield,   TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_chill_shield,      SLOT(220),      75,     12,
        "icy aura",     "The icy aura around your body dies.",  "$p icy aura fades.",
        KINGDOM_NONE ,  GROUP_COLD
    },

   {
        "aid",
        /* 1, 2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 28,103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9, 7,  9,  9,  9,  9,  5, 9, 9, 9, 9, 7, 9, 9, 7, 8, 8},
        {0,0,0,0,0},spell_aid, TAR_CHAR_DEFENSIVE, POS_STANDING,
        &gsn_aid, SLOT(409),  60,30,
        "",     "You don't feel aided anymore.", "",
	KINGDOM_NONE, GROUP_BENEDICTIONS
        },

    {
	"acid blast",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 28, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  9,  8,  8, 7, 6, 9, 9, 8, 6, 7, 8, 9, 8},
	{0,0,0,0,0},spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_blast,	SLOT(70),	20,	12,
	"acid blast",		"!Acid Blast!", "",KINGDOM_NONE,
	GROUP_ACID
    },

    {
	"armor",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 3},
	{0,1,0,0,0},spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less armored.",	"",
	KINGDOM_NONE, GROUP_PROTECTIVE
    },
    
    {
	"decay",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 3},
	{1,0,0,0,0},spell_decay,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"The corrosive effect wears off.",	"",
	KINGDOM_NONE, GROUP_PROTECTIVE
    },
    
    {
	"bless",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 7, 103,103, 6, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  3,  9,  4,  5,  1, 3, 4, 5, 6, 3, 4, 3, 4, 6, 7},
	{0,0,0,0,0},spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_bless,		SLOT( 3),	 5,	12,
	"",			"You feel less righteous.",
	"$p's holy aura fades.", KINGDOM_NONE, GROUP_BENEDICTIONS
    },
    
    {
	"hold person",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 7, 103,103, 6, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  3,  9,  4,  5,  1, 3, 4, 5, 6, 3, 4, 3, 4, 6, 6},
	{0,0,0,0,0},spell_hold_person,		TAR_IGNORE,	POS_FIGHTING,
	&gsn_hold_person,		SLOT( 3),	 5,	18,
	"",			"Hold person wears off",
	"", KINGDOM_NONE, GROUP_BENEDICTIONS
    },
    
    {
	"paralysis",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 7, 103,103, 6, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  3,  9,  4,  5,  1, 3, 4, 5, 6, 3, 4, 3, 4, 6, 5},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,	POS_FIGHTING,
	&gsn_paralysis,		SLOT( 3),	 5,	18,
	"",			"You feel less restrained.",
	"", KINGDOM_NONE, GROUP_BENEDICTIONS
    },

    {
	"blindness",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 12,  8, 103,103,103, 4, 6, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  6,  7,  8, 8,  5, 6, 4, 4, 7, 5, 3, 3, 5, 7, 5},
	{0,0,0,0,0},spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again.",	"", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },
    
     {
	"wither",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 12,  8, 103,103,103, 4, 6, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  6,  7,  8, 8,  5, 6, 4, 4, 7, 5, 3, 3, 5, 7, 4},
	{0,0,0,0,0},spell_wither,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_wither,		SLOT( 4),	 15,	12,
	"",			"You feel more lively.",	"", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },

    {
	"burning hands",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  6,  7,  8,  8,  5, 6, 4, 4, 7, 5, 3, 3, 6, 9, 8},
	{0,0,0,0,0},spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_burning_hands,	SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	"", KINGDOM_NONE,
	GROUP_FIRE
    },

    {
	"call lightning",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,18, 103,103,11, 103,103,17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  9,  7,  6, 8, 6, 8, 8, 9, 7, 6, 5, 9, 8},
	{0,1,0,0,0},spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!",	"", KINGDOM_NONE,
	GROUP_NATURE
    },

    {   "calm",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
  	{ 103,16, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  	{  9,  9,  7,  8,  9,  7,  7,  6, 7, 7, 6, 8, 8, 6, 7, 7, 9, 7},
	{0,0,0,0,0},spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.", "",
    KINGDOM_NONE, GROUP_BEGUILING
    },
    {
	"cancellation",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 18, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  9,  9,  9,  8,  9, 9, 7, 9, 9, 9, 7, 9, 8, 9, 8},
	{0,1,0,0,0},spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(507),	20,	12,
	"",			"!cancellation!",	"", KINGDOM_NONE,
	GROUP_DEFENSIVE
    },
    {
	"cause critical",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,13, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  6,  9,  5, 6, 9, 9, 9, 7, 9, 9, 7, 8, 4},
	{1,0,0,0,0},spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"cause light",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 1, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  5,  9,  4,  9,  3, 6, 9, 9, 9, 5, 9, 9, 5, 9, 7},
	{1,1,1,0,0},spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"cause serious",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 7, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  6,  9,  5,  9,  4, 6, 9, 9, 9, 6, 9, 9, 6, 9, 8},
	{1,1,1,0,0},spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"chain lightning",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 33, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  7,  6,  8, 7, 6, 8, 8, 8, 6, 6, 7, 9, 8},
	{0,0,0,0,0},spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	"", KINGDOM_NONE,
	GROUP_LIGHTNING
    },

    {
	"change sex",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  8,  6,  6,  5, 6, 5, 7, 7, 6, 5, 5, 7, 8, 8},
	{1,1,1,0,0},spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(82),	15,	12,
	"",			"Your body feels familiar again.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },

    {
	"charm person",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 8},
	{0,1,0,0,0},spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",			"You feel more self-confident.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },

    {
	"chill touch",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  4, 103,103,103,103, 3, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 2, 3, 4, 4, 2, 3, 7, 9, 8},
	{0,0,0,0,0},spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	"", KINGDOM_NONE,
	GROUP_COLD
    },

    {
	"colour spray",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 16, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 4, 7, 9, 8},
	{0,0,0,0,0},spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"continual light",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  6,  4, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 4, 5, 7, 6},
	{0,0,0,0,0},spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	"", KINGDOM_NONE,
	GROUP_CREATION
    },

    {
	"control weather",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 15, 19, 103,103,103,103,103,19, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 4, 2, 8, 5},
	{0,0,0,0,0},spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	"", KINGDOM_NONE,
	GROUP_NATURE
    },

    {
	"create food",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 10,  5, 103,103, 3, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 2, 4, 9, 5},
	{0,1,0,0,0},spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	"", KINGDOM_NONE,
	GROUP_CREATION
    },

    {
	"create rose",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 16, 103,103,103,10, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 2, 3, 9, 6},
	{0,1,0,0,0},spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	"", KINGDOM_NONE,
	GROUP_CREATION
    },

    {
	"create spring",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 14, 17, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 3, 4, 8, 6},
	{0,1,0,0,0},spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	"", KINGDOM_NONE,
	GROUP_CREATION
    },

    {
	"create water",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  8,  3, 103,103, 4, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  3,  5,  4,  4,  5, 5, 3, 4, 5, 5, 3, 2, 1, 9, 6},
	{0,1,0,0,0},spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	"", KINGDOM_NONE,
	GROUP_CREATION
    },

    {
	"cure blindness",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 7},
	{0,0,0,0,0},spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_blindness,	SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	"", KINGDOM_NONE,
	GROUP_CURATIVE
    },

    {
	"cure critical",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,13, 103,103,10, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  9,  9,  9,  9,  5, 9, 9, 9, 9, 7, 9, 9, 6, 8, 8},
	{0,0,0,0,0},spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_critical,	SLOT(15),	20,	12,
	"",			"!Cure Critical!",	"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"cure disease",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,13, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 1, 1, 5},
	{0,0,0,0,0},spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_cure_disease,	SLOT(501),	20,	12,
	"",			"!Cure Disease!",	"", KINGDOM_NONE,
	GROUP_CURATIVE
    },

    {
	"cure light",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103, 1, 103,103, 1, 6 ,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  9,  9, 9,  9,  2, 9, 9, 9, 9, 6, 9, 9, 4, 7, 6},
	{1,1,0,0,0},spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(16),	10,	12,
	"",			"!Cure Light!",		"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"cure poison",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		       
	{ 103,14, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  9,  9,  9,  9,  3, 9, 9, 9, 9, 6, 9, 9, 2, 8, 7},
	{1,1,0,0,0},spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_cure_poison,	SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	"", KINGDOM_NONE,
	GROUP_CURATIVE
    },

    {
	"cure serious",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103, 7, 103,103, 5,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  5,  9,  9, 9,  9,  4, 9, 9, 9, 9, 7, 9, 9, 6, 8, 7},
	{0,0,0,0,0},spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_serious,	SLOT(61),	15,	12,
	"",			"!Cure Serious!",	"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"curse",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 18, 18, 103,103,103,8, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9, 7,  7,  5, 6, 5, 5, 9, 7, 4, 5, 7, 9, 6},
	{0,1,0,0,0},spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off.",
	"$p is no longer impure.", KINGDOM_NONE, GROUP_MALADICTIONS
    },
    {
        "cursed lands",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  1, 103,103,103,103,103,103,103,5 ,1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_cursed_lands,     TAR_IGNORE,     POS_STANDING,
        &gsn_cursed_lands,      SLOT(675),      200,    24,
        "",             "!cursed lands!",       "",
        KINGDOM_NONE ,  GROUP_MALADICTIONS
    },


    {
	"demonfire",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,34, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  7,  7,  5, 6, 5, 5, 9, 7, 4, 5, 8, 9, 8},
	{0,0,0,0,0},spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(505),	20,	12,
	"torments",		"!Demonfire!",		"", KINGDOM_NONE,
	GROUP_FIRE
    },

    {
	"detect evil",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 12,  4, 103,103, 1,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  7,  9, 7,  7,  3, 6, 3, 5, 9, 7, 4, 3, 5, 9, 5},
	{0,1,0,0,0},spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears.",
	"", KINGDOM_NONE, GROUP_DETECTION
    },

    {
        "detect good",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 12,  4, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  7,  9,  7,  7,  3, 6, 3, 5, 9, 7, 4, 3, 5, 9, 5},
        {0,1,0,0,0},spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,         SLOT(513),        5,     12,
        "",           "The gold in your vision disappears.",	"",
	KINGDOM_NONE, GROUP_DETECTION
    },

    {
	"dark vision",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103,103,103,103, 5,  7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  7,  9,  7,  7,  8, 6, 7, 7, 9, 7, 4, 5, 6, 9, 6},
	{0,0,0,0,0},spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_dark_vision,	SLOT( 0),	0,	6,
	"",			"Your dark vision fades.", "", KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"detect hidden",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,10, 103,103,103,10, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  4,  7,  7,  8, 6, 7, 7, 9, 7, 7, 7, 7, 6, 6},
	{0,0,0,0,0},spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_hidden,	SLOT(44),	 5,	6,
	"",			"You feel less aware of your surroundings.",
	"", KINGDOM_NONE, GROUP_DETECTION
    },

    {
	"detect invis",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  3,  8, 103,103, 6, 10, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  7, 7,  6, 6, 4, 7, 5, 7, 4, 5, 7, 8, 7},
	{0,0,0,0,0},spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects.",
	"", KINGDOM_NONE, GROUP_DETECTION
    },

    {
	"detect magic",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  2,  6, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  7,  7,  6, 6, 4, 7, 5, 7, 4, 5, 7, 9, 7},
	{0,1,0,0,0},spell_detect_magic,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"The detect magic wears off.",	"",
	KINGDOM_NONE, GROUP_DETECTION
	},
	
	{
	"detect undead",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  2,  6, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  7,  7,  6, 6, 4, 7, 5, 7, 4, 5, 7, 9, 2},
	{0,0,0,0,0},spell_detect_undead,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"You can no longer detect the undead.",	"",
	KINGDOM_NONE, GROUP_DETECTION
	},
    {
	"detect poison",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 15,  7, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  7,  7,  6, 6, 4, 7, 5, 7, 4, 5, 1, 9, 3},
	{0,1,0,0,0},spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	"", KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"dispel evil",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,15, 103,103,25, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  5,  7,  9,  7,  7,  4, 6, 4, 7, 5, 7, 4, 5, 7, 9, 8},
	{0,0,0,0,0},spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
        "dispel good",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 103,103,103,103,103,30,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  7, 7,  4, 6, 4, 7, 5, 7, 4, 5, 7, 9, 5},
        {0,0,0,0,0},spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
        "dispel good",          "!Dispel Good!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"dispel magic",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 16, 24, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  9,  9,  9,  8,  7, 9, 5, 9, 9, 9, 5, 9, 7, 9, 7},
	{0,1,0,0,0},spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	"", KINGDOM_NONE,
	GROUP_DEFENSIVE
    },

    {
	"earthquake",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,10, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 7, 9, 9, 8, 7, 9, 5, 9, 6},
	{0,0,0,0,0},spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		"", KINGDOM_NONE,
	GROUP_NATURE
    },

    {
	"enchant armor",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 16, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8, 9, 7, 9, 9, 8, 7, 5, 8, 9, 8},
	{0,0,0,0,0},spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	"", KINGDOM_NONE,
	GROUP_ENCHANTMENT
    },

    {
	"enchant weapon",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 17, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8, 9, 7, 9, 9, 8, 7, 2, 8, 9, 8},
	{0,0,0,0,0},spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	"", KINGDOM_NONE,
	GROUP_ENCHANTMENT
    },

    {
	"energy drain",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 19, 22, 103,103,103,16,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9, 9,  8, 9, 6, 9, 9, 8, 5, 5, 8, 9, 3},
	{0,1,0,0,0},spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
	"energy drain",		"!Energy Drain!",	"", KINGDOM_NONE,
	GROUP_NECRO
    },

    {
	"faerie fire",
	/* 1,  2,  3,  4,  5,  6,  7, 8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  6,  3, 103,103,103,103, 8,103, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        {  9,  9,  9,  9,  9,  9,  9, 8,  9, 6, 9 ,9, 8, 5, 5, 8, 9, 8},
	{0,0,0,0,0},spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_faerie_fire,	SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	"", KINGDOM_NONE, GROUP_NATURE
    },

    {
	"faerie fog",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 14, 21, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  6,  8,  9, 4, 9 ,9, 8, 4, 4, 9 ,9, 8},
	{0,1,0,0,0},spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"!Faerie Fog!",		"", KINGDOM_NONE,
	GROUP_NATURE
    },

    {
	"master sight",		
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  9,  8,  8, 7, 7, 9, 9, 8, 7, 7, 7, 9, 8},
	{0,0,0,0,0},spell_master_sight,	 TAR_CHAR_SELF,    POS_STANDING,
	&gsn_master_sight,	SLOT(521),	36,	25,
	"master sight",		"Your eyes becomes dull.","",KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"fear",		
	/* 1,  2,  3,  4,  5,  6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 22, 103,103,103,103,27,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  9,  8,  8, 7, 7, 9, 9, 8, 6, 7, 9, 9, 5},
	{0,0,0,0,0},spell_fear,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(1101),	20,	12,
	"fear",		       "!Fear!",		"", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },
    {
	"shell",	
        /* 1,  2,  3,  4,  5,  6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 22, 103,103,103,103,27,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  9, 8,  8,  7, 7, 9, 9, 8, 6, 7, 9 ,9, 8},
	{0,0,0,0,0},spell_shell,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,			SLOT(1102),	20,	18,
	"shell",		       "!Shell!","",KINGDOM_NONE,
	GROUP_NATURE
    },



    {
	"fireball",		
	/* 1,  2,  3,  4,  5,  6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 22, 103,103,103,103,27,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  9,  5, 8,  7, 4, 9, 9, 8, 4, 4, 7, 9, 6},
	{0,0,0,0,0},spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!",		"", KINGDOM_NONE,
	GROUP_FIRE
    },

    {
	"fireproof",		
	/* 1,  2,  3,  4,   5,  6,   7,   8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 15, 14, 103,103, 20, 103, 103, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,   7,   9,  9,   6,   5,  7, 5, 9, 9, 8, 5, 5, 7, 9, 8},
	{0,1,0,0,0},spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",			"",	"$p's protective aura fades.",
	KINGDOM_NONE, GROUP_PROTECTIVE
    },

    {
	"flamestrike",		
	/* 1,   2,  3,   4,  5, 6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103, 20, 103, 103,10,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        {  9,   9,  7,   7,  9, 9,  6,  6,  7, 8, 9, 9, 8, 8, 8, 7 ,9, 7},
	{0,0,0,0,0},spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		"",
	KINGDOM_NONE, GROUP_FIRE
    },

    {
	"fly",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 10, 18, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  6,  6,  6,  5,  6, 6, 4, 6, 6, 6, 4, 5, 6, 9, 8},
	{1,0,0,0,0},spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_fly,			SLOT(56),	10,	12,
	"",			"You slowly float to the ground.",
	"", KINGDOM_NONE, GROUP_TRANSPORTATION
    },

    {
	"floating disc",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  4, 10, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  9,  6,  6, 7, 8, 9, 9, 8, 8, 7, 7, 9, 8},
	{0,0,0,0,0},spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	"",			"!Floating disc!",	"", KINGDOM_NONE,
	GROUP_CREATION
    },

    {
        "frenzy",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 103,24, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  9,  6,  6, 7, 8, 9, 9, 8, 8, 9, 7, 9, 8},
        {0,0,0,0,0},spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Your rage ebbs.",	"", KINGDOM_NONE,
	GROUP_BENEDICTIONS
    },

    {
	"gate",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 27, 17, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  9,  9,  8,  8, 8, 8, 9, 9, 8, 8, 8, 8, 9, 8},
	{0,1,0,0,0},spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

    {
	"giant strength",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 11, 19, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  8,  7,  7,  7, 7, 6, 7, 7, 7, 6, 4, 7, 9, 7},
	{1,0,0,0,0},spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_giant_strength,		SLOT(39),	20,	12,
	"",			"You feel weaker.",	"", KINGDOM_NONE,
	GROUP_ENHANCEMENT
    },

    {
	"harm",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,23, 103,103,103,29,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  6,  9,  6, 9,  5, 6, 8, 9, 9, 9, 9, 9, 7, 9, 5},
	{1,0,0,0,0},spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"haste",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 21, 29, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  6,  9,  7,  6,  9, 7, 4, 9, 7, 7, 4, 5, 7, 9, 6},
	{0,0,0,0,0},spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_haste,		SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	"",
	KINGDOM_NONE, GROUP_ENHANCEMENT
    },

    {
	"heal",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,21, 103,103,29,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  6,  9, 6,  9,  5, 6, 8, 9, 9, 9, 9, 9, 7, 9, 7},
	{0,0,0,0,0},spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"heat metal",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,16, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  8,  9,  5, 8, 7, 6, 9, 9, 7, 6, 4, 9, 8},
	{0,0,0,0,0},spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
	"spell",		"!Heat Metal!",		"", KINGDOM_NONE,
	GROUP_FIRE
    },

    {
	"holy word",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,36, 103,103,38, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  6,  9,  6,  9,  5, 6, 8, 9, 9, 9, 9, 9, 7, 9, 7},
	{0,0,0,0,0},spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
	"divine wrath",		"!Holy Word!",		"", KINGDOM_NONE,
	GROUP_BENEDICTIONS
    },

    {
	"identify",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 15, 16, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8,  6,  6, 8, 5, 8, 8, 8, 5, 4, 6, 9, 7},
	{0,1,0,0,0},spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(103),	12,	24,
	"",			"!Identify!",		"", KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"infravision",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  9, 13, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8,  4,  4, 8, 2, 8, 8, 8, 2, 3, 4, 9, 2},
	{0,0,0,0,0},spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_infravision,			SLOT(77),	 5,	12,
	"",			"You no longer see in the dark.",
	"", KINGDOM_NONE, GROUP_DETECTION
    },

    {
	"invis",
	/* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 7},
	{0,0,0,0,0},spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",
	"$p fades into view.", KINGDOM_NONE, GROUP_ILLUSION
    },
    
 /*   {
	"improved invis",
	 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8		
	{  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
	{0,0,0,0,0},spell_improved_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_improved_invis,		SLOT(29),	 50,	12,
	"",			"You are no longer invisible.",
	"", KINGDOM_NONE, GROUP_ILLUSION
    },*/

    {
	"know alignment",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 12,  9, 103,103, 4, 5, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9, 8,  6,  5, 8, 5, 8, 8, 8, 6, 4, 4, 9, 8},
	{0,1,0,0,0},spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	"", KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"scan person",
	/* 1,  2,  3,  4,  5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 12,  9, 103,103, 4, 5, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9, 8,  9,  9, 8, 6, 8, 8, 8, 6, 5, 8, 9, 8},
	{0,1,0,0,0},spell_scan_person,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(1013),	 9,	48,
	"",			"!Scan Person!",	"", KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"lightning bolt",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 13, 103,103,103,103,13, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8,  6,  9, 8, 3, 8, 8, 8, 3, 4, 6, 9, 7},
	{0,0,0,0,0},spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(30),	15,	12,
	"lightning bolt",	"!Lightning Bolt!",	"", KINGDOM_NONE,
	GROUP_LIGHTNING
    },

    {
	"locate object",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  9, 15, 103,103,19, 103,14, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8,  9,  6, 8, 5, 8, 8, 8, 5, 4, 7, 9, 8},
	{0,1,0,0,0},spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	"", KINGDOM_NONE,
	GROUP_DETECTION
    },

    {
	"magic missile",	
	/*1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 1, 103,103,103,103, 1, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 5,  5,  5,  5,  5,  3,  3,  4,  3, 2, 4, 4, 3, 2, 2, 5 ,5, 7},
	{0,1,0,0,0},spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(32),	15,	12,
	"magic missile",	"!Magic Missile!",	"", KINGDOM_NONE,
	GROUP_PRO_MAGIC
    },

    {
	"mass healing",
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,38, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        {  9,  9,  7,  9,  9,  8,  9,  6,  9, 9, 9, 9, 8, 9, 9, 8 ,1, 7},
	{0,0,0,0,0},spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"mass invis",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 22, 25, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,   9,  9,  9,  9,  9,  9,  9,  8, 4, 8, 8, 8, 4, 5, 9, 9, 7},
	{0,0,0,0,0},spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"You are no longer invisible.",
	"", KINGDOM_NONE, GROUP_ILLUSION
    },

    {
        "nexus",
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 40, 35, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  9,  9,  8,  8,  8, 8, 9, 9, 8, 8, 8, 9, 9, 9},
        {0,0,0,0,0},spell_nexus,            TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(520),       150,   36,
        "",                     "!Nexus!",		"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

    {
	"pass door",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 24, 32, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  9,  9,  8,  8,  8, 8, 9, 9, 8, 8, 8, 8, 9, 7},
	{1,1,1,0,0},spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again.",	"",
	KINGDOM_NONE, GROUP_ILLUSION
    },

    {
	"plague",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 23, 17, 103,103,103,25, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  9,  9,  8,  8,  8, 8, 9, 9, 8, 5, 8, 6, 9, 8},
	{1,1,1,0,0},spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
	"sickness",		"Your sores vanish.",	"", KINGDOM_NONE,
	GROUP_POISON
    },


    {
        "portal",               
       /*  1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 35, 30, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  8,  9,  9,  8,  8,  8, 8, 9, 9, 8, 8, 8, 8 ,9, 8},
        {0,0,0,0,0},spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(519),       100,     24,
        "",                     "!Portal!",		"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

    {
	"protection evil",	
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 12,  9, 103,103,11, 103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  5,  8,  9,  9,  8,  6,  8, 6, 9, 9, 8, 7, 5, 5, 8, 8},
	{0,1,0,0,0},spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	"",
	KINGDOM_NONE, GROUP_PROTECTIVE
    },

    {
	"protection good",	
	/* 1,  2,  3,  4,  5,  6, 7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 12,  9, 103,103,11,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  9, 8,  6,  8, 6, 9, 9, 8, 7, 5, 5, 9, 4},
	{0,1,0,0,0},spell_protection_good,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(1022), 	5,	12,
	"",			"You feel less protected.",	"",
	KINGDOM_NONE, GROUP_PROTECTIVE
    },

    {
        "ray of truth",         
       /*  1,  2, 3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 103,35,103,103,35, 103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  4,  8,  9,  9,  8,  3,  9, 9, 9, 9, 9, 9, 9, 7, 7, 6},
        {0,0,0,0,0},spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_ray_of_truth,      SLOT(518),      20,     12,
        "ray of truth",         "!Ray of Truth!",	"", KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
	"recharge",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  9, 103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  9,  9,  9,  9, 5, 9, 9, 9, 7, 4, 9, 9, 8},
	{0,0,0,0,0},spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		"", KINGDOM_NONE,
	GROUP_ENCHANTMENT
    },

    {
	"refresh",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  8,  5, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  4,  8,  9,  9,  9,  5,  9, 4, 9, 9, 6, 5, 4, 6 ,8, 8},
	{0,1,0,0,0},spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	15,
	"refresh",		"!Refresh!",		"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"remove curse",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,18, 103,103,13, 103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  8,  9,  9,  9,  5,  9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
	{0,0,0,0,0},spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_remove_curse,	SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	"", KINGDOM_NONE,
	GROUP_CURATIVE
    },

    {
	"sanctuary",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/				
	{ 36, 20, 103,103,27, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  8,  9,  7,  9,  5, 9, 9, 9, 9, 9, 9, 9, 7, 1, 7},
	{0,0,0,0,0},spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	"", KINGDOM_NONE, GROUP_DEFENSIVE
    },

    {
	"shield",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 20, 35, 103,103,103,103,103,21, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  3,  5,  9,  5,  5,  3, 4, 3, 6, 6, 5, 3, 2, 4, 7, 5},
	{0,0,0,0,0},spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(67),	12,	12,
	"",			"Your force shield shimmers then fades away.",
	"", KINGDOM_NONE, GROUP_DEFENSIVE
    },

    {
	"shocking grasp",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 10, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  9, 4, 3, 6, 6, 5, 3, 2, 7, 9, 7},
	{0,0,0,0,0},spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(103),	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	"", KINGDOM_NONE,
	GROUP_LIGHTNING
    },

    {
	"sleep",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 10, 103,103,103,103,25, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  8,  7,  9, 7, 7, 9, 9, 8, 6, 7, 8, 9, 7},
	{0,0,0,0,0},spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired.",	"", KINGDOM_NONE,
	GROUP_BEGUILING
    },

    {
        "slow",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        { 23, 30, 103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  9, 4, 3, 6, 6, 5, 3, 2, 7, 9, 6},
        {0,0,0,0,0},spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_slow,                   SLOT(515),      30,     12,
        "",                     "You feel yourself speed up.",	"",
	KINGDOM_NONE, GROUP_MALADICTIONS
    },

    {
	"stone skin",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 25, 40, 103,103,103,103,25, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  3,  5,  9,  5,  5,  3, 4, 3, 6, 6, 5, 3, 2, 2, 8, 7},
	{0,0,0,0,0},spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	12,
	"",			"Your skin feels soft again.",	"",
	KINGDOM_NONE, GROUP_DEFENSIVE
    },

    {
	"summon",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 24, 12, 103,103,21, 20, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  8,  7,  9,  8,  7,  8, 7, 7, 9, 9, 8, 6, 7, 7, 9, 6},
	{0,1,0,0,0},spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Summon!",		"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

    {
	"summon dead",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  1,  1,  1,  1,  1, 30,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  7,  9,  8,  7,  6, 7, 7, 9, 9, 8, 5, 7, 7, 9, 8},
	{0,0,0,0,0},spell_summon_dead,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(952),	10,	10,
	"summon dead",		"!Summon Dead!",
	"",KINGDOM_NONE, GROUP_NECRO
    },

    {
	"teleport",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 13, 22, 103,103,103, 7, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  6,  5,  5,  5,  6, 5, 5, 5, 5, 5, 5, 5, 6, 9, 6},
	{0,1,0,0,0},spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!",		"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

    {
	"ventriloquate",	
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  1, 103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  6,  8,  6,  5,  6,  5, 3, 4, 3, 5, 3, 3, 9, 9, 8},
	{0,0,0,0,0},spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	"", KINGDOM_NONE,
	GROUP_ILLUSION
    },

    {
	"weaken",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 11, 14, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  9,  4, 3, 6, 6, 5, 3, 2, 6, 7, 8},
	{1,0,0,0,0},spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_weaken,		SLOT(68),	20,	12,
	"spell",		"You feel stronger.",	"", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },
    
    {
	"feeblemind",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 11, 14, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  9,  4, 3, 6, 6, 5, 3, 2, 6, 7, 8},
	{1,0,0,0,0},spell_feeblemind,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_feeblemind,	SLOT(68),	20,	12,
	"spell",		"Your head feels lighter.","", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },

    {
        "whirlwind",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {  6, 7, 7, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_whirlwind,		SLOT(982),	0,	12,
        "WhirlWind",		"!Whirlwind!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "dive",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6, 7, 7, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,		TAR_CHAR_SELF,		POS_FIGHTING,
        &gsn_dive,		SLOT(982),	0,	12,
        "",		"You float to the surface",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
	"word of recall",
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 32, 28, 103,103,32, 103,29, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  8,  9,  8,  9,  6,  7, 7, 9, 9, 8, 7, 7, 8, 9, 7},
	{1,1,1,0,0},spell_word_of_recall,	TAR_IGNORE,		POS_RESTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!",	"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

/*
 * Dragon breath
 */
    {
	"acid breath",		
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,   9,  9,  8,  9,  8,  9,  9, 8, 6, 9, 9, 8, 6, 7, 9, 9, 8},
	{0,0,0,0,0},spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(200),	40,	15,
	"blast of acid",	"!Acid Breath!",	"", KINGDOM_NONE,
	GROUP_ACID
    },

    {
	"fire breath",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,   9,  9,  8,  9,  8,  9,  9,  8, 6, 9, 9, 8, 6, 7, 9, 9, 8},
	{0,0,0,0,0},spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(201),	90,	15,
	"blast of flame",	"The smoke leaves your eyes.",	"",
	KINGDOM_NONE, GROUP_FIRE
    },

    {
	"frost breath",		
        /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  8,  9,  9,  8, 6, 9, 9, 8, 6, 7, 9, 9, 8},
	{0,0,0,0,0},spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(202),	50,	15,
	"blast of frost",	"!Frost Breath!",	"", KINGDOM_NONE,
	GROUP_COLD
    },

    {
	"gas breath",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  8,  9,  9,  8, 6, 9, 9, 8, 6, 7, 9, 9, 9},
	{0,0,0,0,0},spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	75,	15,
	"blast of gas",		"!Gas Breath!",		"", KINGDOM_NONE,
	 GROUP_POISON
    },

    {
	"lightning breath",	
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  8,  9,  8,  9,  9,  8, 6, 9, 9, 8, 6, 7, 9, 9, 8},
	{0,0,0,0,0},spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(204),	80,	15,
	"blast of lightning",	"!Lightning Breath!",	"", KINGDOM_NONE,
	 GROUP_LIGHTNING
    },

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
    {
        "general purpose",      
       /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        {103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 9, 9, 1},
        { 9,  9,  9,  8,  9,  8,  4,  9,  8, 3, 9, 9, 4, 3, 5, 9, 9, 8},
        {0,0,0,0,0},spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(401),      0,      12,
        "general purpose ammo", "!General Purpose Ammo!",	"",
	KINGDOM_NONE, GROUP_NONE
    },

    {
        "high explosive",       
       /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        {103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  8,  9,  8,  9,  9,  8, 6, 9, 9, 8, 6, 7, 9, 9, 8},
        {0,0,0,0,0},spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(402),      0,      12,
        "high explosive ammo",  "!High Explosive Ammo!",	"",
	KINGDOM_NONE, GROUP_NONE
    },

/* combat and weapons skills */


    {
	"axe",			
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103, 1, 103, 1, 103, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  9,  7,  9,  9,  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
        "",                     "!Axe!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

    {
        "dagger",
        /* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
        {  1, 103, 1,  1, 103, 1,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  5,  6,  7,  7,  4,  5,  4,  9, 9, 7, 9, 6, 6, 7, 7, 9, 9, 7},
                {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
        "",                     "!Dagger!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

    {
	"flail",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103, 1, 103, 1, 103, 1, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  9,  7,  9,  9,  9,  6,  9, 9, 9, 9, 9, 9, 9, 7, 7, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
        "",                     "!Flail!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

    {
	"mace",			
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103, 1,  1,  1,  1,  1, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  9,  7,  9,  9,  9,  6,  9, 9, 9, 9, 9, 9, 9, 7, 9, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
        "",                     "!Mace!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

    {
	"polearm",		
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103,103, 1, 1,   1, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6,  7, 9,   9,  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
        "",                     "!Polearm!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

    {
	"shield block",
	/* 1,  2, 3, 4, 5, 6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,17, 7, 1, 1, 1, 12, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7, 6, 7, 7, 7,  9,  8, 6, 9, 9, 9, 9, 9, 9, 7, 8, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"spear",
	/* 1,  2,  3,  4,  5,  6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103, 1,  1, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  9,  7,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      0,
        "",                     "!Spear!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

  {
        "swim",
        /* 1,  2,  3,  4,  5,  6, 7, 8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103, 1,  1, 103, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  5,  5,  5,  5,  5,  5, 5, 5,  5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_swim,              SLOT( 0),       0,      0,
        "",                     "!Swim!",               "",
	KINGDOM_NONE,GROUP_NONE
    },

    {
	"sword",
	/* 1,  2,  3,  4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 103,103, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  4,  5,  5,  5, 5, 5, 9, 9, 6, 9, 6, 9, 5, 9, 9, 7, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
        "",                     "!sword!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },
   {
        "arrow",
        /* 1,  2,  3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		     
	{ 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        {  6,  7,  5, 7, 5, 9, 9, 9, 9, 9, 5, 9, 9, 9, 9, 9, 7, 8},
        {0,0,0,0,0},spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_arrow,     SLOT(0),        0,      0,
        "arrow",        "!arrow!",      "",
        KINGDOM_NONE , GROUP_WEAPONMASTER
    },
 {
        "lance",
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6, 7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_lance,     SLOT(0),        0,      0,
        "lance",        "!lance!",      "",
        GROUP_NONE, GROUP_WEAPONMASTER
    },
    {
	"bow",
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 	
	{ 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  5, 7, 5, 9, 9, 9, 9, 9, 5, 9, 9, 9, 9, 8, 7, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bow,            	SLOT( 0),       0,      12,
        "",                     "!bow!",		"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },
    {
	"whip",
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{  1,  1, 103,1,103,1,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  5, 7, 5, 9, 9, 9, 9, 9, 5, 9, 9, 9, 9, 9, 9, 6},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
        "",                     "!Whip!",	"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },
    {
	"smithing",
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{  1,  1, 103,1,103,1,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6,  7,9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_smithing,            	SLOT( 0),       0,      0,
        "",                     "!Smithing!",	"", KINGDOM_NONE,
	GROUP_WEAPONMASTER
    },

    {
        "backstab",             
        /* 1,  2,  3, 4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,103, 1,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9, 8,  6,  9,  7,  9,  9, 9, 9, 6, 8, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     12,
        "backstab",             "!Backstab!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"bash",			
	/* 1,  2,  3, 4, 5,  6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,103,1,103,15,15, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6, 7, 9,  9, 9, 9, 9, 9, 9, 7, 7, 9, 9, 1, 1, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      18,
        "bash",                 "!Bash!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    {
	"multiburst",
	/* 1,  2,  3,  4, 5,  6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,103, 1,103,15,15, 4, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        {  9,  9,  9,  9, 9,  9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_CHAR_DEFENSIVE,     POS_FIGHTING,
        &gsn_multiburst,      	SLOT( 0),       0,      60,
        "Multiburst",           "!Multiburst!",		"", KINGDOM_NONE,
	GROUP_NONE
    },

    {
	"shield bash",
	/* 1,  2,  3,  4,  5, 6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,103,103,10,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6,  7,  9, 9,  9,  9, 9, 9, 9, 7, 7, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_shieldbash,       	SLOT( 0),       0,      18,
        "shield bash",          "!Shieldbash!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"berserk",
	/* 1,  2,  3,  4, 5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,103,18,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6,  7, 9,  9,  9,  9, 9, 9, 9, 7, 7, 9, 9, 9, 8, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        "",                     "You feel your pulse slow down.",
	"", KINGDOM_NONE, GROUP_FIGHTMASTER
    },


    {
	"camp",
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 3, 3,103,3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  8,  8,  8, 7, 9, 9, 9, 9, 9, 9, 4, 7, 7, 9, 9, 6, 7, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_camp,		SLOT( 0),	0,	24,
	"camp",		"You can handle more camps now.",
	"", KINGDOM_NONE, GROUP_NATURE
    },
    {
	"bandage",
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 	
	{ 103,103, 3, 3,103,3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6, 7, 8, 7, 8, 5, 8, 8, 6, 7, 7, 8, 7, 7, 6, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_bandage,		SLOT( 0),	0,	12,
	"bandage",		"You feel less healthly",
	"", KINGDOM_NONE, GROUP_KINGDOM
// ex-horde skill
    },
    {
	"lay hands",
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 	
	{ 103,103, 3, 3,103,3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_layhands,		SLOT( 0),	0,	12,
	"layhands",		"You feel like you can heal more now.",
	"", KINGDOM_NONE, GROUP_HEALING
    },


    {
        "disarm",
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,103,12,10,10,11,13,11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  5, 7, 9, 9, 9, 9, 9, 9, 9, 7, 7, 9, 9, 9, 7, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     18,
        "",                     "!Disarm!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "dodge",
        /* 1,  2,  3, 4, 5,  6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,103, 1,13,103,103,1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6, 7, 5,  8, 7, 7, 8, 8, 8, 7, 7, 8, 8, 7, 3, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
        "",                     "!Dodge!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "enhanced damage",
        /* 1,  2, 3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,30,25, 1, 4, 1,11,13, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7, 5, 7, 9, 9, 9, 9, 9, 9, 9, 7, 7, 9, 9, 9, 6, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
        "",                     "!Enhanced Damage!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "brutal damage",
        /* 1,  2, 3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,30,25, 1, 4, 1,11,13, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7, 5, 7, 9, 9, 9, 9, 9, 9, 9, 7, 7, 9, 9, 9, 6, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_brutal_damage,   SLOT( 0),        0,     0,
        "",                     "!Brutal Damage!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "critical strike",
        /* 1,  2, 3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,30,25, 1, 4, 1,11,13, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_critical,   SLOT( 0),        0,     0,
        "",                     "!Critical Strike",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"sharpen weapon",
	/* 1,  2,  3,  4, 5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,103,40,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  5,  7, 9,  9,  9,  9,  9, 9, 9, 7, 7, 9, 9, 9, 9, 9},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_sharpen,		SLOT( 0),	0,	12,
	"",			"!Sharpen Weapon!", "", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"envenom",
	/* 1,  2,  3, 4,  5,  6,  7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,10,103,103,103,25,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  8,  9, 7,  5,  9,  9, 9, 9, 9, 9, 5, 7, 9, 9, 7, 8, 9},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	12,
	"",			"!Envenom!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"hand to hand",
	/* 1, 2, 3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 1*/		 
	{ 25,10,15, 6, 7,14, 1,14, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6, 7, 5, 7, 9, 9, 9, 9, 9, 9, 9, 7, 7, 9, 9, 8, 7, 7},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "kick",                 
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,12, 14, 8,103,9, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  3,  5,  5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 5, 7},
        {0,0,0,0,0},spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
        "kick",                 "!Kick!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "feign death",                 
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,12, 14, 8,103,9, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  3,  5,  5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 5, 5},
        {0,0,0,0,0},spell_null,             TAR_CHAR_SELF,     POS_FIGHTING,
        &gsn_feign_death,              SLOT( 0),        0,     24,
        "",                 "",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "cloak of fear",                 
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,12, 14, 8,103,9, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  3,  5,  5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 5, 4},
        {0,0,0,0,0},spell_cloak_of_fear,             TAR_CHAR_DEFENSIVE,     POS_FIGHTING,
        &gsn_cloak_fear,              SLOT( 0),        75,     12,
        "",                 "You seem less intimidating",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "parry",                
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,20, 13, 1, 1, 1,11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  7, 7, 9, 9, 9, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
        "",                     "!Parry!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

  /*  {
        "mounted fighting",
        // 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8        

        { 103,20, 13, 1, 1, 1,11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  7, 7, 9, 9, 9, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mounted_fighting,             SLOT( 0),        0,     0,
        "",                     "!Parry!",              "", KINGDOM_NONE,
        GROUP_FIGHTMASTER
    },
*/
    {
        "rescue",               
        /* 1,  2,  3, 4, 5, 6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,103,103,1, 1,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  5, 7, 9, 9,  9,  9, 9, 9, 9, 8, 8, 9, 9, 9, 7, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
        "",                     "!Rescue!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"trip",			
	/* 1,  2,  3, 4, 5,  6,7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 1,15,103,10,8,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  3,  5,  5, 5, 4,  7,7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 5, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	12,
	"trip",			"!Trip!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
	"submission",			
	/* 1,  2,  3, 4, 5,  6,7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 1,15,103,10,8,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  3,  5,  5, 5, 4,  7,7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 5, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_submission,	SLOT( 0),	0,	10,
	"armlock",			"You break out of the armlock",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "extra attack",        
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 30, 24, 12, 5, 6, 7,10, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_extra_attack,     SLOT( 0),        0,     0,
        "",                     "!Extra Attack!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "second attack",        
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 30, 24, 12, 5, 6, 7,10, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  3,  5,  5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 6, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
        "",                     "!Second Attack!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "third attack",         
        /* 1,  2,   3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,103, 24,12,13,13,16,25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 4,   6,   6, 6, 8, 8, 8, 8, 8, 8, 8, 6, 6, 8, 8, 8, 7, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
        "",                     "!Third Attack!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    {
        "fourth attack",  
        /* 1,  2,  3,  4, 5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 103,103,103,30,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  5,  7,  7,  7,  8, 8,  8,  8, 8, 8, 8, 6, 6, 8, 8, 8 ,7, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fourth_attack,     SLOT(0),        0,      0,
        "",                     "!Fourth Attack!", 	"",
	KINGDOM_NONE,	GROUP_FIGHTMASTER
    },
 {
        "fifth attack",  
      /* 1,  2,  3,  4, 5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
      { 103,103,103,30,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {  5,  7,  7,  7, 8,  8,  8,  8,  8, 8, 8, 6, 6, 8, 8, 8, 7, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fifth_attack,     SLOT(0),        0,      0,
        "",                     "!Fifth Attack!",      "",
        KINGDOM_NONE,     GROUP_FIGHTMASTER
    },
 {
        "sixth attack",  
      /* 1,  2,  3,  4, 5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
      { 103,103,103,30,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {  5,  7,  7,  7, 8,  8,  8,  8,  8, 8, 8, 6, 6, 8, 8, 8, 7, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sixth_attack,     SLOT(0),        0,      0,
        "",                     "!Sixth Attack!",      "",
        KINGDOM_NONE,     GROUP_FIGHTMASTER
    },

{
        "7 attack",  
      /* 1,  2,  3,  4, 5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
      { 103,103,103,30,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {  5,  7,  7,  7, 8,  8,  8,  8,  8, 8, 8, 6, 6, 8, 8, 8, 7, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_7_attack,     SLOT(0),        0,      0,
        "",                     "!Sixth Attack!",      "",
        KINGDOM_NONE,     GROUP_FIGHTMASTER
    },

{
        "8 attack",  
      /* 1,  2,  3,  4, 5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
      { 103,103,103,30,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {  5,  7,  7,  7, 8,  8,  8,  8,  8, 8, 8, 6, 6, 8, 8, 8, 7, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_8_attack,     SLOT(0),        0,      0,
        "",                     "!Sixth Attack!",      "",
        KINGDOM_NONE,     GROUP_FIGHTMASTER
    },

{
        "9 attack",  
      /* 1,  2,  3,  4, 5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
      { 103,103,103,30,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {  5,  7,  7,  7, 8,  8,  8,  8,  8, 8, 8, 6, 6, 8, 8, 8, 7, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_9_attack,     SLOT(0),        0,      0,
        "",                     "!Sixth Attack!",      "",
        KINGDOM_NONE,     GROUP_FIGHTMASTER
    },
/* non-combat skills */

    {
	"fast healing",		
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 15,  9, 16, 6, 7, 7, 9, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  5,  7,  7, 6, 6, 7, 7, 5, 7, 5, 5, 5, 6, 7, 7, 6, 7, 5},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	"", KINGDOM_NONE,
	GROUP_HEALING
    },

    {
	"haggle",		
	/* 1,  2,  3, 4, 5,6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{  7, 18,  1,14,12,8,17,15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7, 6, 5,7, 6, 5, 7, 5, 5, 5, 6, 7, 7, 6, 7, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"hide",			
	/* 1,  2,  3, 4,  5,  6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 1,103,103,103,4,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  7,   8, 8,  5,  8, 6, 5, 7, 9, 9, 5, 7, 8, 8, 9, 9, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	18,
	"",			"!Hide!",		"", KINGDOM_NONE,
	GROUP_STEALTH
    },

    {
	"lore",
	/* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,18,21,19,20,19,18, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  7,  7,  7, 6, 5, 7, 6, 5, 7, 5, 5, 5, 6, 7, 7, 7, 7, 7},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	"",			"!Lore!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"meditation",		
	/* 1,  2,  3, 4,  5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{  6,  6, 15, 15,14,15,15,16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7, 9, 7, 6, 5, 7, 3, 5, 5, 6, 5, 5, 6, 8, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		"", KINGDOM_NONE,
	GROUP_WIZARD
    },

    {
	"peek",
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 1,103,103,103,103,103,1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  7,  8, 8,  5,  8,  6,  5, 7,  9, 9, 5, 7, 8, 8, 9, 9, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"pick lock",		
	/* 1,  2,  3, 4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 7,103,103,10, 14, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  7,  8, 8,  5,  8,  6,  5,  7, 9, 9, 5, 7, 8, 8, 9, 9, 9},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"relock",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103,10,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  7,   8, 8,  5,  8,  6,  5, 7, 9, 9, 5, 7, 8, 8, 9, 9, 9},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_relock,		SLOT( 0),	 0,	12,
	"",			"!Relock!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"sneak",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
	{ 103,103, 4,103,103,103, 4, 103,1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  7,  8, 8,  5,  8,  6,  5, 7,  9, 9, 5, 7, 8, 8, 9, 7, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"You no longer feel stealthy.",	"",
	KINGDOM_NONE, GROUP_STEALTH
    },

    {
	"steal",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,103, 5,103,103,103,103,103,1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8, 8,  6,  8,  7,  7, 7,  9, 9, 8, 8, 8, 8, 8, 8, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	12,
	"",			"!Steal!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"scrolls",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1,  1,  1,103,103, 1, 103, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8, 8,  7,  7,  7,  6, 7,  5, 8, 7, 7, 5, 6, 7, 7, 6},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	12,
	"",			"!Scrolls!",		"", KINGDOM_NONE,
	GROUP_WIZARD
    },

    {
	"staves",		
	/* 1,  2,  3,  4,  5,  6,  7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1,  1, 103,103, 1, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8,  8,  7,  7,  7, 6, 7, 5, 8, 7, 7, 5, 6, 7, 8, 7},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		"", KINGDOM_NONE,
	GROUP_WIZARD
    },

    {
	"wands",		
	/* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1,  1, 103,103,103, 1, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8,  8,  7,  7,  7,  6,  7, 5, 8, 7, 7, 5, 6, 7, 7, 6},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		"", KINGDOM_NONE,
	GROUP_WIZARD
    },

    {
	"recall",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  1,  1,  1, 1,  1,  1,  1,  1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9, 9,  9,  9,  9,  9, 9,  9, 9, 9, 9, 9, 9, 9, 9, 9},
	{1,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	24,
	"",			"!Recall!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "wanted",               
       /* 1,  2,  3, 4,  5, 6, 7, 8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 1,  1,  1,  1, 1, 1, 1, 1 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 5,  5,  5,  5, 5, 5, 5, 5,  5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_wanted,                   SLOT(0),        0,       0,
        "",                     "!Wanted!", "",	KINGDOM_KAZMIR, 
	GROUP_KINGDOM
    },

    {
        "poison smoke",         
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,20, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8,  8,  7,  7,  7,  8,  7, 7, 8, 5, 7, 5, 6, 7, 8, 6},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_poison_smoke,	SLOT( 0),	20,	12,
	"poison smoke",		"!Poison Smoke!", "", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },

    {
        "blindness dust",         
        {  103, 103, 103, 103, 103, 103, 18, 103 , 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  8,  8,  8,  7,  7,  7,  8,  7,  7, 8 ,  5,  7,  5,  6, 6},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_blindness_dust,	SLOT( 0),	20,	12,
	"blindness dust",	"!Blindness Dust!", "", KINGDOM_NONE,
	GROUP_MALADICTIONS
    },

    {
        "dual backstab",  
        /* 1,  2,  3, 4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,30,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8, 8,  5,  7,  7,  8,  7, 7, 8, 6, 7, 9, 9, 9, 9, 7},
        {0,0,0,0,0},spell_null,             TAR_CHAR_OFFENSIVE,           POS_STANDING,
        &gsn_dual_backstab,       SLOT(0),        0,       0,
        "second backstab",      "!dual backstab!", "",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },
    
    {
        "double whirlwind",
	/* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {  6, 7, 7, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 9, 9, 8},
        {0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_double_whirlwind,		SLOT(0),	0,	0,
        "WhirlWind",		"!Whirlwind!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "dual wield",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,30,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8,  8,  7,  7, 7,  8, 7, 7, 8, 5, 7, 5, 6, 7, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,           POS_RESTING,
        &gsn_dual_wield,        SLOT(0),        0,       0,
        "",      "!dual wield!", "",		KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "iceball",         
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,32, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8,  6,  6,  8, 3, 8, 8, 8, 3, 4, 6, 7, 7},
        {0,0,0,0,0},spell_iceball,     TAR_IGNORE,     POS_FIGHTING,
        NULL,                   SLOT(513),        15,       12,
        "iceball",         "!Iceball!", "",	KINGDOM_NONE,
	GROUP_COLD
    },

    {
        "cleave",         
         /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103,103,103,103,103,25, 103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {   6,  7,  7,  7,  9,  9,  9,  9,  7, 9, 9, 8, 8, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,     TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_cleave,         SLOT(0),        0,       24,
        "cleave",         "!Cleave!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "counter",         
       /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,25, 30, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 6,  7,  7,  7,  9,  9,  9,  9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_counter,         SLOT(0),        0,       0,
        "",         "!Counter!", "",		KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "camouflage",         
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,103, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  7,  9,  9,  9, 9, 9, 6, 8, 8, 9, 9, 7 ,8, 9},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_camouflage,         SLOT(0),        0,       18,
        "",         "!Camouflage!", "",	KINGDOM_NONE,
	GROUP_STEALTH
    },

    {
        "ambush",      
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,103,10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  7,  9,  9,  9, 9, 9, 6, 8, 8, 9, 9, 8, 9, 8},
        {0,0,0,0,0},spell_null,        TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_ambush,                    SLOT(0),        0,       12,
        "surprise attack", "!Ambush!", "",	KINGDOM_NONE,
	GROUP_STEALTH
    },

    {
        "circle",         
       /* 1,  2,  3, 4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,10,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  8,  8,  8, 5,  7,  7,  8,  7, 7, 8, 6, 7, 9, 9, 8, 9, 8},
        {0,0,0,0,0},spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_circle,             SLOT(0),        0,       12,
        "circle stab", "!Circle!", "",		KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "nerve",         
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,20, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8,  8,  6,  7,  7,  8,  7, 7, 8, 5, 7, 9, 9, 8, 7, 8},
        {0,0,0,0,0},spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_nerve,             SLOT(0),        0,       18,
        "", "Your nerves feel better.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "endure",
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,10, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8,  8,  5,  7,  7,  8,  7, 7, 8, 6, 4, 9, 9, 7, 7, 8},
        {0,0,0,0,0},spell_null,        TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_endure,             SLOT(0),        0,       24,
        "", "You feel susceptible to magic again.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "quiet movement", 
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         {103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 9,  8,  8,  8,  5,  7,  7,  8, 7, 7, 6, 6, 7, 9, 9, 6, 7, 8},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_quiet_movement,             SLOT(0),        0,       0,
        "", "!quiet movement!", "",	KINGDOM_NONE,
	GROUP_STEALTH
    },
    {
        "forestry", 
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         {103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 9,  8,  8,  8,  5,  7,  7,  8, 7, 7, 6, 6, 7, 9, 9, 6, 7, 8},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_forestry,             SLOT(0),        0,       0,
        "", "!forestry!", "",	KINGDOM_NONE,
	GROUP_STEALTH
    },
    {
        "reaction", 
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         {103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 5,  5,  5,  5,  5,  5,  5,  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_reaction,             SLOT(0),        0,       0,
        "", "!reaction!", "",	KINGDOM_NONE,
	GROUP_STEALTH
    },
    {
        "mountaineer", 
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         {103,103,103,103,103,103,103, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 9,  8,  8,  8,  5,  7,  7,  8, 7, 7, 6, 6, 7, 9, 9, 6, 7, 7},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_mountaineer,             SLOT(0),        0,       0,
        "", "!mountaineer!", "",	KINGDOM_NONE,
	GROUP_STEALTH
    },

    {
        "herbs",         
       /* 1,  2,  3, 4,   5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,103,10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9, 9, 9, 6, 9, 9, 9, 9, 6 ,9, 8},
        {0,0,0,0,0},spell_null,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        &gsn_herbs,             SLOT(0),        0,       18,
        "", "The herbs look more plentiful here.", "",	KINGDOM_NONE,
	GROUP_HEALING
    },

    {
        "tame",        
       /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,103,15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9, 9, 9, 6, 9, 9, 9, 9, 6, 9, 9},
        {0,0,0,0,0},spell_null,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        &gsn_tame,             SLOT(0),        0,       18,
        "", "!tame!", "",	KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "butcher",         
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,103, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,   9, 9, 9, 6, 9, 9, 9, 9, 7 ,9, 8},
        {0,0,0,0,0},spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_butcher,             SLOT(0),        0,       0,
        "", "!butcher!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "bark skin",         
       /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,103,15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9, 9, 9, 6, 9, 9, 9, 9, 6, 8, 9},
        {0,0,0,0,0},spell_null,        TAR_CHAR_SELF,          POS_STANDING,
        &gsn_barkskin,             SLOT(515),        40,       0,
        "", "The bark on your skin flakes off.", "",	KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "bear call",        
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         {103,103,103,103,103,103,103,23, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         { 9,  9,  9,  9,  9,  9,  9,  9, 9, 9, 6, 9, 9, 9, 9, 6 ,9, 8},
        {0,0,0,0,0},spell_null,		TAR_IGNORE,          POS_FIGHTING,
        &gsn_bear_call,		SLOT(518),	50,       0,
        "", "You feel you can handle more bears now.", "",
	KINGDOM_NONE, GROUP_NATURE
    },

    {
        "ranger staff",         
       /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,103, 30,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9, 9, 9, 6, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_ranger_staff,      TAR_IGNORE,          POS_FIGHTING,
        NULL,             SLOT(519),        75,       0,
        "", "!ranger staff!", "", 	KINGDOM_NONE,
	GROUP_NATURE
    },


    {
        "animate dead",            
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 25, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  7,  9,  7,  9,  6,  6, 6, 9, 9, 9, 6, 9, 7, 9, 5},
        {0,0,0,0,0},spell_animate_dead,        TAR_IGNORE,             POS_STANDING,
        &gsn_animate_dead,                SLOT(577),      100,    15,
        "",  "You can create one more zombie now!",          "",
	KINGDOM_NONE, GROUP_NECRO
    },

    {
        "assassinate",         
       /*  1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,35, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  8,  8,  8,  5,  7,  7,  8,  7, 7, 8, 6, 7, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,          POS_STANDING,
        &gsn_assassinate,       SLOT(0),        0,       18,
        "assassination attempt", "!assassinate!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "hellfire",         
       /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,35,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  8,  7,  9,  7,  7,  5,6, 5, 5, 9, 7, 4, 5, 5, 7, 9},
        {0,0,0,0,0},spell_hellfire,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        NULL,       SLOT(520),        20,       12,
        "hellfire", "!hellfire!", "",	KINGDOM_NONE, GROUP_FIRE
    },

    {
        "caltraps",         
       /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,13, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  8,  8,  8,  7,  7,  7,  8,  7, 7, 8, 7, 5, 7, 6, 7, 7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,          POS_FIGHTING,
        &gsn_caltraps,       SLOT(0),        0,       18,
        "caltraps", "Your feet feel less sore.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "vanish",         
        /* 1,  2,  3,  4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103, 103,103,103,103,16,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  8,  8,  8,  7,  7,  7,  8,  7,  7,8, 7, 5, 7, 6, 7, 7, 9},
        {0,0,0,0,0},spell_null,      TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_vanish,       SLOT(0),        25,       18,
        "", "!vanish!", "", 	KINGDOM_NONE, GROUP_STEALTH
    },

    {
        "strangle",         
        /* 1,  2,   3,  4,  5,  6,  7, 8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103, 103,103,103,103,103,20,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  8,  8,  8,  7,  6,  7,  8,  7,  7, 8, 7, 5, 9, 9, 9 ,7, 9},
        {0,0,0,0,0},spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_strangle,       SLOT(0),        0,       8,
        "strangulation", "Your neck feels better.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "throw",         
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,15, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 7,  8,  8,  8,  7,  6,  7,  8,  7,  7, 8, 7, 5, 9, 9, 8, 6, 9},
        {0,0,0,0,0},spell_null,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_throw,       SLOT(0),        0,       18,
        "throw", "!throw!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "bloodthirst",         
        /* 1,  2,  3, 4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 15, 15, 15, 15, 15, 15, 15, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 7,  8,  8,  8,  7,  6,  7,  8,  7, 7, 8, 7, 5, 9, 9, 8, 7, 8},
        {0,0,0,0,0},spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_bloodthirst,       SLOT(0),        0,       12,
        "", "Your bloody rage fades away.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
// ex-horde skill
    },

    {
        "spellbane", 
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 10, 10, 10,10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  6,  7,  8, 7, 7, 8, 7, 7, 5, 5, 7, 7, 8},
        {0,0,0,0,0},spell_null,      TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spellbane,       SLOT(0),        0,       12,
        "spellbane", "You feel less resistant to magic.","",KINGDOM_NONE,
	GROUP_FIGHTMASTER
//ex-horde skill
    },

    {
        "resistance",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 12, 12, 12,12, 12, 12, 12, 12, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  6,  7,  8, 7, 7, 8, 7, 7, 5, 5, 7, 7, 9},
        {0,0,0,0,0},spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_resistance,       SLOT(0),        0,       24,
        "", "You feel less tough.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
//ex-horde skill
    },

    {
        "deathblow",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 20, 20, 20,20, 20, 20, 20, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  6,  7,  8, 7, 7, 8, 7, 7, 5, 5, 7, 7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_deathblow,       SLOT(0),        0,       0,
        "", "!deathblow!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
// ex-horde skill
    },

    {
        "transform",         
        /* 1,  2,  3, 4,  5,  6,  7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 10, 10, 10,10, 10, 10, 10,10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  7,  7, 8, 7, 7, 8, 7, 7, 7, 5, 7, 7, 8},
        {0,0,0,0,0},spell_transform,      TAR_CHAR_SELF,       POS_STANDING,
        NULL,       SLOT(522),        100,       24,
        "", "You feel your healthy form lessen.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    {
        "mana transfer",         
       /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {10, 10, 10,10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8, 7,  7,  7,  8, 7, 6, 8, 7, 7, 7, 5, 7, 7, 8},
        {0,0,0,0,0},spell_mana_transfer,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(900),        50,       24,
        "", "!mana transfer!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "mental knife",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 15, 15, 15,103,15, 15, 15, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  7,  7,  8, 7, 5, 8, 7, 7, 6, 5, 7, 7, 8},
        {0,0,0,0,0},spell_mental_knife,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        NULL,       SLOT(524),        35,       12,
        "mental knife", "Your mental pain dissipates.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "celestial spirit",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 25, 25, 25,103,25, 25, 25, 25, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  7,  7,  8, 7,  5, 8, 7, 7, 6, 5, 7, 7, 8},
        {0,0,0,0,0},spell_celestial_spirit,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_celestial_spirit,       SLOT(525),        100,       12,
        "", "You feel your celestial copula return.","",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "scourge",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 30, 30, 30,103,30, 30, 30, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  8,  7,  7,  7,  8,  7, 5, 8, 7, 7, 6, 5, 7, 7, 8},
        {0,0,0,0,0},spell_scourge,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(526),        40,       12,
        "illuminated scourge", "!scourge!", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "manacles",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 20, 20, 20,20, 20, 20, 20, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  8,  7,  7,  7,  8, 7, 5, 8, 7, 7, 6, 5, 7, 7, 8},
        {0,0,0,0,0},spell_manacles,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        NULL,       SLOT(528),        75,       12,
        "", "Your shackles dissolve.", "", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "shield of justice",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 10, 10, 10,10, 10, 10, 10, 10, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 7,  7,  6,  8,  7,  7,  7,  8, 7,  5, 8, 7, 7, 6, 5, 1 ,1, 5},
        {0,0,0,0,0},spell_shield_justice,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(529),        60,       12,
        "", "!shield!", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "guard",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 22, 22, 22,22, 22, 22, 22, 22, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  5,  8,  7,  7,  7,  8, 7, 7, 8, 7, 7, 9, 7, 7, 7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_guard,       SLOT(0),        0,       12,
        "", "", "",	KINGDOM_NONE, GROUP_FIGHTMASTER
    },

    {
        "guard call",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 15, 15, 15,15, 15, 15, 15, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  5,  8,  7,  7,  7,  8, 7, 7, 8, 7, 7, 9, 7, 7, 7, 8},
        {0,0,0,0,0},spell_guard_call,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_guard_call,       SLOT(1030),        75,       12,
        "", "You can shift more guards now.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "nightwalker",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 25, 25, 25,25, 25, 25, 25, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  5,  7,  6,  7,  8, 7, 7, 8, 7, 7, 5, 7, 1, 1, 7},
        {0,0,0,0,0},spell_nightwalker,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_nightwalker,       SLOT(1031),        75,       12,
        "", "You feel your summoning power return.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
//        "eyes of intrigue",         
        "spynetwork",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 13, 13, 13,13, 13, 13, 13, 13, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  5,  7,  6,  6,  8, 7, 5, 8, 7, 7, 5, 7, 7, 7, 8},
        {0,0,0,0,0},spell_eyes,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(1032),        75,       12,
        "", "!eyes of intrigue!", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "fade",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 20, 20, 20,20, 20, 20, 20, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7, 5, 8, 7, 7, 5, 5, 7, 7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_fade,       SLOT(0),        0,       24,
        "", "!fade!", "",KINGDOM_NONE, GROUP_FIGHTMASTER
    },

    {
        "shadow cloak",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 10, 10, 10,10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7, 5, 8, 7, 7, 5, 5, 7, 7, 7},
        {0,0,0,0,0},spell_shadow_cloak,      TAR_CHAR_SELF,       POS_STANDING,
        NULL,       SLOT(1033),        10,       12,
        "", "The shadows no longer protect you.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    {
        "nightfall",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 16, 16, 16,16, 16, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7, 5, 8, 7, 7, 5, 5, 7, 7, 8},
        {0,0,0,0,0},spell_nightfall,      TAR_IGNORE,       POS_STANDING,
        NULL,       SLOT(1034),        50,       12,
        "nightfall", "You are now able to control lights.","",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

   {
/*	"shadow walk",	    {  40,40,40,40,40,40,40,40, 1, 1, 1, 1, 1, 1, 1}, */
	"shadow walk",		
	/* 1,  2,  3, 4,  5,  6,   7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{ 8,  8,  8,  6,  4,  6,  7,  8,   7, 5, 8, 7, 7, 5, 5, 7 ,7, 8},
	{0,0,0,0,0},spell_shadow_walk,	TAR_IGNORE,		POS_STANDING,
	NULL,	   SLOT(578),	100,	12,
	"", "!shadow walk!",	"", KINGDOM_NONE,
	GROUP_NECRO
    },

    {
        "garble",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 10, 10, 10,10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7, 5, 8, 7, 7, 5, 5, 7 ,7, 9},
        {1,0,0,0,0},spell_garble,      TAR_CHAR_DEFENSIVE,       POS_FIGHTING,
        &gsn_garble,       SLOT(1035),        30,       12,
        "", "Your tongue untwists.", "",KINGDOM_NONE, GROUP_NECRO
    },

    {
        "mirror",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 15, 15, 15,15, 15, 15, 15, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7, 5, 8, 7, 7, 5, 5, 7, 7, 8},
        {0,0,0,0,0},spell_mirror,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
        &gsn_mirror,       SLOT(1036),        40,       12,
        "", "You fade away.", "",KINGDOM_NONE, GROUP_NECRO
    },

    {
        "confuse",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 20, 20, 20,20, 20, 20, 20, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7, 5, 8, 7, 7, 5, 5, 7, 7, 8},
        {1,0,0,0,0},spell_confuse,      TAR_CHAR_DEFENSIVE,       POS_FIGHTING,
        &gsn_confuse,       SLOT(1037),        20,       12,
        "", "You feel less confused.", "",KINGDOM_NONE, GROUP_NECRO
    },

    {
        "doppelganger",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 25, 25, 25,25, 25, 25, 25, 25, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8, 7,  5, 8, 7, 7, 5, 5, 7, 7, 9},
        {0,0,0,0,0},spell_doppelganger,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
        &gsn_doppelganger,       SLOT(527),        75,       12,
        "", "You return to your natural form.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "hunt",    
       /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
       { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  8,  7,  8,  8,  8,   7, 8, 5, 6, 9, 9, 8, 7, 7, 7},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_hunt,       SLOT(0),        0,       6,
        "", "!hunt!", "",	KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "world find",    
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,   9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_world_find,       SLOT(0),        0,       0,
        "", "!world find!", "",	KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "track",    
       /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
       { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  8,  7,  8,  8,  8,  7,  8, 5, 6, 9, 9, 8, 6, 9, 9},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_track,       SLOT(0),        0,       12,
        "", "!track!", "",	KINGDOM_NONE,
	GROUP_NATURE
    },
    {
        "riding",    
       /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
       { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  8,  7,  8,  8,  8,  7,  8, 8, 8, 9, 9, 8, 8, 8, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_riding,       SLOT(0),        0,       12,
        "", "!riding!", "",	KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "panic",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  30, 30, 30,30,30,30,30,30 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 7 ,7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_panic,       SLOT(0),        80,       12,
        "", "You are not panicked any more.", "",KINGDOM_KAZMIR,
	GROUP_KINGDOM
    },
    {
        "lightning shield",    
       /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
       { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 9,  9,  9,  8,  8,  8,  7,  7,  8,  5, 9, 9, 8, 6, 5, 7 ,9, 8},
        {0,0,0,0,0},spell_lightning_shield,      TAR_IGNORE,       POS_STANDING,
        NULL,       SLOT(614),        150,       24,
        "lightning shield", "Now you can shield your room again.", "",
	KINGDOM_NONE, GROUP_LIGHTNING
    },
    {
        "shocking trap",    
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 9,  9,  9,  7,  9,  9,  5,  8,  7,  4, 9, 9, 8, 4, 4, 5, 8, 8},
        {0,0,0,0,0},spell_shocking_trap,      TAR_IGNORE,       POS_STANDING,
        NULL,       SLOT(615),        150,       24,
        "shocking trap", "Now you can trap more rooms with shocks.", "",
	KINGDOM_NONE, GROUP_LIGHTNING
    },
    {
        "dirt kicking",    
        /* 1,  2,  3,  4,  5,  6,  7,  8   9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 6,  6,  8,  8,  5,  7,  7,  8,  7,  7, 6, 6, 7, 9, 9, 7, 6, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_dirt,       SLOT(0),        0,       12,
        "dirt", "The dirt in your eyes goes away.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },


    {
        "amnesia",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9, 9 ,9, 9},
        {1,1,0,0,0},spell_amnesia,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
        NULL,       SLOT(1038),        100,       12,
        "", "!amnesia!", "",	KINGDOM_NONE,
	GROUP_BEGUILING
    },

    {
        "chaos blade",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 23, 23, 23,23, 23, 23, 23, 23, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8,  7,5, 8 ,7, 7, 5, 5, 7 ,7, 8},
        {0,0,0,0,0},spell_chaos_blade,      TAR_IGNORE,       POS_STANDING,
        &gsn_chaos_blade,       SLOT(550),        60,       12,
        "", "!chaos blade!", "",KINGDOM_NONE,
	GROUP_KINGDOM
    },

    {
        "brand",    
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103, 103, 103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9, 9 ,9, 9},
        {0,0,0,0,0},spell_brand, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(551),        10,       0,
        "", "!brand!", "",	KINGDOM_NONE,
	GROUP_CREATION
    },

    {
        "remove brand",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103, 103, 103,103,103,103,103,103 , 1, 1, 1, 1,1, 1, 1, 1, 1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9, 9 ,9, 9},
        {0,1,0,0,0},spell_remove_brand, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(552),        10,       0,
        "", "!remove brand!", "",	KINGDOM_NONE,
	GROUP_CREATION
    },

    {
        "wrath",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103, 103, 103,103,20,103,103,103 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 7 ,9, 9},
        {0,0,0,0,0},spell_wrath, TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_wrath,       SLOT(5103),        20,       12,
        "heavenly wrath", "The curse wears off.", "",	KINGDOM_NONE,
	GROUP_ATTACK
    },

   {
        "stalker",         
         /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  23, 23, 23,23,23,23,23,23 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
       { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 7 ,7, 8},
        {0,0,0,0,0},spell_stalker,      TAR_IGNORE,       POS_STANDING,
        &gsn_stalker,       SLOT(554),        100,       12,
        "", "You feel up to summoning another stalker.", "",KINGDOM_NONE,
	GROUP_CREATION
    },

    {
        "randomizer",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  28, 28, 28,28,28,28,28,28 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
       { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 7 ,7, 9},
        {0,0,0,0,0},spell_randomizer,      TAR_IGNORE,       POS_STANDING,
        &gsn_randomizer,       SLOT(555),        200,       24,
        "", "You feel your randomness regenerating.", "",KINGDOM_KAZMIR,
	GROUP_KINGDOM
    },

    {
        "tesseract",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  29, 29, 29,103,29,29,29,29 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
        { 9,  9,  8,  8,  9,  9,  8,  8,  8,  8, 9 ,  9,  8,  8, 8, 9 ,9, 8 },
        {0,0,0,0,0},spell_tesseract,      TAR_IGNORE,       POS_STANDING,
        &gsn_tesseract,       SLOT(556),        150,       12,
        "", "!tesseract!", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "trophy",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  23, 23, 23,23,23,23,23,23 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 5,  5,  9,  9,  9,  9,  9,  9,  6,  9, 9 ,  6,  6,  9,  9, 9 ,9, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_trophy,       SLOT(0),        30,       12,
        "", "You feel ready to make a new trophy.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
//ex-horde skill

    {
        "truesight",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
       { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 5, 9, 9},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_truesight,       SLOT(0),        50,       12,
        "", "Your eyes see less truly.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
// ex-horde skill
    },

    {
        "brew",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  23, 23, 23,23,23,23,23,23 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 7 ,7, 8},
        {0,0,0,0,0},spell_brew,      TAR_OBJ_INV,       POS_STANDING,
        &gsn_brew,       SLOT(557),        25,       12,
        "", "You feel like you can start brewing again.", "",KINGDOM_NONE,
	GROUP_WIZARD
    },

    {
        "shadowlife",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  30, 30, 30,30,30,30,30,30 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 8,  8,  8,  6,  7,  6,  7,  8,  7,  5, 8 ,  7,  7,  5,  5, 7 ,7, 8},
        {0,0,0,0,0},spell_shadowlife,      TAR_IGNORE,       POS_STANDING,
        &gsn_shadowlife,       SLOT(558),        80,       12,
        "", "Your feel more shadowy.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "justice badge",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_justice_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_justice_badge,       SLOT(560),        50,       12,
        "", "!justice badge!", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "remove badge",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  28, 28, 28,28,28,28,28,28 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7, 7, 8},
        {0,0,0,0,0},spell_remove_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_remove_badge,       SLOT(561),        100,       12,
        "", "!remove badge!", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "dragon strength", 
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 9},
        {0,0,0,0,0},spell_dragon_strength, TAR_CHAR_SELF, POS_FIGHTING,
        &gsn_dragon_strength, SLOT(562),    75,    12,
        "", "You feel the strength of the dragon leave you.", "",
	KINGDOM_NONE, GROUP_ENHANCEMENT
    },

    {
        "dragon breath", 
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {103,103,103,103,103,103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 9,  9,  9,  9,  9,  9,  9,  9,  9,  5, 8 ,  9,  9,  7,  7, 7, 7, 8},
        {0,0,0,0,0},spell_dragon_breath,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_dragon_breath,  SLOT(563),   75,   12,
        "", "Your throat clears up.", "",	KINGDOM_NONE,
	GROUP_ATTACK
    },

    {
        "golden aura",        
         /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
         {  10,10,10,10,10,10,10,10 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7 ,7, 8},
        {0,0,0,0,0},spell_golden_aura,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(564),        25,       12,
        "", "You feel the golden aura dissipate.", "",	KINGDOM_NONE,
	GROUP_KINGDOM
    },

    {
        "dragonplate",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  15,15,15,15,15,15,15,15 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7, 7, 8},
        {0,0,0,0,0},spell_dragonplate,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(565),        60,       12,
        "", "", "",	KINGDOM_NONE, GROUP_FIGHTMASTER
    },

    {
        "squire",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_squire,      TAR_IGNORE,   POS_STANDING,
        &gsn_squire,       SLOT(566),        100,       12,
        "", "You feel up to worrying about a new squire.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    {
        "earth elemental",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_earth_elemental,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(600),        150,       24,
        "", "You feel your elemental of earth returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    {
        "air elemental",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_air_elemental,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(601),        100,       12,
        "", "You feel your elemental of air returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    {
        "fire elemental",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_fire_elemental,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(602),        125,       24,
        "", "You feel your elemental of fire returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    {
        "water elemental",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_water_elemental,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(603),        75,       12,
        "", "You feel your elemental of water returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    
        {
        "white tiger",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_white_tiger,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(983),        75,       12,
        "", "You feel your white tiger returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    
        {
        "forest wolf",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_wolf,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(984),        75,       12,
        "", "You feel your wolf returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    
        {
        "forest bear",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_forest_bear,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(985),        75,       12,
        "", "You feel your bear returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },
    
        {
        "forest lion",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25, 25, 25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5, 7,7, 8},
        {0,0,0,0,0},spell_lion,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(986),        75,       12,
        "", "You feel your lion returning.","",KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "dragonsword",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  25,25,25,25,25,25,25,25 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 7,  7,  7,  6,  7,  6,  7,  6,  7,  5, 6 ,  7,  7,  5,  5,7,7, 8},
        {0,0,0,0,0},spell_dragonsword,      TAR_IGNORE,   POS_STANDING,
        &gsn_dragonsword,       SLOT(567),        70,       12,
        "", "", "",	KINGDOM_NONE, GROUP_FIGHTMASTER
    },
    {
        "warcry",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 103,20,103,103,103,20 , 1, 1, 1, 1, 1, 1, 1, 1,1, 1},
        { 6,  6,  7,  7,  9,  8,  9,  9,  6,  9, 7 ,  8,  7,  9,  9,9,7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_warcry,       SLOT(0),        30,       12,
        "", "Your warcry has worn off.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "stoneform",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 103,20,103,103,103,20 , 1, 1, 1, 1, 1, 1, 1, 1,1, 1},
        { 6,  6,  7,  7,  9,  8,  9,  9,  6,  9, 7 ,  8,  7,  9,  9,9,7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_stoneform,       SLOT(0),        0,       12,
        "", "", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "lifesong",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 103,20,103,103,103,20 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  8,  7,  9,  7,  7,  7,  8,  7, 8 ,  8,  6,  6,  7,7,7, 8},
        {0,0,0,0,0},spell_lifesong,     TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(762),      75,    12,
        "",                     "!Life Song!",       "", KINGDOM_NONE,
        GROUP_HEALING
    },
    {
        "endursong",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 103,20,103,103,103,20 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  8,  7,  9,  7,  7,  7,  8,  7, 8 ,  8,  6,  6, 7,1,1, 8},
	   {0,0,0,0,0},spell_endursong,     TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(762),      50,    12,
        "",                     "!Endurance Song!",       "",KINGDOM_NONE,
        GROUP_HEALING
    },
    {
        "entangle",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103,103,103,103,103,103,103,20 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  8,  7,  9,  7,  7,  7,  8,  7, 8 ,  8,  6,  6,  7,7,7, 7},
        {0,0,0,0,0},spell_entangle,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_entangle,       SLOT(568),        40,       12,
        "entanglement", "You feel less entangled.", "",	KINGDOM_NONE,
	GROUP_NATURE
    },

    {
        "love potion",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103, 103, 103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1, 1,1, 1},
        { 9,  9,  8,  7,  9,  7,  7,  7,  8,  6, 8 ,  8,  6,  6,  7,7,7, 7},
        {1,1,1,0,0},spell_love_potion, TAR_CHAR_SELF,       POS_STANDING,
        &gsn_love_potion,  SLOT(666),        10,       0,
        "", "You feel less dreamy-eyed.", "",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "deafen",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103,103,103,103,103,22,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  8,  7,  9,  7,  7,  7,  8,  7, 8 ,  8,  6,  6,  7,7,7, 7},
        {1,1,0,0,0},spell_deafen,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_deafen,       SLOT(570),        40,       12,
        "deafen", "The ringing in your ears finally stops.", "",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },
/*
    {
        "protection heat cold",         {  103,103,103,103,103,103,103,10 , 1, 1, 1, 1, 1, 1, 1, 1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0,0,0,0,0},spell_protection_heat_cold,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_protection_heat_cold,       SLOT(571),        15,       12,
        "", "You feel less protected from the elements.", "",	KINGDOM_NONE
    },
*/
    {
        "protective shield",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  16,18,103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9,9,9, 8},
        {0,1,0,0,0},spell_protective_shield,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_protective_shield,       SLOT(572),        70,       12,
        "", "Your shield fades away.", "", KINGDOM_NONE,
	GROUP_DEFENSIVE
    },
    {
	"dimension",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 103, 103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9,9,9, 9},
	{0,0,0,0,0},spell_dimension,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_dimension,		SLOT( 575),   100,	12,
	"",			"",
	"$p turns back into three dimension.", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },

    {
        "duo dimension",         
        { 103,103,103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9,9,9, 7},
        {0,0,0,0,0},spell_duo_dimension,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_duo_dimension,       SLOT(576),       100,       12,
        "", "You turn back into three dimension.", "", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    },
    {
        "trance",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  14,15,103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  7,  9,  9,  6, 9 ,  9,  9,  6,  5,7,7, 8},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,   POS_SLEEPING,
        &gsn_trance,       SLOT(0),        0,       0,
        "", "", "",	KINGDOM_NONE, GROUP_WIZARD
    },

    {
        "disperse",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  20,20,20,20,20,20,20,20 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  7,  9,  9,  6, 9 ,  9,  9,  6,  5,7,7, 8},
        {0,0,0,0,0},spell_disperse,      TAR_IGNORE,   POS_STANDING,
        &gsn_disperse,       SLOT(573),        100,       24,
        "", "You feel up to doing more dispersing.", "",KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "disintegrate",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103,103,103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9,7,7, 8},
        {1,0,1,0,0},spell_disintegrate,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        NULL,                    SLOT(574),    100,       0,
        "", "!disintegrate!", "",	KINGDOM_NONE,
	GROUP_SUDDENDEATH
    },
    
    {
        "power word kill",         
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103,103,103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9,7,7, 7},
        {0,0,0,0,0},spell_power_word_kill,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        NULL,                    SLOT(574),    100,       18,
        "", "power word kill", "",	KINGDOM_NONE,
	GROUP_SUDDENDEATH
    },

    {
        "blackjack",		
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 20, 103, 103, 103, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  8,  8,  8,  7,  6,  7,  8,  7,  7, 8 ,  7,  5,  9,  9,9,7, 8},
        {0,0,0,0,0},spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
        &gsn_blackjack,		SLOT( 0),    0,       8,
        "blackjack",		"Your head feels better.","", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
        "gouge",		
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  103, 103, 20, 103, 103, 103, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  8,  8,  8,  7,  6,  7,  8,  7,  7, 8 ,  7,  5,  9,  9,9,7, 8},
        {0,0,0,0,0},spell_null,		TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_gouge,		SLOT( 0),    0,       12,
        "gouge",		"","", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
        "spellcraft",		
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  1, 1, 103, 103, 103, 103, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 9 ,  9,  9,  9,  9,9,9, 8},
        {0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_FIGHTING,
        &gsn_spell_craft,	SLOT(0),    0,       0,
        "spellcraft",		"!Spellcraft!","", KINGDOM_NONE,
	GROUP_WIZARD
    },

    {
	"acute vision", 	
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 103, 103, 103, 103, 8 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  7,  9,  9,  6, 9 ,  9,  9,  6,  5,6,8, 8},
	{0,0,0,0,0},spell_null,		TAR_CHAR_SELF,		POS_RESTING,
	&gsn_acute_vision,	SLOT( 0),	 10,	6,
	"",			"Your vision seems duller",
	"", KINGDOM_NONE, GROUP_DETECTION
    },

    {
	"charge",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 16, 10, 20, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 6,  7,  6,  7,  9,  9,  9,  9,  9,  9, 9 ,  7,  7,  9,  9,9,9, 7},
	{0,0,0,0,0},spell_null,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_charge,		SLOT( 0),	0,	12,
	"charge",		"!Charge!",	"",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
        {
	"breath",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 16, 10, 20, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 6,  7,  6,  7,  9,  9,  9,  9,  9,  9, 9 ,  7,  7,  9,  9,9,9, 8},
	{0,0,0,0,0},spell_null,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_breath,		SLOT( 0),	0,	12,
	"blast of flame",		"The smoke leaves your eyes.",	"",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
	"phase",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 16, 10, 20, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 6,  7,  6,  7,  9,  9,  9,  9,  9,  9, 9 ,  7,  7,  9,  9,9,9, 8},
	{0,0,0,0,0},spell_null,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_phase,		SLOT( 0),	0,	12,
	"",		"Your head seems clearer.",	"",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
    {
	"crush",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 16, 10, 20, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 6,  7,  6,  7,  9,  9,  9,  9,  9,  9, 9 ,  7,  7,  9,  9,9,9, 8},
	{0,0,0,0,0},spell_null,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_crush,		SLOT( 0),	0,	12,
	"crush",		"!crush!",	"",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
     {
	"swoop",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 103, 103, 16, 10, 20, 103, 103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 6,  7,  6,  7,  9,  9,  9,  9,  9,  9, 9 ,  7,  7,  9,  9,9,9, 7},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_swoop,		SLOT( 0),	0,	12,
	"swoop",		"!swoop!",	"",	KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },
    
     {
	"revive",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 7},
	{0,1,0,0,0},spell_revive,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT( 936),	 150,	12,
	"",	"",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"resurrection",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 7},
	{0,0,0,0,0},spell_resurrection,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT( 937),	 300,	12,
	"",	"",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"imbue",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 16, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8, 9, 7, 9, 9, 8, 7, 5, 8, 9, 8},
	{0,0,0,0,0},spell_imbue,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(938),	100,	24,
	"",			"!Imbue!",	"$p's magical aura fades.", KINGDOM_NONE,
	GROUP_ENCHANTMENT
    },
    
    {
        "blade barrier",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 7},
        {0,0,0,0,0},spell_blade_barrier,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_blade_barrier,             SLOT(939),        100,     15,
        "swirling hammers",                     "The hammers stop swirling and vanish",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER  
    },
    
    {
        "Triggered Ironwill",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 7},
        {0,0,0,0,0},spell_null,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_triggered_ironwill,             SLOT(940),        5,     12,
        "",                     "Readyed Ironwill",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER  
    },
    
    {
	"shield of the dead",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 5},
	{0,0,0,0,0},spell_shield_of_the_dead,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT( 941),	 50,	18,
	"",	"",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"raise dead",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 7},
	{0,0,0,0,0},spell_raise_dead,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT( 942),	 150,	12,
	"",	"",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"undead mastery",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 7},
	{0,0,0,0,0},spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_undead_mastery,	SLOT( 0),	 0,	0,
	"",	"",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"strengthen ether",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 4},
	{0,0,0,0,0},spell_strengthen_ether,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_strengthen_ether,	SLOT( 943),	 50,	12,
	"",	"Your connection to the etherplane is severed",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"etherbind",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 4},
	{0,0,0,0,0},spell_etherbind,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 944),	 15,	12,
	"Etherbind",			"",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"wail of banshee",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 8},
	{0,0,0,0,0},spell_wail_of_banshee,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_wail_banshee,	SLOT( 945),	 200,	18,
	"",			"Your throat clears.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"malison",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 3},
	{0,0,0,0,0},spell_malison,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_malison,		SLOT( 946),	 20,	12,
	"",			"You feel more resistant to magic.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"bone armor",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 6},
	{0,0,0,0,0},spell_bone_armor,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_bone_armor,	SLOT( 947),	 60,	12,
	"",			"Your bonearmor crumbles and fades.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"death door",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 4},
	{0,0,0,0,0},spell_death_door,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_death_door,	SLOT( 948),	 300,	12,
	"",			"You feel ready to cheat death.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"death door protect",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 9},
	{0,0,0,0,0},spell_null,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_death_door_protect,	SLOT( 949),	 20,	12,
	"",			"You feel uncertain about dying.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
    {
	"control undead",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 20, 103,103,103,103,38, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  8,  9, 9, 7, 9, 9, 8, 7, 9, 9, 9, 6},
	{0,0,0,0,0},spell_control_undead,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_control_undead,	SLOT( 950),	 20,	12,
	"",			"You feel more self-confident.",
	"", KINGDOM_NONE, GROUP_BEGUILING
    },
    
/*New kingdom spell/skill*/
    {
        "soul coil",
       /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 9,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 9, 9, 8, 8, 8, 8, 8, 8},
        {0,0,0,0,0},spell_soul_coil,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(951),25,12,
        "soul coil","!soulcoil!","",KINGDOM_NONE,GROUP_FIGHTMASTER
    },
   {
        "death coil",
       /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 9,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 9, 9, 8, 8, 8, 8, 8, 8},
        {0,0,0,0,0},spell_death_coil,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(952),25,12,
        "death coil","!deathcoil!","",KINGDOM_NONE,GROUP_FIGHTMASTER
    },
   {
        "aura of ignorance",
       /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 9,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 9, 9, 8, 8, 8, 8, 8, 9},
        {0,0,0,0,0},spell_aura_ignorance,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        NULL,SLOT(953),25,12,
        "","You are no longer affected of aura of ignorance","",KINGDOM_NONE,GROUP_WIZARD
    },                  
    {
        "aldonian trance",
        /* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  14,15,103,103,103,103,103,103 , 1, 1, 1, 1, 1, 1, 1,1,1, 1},
        { 9,  9,  9,  9,  9,  9,  7,  9,  9,  6, 9 ,  9,  9,  6,  5,7,7, 9},
        {0,0,0,0,0},spell_null,      TAR_IGNORE,   POS_SLEEPING,
        &gsn_transcendental_meditation,       SLOT(0),        0,       0,
        "", "", "",     KINGDOM_NONE, GROUP_WIZARD
    },
       {
        "Lightning Reflexes",
        /* 1,  2,  3, 4, 5,  6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 103,103, 1,13,103,103,1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  6,  7,  6, 7, 5,  8, 7, 7, 8, 8, 8, 7, 7, 8, 8, 7, 3, 5},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_lightning_reflexes,             SLOT( 0),        0,     0,
        "",                     "!Lightning Reflexes!","",KINGDOM_NONE,
        GROUP_FIGHTMASTER                                              
       },
     {
        "Wraithform",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 9},
        {0,0,0,0,0},spell_fade,            TAR_CHAR_DEFENSIVE,       POS_STANDING,
        &gsn_fade,             SLOT(954),        5,     12,
        "",                     "You are no longer a Wraith.",
        "$p fades into view.", KINGDOM_NONE, GROUP_FIGHTMASTER   },

     {
        "Ironwill",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_ironwill,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_ironwill,             SLOT(955),        5,     12,
        "",                     "You are no longer determinat.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER   }, 
        
     {
        "divine deflection",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_divine_deflection,            TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_divine_deflection,     SLOT(956),        250,     12,
        "",                     "You can now deflect ones more.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     }, 
     
     { 
        "divine deflection aura",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 7},
        {0,0,0,0,0},spell_null,            TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_aura_divine_deflection,     SLOT(957),        250,     12,
        "reflection",                     "You can now deflect ones more.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },      

     {
        "signet ring",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 6},
        {0,0,0,0,0},spell_gondor_ring,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_gondor_ring,     SLOT(958),        5,     12,
        "",                     "You can now get new signet ring.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },           

     {
        "sword of flame",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_gondor_sword,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_gondor_sword,     SLOT(959),        5,     12,
        "",                     "You can now get new sword of flame.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },                
     

     {
        "justice brooch",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_gondor_ornament,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_gondor_ornament,     SLOT(960),        5,     12,
        "",                     "You can now get new justice brooch.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },                
     

     {
        "argus dagger",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_argus_dagger,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_argus_dagger,     SLOT(961),        5,     12,
        "",                     "You can now get new argus dagger.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },                
      
      
     {
        "argus orb",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_argus_orb,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_argus_orb,     SLOT(962),        5,     12,
        "",                     "You can now get new orb.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },                

     {
        "dragon call",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 9},
        {0,0,0,0,0},spell_argus_dragon,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_argus_dragon,     SLOT(963),        5,     12,
        "",                     "You can now command a new Dragon.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },                
     
     {
        "witchelf",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_wicthelf,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_wicthelf,     SLOT(964),        5,     12,
        "",                     "You can now command a new Witchelf.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },
     {
        "form wraith",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_null,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_form_wraith,     SLOT(965),        5,     12,
        "",                     "You can now command a new Witchelf.",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },
     
     {
        "paranoia",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_paranoia,            TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_paranoia,     SLOT(966),        5,     12,
        "",                     "",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },
     {
        "war demon",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_nassius_demon,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spell_nassius_demon,     SLOT(967),        5,     12,
        "",                     "",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },
    {
        "blade dance",        
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 30, 24, 12, 5, 6, 7,10, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_blade_dance,     SLOT( 0),        0,     0,
        "",                     "!Extra Attack!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"bagsteal",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,103, 5,103,103,103,103,103,1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8, 8,  6,  8,  7,  7, 7,  9, 9, 8, 8, 8, 8, 8, 8, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_bagsteal,		SLOT( 0),	 0,	12,
	"",			"!Steal!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"bagpeek",		
	/* 1,  2,  3, 4,  5,  6,  7,  8  9,  0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103,103, 5,103,103,103,103,103,1,  1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  8,  8, 8,  6,  8,  7,  7, 7,  9, 9, 8, 8, 8, 8, 8, 8, 8},
	{0,0,0,0,0},spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_bagpeek,		SLOT( 0),	 0,	12,
	"",			"!Steal!",		"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"transfer object",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 16, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8, 9, 7, 9, 9, 8, 7, 5, 8, 9, 9},
	{0,0,0,0,0},spell_transfer_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(969),	100,	24,
	"",			"!transfer object!",	"", KINGDOM_NONE,
	GROUP_ENCHANTMENT
    },

     {
        "claw of the demon",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_narssius_claw,            TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_spell_narssius_claw,     SLOT(968),        50,     12,
        "",                     "",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },     
//Talastin skill/spells
     {
        "imperial shield",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_talastin_shield,            TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_spell_talastin_shield,     SLOT(969),        50,     12,
        "",                     "",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },     
     {
        "soldier",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_talastin_soldier,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spell_talastin_soldier,     SLOT(970),        50,     12,
        "",                     "",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },
     {
	"mindblock",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 5},
	{1,0,0,0,0},spell_talastin_mindblock,		TAR_CHAR_SELF,	POS_STANDING,
	&gsn_spell_talastin_mindblock,	SLOT(971),	 50,	12,
	"",			"You feel less blocked.",	"",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },          
//Drakkon    
     {
	"illusion fields",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 8},
	{0,0,0,0,0},spell_illusion_fields,		TAR_CHAR_SELF,	POS_STANDING,
	&gsn_spell_illusion_fields,	SLOT(972),	 150,	24,
	"",			"You feel less hidden.",	"",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },          
     {
	"dragon speed",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 7},
	{0,0,0,0,0},spell_dragon_speed,		TAR_CHAR_SELF,	POS_STANDING,
	&gsn_spell_dragon_speed,	SLOT(973),	 50,	12,
	"",			"You feel slow.",	"",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },          
     {
	"dragon wrath",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 7},
	{0,0,0,0,0},spell_dragon_wrath,		TAR_CHAR_SELF,	POS_STANDING,
	&gsn_spell_dragon_wrath,	SLOT(974),	 50,	12,
	"",			"You feel less wrath.",	"",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },
    
    {
	"protection undead",	
	/* 1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 12,  9, 103,103,11, 103,103,103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  5,  8,  9,  9,  8,  6,  8, 6, 9, 9, 8, 7, 5, 5, 8, 5},
	{0,0,0,0,0},spell_protection_undead, TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(974), 	5,	12,
	"",			"You feel less protected.",	"",
	KINGDOM_NONE, GROUP_PROTECTIVE
    },          

    {
	"dragon lure",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 7},
	{0,0,0,0,0},spell_dragon_lure,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_spell_dragon_lure,	SLOT(976),	 100,	12,
	"",			"You feel free.",	"",
	KINGDOM_NONE, GROUP_FIGHTMASTER
    },
//ARKADIA    
    {
        "6th Sense",        
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 30, 24, 12, 5, 6, 7,10, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_6th_Sense ,     SLOT( 0),        0,     0,
        "",                     "!Extra Attack!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },    
     {
        "call treefolk",
        /* 1,  2,  3,  4,  5,  6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        {  5, 103,103,103,103, 9, 7, 103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  7,  9,  8, 6,  6, 8, 3, 8, 8, 8, 3, 4, 6, 9, 8},
        {0,0,0,0,0},spell_call_treefolk,            TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spell_call_treefolk,     SLOT(977),        50,     12,
        "",                     "",
        "", KINGDOM_NONE, GROUP_FIGHTMASTER
     },
     {
	"force nature",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{  7,  2, 103,103, 2, 103, 3,  8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  2,  4,  8,  3,  4,  1, 3, 1, 2, 3, 2, 1, 1, 1, 6, 5},
	{0,0,0,0,0},spell_force_nature,		TAR_CHAR_SELF,	POS_STANDING,
	&gsn_spell_force_nature,	SLOT(978),	 200,	12,
	"",			"You feel less wrath.",	"",
	KINGDOM_NONE, GROUP_NATURE
    },          

    {
	"torturer",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
        {0,0,0,0,0},spell_torturer,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_torturer,        	SLOT( 0),       200,	24,
        "torturer","",
	"", KINGDOM_NONE, GROUP_FIGHTMASTER
    },

   {
	"agony",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
        {0,0,0,0,0},spell_agony,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_agony,        	SLOT( 0),       200,	24,
        "agony","",
	"", KINGDOM_NONE, GROUP_FIGHTMASTER
    },
    
    {
        "battle prowess",        
        /* 1,  2,  3, 4, 5, 6, 7, 8  9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		 
        { 30, 24, 12, 5, 6, 7,10, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_battle_prowess,     SLOT( 0),        0,     0,
        "",                     "!Extra Attack!",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },    

    {
	"heal anywhere",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 16, 103,103,103,103,103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  9,  9,  9,  9,  9,  8, 9, 7, 9, 9, 8, 7, 5, 8, 9, 8},
	{0,0,0,0,0},spell_heal_anywhere,	TAR_IGNORE,		POS_STANDING,
	&gsn_spell_heal_anywhere,			SLOT(979),	20,	24,
	"",			"You can now heal anywhere",	"", KINGDOM_NONE,
	GROUP_FIGHTMASTER
    },

    {
	"cure corruption",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
	{0,0,0,0,0},spell_cure_corruption,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(980),	 5,	12,
	"",			"!Cure corruption!",	"", KINGDOM_NONE,
	GROUP_CURATIVE
    },
    
    {
	"contagion",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
	{0,0,0,0,0},spell_contagion,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_contagion,		SLOT(980),	 5,	12,
	"",			"You feel less contagious","", KINGDOM_NONE,
	GROUP_CURATIVE
    },
    
    {
	"contagions",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
	{0,0,0,0,0},spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_contagion_s,	SLOT(0),	 0,	0,
	"",			"","", KINGDOM_NONE,
	GROUP_CURATIVE
    },
    {
	"contagioni",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
	{0,0,0,0,0},spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_contagion_i,	SLOT(0),	 0,	0,
	"",			"","", KINGDOM_NONE,
	GROUP_CURATIVE
    },
    {
	"contagionw",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
	{0,0,0,0,0},spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_contagion_w,	SLOT(0),	 0,	0,
	"",			"","", KINGDOM_NONE,
	GROUP_CURATIVE
    },
    {
	"contagionc",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
	{0,0,0,0,0},spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_contagion_c,		SLOT(0),	 0,	0,
	"",			"","", KINGDOM_NONE,
	GROUP_CURATIVE
    },
    {
	"contagiond",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 7},
	{0,0,0,0,0},spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_contagion_d,	SLOT(0),	 0,	0,
	"",			"","", KINGDOM_NONE,
	GROUP_CURATIVE
    },

    {
	"aim",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 7},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_aim,        	SLOT( 0),       0,      24,
        "",                     "You feel your pulse slow down.",
	"", KINGDOM_NONE, GROUP_FIGHTMASTER
    },    
    {
	"jab",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 103, 6, 103,103, 7, 103,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  7,  9,  9,  9,  9,  6, 9, 9, 9, 9, 8, 9, 9, 7, 9, 8},
        {0,0,0,0,0},spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_jab,        	SLOT( 0),       0,      4,
        "jab","",
	"", KINGDOM_NONE, GROUP_FIGHTMASTER
    },        
        

    {
	"fixteleport",
	/* 1,  2,  3,  4,  5,  6,  7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/		
	{ 13, 22, 103,103,103, 7, 103,103,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9,  9,  6,  6,  5,  5,  5,  6, 5, 5, 5, 5, 5, 5, 5, 6, 9, 8},
	{0,0,0,0,0},spell_kazmir_asshole,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,	 		SLOT(981),	1,	12,
	"",			"!Teleport!",		"", KINGDOM_NONE,
	GROUP_TRANSPORTATION
    }    ,
    {
         "plague immunity",
         /* 1,  2,  3,  4, 5, 6, 7, 8 , 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
         { 103,103,103,103,1,103,1,103,103,1, 1, 1, 1, 1, 1, 1, 1, 1},
         {  9,  9,  7,  8, 9, 7, 7, 5,  6, 4, 6, 8, 9, 5, 7, 4, 7, 5},
         {1,0,0,0,0},spell_plague_immunity,TAR_CHAR_SELF,POS_STANDING,
         NULL,SLOT(982),20,11,
         "","You feel less vigorous.","",KINGDOM_NONE,
	GROUP_DEFENSIVE
   },
{
  "storm vengeance",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 8, 9, 7, 9, 5, 9, 8, 8, 9, 9, 8, 8, 6, 6, 7},
        {0,0,0,0,0},spell_storm_vengeance,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        &gsn_storm_vengeance,SLOT(1144),33,32,
        "storm vengeance","",
	"", KINGDOM_NONE,GROUP_BENEDICTIONS
    },    

{
  "arcane power",
        /* 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
        { 47,50,50,50 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {  9, 9, 6, 8, 9, 7, 9, 5, 9, 8, 8, 9, 9, 8, 8, 6, 6, 7},
        {0,0,0,0,0},spell_null,TAR_CHAR_OFFENSIVE,POS_FIGHTING,
        &gsn_arcane_power,SLOT(1144),33,32,
        "arcane power","You start to get back to your feet.",
	"", KINGDOM_NONE,GROUP_BENEDICTIONS
    },

    {
	"toxic",
	/* 1, 2, 3,  4,  5, 6, 7,  8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8*/
	{ 99,99,99,103,103,99,103,103,99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
        {  9, 9, 7,  6,  9, 7, 6,  7, 6, 5, 7, 6, 7, 4, 6, 5, 8, 6},
	{0,0,0,0,0},spell_null,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_toxic,		SLOT(983),	10,	12,
	"toxic",		"You feel less toxicated.",
	"The toxic on $p dries up.", KINGDOM_NONE,
	GROUP_POISON
    }

};
 
const   struct  auth_type      auth_table     []     =
{
    { "avatar",                "104",
                       { "goto", "poofin", "poofout", "holylight","owhere",
                         "incognito", "wizinvis",  "memory", "mwhere",
                         "stat", "wiznet", "immtalk", "imotd", "smote",
                         "prefix", "wizhelp" } },

    { "angel",         "105",
                        { "force", "penalty" } },
    { "demigod",       "106",
                       { "echo", "return", "switch", "at" } },

    { "immortal",      "107",
                       { "nochannels", "noemote", "noshout", "notell",
                       "transfer", "peace", "snoop", "string", "clone" } },

    { "god",           "108",
                       { "flag", "freeze", "load", "newlock", "pecho",
                         "purge", "restore", "gecho", "vnum", "zecho",
                         "guild", "sockets","puppet","gecho_font" } },

    { "deity",         "109",
                       { "disconnect", "pardon", "slay" } },

    { "supreme",       "110",
                       { "allow", "ban", "set","wizlock","smite","lookup",
			 "popularity","objdump" } },

    { "creator",       "115",
                       { "deny", "permban", "protect", "reboot","shutdown",
                       "log","rename","font" } },

    { "implementor",   "120",
                       { "advance", "dump", "trust", "violate", "grant",
                         "revoke","kingdom_scan" } },

    { "builder",	"build",
			{ "edit","vlist","exlist","wrlist","resets",
			"immtalk" } },
    { "", 0, { "" } }
 };
