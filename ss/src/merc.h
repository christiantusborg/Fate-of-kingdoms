/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *    ROM 2.4 is copyright 1993-1998 Russ Taylor                           *
 *    ROM has been brought to you by the ROM consortium                    *
 *        Russ Taylor (rtaylor@hypercube.org)                              *
 *        Gabrielle Taylor (gtaylor@hypercube.org)                         *
 *        Brian Moore (zump@rom.org)                                       *
 *    By using this code, you have agreed to follow the terms of the       *
 *    ROM license, in the file Rom24/doc/rom.license                       *
 ***************************************************************************/

#include <stdio.h>              /* FILE */
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Accommodate old non-Ansi compilers.
 */
#include <limits.h>             /* for CHAR_BIT */

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

#if defined(TRADITIONAL)
#define DECLARE_OBJ_FUN( fun )		void fun( )
#define DECLARE_ROOM_FUN( fun )		void fun( )
#define const
#define args( list )            ( )
#define DECLARE_DO_FUN( fun )        void fun( )
#define DECLARE_SPEC_FUN( fun )        bool fun( )
#define DECLARE_SPELL_FUN( fun )    void fun( )
#define DECLARE_EVENT_FUN( fun )    void fun( )
#else
#define args( list )            list
#define DECLARE_DO_FUN( fun )        DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )        SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )    SPELL_FUN fun
#define DECLARE_OBJ_FUN( fun )		OBJ_FUN	  fun
#define DECLARE_ROOM_FUN( fun )		ROOM_FUN  fun
#define DECLARE_EVENT_FUN( fun )        EVENT_FUN fun
#endif

/* system calls */
int unlink();
int system();

void event_check_social(void *data, char *argument, int8_t event_type);
void event_send_to_character(void *data, char *argument, int8_t event_type);
void event_mpsleep_function(void *data, char *argument, int8_t event_type);
void event_unfreeze_char(void *data, char *argument, int8_t event_type);

/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */

#if    !defined(FALSE)
#define FALSE     false
#endif

#if    !defined(TRUE)
#define TRUE     true
#endif

#if    defined(_AIX)
#if    !defined(const)
#define const
#endif
typedef int sh_int;
#define unix
#else
typedef short int sh_int;
#endif

typedef unsigned int uint;

/* Variations of void */
#define ACMD(name)           void name(CHAR_DATA *ch, char *argument)



/*
 * String and memory management parameters.
 */
#define MAX_STRING         4194304  /*4mb */
#define MAX_STRING_LENGTH     4096  /*4kb */
#define MAX_KEY_HASH          1024  /*1kb */
#define MAX_INPUT_LENGTH       768
#define PAGELEN                 40  //22
#define LINE_SEPERATOR "{b-------------------------------------------------------------------------------{n\n\r"

/* I am lazy :) */
#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH


/*
 * Structure types.
*/
typedef struct crs_data CRS;
typedef struct acct_data ACCT_DATA;
typedef struct affect_data AFFECT_DATA;
typedef struct area_data AREA_DATA;
typedef struct ban_data BAN_DATA;
typedef struct buf_type BUFFER;
typedef struct char_data CHAR_DATA;
typedef struct char_class CHAR_CLASS;
typedef struct descriptor_data DESCRIPTOR_DATA;
typedef struct exit_data EXIT_DATA;
typedef struct extra_descr_data EXTRA_DESCR_DATA;
typedef struct help_data HELP_DATA;
typedef struct contents_data CONTENTS_DATA;
typedef struct help_area_data HELP_AREA;
typedef struct kill_data KILL_DATA;
typedef struct mem_data MEM_DATA;
typedef struct mob_index_data MOB_INDEX_DATA;
typedef struct note_data NOTE_DATA;
typedef struct obj_data OBJ_DATA;
typedef struct obj_index_data OBJ_INDEX_DATA;
typedef struct pc_data PC_DATA;
typedef struct reset_data RESET_DATA;
typedef struct room_index_data ROOM_INDEX_DATA;
typedef struct shop_data SHOP_DATA;
typedef struct time_info_data TIME_INFO_DATA;
typedef struct weather_data WEATHER_DATA;
typedef struct prog_list PROG_LIST;
typedef struct prog_code PROG_CODE;
typedef struct colour_data COLOUR_DATA;
typedef struct grant_data GRANT_DATA;
typedef struct key_data KEY_DATA;   // keyring - Kira
typedef struct mprog_tag_data MPROG_TAG;
typedef struct mprog_event_data MPROG_EVENT;
typedef struct expedition_data EXPEDITION_DATA;
typedef struct junkyard_data JUNKYARD_DATA;
typedef struct packrat_data PACKRAT_DATA;
typedef struct room_history_data ROOM_HISTORY_DATA;
typedef struct war_t WAR_TABLE;
typedef struct gq_t GQ_TABLE;
typedef struct player_shop_data PLAYER_SHOP_DATA;
typedef struct donation_room_data DONATION_ROOM_DATA;
typedef struct chararray_type CHARARRAY;
typedef struct item_shop_data ITEM_SHOP_DATA;
typedef struct item_donation_room_data ITEM_DONATION_ROOM_DATA;
typedef struct rune_slot_data RUNE_SLOT_DATA;

/* Mapper stuff */
//extern int top_map_visit;
//extern int top_map_qd;

/* Rendelven - Clan System Data */
typedef struct clan_data CLAN_DATA;
typedef struct clan_rank_data RANK_DATA;
typedef struct clan_roster_data ROSTER_DATA;
typedef struct disabled_data DISABLED_DATA;
typedef struct player_ranks_data PLAYER_RANKS_DATA;
extern int maxClans;
#define MAX_RANK 6

/* Kira - Group Data */
typedef struct group_data GROUP_DATA;
extern int maxGroups;
#define MAX_INVITES 15
#define MAX_GROUP_NAME_LENGTH 30

// FAMILY
typedef struct family_data FAMILY_DATA;

#define PORT_MAIN                       1234

#define IS_MAIN_PORT()                 (port == PORT_MAIN)

// Family status

#define FSTATUS_ALONE			0
#define FSTATUS_HEAD 			1
#define FSTATUS_ELDER			2
#define FSTATUS_MEMBER			3
#define FSTATUS_PENDING			4

// Proposal status
#define PROPOSAL_PENDING		0
#define PROPOSING			1

// Family relationship

#define RELATION_ALONE			0
#define RELATION_FATHER			1
#define RELATION_MOTHER			2
#define	RELATION_SON			3
#define RELATION_DAUGHTER		4
#define RELATION_BROTHER		5
#define RELATION_SISTER			6

// Family cost -- How much it costs to register a family

#define FAMILY_COST				50000

// Family data

struct family_data {
   FAMILY_DATA *next;
   bool valid;
   char *name;
};

//Clan defines
#define CONTROL_STATE_UNCLAIMED 0
#define CONTROL_STATE_LOCKED 1
#define CONTROL_STATE_CONTEST 2
#define CONTROL_POINT_MAX 500

// hunt defines //
#define WORLD_SIZE                      32700
#define GO_OK_SMARTER           1

#define HASH_KEY(ht,key)        ((((unsigned int)(key))*17)%(ht)->table_size)
#define IS_DIR                          (get_room_index(q_head->room_nr)->exit[i])
#define GO_OK                           (!IS_SET(IS_DIR->exit_info, EX_CLOSED))
// ----------- //

// define types for simplify hunt code... Saybor //
typedef struct hash_link HASH_LINK;
typedef struct hash_header HASH_HEADER;
typedef struct room_q ROOM_Q;

// HUNT DATA //
struct hash_link {
   int key;
   HASH_LINK *next;
   void *data;
};
struct hunting_data {
   char *name;
   CHAR_DATA **victim;
};
struct room_q {
   int room_nr;
   ROOM_Q *next_q;
};
struct nodes {
   int visited;
   int ancestor;
};

struct hash_header {
   int rec_size;
   int table_size;
   int *keylist, klistsize, klistlen;  /* this is really lame, AMAZINGLY lame */
   HASH_LINK **buckets;
};

//Exploration holder.
//Basically just a linked list of int's where we only use the bits.
typedef struct explore_holder {
   struct explore_holder *next;
   unsigned int bits;
   int mask;
} EXPLORE_HOLDER;

typedef struct exploration_data {
   EXPLORE_HOLDER *bits;
   int set;
} EXPLORE_DATA;

// Trap data - Kira

#define TRAP_DAM_POLLEN       0
#define TRAP_DAM_FIRE         1
#define TRAP_DAM_COLD         2
#define TRAP_DAM_ACID         3
#define TRAP_DAM_LIGHTNING    4
#define TRAP_DAM_PIERCE       5
#define TRAP_DAM_SLASH        6
#define TRAP_DAM_GAS	      7
#define TRAP_DAM_SNARE	      8
#define TRAP_DAM_POISON       9
#define TRAP_DAM_SULFUR       10
#define TRAP_DAM_BACKLASH     11
#define TRAP_DAM_BEAR         12

// Keyring data - Kira

KEY_DATA *new_key args((void));
OBJ_DATA *find_keyring args((CHAR_DATA * ch));
int remove_key args((CHAR_DATA * ch, OBJ_DATA * keyring, char *name));
int add_key args((OBJ_DATA * keyring, OBJ_DATA * key));
void free_key args((KEY_DATA * key));
void list_keys args((CHAR_DATA * ch, OBJ_DATA * keyring));
void load_keyring args((FILE * fp, OBJ_DATA * obj));
void save_keyring args((FILE * fp, OBJ_DATA * obj));

struct key_data {
   KEY_DATA *next;
   bool valid;
   OBJ_INDEX_DATA *index;
   int value[1];
};

MPROG_TAG *new_mprog_tag args((void));
bool ch_affected(CHAR_DATA * ch, char *argument);
void free_mprog_tag args((MPROG_TAG * pTag));
bool has_mobprog_tag args((CHAR_DATA * ch, char *tag));
bool has_mobprog_tag_group_single(CHAR_DATA * ch, char *tag);
bool has_mobprog_tag_group_full(CHAR_DATA * ch, char *tag);
bool add_mobprog_tag args((CHAR_DATA * ch, char *tag, long minutes));
bool remove_mobprog_tag args((CHAR_DATA * ch, char *tag));
void update_timed_tags args((CHAR_DATA * ch));
void do_mobprog_tag_list(CHAR_DATA * ch, CHAR_DATA * victim, char *argument);

struct mprog_tag_data {
   char *tag;
   long expiration;
   MPROG_TAG *next;
};

/* Roster info:
 * This holds the information one person in the clan
 * clan_data has a list 
 */
struct clan_roster_data {
   ROSTER_DATA *next;
   char *name;
   sh_int level;
   sh_int rank;
   int class;
   int race;
   char *laston;
   sh_int valid;
   sh_int sex;                  /* member sex, for gender-specific-ranks */
};

/* This holds information on specific 'ranks'.
   Can add more info on each rank if needed. */
struct clan_rank_data {
   char *name;                  /* Rank Title */
   sh_int max_members;          /* max members for this rank, 0 if unlimited */
   sh_int current_members;      /* List how many members are of this rank */
   sh_int min_level;            /* minimum level you must be to be promoted to this rank; 0 is any */
};


/* one disabled command */
struct disabled_data {
   DISABLED_DATA *next;         /* pointer to next node */
   struct cmd_type const *command;  /* pointer to the command struct */
   char *disabled_by;           /* name of disabler */
   sh_int level;                /* level of disabler */
};

/* The different leaderboard ranks */
#define RANK_LEVELS              1
#define RANK_QUESTPOINTS         2
#define RANK_WAR_KILLS           3
#define RANK_PLAYER_KILLS        4
#define RANK_PLAYER_DEATHS       5
#define RANK_COMBAT_ROOM_KILLS   6
#define RANK_FAUX_LEVELS         7
#define RANK_ARENA_KILLS         8
#define RANK_ARENA_DEATHS        9
#define RANK_COMPLETED_EXPEDITIONS 10
#define RANK_MUDPIES             11
#define RANK_GUESS_PHRASE_WINS   12
#define RANK_COMPLETED_QUESTS    13
#define RANK_SCRABBLE_WORDS      14
#define RANK_MOBS_KILLED         15
#define RANK_GOLD                16
#define RANK_COMPLETED_GQS       17

/* Rank data */
struct player_ranks_data {
   PLAYER_RANKS_DATA *next;     /* pointer to next node */
   char *name;                  /* name of player */
   int level;                   /* level of player */
   int combat_kills;
   int questpoints;
   int wkills;
   int pkills;
   int pdeaths;
   int faux;
   int expeditions_complete;
   int arena_kills;
   int arena_deaths;
   int mudpie;
   int gwins;
   int qcomplete;
   int turnins;
   int kills_mob;
   long gold;
   int gqwins;
   char *clan;
};

/*
 * Function types.
 */
typedef void DO_FUN args((CHAR_DATA * ch, char *argument));
typedef bool SPEC_FUN args((CHAR_DATA * ch));
typedef void SPELL_FUN args((int sn, int level, CHAR_DATA * ch, void *vo, int target));

typedef void OBJ_FUN args((OBJ_DATA * obj, char *argument));
typedef void ROOM_FUN args((ROOM_INDEX_DATA * room, char *argument));
typedef void EVENT_FUN args((void *data, char *argument, int8_t event_type));

MPROG_EVENT *new_mprog_event args((void));
void free_mprog_event args((MPROG_EVENT * pEvent));
MPROG_EVENT *create_mprog_event args((DO_FUN * function, CHAR_DATA * mob));

struct mprog_event_data {
   CHAR_DATA *mob;
   DO_FUN *function;
   MPROG_EVENT *next;
};

//Junkyard Data
#define MAX_JUNKYARD 10

JUNKYARD_DATA *new_junkyard_data(void);
void free_junkyard_data(JUNKYARD_DATA * junk);
void junkyard_update(void);
void fwrite_junkyard(FILE * fp, CHAR_DATA * ch);
int check_junkyard_obj(CHAR_DATA * ch, OBJ_DATA * obj);
bool is_junkyard_mob(CHAR_DATA * ch, CHAR_DATA * victim);

struct junkyard_data {
   MOB_INDEX_DATA *junkyard_giver;
   int8_t num_tasks;
   OBJ_INDEX_DATA *tasks[MAX_JUNKYARD];
   char *room[MAX_JUNKYARD];
   bool task_status[MAX_JUNKYARD];
   int16_t junkyard_timer;
   JUNKYARD_DATA *next;
};

//Junkyard Items
#define JUNK1    13906
#define JUNK2    13907
#define JUNK3    13908
#define JUNK4    13909
#define JUNK5    13910
#define JUNK6    13911
#define JUNK7    13912
#define JUNK8    13913
#define JUNK9    13914
#define JUNK10   13915

//Expedition Data
#define MAX_EXPEDITION 10

EXPEDITION_DATA *new_expedition_data(void);
void free_expedition_data(EXPEDITION_DATA * camp);
void expedition_update(void);
void fwrite_expedition(FILE * fp, CHAR_DATA * ch);
int check_expedition_mob(CHAR_DATA * ch, CHAR_DATA * victim);
bool is_expedition_mob(CHAR_DATA * ch, CHAR_DATA * victim);

struct expedition_data {
   MOB_INDEX_DATA *expedition_giver;
   int8_t num_tasks;
   MOB_INDEX_DATA *tasks[MAX_EXPEDITION];
   bool task_status[MAX_EXPEDITION];
   int16_t expedition_timer;
   EXPEDITION_DATA *next;
};

struct grant_data {
   GRANT_DATA *next;
   DO_FUN *do_fun;
   char *name;
   int duration;
   int level;
};

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_GOD            6
#define MAX_DEVOUT	   6
#define MAX_QUESTION       9
#define MAX_AMOUNT_CARRY   500
#define MAX_DECK_ITEMS     10
#define MAX_SKILL          318
#define MAX_NPC_FOLLOWERS  7
#define MAX_PK_RANGE       25
#define MAX_TRAPS_IN_AREA  3
#define MAX_RACE_TIER      15   //Race tiers (used in do_racelist)
#define MAX_WHO_DESC_LENGTH 17   //Who string length
#define MAX_TITLE_LENGTH   40   //Title length
#define MAX_EVENT_SECONDS  300
#define MAX_GROUP          32
#define MAX_IN_GROUP       23
#define MAX_ALIAS          60
#define MAX_FRIEND         70
#define MAX_VIEW           20
#define MAX_IGNORE         20
#define MAX_CONSENT        3
#define MAX_CLASS          7
#define MAX_TIER           1
#define MAX_PC_RACE        37
#define MAX_DAMAGE_MESSAGE 52
#define MAX_VITALITY       400
#define MAX_LIMIT_BREAK    4000
#define MAX_CHANNEL_HISTORY 10
#define MAX_VNUM           32768
#define MAX_HP             250
#define MAX_MANA           250
#define MAX_LISTED_RANKS   50
#define VITALITY_LOSS_LEVEL 25
#define FAUX_REQ           1000
#define MAX_DAMAGE         100

#define MAX_LEVEL           LEVEL_HEAD_IMP
#define LEVEL_HERO          150
#define LEVEL_SUPERHERO     151

#define LEVEL_HEAD_IMP      161
#define LEVEL_IMP           160
#define LEVEL_CODER         159
#define LEVEL_ENFORCER      158
#define LEVEL_CLAN_AFFAIRS  157
#define LEVEL_HEAD_RP       156
#define LEVEL_EVENT_RUNNER  155
#define LEVEL_HEAD_BUILDER  154
#define LEVEL_BUILDER       153
#define LEVEL_IMMORTAL      152
#define LEVEL_TRAINEE       LEVEL_IMMORTAL
#define L_IMM               LEVEL_IMMORTAL

/* for command types */
#define ML      MAX_LEVEL
#define L1      LEVEL_CODER
#define L2      LEVEL_ENFORCER
#define L3      LEVEL_CLAN_AFFAIRS
#define L4      LEVEL_HEAD_RP
#define L5      LEVEL_EVENT_RUNNER
#define L6      LEVEL_HEAD_BUILDER
#define L7      LEVEL_BUILDER
#define IM      LEVEL_IMMORTAL

/* Maxes on Stat apps */
#define MAX_STR 60
#define MAX_INT 100
#define MAX_WIS 150
#define MAX_DEX 25
#define MAX_CON 90

#define MAX_STAT 350

#define VALUE_GOLD 		0
#define MAX_AUCTION_PAYMENT     1
#define VALUE_AQUEST_POINT	2
#define VALUE_IQUEST_POINT	3
#define VALUE_PLATINUM	        4
#define VALUE_DIAMOND           5
#define VALUE_BLACK_DIAMOND     6
#define VALUE_TRIDIAMOND        7

#define WEDDING_COST            50000

/* Channel Numbers - used for public_ch - Kira */
#define FLAME           1
#define IMMTALK         2
#define FTALK           3
#define FAMILYTALK      4
#define GRATZ           5
#define GTELL           6
#define IC              7
#define MUSIC           8
#define NEWBIE          9
#define GOSSIP          10
#define QCHAT           11
#define QA              12
#define QUOTE           13
#define SPOUSE          14
#define HELPER          15
#define WORSHIPPER      16
#define TELLS           17
#define REPLY           18
#define LTALK           19
#define CLAN            20
#define GCLAN           21

/* Added this for "orphaned help" code. Check do_help() -- JR */
#define MAX_CMD_LEN			50

/* Defines for classes, be sure to update with each new class! */

/* Tier 0 */
#define CLASS_MAGE        0
#define CLASS_CLERIC      1
#define CLASS_THIEF       2
#define CLASS_WARRIOR     3
#define CLASS_GAIAN       4
#define CLASS_PSIONICIST    5
#define CLASS_ARCHER      6

#define PULSE_PER_SECOND    4
#define PULSE_VIOLENCE      ( 3 * PULSE_PER_SECOND)
#define PULSE_MOBILE        ( 4 * PULSE_PER_SECOND)
#define PULSE_OBJ_PROG      ( 4 * PULSE_PER_SECOND)
#define PULSE_MUSIC         ( 6 * PULSE_PER_SECOND)
#define PULSE_HEALTH        (50 * PULSE_PER_SECOND)
#define PULSE_TICK          (60 * PULSE_PER_SECOND)
#define PULSE_AREA          (120 * PULSE_PER_SECOND) // 2 minutes
#define PULSE_NEWBIE        (180 * PULSE_PER_SECOND)
#define PULSE_POTATO        (75 * PULSE_PER_SECOND)   // Kira
#define PULSE_REQUIRE       (30 * PULSE_PER_SECOND)
#define PULSE_FTAG	    PULSE_PER_SECOND // MachinShin
#define PULSE_START_GUESS   3600 // Kira - (15 minutes)
#define PULSE_EXITS	    ( 7 * PULSE_PER_SECOND)   // Kira
#define PULSE_NOFLOOR       1;  // Kira
#define PULSE_SLIPPERY      90;
#define PULSE_START_FTAG    14400;  // Kira - 60 minutes - 240 is a minute
#define PULSE_GHOST         240; // Kira - 5 minutes is 1200
#define PULSE_SCATTER       ( 36000 * PULSE_PER_SECOND ) // Kira - 10 hours
#define PULSE_SCRABBLE      ( 45000 * PULSE_PER_SECOND ) // Kira - 15 hours
#define PULSE_TRACK         (5 * PULSE_PER_SECOND)
#define PULSE_GAME_DATA     (3500 * PULSE_PER_SECOND)
#define PULSE_GRENADE       ( 1 * PULSE_PER_SECOND)

#define MISSING_TARGET_DELAY    10

/*
 * This structure is used for the
 * pathfinding algorithm.
 */

char *pathfind args((ROOM_INDEX_DATA * from, ROOM_INDEX_DATA * to));
typedef struct heap_data {
   sh_int iVertice;
   ROOM_INDEX_DATA **knude;
   int size;
} HEAP;

/* Key Ring Stuff - Kira */
#define KR_OK			0
#define KR_NO_KEYRING		1
#define KR_NO_KEY		2
#define KR_NO_REMKEY		3
#define KR_INVALID_KEYRING	4
#define KR_INVALID_KEY		5
#define KR_INVALID_NAME		6
#define KR_KEY_DUP		7

/*
 * ColoUr stuff v2.0, by Lope.
 */
#define CLEAR       "\e[0m"     /* Resets Colour    */
#define BLINK       "\e[5m"     /* Rendelven - blink on */
#define C_RED       "\e[0;31m"  /* Normal Colours    */
#define C_GREEN     "\e[0;32m"
#define C_YELLOW    "\e[0;33m"
#define C_BLUE      "\e[0;34m"
#define C_MAGENTA   "\e[0;35m"
#define C_CYAN      "\e[0;36m"
#define C_WHITE     "\e[0;37m"
#define C_D_GREY    "\e[1;30m"  /* Light Colors        */
#define C_B_RED     "\e[1;31m"
#define C_B_GREEN   "\e[1;32m"
#define C_B_YELLOW  "\e[1;33m"
#define C_B_BLUE    "\e[1;34m"
#define C_B_MAGENTA "\e[1;35m"
#define C_B_CYAN    "\e[1;36m"
#define C_B_WHITE   "\e[1;37m"

