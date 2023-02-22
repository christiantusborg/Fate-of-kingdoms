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
*     the law.										   *
*     											   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)			   *
***************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "tables.h"


/* for position */
const struct position_type position_table[] =
{
    {	"dead",			"dead"	},
    {	"mortally wounded",	"mort"	},
    {	"incapacitated",	"incap"	},
    {	"stunned",		"stun"	},
    {	"sleeping",		"sleep"	},
    {	"resting",		"rest"	},
    {   "sitting",		"sit"   },
    {	"fighting",		"fight"	},
    {	"standing",		"stand"	},
    {	NULL,			NULL	}
};

/* for sex */
const struct sex_type sex_table[] =
{
   {	"none"		},
   {	"male"		},
   {	"female"	},
   {	"either"	},
   {	NULL		}
};

/* for sizes */
const struct size_type size_table[] =
{
    {	"tiny"		},
    {	"small" 	},
    {	"medium"	},
    {	"large"		},
    {	"huge", 	},
    {	"giant" 	},
    {	NULL		}
};

/* various flag tables */
const struct flag_type act_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"sentinel",		B,	TRUE	},
    {	"scavenger",		C,	TRUE	},
    {	"aggressive",		F,	TRUE	},
    {	"stay_area",		G,	TRUE	},
    {	"wimpy",		H,	TRUE	},
    {	"pet",			I,	TRUE	},
    {	"train",		J,	TRUE	},
    {	"practice",		K,	TRUE	},
    {	"undead",		O,	TRUE	},
    {	"cleric",		Q,	TRUE	},
    {	"mage",			R,	TRUE	},
    {	"thief",		S,	TRUE	},
    {	"warrior",		T,	TRUE	},
    {	"noalign",		U,	TRUE	},
    {	"nopurge",		V,	TRUE	},
    {	"outdoors",		W,	TRUE	},
    {	"indoors",		Y,	TRUE	},
    {	"healer",		aa,	TRUE	},
    {	"mountable",		bb,	TRUE	},
    {	"update_always",	cc,	TRUE	},
    {	"changer",		dd,	TRUE	},
    {   "notrack",		ee,     TRUE    },
    {	NULL,			0,	FALSE	}
};

const struct flag_type plr_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"autoassist",		C,	FALSE	},
    {	"autoexit",		D,	FALSE	},
    {	"autoloot",		E,	FALSE	},
    {	"autosac",		F,	FALSE	},
    {	"autogold",		G,	FALSE	},
    {	"autosplit",		H,	FALSE	},
    {	"color",		I,	FALSE	},
    {	"holylight",		N,	FALSE	},
    {	"can_loot",		P,	FALSE	},
    {	"nosummon",		Q,	FALSE	},
    {	"nofollow",		R,	FALSE	},
    {	"permit",		U,	TRUE	},
    {	"log",			W,	FALSE	},
    {	"deny",			X,	FALSE	},
    {	"freeze",		Y,	FALSE	},
    {	"thief",		Z,	FALSE	},
    {	"killer",		aa,	FALSE	},
    {	NULL,			0,	0	}
};

