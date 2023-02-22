#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "fok_struct.h"

// ***** struct NPlayer races Begin ***** 
const 	struct	new_race_type	new_race_table	[]		=
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

//  player race 
  { // 1 
   "human",  //name
    TRUE,    //can be player
    0,       //act
    0,       //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    0,       //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 2 
   "elf",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    RES_CHARM|RES_MENTAL, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 3 
   "drow",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_DODGE|OFF_FAST|OFF_KICK_DIRT|OFF_TRIP,       //Offenciv
    0,       //Immune
    RES_CHARM|RES_MENTAL, //resist
    VULN_LIGHT,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 4 
   "dwarf",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_BERSERK,       //Offenciv
    0,       //Immune
    RES_POISON|RES_DISEASE, //resist
    VULN_DROWNING,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 5
   "duergar",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_BERSERK,       //Offenciv
    0,       //Immune
    RES_POISON|RES_DISEASE, //resist
    VULN_LIGHT|VULN_DROWNING,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 6
   "ogre",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,       //Offenciv
    0,       //Immune
    RES_POISON|RES_DISEASE, //resist
    VULN_MAGIC|VULN_SUMMON,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 7
   "orc",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,       //Offenciv
    0,       //Immune
    RES_POISON|RES_DISEASE, //resist
    VULN_MAGIC|VULN_SUMMON,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },
  
  { // 8
   "gnome",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 9
   "pixie",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED|AFF_FLYING,  //affect 1
    0,       //affect 2
    DETECT_MAGIC|DETECT_EVIL|DETECT_INVIS|ACUTE_VISION,//detect
    OFF_FAST,       //Offenciv
    0,       //Immune
    RES_CHARM|RES_SUMMON|RES_MAGIC|RES_LIGHT, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 10
   "Haoou",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    DETECT_INVIS,//detect
    OFF_FAST,       //Offenciv
    0,       //Immune
    RES_SILVER|RES_CHARM, //resist
    VULN_CHARM|VULN_SUMMON,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 11
   "troll",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    DETECT_GOOD,//detect
    OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,       //Offenciv
    0,       //Immune
    RES_POISON|RES_DISEASE, //resist
    VULN_FIRE,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 12
   "minotaur",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    0,//detect
    OFF_BASH|OFF_DISARM|OFF_KICK_DIRT|OFF_KICK,       //Offenciv
    0,       //Immune
    RES_POISON, //resist
    VULN_DISEASE,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K|W //Parts flag 
  },

  { // 13
   "skaven",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,//detect
    0,       //Offenciv
    IMM_POISON|IMM_DISEASE,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K|Q //Parts flag 
  },

  { // 14
   "Imp",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED|AFF_FLYING,  //affect 1
    0,       //affect 2
    DETECT_MAGIC|DETECT_GOOD|DETECT_INVIS|ACUTE_VISION,//detect
    OFF_FAST,       //Offenciv
    0,       //Immune
    RES_CHARM|RES_SUMMON|RES_MAGIC, //resist
    VULN_LIGHT,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },


  { // 15
   "hobbit",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 16
   "merma",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    OFF_DODGE,       //detect
    0,       //Offenciv
    IMM_DROWNING,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 17
   "centaur",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    OFF_KICK_DIRT|OFF_KICK,       //detect
    0,       //Offenciv
    0,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 18 
   "half-elf",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    RES_CHARM|RES_MENTAL, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 19 
   "felar",  //name
    TRUE,    //can be player
    0,       //act 1
    AFF_INFRARED,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_TAIL,       //Offenciv
    0,       //Immune
    0, //resist
    VULN_SUMMON,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 20 
   "thri-kreen",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    0,       //detect
    OFF_TAIL,       //Offenciv
    0,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 21
   "illithid",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    0, //resist
    0,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },

  { // 22
   "vampire",  //name
    TRUE,    //can be player
    0,       //act 1
    0,  //affect 1
    0,       //affect 2
    0,       //detect
    0,       //Offenciv
    0,       //Immune
    0, //resist
    VULN_LIGHT|VULN_PIERCE|VULN_HOLY|VULN_SILVER,       //vuln
    A|H|M|V, //form flag
    A|B|C|D|E|F|G|H|I|J|K //Parts flag 
  },