#define BG_RED       "\e[0;41m" /* Normal Colours    */
#define BG_GREEN     "\e[0;42m"
#define BG_YELLOW    "\e[0;43m"
#define BG_BLUE      "\e[0;44m"
#define BG_MAGENTA   "\e[0;45m"
#define BG_CYAN      "\e[0;46m"
#define BG_WHITE     "\e[0;47m"
#define BG_D_GREY    "\e[1;40m" /* Light Colors        */
#define BG_B_RED     "\e[1;41m"
#define BG_B_GREEN   "\e[1;42m"
#define BG_B_YELLOW  "\e[1;43m"
#define BG_B_BLUE    "\e[1;44m"
#define BG_B_MAGENTA "\e[1;45m"
#define BG_B_CYAN    "\e[1;46m"
#define BG_B_WHITE   "\e[1;47m"

#define COLOUR_NONE 7           /* White, hmm...    */
#define RED         1           /* Normal Colours    */
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7
#define BLACK       0

#define NORMAL      0           /* Bright/Normal colours */
#define BRIGHT      1

#define ALTER_COLOUR( type )    if( !str_prefix( argument, "red" ) )        \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = RED;            \
                }                        \
                else if( !str_prefix( argument, "hi-red" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = RED;            \
                }                        \
                else if( !str_prefix( argument, "green" ) )    \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = GREEN;        \
                }                        \
                else if( !str_prefix( argument, "hi-green" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = GREEN;        \
                }                        \
                else if( !str_prefix( argument, "yellow" ) )    \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = YELLOW;        \
                }                        \
                else if( !str_prefix( argument, "hi-yellow" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = YELLOW;        \
                }                        \
                else if( !str_prefix( argument, "blue" ) )    \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = BLUE;        \
                }                        \
                else if( !str_prefix( argument, "hi-blue" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = BLUE;        \
                }                        \
                else if( !str_prefix( argument, "magenta" ) )    \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = MAGENTA;        \
                }                        \
                else if( !str_prefix( argument, "hi-magenta" ) ) \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = MAGENTA;        \
                }                        \
                else if( !str_prefix( argument, "cyan" ) )    \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = CYAN;            \
                }                        \
                else if( !str_prefix( argument, "hi-cyan" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = CYAN;            \
                }                        \
                else if( !str_prefix( argument, "white" ) )    \
                {                        \
                    ch->pcdata->type[0] = NORMAL;        \
                    ch->pcdata->type[1] = WHITE;        \
                }                        \
                else if( !str_prefix( argument, "hi-white" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = WHITE;        \
                }                        \
                else if( !str_prefix( argument, "grey" ) )    \
                {                        \
                    ch->pcdata->type[0] = BRIGHT;        \
                    ch->pcdata->type[1] = BLACK;        \
                }                        \
                else if( !str_prefix( argument, "beep" ) )    \
                {                        \
                    ch->pcdata->type[2] = 1;            \
                }                        \
                else if( !str_prefix( argument, "nobeep" ) )    \
                {                        \
                    ch->pcdata->type[2] = 0;            \
                }                        \
                else                        \
                {                        \
        write_to_buffer(ch->desc, "Unrecognised colour, unchanged.\n\r", -1 );    \
                    return;                    \
                }

#define LOAD_COLOUR( field )    ch->pcdata->field[1] = fread_number( fp );    \
                if( ch->pcdata->field[1] > 100 )        \
                {                        \
                    ch->pcdata->field[1] -= 100;        \
                    ch->pcdata->field[2] = 1;            \
                }                        \
                else                        \
                {                        \
                    ch->pcdata->field[2] = 0;            \
                }                        \
                if( ch->pcdata->field[1] > 10 )            \
                {                        \
                    ch->pcdata->field[1] -= 10;            \
                    ch->pcdata->field[0] = 1;            \
                }                        \
                else                        \
                {                        \
                    ch->pcdata->field[0] = 0;            \
                }

/*
 * Thanks Dingo for making life a bit easier ;)
 */
#define CH(d)                   ((d)->original ? (d)->original : (d)->character)

/*
 * Site ban structure.
 */

#define BAN_SUFFIX     A
#define BAN_PREFIX     2
#define BAN_NEWBIES    C
#define BAN_ALL        D
#define BAN_PERMIT     E
#define BAN_PERMANENT  F

struct ban_data {
   BAN_DATA *next;
   bool valid;
   sh_int ban_flags;
   sh_int level;
   char *name;
};

struct packrat_data {
   PACKRAT_DATA *next;
   char *obj_vnum;              //Packrat obj vnum
};

/*
 * Drunk struct
 */
struct struckdrunk {
   int min_drunk_level;
   int number_of_rep;
   char *replacement[11];
};

struct struckgibberish {
   int number_of_rep;
   char *replacement[11];
};

struct buf_type {
   BUFFER *next;
   bool valid;
   sh_int state;                /* error state of the buffer */
   sh_int size;                 /* size in k */
   char *string;                /* buffer's string */
};

/*
 * Devouts
 * Religious Spells: Be sure to set the number to
 * the number that is equal to the Devout.
 * -1 means that the spell is not religious.
 * 0 means the spell is for those in Kira's religion.
 */

#define DEVOUT_NULL     0
#define DEVOUT_ALYS     1
#define DEVOUT_GWYNNETH 2
#define DEVOUT_ATASHANT 3
#define DEVOUT_HADEN    4

/*
 * Time and weather stuff.
 */
#define SUN_DARK       0
#define SUN_RISE       1
#define SUN_LIGHT      2
#define SUN_SET        3

#define SKY_CLOUDLESS  0
#define SKY_CLOUDY     1
#define SKY_RAINING    2
#define SKY_LIGHTNING  3

struct time_info_data {
   int hour;
   int day;
   int month;
   int year;
};

struct weather_data {
   int mmhg;
   int change;
   int sky;
   int sunlight;
   int moon_phase_good;         // moon function
   int moon_phase_evil;         // moon function
   int moon_phase_neutral;      // moon function
};

//Update this if you add another phase
#define MAX_MOON_PHASE        3

#define GOOD_MOON_WANING      0
#define GOOD_MOON_NEW_MOON    1
#define GOOD_MOON_WAXING      2
#define GOOD_MOON_FULL_MOON   3

#define EVIL_MOON_WANING      0
#define EVIL_MOON_NEW_MOON    1
#define EVIL_MOON_WAXING      2
#define EVIL_MOON_FULL_MOON   3

#define NEUTRAL_MOON_WANING        0
#define NEUTRAL_MOON_NEW_MOON      1
#define NEUTRAL_MOON_WAXING        2
#define NEUTRAL_MOON_FULL_MOON     3

#define EVENT_TYPE_CHAR    1
#define EVENT_TYPE_OBJECT  2
#define EVENT_TYPE_ROOM    3
#define EVENT_TYPE_MPROG   4
#define EVENT_TYPE_UNKNOWN 0

/*
 * Connected state for a channel.
 */
#define CON_GET_ACCOUNT                 -28
#define CON_GET_OLD_PASSWORD		-27
#define CON_CONFIRM_NEW_ACCOUNT         -26
#define CON_GET_NEW_PASSWORD		-25
#define CON_CONFIRM_NEW_PASSWORD	-24
#define CON_OLD_CHANGE_PASSWORD		-23
#define CON_GET_CHANGE_PASSWORD		-22
#define CON_CONFIRM_CHANGE_PASSWORD	-21
#define CON_OLD_SECURITY_ANSWER	        -20
#define CON_SECURITY_QUESTION		-19
#define CON_SECURITY_ANSWER	        -18
#define CON_ACCOUNT_MENU                -17
#define CON_GET_NAME			-16
#define CON_CONFIRM_NEW_NAME		-15
#define CON_ANSI			-14
#define CON_GET_NEW_RACE		-13
#define CON_GET_NEW_SEX			-12
#define CON_GET_NEW_CLASS		-11
#define CON_GET_ASPECT	  	        -10
#define CON_GET_ALIGNMENT               -9
#define CON_ENTER_EMAIL                 -8
#define CON_CHOOSE_START  	        -7
#define CON_DELETE			-6
#define CON_CONFIRM_DELETE  	        -5
#define CON_CONFIRM_DELETE_QA  	        -4
#define CON_GET_WIZI			-3
#define CON_BREAK_CONNECT		-2
#define CON_COPYOVER_RECOVER		-1
#define CON_PLAYING			 0
#define CON_READ_IMOTD			 1
#define CON_READ_MOTD			 2
#define CON_NOTE_TO			 5
#define CON_NOTE_SUBJECT		 6
#define CON_NOTE_EXPIRE			 7
#define CON_NOTE_TEXT			 8
#define CON_NOTE_FINISH			 9
#define CON_NOTE_FROM			 10

/*
 * Descriptor (channel) structure.
 */
struct descriptor_data {
   DESCRIPTOR_DATA *next;
   DESCRIPTOR_DATA *snoop_by;
   CHAR_DATA *character;
   CHAR_DATA *original;
   ACCT_DATA *account;
   bool valid;
   bool ansi;
   bool telnetga;
   char *host;
   char *ip;
   sh_int descriptor;
   sh_int connected;
   bool fcommand;
   char inbuf[4 * MAX_INPUT_LENGTH];
   char incomm[MAX_INPUT_LENGTH];
   char inlast[MAX_INPUT_LENGTH];
   int repeat;
   char *outbuf;
   int outsize;
   int outtop;
   char *showstr_head;
   char *showstr_point;
   char *run_buf;
   char *run_head;
   void *pEdit;                 /* OLC */
   char **pString;              /* OLC */
   int editor;                  /* OLC */
   int expiry;                  // Hitch
};

/*
 * Attribute bonus structures.
 */
struct str_app_type {
   sh_int tohit;
   sh_int todam;
   sh_int carry;
   sh_int wield;
};

struct int_app_type {
   sh_int manap;
   sh_int learn;
};

struct wis_app_type {
   sh_int practice;
};

struct dex_app_type {
   sh_int defensive;
};

struct con_app_type {
   sh_int hitp;
   sh_int shock;
};

struct mana_app_type {
   sh_int mana;
};

/*
 * TO types for act.
 */
#define TO_ROOM           0
#define TO_NOTVICT        1
#define TO_VICT           2
#define TO_CHAR           3
#define TO_ALL            4
#define TO_WORLD          5

/*
 * Help table types.
 */
struct help_data {
   HELP_DATA *next;
   HELP_DATA *next_area;
   sh_int level;
   char *keyword;
   char *text;
   char *desc;
   char *contents;
   char *related;
   char *lastedited;
};

struct contents_data {
   CONTENTS_DATA *next;
   sh_int level;
   char *name;
};

struct help_area_data {
   HELP_AREA *next;
   HELP_DATA *first;
   HELP_DATA *last;
   char *filename;
   bool changed;
   bool valid;
};

struct chararray_type {
   int32_t iElements;
   char **szArrayList;
};

CHARARRAY *CreateArrayList();
void AddItemToArrayList(const char *pszInput, CHARARRAY * arrayList);
void FreeArrayList(CHARARRAY * arrayList);
char *GetArrayListElement(CHARARRAY * arrayList, int32_t iElement);
void ShowArrayListToChar(CHARARRAY * arrayList, CHAR_DATA * ch);
void fwrite_ArrayList(CHARARRAY * arrayList, FILE * fp);
void fread_ArrayList(CHARARRAY * arrayList, FILE * fp);

void do_playershop_buy(CHAR_DATA * ch, char *argument);
void do_playershop_list(CHAR_DATA * ch, char *argument);
void check_playershop_expiration(CHAR_DATA * ch);

/*
 * Shop types.
 */
#define MAX_PLAYER_SHOP_ITEM  100
#define MAX_PLAYER_SINGLE_ITEM 100

#define SHOP_UNIT_GOLD  0
#define SHOP_UNIT_QUEST 1

struct item_shop_data {
   OBJ_DATA *shop_item;
   int8_t shop_item_count;
   int16_t shop_item_cost;
   int8_t shop_item_unit;
};

struct player_shop_data {
   int32_t shop_vnum;
   char *shop_owner;
   long shop_owner_id;
   char *shop_name;

   ITEM_SHOP_DATA *shop_items[MAX_PLAYER_SHOP_ITEM];

   int16_t qp_holdings;
   int16_t gold_holdings;

   CHARARRAY *logArrayList;

   long permit_expiration;

   PLAYER_SHOP_DATA *next;
};

/*
 * Donation room types
 */

#define MAX_DONATION_ROOM_ITEM  300
#define MAX_DONATION_SINGLE_ITEM 100

struct item_donation_room_data {
   OBJ_DATA *donation_room_item;
   int8_t donation_item_count;
   char * donated_by;
};


struct donation_room_data {
   int donation_room_vnum;

   ITEM_DONATION_ROOM_DATA *donation_room_items[MAX_DONATION_ROOM_ITEM];

   DONATION_ROOM_DATA *next;
};


#define MAX_TRADE    5

struct shop_data {
   SHOP_DATA *next;             /* Next shop in list        */
   sh_int keeper;               /* Vnum of shop keeper mob    */
   sh_int buy_type[MAX_TRADE];  /* Item types shop will buy    */
   sh_int profit_buy;           /* Cost multiplier for buying    */
   sh_int profit_sell;          /* Cost multiplier for selling    */
   sh_int open_hour;            /* First opening hour        */
   sh_int close_hour;           /* First closing hour        */
};

/*
 * Per-class stuff.
 */

#define MAX_GUILD  2
#define MAX_STATS  6
#define STAT_STR   0
#define STAT_INT   1
#define STAT_WIS   2
#define STAT_DEX   3
#define STAT_CON   4
#define STAT_LUCK  5

struct class_type {
   char *name;                  /* the full name of the class  */
   char who_name[9];            /* Three-letter name for 'who' */
   sh_int attr_prime;           /* Prime attribute             */
   sh_int weapon;               /* First weapon                */
   sh_int skill_adept;          /* Maximum skill level         */
   sh_int thac0_00;             /* Thac0 for level  0          */
   sh_int thac0_32;             /* Thac0 for level 32          */
   sh_int hp_min;               /* Min hp gained on leveling   */
   sh_int hp_max;               /* Max hp gained on leveling   */
   sh_int mana_min;             /* Min mana gained on leveling */
   sh_int mana_max;             /* Max mana gained on leveling */
   bool fMana;                  /* Class gains mana on level   */
   sh_int tier;                 /* Tier                        */  
};

struct item_type {
   int type;
   char *name;
};

struct weapon_type {
   char *name;
   sh_int vnum;
   sh_int *gsn;
   int dice[2];
};

struct armor_type {
   char *name;
   int ac;
   int max_dex;
   int acheck;
   int spell_fail;
};

struct wiznet_type {
   char *name;
   long flag;
   int level;
};

struct attack_type {
   char *name;                  /* name         */
   char *noun;                  /* message      */
   int damage;                  /* damage class */
};

struct monster_advancement {
   int hit;
   int ac;
   int bab;
   int dam;
};

struct race_type {
   char *name;                  /* call name of the race          */
   bool pc_race;                /* can be chosen by pcs           */
   long act;                    /* act bits for the race          */
   long aff;                    /* aff bits for the race          */
   long off;                    /* off bits for the race          */
   long imm;                    /* imm bits for the race          */
   long res;                    /* res bits for the race          */
   long vuln;                   /* vuln bits for the race         */
   long form;                   /* default form flag for the race */
   long parts;                  /* default parts for the race     */
   char *walkmsg;
   int level;
   int hit[3];
   int ac;
   int ab;
   int dam[3];
};

struct pc_race_type {           /* additional data for pc races    */
   char *name;                  /* MUST be in race_type            */
   char who_name[8];
   sh_int class_mult[MAX_CLASS]; /* exp multiplier for class, * 100 */
   char *skills[5];             /* bonus skills for the race       */
   sh_int stats[MAX_STATS];     /* starting stats                  */
   sh_int training_cost[MAX_STATS]; /* training costs                  */
   sh_int size;                 /* aff bits for the race           */
   sh_int tier;                 /* race tier            */
   char *race_type;             /* race category type         */
};

struct spec_type {
   char *name;                  /* special function name */
   SPEC_FUN *function;          /* the function          */
};

/*
 * Data structure for notes.
 */

struct note_data {
   NOTE_DATA *next;
   bool valid;
   sh_int type;
   char *sender;
   bool anon;                   // MachinShin
   char *date;
   char *to_list;
   char *subject;
   char *text;
   time_t date_stamp;
   time_t expire;
   int note_num;
};

struct acct_data {
   ACCT_DATA *next;
   DESCRIPTOR_DATA *desc;
   bool valid;
   bool deny;
   bool permit;
   bool immortal;
   sh_int tries;
   sh_int delete;
   sh_int reconnect;
   char *character[10];
   char *name;
   char *pwd;
   char *security_question;
};

/*
 * An affect.
 */
struct affect_data {
   AFFECT_DATA *next;
   bool valid;
   sh_int where;
   sh_int type;
   sh_int level;
   float duration;
   sh_int location;
   sh_int modifier;
   int bitvector;
};

/* where definitions */
#define TO_AFFECTS   0
#define TO_OBJECT    1
#define TO_IMMUNE    2
#define TO_RESIST    3
#define TO_VULN      4
#define TO_WEAPON    5
#define TO_AFFECTS2  6
#define TO_OBJECT2   7

#define MAX_RUNE_SLOTS 10

#define IS_RUNE(obj_rune)  ( (obj_rune)->item_type == ITEM_RUNE )

#define RUNE_TYPE_NORMAL   (A)
#define RUNE_TYPE_WHITE    (2)
#define RUNE_TYPE_CHAOS    (C)
#define RUNE_TYPE_CLEAN    (D)
#define RUNE_TYPE_MASTER   (E)

#define RUNE_SPEC_NO_SLOTS  10
#define RUNE_SPEC_BLOW_UP   15
#define RUNE_SPEC_BOTH      20

struct rune_slot_data {
   int8_t rune_type;
   char *rune_description;
   AFFECT_DATA rune_affect;     //any allocated rune slot will always have an affect
   bool slot_taken;
   RUNE_SLOT_DATA *next;
};

/*
 * A kill structure (indexed by level).
 */
struct kill_data {
   sh_int number;
   sh_int killed;
};

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_FIDO       3090
#define MOB_VNUM_CITYGUARD  3060
#define MOB_VNUM_VAMPIRE    3404
#define MOB_VNUM_FAMILY_MOB 20250
#define MOB_VNUM_ANIMATE    13820
#define MOB_VNUM_QUEST_ASSISTANT 129
#define MOB_VNUM_WERETIGER  13858
#define MOB_SPELLUP         13859

//Summons
#define MOB_VNUM_AIR_ELEMENTAL   13816
#define MOB_VNUM_FIRE_ELEMENTAL  13817
#define MOB_VNUM_EARTH_ELEMENTAL 13819
#define MOB_VNUM_WATER_ELEMENTAL 13818
#define MOB_VNUM_LESSER_GOLEM    13833
#define MOB_VNUM_STONE_GOLEM     13834
#define MOB_VNUM_IRON_GOLEM      13835
#define MOB_VNUM_SUM_SHADOW      13836
#define MOB_VNUM_SKELETON        13837
#define MOB_VNUM_ZOMBIE          13838
#define MOB_VNUM_MUMMY           13839
#define MOB_VNUM_LION            13840
#define MOB_VNUM_HUNTER          13841
#define MOB_VNUM_TREANT          13842
#define MOB_VNUM_NIGHTWALKER     13843
#define MOB_VNUM_SHADOW          13844
#define MOB_VNUM_SQUIRE          13845
#define MOB_VNUM_SPECIAL_GUARD   13846
#define MOB_VNUM_STALKER         13847
#define MOB_VNUM_WOLF            13848
#define MOB_VNUM_WAR_HOUND       13849
#define MOB_VNUM_DEMON           13850

#define MOB_VNUM_PANTHER        13821
#define MOB_VNUM_EAGLE		13822
#define MOB_VNUM_BEAR		13823
#define MOB_VNUM_DRAGON		13824
#define MOB_VNUM_GRYPHON	13825

#define MOB_VNUM_IDIOT             13814
#define MOB_VNUM_QUEST_BRO         13815
#define MOB_VNUM_JACK_SKELLINGTON  13827  //Halloween Only

#define MOB_VNUM_PATROLMAN  2106
#define GROUP_VNUM_TROLLS   2100
#define GROUP_VNUM_OGRES    2101
#define MOB_VNUM_GUARDIAN   13826

/* RT ASCII conversions -- used so we can have letters in this file */

#define A            1
// #define B            2
#define C            4
#define D            8
#define E           16
#define F           32
#define G           64
#define H          128

#define I          256
#define J          512
#define K         1024
// #define L         2048
#define M         4096
#define N         8192
#define O        16384
#define P        32768

#define Q        65536
#define R       131072
#define S       262144
#define T       524288
#define U      1048576
#define V      2097152
#define W      4194304
#define X      8388608

#define Y     16777216
#define Z     33554432
#define aa    67108864          /* doubled due to conflicts */
#define bb   134217728
#define cc   268435456
#define dd   536870912
#define ee  1073741824

/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC         (A)  /* Auto set for mobs  */
#define ACT_SENTINEL       (2)  /* Stays in one room  */
#define ACT_SCAVENGER      (C)  /* Picks up objects   */
#define ACT_AGGRESSIVE     (D)  /* Attacks PC's       */
#define ACT_STAY_AREA      (E)  /* Won't leave area   */
#define ACT_WIMPY          (F)
#define ACT_PET            (G)  /* Auto set for pets  */
#define ACT_TRAIN          (H)  /* Can train PC's     */
#define ACT_PRACTICE       (I)  /* Can practice PC's  */
#define ACT_UNDEAD         (J)
#define ACT_CLERIC         (K)
#define ACT_MAGE           (2048)
#define ACT_THIEF          (M)
#define ACT_WARRIOR        (N)
#define ACT_NOALIGN        (O)
#define ACT_NOPURGE        (P)
#define ACT_OUTDOORS       (Q)
#define ACT_INDOORS        (R)
#define ACT_IS_HEALER      (S)
#define ACT_GAIN           (T)
#define ACT_UPDATE_ALWAYS  (U)
#define ACT_IS_CHANGER     (V)
#define ACT_MORTINVIS      (W)
#define ACT_IS_RESTRINGER  (X)
#define ACT_SAFE           (Y)
#define ACT_MOUNTABLE	   (Z) //remove this
#define ACT_NOSCRY	   (aa)
#define ACT_NOQUEST        (bb)
#define ACT_QPSHOPKEEPER   (cc)
#define ACT_ADJUSTER	   (dd)
#define ACT_SERVICES       (ee)

/* NPC ACT3 FLAGS */
#define ACT3_RECHARGER      (A)
#define ACT3_NO_BASH_TIMER  (2)
#define ACT3_DELIVERY       (C)
#define ACT3_NOWHERE        (D)
#define ACT3_NOEXP          (E)
#define ACT3_NOGATE         (F)
#define ACT3_TEACHER        (G)
#define ACT3_NOTRACK        (H)
#define ACT3_HUNTER         (I)
#define ACT3_INVADER        (J)
#define ACT3_WORTHQP        (K)
#define ACT3_WORTHXP        (2048)
#define ACT3_WORTHGP        (M)
#define ACT3_NODETER        (N)
#define ACT3_PARTY_QUEST    (O)
#define ACT3_PEACEFUL       (P)
#define ACT3_NOFLEE         (Q)

/* damage classes */
#define DAM_NONE            0
#define DAM_BASH            1
#define DAM_PIERCE          2
#define DAM_SLASH           3
#define DAM_FIRE            4
#define DAM_COLD            5
#define DAM_LIGHTNING       6
#define DAM_ACID            7
#define DAM_POISON          8
#define DAM_NEGATIVE        9
#define DAM_HOLY           10
#define DAM_ENERGY         11
#define DAM_MENTAL         12
#define DAM_DISEASE        13
#define DAM_DROWNING       14
#define DAM_LIGHT          15
#define DAM_OTHER          16
#define DAM_HARM           17
#define DAM_CHARM          18
#define DAM_SOUND          19
#define DAM_AIR		   20
#define DAM_WIND 	   21
#define DAM_BLIND	   22
#define DAM_WEAKEN         23
#define DAM_SILVER         24
#define DAM_WATER          25

/* OFF bits for mobiles */
#define OFF_AREA_ATTACK    (A)
#define OFF_BACKSTAB       (2)
#define OFF_BASH           (C)
#define OFF_BERSERK        (D)
#define OFF_DISARM         (E)
#define OFF_DODGE          (F)
#define OFF_FADE           (G)
#define OFF_FAST           (H)
#define OFF_KICK           (I)
#define OFF_KICK_DIRT      (J)
#define OFF_PARRY          (K)
#define OFF_RESCUE         (2048)
#define OFF_TAIL           (M)
#define OFF_TRIP           (N)
#define OFF_CRUSH          (O)
#define ASSIST_ALL         (P)
#define ASSIST_ALIGN       (Q)
#define ASSIST_RACE        (R)
#define ASSIST_PLAYERS     (S)
#define ASSIST_GUARD       (T)
#define ASSIST_VNUM        (U)
#define OFF_CIRCLE         (V)

/* return values for check_imm */
#define IS_NORMAL          0
#define IS_IMMUNE          1
#define IS_RESISTANT       2
#define IS_VULNERABLE      3

/* IMM bits for mobs */
#define IMM_SUMMON         (A)
#define IMM_CHARM          (2)
#define IMM_MAGIC          (C)
#define IMM_WEAPON         (D)
#define IMM_BASH           (E)
#define IMM_PIERCE         (F)
#define IMM_SLASH          (G)
#define IMM_FIRE           (H)
#define IMM_COLD           (I)
#define IMM_LIGHTNING      (J)
#define IMM_ACID           (K)
#define IMM_POISON         (2048)
#define IMM_NEGATIVE       (M)
#define IMM_HOLY           (N)
#define IMM_ENERGY         (O)
#define IMM_MENTAL         (P)
#define IMM_DISEASE        (Q)
#define IMM_DROWNING       (R)
#define IMM_LIGHT          (S)
#define IMM_SOUND          (T)
#define IMM_WOOD           (U)
#define IMM_SILVER         (V)
#define IMM_IRON           (W)
#define IMM_AIR		   (X)
#define IMM_BLIND	   (Y)
#define IMM_WEAKEN         (Z)
#define IMM_WATER          (aa)

/* RES bits for mobs */
#define RES_SUMMON         (A)
#define RES_CHARM          (2)
#define RES_MAGIC          (C)
#define RES_WEAPON         (D)
#define RES_BASH           (E)
#define RES_PIERCE         (F)
#define RES_SLASH               (G)
#define RES_FIRE                (H)
#define RES_COLD                (I)
#define RES_LIGHTNING           (J)
#define RES_ACID                (K)
#define RES_POISON              (2048)
#define RES_NEGATIVE            (M)
#define RES_HOLY                (N)
#define RES_ENERGY              (O)
#define RES_MENTAL              (P)
#define RES_DISEASE             (Q)
#define RES_DROWNING            (R)
#define RES_LIGHT        (S)
#define RES_SOUND        (T)
#define RES_WOOD                (U)
#define RES_SILVER              (V)
#define RES_IRON                (W)
#define RES_WATER               (X)

/* VULN bits for mobs */
#define VULN_SUMMON        (A)
#define VULN_CHARM        (2)
#define VULN_MAGIC              (C)
#define VULN_WEAPON             (D)
#define VULN_BASH               (E)
#define VULN_PIERCE             (F)
#define VULN_SLASH              (G)
#define VULN_FIRE               (H)
#define VULN_COLD               (I)
#define VULN_LIGHTNING          (J)
#define VULN_ACID               (K)
#define VULN_POISON             (2048)
#define VULN_NEGATIVE           (M)
#define VULN_HOLY               (N)
#define VULN_ENERGY             (O)
#define VULN_MENTAL             (P)
#define VULN_DISEASE            (Q)
#define VULN_DROWNING           (R)
#define VULN_LIGHT        (S)
#define VULN_SOUND        (T)
#define VULN_WOOD               (U)
#define VULN_SILVER             (V)
#define VULN_IRON        (W)
#define VULN_WATER              (X)

/* body form */
#define FORM_EDIBLE             (A)
#define FORM_POISON             (2)
#define FORM_MAGICAL            (C)
#define FORM_INSTANT_DECAY      (D)
#define FORM_OTHER              (E) /* defined by material bit */

/* actual form */
#define FORM_ANIMAL             (F)
#define FORM_SENTIENT           (G)
#define FORM_UNDEAD             (H)
#define FORM_CONSTRUCT          (I)
#define FORM_MIST               (J)
#define FORM_INTANGIBLE         (K)
#define FORM_BIPED              (2048)
#define FORM_CENTAUR            (M)
#define FORM_INSECT             (N)
#define FORM_SPIDER             (O)
#define FORM_CRUSTACEAN         (P)
#define FORM_WORM               (Q)
#define FORM_BLOB        	(R)
#define FORM_MAMMAL             (S)
#define FORM_BIRD               (T)
#define FORM_REPTILE            (U)
#define FORM_SNAKE              (V)
#define FORM_DRAGON             (W)
#define FORM_AMPHIBIAN          (X)
#define FORM_FISH               (Y)
#define FORM_COLD_BLOOD         (Z)
#define FORM_NO_CORPSE          (aa)

/* body parts */
#define PART_HEAD               (A)
#define PART_ARMS               (2)
#define PART_LEGS               (C)
#define PART_HEART              (D)
#define PART_BRAINS             (E)
#define PART_GUTS               (F)
#define PART_HANDS              (G)
#define PART_FEET               (H)
#define PART_FINGERS            (I)
#define PART_EAR                (J)
#define PART_EYE        (K)
#define PART_LONG_TONGUE        (2048)
#define PART_EYESTALKS          (M)
#define PART_TENTACLES          (N)
#define PART_FINS               (O)
#define PART_WINGS              (P)
#define PART_TAIL               (Q)

/* for combat */
#define PART_CLAWS              (R)
#define PART_FANGS              (S)
#define PART_HORNS              (T)
#define PART_SCALES             (U)
#define PART_TUSKS              (V)

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND                (A)
#define AFF_INVISIBLE            (2)
#define AFF_TRUE_SIGHT           (C)
#define AFF_WARRIOR_BONUS        (D)
#define AFF_CLERIC_BONUS         (E)
#define AFF_THIEF_BONUS          (F)
#define AFF_MAGE_BONUS           (G)
#define AFF_SANCTUARY            (H)
#define AFF_FAERIE_FIRE          (I)
#define AFF_INFRARED             (J)
#define AFF_CURSE                (K)
#define AFF_CORRUPTION           (2048)
#define AFF_POISON               (M)
#define AFF_UNBREAKABLE          (N)
#define AFF_PSI_BONUS            (O)
#define AFF_SNEAK                (P)
#define AFF_HIDE                 (Q)
#define AFF_SLEEP                (R)
#define AFF_CHARM                (S)
#define AFF_FLYING               (T)
#define AFF_PASS_DOOR            (U)
#define AFF_HASTE                (V)
#define AFF_CALM                 (W)
#define AFF_PLAGUE               (X)
#define AFF_WEAKEN               (Y)
#define AFF_NIGHT_VISION         (Z)
#define AFF_BERSERK              (aa)
#define AFF_SWIM                 (bb)
#define AFF_REGENERATION         (cc)
#define AFF_SLOW                 (dd)
#define AFF_UNDERWATER_BREATHING (ee)

/*
 * Bits for 'affected2_by'.
 * Used in #MOBILES.
 */

#define AFF2_MYSTIC_MIGHT      (A)
#define AFF2_BLUR              (2)
#define AFF2_DETECT_LOCATION   (C)
#define AFF2_DETER	       (D)
#define AFF2_HOLY_SYMBOL       (E)
#define AFF2_GAIAN_BONUS       (F)
#define AFF2_WHISPERING_WIND   (G)
#define AFF2_FARSIGHT	       (H)
#define AFF2_PSYCHIC_AURA      (I)
#define AFF2_IRON_GRIP	       (J)
#define AFF2_PROTECT_VOODOO    (K)
#define AFF2_ELEMENTAL_AURA    (2048)
#define AFF2_STEEL_SKIN	       (M)
#define AFF2_HEAVYFEET         (N)
#define AFF2_MAGIC_GUARD       (O)

/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL  0
#define SEX_MALE     1
#define SEX_FEMALE   2

/* dice */
#define DICE_NUMBER  0
#define DICE_TYPE    1
#define DICE_BONUS   2

/* size */
#define SIZE_TINY    0
#define SIZE_SMALL   1
#define SIZE_MEDIUM  2
#define SIZE_LARGE   3
#define SIZE_HUGE    4
#define SIZE_GIANT   5

/* QUEST ITEMS */
#define QUEST_ITEM2                13818  // quest shield
#define QUEST_ITEM3                13816  // wings of flight
#define QUEST_ITEM4                13819  // Decanter endless water
#define QUEST_ITEM5                13815  // Ring of regeneration
#define QUEST_ITEM11               13844  // Aura of Sanctuary
#define QUEST_ITEM12               13818  // Credit Card
#define QUEST_ITEM16               1229   // Sleeping bag
#define QUEST_ITEM18               13964
//missing 18-28
#define QUEST_ITEM29		   13878 // bag of spells
#define QUEST_ITEM30		   20377 // Mini-questmaster - sphere of knowledge
#define QUEST_ITEM31		   16432 // Exp saver - Guardian Angel
#define QUEST_ITEM32		   16414 // House ticket
//33
#define QUEST_ITEM34		   16415 // Religious Emblem
#define QUEST_ITEM35               16416  // Translucent Robe
//36, 37
#define QUEST_ITEM38               16463  // Quest Light

/* Warrior QEQ */
#define WARRIOR_UNDERBELLY         16419
#define WARRIOR_FLAMBERGE          16418
#define WARRIOR_LOOP               16420
#define WARRIOR_BALDRIC            16421
#define WARRIOR_GAUNTLETS          16422
#define WARRIOR_SABATONS           16423
#define WARRIOR_CHAPS              16424
#define WARRIOR_SKIRT              16425
#define WARRIOR_SLEEVES            16426
#define WARRIOR_PLATE              16427
#define WARRIOR_HELM               16456

/* Cleric QEQ */
#define CLERIC_MORNING_STAR        16393
#define CLERIC_STANDARD            16394
#define CLERIC_NEUTRAL_TOME        16395
#define CLERIC_EVIL_TOME           16396
#define CLERIC_GOOD_TOME           16397
#define CLERIC_NEUTRAL_BELT        16398
#define CLERIC_EVIL_BELT           16399
#define CLERIC_GOOD_BELT           16400
#define CLERIC_GAUNTLETS           16401
#define CLERIC_BOOTS               16402
#define CLERIC_SKIRT               16403
#define CLERIC_SLEEVES             16407
#define CLERIC_CHAIN_MAIL          16408
#define CLERIC_NEUTRAL_DIADEM      16411
#define CLERIC_EVIL_DIADEM         16412
#define CLERIC_GOOD_DIADEM         16468

/* Thief QEQ */
#define THIEF_CASTER               16383
#define THIEF_STARS                20376
#define THIEF_BACKPACK             16384
#define THIEF_GEM_POUCH            16385
#define THIEF_BANDILERO            16386
#define THIEF_GLOVES               16387
#define THIEF_SLIPPERS             16388
#define THIEF_PANTS                16389
#define THIEF_SLEEVES              16390
#define THIEF_QUILTED_SILK         16391
#define THIEF_COWL                 16392

/* Mage QEQ */
#define MAGE_STAFF                 16372
#define MAGE_HARNESS               16373
#define MAGE_POUCH                 16374
#define MAGE_BELT                  16375
#define MAGE_JESSES                16376
#define MAGE_SLIPPERS              16377
#define MAGE_SKIRT                 16378
#define MAGE_TATTOOS               16379
#define MAGE_ROBE                  16380
#define MAGE_HEADBAND              16381

/* Psi QEQ */
#define PSI_CESTI                  16365
#define PSI_HAMMOCK                16366
#define PSI_SASH                   16367
#define PSI_BELT                   16368
#define PSI_WRAPPINGS              16369
#define PSI_SANDALS                16370
#define PSI_PANTALOONS             16095
#define PSI_SLEEVES                16371
#define PSI_VEST                   16410
#define PSI_TURBAN                 16466

/* Psi QEQ */
#define GAIAN_SICKLE               16355
#define GAIAN_LEAF                 16356
#define GAIAN_SPORRAN              16357
#define GAIAN_BELT                 16358
#define GAIAN_GLOVES               16359
#define GAIAN_BOOTS                16360
#define GAIAN_LEGGINGS             16361
#define GAIAN_VINES                16362
#define GAIAN_TUNIC                16363
#define GAIAN_TORQUE               16364

/* Autoquest items */
#define QUEST_OBJQUEST1 13833
#define QUEST_OBJQUEST2 13834
#define QUEST_OBJQUEST3 13835
#define QUEST_OBJQUEST4 13836
#define QUEST_OBJQUEST5 13837

/* Area Vnums */
#define AREA_VNUM_LOAD		   33
#define AREA_VNUM_FTAG_ARENA       37
#define AREA_VNUM_ARENA		   38
#define AREA_VNUM_PLAYER_HOMES     25
#define AREA_VNUM_ORIENTAL_EVERGLADE 66
#define AREA_VNUM_SECRETS_WIND     52
#define AREA_VNUM_REDLIGHT         68
#define AREA_VNUM_DRUNK_RACES      88
#define AREA_VNUM_LABYRINTH        86
#define AREA_VNUM_CKPQ             82
#define AREA_VNUM_NEWBIE_ACADEMY   71

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */

/*Card Deck*/
#define OBJ_VNUM_HOUSE_TOKEN        16414
#define OBJ_VNUM_RACE_CHANGE        16413
#define OBJ_VNUM_PLUSHIE_TOKEN      16495
#define OBJ_VNUM_FREE_FAUX          16496
#define OBJ_VNUM_STR_RUNE           13828
#define OBJ_VNUM_INT_RUNE           13829
#define OBJ_VNUM_WIS_RUNE           13830
#define OBJ_VNUM_DEX_RUNE           13831
#define OBJ_VNUM_CON_RUNE           13850
#define OBJ_VNUM_LUCK_RUNE          13852
//End Card Deck

/* Mage Outfit */
#define OBJ_VNUM_MAGE_STAFF        13817
#define OBJ_VNUM_MAGE_ROBES        13862
#define OBJ_VNUM_MAGE_SLIPPERS     13863
#define OBJ_VNUM_MAGE_RING         13872
#define OBJ_VNUM_MAGE_HAT          13875

/* Warrior Outfit */
#define OBJ_VNUM_WARRIOR_SHIELD    13916
#define OBJ_VNUM_WARRIOR_SWORD     13917
#define OBJ_VNUM_WARRIOR_HELMET    13918
#define OBJ_VNUM_WARRIOR_CHAINMAIL 13919
#define OBJ_VNUM_WARRIOR_BOOTS     13930

/* Thief Outfit */
#define OBJ_VNUM_THIEF_CLAW        13931
#define OBJ_VNUM_THIEF_STARS       13932
#define OBJ_VNUM_THIEF_BELT        13933
#define OBJ_VNUM_THIEF_POUCH       13934
#define OBJ_VNUM_THIEF_CLOAK       13935
#define OBJ_VNUM_THIEF_MASK        13936
#define OBJ_VNUM_THIEF_VEST        13937

/* Cleric outfit */
#define OBJ_VNUM_CLERIC_AID        13938
#define OBJ_VNUM_CLERIC_ROBES      13939
#define OBJ_VNUM_CLERIC_BOOK       13940
#define OBJ_VNUM_CLERIC_DOVE       13941
#define OBJ_VNUM_CLERIC_GLOVES     13942

/* Gaian outfit */
#define OBJ_VNUM_GAIAN_ORB         13943
#define OBJ_VNUM_GAIAN_CLOAK       13944
#define OBJ_VNUM_GAIAN_BRANCH      13945
#define OBJ_VNUM_GAIAN_POT         13946
#define OBJ_VNUM_GAIAN_PENDANT     13947

/* Psionicist outfit */
#define OBJ_VNUM_PSIONICIST_HOOD    13948
#define OBJ_VNUM_PSIONICIST_BARRIER 13949
#define OBJ_VNUM_PSIONICIST_MIND    13950
#define OBJ_VNUM_PSIONICIST_VEST    13951
#define OBJ_VNUM_PSIONICIST_SASH    13952

/* Archer outfit */
#define OBJ_VNUM_ARCHER_ARROW       13953
#define OBJ_VNUM_ARCHER_BOW         13954
#define OBJ_VNUM_ARCHER_HAT         13955
#define OBJ_VNUM_ARCHER_OVERALL     13956
#define OBJ_VNUM_ARCHER_BELT        13957
#define OBJ_VNUM_ARCHER_WATERSKIN   13958

/* Lights of Aspects */
#define OBJ_VNUM_LIGHT_ASAYA       13900
#define OBJ_VNUM_LIGHT_NOS         13901
#define OBJ_VNUM_LIGHT_LHEIUS      13904

/* Starter eq for all */
#define OBJ_VNUM_SLINGBAG          13959
#define OBJ_VNUM_WATER_FLASK       13960
#define OBJ_VNUM_BREAD             13961
#define OBJ_VNUM_COCOON            13962
#define OBJ_VNUM_WISHING_STONE     13921
#define OBJ_VNUM_CANDLE            13871

/*Mark of the Legendary <class> stuff*/
#define OBJ_MARK_MAGE     6617
#define OBJ_MARK_WARRIOR  6618
#define OBJ_MARK_THIEF    6619
#define OBJ_MARK_CLERIC   6634
#define OBJ_MARK_GAIAN    6635
#define OBJ_MARK_PSIONICIST 6636
#define OBJ_MARK_ARCHER   6637

#define OBJ_VNUM_QUEST_PILL         19476
#define OBJ_VNUM_RESTRING_TOKEN     16497
#define OBJ_VNUM_STUCK_ARROW        13905
#define OBJ_VNUM_GOLD_ONE           2
#define OBJ_VNUM_GOLD_SOME          3
#define OBJ_VNUM_COINS              5

#define OBJ_VNUM_START_SWORD       20126
#define OBJ_VNUM_START_MACE        20124
#define OBJ_VNUM_START_DAGGER      20282
#define OBJ_VNUM_START_AXE         20279
#define OBJ_VNUM_START_FLAIL       20274
#define OBJ_VNUM_START_WHIP        20276
#define OBJ_VNUM_START_POLEARM     20283
#define OBJ_VNUM_START_CLAW        20284
#define OBJ_VNUM_START_BOW         20286
#define OBJ_VNUM_START_STAFF       20185

#define OBJ_VNUM_MUTILATED_CORPSE_PC 9
#define OBJ_VNUM_CORPSE_NPC        10
#define OBJ_VNUM_CORPSE_PC         11
#define OBJ_VNUM_SEVERED_HEAD      12
#define OBJ_VNUM_TORN_HEART        13
#define OBJ_VNUM_SLICED_ARM        14
#define OBJ_VNUM_SLICED_LEG        15
#define OBJ_VNUM_GUTS              16
#define OBJ_VNUM_BRAINS            17

#define OBJ_VNUM_MUSHROOM          20
#define OBJ_VNUM_LIGHT_BALL        21
#define OBJ_VNUM_SPRING            22
#define OBJ_VNUM_DISC              23
#define OBJ_VNUM_PORTAL            25

#define OBJ_VNUM_CARD_TICKET       16473
#define OBJ_VNUM_BONUS_WAND	   13877
#define OBJ_VNUM_BONUS_PORTAL      13880
#define OBJ_VNUM_REMORT_MEDAL      16436
#define OBJ_VNUM_DOOR1             13879
#define OBJ_VNUM_DOOR2             13881
#define OBJ_VNUM_CRYSTAL_FRAGMENT  24782
#define OBJ_VNUM_ENCHANTED_CRYSTAL 24783
#define OBJ_VNUM_QUEST_COIN        13888
#define OBJ_VNUM_FLOATABOVE        16457
#define OBJ_VNUM_FLOATAROUND       16465
#define OBJ_VNUM_COSTUME           13858

#define OBJ_VNUM_PROPHET_GEM       13882
#define OBJ_VNUM_ROSE              20245
#define OBJ_VNUM_CAMPFIRE          13832
#define OBJ_VNUM_PIT               3010
#define OBJ_VNUM_HOT_POTATO        13838
#define OBJ_VNUM_HEALING_BED	   13847
#define OBJ_VNUM_TRAP              16486
#define OBJ_VNUM_GAIAN_TREE        13825
#define OBJ_VNUM_GAIAN_FRUIT       13826

#define OBJ_VNUM_GUESS_LETTER      13843

#define OBJ_VNUM_GUIDE             20100
#define OBJ_VNUM_BARREL            8700
#define OBJ_VNUM_MUDPIE_TOKEN	   13848
#define OBJ_VNUM_PKTROPHY	   13860
#define OBJ_VNUM_FOOD_CREATION     13873
#define OBJ_VNUM_PACKRAT_BIN       20399

/* SCATTER STUFF - Kira */
#define OBJ_VNUM_CHOCO_BUNNY1      16487
#define OBJ_VNUM_CHOCO_BUNNY2      16488
#define OBJ_VNUM_CHOCO_BUNNY3      16489
#define OBJ_VNUM_CHOCO_BUNNY4      16490
#define OBJ_VNUM_CHOCO_BUNNY5      16491
#define OBJ_VNUM_CHOCO_BUNNY6      16492

/* Scrabble Prizes - Kira */
#define OBJ_VNUM_PRINT             24783

/* Scrabble Pieces - Kira */
#define OBJ_VNUM_SCRABBLE_A	24750
#define OBJ_VNUM_SCRABBLE_B	24752
#define OBJ_VNUM_SCRABBLE_C	24753
#define OBJ_VNUM_SCRABBLE_D	24754
#define OBJ_VNUM_SCRABBLE_E	24755
#define OBJ_VNUM_SCRABBLE_F	24756
#define OBJ_VNUM_SCRABBLE_G	24757
#define OBJ_VNUM_SCRABBLE_H	24758
#define OBJ_VNUM_SCRABBLE_I	24759
#define OBJ_VNUM_SCRABBLE_J	24760
#define OBJ_VNUM_SCRABBLE_K	24761
#define OBJ_VNUM_SCRABBLE_L	24762
#define OBJ_VNUM_SCRABBLE_M	24763
#define OBJ_VNUM_SCRABBLE_N	24764
#define OBJ_VNUM_SCRABBLE_O	24765
#define OBJ_VNUM_SCRABBLE_P	24766
#define OBJ_VNUM_SCRABBLE_Q	24767
#define OBJ_VNUM_SCRABBLE_R	24768
#define OBJ_VNUM_SCRABBLE_S	24769
#define OBJ_VNUM_SCRABBLE_T	24770
#define OBJ_VNUM_SCRABBLE_U	24771
#define OBJ_VNUM_SCRABBLE_V	24772
#define OBJ_VNUM_SCRABBLE_W	24773
#define OBJ_VNUM_SCRABBLE_X	24774
#define OBJ_VNUM_SCRABBLE_Y	24775
#define OBJ_VNUM_SCRABBLE_Z	24776
#define OBJ_VNUM_SCRABBLE_BLANK 24779

#define OBJ_VNUM_WHISTLE           2116

#define OBJ_VNUM_BASS              13814  // Type of fish
#define OBJ_VNUM_RAINBOWFISH       13820  // Shallow
#define OBJ_VNUM_COD               13821  // Deep
#define OBJ_VNUM_SALMON1           13822  // Shallow or deep
#define OBJ_VNUM_PERCH             14187  // Shallow or deep
#define OBJ_VNUM_SALMON2	   13823 // Shallow or deep
#define OBJ_VNUM_REDSNAPPER	   14189 // Shallow or deep
#define OBJ_VNUM_SMELT	 	   14190 // Shallow
#define OBJ_VNUM_SARDINE           14191  // Shallow
#define OBJ_VNUM_CRAPPIE           14192  // Shallow
#define OBJ_VNUM_HERRING           14193  // Shallow or deep
#define OBJ_VNUM_CATFISH           14188  // Deep
#define OBJ_VNUM_EEL               14194  // Deep
#define OBJ_VNUM_TROUT		   14195 // Shallow or deep
#define OBJ_VNUM_WALLEYE	   14196 // Deep
#define OBJ_VNUM_PIKE              14197  // Deep
#define OBJ_VNUM_MUSKIE            14198  // Deep
#define OBJ_VNUM_BOOT		   14199 // Shallow

#define OBJ_VNUM_WARTROPHY         14186
#define OBJ_VNUM_VOODOO            13963

#define	LOCKER_VNUM		   14185 // 100 items
#define LOCKER_UPGRADE_1	   14184 // 500 items
#define LOCKER_UPGRADE_2	   14183 // 750 items
#define LOCKER_UPGRADE_3	   14182 // 1000 items */

// Mining Ores
#define OBJ_VNUM_BRONZE_ORE     13867
#define OBJ_VNUM_IRON_ORE	13868
#define OBJ_VNUM_STEEL_ORE	13869
#define OBJ_VNUM_SILVER_ORE	13870
#define OBJ_VNUM_GOLD_ORE	13871
#define OBJ_VNUM_GEMSTONE_ORE	13872

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT       1
#define ITEM_SCROLL      2
#define ITEM_WAND        3
#define ITEM_STAFF       4
#define ITEM_WEAPON      5
#define ITEM_TREASURE    6
#define ITEM_ARMOR       7
#define ITEM_POTION      8
#define ITEM_CLOTHING    9
#define ITEM_FURNITURE   10
#define ITEM_TRASH       11
#define ITEM_CONTAINER   12
#define ITEM_DRINK_CON   13
#define ITEM_KEY         14
#define ITEM_FOOD        15
#define ITEM_MONEY       16
#define ITEM_BOAT        17
#define ITEM_CORPSE_NPC  18
#define ITEM_CORPSE_PC   19
#define ITEM_FOUNTAIN    20
#define ITEM_PILL        21
#define ITEM_PROTECT     22
#define ITEM_MAP         23
#define ITEM_PORTAL      24
#define ITEM_WARP_STONE  25
#define ITEM_ROOM_KEY    26
#define ITEM_GEM         27
#define ITEM_JEWELRY     28
#define ITEM_JUKEBOX     29
#define ITEM_QUEST       30
#define ITEM_MUDPIE       31
#define ITEM_ROD	 32
#define ITEM_BAIT        33
#define ITEM_QUESTPILL   34
#define ITEM_SHOVEL	 35
#define ITEM_LOCKER      36
#define ITEM_RETURN_SCROLL    37
#define ITEM_TELE_ROCK        38
#define ITEM_CONTRACT         39
#define ITEM_GATEWAY          40
#define ITEM_THROWING_STARS   41
#define ITEM_SUMMONING_STONE  43 /* used for find familiar spell */
#define ITEM_MATCHES          44
#define ITEM_ARROW	      45   /* For Bows */
#define ITEM_PHOENIX_DOWN     46 /* When carried, player does not lose exp when dead */
#define ITEM_EXP_SAVER        47 /* Don't Lose as much exp when you die - quest item */
#define ITEM_RAZOR	      48
#define ITEM_KEYRING	      49
#define ITEM_TRAP	      50
#define ITEM_OWLEYE	      51
#define ITEM_OWLHEAD	      52
#define ITEM_SPIRIT_GEM       53
#define ITEM_COSTUME          54
#define ITEM_GRENADE          55
#define ITEM_NUCLEAR          56
#define ITEM_WETSTONE         57
#define ITEM_RUNE             58
#define ITEM_UNIQUE           59

/* grenade flags */
#define GRENADE_IS_MINE		(A)
#define GRENADE_QUIET_TICK	(2)
#define GRENADE_LIGHTABLE	(C)
#define GRENADE_PULLABLE	(D)

/* grenade states - Jeff Maxx */
#define GRENADE_NOTPULLED   0
#define GRENADE_PULLED      1

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW          (A)
#define ITEM_HUM           (2)
#define ITEM_DARK          (C)
#define ITEM_LOCK          (D)
#define ITEM_EVIL          (E)
#define ITEM_INVIS         (F)
#define ITEM_MAGIC         (G)
#define ITEM_NODROP        (H)
#define ITEM_BLESS         (I)
#define ITEM_ANTI_GOOD     (J)
#define ITEM_ANTI_EVIL     (K)
#define ITEM_ANTI_NEUTRAL  (2048)
#define ITEM_NOREMOVE      (M)
#define ITEM_INVENTORY     (N)
#define ITEM_NOPURGE       (O)
#define ITEM_ROT_DEATH     (P)
#define ITEM_VIS_DEATH     (Q)
#define ITEM_NONMETAL      (R)
#define ITEM_NOLOCATE      (S)
#define ITEM_MELT_DROP     (T)
#define ITEM_HAD_TIMER     (U)
#define ITEM_SELL_EXTRACT  (V)
#define ITEM_BURN_PROOF    (W)
#define ITEM_NOUNCURSE     (X)
#define ITEM_KEEP          (Y)
#define ITEM_DEATHKEEP     (Z)
#define ITEM_NODONATE      (aa)
#define ITEM_COAT          (bb)
#define ITEM_NOTEPAPER     (cc)
#define ITEM_NO_PK         (dd) //REMOVE THIS BIT
#define ITEM_HIDDEN	   (ee)

/*
 * Extra2 Flags - Kira 12.Feb.2008
 */

#define ITEM2_COLDPROOF     (A)
#define ITEM2_ACIDPROOF     (2)
#define ITEM2_SHOCKPROOF    (C)
#define ITEM2_DISPLAY	    (D)
#define ITEM2_NOGIVE        (E)
#define ITEM2_UNSEEN        (F)

/* Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE          (A)
#define ITEM_WEAR_FINGER   (2)
#define ITEM_WEAR_NECK     (C)
#define ITEM_WEAR_BODY     (D)
#define ITEM_WEAR_HEAD     (E)
#define ITEM_WEAR_LEGS     (F)
#define ITEM_WEAR_FEET     (G)
#define ITEM_WEAR_HANDS    (H)
#define ITEM_WEAR_ARMS     (I)
#define ITEM_WEAR_SHIELD   (J)
#define ITEM_WEAR_TORSO    (K)
#define ITEM_WEAR_WAIST    (2048)
#define ITEM_WEAR_WRIST    (M)
#define ITEM_WIELD         (N)
#define ITEM_HOLD          (O)
#define ITEM_NO_SAC        (P)
#define ITEM_WEAR_FLOAT    (Q)
#define ITEM_WEAR_BACK     (R)
#define ITEM_WEAR_EAR      (S)
#define ITEM_WEAR_EYES     (T)
#define ITEM_WEAR_PIN      (U)
#define ITEM_WEAR_FLOAT_AROUND (V)
#define ITEM_WEAR_FLOAT_ABOVE   (W)
#define ITEM_RELIGION      (X)
#define ITEM_WEAR_CLAN     (Y)
#define ITEM_WEAR_BELT     (Z)
#define ITEM_WEAR_SLEEPING (aa)
#define ITEM_WEAR_UNIQUE   (bb)
#define ITEM_WEAR_TATTOO   (cc)

/* weapon types */
#define WEAPON_FLAMING    (A)
#define WEAPON_FROST      (2)
#define WEAPON_VAMPIRIC   (C)
#define WEAPON_SHARP      (D)
#define WEAPON_VORPAL     (E)
#define WEAPON_TWO_HANDS  (F)
#define WEAPON_SHOCKING   (G)
#define WEAPON_POISON     (H)
#define WEAPON_CURSE      (I)
#define WEAPON_OFFHAND    (J)
#define WEAPON_MANA_DRAIN (K)
#define WEAPON_MENTAL     (2048)
#define WEAPON_SILVER     (M)

/* object types */
#define OBJECT_FLAMING    (A)
#define OBJECT_FROST      (2)
#define OBJECT_VAMPIRIC   (C)
#define OBJECT_SHARP      (D)
#define OBJECT_VORPAL     (E)
#define OBJECT_TWO_HANDS  (F)
#define OBJECT_SHOCKING   (G)
#define OBJECT_POISON     (H)

/* gate flags */
#define GATE_NORMAL_EXIT  (A)
#define GATE_NOCURSE      (2)
#define GATE_GOWITH       (C)
#define GATE_BUGGY        (D)
#define GATE_RANDOM       (E)

/* furniture flags */
#define STAND_AT    (A)
#define STAND_ON    (2)
#define STAND_IN    (C)
#define SIT_AT      (D)
#define SIT_ON      (E)
#define SIT_IN      (F)
#define REST_AT     (G)
#define REST_ON     (H)
#define REST_IN     (I)
#define SLEEP_AT    (J)
#define SLEEP_ON    (K)
#define SLEEP_IN    (2048)
#define PUT_AT      (M)
#define PUT_ON      (N)
#define PUT_IN      (O)
#define PUT_INSIDE  (P)

/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE            0
#define APPLY_STR             1
#define APPLY_DEX             2
#define APPLY_INT             3
#define APPLY_WIS             4
#define APPLY_CON             5
#define APPLY_SEX             6
#define APPLY_CLASS           7
#define APPLY_LEVEL           8
#define APPLY_AGE             9
#define APPLY_HEIGHT         10
#define APPLY_WEIGHT         11
#define APPLY_MANA           12
#define APPLY_HIT            13
#define APPLY_QP_MULTIPLIER  14
#define APPLY_GOLD           15
#define APPLY_EXP            16
#define APPLY_AC             17
#define APPLY_HITROLL        18
#define APPLY_DAMROLL        19
#define APPLY_SAVES          20
#define APPLY_SAVING_PARA    21
#define APPLY_SAVING_ROD     22
#define APPLY_SAVING_PETRI   23
#define APPLY_SAVING_BREATH  24
#define APPLY_SAVING_SPELL   25
#define APPLY_SPELL_AFFECT   26
#define APPLY_LUCK           27
#define APPLY_RES_POISON     28
#define APPLY_SANCTUARY	     29
#define APPLY_RES_COLD	     30
#define APPLY_HEAL           31
#define APPLY_MANA_HEAL      32
#define APPLY_RES_NEGATIVE   33
#define APPLY_RES_BASH       34
#define APPLY_RES_CHARM      35
#define APPLY_RES_BLINDNESS  36
#define APPLY_RES_DIRTKICK   37
#define APPLY_RES_ELEMENTS   38
#define APPLY_RES_WEAKEN     39
#define APPLY_RES_TRIP       40
#define APPLY_RES_KICK       41
#define APPLY_CARRY_CAPACITY 42
#define APPLY_RES_DISARM     43
#define APPLY_RES_STEAL      44
#define APPLY_RES_BACKSTAB   45
#define APPLY_RES_HOLDSPELLS 46
#define APPLY_RES_HOLY       47
#define APPLY_MANA_MULTIPLIER 48
#define APPLY_RES_FIRE       49
#define APPLY_SNEAK          50
#define APPLY_HIDE           51
#define APPLY_GOLD_MULTIPLIER 52
#define APPLY_SPELL_DURATION 53
#define APPLY_RES_WEAPON     54
#define APPLY_DAM_MULT       55
#define APPLY_MANA_REDUCTION 56
#define APPLY_NAC            57

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE   1
#define CONT_PICKPROOF   2
#define CONT_CLOSED      4
#define CONT_LOCKED      8
#define CONT_PUT_ON     16

/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_RECALL          20143
#define ROOM_VNUM_CLAN_HEAD       20260
#define ROOM_VNUM_CRYSTALS        20230
#define ROOM_VNUM_BUFFER          20229
#define ROOM_VNUM_LIMBO           2
#define ROOM_VNUM_IMM_CLAN        1200
#define ROOM_VNUM_SCATTER         1299
#define ROOM_VNUM_NORTH_OF_START_ROOM 19452
#define ROOM_VNUM_LOAD            13814

#define ROOM_VNUM_TEMPLE          20143
#define ROOM_VNUM_HOSPITAL	  20263
#define ROOM_VNUM_FAMILY_REGISTER 20250
#define ROOM_VNUM_SCHOOL          3700
#define ROOM_VNUM_CORNER          3
#define ROOM_VNUM_DONATION_PIT    20114

#define ROOM_VNUM_LOSERS          550  // Arcadia Arena Losers
#define ROOM_VNUM_WINNERS         549  // Arcadia Arena Winners Room
#define ROOM_VNUM_WARMORGUE       20251   // Arcadia War Morgue

#define ROOM_VNUM_FTAG_PREP_ROOM  9800

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK           (A)
#define ROOM_NO_MOB         (2)
#define ROOM_INDOORS        (C)
#define ROOM_PRIVATE        (D)
#define ROOM_SAFE           (E)
#define ROOM_SOLITARY       (F)
#define ROOM_PET_SHOP       (G)
#define ROOM_NO_RECALL      (H)
#define ROOM_IMP_ONLY       (I)
#define ROOM_GODS_ONLY      (J)
#define ROOM_HEROES_ONLY    (K)
#define ROOM_NEWBIES_ONLY   (2048)
#define ROOM_LAW            (M)
#define ROOM_NOWHERE        (N)
#define ROOM_NO_SUMMON      (O)
#define ROOM_COMBAT         (P)
#define ROOM_AREA_LOCK      (Q)
#define ROOM_MANOR          (R)
#define ROOM_QUICKSAND      (S)
#define ROOM_UNDERWATER     (T)
#define ROOM_SHALLOW_FISHING (U)
#define ROOM_ARENA          (V)
#define ROOM_COLD           (W)
#define ROOM_NOMAGIC        (X)
#define ROOM_MANOR_MAIN     (Y)
#define ROOM_NO_SCAN	    (Z)
#define ROOM_NO_EXITS	    (aa)
#define ROOM_LOCKER	    (bb)
#define ROOM_BANK	    (cc)
#define ROOM_DEEP_FISHING   (dd)
#define ROOM_FIDO_EXPRESS   (ee)

/*
 * Alt Room Flags
 */

#define ALT_ROOM_NOMAP      (A)
#define ALT_ROOM_ARCHER	    (2)
#define ALT_ROOM_NOFLOOR    (C) //Player can fall thru downward exits
#define ALT_ROOM_NO_SCAN_IN (D)
#define ALT_ROOM_NO_ROOM_MAP (E)
#define ALT_ROOM_DAMAGE	    (F)
#define ALT_ROOM_NO_QUIT    (G)
#define ALT_ROOM_SLIPPERY   (H)
#define ALT_ROOM_CHAPEL	    (I)
#define ALT_ROOM_MINING     (J)
#define ALT_ROOM_ART        (K)
#define ALT_ROOM_NOFLOOR_SOUTH (2048)  //Player can fall thru south exits
#define ALT_ROOM_RAIDER     (M)
#define ALT_ROOM_QUIET      (N)
#define ALT_ROOM_VOTING_BOOTH (O)
#define ALT_ROOM_MAGE       (P) //Mage only rooms
#define ALT_ROOM_WARRIOR    (Q) //Warrior only rooms
#define ALT_ROOM_THIEF      (R) //Thief only rooms
#define ALT_ROOM_CLERIC     (S) //Cleric only rooms
#define ALT_ROOM_GAIAN      (T) //Gaian only rooms
#define ALT_ROOM_PSIONICIST   (U) //Psionicist only rooms
#define ALT_ROOM_OTHER      (V) //If room has other customized exit
#define ALT_ROOM_NOENTRY    (W) //Players cannot enter this room
#define ALT_ROOM_NOGATE     (X)
#define ALT_ROOM_EVIL_ONLY  (Y)
#define ALT_ROOM_GOOD_ONLY  (Z)
#define ALT_ROOM_NEUTRAL_ONLY (aa)

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH  0
#define DIR_EAST   1
#define DIR_SOUTH  2
#define DIR_WEST   3
#define DIR_UP     4
#define DIR_DOWN   5

/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR       (A)
#define EX_CLOSED       (2)
#define EX_LOCKED       (C)
#define EX_PICKPROOF    (D)
#define EX_NOPASS       (E)
#define EX_EASY         (F)
#define EX_HARD         (G)
#define EX_INFURIATING  (H)
#define EX_NOCLOSE      (I)
#define EX_NOLOCK       (J)
#define EX_DIG		(K)
#define EX_UNDIG	(2048)
#define EX_ONEWAY	(M)

/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE         0
#define SECT_CITY           1
#define SECT_FIELD          2
#define SECT_FOREST         3
#define SECT_HILLS          4
#define SECT_MOUNTAIN       5
#define SECT_WATER_SWIM     6
#define SECT_WATER_NOSWIM   7
#define SECT_UNUSED         8
#define SECT_AIR            9
#define SECT_DESERT        10
#define SECT_RUBBLE        11
#define SECT_MAX           12

/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE     -1
#define WEAR_LIGHT     0
#define WEAR_HEAD      1
#define WEAR_EYES      2
#define WEAR_EAR_L     3
#define WEAR_EAR_R     4
#define WEAR_NECK_1    5
#define WEAR_NECK_2    6
#define WEAR_PIN_1     7
#define WEAR_PIN_2     8
#define WEAR_BACK      9
#define WEAR_TORSO     10
#define WEAR_BODY      11
#define WEAR_HANDS     12
#define WEAR_ARMS      13
#define WEAR_WRIST_L   14
#define WEAR_WRIST_R   15
#define WEAR_FINGER_L  16
#define WEAR_FINGER_R  17
#define WEAR_LEGS      18
#define WEAR_FEET      19
#define WEAR_WAIST     20
#define WEAR_BELT      21
#define WEAR_SHIELD    22
#define WEAR_WIELD     23
#define WEAR_SECONDARY 24
#define WEAR_HOLD      25
#define WEAR_RELIGION  26
#define WEAR_FLOAT     27
#define WEAR_FLOAT_ABOVE 28
#define WEAR_FLOAT_AROUND 29
#define WEAR_CLAN      30
#define WEAR_SLEEPING  31
#define WEAR_UNIQUE    32
#define WEAR_TATTOO    33
#define MAX_WEAR       34

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK   0

/*
 * Positions.
 */
#define POS_DEAD      0
#define POS_MORTAL    1
#define POS_INCAP     2
#define POS_STUNNED   3
#define POS_SLEEPING  4
#define POS_SITTING   5
#define POS_RESTING   6
#define POS_FIGHTING  7
#define POS_STANDING  8

/* Freezetag! */
#define TAG_PLAYING                     (A)
#define TAG_FROZEN                      (2)
#define TAG_RED                         (C)
#define TAG_BLUE                        (D)

/*
 * ACT bits for players.
 */
#define PLR_IS_NPC        (A)   /* Don't EVER set.    */
#define PLR_AUTOASSIST    (2)
#define PLR_AUTOEXIT      (C)
#define PLR_AUTOLOOT      (D)
#define PLR_AUTOSAC       (E)
#define PLR_AUTOGOLD      (F)
#define PLR_AUTOSPLIT     (G)
#define PLR_HOLYLIGHT     (H)
#define PLR_CANLOOT       (I)
#define PLR_NOSUMMON      (J)
#define PLR_NOFOLLOW      (K)
#define PLR_PERMIT        (M)
#define PLR_LOG           (N)
#define PLR_DENY          (O)
#define PLR_FREEZE        (P)
#define PLR_THIEF         (Q)
#define PLR_MANA_REDUC    (R)
#define PLR_NOEXP         (S)
#define PLR_QUESTOR       (T)
#define PLR_NOTITLE       (U)
#define PLR_NOTEWRITE     (V)
#define PLR_NOCANCEL	  (W)
#define PLR_WAR		  (X)
#define PLR_MIRROR        (Y)
#define PLR_NOCOMBAT      (Z)
#define PLR_PKAGRO        (aa)
#define PLR_AUTODIG       (bb)
#define PLR_NOCHALLENGE   (cc)
#define PLR_AUTOMAP	  (dd)
#define PLR_REVENGE       (ee)

/* plr2 flags */
#define PLR2_MARRIED         (A)
#define PLR2_LINKDEAD        (2)
#define PLR2_ARENA           (C)
#define PLR2_BET             (D)
#define PLR2_AUTOPEEK        (E)
#define PLR2_AUTOSTATS       (F)
#define PLR2_HAS_HOME        (G)
#define PLR2_HIGH_PRIEST     (H)
#define PLR2_PROMPT_PERCENT  (I)
#define PLR2_NOWALKIN        (J)
#define PLR2_ENEMY_PROMPT_DOTS (K)  // npc prompts
#define PLR2_AUTOWAKE        (2048)
#define PLR2_NOTRAN	     (M)
#define PLR2_UNTANGLED       (N)
#define PLR2_NOTAKE   	     (O)
#define PLR2_CODING          (P)
#define PLR2_AUTOSCAN	     (Q)
#define PLR2_NOIDLE	     (R)
#define PLR2_NOICTITLE	     (S)
#define PLR2_IN_GQ           (T)
#define PLR2_AUTOVNUM        (U)
#define PLR2_NOVITALITY      (V)
#define PLR2_NOBATTLECOLOR   (W)
#define PLR2_AUTOPETATTACK   (X)
#define PLR2_DONOR           (Y)
#define PLR2_AUTOMULTS       (Z)
#define PLR2_PK_PENDING      (aa)
#define PLR2_REMOVE_ME       (bb)
#define PLR2_REMOVE_ME2      (cc)

/* RT comm flags -- may be used on both mobs and chars */

/* Yay enums! Auto adjust numbers as flags are added and removed -Maxxus */
enum {
   COMM_QUIET, COMM_DEAF, COMM_NOWIZ, COMM_NOGQ, COMM_NOQA, COMM_NOMUSIC, COMM_NOCLAN, COMM_NOQUOTE, COMM_COMPACT, COMM_AUTOLOOK, COMM_PROMPT, COMM_COMBINE, COMM_SHOW_AFFECTS, COMM_NOGRATS, COMM_NOEMOTE, COMM_NOTELL, COMM_NOCHANNELS, COMM_SNOOP_PROOF, COMM_AFK, COMM_GOSSIPOFF, COMM_NOBOARD, 
COMM_TITLELOCK, COMM_STATUS, COMM_NEWPLAYER, COMM_NOQCHAT, COMM_LTALK, COMM_AUTOOPEN, COMM_NOADVICE, COMM_FLAMEOFF, COMM_CATCH, COMM_HELPER, COMM_NEWBIEOFF, COMM_LONG, COMM_NOTEON, COMM_TICKON, COMM_FTALK, COMM_FAMILYTALK, COMM_AUTODESC, COMM_HIDEDESC, COMM_NOGTELL, COMM_NOFIDO, COMM_RPOFF, COMM_ALLOWBEEP, COMM_IAW, COMM_HELPERCHAN, COMM_MOBDEATHS, COMM_WEATHER, COMM_ZONE, COMM_DEATHCRY, COMM_RSOCIAL, COMM_LEVELS, COMM_NOOBJLEVEL, COMM_MINIQUEST, COMM_NOROOMTALK, COMM_BUSY, COMM_SPOUSEOFF, COMM_QUEST, COMM_ANTISOCIAL, COMM_NOAUCTION, COMM_NOFTAG, COMM_NOWORSHIPPER, COMM_BRIEF, COMM_NOTELLS
   , COMM_AFKLOCK, COMM_ENEMY_PROMPT_PERCENT, COMM_TICK_PROMPT, COMM_NOLTALK, COMM_FAUX_LEVELS, COMM_PKILLS, COMM_GCLAN, COMM_NOGCLAN,
      COMM_MAX
};

/* WIZnet flags */
#define WIZ_ON                  (A)
#define WIZ_TICKS               (2)
#define WIZ_LOGINS              (C)
#define WIZ_SITES               (D)
#define WIZ_LINKS               (E)
#define WIZ_DEATHS              (F)
#define WIZ_RESETS              (G)
#define WIZ_MOBDEATHS           (H)
#define WIZ_FLAGS               (I)
#define WIZ_PENALTIES           (J)
#define WIZ_SACCING             (K)
#define WIZ_LEVELS              (2048)
#define WIZ_SECURE              (M)
#define WIZ_SWITCHES            (N)
#define WIZ_SNOOPS              (O)
#define WIZ_RESTORE             (P)
#define WIZ_LOAD                (Q)
#define WIZ_NEWBIE              (R)
#define WIZ_PREFIX              (S)
#define WIZ_SPAM                (T)
#define WIZ_HELPS               (U)
#define WIZ_SCATTERS            (V)
#define WIZ_MULTIPLAY           (W)
#define WIZ_GROUPS              (X)
#define WIZ_ANUKE               (Y)
#define WIZ_TRANSFER		(Z)
#define WIZ_SLAYS		(aa)
#define WIZ_VIOLSAFE		(bb)
#define WIZ_AUTOSAVE		(cc)
#define WIZ_CHEATING		(dd)
#define WIZ_OLC			(ee)

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct mob_index_data {
   MOB_INDEX_DATA *next;
   SPEC_FUN *spec_fun;
   SHOP_DATA *pShop;
   PROG_LIST *mprogs;
   AREA_DATA *area;             /* OLC */
   sh_int vnum;
   sh_int group;
   bool new_format;
   sh_int count;
   sh_int killed;
   char *player_name;
   char *short_descr;
   char *long_descr;
   char *description;
   long act;
   long affected_by;
   long affected2_by;
   sh_int alignment;
   sh_int level;
   sh_int hitroll;
   long hit[3];
//   sh_int mana[3];
   sh_int damage[3];
   sh_int ac;
   sh_int dam_type;
   long off_flags;
   long imm_flags;
   long res_flags;
   long vuln_flags;
   sh_int start_pos;
   sh_int default_pos;
   sh_int sex;
   sh_int race;
   long wealth;
   long form;
   long parts;
   sh_int size;
   char *material;
   long mprog_flags;
   int32_t act3_flags;
   char *story;
};

/* memory settings */
#define MEM_CUSTOMER  A
#define MEM_SELLER    B
#define MEM_HOSTILE   C
#define MEM_AFRAID    D

/* memory for mobs */
struct mem_data {
   MEM_DATA *next;
   bool valid;
   int id;
   int reaction;
   time_t when;
};

/* For Erwin's Noteboard
 * -- JR 09/25/00
 */

#include "board.h"

/*
 * One character (PC or NPC).
 */
struct char_data                // struct char_data
{
   CHAR_DATA *next;
   CHAR_DATA *next_in_room;
   CHAR_DATA *master;
   CHAR_DATA *leader;
   CHAR_DATA *fighting;
   CHAR_DATA *reply;
   CHAR_DATA *pet;
   CHAR_DATA *mprog_target;
   CHAR_DATA *questgiver;
   CHAR_DATA *last_fought;
   CHAR_DATA *target;
   int questvnum;
   int ac;
   sh_int nextbash;             // Kira
   sh_int nextquest;
   sh_int countdown;
   sh_int questobj;
   sh_int questmob;
   sh_int traptimer;
   ROOM_INDEX_DATA *questroom;  // MachinShin
   sh_int stunned;
   sh_int silenced;
   sh_int headache;             //Neural Overload - Kira
   MEM_DATA *memory;
   SPEC_FUN *spec_fun;
   MOB_INDEX_DATA *pIndexData;
   DESCRIPTOR_DATA *desc;
   AFFECT_DATA *affected;
   OBJ_DATA *carrying;
   OBJ_DATA *on;
   ROOM_INDEX_DATA *in_room;
   ROOM_INDEX_DATA *was_in_room;
   AREA_DATA *zone;
   PC_DATA *pcdata;
   bool valid;
   CHAR_DATA *hunting;          /* hunt data */
   char *name;
   sh_int aspect;
   char *devout;                // Kira -- pointer for favoured IMM
   long id;
   sh_int version;
   char *short_descr;
   char *long_descr;
   char *description;
   char *in_mind;
   char *prompt;
   bool show_prompt;
   char *prefix;
   char *married_to;
   char *island;
   char *continent;
   sh_int group;
   sh_int clan;
   sh_int rank;                 /* Rendelven - Clan System */
   sh_int sex;
   sh_int class;
   sh_int num_classes;          /* Number of classes character has. LOR */
   CHAR_CLASS *head_class;      /* Head of linked list of classes. LOR */
   sh_int race;
   sh_int level;
   int played;
   int gpoint;
   int subtype;
   int lines;                   /* for the pager */
   int tells;
   time_t logon;
   sh_int timer;
   sh_int wait;
   sh_int daze;
   int64_t hit;                 // int32_t
   int64_t max_hit;
   int64_t mana;
   int64_t max_mana;
   int mudpie;
   int gwins;
   long gold;
   long platinum;
   long diamond;
   long black_diamond;
   long tridiamond;
   long pot;
   long gambled;
   int64_t exp;
   long act;
   int act2;
   char comm[BITNSLOTS(COMM_MAX)];
   int plr;
   int violsafe;                /*violence and safe timers */
   int revenge;                 /*revenge timer */
   int wkills;                  /* pc's kills in current war            */
   int wkills_total;            /* pc's total kills in all wars  */
   int wdeaths;                 /* pc's deaths in current war           */
   int wdeaths_total;           /* pc's total deaths in all wars        */
   int pkills;
   int pdeaths;
   int arena_kills;
   int arena_deaths;
   long wiznet;                 /* wiz stuff */
   long imm_flags;
   long res_flags;
   long vuln_flags;
   sh_int invis_level;
   sh_int incog_level;
   sh_int ghost_level;
   long affected_by;
   long affected2_by;
   sh_int position;
   long practice;
   sh_int train;
   sh_int carry_weight;
   sh_int carry_number;
   sh_int saving_throw;
   sh_int alignment;
   int hitroll;
   int damroll;
   sh_int armor;
   int narmor; /* natural armor */
   sh_int wimpy;
   sh_int clead;
   /* stats */
   sh_int perm_stat[MAX_STATS];
   sh_int mod_stat[MAX_STATS];
   /* parts stuff */
   time_t llogoff;
   long form;
   long parts;
   sh_int size;
   char *material;
   /* mobile stuff */
   long off_flags;
   sh_int damage[3];
   sh_int dam_type;
   sh_int start_pos;
   sh_int default_pos;
   sh_int mprog_delay;
   /* tier */
   sh_int tier;
   long faux_total;
   long free_faux;
   /* freezetag */
   long tag_flags;
   int32_t act3_flags;
   int16_t num_events;
   int16_t num_mprog_events;
   int16_t mpsleep_timer;

};

/*
 * Data which only PC's have.
 */
struct pc_data                  // struct pc_data //pcdata
{
   PC_DATA *next;
   BUFFER *buffer;
   COLOUR_DATA *code;           /* Data for color configuration    */
   bool valid;
   char thistory[MAX_CHANNEL_HISTORY][MSL];
   char gossiphistory[MAX_CHANNEL_HISTORY][MSL];
   sh_int final_stat[MAX_STATS];
   char *bamfin;
   char *bamfout;
   char *wiziname;
   char *storedwiziname;
   int wizinametoggle;
   int buildpoints;
   int turnins;
   sh_int scountdown;
   int bptotal;
   time_t test;
   sh_int guess_wait;
   float cooldown[MAX_SKILL];
   char *title;
   time_t logontime;
   char *birthday;
   char *created;
   char *alts;
   char *pretitle;
   bool on_quest;
   sh_int group_pending;
   int status;
   int pstatus;
   int relation;
   sh_int raider;               //Kira
   int vitality;
   float limitbreak;
   int max_vitality;
   int war;
   int qcomplete;
   long lost_exp;               // Kira - resurrection stuff
   long lost_gold;              // Kira - resurrection stuff
   int gq_kills;
   sh_int chaltimer;
   int safe_room_timer;
   char *surname;
   char *pending_surname;
   char *mstatus;
   char *warcry;
   char *bet;
   char *who_descr;
   char *friend[MAX_FRIEND];
   char *view[MAX_VIEW];
   char *consent[MAX_CONSENT];
   char *ignore[MAX_IGNORE];
   char *afk;
   char *arena_target;
   char *walkin;
   char *walkout;
   char *worldin;
   char *gate1;
   char *gate2;
   char *spellup1;
   char *spellup2;
   char *spellup3;
   char *remember;
   char *irestore;
   EXPLORE_DATA *explored;
   int curr_level;
   time_t release_date;         /* Auto-helling.. Altrag */
   char *helled_by;
   int32_t perm_hit;
   int32_t perm_mana;
   int32_t perm_move;
   sh_int true_sex;
   sh_int ghost;                // Only 5 minutes long - Kira - 10/May/2008
   int word_room;               // memorize spell for word of recall
   int last_level;
   int bounty;
   int gset;
   int gohome;
   int gowork;
   char *intro;
   sh_int condition[4];
   sh_int learned[MAX_SKILL];
   long gold_bank;
   long platinum_bank;
   long diamond_bank;
   long black_diamond_bank;
   long tridiamond_bank;
   bool confirm_delete;
   char *alias[MAX_ALIAS];
   char *alias_sub[MAX_ALIAS];
   int kills_mob;
   int kills_pc;
   int deaths_mob;
   int deaths_pc;
   char *email;                 // Hitch 29/10/05
   bool email_private;          // Hitch 29/10/05
   char *from;                  // Kira 6 Feb 2006
   char *www;                   // Hitch 29/10/05
   bool www_private;            // Hitch 29/10/05
   char *battlecry;             // Hitch 19/11/05
   char *victory;               // Kira 4 Mar 2009
   char *recallmsg;             // Kira 29 June 2011
   char *limit_break_mesg;      // Kira
   char *claninvite;            // Kira 24 June 2009
   char *lasthost;              // Hitch 27/11/05
   char *lastip;                // Hitch 27/11/05
   char *ictitle;
   char *auras;
   char *recall;

   /* Daemon data */
   char *daemon_name;
   char *daemon_short_descr;
   char *daemon_long_descr;
   char *daemon_descr;

   int gqwins;
   int questpoints;
   int expeditions_complete;

   BOARD_DATA *board;           /* The current board        */
   time_t last_note[MAX_BOARD]; /* last note for the boards */
   PACKRAT_DATA *packrat_list;
   int packrat_count;           //overall items packed
   sh_int packrat_count2;       //counts to 5 and starts over - for qps
   NOTE_DATA *in_progress;
   GRANT_DATA *granted;
   int security;                /* OLC *//* Builder security */
   int text[3];                 /* {t */
   int auction[3];              /* {a */
   int auction_text[3];         /* {A */
   int music[3];                /* {e */
   int music_text[3];           /* {E */
   int question[3];             /* {q */
   int question_text[3];        /* {Q */
   int answer[3];               /* {f */
   int answer_text[3];          /* {F */
   int quote[3];                /* {h */
   int quote_text[3];           /* {H */
   int immtalk_text[3];         /* {i */
   int immtalk_type[3];         /* {I */
   int info[3];                 /* {j */
   int say[3];                  /* {6 */
   int say_text[3];             /* {7 */
   int tell[3];                 /* {k */
   int tell_text[3];            /* {K */
   int reply[3];                /* {l */
   int reply_text[3];           /* {L */
   int gtell_text[3];           /* {n */
   int gtell_type[3];           /* {N */
   int wiznet[3];               /* {B */
   int room_title[3];           /* {s */
   int room_text[3];            /* {S */
   int room_exits[3];           /* {o */
   int room_things[3];          /* {O */
   int prompt[3];               /* {p */
   int fight_death[3];          /* {1 */
   int fight_yhit[3];           /* {2 */
   int fight_ohit[3];           /* {3 */
   int fight_thit[3];           /* {4 */
   int fight_skill[3];          /* {5 */
   MPROG_TAG *tag_list;         // Mobprog tags
   EXPEDITION_DATA *expedition;     // player expedition
   JUNKYARD_DATA *junkyard;     // player junkyard quest
   int8_t next_junkyard;        // time until next junkyard quest
   int8_t next_expedition;        // time until next expedition
   int8_t gq_timer;             // time until next joining of global quest
   bool char_is_frozen;

};

#define CHAR_IS_FROZEN(ch)      ( !IS_NPC(ch) && ch->pcdata->char_is_frozen )

#define CAN_ACCESS(room)        ( !IS_SET(room->room_flags, ROOM_PRIVATE) \
                                &&   !IS_SET(room->room_flags, ROOM_SOLITARY) \
                                &&   !IS_SET(room->room_flags, ROOM_GODS_ONLY) \
                                &&   !IS_SET(room->room_flags, ROOM_IMP_ONLY)  \
                                &&   !IS_SET( room->area->area_flags, AREA_NOAREA ))

struct clan_data {
   char *name;                  /* name of clan */
   char *leader[5];             /* Leaders of the clan */
   char *who_name;              /* the who name for the clan */
   char *whois_name;            /* The 'whois' name for clan. Long title */
   char *deity;                 /* 'deity' of the clan */
   char *webpage;               /* webpage for the clan */
   char *clan_type;
   char *continent;
   bool independent;            /* Not a true clan "loner" clan */
   RANK_DATA ranks[2][10];      /* our table of ranks, should make MAX_RANK */
   ROSTER_DATA *roster;
   int room[5];                 /* clan rooms, recall, donation, morgue, jail, death */
   sh_int max_members;          /* The max amount of members allowed in ; 0 for unlimited */
   sh_int current_members;      /* how many members are in the clan */
   sh_int security[4];          /* This is for promote/demote/induct/outcast. sets rank levels for access */
   sh_int min_level;            /* the minimum level you must be to join this clan */
   sh_int max_level;            /* the maximum level you can be to -join- this clan */
   long bank;                   /* Tax rate for clan */
   bool valid;
   char history[MAX_CHANNEL_HISTORY][MSL];
};

//Create groups - Kira
struct group_data {
   char *name;                  /* Name of group */
   CHAR_DATA *leader;            /* Leader of group */
   bool private;                /* Private or Public group */
   char* created;               /* When was the group created */
   int kills;                   /* Mobs killed */
   long exp;                    /* EXP earned by group */
   char history[MAX_CHANNEL_HISTORY][MSL];       /* Keep a channel history of gtell */
};


/*
 * Aspect table god fields.
 */

struct god_type {
   char *name;                  // call name of the god/aspect
};

/*
 * Devout table god fields.
 */

struct devout_type {
   char *name;                  // call name of the god/devout
};

/*
 * Liquids.
 */
#define LIQ_WATER        0

struct liq_type {
   char *liq_name;
   char *liq_color;
   sh_int liq_affect[5];
};

struct material_type {
   char *material_name;
};

/*
 * Extra description data for a room or object.
 */
struct extra_descr_data {
   EXTRA_DESCR_DATA *next;      /* Next in list            */
   bool valid;
   char *keyword;               /* Keyword in look/examine */
   char *description;           /* What to see             */
};

/*
 * Prototype for an object.
 */
struct obj_index_data {
   OBJ_INDEX_DATA *next;
   EXTRA_DESCR_DATA *extra_descr;
   AFFECT_DATA *affected;
   AREA_DATA *area;             /* OLC */
   PROG_LIST *oprogs;
   long oprog_flags;
   bool new_format;
   char *name;
   char *short_descr;
   char *description;
   sh_int class;
   int clan;
   int trap_dam;
   sh_int vnum;
   sh_int reset_num;
   char *material;
   char *owner;
   sh_int item_type;
   int extra_flags;
   int extra2_flags;
   int affects;
   int wear_flags;
   sh_int level;
   sh_int condition;
   sh_int count;
   sh_int weight;
   int cost;
   long timer_olc;              //Kira's Addition 24 Oct 2008
   int uses;
   int teleport;
   int value[6];
   sh_int limit;
   int8_t num_rune_slots;
   int8_t rune_chance_blowup;
};

/*
 * One object.
 */
struct obj_data {
   OBJ_DATA *next;
   OBJ_DATA *next_content;
   OBJ_DATA *contains;
   OBJ_DATA *in_obj;
   OBJ_DATA *on;
   CHAR_DATA *carried_by;
   EXTRA_DESCR_DATA *extra_descr;
   AFFECT_DATA *affected;
   OBJ_INDEX_DATA *pIndexData;
   ROOM_INDEX_DATA *in_room;
   CHAR_DATA *oprog_target;
   sh_int oprog_delay;
   bool valid;
   bool enchanted;
   char *owner;
   char *owner2;
   char *name;
   char *short_descr;
   char *description;
   int butchered;
   int trap_dam;
   int item_type;
   int extra_flags;
   int extra2_flags;
   int affects;
   int wear_flags;
   sh_int wear_loc;
   sh_int weight;
   int cost;
   int teleport;
   sh_int level;
   sh_int condition;
   char *material;
   sh_int class;
   long timer_olc;              //Kira
   int uses;
   sh_int timer;
   int value[6];
   int clan;
   int num_events;
   KEY_DATA *keys;
   time_t lastquad;             //Kira
   int seconds;                 //Kira
   int8_t num_rune_slots;
   RUNE_SLOT_DATA *rune_slots;
   int8_t rune_chance_blowup;
};

/*
 * Exit data.
 */
struct exit_data {
   union {
      ROOM_INDEX_DATA *to_room;
      sh_int vnum;
   } u1;
   sh_int exit_info;
   sh_int key;
   char *keyword;               // what player types when opening door
   char *name;                  //what player sees when opening door
   char *description;
   EXIT_DATA *next;             /* OLC */
   int rs_flags;                /* OLC */
   int orig_door;               /* OLC */
   bool color;

};

/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct reset_data {
   RESET_DATA *next;
   char command;
   sh_int arg1;
   sh_int arg2;
   sh_int arg3;
   sh_int arg4;
   sh_int arg5;
   sh_int arg6;
};

/*
 * Area definition.
 */
struct area_data {
   AREA_DATA *next;
   HELP_AREA *helps;
   char *file_name;
   char *name;
   char *credits;
   sh_int age;
   sh_int nplayer;
   sh_int low_range;
   sh_int high_range;
   sh_int min_vnum;
   sh_int max_vnum;
   sh_int min_level;
   sh_int max_level;
   sh_int lock;
   sh_int timer;                /* OLC Addition -- Area mission timers */
   bool empty;
   char *builders;              /* OLC *//* Listing of */
   int vnum;                    /* OLC *//* Area vnum  */
   int area_flags;              /* OLC */
   int security;                /* OLC *//* Value 1-9  */
   char *clan;                  // OLC addition - Kira
   char *continent;             // OLC addition - Kira
   char *clan_owner;            //Kira
   int control_points;          //Kira
   sh_int control_state;        //Kira
   long control_timer;          //Kira
   char *spawn;                 // OLC Addition
   char *align;

   /*Area template data - each variable begins with a 't' for 'template' */
   char *ttheme;
   char *tlayout;
   char *tnotes;
   char *tideas;
   char *tstatus;

   /*Rubric data */
   sh_int gstyle_given;
   sh_int gstyle_possible;
   sh_int gcreativity_given;
   sh_int gcreativity_possible;
   sh_int gtechnical_given;
   sh_int gtechnical_possible;
   sh_int gextra_given;
   sh_int gflow_given;
   sh_int gflow_possible;
   char *gcomments;

};

struct room_history_data {
   char *name;
   int went;
   ROOM_HISTORY_DATA *next;
   ROOM_HISTORY_DATA *prev;
};

/*
 * Room type.
 */
struct room_index_data {
   sh_int heap_index;
   sh_int steps;
   bool visited;
   ROOM_INDEX_DATA *next;
   CHAR_DATA *people;
   OBJ_DATA *contents;
   EXTRA_DESCR_DATA *extra_descr;
   AREA_DATA *area;
   EXIT_DATA *exit[6];
   RESET_DATA *reset_first;     /* OLC */
   RESET_DATA *reset_last;      /* OLC */
   PROG_LIST *rprogs;
   CHAR_DATA *rprog_target;
   ROOM_HISTORY_DATA *history;
   long rprog_flags;
   sh_int rprog_delay;
   char *name;
   char *description;
   char *owner;
   char *visitors;
   sh_int vnum;
   int room_flags;
   int32_t alt_room_flags;
   sh_int light;
   sh_int sector_type;
   sh_int heal_rate;
   sh_int climbup_vnum;
   sh_int climbdown_vnum;
   sh_int max_ores;
   sh_int mined_ores;
   sh_int mana_rate;
   sh_int clan;
   char *graffiti;
   char *roomcredit;
   char *climbup_mesg;
   char *climbdown_mesg;
   char *climberup_mesg;
   char *climberdown_mesg;
   int num_events;
};

/*
 *  Target types.
 */
#define TAR_IGNORE              0
#define TAR_CHAR_OFFENSIVE      1
#define TAR_CHAR_DEFENSIVE      2
#define TAR_CHAR_SELF           3
#define TAR_OBJ_INV             4
#define TAR_OBJ_CHAR_DEF        5
#define TAR_OBJ_CHAR_OFF        6
#define TAR_OBJ_TRAN		7

#define TARGET_CHAR             0
#define TARGET_OBJ              1
#define TARGET_ROOM             2
#define TARGET_NONE             3

struct char_class {
   CHAR_CLASS *next;            /* Next node. */
   sh_int class;                /* What class is this one? */

};

/*
 * Skills include spells as a particular case.
 */
struct skill_type {
   char *name;                  /* Name of skill        */
   sh_int skill_level[MAX_CLASS];   /* Level needed by class */
   sh_int rating;               /* How hard it is to learn    */
   SPELL_FUN *spell_fun;        /* Spell pointer (for spells)    */
   DO_FUN *do_fun;
   sh_int target;               /* Legal targets        */
   sh_int minimum_position;     /* Position for caster / user    */
   sh_int *pgsn;                /* Pointer to associated gsn    */
   sh_int slot;                 /* Slot for #OBJECT loading    */
   sh_int beats;                /* Waiting time after use    */
   char *noun_damage;           /* Damage message        */
   char *msg_off;               /* Wear off message        */
   char *msg_obj;               /* Wear off message for obects    */
   sh_int religion;             /* Religious spells, -1 is false, > -1 is true */
   char *race;                  /* Race skills and spells - Kira */
   sh_int buffer_sksp;          /* Is it a buffer skill or spell? */
   double cooldown;             /* Is there a wait time before skill can be used again */
};

/*
 * MOBprog definitions
 */
#define TRIG_ACT     (A)
#define TRIG_BRIBE   (2)
#define TRIG_DEATH   (C)
#define TRIG_ENTRY   (D)
#define TRIG_FIGHT   (E)
#define TRIG_GIVE    (F)
#define TRIG_GREET   (G)
#define TRIG_GRALL   (H)
#define TRIG_KILL    (I)
#define TRIG_HPCNT   (J)
#define TRIG_RANDOM  (K)
#define TRIG_SPEECH  (2048)
#define TRIG_EXIT    (M)
#define TRIG_EXALL   (N)
#define TRIG_DELAY   (O)
#define TRIG_SURR    (P)
#define TRIG_GET     (Q)
#define TRIG_DROP    (R)
#define TRIG_SIT     (S)
#define TRIG_WEAR    (T)
#define TRIG_COMMAND (U)
#define TRIG_LOAD    (V)
#define TRIG_REMOVE  (W)
#define TRIG_LEVELUP (X)

/*
 * Prog types
 */
#define PRG_MPROG	0
#define PRG_OPROG	1
#define PRG_RPROG	2

struct prog_list {
   int trig_type;
   char *trig_phrase;
   sh_int vnum;
   char *code;
   PROG_LIST *next;
   bool valid;
};

struct prog_code {
   sh_int vnum;
   char *code;
   PROG_CODE *next;
};


/*
 * These are skill_lookup return values for common skills and spells.
 */

extern sh_int gsn_awakening;
extern sh_int gsn_immortal_blessing;
extern sh_int gsn_farsight;
extern sh_int gsn_magic_guard;
extern sh_int gsn_tidal_wave;
extern sh_int gsn_stone_hammer;
extern sh_int gsn_pickpocket;
extern sh_int gsn_elemental_aura;
extern sh_int gsn_double_gold;
extern sh_int gsn_holy_symbol;
extern sh_int gsn_double_questpoints;
extern sh_int gsn_target;
extern sh_int gsn_detect_location;
extern sh_int gsn_deter;
extern sh_int gsn_shockwave;
extern sh_int gsn_wrath_wild;
extern sh_int gsn_shadow_walk;
extern sh_int gsn_elemental_aura;
extern sh_int gsn_psychic_aura;
extern sh_int gsn_soulburn;
extern sh_int gsn_starburst;
extern sh_int gsn_showdown;
extern sh_int gsn_create_tree;
extern sh_int gsn_full_health;
extern sh_int gsn_restore_megamana;
extern sh_int gsn_time_leap;
extern sh_int gsn_make_bag;
extern sh_int gsn_create_bed;
extern sh_int gsn_quaddam;
extern sh_int gsn_limit_break;
extern sh_int gsn_tree_of_life;
extern sh_int gsn_lifestream;
extern sh_int gsn_lights_out;
extern sh_int gsn_gust_of_wind;
extern sh_int gsn_sharp_mind;
extern sh_int gsn_mana_reflection;
extern sh_int gsn_holy_shield;
extern sh_int gsn_infinity;
extern sh_int gsn_colossus;
extern sh_int gsn_pummel;
extern sh_int gsn_daemon_roar;
extern sh_int gsn_daemon;
extern sh_int gsn_oblivion;
extern sh_int gsn_hamstring;
extern sh_int gsn_strike;
extern sh_int gsn_fireball;
extern sh_int gsn_demonfire;
extern sh_int gsn_acid_blast;
extern sh_int gsn_frost_breath;
extern sh_int gsn_gas_breath;
extern sh_int gsn_lightning_breath;
extern sh_int gsn_acid_breath;
extern sh_int gsn_fire_breath;
extern sh_int gsn_pull;
extern sh_int gsn_flaming_arrow;
extern sh_int gsn_venomous_arrow;
extern sh_int gsn_nightmare_touch;
extern sh_int gsn_feigned_agony;
extern sh_int gsn_stone_skin;
extern sh_int gsn_flesh_armor;
extern sh_int gsn_bodycheck;
extern sh_int gsn_plague_vomit;
extern sh_int gsn_critical;
extern sh_int gsn_energy_drain;
extern sh_int gsn_psi_bonus;
extern sh_int gsn_mage_bonus;
extern sh_int gsn_thief_bonus;
extern sh_int gsn_counter;
extern sh_int gsn_mana_multiplier;
extern sh_int gsn_cleric_bonus;
extern sh_int gsn_warrior_bonus;
extern sh_int gsn_gaian_bonus;
extern sh_int gsn_holy_water;
extern sh_int gsn_spirit_bolt;
extern sh_int gsn_white_flame;
extern sh_int gsn_psychosis;
extern sh_int gsn_spasm;
extern sh_int gsn_mind_freeze;
extern sh_int gsn_mind_assault;
extern sh_int gsn_ballistic_attack;
extern sh_int gsn_warmth;
extern sh_int gsn_uppercut;
extern sh_int gsn_steel_skin;
extern sh_int gsn_jab;
extern sh_int gsn_psionic_resuscitation;
extern sh_int gsn_converge;
extern sh_int gsn_faerie_fire;
extern sh_int gsn_lucky_shot;
extern sh_int gsn_expert_archery;
extern sh_int gsn_master_archery;
extern sh_int gsn_intermediate_archery;
extern sh_int gsn_novice_archery;
extern sh_int gsn_summon_shadow;
extern sh_int gsn_treant_call;
extern sh_int gsn_war_hounds;
extern sh_int gsn_lasso;
extern sh_int gsn_hunt;
extern sh_int gsn_reflex;
extern sh_int gsn_cover;
extern sh_int gsn_shoot;
extern sh_int gsn_silent_shoot;
extern sh_int gsn_track;
extern sh_int gsn_spellbane;
extern sh_int gsn_fire_arrow;
extern sh_int gsn_poison_arrow;
extern sh_int gsn_neural_overload;
extern sh_int gsn_mental_shield;
extern sh_int gsn_share_wisdom;
extern sh_int gsn_share_strength;
extern sh_int gsn_share_intellect;
extern sh_int gsn_shadow_form;
extern sh_int gsn_inertial_barrier;
extern sh_int gsn_mental_barrier;
extern sh_int gsn_combat_empathy;
extern sh_int gsn_adrenaline_control;
extern sh_int gsn_heighten;
extern sh_int gsn_levitation;
extern sh_int gsn_enhanced_vision;
extern sh_int gsn_jump;
extern sh_int gsn_rapidity;
extern sh_int gsn_lightspeed;
extern sh_int gsn_spunch;
extern sh_int gsn_brilliance;
extern sh_int gsn_feeble_mind;
extern sh_int gsn_infrared;
extern sh_int gsn_haste;
extern sh_int gsn_fire_storm;
extern sh_int gsn_true_sight;
extern sh_int gsn_unbreakable;
extern sh_int gsn_bless;
extern sh_int gsn_chain_lightning;
extern sh_int gsn_lightning_bolt;
extern sh_int gsn_blizzard;
extern sh_int gsn_brambles;
extern sh_int gsn_burning_hands;
extern sh_int gsn_call_lightning;
extern sh_int gsn_cause_critical;
extern sh_int gsn_cause_light;
extern sh_int gsn_cause_serious;
extern sh_int gsn_chill_touch;
extern sh_int gsn_cyclone;
extern sh_int gsn_earthquake;
extern sh_int gsn_fireball;
extern sh_int gsn_flamestrike;
extern sh_int gsn_ice_bolt;
extern sh_int gsn_lighting_bolt;
extern sh_int gsn_magic_missile;
extern sh_int gsn_moon_blade;
extern sh_int gsn_tsunami;
extern sh_int gsn_shocking_grasp;
extern sh_int gsn_whirlwind;
extern sh_int gsn_acid_storm;
extern sh_int gsn_acid_rain;
extern sh_int gsn_angelfire;
extern sh_int gsn_magic_slash;
extern sh_int gsn_wild_magic;
extern sh_int gsn_swarm;
extern sh_int gsn_pass_door;
extern sh_int gsn_backstab;
extern sh_int gsn_explode;
extern sh_int gsn_pinpoint;
extern sh_int gsn_forage;
extern sh_int gsn_multiexp;
extern sh_int gsn_quadexp;
extern sh_int gsn_embedded_spikes;
extern sh_int gsn_retreat;
extern sh_int gsn_immunize;
extern sh_int gsn_bludgeon;
extern sh_int gsn_blackjack;
extern sh_int gsn_brew;
extern sh_int gsn_scribe;
extern sh_int gsn_sharpen;
extern sh_int gsn_blind_rage;
extern sh_int gsn_slow;
extern sh_int gsn_quicken;
extern sh_int gsn_trapset;
extern sh_int gsn_bind;
extern sh_int gsn_avenger;
extern sh_int gsn_windpipe;
extern sh_int gsn_iron_grip;
extern sh_int gsn_circle;
extern sh_int gsn_dodge;
extern sh_int gsn_envenom;
extern sh_int gsn_hyperbody;
extern sh_int gsn_holy_sword;
extern sh_int gsn_fire_storm;
extern sh_int gsn_shadow_shield;
extern sh_int gsn_camouflage;
extern sh_int gsn_evade;
extern sh_int gsn_feed;
extern sh_int gsn_hide;
extern sh_int gsn_peek;
extern sh_int gsn_door_bash;
extern sh_int gsn_pick_lock;
extern sh_int gsn_sneak;
extern sh_int gsn_steal;
extern sh_int gsn_disarm;
extern sh_int gsn_enhanced_damage;
extern sh_int gsn_kick;
extern sh_int gsn_green_death;
extern sh_int gsn_parry;
extern sh_int gsn_rescue;
extern sh_int gsn_second_attack;
extern sh_int gsn_third_attack;
extern sh_int gsn_search;
extern sh_int gsn_mend;
extern sh_int gsn_call_wild;
extern sh_int gsn_holy_fury;
extern sh_int gsn_endurance;
extern sh_int gsn_shield_rush;
extern sh_int gsn_stun;
extern sh_int gsn_blindness;
extern sh_int gsn_charm_person;
extern sh_int gsn_curse;
extern sh_int gsn_invis;
extern sh_int gsn_mass_invis;
extern sh_int gsn_plague;
extern sh_int gsn_poison;
extern sh_int gsn_fire_breath;
extern sh_int gsn_sleep;
extern sh_int gsn_fly;
extern sh_int gsn_sanctuary;
extern sh_int gsn_familiar;
extern sh_int gsn_heavyfeet;
extern sh_int gsn_door;
extern sh_int gsn_flanking;
extern sh_int gsn_impale;
extern sh_int gsn_barkskin;
extern sh_int gsn_fumble;
extern sh_int gsn_weaken;
extern sh_int gsn_channel;
extern sh_int gsn_underwater_breathing;
extern sh_int gsn_blink;
extern sh_int gsn_purify;
extern sh_int gsn_blur;
extern sh_int gsn_focus;

/* new gsns */
extern sh_int gsn_axe;
extern sh_int gsn_dagger;
extern sh_int gsn_bow;
extern sh_int gsn_claw;
extern sh_int gsn_flail;
extern sh_int gsn_mace;
extern sh_int gsn_polearm;
extern sh_int gsn_shield_block;
extern sh_int gsn_spear;
extern sh_int gsn_sword;
extern sh_int gsn_whip;

extern sh_int gsn_throw;
extern sh_int gsn_bash;
extern sh_int gsn_berserk;
extern sh_int gsn_dirt;
extern sh_int gsn_hand_to_hand;
extern sh_int gsn_trip;
extern sh_int gsn_camp;

extern sh_int gsn_fast_healing;
extern sh_int gsn_haggle;
extern sh_int gsn_meditation;

extern sh_int gsn_scrolls;
extern sh_int gsn_staves;
extern sh_int gsn_wands;
extern sh_int gsn_recall;
extern sh_int gsn_portals;
extern CRS crs_info;

extern sh_int gsn_rune;

/*
 * Utility macros.
 */
#define IS_VALID(data)        ((data) != NULL && (data)->valid)
#define VALIDATE(data)        ((data)->valid = TRUE)
#define INVALIDATE(data)    ((data)->valid = FALSE)
#define UMIN(a, b)        ((a) < (b) ? (a) : (b))
#define UMAX(a, b)        ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)        ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)        ((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)        ((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)    ((flag) & (bit))
#define SET_BIT(var, bit)    ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define IS_NULLSTR(str)        ((str) == NULL || (str)[0] == '\0')
#define ENTRE(min,num,max)    ( ((min) < (num)) && ((num) < (max)) )
#define CHECK_POS(a, b, c)    {                            \
                    (a) = (b);                    \
                    if ( (a) < 0 )                    \
                    dbbug( "CHECK_POS : " c " == %d < 0", a );    \
                }                            \