const struct flag_type affect_flags[] =
{
    {	"blind",		A,	TRUE	},
    {	"invisible",		B,	TRUE	},
    {	"sanctuary",		H,	TRUE	},
    {	"faerie_fire",		I,	TRUE	},
    {	"infrared",		J,	TRUE	},
    {	"curse",		K,	TRUE	},
    {	"poison",		M,	TRUE	},
    {	"protect_evil",		N,	TRUE	},
    {	"camouflage",		O,	TRUE	},
    {	"sneak",		P,	TRUE	},
    {	"hide",			Q,	TRUE	},
    {	"sleep",		R,	TRUE	},
    {	"charm",		S,	TRUE	},
    {	"flying",		T,	TRUE	},
    {	"pass_door",		U,	TRUE	},
    {	"haste",		V,	TRUE	},
    {	"calm",			W,	TRUE	},
    {	"plague",		X,	TRUE	},
    {	"weaken",		Y,	TRUE	},
    {	"faerie_fog",		Z,	TRUE	}, 
	/* Could have trans faerie_fog by mistake watch out */
    {	"berserk",		aa,	TRUE	},
    {	"detect_fade",		bb,	TRUE	},
    {	"fade",			cc,	TRUE	},
    {	"slow",			dd,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type affect2_flags[] =
{
    {	"stoneshape",		I,	TRUE	},
    {	"phasing",		J,	TRUE	},
    {	"submerged",		K,	TRUE	},
    {	"ghoul",		O,	TRUE	},
    {	"vampiric",		P,	TRUE	},
    {   NULL,                   0,      0       }
};

const struct flag_type detect_flags[] =
{
    {	"detect_evil",		C,	TRUE	},
    {	"detect_invis",		D,	TRUE	},
    {	"detect_magic",		E,	TRUE	},
    {	"detect_hidden",	F,	TRUE	},
    {	"detect_good",		G,	TRUE	},
    {   "detect_steal",		DETECT_STEAL,  TRUE },
    {	"acute_vision",		ee,	TRUE	},
    {   NULL,                   0,      0       }
};

const struct flag_type off_flags[] =
{
    {	"area_attack",		A,	TRUE	},
    {	"backstab",		B,	TRUE	},
    {	"bash",			C,	TRUE	},
    {	"berserk",		D,	TRUE	},
    {	"disarm",		E,	TRUE	},
    {	"dodge",		F,	TRUE	},
    {	"fade",			G,	TRUE	},
    {	"fast",			H,	TRUE	},
    {	"kick",			I,	TRUE	},
    {	"dirt_kick",		J,	TRUE	},
    {	"parry",		K,	TRUE	},
    {	"rescue",		L,	TRUE	},
    {	"tail",			M,	TRUE	},
    {	"trip",			N,	TRUE	},
    {	"crush",		O,	TRUE	},
    {	"assist_all",		P,	TRUE	},
    {	"assist_align",		Q,	TRUE	},
    {	"assist_race",		R,	TRUE	},
    {	"assist_players",	S,	TRUE	},
    {	"assist_guard",		T,	TRUE	},
    {	"assist_vnum",		U,	TRUE	},
    {	"xattack",		W,	TRUE	},
    {	"xattack2",		X,	TRUE	},
    {	"xattack3",		Y,	TRUE	},
    {	"xattack4",		Z,	TRUE	},
    {	"xattack5",		aa,	TRUE	},
    {	"xattack6",		bb,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type imm_flags[] =
{
    {	"summon",		A,	TRUE	},
    {	"charm",		B,	TRUE	},
    {	"magic",		C,	TRUE	},
    {	"weapon",		D,	TRUE	},
    {	"bash",			E,	TRUE	},
    {	"pierce",		F,	TRUE	},
    {	"slash",		G,	TRUE	},
    {	"fire",			H,	TRUE	},
    {	"cold",			I,	TRUE	},
    {	"acid",			K,	TRUE	},
    {	"poison",		L,	TRUE	},
    {	"negative",		M,	TRUE	},
    {	"holy",			N,	TRUE	},
    {	"energy",		O,	TRUE	},
    {	"mental",		P,	TRUE	},
    {	"disease",		Q,	TRUE	},
    {	"drowning",		R,	TRUE	},
    {	"light",		S,	TRUE	},
    {	"lightning",		J,	TRUE	},
    {	"sound",		T,	TRUE	},
    {	"wood",			X,	TRUE	},
    {	"silver",		Y,	TRUE	},
    {	"iron",			Z,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type form_flags[] =
{
    {	"edible",		FORM_EDIBLE,		TRUE	},
    {	"poison",		FORM_POISON,		TRUE	},
    {	"magical",		FORM_MAGICAL,		TRUE	},
    {	"instant_decay",	FORM_INSTANT_DECAY,	TRUE	},
    {	"other",		FORM_OTHER,		TRUE	},
    {	"animal",		FORM_ANIMAL,		TRUE	},
    {	"sentient",		FORM_SENTIENT,		TRUE	},
    {	"undead",		FORM_UNDEAD,		TRUE	},
    {	"construct",		FORM_CONSTRUCT,		TRUE	},
    {	"mist",			FORM_MIST,		TRUE	},
    {	"intangible",		FORM_INTANGIBLE,	TRUE	},
    {	"biped",		FORM_BIPED,		TRUE	},
    {	"centaur",		FORM_CENTAUR,		TRUE	},
    {	"insect",		FORM_INSECT,		TRUE	},
    {	"spider",		FORM_SPIDER,		TRUE	},
    {	"crustacean",		FORM_CRUSTACEAN,	TRUE	},
    {	"worm",			FORM_WORM,		TRUE	},
    {	"blob",			FORM_BLOB,		TRUE	},
    {	"mammal",		FORM_MAMMAL,		TRUE	},
    {	"bird",			FORM_BIRD,		TRUE	},
    {	"reptile",		FORM_REPTILE,		TRUE	},
    {	"snake",		FORM_SNAKE,		TRUE	},
    {	"dragon",		FORM_DRAGON,		TRUE	},
    {	"amphibian",		FORM_AMPHIBIAN,		TRUE	},
    {	"fish",			FORM_FISH ,		TRUE	},
    {	"cold_blood",		FORM_COLD_BLOOD,	TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type part_flags[] =
{
    {	"head",			PART_HEAD,		TRUE	},
    {	"arms",			PART_ARMS,		TRUE	},
    {	"legs",			PART_LEGS,		TRUE	},
    {	"heart",		PART_HEART,		TRUE	},
    {	"brains",		PART_BRAINS,		TRUE	},
    {	"guts",			PART_GUTS,		TRUE	},
    {	"hands",		PART_HANDS,		TRUE	},
    {	"feet",			PART_FEET,		TRUE	},
    {	"fingers",		PART_FINGERS,		TRUE	},
    {	"ear",			PART_EAR,		TRUE	},
    {	"eye",			PART_EYE,		TRUE	},
    {	"long_tongue",		PART_LONG_TONGUE,	TRUE	},
    {	"eyestalks",		PART_EYESTALKS,		TRUE	},
    {	"tentacles",		PART_TENTACLES,		TRUE	},
    {	"fins",			PART_FINS,		TRUE	},
    {	"wings",		PART_WINGS,		TRUE	},
    {	"tail",			PART_TAIL,		TRUE	},
    {	"claws",		PART_CLAWS,		TRUE	},
    {	"fangs",		PART_FANGS,		TRUE	},
    {	"horns",		PART_HORNS,		TRUE	},
    {	"scales",		PART_SCALES,		TRUE	},
    {	"tusks",		PART_TUSKS,		TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type comm_flags[] =
{
    {	"quiet",		COMM_QUIET,		TRUE	},
    {   "deaf",			COMM_DEAF,		TRUE	},
    {   "nowiz",		COMM_NOWIZ,		TRUE	},
    {   "nogossip",		COMM_NOGOSSIP,		TRUE	},
    {   "noquestion",		COMM_NOQUESTION,	TRUE	},
    {   "nomusic",		COMM_NOMUSIC,		TRUE	},
    {   "noquote",		COMM_NOQUOTE,		TRUE	},
    {   "shoutsoff",		COMM_SHOUTSOFF,		TRUE	},
    {   "true_trust",		COMM_TRUE_TRUST,	TRUE	},
    {   "compact",		COMM_COMPACT,		TRUE	},
    {   "brief",		COMM_BRIEF,		TRUE	},
    {   "prompt",		COMM_PROMPT,		TRUE	},
    {   "combine",		COMM_COMBINE,		TRUE	},
    {   "telnet_ga",		COMM_TELNET_GA,		TRUE	},
    {   "show_affects",		COMM_SHOW_AFFECTS,	TRUE	},
    {   "noemote",		COMM_NOEMOTE,		FALSE	},
    {   "noshout",		COMM_NOSHOUT,		FALSE	},
    {   "notell",		COMM_NOTELL,		FALSE	},
    {   "nochannels",		COMM_NOCHANNELS,	FALSE	},
    {   "snoop_proof",		COMM_SNOOP_PROOF,	FALSE	},
    {   "afk",			COMM_AFK,		TRUE	},
    {   "nonewbie",		COMM_NONEWBIE,		FALSE	},
    {	"noquest",		COMM_NOQUEST,		FALSE	},
    {   "busy",			COMM_BUSY,		TRUE	},
    {	NULL,			0,			0	}
};

struct kingdom_type kingdom_table [] =
{
  { "None", "the shameless populace","",69,2,0,NULL,
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
},
  { "GONDOR_short","GONDOR_long","GONDOR",201,202,0,NULL,
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
},
  { "{bAL{DDON","The Circle of Aldon","aldon",261,264,0,NULL,
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"Tori","Sempai","Teisoku","Zenkutsu","Haisoku",
"Sensei","Shidoshi","Hidari","Soke","Shihan"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0.0}
},
  { "CRESCENT_short",         "CRESCENT_long","CRESCENT",252,254,0,NULL,
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
},
  { "AGRUS_short", 	"AGRUS_long","AGRUS",231,230,0,NULL,
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
},
  { "HORDE_short", 	    	"HORDE_long","HORDE",221,226,0,NULL, 
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
},
  { "GEMMA_short",        "GEMMA_long","GEMMA",242,244,0,NULL, 
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
},
  {"XELENIA_short","XELENIA_long","XELENIA",242,244,0,NULL, 
{"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE",
"NONE","NONE","NONE","NONE","NONE"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{"title1","title2","title3","title4","title5",
"title6","title7","title8","title9","title10"},
{0,0,0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0}
}
};

const struct flag_type mprog_flags[] =
{
    {  "act",                  TRIG_ACT,               TRUE    },
    {  "bribe",                TRIG_BRIBE,             TRUE    },
    {  "death",                TRIG_DEATH,             TRUE    },
    {  "entry",                TRIG_ENTRY,             TRUE    },
    {  "fight",                TRIG_FIGHT,             TRUE    },
    {  "give",                 TRIG_GIVE,              TRUE    },
    {  "greet",                TRIG_GREET,             TRUE    },
    {  "grall",                TRIG_GRALL,             TRUE    },
    {  "kill",                 TRIG_KILL,              TRUE    },
    {  "hpcnt",                TRIG_HPCNT,             TRUE    },
    {  "random",               TRIG_RANDOM,            TRUE    },
    {  "speech",               TRIG_SPEECH,            TRUE    },
    {  "exit",                 TRIG_EXIT,              TRUE    },
    {  "exall",                TRIG_EXALL,             TRUE    },
    {  "delay",                TRIG_DELAY,             TRUE    },
    {  "surr",                 TRIG_SURR,              TRUE    },
    {  NULL,                   0,                      TRUE    }
};

 const struct flag_type area_flags[] =
 {
     {	"none",			AREA_NONE,		FALSE	},
     {	"changed",		AREA_CHANGED,		TRUE	},
     {	"added",		AREA_ADDED,		TRUE	},
     {	"loading",		AREA_LOADING,		FALSE	},
     {	NULL,			0,			0	}
};


 const struct flag_type sex_flags[] =
 {
     {	"male",			SEX_MALE,		TRUE	},
     {	"female",		SEX_FEMALE,		TRUE	},
     {	"neutral",		SEX_NEUTRAL,		TRUE	},
     {   "random",               3,                      TRUE    },   /* ROM */
     {	"none",			SEX_NEUTRAL,		TRUE	},
     {	NULL,			0,			0	}
 };



 const struct flag_type exit_flags[] =
 {
     {   "door",		EX_ISDOOR,		TRUE    },
     {	"closed",		EX_CLOSED,		TRUE	},
     {	"locked",		EX_LOCKED,		TRUE	},
     {	"pickproof",		EX_PICKPROOF,		TRUE	},
     {   "nopass",		EX_NOPASS,		TRUE	},
     {   "easy",		EX_EASY,		TRUE	},
     {   "hard",		EX_HARD,		TRUE	},
     {	"infuriating",		EX_INFURIATING,		TRUE	},
     {	"noclose",		EX_NOCLOSE,		TRUE	},
     {	"nolock",		EX_NOLOCK,		TRUE	},
     {  "none",			0,			TRUE	},
     {	NULL,			0,			0	}
 };



 const struct flag_type door_resets[] =
 {
     {	"open and unlocked",	0,		TRUE	},
     {	"closed and unlocked",	1,		TRUE	},
     {	"closed and locked",	2,		TRUE	},
     {	NULL,			0,		0	}
 };



 const struct flag_type room_flags[] =
 {
     {	"dark",			ROOM_DARK,		TRUE	},
     {	"no_mob",		ROOM_NO_MOB,		TRUE	},
     {	"indoors",		ROOM_INDOORS,		TRUE	},
     {	"private(2)",		ROOM_PRIVATE,		TRUE    },
     {	"safe",			ROOM_SAFE,		TRUE	},
     {	"solitary(1)",		ROOM_SOLITARY,		TRUE	},
     {	"pet_shop",		ROOM_PET_SHOP,		TRUE	},
     {	"no_recall",		ROOM_NO_RECALL,		TRUE	},
     {	"imp_only",		ROOM_IMP_ONLY,		TRUE    },
     {	"gods_only",	        ROOM_GODS_ONLY,		TRUE    },
     {	"heroes_only",		ROOM_HEROES_ONLY,	TRUE	},
     {	"newbies_only",		ROOM_NEWBIES_ONLY,	TRUE	},
     {	"law",			ROOM_LAW,		TRUE	},
     {  "nowhere",		ROOM_NOWHERE,		TRUE	},
     {  "registry",		ROOM_REGISTRY,		TRUE	},
     {  "bounty",		ROOM_BOUNTY,		TRUE	},
     {  "arena",		ROOM_ARENA,		TRUE	},
     {  "bank",			ROOM_BANK,		TRUE	},
     {  "no_magic",		ROOM_NO_MAGIC,		TRUE	},
     {  "no_summon",		ROOM_NOSUMMON,		TRUE	},
     {  "wilderness",		ROOM_WILDERNESS,	TRUE	},
     {	NULL,			0,			0	}
 };



 const struct flag_type sector_flags[] =
 {
     {	"inside",	SECT_INSIDE,		TRUE	},
     {	"city",		SECT_CITY,		TRUE	},
     {	"field",	SECT_FIELD,		TRUE	},
     {	"forest",	SECT_FOREST,		TRUE	},
     {	"hills",	SECT_HILLS,		TRUE	},
     {	"swim",		SECT_WATER_SWIM,	TRUE	},
     {	"noswim",	SECT_WATER_NOSWIM,	TRUE	},
     {   "unused",	SECT_UNUSED,		TRUE	},
     {	"air",		SECT_AIR,		TRUE	},
     {	"desert",	SECT_DESERT,		TRUE	},
     {	"road",		SECT_ROAD,		TRUE	},
     {	"mountain_norm",SECT_MOUNTAIN,		TRUE	},
     {	"mountain_rock",SECT_ROCK_MOUNTAIN,	TRUE	},
     {	"mountain_snow",SECT_SNOW_MOUNTAIN,	TRUE	},
     {	"swamp",	SECT_SWAMP,		TRUE	},
     {	"jungle",	SECT_JUNGLE,		TRUE	},
     {	"ruins",	SECT_RUINS,		TRUE	},
     {	"enter",	SECT_ENTER,		TRUE	},
     {	"cavern",	SECT_CAVERN,	TRUE	},
     {	NULL,		0,			0 }
 };



 const struct flag_type type_flags[] =
 {
     {	"light",		ITEM_LIGHT,		TRUE	},
     {	"scroll",		ITEM_SCROLL,		TRUE	},
     {	"wand",			ITEM_WAND,		TRUE	},
     {	"staff",		ITEM_STAFF,		TRUE	},
     {	"weapon",		ITEM_WEAPON,		TRUE	},
     {	"treasure",		ITEM_TREASURE,		TRUE	},
     {	"armor",		ITEM_ARMOR,		TRUE	},
     {	"potion",		ITEM_POTION,		TRUE	},
     {	"clothing",		ITEM_CLOTHING,		TRUE	},
     {	"furniture",		ITEM_FURNITURE,		TRUE	},
     {	"ingredient",		ITEM_INGREDIENT,	TRUE	},
     {	"trash",		ITEM_TRASH,		TRUE	},
     {	"container",		ITEM_CONTAINER,		TRUE	},
     {	"drinkcontainer",	ITEM_DRINK_CON,		TRUE	},
     {	"key",			ITEM_KEY,		TRUE	},
     {	"food",			ITEM_FOOD,		TRUE	},
     {	"money",		ITEM_MONEY,		TRUE	},
     {	"boat",			ITEM_BOAT,		TRUE	},
     {	"npccorpse",		ITEM_CORPSE_NPC,	TRUE	},
     {	"pc corpse",		ITEM_CORPSE_PC,		FALSE	},
     {	"fountain",		ITEM_FOUNTAIN,		TRUE	},
     {	"pill",			ITEM_PILL,		TRUE	},
     {	"protect",		ITEM_PROTECT,		TRUE	},
     {	"map",			ITEM_MAP,		TRUE	},
     {   "portal",		ITEM_PORTAL,		TRUE	},
     {   "warpstone",		ITEM_WARP_STONE,	TRUE	},
     {	"roomkey",		ITEM_ROOM_KEY,		TRUE	},
     { 	"gem",			ITEM_GEM,		TRUE	},
     {	"jewelry",		ITEM_JEWELRY,		TRUE	},
     {  "arrow",		ITEM_ARROW,		TRUE	},
     {	"jukebox",		ITEM_JUKEBOX,		TRUE	},
     {	"trap",			ITEM_TRAP,		TRUE	},
     {	NULL,			0,			0	}
 };

/* RACE ITEM PREVENTION TABLE FOR OLC 
   Coded by Takeda 2/26/00 */
const struct flag_type race_flags[] =
 {
     {  "human",		RACE_HUMAN,		TRUE	},
     {  "elf",			RACE_ELF,		TRUE	},
     {  "drow",			RACE_DROW,		TRUE	},
     {  "dwarf",		RACE_DWARF,		TRUE	},
     {  "duergar",		RACE_DUERGAR,		TRUE	},
     {  "pixie",		RACE_PIXIE,		TRUE	},
     {  "gargoyle",		RACE_GARGOYLE,		TRUE	},
     {  "arial",		RACE_ARIAL,		TRUE	},
     {  "draconian",		RACE_DRACONIAN,		TRUE	},
     {  "felar",		RACE_FELAR,		TRUE	},
     {  "merman",		RACE_MERMAN,		TRUE	},
     {  "giant",		RACE_GIANT,		TRUE	},
     {  "troll",		RACE_TROLL,		TRUE	},
     {  "ogre",			RACE_OGRE,		TRUE	},
     {  "orc",			RACE_ORC,		TRUE	},
     {  "darnok",		RACE_DARNOK,		TRUE	},
     {  "illithid",		RACE_ILLITHID,		TRUE	},
     {	NULL,			0,			0	}
 };

/* CLASS ITEM PREVENTION TABLE FOR OLC 
   Coded by Takeda 2/26/00 */
const struct flag_type class_flags[] =
 { 
    {  "warrior",		CLASS_WARRIOR,		TRUE	},
    {  "shaman",		CLASS_MERCENARY,	TRUE	},
    {  "paladin",		CLASS_PALADIN,		TRUE	},
    {  "black_knight",		CLASS_BLACK_KNIGHT,	TRUE	},
    {  "thief",			CLASS_THIEF,		TRUE	},
    {  "inquisitor",		CLASS_INQUISITOR,	TRUE	},
    {  "spellfilcher",		CLASS_SPELLFILCHER,	TRUE	},
    {  "cleric",		CLASS_CLERIC,		TRUE	},
    {  "prodigy",		CLASS_PRODIGY,		TRUE	},
    {  "mage",			CLASS_MAGE,		TRUE	},
    {  "ranger",		CLASS_RANGER,		TRUE	},
    {  "assassin",		CLASS_ASSASSIN,		TRUE	},
    {  "bard",			CLASS_BARD,		TRUE	},
    {  "maligner",		CLASS_MALIGNER,		TRUE	},
    {  "alchemist",		CLASS_ALCHEMIST,	TRUE	},
    {  "druid",			CLASS_DRUID,		TRUE	},
    {  "monk",			CLASS_MONK,		TRUE	},
    {  "necromancer",		CLASS_NECROMANCER,	TRUE	},
     {	NULL,			0,			0	}
 };

 const struct flag_type extra_flags[] =
 {
     {	"glow",			ITEM_GLOW,		TRUE	},
     {	"hum",			ITEM_HUM,		TRUE	},
     {	"dark",			ITEM_DARK,		TRUE	},
     {	"lock",			ITEM_LOCK,		TRUE	},
     {	"evil",			ITEM_EVIL,		TRUE	},
     {	"hidden",		ITEM_HIDDEN,		TRUE	},
     {	"invis",		ITEM_INVIS,		TRUE	},
     {	"magic",		ITEM_MAGIC,		TRUE	},
     {	"nodrop",		ITEM_NODROP,		TRUE	},
     {	"bless",		ITEM_BLESS,		TRUE	},
     {	"antigood",		ITEM_ANTI_GOOD,		TRUE	},
     {	"antievil",		ITEM_ANTI_EVIL,		TRUE	},
     {	"antineutral",		ITEM_ANTI_NEUTRAL,	TRUE	},
     {	"noremove",		ITEM_NOREMOVE,		TRUE	},
     {	"inventory",		ITEM_INVENTORY,		TRUE	},
     {	"nolocate",		ITEM_NOLOCATE,		TRUE	},
     {	"nopurge",		ITEM_NOPURGE,		TRUE	},
     {	"rotdeath",		ITEM_ROT_DEATH,		TRUE	},
     {	"visdeath",		ITEM_VIS_DEATH,		TRUE	},
     {  "nonmetal",		ITEM_NONMETAL,		TRUE	},
     {	"meltdrop",		ITEM_MELT_DROP,		TRUE	},
     {	"hadtimer",		ITEM_HAD_TIMER,		TRUE	},
     {	"sellextract",		ITEM_SELL_EXTRACT,	TRUE	},
     {	"burnproof",		ITEM_BURN_PROOF,	TRUE	},
     {	"nouncurse",		ITEM_NOUNCURSE,		TRUE	},
     {	"dimension",		ITEM_DIMENSION,		TRUE	},
     {  "indestruct",		ITEM_INDESTRUCT,	TRUE	},
     {  "nodonate",		ITEM_NODONATE,		TRUE	},
     {  "mobgive",		ITEM_MOBGIVE,		TRUE	},
//     {  "mobgive",		ITEM_SHOPDROP,		TRUE	},
	
     {	NULL,			0,			0	}
 };


const struct flag_type wear_flags[] =
 {
     {	"take",			ITEM_TAKE,		TRUE	},
     {	"finger",		ITEM_WEAR_FINGER,	TRUE	},
     {	"neck",			ITEM_WEAR_NECK,		TRUE	},
     {	"body",			ITEM_WEAR_BODY,		TRUE	},
     {	"head",			ITEM_WEAR_HEAD,		TRUE	},
     {	"legs",			ITEM_WEAR_LEGS,		TRUE	},
     {	"feet",			ITEM_WEAR_FEET,		TRUE	},
     {	"hands",		ITEM_WEAR_HANDS,	TRUE	},
     {	"arms",			ITEM_WEAR_ARMS,		TRUE	},
     {	"shield",		ITEM_WEAR_SHIELD,	TRUE	},
     {	"about",		ITEM_WEAR_ABOUT,	TRUE	},
     {	"waist",		ITEM_WEAR_WAIST,	TRUE	},
     {	"wrist",		ITEM_WEAR_WRIST,	TRUE	},
     {	"wield",		ITEM_WIELD,		TRUE	},
     {	"hold",			ITEM_HOLD,		TRUE	},
     {  "nosac",		ITEM_NO_SAC,		TRUE	},
     {	"wearfloat",		ITEM_WEAR_FLOAT,	TRUE	},
     {	"ornament",		ITEM_WEAR_ORNAMENT,	TRUE	},
     {	"tattoo",		ITEM_WEAR_TATTOO,	TRUE	},
     {	NULL,			0,			0	}
 };

 /*
  * Used when adding an affect to tell where it goes.
  * See addaffect and delaffect in act_olc.c
  */
 const struct flag_type apply_flags[] =
 {
     {	"none",			APPLY_NONE,		TRUE	},
     {	"strength",		APPLY_STR,		TRUE	},
     {	"dexterity",		APPLY_DEX,		TRUE	},
     {	"intelligence",		APPLY_INT,		TRUE	},
     {	"wisdom",		APPLY_WIS,		TRUE	},
     {	"constitution",		APPLY_CON,		TRUE	},
     {	"sex",			APPLY_SEX,		TRUE	},
     {	"class",		APPLY_CLASS,		TRUE	},
     {	"level",		APPLY_LEVEL,		TRUE	},
     {	"age",			APPLY_AGE,		TRUE	},
     {	"height",		APPLY_HEIGHT,		TRUE	},
     {	"weight",		APPLY_WEIGHT,		TRUE	},
     {	"mana",			APPLY_MANA,		TRUE	},
     {	"hp",			APPLY_HIT,		TRUE	},
     {	"move",			APPLY_MOVE,		TRUE	},
     {	"gold",			APPLY_GOLD,		TRUE	},
     {	"experience",		APPLY_EXP,		TRUE	},
     {	"ac",			APPLY_AC,		TRUE	},
     {	"hitroll",		APPLY_HITROLL,		TRUE	},
     {	"damroll",		APPLY_DAMROLL,		TRUE	},
     {	"saves",		APPLY_SAVES,		TRUE	},
     {	"savingpara",		APPLY_SAVING_PARA,	TRUE	},
     {	"savingrod",		APPLY_SAVING_ROD,	TRUE	},
     {	"savingpetri",		APPLY_SAVING_PETRI,	TRUE	},
     {	"savingbreath",		APPLY_SAVING_BREATH,	TRUE	},
     {	"savingspell",		APPLY_SAVING_SPELL,	TRUE	},
     {	"spellaffect",		APPLY_SPELL_AFFECT,     FALSE	},
     {	NULL,			0,			0	}
 };



 /*
  * What is seen.
  */
 const struct flag_type wear_loc_strings[] =
 {
     {	"in the inventory",	WEAR_NONE,	TRUE	},
     {	"as a light",		WEAR_LIGHT,	TRUE	},
     {	"on the left finger",	WEAR_FINGER_L,	TRUE	},
     {	"on the right finger",	WEAR_FINGER_R,	TRUE	},
     {	"around the neck (1)",	WEAR_NECK_1,	TRUE	},
     {	"around the neck (2)",	WEAR_NECK_2,	TRUE	},
     {	"on the body",		WEAR_BODY,	TRUE	},
     {	"over the head",	WEAR_HEAD,	TRUE	},
     {	"on the legs",		WEAR_LEGS,	TRUE	},
     {	"on the feet",		WEAR_FEET,	TRUE	},
     {	"on the hands",		WEAR_HANDS,	TRUE	},
     {	"on the arms",		WEAR_ARMS,	TRUE	},
     {	"as a shield",		WEAR_SHIELD,	TRUE	},
     {	"about the shoulders",	WEAR_ABOUT,	TRUE	},
     {	"around the waist",	WEAR_WAIST,	TRUE	},
     {	"on the left wrist",	WEAR_WRIST_L,	TRUE	},
     {	"on the right wrist",	WEAR_WRIST_R,	TRUE	},
     {	"wielded",		WEAR_WIELD,	TRUE	},
     {	"held in the hands",	WEAR_HOLD,	TRUE	},
     {	"floating nearby",	WEAR_FLOAT,	TRUE	},
     {	"as ornament",		WEAR_ORNAMENT_1,	TRUE	},
     {	"as another ornament",	WEAR_ORNAMENT_2,	TRUE	},
     {  "branded",		WEAR_TATTOO,	TRUE	},
     {	NULL,			0	      , 0	}
 };


 const struct flag_type wear_loc_flags[] =
 {
     {	"none",		WEAR_NONE,	TRUE	},

     {	"lfinger",	WEAR_FINGER_L,	TRUE	},
     {	"rfinger",	WEAR_FINGER_R,	TRUE	},
     {	"hands",	WEAR_HANDS,	TRUE	},
     {	"lwrist",	WEAR_WRIST_L,	TRUE	},
     {	"rwrist",	WEAR_WRIST_R,	TRUE	},
     {	"arms",		WEAR_ARMS,	TRUE	},
     
     {	"head",		WEAR_HEAD,	TRUE	},
     {	"neck1",	WEAR_NECK_1,	TRUE	},
     {	"neck2",	WEAR_NECK_2,	TRUE	},

     {	"body",		WEAR_BODY,	TRUE	},
     {	"about",	WEAR_ABOUT,	TRUE	},
     {	"waist",	WEAR_WAIST,	TRUE	},

     {	"legs",		WEAR_LEGS,	TRUE	},
     {	"feet",		WEAR_FEET,	TRUE	},



     {	"light",	WEAR_LIGHT,	TRUE	},
     {	"shield",	WEAR_SHIELD,	TRUE	},
     {	"wielded",	WEAR_WIELD,	TRUE	},
     {	"second",	WEAR_SECONDARY,	TRUE	},
     {	"hold",		WEAR_HOLD,	TRUE	},
     {	"floating",	WEAR_FLOAT,	TRUE	},
     {	"ornament1",	WEAR_ORNAMENT_1,TRUE	},
     {	"ornament2",	WEAR_ORNAMENT_2,TRUE	},
     {	"tattoo",	WEAR_TATTOO,	TRUE	},
     {	NULL,		0,		0	}
 };

 const struct flag_type container_flags[] =
 {
     {	"closeable",		1,		TRUE	},
     {	"pickproof",		2,		TRUE	},
     {	"closed",		4,		TRUE	},
     {	"locked",		8,		TRUE	},
     {	"puton",		16,		TRUE	},
     {	NULL,			0,		0	}
 };

 /*****************************************************************************
                       ROM - specific tables:
  ****************************************************************************/




 const struct flag_type ac_type[] =
 {
    {   "pierce",        AC_PIERCE,            TRUE    },
     {   "bash",          AC_BASH,              TRUE    },
     {   "slash",         AC_SLASH,             TRUE    },
     {   "exotic",        AC_EXOTIC,            TRUE    },
     {   NULL,              0,                    0       }
 };


 const struct flag_type size_flags[] =
 {
     {   "tiny",          SIZE_TINY,            TRUE    },
     {   "small",         SIZE_SMALL,           TRUE    },
     {   "medium",        SIZE_MEDIUM,          TRUE    },
     {   "large",         SIZE_LARGE,           TRUE    },
     {   "huge",          SIZE_HUGE,            TRUE    },
     {   "giant",         SIZE_GIANT,           TRUE    },
     {   NULL,              0,                    0       },
 };


 const struct flag_type trap_triggers[] =
 {
     {   "movement",	TRAP_EFF_MOVE,		TRUE    },
     {   "get-put",	TRAP_EFF_OBJECT,	TRUE    },
     {   NULL,		0,			0       }
 };


 const struct flag_type trap_damage[] =
 {
     {   "sleep",	TRAP_DAM_SLEEP,		TRUE    },
     {   "teleport",	TRAP_DAM_TELEPORT,	TRUE    },
     {   "fire",	TRAP_DAM_FIRE,		TRUE    },
     {   "cold",	TRAP_DAM_COLD,		TRUE    },
     {   "acid",	TRAP_DAM_ACID,		TRUE    },
     {   "energy",	TRAP_DAM_ENERGY, 	TRUE    },
     {   "blunt",	TRAP_DAM_BLUNT,		TRUE    },
     {   "pierce",	TRAP_DAM_PIERCE,	TRUE    },
     {   "slash",	TRAP_DAM_SLASH,		TRUE    },
     {   NULL,		0,			0       }
 };



 const struct flag_type weapon_class[] =
 {
     {   "exotic",	WEAPON_EXOTIC,		TRUE    },
     {   "sword",	WEAPON_SWORD,		TRUE    },
     {   "dagger",	WEAPON_DAGGER,		TRUE    },
     {   "spear",	WEAPON_SPEAR,		TRUE    },
     {   "mace",	WEAPON_MACE,		TRUE    },
     {   "axe",		WEAPON_AXE,		TRUE    },
     {   "flail",	WEAPON_FLAIL,		TRUE    },
     {   "whip",	WEAPON_WHIP,		TRUE    },
     {   "polearm",	WEAPON_POLEARM,		TRUE    },
     {   "bow",		WEAPON_BOW,		TRUE	},
     {   "arrow",	WEAPON_ARROW,		TRUE	},
     {   "lance",	WEAPON_LANCE,		TRUE	},
     {   NULL,		0,			0       }
 };


 const struct flag_type weapon_type2[] =
 {
     {   "flaming",       WEAPON_FLAMING,       TRUE    },
     {   "frost",         WEAPON_FROST,         TRUE    },
     {   "vampiric",      WEAPON_VAMPIRIC,      TRUE    },
     {   "sharp",         WEAPON_SHARP,         TRUE    },
     {   "vorpal",        WEAPON_VORPAL,        TRUE    },
     {   "twohands",     WEAPON_TWO_HANDS,     TRUE    },
     {	"shocking",	 WEAPON_SHOCKING,      TRUE    },
     {	"poison",	WEAPON_POISON,		TRUE	},
     {	"manadrain",	WEAPON_MANADRAIN,	TRUE	},

      {	"acid",	WEAPON_ACID,	TRUE	},    
      {	"lightning",	WEAPON_LIGHTNING,	TRUE	},    
      {	"negative",	WEAPON_NEGATIVE,	TRUE	},    
      {	"harm",	WEAPON_HARM,	TRUE	},    
      {	"disease",	WEAPON_DISEASE,	TRUE	},
      {	"energy",	WEAPON_ENERGY,	TRUE	},  
      {	"sound",	WEAPON_SOUND,	TRUE	},

      
     {   NULL,              0,                    0       }
 };

 const struct flag_type res_flags[] =
 {
     {	"summon",	 RES_SUMMON,		TRUE	},
     {   "charm",         RES_CHARM,            TRUE    },
     {   "magic",         RES_MAGIC,            TRUE    },
     {   "weapon",        RES_WEAPON,           TRUE    },
     {   "bash",          RES_BASH,             TRUE    },
     {   "pierce",        RES_PIERCE,           TRUE    },
     {   "slash",         RES_SLASH,            TRUE    },
     {   "fire",          RES_FIRE,             TRUE    },
     {   "cold",          RES_COLD,             TRUE    },
     {   "acid",          RES_ACID,             TRUE    },
     {   "poison",        RES_POISON,           TRUE    },
     {   "negative",      RES_NEGATIVE,         TRUE    },
     {   "holy",          RES_HOLY,             TRUE    },
     {   "energy",        RES_ENERGY,           TRUE    },
     {   "mental",        RES_MENTAL,           TRUE    },
     {   "disease",       RES_DISEASE,          TRUE    },
     {   "drowning",      RES_DROWNING,         TRUE    },
     {   "light",         RES_LIGHT,            TRUE    },
     {   "lightning",     RES_LIGHTNING,        TRUE    },
     {	"sound",	RES_SOUND,		TRUE	},
     {	"wood",		RES_WOOD,		TRUE	},
     {	"silver",	RES_SILVER,		TRUE	},
     {	"iron",		RES_IRON,		TRUE	},
     {   NULL,          0,            0    }
 };


const struct flag_type vuln_flags[] =
{
    {	"summon",	 VULN_SUMMON,		TRUE	},
    {	"charm",	VULN_CHARM,		TRUE	},
    {   "magic",         VULN_MAGIC,           TRUE    },
    {   "weapon",        VULN_WEAPON,          TRUE    },
    {   "bash",          VULN_BASH,            TRUE    },
    {   "pierce",        VULN_PIERCE,          TRUE    },
    {   "slash",         VULN_SLASH,           TRUE    },
    {   "fire",          VULN_FIRE,            TRUE    },
    {   "cold",          VULN_COLD,            TRUE    },
    {   "acid",          VULN_ACID,            TRUE    },
    {   "poison",        VULN_POISON,          TRUE    },
    {   "negative",      VULN_NEGATIVE,        TRUE    },
    {   "holy",          VULN_HOLY,            TRUE    },
    {   "energy",        VULN_ENERGY,          TRUE    },
    {   "mental",        VULN_MENTAL,          TRUE    },
    {   "disease",       VULN_DISEASE,         TRUE    },
    {   "drowning",      VULN_DROWNING,        TRUE    },
    {   "light",         VULN_LIGHT,           TRUE    },
    {   "lightning",     VULN_LIGHTNING,       TRUE    },
    {	"sound",	 VULN_SOUND,		TRUE	},
    {   "wood",          VULN_WOOD,            TRUE    },
    {   "silver",        VULN_SILVER,          TRUE    },
    {   "iron",          VULN_IRON,            TRUE    },
    {   NULL,              0,                    0       }
 };

 const struct flag_type position_flags[] =
 {
     {   "dead",           POS_DEAD,            FALSE   },
     {   "mortal",         POS_MORTAL,          FALSE   },
     {   "incap",          POS_INCAP,           FALSE   },
     {   "stunned",        POS_STUNNED,         FALSE   },
     {   "sleeping",       POS_SLEEPING,        TRUE    },
     {   "resting",        POS_RESTING,         TRUE    },
     {   "sitting",        POS_SITTING,         TRUE    },
     {   "fighting",       POS_FIGHTING,        FALSE   },
     {   "standing",       POS_STANDING,        TRUE    },
     {   NULL,              0,                    0       }
 };

 const struct flag_type portal_flags[]=
 {
     {   "normal_exit",	  GATE_NORMAL_EXIT,	TRUE	},
     {	"no_curse",	  GATE_NOCURSE,		TRUE	},
     {   "go_with",	  GATE_GOWITH,		TRUE	},
     {   "buggy",	  GATE_BUGGY,		TRUE	},
     {	"random",	  GATE_RANDOM,		TRUE	},
     {   NULL,		  0,			0	}
 };

 const struct flag_type ingredient_flags[]=
 {
     {  "dragonblood",	  ING_DRAGONBLOOD,	TRUE	},
     {	"mandrake",	  ING_MANDRAKE,		TRUE	},
     {	"ivory",	  ING_IVORY,		TRUE	},
     {	"guano",	  ING_GUANO,		TRUE	},
     {	"lizardeye",	  ING_LIZARDEYE,	TRUE	},
     {	"wax",		  ING_WAX,		TRUE	},
     {	"bone",		  ING_BONE,		TRUE	},
     {	"salt",		  ING_SALT,		TRUE	},
     {	"firefly",	  ING_FIREFLY,		TRUE	},
     {	"poison ivy",	  ING_POISON_IVY,	TRUE	},
     {	"aloe",		  ING_ALOE,		TRUE	},
     {	"anemone",	  ING_ANEMONE,		TRUE	},
     {	"barberry",	  ING_BARBERRY,		TRUE	},
     {	"coriander",	  ING_CORIANDER,	TRUE	},
     {	"dragoneye",	  ING_DRAGONEYE,	TRUE	},
     {	"garlic",	  ING_GARLIC,		TRUE	},
     {	"grapeleaf",	  ING_GRAPELEAF,	TRUE	},
     {	"lavender",	  ING_LAVENDER,		TRUE	},
     {	"hemlock",	  ING_HEMLOCK,		TRUE	},
     {	"hyacinth",	  ING_HYACINTH,		TRUE	},
     {	"andromeda",	  ING_ANDROMEDA,	TRUE	},
//     frostroot
//     wolfsbane
//black iris 
// ivory carnation
//Winter Blossom
//Arcanium Ivy
//Midnight Bloom
//Sacelli

     {	NULL,		  0,			0	}
 };

const struct flag_type furniture_flags[]=
 {
     {   "stand_at",	  STAND_AT,		TRUE	},
     {	"stand_on",	  STAND_ON,		TRUE	},
     {	"stand_in",	  STAND_IN,		TRUE	},
     {	"sit_at",	  SIT_AT,		TRUE	},
     {	"sit_on",	  SIT_ON,		TRUE	},
     {	"sit_in",	  SIT_IN,		TRUE	},
     {	"rest_at",	  REST_AT,		TRUE	},
     {	"rest_on",	  REST_ON,		TRUE	},
     {	"rest_in",	  REST_IN,		TRUE	},
     {	"sleep_at",	  SLEEP_AT,		TRUE	},
     {	"sleep_on",	  SLEEP_ON,		TRUE	},
     {	"sleep_in",	  SLEEP_IN,		TRUE	},
     {	"put_at",	  PUT_AT,		TRUE	},
     {	"put_on",	  PUT_ON,		TRUE	},
     {	"put_in",	  PUT_IN,		TRUE	},
     {	"put_inside",	  PUT_INSIDE,		TRUE	},
     {	NULL,		  0,			0	}
 };
 
 const	struct	flag_type	apply_types	[]	=
 {
 	{	"affects",	TO_AFFECTS,	TRUE	},
 	{	"object",	TO_OBJECT,	TRUE	},
 	{	"immune",	TO_IMMUNE,	TRUE	},
 	{	"resist",	TO_RESIST,	TRUE	},
 	{	"vuln",		TO_VULN,	TRUE	},
 	{	"weapon",	TO_WEAPON,	TRUE	},
 	{	NULL,		0,		TRUE	}
 };

 const	struct	bit_type	bitvector_type	[]	=
 {
 	{	affect_flags,	"affect"	},
 	{	apply_flags,	"apply"		},
 	{	imm_flags,	"imm"		},
 	{	res_flags,	"res"		},
 	{	vuln_flags,	"vuln"		},
 	{	weapon_type2,	"weapon"	}
 };

const   struct  prac_type    prac_table[] =
{
    {   "none",         "none",           	GROUP_NONE              },
    {   "fire",		"fire",			GROUP_FIRE		},
    {	"cold",		"cold",			GROUP_COLD		},
    {	"lightning",	"lightning",		GROUP_LIGHTNING		},
    {	"projectile_magic","projectile_magic",	GROUP_PRO_MAGIC		},
    {	"stealth",	"stealth",		GROUP_STEALTH		},
    {	"acid",		"acid",			GROUP_ACID		},
    {	"poison",	"poison",		GROUP_POISON		},
    {   "weaponmaster", "weaponmaster",  	GROUP_WEAPONMASTER     },
    {   "attack",       "attack",         	GROUP_ATTACK            },
    {   "beguiling",    "beguiling",      	GROUP_BEGUILING         },
    {   "benedictions", "benedictions",   	GROUP_BENEDICTIONS      },
    {   "creation",     "creation",      	GROUP_CREATION          },
    {   "curative",     "curative",       	GROUP_CURATIVE          },
    {   "detection",    "detection",      	GROUP_DETECTION         },
    {   "enchantment",  "enchantment",    	GROUP_ENCHANTMENT       },
    {   "enhancement",  "enhancement",    	GROUP_ENHANCEMENT       },
    {   "healing",      "healing",        	GROUP_HEALING           },
    {   "illusion",     "illusion",  		GROUP_ILLUSION          },
    {   "maladictions", "maladictions",		GROUP_MALADICTIONS      },
    {   "protective",   "protective",     	GROUP_PROTECTIVE        },
    {   "transportation","transportation",	GROUP_TRANSPORTATION    },
    {   "nature",      "nature",        	GROUP_NATURE           },
    {   "fightmaster",  "fightmaster",		GROUP_FIGHTMASTER       },
    {   "suddendeath",  "suddendeath",		GROUP_SUDDENDEATH       },
    {   "kingdom",      "kingdom",		GROUP_KINGDOM		},
    {   "defensive",    "defensive",		GROUP_DEFENSIVE         },
    {   "wizard",       "wizard",  	        GROUP_WIZARD            },
    {   "mental",	"mind",			GROUP_MIND		},
    {   "necromancy",	"necromancy",		GROUP_NECRO		},
    {   NULL,           NULL,                   0                       }
};

const struct flag_type oprog_flags[] =
{
    {	"act",			TRIG_ACT,		TRUE	},
    {	"fight",		TRIG_FIGHT,		TRUE	},
    {	"give",			TRIG_GIVE,		TRUE	},
    {   "greet",		TRIG_GRALL,		TRUE	},
    {	"random",		TRIG_RANDOM,		TRUE	},
    {   "speech",		TRIG_SPEECH,		TRUE	},
    {	"exall",		TRIG_EXALL,		TRUE	},
    {	"delay",		TRIG_DELAY,		TRUE	},
    {	"drop",		TRIG_DROP,		TRUE	},
    {	"get",		TRIG_GET,		TRUE	},
    {	"sit",		TRIG_SIT,		TRUE	},
    {	NULL,			0,			TRUE	},
};

const struct flag_type rprog_flags[] =
{
    {	"act",			TRIG_ACT,		TRUE	},
    {	"fight",		TRIG_FIGHT,		TRUE	},
    {	"drop",			TRIG_DROP,		TRUE	},
    {	"greet",		TRIG_GRALL,		TRUE	},
    {	"random",		TRIG_RANDOM,		TRUE	},
    {	"speech",		TRIG_SPEECH,		TRUE	},
    {	"exall",		TRIG_EXALL,		TRUE	},
    {	"delay",		TRIG_DELAY,		TRUE	},
    {	NULL,			0,			TRUE	},
};
