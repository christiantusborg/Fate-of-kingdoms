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
#include "fok_struct.h"

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
   { "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
   { "staff",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
   { "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm	},
   { "bow",	OBJ_VNUM_SCHOOL_BOW,	WEAPON_BOW,	&gsn_bow	},
   { "arrow",   OBJ_VNUM_SCHOOL_POLEARM,WEAPON_ARROW,   &gsn_arrow      },
   { "lance",   OBJ_VNUM_SCHOOL_LANCE,  WEAPON_LANCE,   &gsn_lance      },
   { NULL,	0,				0,	NULL		}
};



/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           "{C[{BWIZNET{C]{x ",	WIZ_ON,         IM },
   {    "prefix",	"{C[{GGAME{C]{x ",	WIZ_PREFIX,	IM },
   {    "ticks",        "{C[{RSYSTEM{C]{x ",	WIZ_TICKS,      L2 },
   {    "logins",       "{C[{RLOGIN{C]{x ",	WIZ_LOGINS,     IM },
   {    "sites",        "{C[{RLOGIN{C]{x ",	WIZ_SITES,      L4 },
   {    "links",        "{C[{RSYSTEM{C]{x ",	WIZ_LINKS,      L7 },
   {	"newbies",	"{C[{RLOGIN{C]{x ",	WIZ_NEWBIE,	IM },
   {	"spam",		"{C[{RSYSTEM{C]{x ",	WIZ_SPAM,	L5 },
   {    "deaths",       "{C[{GGAME{C]{x ",	WIZ_DEATHS,     L2 },
   {    "resets",       "{C[{RSYSTEM{C]{x ",	WIZ_RESETS,     L4 },
   {    "mobdeaths",    "{C[{GGAME{C]{x ",	WIZ_MOBDEATHS,  L4 },
   {    "flags",	"{C[{RSYSTEM{C]{x ",	WIZ_FLAGS,	L5 },
   {	"penalties",	"{C[{GSYSTEM{C]{x ",	WIZ_PENALTIES,	L5 },
   {	"saccing",	"{C[{GGAME{C]{x ",	WIZ_SACCING,	L5 },
   {	"levels",	"{C[{GGAME{C]{x ",	WIZ_LEVELS,	IM }, 
   {	"load",		"{C[{GSYSTEM{C]{x ",	WIZ_LOAD,	L2 },
   {	"restore",	"{C[{GSYSTEM{C]{x ",	WIZ_RESTORE,	L2 },
   {	"snoops",	"{C[{GSYSTEM{C]{x ",	WIZ_SNOOPS,	L2 },
   {	"switches",	"{C[{GSYSTEM{C]{x ",	WIZ_SWITCHES,	L2 },
   {	"secure",	"{C[{GSYSTEM{C]{x ",	WIZ_SECURE,	L1 },
   {	"quests",	"{C[{GSYSTEM{C]{x ",	WIZ_QUESTS,	IM },
   {    "Password",     "{C[{RLOGIN{C]{x ",	WIZ_PASSWORD,   L5 },
   {    "delete",       "{C[{GSYSTEM{C]{x ",	WIZ_DELETE,   L5 },
   {    "cheat",       "{C[{GSYSTEM{C]{x ",	WIZ_CHEAT,   L5 },
   {    "rp",          "{C[{GGAMEE{C]{x ",	WIZ_RP,   L5 },   
   {    "prog",        "{C[{RSYSTEM{C]{x ",	WIZ_PROG,   L5 },      
   
   {	NULL,		"", 0,		0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[]		=
{
    { 	"none",		"hit",		-1		},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	},
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"digestion",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
    {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
    {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	},
    {	"flbite",	"flaming bite", DAM_FIRE	},
    {	"frbite",	"freezing bite", DAM_COLD	},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	},
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"flame",	DAM_FIRE	},
    {   "chill",	"chill",	DAM_COLD	},
    {   "drown",        "drowning",        DAM_DROWNING     },          
    {   "illum",        "illumination",    DAM_LIGHT        },          
    {   "sonicb",       "sonic boom",      DAM_SOUND        },          
    {   "demonst",      "demonic strike",  DAM_HARM         },          
    {   "pestil",       "pestilence",      DAM_DISEASE      },    
    {   "abysmal",	"Abysmal Blight",  DAM_NEGATIVE	},
    {   NULL,		NULL,		0		}
};