/*
 * Character macros.
 */
#define IS_NPC(ch)        (IS_SET((ch)->act, ACT_IS_NPC))
#define IS_IMMORTAL(ch)        (ch->level >= LEVEL_IMMORTAL)
#define IS_HERO(ch)        (ch->level == LEVEL_HERO)
#define IS_AFFECTED(ch, sn)    (IS_SET((ch)->affected_by, (sn)))
#define IS_AFFECTED2(ch, sn)   (IS_SET((ch)->affected2_by, (sn)))

#define GET_AGE(ch)        ((int) (17 + ((ch)->played \
                    + current_time - (ch)->logon )/72000))

#define IS_QUESTOR(ch)  (IS_SET((ch)->act, PLR_QUESTOR))
#define IS_GOOD(ch)        (ch->alignment > 0)
#define IS_EVIL(ch)        (ch->alignment < 0)
#define IS_NEUTRAL(ch)     (ch->alignment == 0)

#define GET_OCHAR_DESCRIPTOR(d)  ((d) ? (((d)->connected == CON_PLAYING) ? ((d)->original ? (d)->original : (d)->character) : NULL) : NULL)

#define IS_AWAKE(ch)        (ch->position > POS_SLEEPING)

#define D20LVL(ch) (UMIN((ch)->level / (LEVEL_HERO / 19), 19))
#define D20MA(int) (UMIN((int) / (LEVEL_HERO / 19), 19))
#define AMOD(stat) (((stat) - 0.5) / 2 - 5)
#define SMOD(size) (2 - (size))