// End player Races
{ //23
"giant",
FALSE,
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



{ //24
"gargoyle",
FALSE,
0,
AFF_INFRARED,
0,
DETECT_MAGIC,
OFF_PARRY,
0,
RES_SUMMON|RES_LIGHTNING|RES_ACID	,	VULN_LIGHT	,
A|H|M|V,
A|B|C|D|E|F|G|H|I|J|K
},

{ //25
"draconian",
FALSE,
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

{ //26
"darnok",
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


{//27
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


{ //28
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
{ //29
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
{ //30
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
{ //31
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

{ //32
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
{ //33
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
{ //34
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
{ //35
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
{ //36
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
{ //37
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
{ //38
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

{ //39
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
    { //40
	"bat",			FALSE,
	0,		 AFF_FLYING,	0, 0, OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P
    },

    { //41
	"bear",			FALSE,
	0,		0,
	0, 0, OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {  //42
	"cat",			FALSE,
	0,		0,	0, 0, OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {  //43
	"centipede",		FALSE,
	0,			0, 0,	0,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K
    },

    {  //44
	"dog",			FALSE,
	0,		0,	0, 0,	OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
    },

    { //45
	"doll",			FALSE,
	0,		0,	0, 0,	0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K
    },

    { //46
        "dragon", 		FALSE,
	0, 			AFF_INFRARED|AFF_FLYING,	0, 0, 0,
	0,			RES_FIRE|RES_BASH|RES_CHARM,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },

    { //47
	"fido",			FALSE,
	0,		0,	0, 0,	OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },

    { //48
	"fox",			FALSE,
	0,		0, 0, 0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    { //49
	"goblin",		FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { //50
	"hobgoblin",		FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y
    },

    { //51
	"kobold",		FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {  //52
	"lizard",		FALSE,
	0,		0,		0, 0, 0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {  //53
	"modron",		FALSE,
	0,		AFF_INFRARED,
	0, 0, ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K
    },

    {  //54
	"orc_npc",			FALSE,
	0,		AFF_INFRARED,	0, 0, 0,
	0,		RES_DISEASE,	VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  //55
	"pig",			FALSE,
	0,		0,		0, 0, 0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },

    { //56
	"rabbit",		FALSE,
	0,		0,	0, 0,	OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K
    },

    { //57
	"school monster",	FALSE,
	ACT_NOALIGN,		0, 0, 0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },

    {  //58
	"snake",		FALSE,
	0,		0,		0, 0, 0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },

    { //59
	"song bird",		FALSE,
	0,		AFF_FLYING,	0, 0, OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {  //60
	"troll_npc",		FALSE,
	0,		AFF_INFRARED, 0,
	DETECT_HIDDEN, OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V
    },

    {  //61
	"water fowl",		FALSE,
	0,		AFF_FLYING, 0, 0,		0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    { //62
	"wolf",			FALSE,
	0,		0,	0, 0, OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {  //63
	"wyvern",		FALSE,
	0,		AFF_FLYING, 0,
	DETECT_INVIS|DETECT_HIDDEN, OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X
    },

    {  //64
	"unique",		FALSE,
	0,		0,		0, 0, 0,
	0,		0,		0,
	0,		0
    },


    {
	NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};
// ***** struct NPlayer races End ***** 
// ***** struct Player races Begin ***** 
struct	new_pc_race_type	new_pc_race_table	[NEW_MAX_PC_RACE]	=
{
 { "null",      //Name
   "",  //Who name
   "",        //Short name
   1,            // Dice hp
   1,            // Dice Mana
   1,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {100,100,100,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,100,100,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,100,100,100,100,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,100,100,100,100,100,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {18 ,18 ,18 ,18 ,18 ,18 ,21},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {7  ,7  ,7  ,7  ,7  ,7  ,7  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {1,1,1},
  //Good,Neutral,Evil
   {1,1,1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed  

 },  
 { "Human",      //Name
   "{BHuman{X",  //Who name
   "hum",        //Short name
   4,            // Dice hp
   3,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,100,100,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,100,100,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,100,100,100,100,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,999,100,100,100,100,100,
  //Thr,Ilt,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    100,100,100,100},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {19 ,19 ,19 ,19 ,19 ,19 ,21},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {7  ,7  ,7  ,7  ,7  ,7  ,7  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {1,1,1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  
 { "Elf",      //Name
   "{gElf{X",  //Who name
   "elf",        //Short name
   3,            // Dice hp
   5,            // Dice Mana
   5,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,100,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,999,999,100,999,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,100,999,100,100,

  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {16 ,20 ,17 ,20 ,20 ,16 ,24},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {6  ,13  ,7  ,10  ,10  ,8  ,15  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {1,1,-1},
  //Good,Neutral,Evil
   {1,1,-1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 }, 

 { "Drow",      //Name
   "{GDrow{X",  //Who name
   "dro",        //Short name
   3,            // Dice hp
   4,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,100,999,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,100,100,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,999,999,100,100,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {15 ,22 ,17 ,19 ,21 ,15 ,19},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {6  ,12  ,6  ,9  ,11 ,7  ,8  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },   

 { "Dwarf",      //Name
   "{yDwarf{X",  //Who name
   "dwa",        //Short name
   5,            // Dice hp
   2,            // Dice Mana
   2,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,100,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,100,100,999,999,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {21 ,14 ,19 ,18 ,11 ,21 ,12},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {18  ,20  ,7  ,8  ,5  ,16  ,5  },
   SIZE_SMALL,//Size  
  //Good,Neutral,Evil
   {1,1,-1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 }, 
 
 { "Duergar",      //Name
   "{yDuergar{X",  //Who name
   "due",        //Short name
   4,            // Dice hp
   4,            // Dice Mana
   3,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,999,999,999,100,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,999,999,100,100,999,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {21 ,14 ,20 ,18 ,13 ,20 ,10},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {15  ,10  ,10  ,10  ,6  ,15  ,6  },
   SIZE_SMALL,//Size  
  //Good,Neutral,Evil
   {-1,1,1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  
 
 { "Ogre",      //Name
   "{yOgre{X",  //Who name
   "ogr",        //Short name
   6,            // Dice hp
   3,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,999,999,100,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,100,999,999,999,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {23 ,9 ,12 ,9 ,10 ,25 ,8},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {20  ,3  ,7  ,5  ,6  ,19  ,3  },
   SIZE_HUGE,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
  
 }, 

 { "Orc",      //Name
   "{yOrc{X",  //Who name
   "orc",        //Short name
   5,            // Dice hp
   5,            // Dice Mana
   5,            // Dice Move
     //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,100,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,100,100,100,100,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {22 ,11 ,12 ,15 ,16 ,22 ,12},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {10  ,6  ,6  ,6  ,6  ,15  ,3  },
   SIZE_LARGE,//Size  
  //Good,Neutral,Evil
   {-1,1,1},
  //Good,Neutral,Evil
   {-1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
  
 }, 
 
 { "Gnome",      //Name
   "{yGnome{X",  //Who name
   "gno",        //Short name
   2,            // Dice hp
   5,            // Dice Mana
   3,            // Dice Move
  //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,100,999,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,100,999,999,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {10 ,23 ,15 ,19 ,15 ,13 ,16},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {4  ,18  ,8  ,9  ,9  ,5  ,4  },
   SIZE_SMALL,//Size  
  //Good,Neutral,Evil
   {1,1,-1},
  //Good,Neutral,Evil
   {1,1,-1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
  
 },  

 { "Pixie",      //Name
   "{yPixie{X",  //Who name
   "pix",        //Short name
   1,            // Dice hp
   6,            // Dice Mana
   2,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,100,999,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,100,999,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,999,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {7 ,24 ,24 ,17 ,20 ,9 ,16},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {1  ,19  ,12  ,10  ,10  ,1  ,5  },
   SIZE_TINY,//Size  
  //Good,Neutral,Evil
   {1,-1,-1},
  //Good,Neutral,Evil
   {1, 1, 1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
  
 },  


 { "Haoou",      //Name
   "{yHaoou{X",  //Who name
   "Hao",        //Short name
   1,            // Dice hp
   3,            // Dice Mana
   1,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,100,100,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,100,999,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,100,100,100,999,999,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {1 ,1 ,1 ,1 ,1 ,1 ,1},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {1  ,1  ,1  ,1  ,1  ,1  ,1  },
   SIZE_TINY,//Size  
  //Good,Neutral,Evil
   {-1, 1,1},
  //Good,Neutral,Evil
   {1, 1, 1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  

 { "Troll",      //Name
   "{yTroll{X",  //Who name
   "tro",        //Short name
   7,            // Dice hp
   3,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,999,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {24 ,7 ,7 ,10 ,12 ,25 ,7},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {21  ,6  ,6  ,10  ,10  ,20  ,1  },
   SIZE_HUGE,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,-1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled   
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  

 { "Minotaur",      //Name
   "{yMinotaur{X",  //Who name
   "min",        //Short name
   6,            // Dice hp
   4,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,999,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {25 ,13 ,13 ,15 ,13 ,21 ,13},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {20  ,6  ,6  ,8  ,5  ,8  ,8  },
   SIZE_LARGE,//Size  
  //Good,Neutral,Evil
   {-1,1,-1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },

 { "Skaven",      //Name
   "{ySkaven{X",  //Who name
   "ska",        //Short name
   3,            // Dice hp
   4,            // Dice Mana
   5,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,999,999,100,100,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {13 ,17 ,16 ,24 ,23 ,13 ,7},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {5  ,10  ,7  ,18  ,15  ,4  ,1  },
   SIZE_SMALL,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,1,1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },

 { "Goblin",      //Name
   "{yGoblin{X",  //Who name
   "Imp",        //Short name
   2,            // Dice hp
   6,            // Dice Mana
   3,            // Dice Move
     //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,100,100,100,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,100,999,100,100,999,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {7 ,24 ,22 ,17 ,21 ,8 ,15},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {1  ,15  ,10  ,10 ,10  ,3  ,7  },
   SIZE_TINY,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },

 { "Hobbit",      //Name
   "{yHobbit{X",  //Who name
   "hob",        //Short name
   2,            // Dice hp
   3,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,999,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,100,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,100,999,100,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {11 ,14 ,14 ,25 ,21 ,11 ,16},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {3  ,5  ,7  ,24  ,18  ,8  ,8  },
   SIZE_SMALL,//Size  
  //Good,Neutral,Evil
   {1,1,-1},
  //Good,Neutral,Evil
   {1,1,-1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },
 
 { "Merman",      //Name
   "{yMermant{X",  //Who name
   "mer",        //Short name
   4,            // Dice hp
   4,            // Dice Mana
   4,            // Dice Move
     //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,100,100,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,100,100,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,100,999,100,100,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,100,100,100,100,100,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {18 ,18 ,20 ,18 ,17 ,18 ,18},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {8  ,8  ,8  ,8  ,8  ,8  ,8  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {1,1,1},
  //Good,Neutral,Evil
   {1,1,1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 }, 
  
 { "Centaur",      //Name
   "{yCentaur{X",  //Who name
   "can",        //Short name
   5,            // Dice hp
   5,            // Dice Mana
   7,            // Dice Move
       //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,999,100,100,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,999,100,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,100,100,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,100,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {20 ,17 ,17 ,18 ,14 ,22 ,19},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {8  ,8  ,8  ,9  ,9  ,7  ,9  },
   SIZE_LARGE,//Size  
  //Good,Neutral,Evil
   {1,1,-1},
  //Good,Neutral,Evil
   {1,1,-1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },   
  
 { "Half-elf",      //Name
   "{yHalf-elf{X",  //Who name
   "hel",        //Short name
   4,            // Dice hp
   4,            // Dice Mana
   4,            // Dice Move
       //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,100,100,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,999,999,100,100,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,100,100,100,100,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {18 ,19 ,18 ,18 ,19 ,17 ,22},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {8  ,8  ,8  ,8  ,8  ,8  ,8  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {1,1,1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  

 { "Gargoyle",      //Name
   "{yGargoyleX",  //Who name
   "Gar",        //Short name
   4,            // Dice hp
   4,            // Dice Mana
   4,            // Dice Move
       //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,100,100,100,100,999,999,100,
  //Bar,Ron,Ran,Sor,Con,Ill,
    100,100,100,100,100,100,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    100,999,999,100,100,100,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,100,100,100,100,100,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {18 ,19 ,18 ,18 ,19 ,17 ,22},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {8  ,8  ,8  ,8  ,8  ,8  ,8  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {1,1,1},
  //Good,Neutral,Evil
   {1,1,1},
   1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },
  
 { "Lich",      //Name
   "{yLich{X",  //Who name
   "Lic",        //Short name
   2,            // Dice hp
   6,            // Dice Mana
   5,            // Dice Move
       //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,999,999,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    100,999,999,999,999,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {10 ,25 ,25 ,23 ,12 ,14 ,6},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {2  ,8  ,8  ,8  ,8  ,8  ,2  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,-1,1},
   0, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  

 { "Thri-kreen",      //Name
   "{yThri-kreen{X",  //Who name
   "thr",        //Short name
   3,            // Dice hp
   3,            // Dice Mana
   4,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,999,999,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,999,999,
  //Thr,Ili,                 
    100,999,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {18 ,16 ,16 ,22 ,18 ,17 ,17},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {15  ,7  ,6  ,12  ,8  ,9  ,6  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {-1,1,-1},
  //Good,Neutral,Evil
   {-1,1,1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
   {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  

 { "Illithid",      //Name
   "{yIllithid{X",  //Who name
   "ill",        //Short name
   2,            // Dice hp
   7,            // Dice Mana
   2,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,999,999,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
   999,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,999,999,
  //Thi,Ili,                 
    999,100,
  //Vwa,Vth,Vcl,Vma
    999,999,999,999},            
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {5 ,25 ,22 ,12 ,8 ,6 ,4},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {1  ,24  ,21  ,3  ,1  ,1  ,1  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,-1,1},
   -1, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 },  
  
 { "Vampire",      //Name
   "{yVampire{X",  //Who name
   "vam",        //Short name
   4,            // Dice hp
   5,            // Dice Mana
   6,            // Dice Move
      //War,mag,Cle,Rog,Pal,Kni,Bkt,
   {999,999,999,999,999,999,999,999,
  //Bar,Ron,Ran,Sor,Con,Ill,
    999,999,999,999,999,999,
  //Alc,Wit,Dia,Pri,Inq,Dru,
    999,999,999,999,999,999,
  //Nec,Sha,Mnk,Thi,Ass,Brd,Spe,
    999,999,999,999,999,999,999,
  //Thi,Ili,                 
    999,999,
  //Vwa,Vth,Vcl,Vma
    100,100,100,100},           
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {23 ,23 ,23 ,23 ,23 ,23 ,25},
  //Str,Int,Wis,Dex,Agi,Con,Cha
   {23  ,23  ,23  ,23  ,23  ,23  ,25  },
   SIZE_MEDIUM,//Size  
  //Good,Neutral,Evil
   {-1,-1,1},
  //Good,Neutral,Evil
   {-1,-1,1},
   0, // Disabled -1 == Always 0 == Crestion 1 == Enabled
  {1,1,1,1,1,1,1,1,1,1} //1 allowed 0 disallowed
 } 
};
// ***** struct Player races End ***** 