/*  adds traps */
const 	struct attack_type	trap_attack_table[] =
{
    { 	"sleep","sleep",TRAP_DAM_SLEEP		},
    { 	"teleport","teleport",TRAP_DAM_TELEPORT	},
    { 	"fire","fire",TRAP_DAM_FIRE		},
    { 	"cold","cold",TRAP_DAM_COLD		},
    { 	"acid","acid",TRAP_DAM_ACID		},
    { 	"energy","energy",TRAP_DAM_ENERGY	},
    { 	"blunt","blunt",TRAP_DAM_BLUNT		},
    { 	"pierce","slash",TRAP_DAM_PIERCE	},
    { 	"slash","slash",TRAP_DAM_SLASH		},
    {   NULL,	NULL,	0			}
};
/*  end */

                
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

   }
};

/* Leader's Name, name of religion, tattoo vnum */
/*
const struct religion_type religion_table [] =
{
  { "", "None", 0 },
  { "Kazmir", "wisdom",		OBJ_VNUM_MARK_WISDOM },
  { "Kazmir", "time",		OBJ_VNUM_MARK_TIME },
  { "Kazmir", "justice",	OBJ_VNUM_MARK_JUSTICE },
  { "Kazmir", "faith",		OBJ_VNUM_MARK_FAITH },
  { "Kazmir", "deception",	OBJ_VNUM_MARK_DECEPTION },
  { "Kazmir", "hate",		OBJ_VNUM_MARK_HATE },
  { "Kazmir", "nature",		OBJ_VNUM_MARK_NATURE },
  { "Kazmir", "creation",	OBJ_VNUM_MARK_CREATION },
  { "Kazmir", "neutraility",	OBJ_VNUM_MARK_NEUTRAILITY },
  { "Kazmir", "war",		OBJ_VNUM_MARK_WAR },
  { "Kazmir", "honor",		OBJ_VNUM_MARK_HONOR },
  { "Kazmir", "understanding",	OBJ_VNUM_MARK_UNDERSTANDING },
  { "Kazmir", "quests",		OBJ_VNUM_MARK_QUESTS },
  { "Kazmir", "law",		OBJ_VNUM_MARK_LAW },
  { "Kazmir", "leadership",	OBJ_VNUM_MARK_LEADERSHIP },
  { "Kazmir", "hope",		OBJ_VNUM_MARK_HOPE },
  { "Kazmir", "knowledge",	OBJ_VNUM_MARK_KNOWLEDGE },
  { "Kazmir", "order",		OBJ_VNUM_MARK_ORDER },
  { "Kazmir", "peace",		OBJ_VNUM_MARK_PEACE },
  { "Kazmir", "chaos",		OBJ_VNUM_MARK_CHAOS },
  { "Kazmir", "death",		OBJ_VNUM_MARK_DEATH },
  { "Kazmir", "life",		OBJ_VNUM_MARK_LIFE },
  { "Kazmir", "mercy",		OBJ_VNUM_MARK_MERCY },
  { "Kazmir", "magic",		OBJ_VNUM_MARK_MAGIC },
};

*/

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
const	struct	str_app_type	str_app		[26]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
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
    {  14, 16,650, 85 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 80 },
    { 85 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 1 },	/* 10 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 2 },	/* 15 */
    { 2 },
    { 2 },
    { 3 },	/* 18 */
    { 3 },
    { 3 },	/* 20 */
    { 3 },
    { 4 },
    { 4 },
    { 4 },
    { 5 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};


const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  12, 99 },
    {  15, 99 }    /* 25 */
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