#define IS_OUTSIDE(ch)        (!IS_SET(                    \
                    (ch)->in_room->room_flags,            \
                    ROOM_INDOORS))

#define WAIT_STATE(ch, npulse)    ((ch)->wait = UMAX((ch)->wait, (npulse)))
#define DAZE_STATE(ch, npulse)  ((ch)->daze = UMAX((ch)->daze, (npulse)))
//
// make gold weightless - Hitch 1/12/05
//
#define get_carry_weight(ch)    ((ch)->carry_weight )

#define HAS_TRIGGER_MOB(ch,trig)    (IS_SET((ch)->pIndexData->mprog_flags,(trig)))
#define HAS_TRIGGER_OBJ(obj,trig) (IS_SET((obj)->pIndexData->oprog_flags,(trig)))
#define HAS_TRIGGER_ROOM(room,trig) (IS_SET((room)->rprog_flags,(trig)))
#define IS_SWITCHED( ch )       ( ch->desc && ch->desc->original )

/*
 * Object macros.
 */
#define CAN_WEAR(obj, part)    (IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)    (IS_SET((obj)->extra_flags, (stat)))
#define IS_OBJ_AFFECT(obj, stat)  (IS_SET((obj)->affects, (stat)))
#define IS_WEAPON_STAT(obj,stat)(IS_SET((obj)->value[4],(stat)))
#define WEIGHT_MULT(obj)    ((obj)->item_type == ITEM_CONTAINER ? \
    (obj)->value[4] : 100)

/*
 * Description macros.
 */
#define DEFAULT_WIZINAME        "An Immortal"

/*#define PERS(ch, looker) ( can_see( looker,(ch) )?             \
                         ( IS_NPC(ch) ? (ch)->short_descr        \
                         : full_name( (ch) ) ) : IS_IMMORTAL(ch) ?      \
                         "An Immortal" :         \
                         "Someone") */

/*#define PERS(ch, looker) ( can_see( looker,(ch) )?             \
                         ( IS_NPC(ch) ? (ch)->short_descr        \
                         : full_name( (ch) ) ) : IS_IMMORTAL(ch) ?      \
                         ((ch)->pcdata->wiziname[0]=='\0' ?          \
                         DEFAULT_WIZINAME :                            \
                         (ch)->pcdata->wiziname ) : "Someone") */

char *PERS(CHAR_DATA * ch, CHAR_DATA * looker);

/*
 * Structure for a social in the socials table.
 */
struct social_type {
   char name[20];
   char *char_no_arg;
   char *others_no_arg;
   char *char_found;
   char *others_found;
   char *vict_found;
   char *char_not_found;
   char *char_auto;
   char *others_auto;
   char *creator;
};

/*
 * Global constants.
 */
extern const struct str_app_type str_app[MAX_STR + 1];
extern const struct int_app_type int_app[MAX_INT + 1];
extern const struct wis_app_type wis_app[MAX_WIS + 1];
extern const struct dex_app_type dex_app[MAX_DEX + 1];
extern const struct con_app_type con_app[MAX_CON + 1];
extern const struct mana_app_type mana_app[20];
extern const struct class_type class_table[MAX_CLASS];
extern const struct weapon_type weapon_table[];
extern const struct armor_type armor_table[];
extern const struct item_type item_table[];
extern const struct wiznet_type wiznet_table[];
extern const struct attack_type attack_table[];
extern const int bab[MAX_CLASS][20][4];
extern const int hth[20][2];
extern const struct monster_advancement ma_table[];
extern const struct race_type race_table[];
extern const struct pc_race_type pc_race_table[];
extern const struct spec_type spec_table[];
char last_command[MAX_STRING_LENGTH];
extern const struct liq_type liq_table[];
extern const struct material_type material_table[];
extern const struct skill_type skill_table[MAX_SKILL];
extern struct social_type *social_table;

/*
 * Global variables.
 */
extern HELP_DATA *help_first;
extern HELP_DATA *help_last;
extern SHOP_DATA *shop_first;
extern CONTENTS_DATA *contents_list;

extern CHAR_DATA *char_list;
extern OBJ_DATA *object_list;

extern PROG_CODE *mprog_list;
extern PROG_CODE *rprog_list;
extern PROG_CODE *oprog_list;

extern char bug_buf[];
extern time_t current_time;
extern bool fLogAll;
extern KILL_DATA kill_table[];
extern char log_buf[];
extern TIME_INFO_DATA time_info;
extern WEATHER_DATA weather_info;
extern bool global_quest;
extern bool global_quest_closed;
extern bool packrat_quest;
extern int packrat_timer;

extern NOTE_DATA *note_free;
extern OBJ_DATA *obj_free;
extern int double_exp;
extern bool double_gold;
extern int quad_exp;

extern bool qreset_on;
extern int qreset_timer;
extern int global_exp;
extern int global_gold;
extern int global_qp;
extern int global_quad;
extern int glo_quest;
extern bool double_qp;
extern bool quad_damage;
extern sh_int display;
extern sh_int qpdisplay;
extern int maxSocial;
extern DISABLED_DATA *disabled_first;  /* interp.c */
extern PLAYER_RANKS_DATA *player_rank_first;
extern const struct god_type god_table[MAX_GOD];
extern const struct devout_type devout_table[MAX_DEVOUT];
extern const char *question_table[MAX_QUESTION];
extern float guess_timer;

// Auction code, thanks Bree!! 29.July.2007 --Kira

typedef struct auction_data AUCTION_DATA;

struct auction_data {
   AUCTION_DATA *next;
   OBJ_DATA *item;
   CHAR_DATA *owner;
   CHAR_DATA *high_bidder;
   sh_int status;
   sh_int slot;
   sh_int bid_type;
   int bid_amount;
};

/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if    defined(_AIX)
char *crypt args((const char *key, const char *salt));
#endif

#if    defined(apollo)
int atoi args((const char *string));
void *calloc args((unsigned nelem, size_t size));
char *crypt args((const char *key, const char *salt));
#endif

#if    defined(hpux)
char *crypt args((const char *key, const char *salt));
#endif

#if    defined(linux)
char *crypt args((const char *key, const char *salt));
#endif

#if    defined(macintosh)
#define NOCRYPT
#if    defined(unix)
#undef    unix
#endif
#endif

#if    defined(MIPS_OS)
char *crypt args((const char *key, const char *salt));
#endif

#if    defined(MSDOS)
#define NOCRYPT
#if    defined(unix)
#undef    unix
#endif
#endif

#if    defined(NeXT)
char *crypt args((const char *key, const char *salt));
#endif

#if    defined(sequent)
char *crypt args((const char *key, const char *salt));
int fclose args((FILE * stream));
int fprintf args((FILE * stream, const char *format, ...));
int fread args((void *ptr, int size, int n, FILE * stream));
int fseek args((FILE * stream, long offset, int ptrname));
void perror args((const char *s));
int ungetc args((int c, FILE * stream));
#endif

#if    defined(sun)
char *crypt args((const char *key, const char *salt));
int fclose args((FILE * stream));
int fprintf args((FILE * stream, const char *format, ...));
#if    defined(SYSV)
siz_t fread args((void *ptr, size_t size, size_t n, FILE * stream));
#elif !defined(__SVR4)
int fread args((void *ptr, int size, int n, FILE * stream));
#endif
int fseek args((FILE * stream, long offset, int ptrname));
void perror args((const char *s));
int ungetc args((int c, FILE * stream));
#endif

#if    defined(ultrix)
char *crypt args((const char *key, const char *salt));
#endif

/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if    defined(NOCRYPT)
#define crypt(s1, s2)    (s1)
#endif

/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 */
#if defined(macintosh)
#define PLAYER_DIR      ""      /* Player files            */
#define TEMP_FILE       "romtmp"
#endif

#if defined(MSDOS)
#define PLAYER_DIR      ""      /* Player files           */
#define TEMP_FILE       "romtmp"
#endif

#if defined(unix)
#define ACCOUNT_DIR     "account/"  /* Account files            */
#define PLAYER_DIR      "player/"   /* Player files           */
#define TEMP_FILE       "player/romtmp"
#endif

// Let try and keep all file path definitions in one place - Maxxus

#define COPYOVER_FILE     "copyover.data" /*This file holds the copyover data */
#define EXE_FILE          "rom" /* This is the executable file */
#define NOTE_DIR          "notes"   /* set it to something you like */
#define AREA_DIR          "area"
#define AREA_LIST         "area/area.lst" /* List of areas         */
#define MEM_DMP           "data/mem.dmp"
#define OBJ_DMP           "data/obj.dmp"
#define MOB_DMP           "data/mob.dmp"
#define DUMP_FILE         "data/dumpworld.cvs"
#define TODO_FILE         "data/todos.dat"
#define GAME_DATA_FILE    "data/gamedata.txt"
#define TODO_BAK_FILE     "data/todos.old"
#define FAMILY_FILE       "data/family.fam"
#define BUG_FILE          "data/bugs.txt" /* For 'bug' and bug()   */
#define TYPO_FILE         "data/typos.txt"   /* For 'typo'            */
#define SHUTDOWN_FILE     "data/shutdown.txt"   /* For 'shutdown'        */
#define BAN_FILE          "data/ban.txt"
#define MUSIC_FILE        "data/music.txt"
#define STATS_FILE        "data/stats.txt"   /* Rendelven - keeps stats on mud */
#define OHELPS_FILE       "data/orphaned_helps.txt"   /* Unmet 'help' requests */
#define CHANGES_FILE      "data/changes.txt" /* changes file  */
#define POLLS_FILE        "data/polls.txt"   /* Voting and Polls file - Kira */
#define MISSING_HELPS     "data/missing_helps.txt" /* missing helpfiles */
#define CLAN_FILE         "data/clan.txt" /* Rendelven - information on the clans. */
#define CONTENTS_FILE     "data/contents.txt"   /* Rendelven - keeps a list of contents */
#define DISABLED_FILE     "data/disabled.txt"   /* disabled commands */
#define PLAYER_RANKS_FILE "data/ranks.txt"   /* rank data */
#define ILLEGAL_NAME_FILE "data/illegalnames.txt"  /* for illegal names - Kira */
#define SOCIAL_FILE       "data/socials.txt" /* or whatever fits you */
#define LAST_COMMAND      "data/last.txt" /* For 'last command' */
#define ARTIFACTS         "data/artifacts.txt"  /* Artifact items */
#define ISSUES_FILE       "data/issues.txt"  /* Issues file */
#define GROUP_SAVE_FILE   "data/groups.dmp"  /* Copyover Group Save File */


/*
 * Disabled Commands Stuff
 */
bool check_disabled(const struct cmd_type *command);
DISABLED_DATA *disabled_first;
#define END_MARKER      "END"   /* for load_disabled() and save_disabled() */

/*
 * Ranked Commands Stuff
 */
bool check_player_ranks(CHAR_DATA * ch);
PLAYER_RANKS_DATA *rank_first;
#define END_MARKER      "END"   /* for load_ranks() and save_ranks() */

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD   CHAR_DATA
#define MID  MOB_INDEX_DATA
#define OD   OBJ_DATA
#define OID  OBJ_INDEX_DATA
#define RID  ROOM_INDEX_DATA
#define SF   SPEC_FUN
#define AD   AFFECT_DATA
#define PC  PROG_CODE

void copyover args((void));
void crash_copyover args((void));
char *crs_sprintf args((bool pTime, bool reason));

#define         CRS_COPYOVER        1
#define         CRS_REBOOT          2
#define         CRS_SHUTDOWN        3

struct crs_data {
   int status;
   int timer;
   char *who;
   char *reason;
};

void reboot_mud args((void));
void shutdown_mud args((void));

/* act_comm.c */
void loginToFriends(CHAR_DATA * ch, char *argument);
void save(CHAR_DATA *ch, bool mesg);
bool are_friends(CHAR_DATA * ch, CHAR_DATA * victim);
void save_char(CHAR_DATA * ch);
void do_say(CHAR_DATA * ch, char *argument);
void public_ch(CHAR_DATA * ch, char *argument, char *type, const int bitname, char *colorcode, int chan,
   CHAR_DATA * tellchar);
bool is_group(CHAR_DATA * ch);
char *str_printf(char *format, ...);
void doprintf(DO_FUN * fn, CHAR_DATA * ch, const char *fmt, ...);
void bust_a_prompt args((CHAR_DATA * ch));
void delete_char args((CHAR_DATA * ch));
void check_sex args((CHAR_DATA * ch));
void add_follower args((CHAR_DATA * ch, CHAR_DATA * master));
void stop_follower args((CHAR_DATA * ch));
void nuke_pets args((CHAR_DATA * ch));
void die_follower args((CHAR_DATA * ch));
bool is_same_group args((CHAR_DATA * ach, CHAR_DATA * bch));
void sendToFriends args((CHAR_DATA * ch, char *argument));
void level_char args((CHAR_DATA * ch));
void advance_stats args((CHAR_DATA * ch));
char *full_name args((CHAR_DATA * ch));

/* act_enter.c */
RID *get_random_room args((CHAR_DATA * ch));
ROOM_INDEX_DATA *get_rand_room(ROOM_INDEX_DATA * pRoomIndex);

/* act_info.c */
char *player_rank_one (CHAR_DATA *ch, int rank);
int blank_tiles(CHAR_DATA *ch);
void getWhoDescr(CHAR_DATA * ch, char *buf);
void guess_win(CHAR_DATA * ch);
int count(CHAR_DATA * ch);
void remove_remort_class(CHAR_DATA * ch);
int faux_cap(CHAR_DATA * ch);   //Kira
bool is_multiclass args((CHAR_DATA * ch));   //Kira
void getPretitle args((CHAR_DATA * ch, char *buf));
void set_title args((CHAR_DATA * ch, char *title));
void set_mstatus args((CHAR_DATA * ch, char *mstatus));
bool IsAFriend args((CHAR_DATA * ch, CHAR_DATA * wch));  //Hitch
bool playerExists args((const char *name));
void sort_lines args((char *strings[], int elements));   //Hitch
void get_ac args((CHAR_DATA * ch, int type));   // Kira
void getAffectDescs args((char *buf, CHAR_DATA * victim, bool longaffs));  //Hitch
void refreshFriends args((CHAR_DATA * ch));  // Hitch 29/01/06
void refreshViews args((CHAR_DATA * ch)); // Hitch 29/01/06
int class_count(CHAR_DATA * ch); //Kira

/* act_move.c */
int explored_percent (CHAR_DATA *ch);
int explored (CHAR_DATA *ch);
bool can_recall(CHAR_DATA * ch, char *argument);
void check_explore args((CHAR_DATA *, ROOM_INDEX_DATA *));
void explore_vnum args((CHAR_DATA *, int));
bool explored_vnum args((CHAR_DATA *, int));

void transfer_char(CHAR_DATA * ch, CHAR_DATA * vch, ROOM_INDEX_DATA * to_room, const char *msg_out,
   const char *msg_travel, const char *msg_in);
int find_exit(CHAR_DATA * ch, char *arg);
void move_char args((CHAR_DATA * ch, int door, bool follow));
void free_runbuf args((DESCRIPTOR_DATA * d));

/* act_obj.c */
//Joe:Lockers  Required for locker code
//void   show_list_to_char args( ( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing ) );
bool remove_obj args((CHAR_DATA * ch, int iWear, bool fReplace));
bool can_loot args((CHAR_DATA * ch, OBJ_DATA * obj));
bool wear_check args((CHAR_DATA * ch, OBJ_DATA * obj, bool show));
void wear_obj args((CHAR_DATA * ch, OBJ_DATA * obj, bool fReplace));
void get_obj args((CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * container, BUFFER *, BUFFER *));

/* act_wiz.c */
void guess_clear(char *argument);
void scatter_guess_letters(char *argument);
void print_blanks(CHAR_DATA * ch);
void set_guess_phrase(char *argument);
bool guess_on(CHAR_DATA * ch);
void packrat_off(void);
void echo(char *argument);
void quadexp(CHAR_DATA * ch, char *argument);
void rank_announce(char *argument);
void doublexp(CHAR_DATA * ch, char *argument);
ROOM_INDEX_DATA *find_location args((CHAR_DATA * ch, char *arg));
void wiznet args((char *string, CHAR_DATA * ch, OBJ_DATA * obj, long flag, long flag_skip, int min_level));
void copyover_recover args((void));
void do_infochan args((CHAR_DATA * ch, char *argument, bool imm, int pointer));
void restore_char args((CHAR_DATA * ch));

#define INFO_NOTE  1

/* alias.c */
void substitute_alias args((DESCRIPTOR_DATA * d, char *input));

/* arena.c */
int pot args((int value));

/* artifacts.c */
void create_artifact(OBJ_INDEX_DATA * pObjIndex);
void destroy_artifact(OBJ_INDEX_DATA * pObjIndex);
bool test_artifact(OBJ_DATA * pObj);
bool increase_artifact(OBJ_DATA * pObj, CHAR_DATA * pCh);
void decrease_artifact(OBJ_DATA * pObj, CHAR_DATA * pCh);
void save_artifacts();
void load_artifacts();

/* auction.c */
AUCTION_DATA *auction_list;

void free_auction(AUCTION_DATA * auction);

/* ban.c */
bool check_ban args((char *site, int type));

/* board.c */
void delete_notes args((CHAR_DATA * ch)); /* Rendelven */
bool unreadnotes(CHAR_DATA * ch);
void board(CHAR_DATA * ch, char *board);

/*
 * comm.c
 */

/* TODO: Make descriptor_list local by changing all references to calls to act_new -M */
DESCRIPTOR_DATA *descriptor_list;
bool merc_down;                 /* Shutdown */
bool wizlock;                   /* Game is wizlocked */
bool newlock;                   /* Game is newlocked */
time_t current_time;            /* time of this pulse */
bool MOBtrigger;                /* act() switch */
char str_boot_time[MIL];        /* Game boot time */
int port, control;              /* Game port and listner socket */

void close_socket args((DESCRIPTOR_DATA * dclose));
bool write_to_descriptor args((int desc, char *txt, int length));
void write_to_buffer args((DESCRIPTOR_DATA * d, const char *txt, int length));
int colour_len(char *str);
int strlen_colour args((char *str));   //Hitch 01/02/06
bool has_number args((char *str));  //Kira 4 August 08
char *strip_colour args((const char *clrstr));  /* Rendelven - returns a color-stripped version of clrstr */
void send_to_desc args((const char *txt, DESCRIPTOR_DATA * d));
void send_to_char args((const char *txt, CHAR_DATA * ch));
void page_to_char args((const char *txt, CHAR_DATA * ch));
void printf_to_desc args((DESCRIPTOR_DATA *, char *, ...));
void printf_to_char args((CHAR_DATA *, char *, ...));
void act_new args((const char *format, CHAR_DATA * ch, const void *arg1, const void *arg2, int type, int min_pos));
void logs args((const char *str));
void logfs args((const char *fmt, ...));
void bugs args((const char *str));
void bugf args((char *, ...));
void printBackTrace args((void));

/* nanny.c */
void printAccountMenu(DESCRIPTOR_DATA * d);
bool surname_exists args((char *surname));

/* db.c */
void remove_player_ranking(CHAR_DATA * ch, PLAYER_RANKS_DATA * p);
void global_rank_update(CHAR_DATA * ch);
void do_rank(CHAR_DATA * ch);
CHAR_DATA *create_mobile(MOB_INDEX_DATA * pMobIndex);
void load_game_data();
void save_game_data();
char *strnzcpy(char *dest, size_t len, char *src);
void reset_area args((AREA_DATA * pArea, bool pTrueReset)); /* OLC */
void reset_room args((ROOM_INDEX_DATA * pRoom, bool pTrueReset)); /* OLC */

char *print_flags args((int flag));
void boot_db args((void));
void area_update args((void));
CD *create_mobile args((MOB_INDEX_DATA * pMobIndex));
void clone_mobile args((CHAR_DATA * parent, CHAR_DATA * clone));
OD *create_object args((OBJ_INDEX_DATA * pObjIndex, int level));
void clone_object args((OBJ_DATA * parent, OBJ_DATA * clone));
void clear_char args((CHAR_DATA * ch));
char *get_extra_descr args((const char *name, EXTRA_DESCR_DATA * ed));
MID *get_mob_index args((int vnum));
OID *get_obj_index args((int vnum));
RID *get_room_index args((int vnum));
PC *get_prog_index args((int vnum, int type));
char fread_letter args((FILE * fp));
int fread_number args((FILE * fp));
long fread_flag args((FILE * fp));
char *fread_string args((FILE * fp));
void fread_to_eol args((FILE * fp));
char *fread_word args((FILE * fp));
float fread_decimal args((FILE * fp));
long flag_convert args((char letter));
void *alloc_mem args((int sMem));
void *alloc_perm args((int sMem));
void free_mem args((void *pMem, int sMem));
char *str_dup args((const char *str));
void free_string args((char *pstr));
#define FUNCTIONIZE(a,b)  a(b)
#define STRINGIZE(a)      #a
#define INT2STRING(i)     FUNCTIONIZE(STRINGIZE,i)
#define FILE_POS       __FILE__ ":" INT2STRING(__LINE__)
int number_fuzzy args((int number));
int number_range args((int from, int to));
int number_percent args((void));
int number_door args((void));
int number_bits args((int width));
long number_mm args((void));
int dice args((int number, int size));
int interpolate args((int level, int value_00, int value_32));
void smash_tilde args((char *str));
void smash_dollar args((char *str));
bool str_cmp args((const char *astr, const char *bstr));
bool str_prefix args((const char *astr, const char *bstr));
bool str_infix args((const char *astr, const char *bstr));
bool str_suffix args((const char *astr, const char *bstr));
char *capitalize args((const char *str));
void append_file args((CHAR_DATA * ch, char *file, char *str));
void tail_chain args((void));
bool check_pet_affected args((int vnum, AFFECT_DATA * paf));
char *str_replace args((char *astr, char *bstr, char *cstr));  // Ty Bree
void dbbug args((const char *str, int param));
void dbbugf args((char *, ...));

void save_contents args((void)); /* Rendelven */

void load_social_table();
void save_social_table();

/* effect.c */
void acid_effect args((void *vo, int level, int dam, int target));
void cold_effect args((void *vo, int level, int dam, int target));
void fire_effect args((void *vo, int level, int dam, int target));
void poison_effect args((void *vo, int level, int dam, int target));
void shock_effect args((void *vo, int level, int dam, int target));

/* event.c */
void create_event(EVENT_FUN * a, void *b, char *c, int d, int8_t e);
void purge_events(void *data, int8_t event_type);

/* fight.c */
bool npc_is_grenade_safe(CHAR_DATA * victim);
void lore(CHAR_DATA * ch, OBJ_DATA * obj);
bool can_pvp_victim(CHAR_DATA * ch, CHAR_DATA * victim);
bool is_same_clan(CHAR_DATA * ch, CHAR_DATA * victim);
bool is_rpk(CHAR_DATA * rch);
bool is_fpk(CHAR_DATA * rch);
bool is_npk(CHAR_DATA * rch);
bool check_counter(CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dam_type);
void set_fighting(CHAR_DATA * ch, CHAR_DATA * victim);
void hunt_victim_attack(CHAR_DATA * ch);
void hunt_victim(CHAR_DATA * ch);
bool check_object_affect(CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * wield);
int xp_compute(CHAR_DATA * gch, CHAR_DATA * victim, int group_level);
void raw_kill(CHAR_DATA * ch, CHAR_DATA * victim);
bool cover_movement(CHAR_DATA * ch);
bool check_obj_dodge(CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj);
bool is_safe args((CHAR_DATA * ch, CHAR_DATA * victim, bool area));
bool is_voodood args((CHAR_DATA * ch, CHAR_DATA * victim));
bool is_safe_spell args((CHAR_DATA * ch, CHAR_DATA * victim, bool area));
void violence_update args((void));
void multi_hit args((CHAR_DATA * ch, CHAR_DATA * victim, int dam_msg));
int damage args((CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dam_msg, int dam_type, bool show, bool spell));
void update_pos args((CHAR_DATA * victim));
void stop_fighting args((CHAR_DATA * ch, bool fBoth));
void check_killer args((CHAR_DATA * ch, CHAR_DATA * victim));

/* group.c */
bool group_in_room_count (CHAR_DATA *ch);
void disband_group (CHAR_DATA *ch, int group);
void quit_group (CHAR_DATA *ch);
void pass_leadership (CHAR_DATA *ch, int group);
int member_count (int i);
int group_lookup(const char *name);

/* handler.c */
char *res_bit_name(int res_flags);
char *vuln_bit_name(int vuln_flags);
bool string_has_symbols (char *argument);
bool guess_all_dashes (char *argument);
void gqcomplete_add(CHAR_DATA *ch, int amount);
bool can_add_follower (CHAR_DATA *ch);
int get_damroll_modifiers(CHAR_DATA * ch);
int get_hitroll_modifiers(CHAR_DATA * ch);
int melee_damage (CHAR_DATA *ch);
int dam_throw(CHAR_DATA *ch);
bool can_set_trap(CHAR_DATA *ch);
int get_spell_duration_multiplier(CHAR_DATA * ch);
void qps_set(CHAR_DATA *ch, int amount);
void gold_set(CHAR_DATA *ch, int amount);
void gold_remove(CHAR_DATA *ch, int amount);
OBJ_DATA *carries(CHAR_DATA *ch, char *argument);
void outfit(CHAR_DATA * ch, char *argument);
bool is_clan_hall(ROOM_INDEX_DATA *room);
int get_gold_multiplier(CHAR_DATA * ch);
int get_qp_multiplier(CHAR_DATA * ch);
bool potion_has_spell (OBJ_INDEX_DATA *obj, char *argument);
bool string_has_dash (char *argument);
void mob_kills_add(CHAR_DATA *ch, int amount);
int group_members_in_room (CHAR_DATA *ch);
int group_level (CHAR_DATA *ch);
int affect_level (CHAR_DATA *ch, int sn);
void remove_from_arena(CHAR_DATA *ch);
void remove_challenge(CHAR_DATA *ch);
void marry (CHAR_DATA *ch, CHAR_DATA *victim);
void spouse_login (CHAR_DATA *ch);
bool is_hubby (CHAR_DATA *ch, CHAR_DATA *victim);
bool has_ictitle(CHAR_DATA * ch);
bool can_door(CHAR_DATA * ch);
void gwins_add(CHAR_DATA * ch, int amount);
void turnins_add(CHAR_DATA *ch, int amount);
void gold_add(CHAR_DATA *ch, int amount);
void qcomplete_add(CHAR_DATA *ch, int amount);
char *format_time(float time);
bool can_gate(CHAR_DATA * ch, CHAR_DATA * victim);
void guess_clean_world(char *argument);
bool is_clan_leader(CHAR_DATA * ch);
bool obj_owned(OBJ_DATA * obj);
char *cooldown_time(CHAR_DATA * ch, int sn);
bool can_hunt(CHAR_DATA * ch, CHAR_DATA * victim);
void limit_gain(CHAR_DATA * ch, float gains);
char *limit_gauge(CHAR_DATA * ch);
float limit_percent(CHAR_DATA * ch);
int get_mana_reduction(CHAR_DATA * ch);
int get_mana_multiplier(CHAR_DATA * ch);
int get_carry_multiplier(CHAR_DATA * ch);
int get_dam_multiplier(CHAR_DATA * ch);
int get_exp_multiplier(CHAR_DATA * ch);
int get_heal_multiplier(CHAR_DATA * ch);
int get_vit_percent(CHAR_DATA * ch);
int heal_percent(int percent, int amount);
void faux_add(CHAR_DATA * ch, int amount);
void faux_remove(CHAR_DATA * ch, int amount);
void qps_add(CHAR_DATA * ch, int amount);
void qps_remove(CHAR_DATA * ch, int amount);
void mudpie_add(CHAR_DATA * ch, int amount);
void mudpie_remove(CHAR_DATA * ch, int amount);
int party_count(CHAR_DATA * ch);
float vitality(CHAR_DATA * ch);
bool leader_buff(CHAR_DATA * ch, int sn);
bool can_increase_stat(CHAR_DATA * ch, int stat);
int curr_max_stat(CHAR_DATA * ch, int stat);
int training_cost(CHAR_DATA * ch, int stat);
bool in_pq_area(CHAR_DATA * ch);
bool is_builder(CHAR_DATA * ch, AREA_DATA * pArea);
void adjust_mob(CHAR_DATA * ch, CHAR_DATA *pMob);
bool is_friend(CHAR_DATA * ch, CHAR_DATA * victim);
bool tells(CHAR_DATA * ch, CHAR_DATA * victim);
bool update_guess(CHAR_DATA * ch, OBJ_DATA * obj);
int get_str(CHAR_DATA * ch);
int get_dex(CHAR_DATA * ch);
int get_int(CHAR_DATA * ch);
int get_con(CHAR_DATA * ch);
int get_wis(CHAR_DATA * ch);
int get_luck(CHAR_DATA * ch);
bool thief_fullset(CHAR_DATA * ch);
bool gaian_fullset(CHAR_DATA * ch);
bool mage_fullset(CHAR_DATA * ch);
bool psi_fullset(CHAR_DATA * ch);
void fullset_qeq(CHAR_DATA * ch, OBJ_DATA * worn);
void thief_weaps(CHAR_DATA * ch, OBJ_DATA * worn);
void thief_bonus(CHAR_DATA * ch, OBJ_DATA * worn);
void mage_bonus(CHAR_DATA * ch, OBJ_DATA * worn);
void strip_mage_bonus(CHAR_DATA * ch);
void mana_multiplier(CHAR_DATA * ch, OBJ_DATA * worn);
void strip_mana_multiplier(CHAR_DATA * ch);
void strip_cleric_bonus(CHAR_DATA * ch);
void cleric_bonus(CHAR_DATA * ch, OBJ_DATA * worn);
void cleric_spell(CHAR_DATA * ch);
void psi_bonus(CHAR_DATA * ch, OBJ_DATA * worn);
void psi_spell(CHAR_DATA * ch);
void strip_psi_bonus(CHAR_DATA * ch);
void gaian_bonus(CHAR_DATA * ch, OBJ_DATA * worn);
void gaian_spell(CHAR_DATA * ch);
void strip_gaian_bonus(CHAR_DATA * ch);
void strip_thief_bonus(CHAR_DATA * ch);
void strip_warrior_bonus(CHAR_DATA * ch);
void warrior_bonus(CHAR_DATA * ch, OBJ_DATA * worn);
void warrior_spell(CHAR_DATA * ch);
void strip_thief_bonus(CHAR_DATA * ch);
void thief_spell(CHAR_DATA * ch);
void mage_spell(CHAR_DATA * ch);
int apply(CHAR_DATA * ch, int apply_type, OBJ_DATA * worn);
void path_to_track(CHAR_DATA * ch, CHAR_DATA * victim, int door);
void destroy_hash_table(struct hash_header *ht, void (*gman) ());
void *hash_find(struct hash_header *ht, int key);
int hash_enter(struct hash_header *ht, int key, void *data);
void _hash_enter(struct hash_header *ht, int key, void *data);
bool exit_ok(EXIT_DATA * pexit);
void donothing();
void init_hash_table(struct hash_header *ht, int rec_size, int table_size);
bool switch_descriptions(OBJ_DATA * obj, int NEW_OBJ_VNUM, char *name);
void set_timer_affect_char(CHAR_DATA * ch, int sn, int timer);
void mutilate_corpse(OBJ_DATA * obj);
int pk_range(int level);
int find_path(int in_room_vnum, int out_room_vnum, CHAR_DATA * ch, int depth, int in_zone);
CHAR_DATA *get_char_area(CHAR_DATA * ch, char *argument);
void remove_mind(CHAR_DATA * ch, char *str);
void back_home(CHAR_DATA * ch);
void add_mind(CHAR_DATA * ch, char *str);
CHAR_DATA *get_char_room_raw(CHAR_DATA * ch, char *name, uint * number, ROOM_INDEX_DATA * room);
bool send_arrow(CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * arrow, int door);
OBJ_DATA *find_arrow(CHAR_DATA * ch);
CHAR_DATA *find_char(CHAR_DATA * ch, char *argument, int door, int range);
bool is_state_room_fight(CHAR_DATA * ch);
int ch_carrying(CHAR_DATA * ch);
int dam_luck(CHAR_DATA * ch, int sn);
int dam_dex(CHAR_DATA * ch, int sn);
int dam_wis(CHAR_DATA * ch, int sn);
int dam_con(CHAR_DATA * ch, int sn);
int dam_int(CHAR_DATA * ch, int sn);
int dam_str(CHAR_DATA * ch, int sn);
int total_int(CHAR_DATA * ch);
int total_con(CHAR_DATA * ch);
int total_luck(CHAR_DATA * ch);
int total_dex(CHAR_DATA * ch);
int total_wis(CHAR_DATA * ch);
int total_str(CHAR_DATA * ch);
AD *affect_find args((AFFECT_DATA * paf, int sn));
void affect_check args((CHAR_DATA * ch, int where, int vector));
int count_users args((OBJ_DATA * obj));
//void     deduct_cost    args( (CHAR_DATA *ch, int cost, int value) );
void deduct_cost(CHAR_DATA * ch, int cost, int value);
bool can_view(CHAR_DATA * ch, CHAR_DATA * victim); //Kira
bool packrat_objects(CHAR_DATA * ch, int vnum); //Kira
void packrat_obj_add(CHAR_DATA * ch, int vnum); //Kira

void affect_enchant args((OBJ_DATA * obj));
int check_immune args((CHAR_DATA * ch, int dam_type));
int armor_lookup args((const char *name));
int weapon_lookup args((const char *name));
char *item_name args((int item_type));
int attack_lookup args((const char *name));
long wiznet_lookup args((const char *name));
int class_lookup args((const char *name));
bool is_old_mob args((CHAR_DATA * ch));
int get_skill args((CHAR_DATA * ch, int sn));
int get_weapon_sn args((CHAR_DATA * ch));
int get_dual_weapon_sn args((CHAR_DATA * ch));
int get_weapon_skill args((CHAR_DATA * ch, int sn));
int get_age args((CHAR_DATA * ch));
void reset_char args((CHAR_DATA * ch));
int get_curr_stat args((CHAR_DATA * ch, int stat));
int get_max_train args((CHAR_DATA * ch, int stat));
int get_hp_percent(CHAR_DATA * ch);
int get_mana_percent(CHAR_DATA * ch);
int get_hours_played(CHAR_DATA * ch);
int exp_til_level(CHAR_DATA * ch);
char *SEX(CHAR_DATA * ch);
char *get_pk_type(CHAR_DATA * ch);
char *get_position(CHAR_DATA * ch);
int can_carry_n args((CHAR_DATA * ch));
int can_carry_i args((CHAR_DATA *));
int can_carry_w args((CHAR_DATA * ch));
char *get_IC_time(CHAR_DATA * ch);
bool is_name args((char *str, char *namelist));
bool is_full_name args((const char *str, char *namelist));
bool is_exact_name args((char *str, char *namelist));
void affect_to_char args((CHAR_DATA * ch, AFFECT_DATA * paf));
void affect_to_obj args((OBJ_DATA * obj, AFFECT_DATA * paf));
void affect_remove args((CHAR_DATA * ch, AFFECT_DATA * paf));
void affect_remove_obj args((OBJ_DATA * obj, AFFECT_DATA * paf));
void affect_strip args((CHAR_DATA * ch, int sn));
bool is_affected args((CHAR_DATA * ch, int sn));
void affect_join args((CHAR_DATA * ch, AFFECT_DATA * paf));
void char_from_room args((CHAR_DATA * ch));
void char_to_room args((CHAR_DATA * ch, ROOM_INDEX_DATA * pRoomIndex));

/* TODO Change all instance of obj_to_char to new format -M */
#define obj_to_char(obj,ch) obj_to_char_new(obj, ch, NULL, NULL)
//void    obj_to_char    args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void obj_to_char_new args((OBJ_DATA * obj, CHAR_DATA * ch, BUFFER * output, BUFFER * roomoutput));
void obj_from_char args((OBJ_DATA * obj));
int apply_ac args((OBJ_DATA * obj, int iWear));
OD *get_eq_char args((CHAR_DATA * ch, int iWear));
void equip_char args((CHAR_DATA * ch, OBJ_DATA * obj, int iWear));
void unequip_char args((CHAR_DATA * ch, OBJ_DATA * obj));
int count_obj_list args((OBJ_INDEX_DATA * obj, OBJ_DATA * list));
void obj_from_room args((OBJ_DATA * obj));
void obj_to_room args((OBJ_DATA * obj, ROOM_INDEX_DATA * pRoomIndex));
void obj_to_obj args((OBJ_DATA * obj, OBJ_DATA * obj_to));
void obj_from_obj args((OBJ_DATA * obj));
void extract_obj args((OBJ_DATA * obj));
void extract_char args((CHAR_DATA * ch, bool fPull));
CD *get_char_room args((CHAR_DATA * ch, ROOM_INDEX_DATA * room, char *argument));
CD *get_char_world args((CHAR_DATA * ch, char *argument));
OD *get_obj_type args((OBJ_INDEX_DATA * pObjIndexData));
OD *get_obj_list args((CHAR_DATA * ch, char *argument, OBJ_DATA * list));
OD *get_obj_carry args((CHAR_DATA * ch, char *argument, CHAR_DATA * viewer));
OD *get_obj_carry_deep args((CHAR_DATA * ch, char *argument, CHAR_DATA * viewer));

OD *get_obj_wear args((CHAR_DATA * ch, char *argument, bool character));
OD *get_obj_here args((CHAR_DATA * ch, ROOM_INDEX_DATA * room, char *argument));
OD *get_obj_world args((CHAR_DATA * ch, char *argument));
OD *create_money args((int gold));
int get_obj_number args((OBJ_DATA * obj));
int get_obj_weight args((OBJ_DATA * obj));
int get_true_weight args((OBJ_DATA * obj));
bool room_is_dark args((CHAR_DATA * ch, ROOM_INDEX_DATA * pRoomIndex));
bool is_room_owner args((CHAR_DATA * ch, ROOM_INDEX_DATA * room));
bool room_is_private args((ROOM_INDEX_DATA * pRoomIndex));
bool can_see args((CHAR_DATA * ch, CHAR_DATA * victim));
bool can_see_obj args((CHAR_DATA * ch, OBJ_DATA * obj));
bool can_see_room args((CHAR_DATA * ch, ROOM_INDEX_DATA * pRoomIndex));
bool can_drop_obj args((CHAR_DATA * ch, OBJ_DATA * obj));
char *affect_loc_name args((int location));
char *affect_bit_name args((int vector));
char *affect2_bit_name args((int vector));
char *extra_bit_name args((int extra_flags));
char *extra2_bit_name args((int extra2_flags));
char *wear_bit_name args((int wear_flags));
char *act_bit_name args((int act_flags));
char *act2_bit_name args((int act2));
char *act3_bit_name args((int act3));
char *off_bit_name args((int off_flags));
char *imm_bit_name args((int imm_flags));
char *form_bit_name args((int form_flags));
char *part_bit_name args((int part_flags));
char *weapon_bit_name args((int weapon_flags));
char *object_bit_name args((int object_flags));
char *comm_bit_name args((char *comm_flags));
char *cont_bit_name args((int cont_flags));
bool remove_voodoo args((CHAR_DATA * ch));
//add_cost    args( (CHAR_DATA *ch, int cost, int value) );
void add_cost(CHAR_DATA * ch, int cost, int value);

int get_str_app_index(int pStr);
int get_int_app_index(int pInt);
int get_con_app_index(int pCon);
int get_wis_app_index(int pWis);
int get_dex_app_index(int pDex);

/* expedition.c */
void ch_printf(CHAR_DATA * ch, char *fmt, ...);

/*
 * Colour Config
 */
void default_colour args((CHAR_DATA * ch));
void all_colour args((CHAR_DATA * ch, char *argument));

/* interp.c */
void load_player_ranks();
void save_player_ranks();
void do_function(CHAR_DATA * ch, DO_FUN * do_fun, char *argument);
void interpret args((CHAR_DATA * ch, char *argument));
bool is_number args((char *arg));
int number_argument args((char *argument, char *arg));
int mult_argument args((char *argument, char *arg));
char *one_argument args((char *argument, char *arg_first));
bool is_granted_name args((CHAR_DATA * ch, char *argument));
bool is_granted args((CHAR_DATA * ch, DO_FUN * do_fun));

/* locker.c */
//Joe:Lockers  Declaration for useful functions
bool is_locker(OBJ_DATA * obj);
OBJ_DATA *get_obj_locker_room args((CHAR_DATA * ch, sh_int vnum));
OBJ_DATA *get_locker args((CHAR_DATA * ch));

/* drunk.c */
char *makedrunk args((char *string, CHAR_DATA * ch));
char *makegibberish args((char *string, CHAR_DATA * ch));

/* magic.c */
int find_spell args((CHAR_DATA * ch, const char *name));
int skill_lookup args((const char *name));
int slot_lookup args((int slot));
bool saves_spell args((int level, CHAR_DATA * victim, int dam_type));
void obj_cast_spell args((int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj));
bool check_dispel(int dis_level, CHAR_DATA * victim, int sn);

/* mob_prog.c */
bool has_item_deep(CHAR_DATA * ch, char *argument, sh_int item_type, bool fWear);
bool in_party(CHAR_DATA * ch, char *argument);
bool is_group_leader(CHAR_DATA * ch);
void program_flow args((sh_int vnum, char *source, CHAR_DATA * mob,
   OBJ_DATA * obj, ROOM_INDEX_DATA * room, CHAR_DATA * ch, const void *arg1, const void *arg2));
void p_act_trigger args((char *argument, CHAR_DATA * mob,
   OBJ_DATA * obj, ROOM_INDEX_DATA * room, CHAR_DATA * ch, const void *arg1, const void *arg2, int type));
bool p_percent_trigger args((CHAR_DATA * mob, OBJ_DATA * obj, ROOM_INDEX_DATA * room, CHAR_DATA * ch, const void *arg1,
   const void *arg2, int type));
void p_bribe_trigger args((CHAR_DATA * mob, CHAR_DATA * ch, int amount));
void p_obj_levelup_trigger(OBJ_DATA * obj, CHAR_DATA * ch, int type);
void p_levelup_trigger(CHAR_DATA * mob, CHAR_DATA * ch, int type);
bool p_exit_trigger args((CHAR_DATA * ch, int dir, int type));
void p_give_trigger args((CHAR_DATA * mob, OBJ_DATA * obj, ROOM_INDEX_DATA * room, CHAR_DATA * ch, OBJ_DATA * dropped,
   int type));
void p_greet_trigger args((CHAR_DATA * ch, int type));
void p_hprct_trigger args((CHAR_DATA * mob, CHAR_DATA * ch));
void p_wear_trigger(OBJ_DATA * obj, CHAR_DATA * ch, int type);
void p_remove_trigger(OBJ_DATA * obj, CHAR_DATA * ch, int type);
bool p_command_trigger args((CHAR_DATA * mob, CHAR_DATA * ch, char *command, char *argument));
bool p_obj_command_trigger args((OBJ_DATA * obj, CHAR_DATA * ch, char *command, char *argument));

bool p_percent_trigger_GRALL args((CHAR_DATA * mob, OBJ_DATA * obj, ROOM_INDEX_DATA * room, CHAR_DATA * ch,
   const void *arg1, const void *arg2, int type));

/* mob_cmds.c */
void mob_interpret args((CHAR_DATA * ch, char *argument));
void obj_interpret args((OBJ_DATA * obj, char *argument));
void room_interpret args((ROOM_INDEX_DATA * room, char *argument));

/* quest.c */
bool chance args((int num));

/* save.c */
void save_acct_obj args((ACCT_DATA * ch));
void save_char_obj args((CHAR_DATA * ch));
bool load_acct_obj args((DESCRIPTOR_DATA * d, char *name));
CHAR_DATA *load_char_obj args((DESCRIPTOR_DATA * d, char *name));
bool check_char_exist args((char *name));

/* skills.c */
char *class_list_short(CHAR_DATA *ch);
char *class_list(CHAR_DATA * ch);
char *class_tier0(CHAR_DATA * ch);
char *class_tier1(CHAR_DATA * ch);
char *class_list(CHAR_DATA * ch);
void remove_class(CHAR_DATA * ch, sh_int class);
bool ready_tier(CHAR_DATA * ch);
bool has_class(CHAR_DATA * ch, sh_int class);
void add_class(CHAR_DATA * ch, sh_int class);
sh_int get_skill_level(CHAR_DATA * ch, sh_int sn);
sh_int get_skill_rating(CHAR_DATA * ch, sh_int sn);
void do_removeclass(CHAR_DATA * ch, char *argument);

bool parse_gen_groups args((CHAR_DATA * ch, char *argument));
int exp_per_level args((CHAR_DATA * ch));
int exp_for_level args((CHAR_DATA * ch));
void check_improve args((CHAR_DATA * ch, int sn, bool success, int multiplier));

/* special.c */
SF *spec_lookup args((const char *name));
char *spec_name args((SPEC_FUN * function));

/* teleport.c */
RID *room_by_name args((char *target, int level, bool error));

//
//  todo.c - Hitch
//
void nullifyBuffer(char *input, int size);

/* update.c */
void guess_update(void);
void weretiger_invasion(void);
void health_update(void);
void faux_rewards (CHAR_DATA *ch);
void advanceQuestItems(CHAR_DATA * ch, OBJ_DATA * list);
void update_bonuses();
void ghost_update(void);
void advanceQuestItems(CHAR_DATA * ch, OBJ_DATA * list);
void track_update(void);
void weather_update(void);
void syncSkills args((CHAR_DATA * ch));
void advance_level args((CHAR_DATA * ch, bool hide, bool mesg));
void gain_exp args((CHAR_DATA * ch, int gain));
void gain_condition args((CHAR_DATA * ch, int iCond, int value));
void update_handler args((void));

/* vote.c */
void load_issues(void);
bool can_vote(CHAR_DATA * ch);

/* string.c */
void string_edit args((CHAR_DATA * ch, char **pString));
void string_append args((CHAR_DATA * ch, char **pString));
char *string_replace args((char *orig, char *old, char *new));
void string_add args((CHAR_DATA * ch, char *argument));
char *format_string args((char *oldstring /*, bool fSpace */ ));
char *first_arg args((char *argument, char *arg_first, bool fCase));
char *string_unpad args((char *argument));
char *string_proper args((char *argument));
char *end_string args((const char *txt, int length));
char *begin_string args((const char *txt, int length));

/* olc.c */
bool run_olc_editor args((DESCRIPTOR_DATA * d));
char *olc_ed_name args((CHAR_DATA * ch));
char *olc_ed_vnum args((CHAR_DATA * ch));

/* olc_act.c */
void show_armorlist(CHAR_DATA * ch);
void set_weapon_dice(OBJ_INDEX_DATA * obj);

/* olc_save.c */
void save_area(AREA_DATA * pArea);
void save_area_list();

/* lookup.c */
int race_lookup args((const char *name));
int item_lookup args((const char *name));
int liq_lookup args((const char *name));
int material_lookup args((const char *name));
CONTENTS_DATA *find_contents args((char *argument));

// todo.c - Hitch
void loadTodos();

/* recycle.c buffer procedures */
int newmobs;
int newobjs;

char *string_space;
char *top_string;
char str_empty[1];
char guess[MSL];
char guess_spaces[MSL];

BUFFER *new_buf args((void));
void free_buf args((BUFFER * buffer));
bool add_buf args((BUFFER * buffer, char *string));
void clear_buf args((BUFFER * buffer));
char *buf_string args((BUFFER * buffer));

/* Rendelven - Clan System, clan.c */
bool can_claim_area(CHAR_DATA *ch, bool mesg);
bool area_claimed(AREA_DATA *pArea);
void reset_control_state(void *data, char *argument, int8_t event_type);
void add_to_roster(CHAR_DATA * ch, char *clanname, sh_int rank);
bool can_demote(CHAR_DATA * ch);
bool can_promote(CHAR_DATA * ch);
bool can_induct(CHAR_DATA * ch);
bool can_outcast(CHAR_DATA * ch);
ROSTER_DATA *insert_roster(ROSTER_DATA * newr, ROSTER_DATA * list);
void save_clans args((void));
void load_clans args((void));
void update_roster args((char *name, CHAR_DATA * ch));
void update_roster_time args((CHAR_DATA * ch));
int clan_lookup args((const char *name));
char *player_clan args((CHAR_DATA * ch));
char *player_rank args((CHAR_DATA * ch));
bool is_clan args((CHAR_DATA * ch));
bool is_same_clan args((CHAR_DATA * ch, CHAR_DATA * victim));
bool is_clead args((CHAR_DATA * ch));

/* mapper.c */
char *strhit(char *str, const char *delimiters);

/* God related shit */
int devout_lookup args((const char *name));

#undef    CD
#undef    MID
#undef    OD
#undef    OID
#undef    RID
#undef    SF
#undef    AD

/*****************************************************************************
 *                                    OLC                                    *
 *****************************************************************************/

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
#define OBJ_VNUM_DUMMY    30

/*
 * Area flags.
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1   /* Area has been modified. */
#define         AREA_ADDED      2   /* Area has been added to */
#define         AREA_LOADING    4   /* Used for counting in db.c */
#define		AREA_NOQUEST    8   /* So players can't quest in certain zones */
#define		AREA_NOAREA	16      /* So area doesn't show up on the area list */
#define         AREA_HUNT       32  /* Anyone Can hunt in these areas */

#define MAX_DIR    6
#define NO_FLAG  -99            /* Must not be used in flags or stats. */

/*
 * Global Constants
 */
extern char *const dir_name[];
extern const sh_int rev_dir[];  /* sh_int - ROM OLC */
extern const struct spec_type spec_table[];

/*
 * Global variables
 */
extern AREA_DATA *area_first;
extern AREA_DATA *area_last;
extern SHOP_DATA *shop_last;

extern bool global_nogate;      /* Switch to turn off gate&summ during quests */
extern bool global_nomagic;     /* Nomagic stuff - Kira */
extern bool global_norecall;    /* Can't recall or use word of recall in this quest */
extern bool global_nolocate;    /* Can't use locate spells in this quest */
extern int u_level;
extern int l_level;

extern int top_affect;
extern int top_area;
extern int top_ed;
extern int top_exit;
extern int top_help;
extern int top_mob_index;
extern int top_obj_index;
extern int top_reset;
extern int top_room;
extern int top_shop;
extern int mobskilled;
extern int top_vnum_mob;
extern int top_vnum_obj;
extern int top_vnum_room;

// extern        char  guess[MSL]; //Guess the phrase
// extern        char  guess_spaces[MSL]; //The --- that players see
extern char str_empty[1];

extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];

/*
 * Autosave Flags - added by MachinShin
 */
#define AUTOSAVE_LOGGED		1
#define AUTOSAVE_INTERVAL	600  /* 10 mins */

/*
 * Freeze Tag States - added by MachinShin
 */
#define FTAG_STATE_NONE		0
#define FTAG_STATE_JOIN		1
#define FTAG_STATE_PREP		2
#define FTAG_STATE_PLAYING	3
#define FTAG_JOIN_TIME		80
#define FTAG_PREP_TIME		30

extern int ftag_state;
extern int ftag_timer;
void ftag_update();

/* memmgmt.c */

void room_record(char *name, ROOM_INDEX_DATA * room, int door);

/* externs */
extern bool MemDebug;
extern char str_empty[1];
extern int mobile_count;

/* stuff for providing a crash-proof buffer */

#define MAX_BUF		16384
#define MAX_BUF_LIST 	10
#define BASE_BUF 	1024

/* valid states */
#define BUFFER_SAFE	0
#define BUFFER_OVERFLOW	1
#define BUFFER_FREED 	2

/* ban data recycling */
#define BD BAN_DATA
BD *new_ban args((void));
void free_ban args((BAN_DATA * ban));
#undef BD

/* roster data recycling */
ROSTER_DATA *new_roster args((void));
void free_roster args((ROSTER_DATA * roster));

/* descriptor recycling */
#define DD DESCRIPTOR_DATA
DD *new_descriptor args((void));
void free_descriptor args((DESCRIPTOR_DATA * d));
#undef DD

/* extra descr recycling */
#define ED EXTRA_DESCR_DATA
ED *new_extra_descr args((void));
void free_extra_descr args((EXTRA_DESCR_DATA * ed));
#undef ED

#define FD FAMILY_DATA
FD *new_family args((void));
void free_family args((FAMILY_DATA * fData));
#undef FD

ACCT_DATA *new_acct args((void));
void free_acct args((ACCT_DATA * acct));

/* affect recycling */
#define AD AFFECT_DATA
AD *new_affect args((void));
void free_affect args((AFFECT_DATA * af));
#undef AD

/* object recycling */
#define OD OBJ_DATA
OD *new_obj args((void));
void free_obj args((OBJ_DATA * obj));
#undef OD

/* character recyling */
#define CD CHAR_DATA
#define PD PC_DATA
CD *new_char args((void));
void free_char args((CHAR_DATA * ch));
PD *new_pcdata args((void));
void free_pcdata args((PC_DATA * pcdata));
#undef PD
#undef CD

RUNE_SLOT_DATA *new_rune_slot(void);
void free_rune_slot(RUNE_SLOT_DATA *);

/* mob id and memory procedures */
#define MD MEM_DATA
long get_pc_id args((void));
long get_mob_id args((void));
MD *new_mem_data args((void));
void free_mem_data args((MEM_DATA * memory));
MD *find_memory args((MEM_DATA * memory, long id));
#undef MD

HELP_AREA *new_had args((void));
HELP_DATA *new_help args((void));
void free_help args((HELP_DATA *));

/* Rendelven */
CONTENTS_DATA *new_contents args((void));
void free_contents args((CONTENTS_DATA *));
